/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooFitCore
 *    File: $Id: RooDataHist.rdl,v 1.16 2002/06/18 22:19:22 verkerke Exp $
 * Authors:
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu
 *   DK, David Kirkby, Stanford University, kirkby@hep.stanford.edu
 * History:
 *   16-Aug-2001 WV Created initial version
 *
 * Copyright (C) 2001 University of California
 *****************************************************************************/
#ifndef ROO_DATA_HIST
#define ROO_DATA_HIST

#include "TObject.h"
#include "RooFitCore/RooTreeData.hh"
#include "RooFitCore/RooDirItem.hh"
#include "RooFitCore/RooArgSet.hh"

class RooAbsArg;
class RooAbsReal ;
class RooAbsCategory ;
class Roo1DTable ;
class RooPlot;
class RooArgSet ;

class RooDataHist : public RooTreeData, public RooDirItem {
public:

  // Constructors, factory methods etc.
  RooDataHist() ; 
  RooDataHist(const char *name, const char *title, const RooArgSet& vars) ;
  RooDataHist(const char *name, const char *title, const RooArgSet& vars, const RooAbsData& data, Double_t initWgt=1.0) ;
  RooDataHist(const char *name, const char *title, const RooArgList& vars, const TH1* hist, Double_t initWgt=1.0) ;
  RooDataHist(const RooDataHist& other, const char* newname = 0) ;
  virtual TObject* Clone(const char* newname=0) const { return new RooDataHist(*this,newname?newname:GetName()) ; }
  virtual ~RooDataHist() ;

  virtual RooAbsData* emptyClone(const char* newName=0, const char* newTitle=0) const {
    return new RooDataHist(newName?newName:GetName(),newTitle?newTitle:GetTitle(),*get()) ; 
  }

  // Add one ore more rows of data
  virtual void add(const RooArgSet& row, Double_t weight=1.0) ;
  void set(const RooArgSet& row, Double_t weight, Double_t wgtErr=-1) ;
  void set(const RooArgSet& row, Double_t weight, Double_t wgtErrLo, Double_t wgtErrHi) ;

  void add(const RooAbsData& dset, const RooFormulaVar* cutVar=0, Double_t weight=1.0 ) ;
  void add(const RooAbsData& dset, const char* cut, Double_t weight=1.0 ) ;

  virtual const RooArgSet* get() const { return &_vars ; } 
  virtual const RooArgSet* get(Int_t masterIdx) const ;
  virtual const RooArgSet* get(const RooArgSet& coord) const ;
  virtual Int_t numEntries(Bool_t useWeights=kFALSE) const ; 
  virtual Bool_t isWeighted() const { return kTRUE ; }

  Double_t sum(Bool_t correctForBinSize) const ;
  Double_t sum(const RooArgSet& sumSet, const RooArgSet& sliceSet, Bool_t correctForBinSize) ;

  virtual Double_t weight() const { return _curWeight ; }
  Double_t weight(const RooArgSet& bin, Int_t intOrder=1, Bool_t correctForBinSize=kFALSE) ;   
  Double_t binVolume() const { return _curVolume ; }
  Double_t binVolume(const RooArgSet& bin) ; 
  inline Double_t weightError() const {
    Double_t lo,hi ;
    weightError(lo,hi) ;
    return (lo+hi)/2 ;
  }

  void weightError(Double_t& lo, Double_t& hi) const ;

  virtual RooPlot *plotOn(RooPlot *frame, const char* cuts="", Option_t* drawOptions="P", const RooAbsBinning* bins=0) const;
  virtual RooPlot *plotOn(RooPlot *frame, const RooFormulaVar* cutVar, Option_t* drawOptions="P", const RooAbsBinning* bins=0) const;

  virtual void reset() ;
  void dump2() ;

protected:
 
  void initialize() ;
  RooDataHist(const char* name, const char* title, RooDataHist* h, const RooArgSet& varSubset, 
	      const RooFormulaVar* cutVar, Bool_t copyCache) ;
  virtual RooAbsData* reduceEng(const RooArgSet& varSubset, const RooFormulaVar* cutVar, Bool_t copyCache=kTRUE) ;
  Double_t interpolateDim(RooRealVar& dim, Double_t xval, Int_t intOrder, Bool_t correctForBinSize) ;

  virtual RooAbsData* cacheClone(const RooArgSet* newCacheVars, const char* newName=0) ;

  Int_t calcTreeIndex() const ;

  Int_t       _arrSize ; //  Size of the weight array
  Int_t*      _idxMult ; //! Multiplier jump table for index calculation
  Double_t*       _wgt ; //[_arrSize] Weight array
  Double_t*    _errLo ; //[_arrSize] Low-side error on weight array
  Double_t*    _errHi ; //[_arrSize] High-side error on weight array
  Double_t*      _binv ; //[_arrSize] Bin volume array
  RooArgSet  _realVars ; // Real dimensions of the dataset 
  TIterator* _realIter ; //! Iterator over realVars

  mutable Double_t _curWeight ; // Weight associated with the current coordinate
  mutable Double_t _curWgtErrLo ; // Error on weight associated with the current coordinate
  mutable Double_t _curWgtErrHi ; // Error on weight associated with the current coordinate
  mutable Double_t _curVolume ; // Volume of bin enclosing current coordinate
  mutable Int_t    _curIndex ; // Current index

private:

  ClassDef(RooDataHist,1) // Binned data set
};

#endif

