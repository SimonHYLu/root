/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooUniformBinning.rdl,v 1.6 2004/08/09 00:00:56 bartoldu Exp $
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
#ifndef ROO_UNIFORM_BINNING
#define ROO_UNIFORM_BINNING

#include "Rtypes.h"
#include "RooFitCore/RooAbsBinning.hh"
class TIterator ;

class RooUniformBinning : public RooAbsBinning {
public:

  RooUniformBinning() ;
  RooUniformBinning(Double_t xlo, Double_t xhi, Int_t nBins) ;
  RooUniformBinning(const RooUniformBinning& other) ;
  RooAbsBinning* clone() const { return new RooUniformBinning(*this) ; }
  virtual ~RooUniformBinning() ;

  virtual void setRange(Double_t xlo, Double_t xhi) ;

  virtual Int_t numBoundaries() const { return _nbins + 1 ; }
  virtual Int_t binNumber(Double_t x) const  ;

  virtual Double_t lowBound() const { return _xlo ; }
  virtual Double_t highBound() const { return _xhi ; }

  virtual Double_t binCenter(Int_t bin) const ;
  virtual Double_t binWidth(Int_t bin) const ;
  virtual Double_t binLow(Int_t bin) const ;
  virtual Double_t binHigh(Int_t bin) const ;

  virtual Double_t averageBinWidth() const { return _binw ; }
  virtual Double_t* array() const ;

  virtual void printToStream(std::ostream &os, PrintOption opt= Standard, TString indent= "") const;

protected:

  mutable Double_t* _array ; //! do not persist
  Double_t _xlo ;
  Double_t _xhi ;
  Int_t    _nbins ;
  Double_t _binw ;


  ClassDef(RooUniformBinning,1) // Uniform binning specification
};

#endif
