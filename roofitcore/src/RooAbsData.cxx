/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooAbsData.cc,v 1.20 2004/11/29 12:22:10 wverkerke Exp $
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

// -- CLASS DESCRIPTION [DATA] --
// RooAbsData is the common abstract base class for binned and unbinned
// datasets. The abstract interface defines plotting and tabulating entry
// points for its contents and provides an iterator over its elements
// (bins for binned data sets, data points for unbinned datasets).

#include "RooFitCore/RooAbsData.hh"
#include "RooFitCore/RooFormulaVar.hh"
using std::cout;
using std::endl;

ClassImp(RooAbsData)
;


RooAbsData::RooAbsData() 
{
  // Default constructor
  _iterator = _vars.createIterator() ;
  _cacheIter = _cachedVars.createIterator() ;
}


RooAbsData::RooAbsData(const char *name, const char *title, const RooArgSet& vars) :
  TNamed(name,title), _vars("Dataset Variables"), _cachedVars("Cached Variables"), 
  _doDirtyProp(kTRUE) 
{
  // Constructor from a set of variables. Only fundamental elements of vars
  // (RooRealVar,RooCategory etc) are stored as part of the dataset

  // clone the fundamentals of the given data set into internal buffer
  TIterator* iter = vars.createIterator() ;
  RooAbsArg *var;
  while(0 != (var= (RooAbsArg*)iter->Next())) {
    if (!var->isFundamental()) {
      cout << "RooAbsSet::initialize(" << GetName() 
	   << "): Data set cannot contain non-fundamental types, ignoring " 
	   << var->GetName() << endl ;
    } else {
      _vars.addClone(*var);
    }
  }
  delete iter ;

  _iterator= _vars.createIterator();
  _cacheIter = _cachedVars.createIterator() ;
}




RooAbsData::RooAbsData(const RooAbsData& other, const char* newname) : 
  TNamed(newname?newname:other.GetName(),other.GetTitle()), _vars(),
  _cachedVars("Cached Variables"), _doDirtyProp(kTRUE)
{
  // Copy constructor
  _vars.addClone(other._vars) ;
  _iterator= _vars.createIterator();
  _cacheIter = _cachedVars.createIterator() ;
}


RooAbsData::~RooAbsData() 
{
  // Destructor
  
  // delete owned contents.
  delete _iterator ;
  delete _cacheIter ;
}



RooAbsData* RooAbsData::reduce(const char* cut) 
{ 
  // Create a subset of the data set by applying the given cut on the data points.
  // The cut expression can refer to any variable in the data set. For cuts involving 
  // other variables, such as intermediate formula objects, use the equivalent 
  // reduce method specifying the as a RooFormulVar reference.

  RooFormulaVar cutVar(cut,cut,*get()) ;
  return reduceEng(*get(),&cutVar,kFALSE) ;
}




RooAbsData* RooAbsData::reduce(const RooFormulaVar& cutVar) 
{
  // Create a subset of the data set by applying the given cut on the data points.
  // The 'cutVar' formula variable is used to select the subset of data points to be 
  // retained in the reduced data collection.
  return reduceEng(*get(),&cutVar,kFALSE) ;
}




RooAbsData* RooAbsData::reduce(const RooArgSet& varSubset, const char* cut) 
{
  // Create a subset of the data set by applying the given cut on the data points
  // and reducing the dimensions to the specified set.
  // 
  // The cut expression can refer to any variable in the data set. For cuts involving 
  // other variables, such as intermediate formula objects, use the equivalent 
  // reduce method specifying the as a RooFormulVar reference.

  // Make sure varSubset doesn't contain any variable not in this dataset
  RooArgSet varSubset2(varSubset) ;
  TIterator* iter = varSubset.createIterator() ;
  RooAbsArg* arg ;
  while(arg=(RooAbsArg*)iter->Next()) {
    if (!_vars.find(arg->GetName())) {
      cout << "RooAbsData::reduce(" << GetName() << ") WARNING: variable " 
	   << arg->GetName() << " not in dataset, ignored" << endl ;
      varSubset2.remove(*arg) ;
    }
  }
  delete iter ;

  if (cut && strlen(cut)>0) {
    RooFormulaVar cutVar(cut,cut,*get()) ;
    return reduceEng(varSubset2,&cutVar,kFALSE) ;      
  } 
  return reduceEng(varSubset2,0,kFALSE) ;
}




RooAbsData* RooAbsData::reduce(const RooArgSet& varSubset, const RooFormulaVar& cutVar) 
{
  // Create a subset of the data set by applying the given cut on the data points
  // and reducing the dimensions to the specified set.
  // 
  // The 'cutVar' formula variable is used to select the subset of data points to be 
  // retained in the reduced data collection.

  // Make sure varSubset doesn't contain any variable not in this dataset
  RooArgSet varSubset2(varSubset) ;
  TIterator* iter = varSubset.createIterator() ;
  RooAbsArg* arg ;
  while(arg=(RooAbsArg*)iter->Next()) {
    if (!_vars.find(arg->GetName())) {
      cout << "RooAbsData::reduce(" << GetName() << ") WARNING: variable " 
	   << arg->GetName() << " not in dataset, ignored" << endl ;
      varSubset2.remove(*arg) ;
    }
  }
  delete iter ;

  return reduceEng(varSubset2,&cutVar,kFALSE) ;
}

