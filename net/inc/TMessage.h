// @(#)root/net:$Name:  $:$Id: TMessage.h,v 1.5 2002/03/19 10:57:24 brun Exp $
// Author: Fons Rademakers   19/12/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TMessage
#define ROOT_TMessage


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMessage                                                             //
//                                                                      //
// Message buffer class used for serializing objects and sending them   //
// over the network.                                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TBuffer
#include "TBuffer.h"
#endif
#ifndef ROOT_MessageTypes
#include "MessageTypes.h"
#endif


class TMessage : public TBuffer {

friend class TSocket;
friend class TPSocket;

private:
   UInt_t   fWhat;   //message type
   TClass  *fClass;  //if message is kMESS_OBJECT, pointer to object's class

   // TMessage objects cannot be copied or assigned
   TMessage(const TMessage &);           // not implemented
   void operator=(const TMessage &);     // not implemented

protected:
   TMessage(void *buf, Int_t bufsize);   // only called by T(P)Socket::Recv()
   void SetLength() const;               // only called by T(P)Socket::Send()

public:
   TMessage(UInt_t what = kMESS_ANY);
   virtual ~TMessage() { }

   void     Forward();
   TClass  *GetClass() const { return fClass; }
   void     Reset();
   void     Reset(UInt_t what) { SetWhat(what); Reset(); }
   UInt_t   What() const { return fWhat; }
   void     SetWhat(UInt_t what);

   ClassDef(TMessage,0)  // Message buffer class
};

#endif
