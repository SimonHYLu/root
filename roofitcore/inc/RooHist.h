/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id$
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2002, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/
#ifndef ROO_HIST
#define ROO_HIST

#include "TGraphAsymmErrors.h"
#include "RooFitCore/RooPlotable.hh"

class TH1;

class RooHist : public TGraphAsymmErrors, public RooPlotable {
public:
  RooHist() {} ;
  RooHist(Double_t nominalBinWidth, Double_t nSigma= 1);
  RooHist(const TH1 &data, Double_t nominalBinWidth= 0, Double_t nSigma= 1);
  RooHist(const TH1 &data1, const TH1 &data2, Double_t nominalBinWidth= 0, Double_t nSigma= 1);
  virtual ~RooHist();

  // add a datapoint for a bin with n entries, using a Poisson error
  void addBin(Axis_t binCenter, Int_t n, Double_t binWidth= 0);
  // add a datapoint for the asymmetry (n1-n2)/(n1+n2), using a binomial error
  void addAsymmetryBin(Axis_t binCenter, Int_t n1, Int_t n2, Double_t binWidth= 0);

  virtual void printToStream(ostream& os, PrintOption opt= Standard, TString indent= "") const;
  inline virtual void Print(Option_t *options= 0) const {
    printToStream(defaultStream(),parseOptions(options));
  }

  Double_t getFitRangeNEvt() const;
  Double_t getFitRangeBinW() const;
  inline Double_t getNominalBinWidth() const { return _nominalBinWidth; }
  inline void setRawEntries(Int_t n) { _rawEntries = n ; } 

protected:
  void initialize();
  Int_t roundBin(Stat_t y);
private:
  Double_t _nominalBinWidth,_nSigma,_entries,_rawEntries;
  ClassDef(RooHist,1) // 1-dimensional histogram with error bars
};

#endif
