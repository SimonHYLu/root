/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id$                                                             *
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

// -- CLASS DESCRIPTION [AUX] ---
// RooLinkedList is an collection class for internal use, storing
// a collection of RooAbsArg pointers in a doubly linked list
// Use RooAbsCollection derived objects for public use
// (RooArgSet and RooArgList) 

#include "RooFitCore/RooLinkedList.hh"
#include "RooFitCore/RooLinkedListIter.hh"
#include "RooFitCore/RooHashTable.hh"

ClassImp(RooLinkedList)
;

TIterator* RooLinkedList::MakeIterator(Bool_t dir) const {
  // Return an iterator over this list
  return new RooLinkedListIter(this,dir) ;
}


void RooLinkedList::Sort(Bool_t ascend) 
{
  // Sort elements of this list according to their
  // RooAbsArg::Compare() ranking via a simple
  // bubble sort algorithm

  if (_size<2) return ;

  Bool_t working(kTRUE) ;
  while(working) {
    working = kFALSE ;
    RooLinkedListElem* ptr = _first;
    while(ptr && ptr->_next) {    
      if ((ascend && ptr->_arg->Compare(ptr->_next->_arg)>0) ||
	  (!ascend && ptr->_arg->Compare(ptr->_next->_arg)<0)) {
	swapWithNext(ptr) ;
	working = kTRUE ;
      }
      ptr = ptr->_next ;
    }
  }

  return ;
}


void RooLinkedList::swapWithNext(RooLinkedListElem* elemB) 
{
  // Swap given to elements in the linked list. Auxiliary function for Sort()
  RooLinkedListElem* elemA = elemB->_prev ;
  RooLinkedListElem* elemC = elemB->_next ;
  RooLinkedListElem* elemD = elemC->_next ;
  if (!elemC) return ;

  if (elemA) {
    elemA->_next = elemC ;
  } else {
    _first = elemC ;
  }
  elemB->_prev = elemC ;
  elemB->_next = elemD ;
  elemC->_prev = elemA ;
  elemC->_next = elemB ;
  if (elemD) {
    elemD->_prev = elemB ;
  } else {
    _last = elemB ;
  }
  return ;
}


void RooLinkedList::Streamer(TBuffer &b)
{
  if (b.IsReading()) {
    Version_t v = b.ReadVersion();
    TObject::Streamer(b);

    Int_t size ;
    RooAbsArg* arg ;

    b >> size ;
    while(size--) {
      b >> arg ;
      Add(arg) ;      
    }

  } else {
    b.WriteVersion(RooLinkedList::IsA());
    TObject::Streamer(b);
    b << _size ;

    RooLinkedListElem* ptr = _first;
    while(ptr) {
      b << ptr->_arg ;
      ptr = ptr->_next ;
    } 
  }
}

