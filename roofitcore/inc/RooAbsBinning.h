/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooAbsBinning.rdl,v 1.7 2004/12/03 13:18:28 wverkerke Exp $
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
#ifndef ROO_ABS_BINNING
#define ROO_ABS_BINNING

#include "Rtypes.h"
#include "RooFitCore/RooPrintable.hh"
#include "TNamed.h" 
class TIterator ;

class RooAbsBinning : public TNamed, public RooPrintable {
public:

  RooAbsBinning(const char* name=0) ;
  RooAbsBinning(const RooAbsBinning&, const char* name=0) {}
  virtual TObject* Clone(const char* newname=0) const { return clone(newname) ; }
  virtual RooAbsBinning* clone(const char* name=0) const = 0 ;
  virtual ~RooAbsBinning() ;

  Int_t numBins() const { return numBoundaries()-1 ; }
  virtual Int_t numBoundaries() const = 0 ;
  virtual Int_t binNumber(Double_t x) const = 0 ;
  virtual Double_t binCenter(Int_t bin) const = 0 ;
  virtual Double_t binWidth(Int_t bin) const = 0 ;
  virtual Double_t binLow(Int_t bin) const = 0 ;
  virtual Double_t binHigh(Int_t bin) const = 0 ;

  virtual void setRange(Double_t xlo, Double_t xhi) = 0 ;
  virtual void setMin(Double_t xlo) { setRange(xlo,highBound()) ; }
  virtual void setMax(Double_t xhi) { setRange(lowBound(),xhi) ; }

  virtual Double_t lowBound() const = 0 ;
  virtual Double_t highBound() const = 0 ;
  virtual Double_t averageBinWidth() const = 0 ;


  virtual Double_t* array() const = 0 ;

  inline virtual void Print(Option_t *options= 0) const {
    printToStream(defaultStream(),parseOptions(options));
  }
  virtual void printToStream(std::ostream &os, PrintOption opt= Standard, TString indent= "") const;

protected:
  

  ClassDef(RooAbsBinning,1) // Abstract base class for binning specification
};

#endif
