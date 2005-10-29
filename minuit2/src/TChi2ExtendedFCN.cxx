// @(#)root/minuit2:$Name:  $:$Id: TChi2ExtendedFCN.cxx,v 1.2 2005/10/27 17:06:29 moneta Exp $
// Author: L. Moneta    10/2005  

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2005 ROOT Foundation,  CERN/PH-SFT                   *
 *                                                                    *
 **********************************************************************/

#include "TChi2ExtendedFCN.h"
#include "TChi2ExtendedFitData.h"

#include "TF1.h"
#include "TVirtualFitter.h"
#include <cassert>

//#include <iostream>

// constructor _ create FitData class

TChi2ExtendedFCN::TChi2ExtendedFCN( const TVirtualFitter & fitter) : 
  fUp(1.)
{ 
  fFunc = dynamic_cast<TF1 *> ( fitter.GetUserFunc() );
  assert(fFunc);
  // default skip empty bins
  fData = new TChi2ExtendedFitData(fitter); 

  // need to set the size so ROOT can calculate ndf.
  fFunc->SetNumberFitPoints(fData->Size());
}

//  this class manages the fit data class. Delete it at the end

TChi2ExtendedFCN::~TChi2ExtendedFCN() {  
  if (fData) { 
    delete fData; 
  }
}


  // implement chi2 function 
double TChi2ExtendedFCN::operator()(const std::vector<double>& par) const {

  assert(fData); 
  assert(fFunc); 


  //  std::cout << "number of params " << par.size() << " in TF1 " << fFunc->GetNpar() << "  " << fFunc->GetNumberFreeParameters() << std::endl;
  
  unsigned int n = fData->Size();
  double chi2 = 0;
  for (unsigned int i = 0; i < n; ++ i) { 
    const std::vector<double> & x = fData->Coords(i); 
    fFunc->InitArgs( &x.front(), &par.front() ); 
    double y = fData->Value(i);
    //std::cout << x[0] << "  " << y << "  " << 1./invError << " params " << par[0] << std::endl;
    double functionValue = fFunc->EvalPar( &x.front(), &par.front() ); 

    double ey =  fData->ErrorY(i);
    double exl = fData->ErrorXLow(i);
    double exh = fData->ErrorXUp(i);
    double eux = 0; 
    if (exh <=0 && exl <= 0) 
      eux = 0;
    else 
      // works only for 1D func
      eux = 0.5 * (exl + exh) * fFunc->Derivative( x[0],  const_cast<double *> ( &par.front() ) );
    double error2 = (ey*ey + eux*eux);
    if (error2 == 0) error2 = 1;

    double tmp = ( y -functionValue );
	  	  
    chi2 += tmp*tmp/error2;
  }

  return chi2;
}

