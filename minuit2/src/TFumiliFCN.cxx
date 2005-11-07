// @(#)root/minuit2:$Name:  $:$Id: TFumiliFCN.cxx,v 1.4 2005/11/05 15:17:35 moneta Exp $
// Author: L. Moneta    10/2005  

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2005 ROOT Foundation,  CERN/PH-SFT                   *
 *                                                                    *
 **********************************************************************/

#include "TFumiliFCN.h"
#include "TChi2FCN.h"
#include "TBinLikelihoodFCN.h"
#include "TChi2FitData.h"
#include "FitterUtil.h"

#include "TF1.h"
#include "TVirtualFitter.h"

//#include <iostream>

#include <cmath>

static const double kPrecision = 1.E-16; 
static const double kEpsilon = 1.E-300; 


// constructor _ create FitData class

TFumiliFCN::TFumiliFCN( const TVirtualFitter & fitter, double up, int strategy, bool skipEmptyBins) : 
  // need to use default constructor for FumiliFCNBase (don't know number of parameters at this stage
  FumiliFCNBase()
{
  fUp = up;
  fFunc = dynamic_cast<TF1 *> ( fitter.GetUserFunc() );
  assert(fFunc != 0);
  // default skip empty bins
  fData = new TChi2FitData(fitter, skipEmptyBins); 
  //std::cout << "Created FitData with size = " << fData->Size() << std::endl;

  // need to set the size so ROOT can calculate ndf.
  fFunc->SetNumberFitPoints(fData->Size());

  fStrategy = strategy; 

  //std::cout << "created FumiliFCN  with dimension " << dimension() << std::endl;
}

//  this class manages the fit data class. Delete it at the end

TFumiliFCN::~TFumiliFCN() {  
  if (fData) { 
    //std::cout << "deleting the data - size is " << fData->Size() << std::endl; 
    delete fData; 
  }
}



// need an initialize method with number of fit parameters to make space for cached 
// quantities (gradient , etc..)

void TFumiliFCN::Initialize(unsigned int nPar) { 
  fParamCache = std::vector<double>(nPar);
  fFunctionGradient = std::vector<double>( nPar );
  // call init function on FumiliFCN
  initAndReset(nPar);
}


void  TFumiliFCN::evaluateAll( const std::vector<double> & p) { 
  
  Calculate_gradient_and_hessian(p);
}


void TFumiliFCN::Calculate_gradient_and_hessian(const std::vector<double> & p)  {



  unsigned int npar = p.size(); 
  if (npar != dimension() ) { 
    // re-initialize the store gradient
    //std::cout << "initialize FumiliFCN and cache" << std::endl;
    Initialize(npar);
  }

  const FumiliFitData & points = *fData; 
  
  
  // set parameters
  fFunc->SetParameters( &p.front() );
  fParamCache = p;
 
  std::vector<double> & grad = gradient(); 
  std::vector<double> & hess = hessian(); 

  // dimension of hessian symmetric matrix  
  unsigned int nhdim = static_cast<int>( 0.5*npar*(npar + 1) ); 
  assert( npar == fFunctionGradient.size() ); 
  assert( npar == grad.size() ); 
  assert( nhdim == hess.size() );
  // reset  
  grad.assign( npar, 0.0); 
  hess.assign( nhdim, 0.0);

  double sum = 0; 

  
  // loop on measurements
  unsigned int nMeasurements = points.Size(); 
  for (unsigned int i = 0; i < nMeasurements; ++i) {
    
    const std::vector<double> & x =  points.Coords(i); 
    fFunc->InitArgs( &x.front(), &fParamCache.front() ); 

    // one should implement integral option (in TFumili is not correct)
    double fval; 
    if ( fData->UseIntegral()) {
      const std::vector<double> & x2 = fData->Coords(i+1); 
      // need to implement derivatives of integral
      fval = FitterUtil::EvalIntegral(fFunc,x,x2,fParamCache);     
      Calculate_numerical_gradient_of_integral( x, x2, fval); 	
    }
    else { 
      fval = fFunc->EvalPar(&x.front(), &fParamCache.front() ); 
      Calculate_numerical_gradient( x, fval); 
    }
    
    // calculate gradient 
    // eventually use function if provides it 

    Calculate_element(i, points, fval, sum, grad, hess); 

    // calculate i -element contribution to the chi2
    // add contributions to previous one 

  }

  // set value of Obj function to be used by Minuit
  setFCNValue(sum); 
	
}


void TFumiliFCN::Calculate_numerical_gradient( const std::vector<double> & x, double f0) {   


  // use a cache for parameters to avoid to copy parameters each time this function is called

  int n = fParamCache.size();
  //std::cout << "Model function Gradient " << std::endl;
  for (int ipar = 0; ipar < n ; ++ipar) { 
    double p0 = fParamCache[ipar]; 
    // use 0.001 of par 
    double h = std::max( 0.001* fabs(p0), 8.0*kPrecision*(fabs(p0) + kPrecision) );  
    fParamCache[ipar] = p0 + h;  
    double f2 =  fFunc->EvalPar( &x.front(), &fParamCache.front() ); 


    if (fStrategy == 2) { 
      //  USE 5 POINT_RULE
      fParamCache[ipar] = p0 - h;  
      double f1 =  fFunc->EvalPar( &x.front(), &fParamCache.front() ); 
      fParamCache[ipar] = p0 + h/2; 
      double g1 =  fFunc->EvalPar( &x.front(), &fParamCache.front() ); 
      fParamCache[ipar] = p0 - h/2; 
      double g2 =  fFunc->EvalPar( &x.front(), &fParamCache.front() ); 
      
      double h2    = 1/(2.*h);
      double d0    = f1 - f2;
      double d2    = 2*(g1 - g2);
      
      //fFunctionGradient[ipar] =  0.5*( f2 - f1)/h;
      fFunctionGradient[ipar] =  h2*(4*d2 - d0)/3.;

    }
    else { 
      //default 2  point rule
      fFunctionGradient[ipar] =  (f2 - f0)/h;
    }

    // reset to old value
    fParamCache[ipar] = p0; 
    //std::cout << " i " << ipar << par[ipar] << "  " << fFunctionGradient[ipar] << " xi = " << x[0] << " fval " << f0 << std::endl; 
  }

}


void TFumiliFCN::Calculate_numerical_gradient_of_integral( const std::vector<double> & x1, const std::vector<double> & x2, double f0) {   


  // use a cache for parameters to avoid to copy parameters each time this function is called

  int n = fParamCache.size();
  //std::cout << "Model function Gradient " << std::endl;
  for (int ipar = 0; ipar < n ; ++ipar) { 
    double p0 = fParamCache[ipar]; 
    // use 0.001 of par 
    double h = std::max( 0.001* fabs(p0), 8.0*kPrecision*(fabs(p0) + kPrecision) );  
    fParamCache[ipar] = p0 + h;  
    double f2 =   FitterUtil::EvalIntegral(fFunc,x1,x2,fParamCache);


    if (fStrategy == 2) { 
      //  USE 5 POINT_RULE
      fParamCache[ipar] = p0 - h;  
      double f1 =  FitterUtil::EvalIntegral(fFunc,x1,x2,fParamCache);
      fParamCache[ipar] = p0 + h/2; 
      double g1 =  FitterUtil::EvalIntegral(fFunc,x1,x2,fParamCache);
      fParamCache[ipar] = p0 - h/2; 
      double g2 =  FitterUtil::EvalIntegral(fFunc,x1,x2,fParamCache); 
      
      double h2    = 1/(2.*h);
      double d0    = f1 - f2;
      double d2    = 2*(g1 - g2);
      
      //fFunctionGradient[ipar] =  0.5*( f2 - f1)/h;
      fFunctionGradient[ipar] =  h2*(4*d2 - d0)/3.;

    }
    else { 
      //default 2  point rule
      fFunctionGradient[ipar] =  (f2 - f0)/h;
    }

    // reset to old value
    fParamCache[ipar] = p0; 
    //std::cout << " i " << ipar << par[ipar] << "  " << fFunctionGradient[ipar] << " xi = " << x[0] << " fval " << f0 << std::endl; 
  }

}



void TFumiliChi2FCN::Calculate_element(int i, const FumiliFitData & points, double fval, double & chi2, std::vector<double> & grad,   std::vector<double> & hess ) {

       
   double invError =  points.InvError(i); 
   double value = points.Value(i);
   double element = invError*( fval - value );
   unsigned int npar = grad.size();

   chi2 += element*element;
   
   for (unsigned int j = 0; j < npar; ++j) { 
     
     double fj =  invError * fFunctionGradient[j]; 
     grad[j] += 2.0 * element * fj; 
     
     //std::cout << " ---------j " << " j  " <<  fFunctionGradient[j] << "  " << grad[j] << std::endl; 
     
     for (unsigned int k = j; k < npar; ++ k) { 
       int idx =  j + k*(k+1)/2; 
       hess[idx] += 2.0 * fj * invError * fFunctionGradient[k]; 
     }
   }
   //      std::cout << "element " << i << " x " << x[0] << " val = " << value << " sig = " << 1/invError << " f(x) " << fval << " element " << element << " gradients:  " << grad[0] << "  " << grad[1] << "  " << grad[2] << std::endl; 

}


void TFumiliBinLikelihoodFCN::Calculate_element(int i, const FumiliFitData & points, double fval, double & logLike, std::vector<double> & grad,   std::vector<double> & hess ) {

  unsigned int npar = grad.size();

  // kEpsilon is smalles number ( 10-300) 
  double logtmp, invFval; 
  if(fval<=kEpsilon) { 
    logtmp = fval/kEpsilon + std::log(kEpsilon) - 1; 
    invFval = 1.0/kEpsilon; 
  } else {        
    logtmp = std::log(fval);
    invFval = 1.0/fval;
  }
  
  double value =  points.Value(i); 
  logLike +=  fval - value*logtmp;
  
 
  for (unsigned int j = 0; j < npar; ++j) {
    
    double fj; 
    if ( fval < kPrecision &&  fabs(fFunctionGradient[j]) < kPrecision ) 
      fj = 1.0; 
    else 
      fj =  fFunctionGradient[j] * ( 1.0 - value*invFval); 
    
    
    // 	    if ( ( ! (fj <= 0) )  && ( ! ( fj > 0) ) ) { 
    // 	      std::cout << "fj is nan -- " << fj << "  " << j << " x " << x[0] << " f(x) = " << fval << "  inv =  " << invFval << "gradient = " 
    // 			<< fFunctionGradient[j] << "  " << fFunctionGradient[j]/fval << std::endl;
    // 	      fj = 0; 
    
    // 	    }
    
    grad[j] += fj;
    
    for (unsigned int k = j; k < npar; ++ k) { 
      int idx =  j + k*(k+1)/2; 
      double fk; 
      if ( fval < kPrecision &&  fabs(fFunctionGradient[k]) < kPrecision ) 
	fk = 1.0; 
      else 
	fk =  fFunctionGradient[k]* ( 1.0 - value*invFval); 
      
      
      hess[idx] += fj * fk;
    }
  }
  
}


void TFumiliUnbinLikelihoodFCN::Calculate_element(int , const FumiliFitData &, double fval, double & logLike, std::vector<double> & grad,   std::vector<double> & hess ) {

  unsigned int npar = grad.size();
  // likelihood
  //if (fval < 1.0E-16) fval = 1.0E-16; // truncate for precision
  
  // kEpsilon is smalles number ( 10-300) 
  double logtmp, invFval; 
  if(fval<=kEpsilon) { 
    logtmp = fval/kEpsilon + std::log(kEpsilon) - 1; 
    invFval = 1.0/kEpsilon; 
  } else {        
    logtmp = std::log(fval);
    invFval = 1.0/fval;
  }
  
  logLike += logtmp;
  for (unsigned int j = 0; j < npar; ++j) {
    
    double fj; 
    if ( fval < kPrecision &&  fabs(fFunctionGradient[j]) < kPrecision ) 
      fj = 1.0; 
    else 
      fj =  invFval * fFunctionGradient[j]; 
    	
    grad[j] -= fj;
    
    for (unsigned int k = j; k < npar; ++ k) { 
      int idx =  j + k*(k+1)/2; 
      double fk; 
      if ( fval < kPrecision &&  fabs(fFunctionGradient[k]) < kPrecision ) 
	fk = 1.0; 
      else 
	fk =  invFval * fFunctionGradient[k]; 
      
      
      hess[idx] += fj * fk;
    }
  }
}


//   // implement chi2 function (should not be needed) 
double TFumiliChi2FCN::operator()(const std::vector<double>& par) const {

  assert(fData != 0); 
  assert(fFunc != 0); 

  TChi2FCN  fcn(fData,fFunc); 
  return fcn(par); 
}


double TFumiliBinLikelihoodFCN::operator()(const std::vector<double>& par) const {

  assert(fData != 0); 
  assert(fFunc != 0); 

  TBinLikelihoodFCN  fcn(fData,fFunc); 
  return fcn(par); 
}


double TFumiliUnbinLikelihoodFCN::operator()(const std::vector<double>& /*par */) const {

  assert(fData != 0); 
  assert(fFunc != 0); 

  //TUnbinLikelihoodFCN  fcn(*fData,*fFunc); 
  //return fcn(par);
  // to be implemented
  return 0; 
}
