/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooFitCore
 *    File: $Id: RooSuperCategory.cc,v 1.4 2001/05/11 23:37:41 verkerke Exp $
 * Authors:
 *   WV, Wouter Verkerke, UCSB, verkerke@slac.stanford.edu
 * History:
 *   01-Mar-2001 WV Create initial version
 *
 * Copyright (C) 2001 University of California
 *****************************************************************************/

#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include "TString.h"
#include "RooFitCore/RooSuperCategory.hh"
#include "RooFitCore/RooStreamParser.hh"
#include "RooFitCore/RooArgSet.hh"
#include "RooFitCore/RooMultiCatIter.hh"
#include "RooFitCore/RooAbsCategoryLValue.hh"

ClassImp(RooSuperCategory)


//WVE change _catList from RooArgSet to TList of RooCategoryProxy, or make RooArgSetProxy ;

RooSuperCategory::RooSuperCategory(const char *name, const char *title, RooArgSet& inputCatList) :
  RooAbsCategoryLValue(name, title) 
{  
  // Constructor from list of input categories

  // Copy category list
  RooArgSet* catList = new RooArgSet("catList") ;
  TIterator* iter = inputCatList.MakeIterator() ;
  RooAbsArg* arg ;
  while (arg=(RooAbsArg*)iter->Next()) {
    if (!arg->IsA()->InheritsFrom(RooAbsCategoryLValue::Class())) {
      cout << "RooSuperCategory::RooSuperCategory(" << GetName() << "): input category " << arg->GetName() 
	   << " is not an lvalue" << endl ;
    }
    catList->add(*arg) ;
  }
  delete iter ;
  _catListProxy = new RooSetProxy("catList","catList",this,*catList,kTRUE,kTRUE) ;
  
  updateIndexList() ;
}


RooSuperCategory::RooSuperCategory(const RooSuperCategory& other, const char *name) :
  RooAbsCategoryLValue(other,name)
{
  // Copy constructor

  RooArgSet* catList = new RooArgSet("catList") ;
  TIterator* iter = other._catListProxy->set()->MakeIterator() ;
  RooAbsArg* arg ;
  while (arg=(RooAbsArg*)iter->Next()) {
    catList->add(*arg) ;
  }
  delete iter ;
  _catListProxy = new RooSetProxy("catList","catList",this,*catList,kTRUE,kTRUE) ;

  updateIndexList() ;
  setIndex(other.getIndex()) ;
}



RooSuperCategory::~RooSuperCategory() 
{
  // Destructor

  // We own the ArgSet of the proxy
  delete _catListProxy->set() ;
}



TIterator* RooSuperCategory::MakeIterator() const 
{
  // Make an iterator over the input categories of this supercategory
  return new RooMultiCatIter(*_catListProxy->set()) ;
}



void RooSuperCategory::updateIndexList()
{
  // Update the list of our category states 

  clearTypes() ;
  RooArgSet* catListClone = _catListProxy->set()->snapshot(kTRUE) ;
  RooMultiCatIter mcIter(*_catListProxy->set()) ;

  while(mcIter.Next()) {
    // Register composite label
    defineType(currentLabel()) ;
  }

  *_catListProxy->set() = *catListClone ;
  delete catListClone ;

  // Renumbering will invalidate cache
  setValueDirty(kTRUE) ;
}


TString RooSuperCategory::currentLabel() const
{
  // Return the name of the current state, 
  // constructed from the state names of the input categories

  TIterator* lIter = _catListProxy->set()->MakeIterator() ;

  // Construct composite label name
  TString label ;
  RooAbsCategory* cat ;
  Bool_t first(kTRUE) ;
  while(cat=(RooAbsCategory*) lIter->Next()) {
    label.Append(first?"{":";") ;
    label.Append(cat->getLabel()) ;      
    first=kFALSE ;
  }
  label.Append("}") ;  
  delete lIter ;

  return label ;
}


RooCatType
RooSuperCategory::evaluate() const
{
  // Calculate the current value 
  if (isShapeDirty()) const_cast<RooSuperCategory*>(this)->updateIndexList() ;
  return *lookupType(currentLabel()) ;
}


Bool_t RooSuperCategory::setIndex(Int_t index, Bool_t printError) 
{
  // Set the value of the super category by specifying the state index code
  // Indirectly sets the values of the input categories
  const RooCatType* type = lookupType(index,kTRUE) ;
  if (!type) return kTRUE ;
  return setType(type) ;
}


Bool_t RooSuperCategory::setLabel(const char* label, Bool_t printError) 
{
  // Set the value of the super category by specifying the state name
  // Indirectly sets the values of the input categories
  const RooCatType* type = lookupType(label,kTRUE) ;
  if (!type) return kTRUE ;
  return setType(type) ;
}


Bool_t RooSuperCategory::setType(const RooCatType* type, Bool_t printError)
{
  // Set the value of the super category by specifying the state object
  // Indirectly sets the values of the input categories

  // WVE: adapt parser to understand composite super categories
  char buf[1024] ;
  strcpy(buf,type->GetName()) ;

  TIterator* iter = _catListProxy->set()->MakeIterator() ;
  RooAbsCategoryLValue* arg ;
  Bool_t error(kFALSE) ;

  // Parse composite label and set label of components to their values
  char* ptr = strtok(buf+1,";}") ;
  while (arg=(RooAbsCategoryLValue*)iter->Next()) {
    error |= arg->setLabel(ptr) ;
    ptr = strtok(0,";}") ;
  }
  
  delete iter ;
  return error ;
}



void RooSuperCategory::printToStream(ostream& os, PrintOption opt, TString indent) const
{
  // Print the state of this object to the specified output stream.

  RooAbsCategory::printToStream(os,opt,indent) ;
  
  if (opt>=Verbose) {     
    os << indent << "--- RooSuperCategory ---" << endl;
    os << indent << "  Input category list:" << endl ;
    _catListProxy->set()->printToStream(os,Standard,TString(indent).Append("  ")) ;
  }
}


Bool_t RooSuperCategory::readFromStream(istream& is, Bool_t compact, Bool_t verbose) 
{
  // Read object contents from given stream
  return kTRUE ;
}



void RooSuperCategory::writeToStream(ostream& os, Bool_t compact) const
{
  // Write object contents to given stream
  RooAbsCategory::writeToStream(os,compact) ;
}
