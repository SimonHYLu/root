// @(#)root/minuit2:$Name:  $:$Id: TChi2FitData.cxx,v 1.2 2005/10/27 17:06:29 moneta Exp $
// Author: L. Moneta    10/2005  

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2005 ROOT Foundation,  CERN/PH-SFT                   *
 *                                                                    *
 **********************************************************************/

#include <cassert>

#include "RConfig.h"
#include "TChi2FitData.h"

#include "TVirtualFitter.h" 

#include <iostream>

#include "TH1.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TMultiGraph.h"

// constructor - create fit data from Histogram content



TChi2FitData::TChi2FitData( const TVirtualFitter & fitter, bool skipEmptyBins) : 
  fSize(0), fSkipEmptyBins(skipEmptyBins), fIntegral(false)
{

  TObject * obj = fitter.GetObjectFit(); 
  
  // downcast to see type of object
  TH1 * hfit = dynamic_cast<TH1*> ( obj );
  if (hfit) { 
    GetFitData(hfit, &fitter);    
    return; 
  } 
  // case of TGraph
  TGraph * graph = dynamic_cast<TGraph*> ( obj );
  if (graph) { 
    GetFitData(graph, &fitter);    
    return; 
  } 
  // case of TGraph2D
  TGraph2D * graph2D = dynamic_cast<TGraph2D*> ( obj );
  if (graph2D) { 
    GetFitData(graph2D, &fitter);    
    return; 
  } 
  // case of TMultiGraph
  TMultiGraph * multigraph = dynamic_cast<TMultiGraph*> ( obj );
  if (multigraph) { 
    GetFitData(graph2D, &fitter);    
    return; 
  } 
  // else 
  std::cout << "other fit type are not yet supported- assert" << std::endl;
  assert(hfit != 0); 

}

// get Histogram Data
void TChi2FitData::GetFitData(const TH1 * hfit, const TVirtualFitter * hFitter) {

  assert(hfit != 0); 
  assert(hFitter != 0);
  //std::cout << "creating Fit Data from histogram " << hfit->GetName() << std::endl; 

//  use TVirtual fitter to get fit range (should have a FitRange class ) 

  // first and last bin
   int hxfirst = hFitter->GetXfirst(); 
   int hxlast  = hFitter->GetXlast(); 
   int hyfirst = hFitter->GetYfirst(); 
   int hylast  = hFitter->GetYlast(); 
   int hzfirst = hFitter->GetZfirst(); 
   int hzlast  = hFitter->GetZlast(); 
   TAxis *xaxis  = hfit->GetXaxis();
   TAxis *yaxis  = hfit->GetYaxis();
   TAxis *zaxis  = hfit->GetZaxis();

   // get fit option 
   Foption_t fitOption = hFitter->GetFitOption();
   if (fitOption.Integral) fIntegral=true;

   int n = (hxlast-hxfirst+1)*(hylast-hyfirst+1)*(hzlast-hzfirst+1); 
   //std::cout << "total bins  " << hxlast-hxfirst+1 << std::endl; 

   fInvErrors.reserve(n);
   fValues.reserve(n);
   fCoordinates.reserve(n);

   int ndim =  hfit->GetDimension();
   assert( ndim > 0 );
   CoordData x = CoordData( hfit->GetDimension() );
   int binx = 0; 
   int biny = 0; 
   int binz = 0; 

   for ( binx = hxfirst; binx <= hxlast; ++binx) {
     if (fIntegral) {
       x[0] = xaxis->GetBinLowEdge(binx);       
     }
     else
       x[0] = xaxis->GetBinCenter(binx);

     if ( ndim > 1 ) { 
       for ( biny = hyfirst; biny <= hylast; ++biny) {
	 if (fIntegral) 
	   x[1] = yaxis->GetBinLowEdge(biny);
	 else
	   x[1] = yaxis->GetBinCenter(biny);
       
	 if ( ndim >  2 ) { 
	   for ( binz = hzfirst; binz <= hzlast; ++binz) {
	     if (fIntegral) 
	       x[2] = zaxis->GetBinLowEdge(binz);
	     else
	       x[2] = zaxis->GetBinCenter(binz);
	     //if (!func->IsInside(x) ) continue;
	     double error =  hfit->GetBinError(binx, biny, binz); 
	     if (fitOption.W1) error = 1;
	     SetDataPoint( x,  hfit->GetBinContent(binx, biny, binz), error );
	   }  // end loop on z bins
	 }
	 else if (ndim == 2) { 
	   // for dim == 2
	   //if (!func->IsInside(x) ) continue;
	     double error =  hfit->GetBinError(binx, biny); 
	     if (fitOption.W1) error = 1;
	     SetDataPoint( x,  hfit->GetBinContent(binx, biny), error );
	 }   
  
       }  // end loop on y bins

     }
     else if (ndim == 1) { 
       // for 1D 
       //if (!func->IsInside(x) ) continue;
       double error =  hfit->GetBinError(binx); 
       if (fitOption.W1) error = 1;
       SetDataPoint( x,  hfit->GetBinContent(binx), error );
     }

   }   // end 1D loop 

   // in case of integral store additional point with upper x values
   if (fIntegral) { 
     x[0] = xaxis->GetBinLowEdge(hxlast) +  xaxis->GetBinWidth(hxlast); 
     if (ndim > 1) { 
       x[1] = yaxis->GetBinLowEdge(hylast) +  yaxis->GetBinWidth(hylast); 
     }
     if (ndim > 2) { 
       x[2] = zaxis->GetBinLowEdge(hzlast) +  zaxis->GetBinWidth(hzlast); 
     }
     fCoordinates.push_back(x);
   }

}


void TChi2FitData::GetFitData(const TGraph * gr, const TVirtualFitter * hFitter ) {

  // fit options
   Foption_t fitOption = hFitter->GetFitOption();

   int  nPoints = gr->GetN();
   double *gx = gr->GetX();
   double *gy = gr->GetY();

   CoordData x = CoordData( 1 );  // 1D graph

   for ( int i = 0; i < nPoints; ++i) { 

     x[0] = gx[i];
     // neglect error in x (it is a different chi2) 
     double errorY = gr->GetErrorY(i); 
     // consider error = 0 as 1 
     if (errorY <= 0) errorY = 1;
     if (fitOption.W1) errorY = 1;
     SetDataPoint( x, gy[i], errorY );

   }
}

// fetch graph 2D data for CHI2 fit. 
// neglect errors in x and y (one use the ExtendedChi2 method)
void TChi2FitData::GetFitData(const TGraph2D * gr, const TVirtualFitter * hFitter ) {

  // fit options
   Foption_t fitOption = hFitter->GetFitOption();

   int  nPoints = gr->GetN();
   double *gx = gr->GetX();
   double *gy = gr->GetY();
   double *gz = gr->GetZ();

   CoordData x = CoordData( 2 );  // 2D graph

   for ( int i = 0; i < nPoints; ++i) { 

     x[0] = gx[i];
     x[1] = gy[i];
     // neglect error in x (it is a different chi2) 
     double error = gr->GetErrorZ(i); 
     // consider error = 0 as 1 
     if (error <= 0) error = 1;
     if (fitOption.W1) error = 1;
     SetDataPoint( x, gz[i], error );

   }
}

// multigraph

void TChi2FitData::GetFitData(const TMultiGraph * mg, const TVirtualFitter * hFitter ) {

  // fit options
   Foption_t fitOption = hFitter->GetFitOption();

   TGraph *gr;
   TIter next(mg->GetListOfGraphs());   
 
   int  nPoints;
   double *gx;
   double *gy;

   CoordData x = CoordData( 1 );  // 1D graph

   while ((gr = (TGraph*) next())) {
     nPoints = gr->GetN();
     gx      = gr->GetX();
     gy      = gr->GetY();
     for ( int i = 0; i < nPoints; ++i) { 

       x[0] = gx[i];
       // neglect error in x (it is a different chi2) 
       double errorY = gr->GetErrorY(i); 
       // consider error = 0 as 1 
       if (errorY <= 0) errorY = 1;
       if (fitOption.W1) errorY = 1;
       SetDataPoint( x, gy[i], errorY );
       
     }
   }

}

void TChi2FitData::SetDataPoint( const CoordData & x, double y, double error) { 
  
  if (error <= 0) {  
    if (SkipEmptyBins() ) 
      return;
    else   
      // set errors to 1 
      error = 1;
  }

  fCoordinates.push_back(x);
  fValues.push_back(y);
  fInvErrors.push_back(1./error);
  fSize++;
}
