#include "BaBar/BaBar.hh"
/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 *    File: $Id: RooBreitWigner.cc,v 1.7 2004/08/07 06:45:41 bartoldu Exp $
 * Authors:                                                                  *
 *   AS, Abi Soffer, Colorado State University, abi@slac.stanford.edu        *
 *   TS, Thomas Schietinger, SLAC, schieti@slac.stanford.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2004, Regents of the University of California          *
 *                          Colorado State University                        *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

// -- CLASS DESCRIPTION [PDF] --


#include <iostream>
#include <math.h>

#include "RooFitModels/RooBreitWigner.hh"
#include "RooFitCore/RooAbsReal.hh"
#include "RooFitCore/RooRealVar.hh"
// #include "RooFitTools/RooRandom.hh"

ClassImp(RooBreitWigner)

RooBreitWigner::RooBreitWigner(const char *name, const char *title,
			 RooAbsReal& _x, RooAbsReal& _mean,
			 RooAbsReal& _width) :
  RooAbsPdf(name,title),
  x("x","Dependent",this,_x),
  mean("mean","Mean",this,_mean),
  width("width","Width",this,_width)
{
}


RooBreitWigner::RooBreitWigner(const RooBreitWigner& other, const char* name) : 
  RooAbsPdf(other,name), x("x",this,other.x), mean("mean",this,other.mean),
  width("width",this,other.width)
{
}


Double_t RooBreitWigner::evaluate() const
{
  Double_t arg= x - mean;  
  return 1. / (arg*arg + 0.25*width*width);
}


Int_t RooBreitWigner::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars) const 
{
  if (matchArgs(allVars,analVars,x)) return 1 ;
  return 0 ;
}


Double_t RooBreitWigner::analyticalIntegral(Int_t code) const 
{
  switch(code) {
  case 1: 
    {
      Double_t c = 2./width;
      return c*(atan(c*(x.max()-mean)) - atan(c*(x.min()-mean)));
    }
  }
  
  assert(0) ;
  return 0 ;
}

