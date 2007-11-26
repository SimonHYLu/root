// @(#)root/eve:$Id$
// Authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TEveElementEditor
#define ROOT_TEveElementEditor

#include <TGedFrame.h>

class TGCheckButton;
class TGNumberEntry;
class TGColorSelect;

class TEveElement;
class TEveTransSubEditor;

class TEveElementEditor : public TGedFrame
{
   TEveElementEditor(const TEveElementEditor&);            // Not implemented
   TEveElementEditor& operator=(const TEveElementEditor&); // Not implemented

protected:
   TEveElement         *fRE; // Model object.

   TGHorizontalFrame   *fHFrame;
   TGCheckButton       *fRnrSelf;
   TGCheckButton       *fRnrChildren;
   TGColorSelect       *fMainColor;
   TGNumberEntry       *fTransparency;
   TEveTransSubEditor  *fHMTrans;

public:
   TEveElementEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                     UInt_t options=kChildFrame, Pixel_t back=GetDefaultFrameBackground());
   ~TEveElementEditor();

   virtual void SetModel(TObject* obj);

   void DoRnrSelf();
   void DoRnrChildren();
   void DoMainColor(Pixel_t color);
   void DoTransparency();

   ClassDef(TEveElementEditor, 1); // Editor for TEveElement class.
};

#endif
