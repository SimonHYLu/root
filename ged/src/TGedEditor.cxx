// @(#)root/ged:$Name:  $:$Id: TGedEditor.cxx,v 1.11 2004/09/21 17:53:10 brun Exp $
// Author: Marek Biskup, Ilka Antcheva 02/08/2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGedEditor                                                           //
//                                                                      //
// Editor is a composite frame that contains TGedToolBox and            //
// TGedAttFrames. It is connected to a Canvas and listens for           //
// selected objects                                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TGedEditor.h"
#include "TCanvas.h"
#include "TGTab.h"
#include "TGedFrame.h"
#include "TGLabel.h"
#include "TGFrame.h"
#include "TBaseClass.h"
#include "TSystem.h"

ClassImp(TGedFrame)
ClassImp(TGedEditor)

//______________________________________________________________________________
TGedEditor::TGedEditor(TCanvas* canvas) :
   TGMainFrame(gClient->GetRoot(), 110, 20)
{

   fTab = new TGTab(this, 110, 30);
   AddFrame(fTab, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 2, 2));
   fTabContainer = fTab->AddTab("Style");
   fStyle = new TGCompositeFrame(fTabContainer, 110, 30, kVerticalFrame);
   fStyle->AddFrame(new TGedNameFrame(fStyle, 1),\
                    new TGLayoutHints(kLHintsTop | kLHintsExpandX,0, 0, 2, 2));
   fTabContainer->AddFrame(fStyle, new TGLayoutHints(kLHintsTop | kLHintsExpandX,\
                                                     0, 0, 2, 2));
   fWid = 1;

   if (canvas) {
      if (!canvas->GetSelected())
         canvas->SetSelected(canvas);
      if (!canvas->GetSelectedPad())
         canvas->SetSelectedPad(canvas);
      fModel  = canvas->GetSelected();
      fPad    = canvas->GetSelectedPad();
      fCanvas = canvas;
      fClass  = fModel->IsA();
      GetEditors();
      ConnectToCanvas(canvas);
      SetWindowName(Form("%s", canvas->GetName()));
   } else {
      fModel  = 0;
      fPad    = 0;
      fCanvas = 0;
      fClass  = 0;
   }

   MapSubwindows();
   Resize(GetDefaultSize());
   MapWindow();
   if (canvas) Resize(GetWidth(), canvas->GetWh());
   gROOT->GetListOfCleanups()->Add(this);
}

//______________________________________________________________________________
void TGedEditor::CloseWindow()
{
   // When closed via WM close button, just unmap (i.e. hide) editor
   // for later use.

   Hide();
   gROOT->GetListOfCleanups()->Remove(this);
}

//______________________________________________________________________________
void TGedEditor::GetEditors()
{
   // Get existing editors of selected object

   // Look in TClass::GetEditorList() for any object deriving from TGedFrame,
   Bool_t found = kFALSE;
   TGedElement *ge;
   TList *list = fModel->IsA()->GetEditorList();
   if (list->First() != 0) {

      TIter next1(list);
      while ((ge = (TGedElement *) next1())) {
         // check if the editor ge->fGedframe is already in the list of fStyle
         TList *l = fStyle->GetList();
         if (l->First() != 0) {
            TGFrameElement *fr;
            TIter next(l);
            while ((fr = (TGFrameElement *) next())) {
               TGedFrame *f = ge->fGedFrame;
               found = fr->fFrame->InheritsFrom(f->ClassName());
               if (found) break;
            }
         }
         if (found == kFALSE)
            fStyle->AddFrame(ge->fGedFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX,\
                                               0, 0, 2, 2));
      }
   } else {

      //search for a class editor = classname + 'Editor'
       GetClassEditor(fModel->IsA());

      //now scan all base classes
      list = fModel->IsA()->GetListOfBases();
      if (list->First() != 0) GetBaseClassEditor(fModel->IsA());
   }

   fStyle->Layout();
   fStyle->MapSubwindows();
}

//______________________________________________________________________________
void TGedEditor::GetBaseClassEditor(TClass *cl)
{
   // Scan the base classes of cl and add attribute editors to the list.

   TList *list = cl->GetListOfBases();
   if (list->First() == 0) return;

   TBaseClass *base;
   TIter next(list);

   while ((base = (TBaseClass *)next())) {

      TClass *c1;
      if ((c1 = base->GetClassPointer())) GetClassEditor(c1);

      if (c1->GetListOfBases()->First() == 0) continue;
      else GetBaseClassEditor(c1);
   }
}

//______________________________________________________________________________
void TGedEditor::GetClassEditor(TClass *cl)
{
   // Add attribute editor of class cl to the list of fStyle frame.

   TClass *class2, *class3;
   Bool_t found = kFALSE;
   class2 = gROOT->GetClass(Form("%sEditor",cl->GetName()));
   if (class2 && class2->InheritsFrom("TGedFrame")) {
      TList *list = fStyle->GetList();
      if (list->First() != 0) {
         TGFrameElement *fr;
         TIter next(list);
         while ((fr = (TGFrameElement *) next())) {;
            found = fr->fFrame->InheritsFrom(class2);
            if (found) break;
         }
      }
      if (found == kFALSE) {
         gROOT->ProcessLine(Form("((TGCompositeFrame *)0x%lx)->AddFrame(new %s((TGWindow *)0x%lx, %d),\
                                 new TGLayoutHints(kLHintsTop | kLHintsExpandX,0, 0, 2, 2))",\
                                 (Long_t)fStyle, class2->GetName(), (Long_t)fStyle, fWid));
         fWid++;
         class3 = (TClass*)gROOT->GetListOfClasses()->FindObject(cl->GetName());
         TGedElement *ge;
         TIter next3(class3->GetEditorList());
         while ((ge = (TGedElement *)next3())) {
            if (!strcmp(ge->fGedFrame->ClassName(), class2->GetName()) && (ge->fCanvas == 0)) {
               ge->fCanvas = fCanvas;
            }
         }
      }
   }
}

//______________________________________________________________________________
void TGedEditor::ConnectToCanvas(TCanvas *c)
{

   TQObject::Connect(c, "Selected(TVirtualPad*,TObject*,Int_t)", "TGedEditor",
                     this, "SetModel(TVirtualPad*,TObject*,Int_t)");
   c->Selected(c->GetSelectedPad(), c->GetSelected(), kButton1Down);
}

//______________________________________________________________________________
void TGedEditor::SetModel(TVirtualPad* pad, TObject* obj, Int_t event)
{

   if (event != kButton1Down) return;

   fModel = obj;
   fPad   = pad;

   if ((obj != 0) && (obj->IsA() != fClass) && !obj->IsA()->InheritsFrom(fClass)) {
      fClass = obj->IsA();
      GetEditors();
   } else if ((obj == 0) && fPad) {
      TCanvas *canvas = fPad->GetCanvas();
      if (canvas) {
         fPad->SetSelected(fPad);
         canvas->Selected(fPad, fPad, 0);
         return;
      } else {
         DeleteEditors();
         DeleteWindow();
         return;
      }
   }

   TGFrameElement *el;
   TIter next(fStyle->GetList());
   while ((el = (TGFrameElement *) next())) {
      if ((el->fFrame)->InheritsFrom("TGedFrame"))
         ((TGedFrame *)(el->fFrame))->SetModel(fPad, fModel, event);
   }
}

//______________________________________________________________________________
void TGedEditor::Show()
{
   // Show editor.

   MapWindow();
   if (!gROOT->GetListOfCleanups()->FindObject(this))
      gROOT->GetListOfCleanups()->Add(this);
}

//______________________________________________________________________________
void TGedEditor::DeleteEditors()
{
   // Delete GUI editors connected to the canvas fCanvas.

   TClass * cl;
   TIter next(gROOT->GetListOfClasses());
   while((cl = (TClass *)next())) {
      if (cl->GetEditorList()->First() != 0) {
         TList *editors = cl->GetEditorList();
         TIter next1(editors);
         TGedElement *ge;
         while ((ge = (TGedElement *)next1())) {
            if (ge->fCanvas == fCanvas) {
               editors->Remove(ge);
            }
         }
      }
   }
}
//______________________________________________________________________________
void TGedEditor::Hide()
{
   // Hide editor.

   gROOT->GetListOfCleanups()->Remove(this);
   UnmapWindow();
}

//______________________________________________________________________________
TGedEditor::~TGedEditor()
{
  // Editor destructor.

  gROOT->GetListOfCleanups()->Remove(this);
  fStyle->Cleanup();
  Cleanup();
}

//______________________________________________________________________________
 void TGedEditor::RecursiveRemove(TObject* obj)
{
   // Remove references to fModel in case the fModel is being deleted
   // Deactivate attribute frames if they point to obj


   if (fModel != obj) return;
   if (obj == fPad) 
      SetModel(fCanvas, fCanvas, 0);
   else
      SetModel(fPad, fPad, 0);
}
