/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooFitCore
 *    File: $Id: RooAbsPdf.rdl,v 1.59 2002/06/19 20:59:39 verkerke Exp $
 * Authors:
 *   DK, David Kirkby, Stanford University, kirkby@hep.stanford.edu
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu
 *   AB, Adrian Bevan, Liverpool University, bevan@slac.stanford.edu
 * History:
 *   07-Mar-2001 WV Created initial version
 *   26-Aug-2001 AB Added TH2F * plot methods
 *
 * Copyright (C) 2001 University of California
 *****************************************************************************/
#ifndef ROO_ABS_PDF
#define ROO_ABS_PDF

#include "RooFitCore/RooAbsReal.hh"
#include "RooFitCore/RooRealIntegral.hh"
#include "RooFitCore/RooNameSet.hh"
#include "RooFitCore/RooNormSetCache.hh"
#include "RooFitCore/RooNormManager.hh"

class RooDataSet;
class RooArgSet ;
class RooRealProxy ;
class RooAbsGenContext ;
class RooFitResult ;
class RooExtendPdf ;
class TPaveText;
class TH1F;
class TH2F;
class TList ;
class RooIntegratorConfig ;

class RooAbsPdf : public RooAbsReal {
public:

  // Constructors, assignment etc
  inline RooAbsPdf() { }
  RooAbsPdf(const char *name, const char *title) ;
  RooAbsPdf(const char *name, const char *title, Double_t minVal, Double_t maxVal) ;
  // RooAbsPdf(const RooAbsPdf& other, const char* name=0);
  virtual ~RooAbsPdf();

  // Toy MC generation
  RooDataSet *generate(const RooArgSet &whatVars, Int_t nEvents = 0, Bool_t verbose=kFALSE) const;
  RooDataSet *generate(const RooArgSet &whatVars, const RooDataSet &prototype, Int_t nEvents= 0,
		       Bool_t verbose=kFALSE) const;


  // PDF specific plotting
  virtual RooPlot *plotOn(RooPlot *frame, Option_t* drawOptions="L", Double_t scaleFactor= 1.0, 
			  ScaleType stype=Relative, const RooAbsData* projData=0, const RooArgSet* projSet=0) const;

  virtual RooPlot *plotCompOn(RooPlot *frame, const char* compNameList, Option_t* drawOptions="L",
			      Double_t scaleFactor= 1.0, ScaleType stype=Relative, 
			      const RooAbsData* projData=0, const RooArgSet* projSet=0) const ;

  virtual RooPlot *plotCompOn(RooPlot *frame, const RooArgSet& compSet, Option_t* drawOptions="L",
			      Double_t scaleFactor= 1.0, ScaleType stype=Relative, 
			      const RooAbsData* projData=0, const RooArgSet* projSet=0) const ;

  virtual RooPlot *plotCompSliceOn(RooPlot *frame, const char* compNameList, const RooArgSet& sliceSet,
				   Option_t* drawOptions="L", Double_t scaleFactor= 1.0, ScaleType stype=Relative, 
				   const RooAbsData* projData=0) const ;

  virtual RooPlot *plotCompSliceOn(RooPlot *frame, const RooArgSet& compSet, const RooArgSet& sliceSet,
				   Option_t* drawOptions="L", Double_t scaleFactor= 1.0, ScaleType stype=Relative, 
				   const RooAbsData* projData=0) const ;

  virtual RooPlot* paramOn(RooPlot* frame, const RooAbsData* data, const char *label= "", Int_t sigDigits = 2,
			   Option_t *options = "NELU", Double_t xmin=0.65,
			   Double_t xmax= 0.99,Double_t ymax=0.95) ;



  // Backward compatibility functions
  inline RooPlot *plotNLLOn(RooPlot* frame, RooDataSet* data, Option_t* drawOptions="L", 
                            Double_t prec=1e-2, Bool_t fixMinToZero=kTRUE) {
    return plotNLLOn(frame,data,kFALSE,RooArgSet(),drawOptions,prec,fixMinToZero) ;
  }
  virtual RooPlot *plotNLLOn(RooPlot* frame, RooDataSet* data, Bool_t extended, Option_t* drawOptions="L", 
                             Double_t prec=1e-2, Bool_t fixMinToZero=kTRUE) {
    return plotNLLOn(frame,data,extended,RooArgSet(),drawOptions,prec,fixMinToZero) ;
  }
  virtual RooPlot *plotNLLOn(RooPlot* frame, RooDataSet* data, Bool_t extended, const RooArgSet& projDeps,
                             Option_t* drawOptions="L", Double_t prec=1e-2, Bool_t fixMinToZero=kTRUE) ;



  // Built-in generator support
  virtual Int_t getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t staticInitOK=kTRUE) const;
  virtual void initGenerator(Int_t code) ;
  virtual void generateEvent(Int_t code);  

  virtual Bool_t isDirectGenSafe(const RooAbsArg& arg) const ; 

  // Interactions with a dataset  
  virtual RooFitResult* fitTo(RooAbsData& data, const RooArgSet& projDeps, 
			      Option_t *fitOpt = "", Option_t *optOpt = "c" ) ;
  virtual RooFitResult* fitTo(RooAbsData& data, Option_t *fitOpt = "", Option_t *optOpt = "c") ;

  // Function evaluation support
  virtual Bool_t traceEvalHook(Double_t value) const ;  
  virtual Double_t getVal(const RooArgSet* set=0) const ;
  Double_t getLogVal(const RooArgSet* set=0) const ;
  Double_t getNorm(const RooArgSet& nset) const { return getNorm(&nset) ; }
  virtual Double_t getNorm(const RooArgSet* set=0) const ;
  void resetErrorCounters(Int_t resetValue=10) ;
  void setTraceCounter(Int_t value, Bool_t allNodes=kFALSE) ;
  Bool_t traceEvalPdf(Double_t value) const ;

  Double_t analyticalIntegralWN(Int_t code, const RooArgSet* normSet) const ;

  virtual Bool_t selfNormalized() const { return kFALSE ; }

  // Support for extended maximum likelihood, switched off by default
  enum ExtendMode { CanNotBeExtended, CanBeExtended, MustBeExtended } ;
  virtual ExtendMode extendMode() const { return CanNotBeExtended ; } 
  inline Bool_t canBeExtended() const { return (extendMode() != CanNotBeExtended) ; }
  inline Bool_t mustBeExtended() const { return (extendMode() == MustBeExtended) ; }
  virtual Double_t expectedEvents() const { return 0 ; } 

  // Printing interface (human readable)
  virtual void printToStream(ostream& stream, PrintOption opt=Standard, TString indent= "") const ;

  static void verboseEval(Int_t stat) { _verboseEval = stat ; }

  inline Bool_t isSelectedComp() const { return _selectComp || _globalSelectComp ; }

  const RooIntegratorConfig* getNormIntConfig() const ;
  const RooIntegratorConfig* getDefaultNormIntConfig() const ;
  const RooIntegratorConfig* getSpecialNormIntConfig() const ;
  static void setDefaultNormIntConfig(const RooIntegratorConfig& config) ;
  void setNormIntConfig() ;
  void setNormIntConfig(const RooIntegratorConfig& config) ;

  virtual void fixAddCoefNormalization(const RooArgSet& addNormSet=RooArgSet()) ;

  virtual Double_t extendedTerm(UInt_t observedEvents) const ;
  
private:

  // This forces definition copy ctor in derived classes 
  RooAbsPdf(const RooAbsPdf& other);

protected:

  friend class RooAddGenContext ;
  friend class RooProdGenContext ;
  friend class RooSimGenContext ;
  friend class RooSimultaneous ;
  friend class RooMCStudy ;
  virtual RooAbsGenContext* genContext(const RooArgSet &vars, 
				       const RooDataSet *prototype=0, Bool_t verbose= kFALSE) const ;


  friend class RooExtendPdf ;
  RooAbsPdf(const RooAbsPdf& other, const char* name=0);

  friend class RooRealIntegral ;
  static Int_t _verboseEval ;

  virtual Bool_t syncNormalization(const RooArgSet* dset, Bool_t adjustProxies=kTRUE) const ;
  virtual Bool_t syncNormalizationPreHook(RooAbsReal* norm,const RooArgSet* dset) const { return kFALSE ; } ;
  virtual void syncNormalizationPostHook(RooAbsReal* norm,const RooArgSet* dset) const {} ;

  virtual RooPlot *plotCompOnEngine(RooPlot *frame, RooArgSet* selNodes, Option_t* drawOptions="L",
				    Double_t scaleFactor= 1.0, ScaleType stype=Relative, 
				    const RooAbsData* projData=0, const RooArgSet* projSet=0) const ;

  virtual void operModeHook() ;

  friend class RooConvolutedPdf ;
  mutable Double_t _rawValue ;
  mutable RooAbsReal* _norm   ;      // Normalization integral (owned by _normMgr)
  mutable RooArgSet* _normSet ;      // Normalization set with for above integral
  mutable RooNormManager _normMgr ;  // Normalization manager

  mutable Int_t _errorCount ;        // Number of errors remaining to print
  mutable Int_t _traceCount ;        // Number of traces remaining to print
  mutable Int_t _negCount ;          // Number of negative probablities remaining to print


  RooIntegratorConfig* _specNormIntConfig ;
  static RooIntegratorConfig* _defaultNormIntConfig ;

  friend class RooAddPdf ;
  void selectComp(Bool_t flag) { _selectComp = flag ; }
  static void globalSelectComp(Bool_t flag) { _globalSelectComp = flag ; }
  Bool_t _selectComp ;               // Component selection flag for RooAbsPdf::plotCompOn
  static Bool_t _globalSelectComp ;  // Global activation switch for component selection
  
  ClassDef(RooAbsPdf,1) // Abstract PDF with normalization support
};

#endif
