// @(#)root/treeplayer:$Name:  $:$Id: TSelector.h,v 1.7 2002/01/15 00:45:21 rdm Exp $
// Author: Rene Brun   05/02/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TSelector
#define ROOT_TSelector


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSelector                                                            //
//                                                                      //
// A utility class for Trees selections.                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef ROOT_TObject
#include "TObject.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif

class TTree;

class TSelector : public TObject {
protected:
   TString   fOption;  //option given to TTree::Process
   TObject  *fObject;  //current oject if processing object (vs. TTree)
   TList    *fInput;   //list of objects available during processing (on PROOF)
   TList    *fOutput;  //list of objects created during processing (on PROOF)

public:
   TSelector();
   virtual            ~TSelector();
   virtual void        Begin(TTree *) { }
   virtual Bool_t      Notify() { return kTRUE; }
   virtual const char *GetOption() const { return fOption; }
   virtual Bool_t      ProcessCut(Int_t entry) { return kTRUE; }
   virtual void        ProcessFill(Int_t entry) { }
   virtual Bool_t      Process() { return kFALSE; }
   virtual void        SetOption(const char *option) { fOption = option; }
   virtual void        SetObject(TObject *obj) { fObject = obj; }
   virtual void        SetInputList(TList *input) { fInput = input; }
   virtual TList      *GetOutputList() const { return fOutput; }
   virtual void        Terminate() { }

   static  TSelector  *GetSelector(const char *filename);

   ClassDef(TSelector,0)  //A utility class for tree and object processing
};

#endif

