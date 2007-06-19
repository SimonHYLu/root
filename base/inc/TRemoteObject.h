// @(#)root/base:$Name:  $:$Id: TFile.h,v 1.64 2007/05/04 15:14:11 rdm Exp $
// Author: Bertrand Bellenot   19/06/2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
#ifndef ROOT_TObjectRemote
#define ROOT_TObjectRemote


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRemoteObject                                                        //
//                                                                      //
// Describes a Remote Object for the browser.                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TSystemDirectory
#include "TSystemDirectory.h"
#endif

#ifndef ROOT_TList
#include "TList.h"
#endif

#ifndef ROOT_TSystem
#include "TSystem.h"
#endif

class TRemoteObject : public TNamed {

protected:
   FileStat_t  fFileStat;        // file status
   Bool_t      fIsFolder;        // is folder flag
   Long64_t    fRemoteAddress;   // remote address
   TString     fClassName;       // real object class name
   TString     fKeyObjectName;   // key object name
   TString     fKeyClassName;    // key object class name

public:
   TRemoteObject();
   TRemoteObject(const char *name, const char *title, const char *classname);

   virtual ~TRemoteObject();

   virtual void            Browse(TBrowser *b);
   Bool_t                  IsFolder() const { return fIsFolder; }
   TList                  *Browse();
   Bool_t                  GetFileStat(FileStat_t *sbuf);
   const char             *GetClassName() const { return fClassName.Data(); }
   const char             *GetKeyObjectName() const { return fKeyObjectName.Data(); }
   const char             *GetKeyClassName() const { return fKeyClassName.Data(); }
   void                    SetFolder(Bool_t isFolder) { fIsFolder = isFolder; }
   void                    SetKeyObjectName(const char *name) { fKeyObjectName = name; }
   void                    SetKeyClassName(const char *name) { fKeyClassName = name; }
   void                    SetRemoteAddress(Long_t addr) { fRemoteAddress = addr; }

   ClassDef(TRemoteObject,0)  //A remote object
};

#endif

