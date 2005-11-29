// @(#)root/minuit2:$Name:  $:$Id: TFitterFumili.cxx,v 1.2 2005/10/27 17:06:29 moneta Exp $
// Author: L. Moneta    10/2005  

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2005 ROOT Foundation,  CERN/PH-SFT                   *
 *                                                                    *
 **********************************************************************/

#include "TROOT.h"
#include "TFitterFumili.h"
#include "TMath.h"
#include "TF1.h"
#include "TH1.h"
#include "TGraph.h"

#include "TFumiliFCN.h"
#include "Minuit2/FumiliMinimizer.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnStrategy.h"
#include "Minuit2/MnPrint.h"

using namespace ROOT::Minuit2;

//#define DEBUG 1

ClassImp(TFitterFumili);

TFitterFumili* gFumili2 = 0;

TFitterFumili::TFitterFumili() {
    SetName("Fumili2");
    gFumili2 = this;
    gROOT->GetListOfSpecials()->Add(gFumili2);
}


// needed this additional contructor ? 
TFitterFumili::TFitterFumili(Int_t /* maxpar */) {
    SetName("Fumili2");
    gFumili2 = this;
    gROOT->GetListOfSpecials()->Add(gFumili2);
}


// create the minimizer engine and register the plugin in ROOT
// what ever we specify only Fumili is created  
void TFitterFumili::CreateMinimizer(EMinimizerType ) { 
  if (PrintLevel() >=1 ) 
    std::cout<<"TFitterFumili: Minimize using new Fumili algorithm "<<std::endl;

  const ModularFunctionMinimizer * minimizer = GetMinimizer();
  if (!minimizer) delete minimizer;
  SetMinimizer( new FumiliMinimizer() );

  //SetStrategy(2);
  // Fumili cannot deal with tolerance too smalls (10-3 corrsponds to 10-7 in FumiliBuilder)
  SetMinimumTolerance(0.001); 

#ifdef DEBUG
  SetPrintLevel(3);
#endif
}


void TFitterFumili::CreateChi2FCN() { 
  SetMinuitFCN(new TFumiliChi2FCN( *this,GetStrategy()) );
}

void TFitterFumili::CreateChi2ExtendedFCN() { 
  //for Fumili use normal method 
  SetMinuitFCN(new TFumiliChi2FCN(*this, GetStrategy()));
}

void TFitterFumili::CreateBinLikelihoodFCN() { 
  SetMinuitFCN( new TFumiliBinLikelihoodFCN( *this, GetStrategy()) );
}
