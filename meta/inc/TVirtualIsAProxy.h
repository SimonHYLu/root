// @(#)root/meta:$Name:  $:$Id: TVirtualIsAProxy.h,v 1.2 2005/06/08 18:51:36 rdm Exp $
// Author: Markus Frank 20/05/2005

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TVirtualIsAProxy
#define ROOT_TVirtualIsAProxy

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TClass                                                               //
//                                                                      //
// Virtual IsAProxy base class.                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TClass;

class TVirtualIsAProxy  {
public:
   virtual ~TVirtualIsAProxy() { }
   virtual void SetClass(TClass *cl) = 0;
   virtual TClass* operator()(const void *obj) = 0;
};

#endif // ROOT_TVirtualIsAProxy
