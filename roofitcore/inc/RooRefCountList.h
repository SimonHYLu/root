/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooRefCountList.rdl,v 1.3 2002/09/17 06:39:34 verkerke Exp $
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
#ifndef ROO_REF_COUNT_LIST
#define ROO_REF_COUNT_LIST

#include "RooFitCore/RooLinkedList.hh"

class RooRefCountList : public RooLinkedList {
public:
  inline RooRefCountList() : RooLinkedList() { }
  virtual ~RooRefCountList() {} ;

  virtual void Add(TObject* arg) { Add(arg,1) ; }
  virtual void Add(TObject* obj, Int_t count) ;
  virtual Bool_t Remove(TObject* obj) ;
  virtual Bool_t RemoveAll(TObject* obj) ;
  Int_t refCount(TObject* obj) ;
  
protected:  
  ClassDef(RooRefCountList,1) // RooLinkedList with reference counting
};

#endif
