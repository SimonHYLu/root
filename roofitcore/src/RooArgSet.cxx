/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooFitCore
 *    File: $Id: RooArgSet.cc,v 1.2 2001/03/16 07:59:11 verkerke Exp $
 * Authors:
 *   DK, David Kirkby, Stanford University, kirkby@hep.stanford.edu
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu
 * History:
 *   07-Mar-2001 WV Created initial version
 *
 * Copyright (C) 2001 University of California
 *****************************************************************************/

#include <iostream.h>
#include "RooFitCore/RooArgSet.hh"
#include "RooFitCore/RooStreamParser.hh"

ClassImp(RooArgSet)


RooArgSet::RooArgSet() :
  _name(), THashList(), _isCopy(kFALSE)
{
}

RooArgSet::RooArgSet(const char *name) :
  _name(name), THashList(), _isCopy(kFALSE)
{
}

RooArgSet::RooArgSet(const char *name, RooAbsArg& var1) :
  _name(name), THashList(), _isCopy(kFALSE)
{
  add(var1);
}

RooArgSet::RooArgSet(const char *name, RooAbsArg& var1,
		       RooAbsArg& var2) :
  _name(name), THashList(), _isCopy(kFALSE)
{
  add(var1); add(var2);
}

RooArgSet::RooArgSet(const char *name, RooAbsArg& var1,
		       RooAbsArg& var2, RooAbsArg& var3) :
  _name(name), THashList(), _isCopy(kFALSE)
{
  add(var1); add(var2); add(var3);
}

RooArgSet::RooArgSet(const char *name, RooAbsArg& var1,
		       RooAbsArg& var2, RooAbsArg& var3,
		       RooAbsArg& var4) :
  _name(name), THashList(), _isCopy(kFALSE)
{
  add(var1); add(var2); add(var3); add(var4);
}

RooArgSet::RooArgSet(const char *name, RooAbsArg& var1,
		       RooAbsArg& var2, RooAbsArg& var3,
		       RooAbsArg& var4, RooAbsArg& var5) :
  _name(name), THashList(), _isCopy(kFALSE)
{
  add(var1); add(var2); add(var3); add(var4); add(var5);
}

RooArgSet::RooArgSet(const char *name, RooAbsArg& var1,
		       RooAbsArg& var2, RooAbsArg& var3,
		       RooAbsArg& var4, RooAbsArg& var5, RooAbsArg& var6) :
  _name(name), THashList(), _isCopy(kFALSE)
{
  add(var1); add(var2); add(var3); add(var4); add(var5); add(var6);
}

RooArgSet::RooArgSet(const char *name, RooAbsArg& var1,
		       RooAbsArg& var2, RooAbsArg& var3,
		       RooAbsArg& var4, RooAbsArg& var5, 
                       RooAbsArg& var6, RooAbsArg& var7) :
  _name(name), THashList(), _isCopy(kFALSE)
{
  add(var1); add(var2); add(var3); add(var4); add(var5); add(var6); add(var7) ;
}

RooArgSet::RooArgSet(const char *name, RooAbsArg& var1,
		       RooAbsArg& var2, RooAbsArg& var3,
		       RooAbsArg& var4, RooAbsArg& var5, 
                       RooAbsArg& var6, RooAbsArg& var7, RooAbsArg& var8) :
  _name(name), THashList(), _isCopy(kFALSE)
{
  add(var1); add(var2); add(var3); add(var4); add(var5); add(var6); add(var7) ;add(var8) ;
}

RooArgSet::RooArgSet(const char *name, const RooArgSet& other) :
  _name(name), THashList(), _isCopy(kTRUE)
{
  TIterator *iterator= other.MakeIterator();
  RooAbsArg *orig(0);
  while(0 != (orig= (RooAbsArg*)iterator->Next())) {
    RooAbsArg *copy= (RooAbsArg*)orig->Clone();
    Add(copy);
  }
  delete iterator;

  iterator = MakeIterator() ;
  while (orig = (RooAbsArg*)iterator->Next()) {
    orig->redirectServers(*this) ;
  }

}

RooArgSet::~RooArgSet() 
{
  // delete all variables in our list if we own them
  if(_isCopy) Delete();
}

RooArgSet* RooArgSet::snapshot() {
  // Take a snap shot: clone current list and recursively add
  // all its external dependents

  // First clone current list and contents
  RooArgSet* snapshot = new RooArgSet(TString("Snapshot of ").Append(GetName()),*this) ;

  // Add external dependents
  TIterator* vIter = snapshot->MakeIterator() ;
  RooAbsArg* var ;

  // Recursively add clones of all servers
  while (var=(RooAbsArg*)vIter->Next()) {
    snapshot->addServerClonesToList(*var) ;
  }

  // Redirect all server connections to internal list members
  vIter->Reset() ;
  while (var=(RooAbsArg*)vIter->Next()) {
    var->redirectServers(*snapshot,kTRUE) ;
  }
  delete vIter ;
  return snapshot ;
}


void RooArgSet::addServerClonesToList(RooAbsArg& var)
{
  // Add clones of servers of given argument to list

  TIterator* sIter = var.serverIterator() ;
  RooAbsArg* server ;
  while (server=(RooAbsArg*)sIter->Next()) {
    if (!find(server->GetName())) {
      RooAbsArg* serverClone = (RooAbsArg*)server->Clone() ;
      serverClone->setAttribute("SnapShot_ExtRefClone") ;
      Add(serverClone) ;      
      addServerClonesToList(*server) ;
    }
  }
  delete sIter ;
}

RooArgSet &RooArgSet::operator=(const RooArgSet& other) {
  // Assignment operator
  
  Int_t index(GetSize());
  while(--index >= 0) {
    RooAbsArg* mine= (RooAbsArg*)At(index);
    RooAbsArg* theirs= other.find(mine->GetName());
    if(!theirs) continue;
    *mine = *theirs ;
  }
  return *this;
}


Bool_t RooArgSet::add(RooAbsArg& var) {
  // Add argument to list

  const char *name= var.GetName();
  // check that this isn't a copy of a list
  if(_isCopy) {
    cout << "RooArgSet(" << _name << "): cannot add variables to a copied list" << endl;
    return kFALSE;
  }

  // is this variable name already in this list?
  RooAbsArg *other(0);
  if(other= find(name)) {
    if(other != &var) {
      // print a warning if this variable is not the same one we
      // already have
      cout << "RooArgSet(" << _name << "): cannot add second variable \"" << name
	   << "\"" << endl;
    }
    // don't add duplicates
    return kFALSE;
  }
  // add a pointer to this variable to our list (we don't own it!)
  Add(&var);
  return kTRUE;
}


Bool_t RooArgSet::remove(RooAbsArg& var) {
  // Remove argument from list

  // check that this isn't a copy of a list
  if(_isCopy) {
    cout << "RooArgSet: cannot remove variables in a copied list" << endl;
    return kFALSE;
  }
  // is var already in this list?
  const char *name= var.GetName();
  RooAbsArg *other= find(name);
  if(other != &var) {
    cout << "RooArgList: variable \"" << name << "\" is not in the list"
	 << " and cannot be removed" << endl;
    return kFALSE;
  }
  Remove(&var);
  return kTRUE;
}




RooAbsArg *RooArgSet::find(const char *name) const {
  // Find object with given name in list
  return (RooAbsArg*)FindObject(name);
}


void RooArgSet::print(RooAbsArg::PrintOption opt) 
{
  printToStream(cout,opt) ;
}


Bool_t RooArgSet::readFromStream(istream& is, Bool_t compact, Bool_t verbose) 
{
  if (compact) {

    TIterator *iterator= MakeIterator();
    RooAbsArg *next(0);
    while(0 != (next= (RooAbsArg*)iterator->Next())) {
      next->readFromStream(is,kTRUE,verbose) ;
    }
    delete iterator;    

  } else {

    RooStreamParser parser(is) ;
    TString token ;
    while (1) {
      token = parser.readToken() ;
      if (token.IsNull()) break ;
      RooAbsArg *arg ;
      if (arg = find(token)) {
	if (parser.expectToken("=",kTRUE)) {
	  parser.zapToEnd() ;
	  continue ;
	}
	arg->readFromStream(is,kFALSE,verbose) ;	
      } else {
	cout << "RooArgSet::readFromStream(" << GetName() << "): argument " 
	     << token << " not in list, ignored" << endl ;
	parser.zapToEnd() ;
      }
    }
  }
}



void RooArgSet::writeToStream(ostream& os, Bool_t compact) 
{
  TIterator *iterator= MakeIterator();
  RooAbsArg *next(0);
  while(0 != (next= (RooAbsArg*)iterator->Next())) {
    if (compact) {
      next->writeToStream(os,kTRUE) ;
      os << " " ;
    } else  {
      os << next->GetName() << " = " ;
      next->writeToStream(os,kFALSE) ;
      os << endl ;
    }
  }
  delete iterator;  
  if (compact) os << endl ;
}



void RooArgSet::printToStream(ostream& os, RooAbsArg::PrintOption opt) {
  // Print content of list
  os << "RooArgSet \"" << _name << "\":" << endl;
  TIterator *iterator= MakeIterator();
  int index= 0;
  RooAbsArg *next(0);
  while(0 != (next= (RooAbsArg*)iterator->Next())) {
    os << " (" << ++index << ") ";
    next->print(opt);
  }
  delete iterator;
}


