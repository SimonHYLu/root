/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooNLLVar.cc,v 1.12 2005/02/14 20:44:25 wverkerke Exp $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2004, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

// -- CLASS DESCRIPTION [PDF] --
// Class RooNLLVar implements a a -log(likelihood) calculation from a dataset
// and a PDF. The NLL is calculated as 
//
//  Sum[data] -log( pdf(x_data) )
//
// In extended mode, a (Nexpect - Nobserved*log(NExpected) term is added

#include "RooFitCore/RooNLLVar.hh"
#include "RooFitCore/RooAbsData.hh"
#include "RooFitCore/RooAbsPdf.hh"
using std::cout;
using std::endl;

ClassImp(RooNLLVar)
;

RooNLLVar::RooNLLVar(const char *name, const char *title, RooAbsPdf& pdf, RooAbsData& data,
		     Bool_t extended, const char* rangeName, Int_t nCPU) : 
  RooAbsOptGoodnessOfFit(name,title,pdf,data,RooArgSet(),rangeName,nCPU),
  _extended(extended)
{
  
}


RooNLLVar::RooNLLVar(const char *name, const char *title, RooAbsPdf& pdf, RooAbsData& data,
		     const RooArgSet& projDeps, Bool_t extended, const char* rangeName,Int_t nCPU) : 
  RooAbsOptGoodnessOfFit(name,title,pdf,data,projDeps,rangeName,nCPU),
  _extended(extended)
{
  
}


RooNLLVar::RooNLLVar(const RooNLLVar& other, const char* name) : 
  RooAbsOptGoodnessOfFit(other,name),
  _extended(other._extended)
{
}


RooNLLVar::~RooNLLVar()
{
}


Double_t RooNLLVar::evaluatePartition(Int_t firstEvent, Int_t lastEvent) const 
{
  Int_t i ;
  Double_t result(0) ;
  
  Double_t sumWeight(0) ;
  for (i=firstEvent ; i<lastEvent ; i++) {
    
    // get the data values for this event
    _dataClone->get(i);
    if (_dataClone->weight()==0) continue ;

    //cout << "RooNLLVar(" << GetName() << ") wgt[" << i << "] = " << _dataClone->weight() << endl ;

    Double_t term = _dataClone->weight() * _pdfClone->getLogVal(_normSet);
    sumWeight += _dataClone->weight() ;

    // If any event evaluates with zero probability, abort calculation
    if(term == 0) {
      cout << "RooNLLVar::evaluatePartition(" << GetName() 
	   << "): WARNING: event " << i << " has zero or negative probability" << endl ;
      return 0 ;
    }

    result-= term;
  }
  
  // include the extended maximum likelihood term, if requested
  if(_extended && firstEvent==0) {
    result+= _pdfClone->extendedTerm((Int_t)_dataClone->sumEntries(),_dataClone->get());
  }    

  // If part of simultaneous PDF normalize probability over 
  // number of simultaneous PDFs: -sum(log(p/n)) = -sum(log(p)) + N*log(n) 
  if (_simCount>1) {
    result += sumWeight*log(1.0*_simCount) ;
  }

  return result ;
}



