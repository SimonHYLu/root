// @(#)root/gl:$Name:  $:$Id: TViewerOpenGL.cxx,v 1.10 2004/08/16 10:00:45 brun Exp $
// Author:  Timur Pocheptsov  03/08/2004

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
#include <iostream>
#include <vector>
#include <memory>

#include <TRootHelpDialog.h>
#include <TViewerOpenGL.h>
#include <TVirtualPad.h>
#include <TVirtualGL.h>
#include <KeySymbols.h>
#include <TVirtualX.h>
#include <TBuffer3D.h>
#include <TGLKernel.h>
#include <TGClient.h>
#include <TGCanvas.h>
#include <Buttons.h>
#include <TAtt3D.h>
#include <TGMenu.h>
#include <TROOT.h>
#include <TMath.h>
#include <HelpText.h>
#include <TAttMarker.h>

#include "TArcBall.h"

static Float_t colors[] = {
   92.f / 255, 92.f / 255, 92.f / 255,
   122.f / 255, 122.f / 255, 122.f / 255,
   184.f / 255, 184.f / 255, 184.f / 255,
   215.f / 255, 215.f / 255, 215.f / 255,
   138.f / 255, 15.f / 255, 15.f / 255,
   184.f / 255, 20.f / 255, 20.f / 255,
   235.f / 255, 71.f / 255, 71.f / 255,
   240.f / 255, 117.f / 255, 117.f / 255,
   15.f / 255, 138.f / 255, 15.f / 255,
   20.f / 255, 184.f / 255, 20.f / 255,
   71.f / 255, 235.f / 255, 71.f / 255,
   117.f / 255, 240.f / 255, 117.f / 255,
   15.f / 255, 15.f / 255, 138.f / 255,
   20.f / 255, 20.f / 255, 184.f / 255,
   71.f / 255, 71.f / 255, 235.f / 255,
   117.f / 255, 117.f / 255, 240.f / 255,
   138.f / 255, 138.f / 255, 15.f / 255,
   184.f / 255, 184.f / 255, 20.f / 255,
   235.f / 255, 235.f / 255, 71.f / 255,
   240.f / 255, 240.f / 255, 117.f / 255,
   138.f / 255, 15.f / 255, 138.f / 255,
   184.f / 255, 20.f / 255, 184.f / 255,
   235.f / 255, 71.f / 255, 235.f / 255,
   240.f / 255, 117.f / 255, 240.f / 255,
   15.f / 255, 138.f / 255, 138.f / 255,
   20.f / 255, 184.f / 255, 184.f / 255,
   71.f / 255, 235.f / 255, 235.f / 255,
   117.f / 255, 240.f / 255, 240.f / 255
};

static const char gHelpViewerOpenGL[] = "\
     PRESS \n\
     \tw\t--- wireframe mode\n\
     \tr\t--- hidden surface mode\n\
     \tj\t--- zoom in\n\
     \tk\t--- zoom out\n\
     HOLD the left mouse button and MOVE mouse to ROTATE object\n\n";

enum EGLViewerCommands {
   kGLHelpAbout,
   kGLHelpOnViewer
};

class TGLPrimitive : public TObject {
public:
   UInt_t fName;
   TGLPrimitive(UInt_t name):fName(name){}
   typedef TGLPrimitive Base;
   virtual void GLDraw()const = 0;
};

class TGLFaceSet : public TGLPrimitive {
private:
   std::vector<Double_t> fPnts;
   std::vector<Double_t> fNormals;
   std::vector<Int_t>    fPols;

   Int_t fNbPols;
   Int_t fColorInd;
   
   //non copyable class
   TGLFaceSet(const TGLFaceSet &);
   TGLFaceSet & operator = (const TGLFaceSet &);

   Int_t CheckPoints(const Int_t * source, Int_t * dest)const;
   static Bool_t Eq(const Double_t * p1, const Double_t * p2)
    {
      return *p1 == *p2 && p1[1] == p2[1] && p1[2] == p2[2];
    }
public:
   TGLFaceSet(const TBuffer3D & buf_initializer, UInt_t name);
   void GLDraw()const;
};

//______________________________________________________________________________
TGLFaceSet::TGLFaceSet(const TBuffer3D & init_buf, UInt_t name)
               :Base(name), fPnts(init_buf.fPnts, init_buf.fPnts + init_buf.fNbPnts * 3),
                fNormals(init_buf.fNbPols * 3), fNbPols(init_buf.fNbPols),
                fColorInd(init_buf.fSegs[0]) 
{
   Int_t * segs = init_buf.fSegs;
   Int_t * pols = init_buf.fPols;
   Double_t * pnts = init_buf.fPnts;

   for (Int_t num_pol = 0, e = init_buf.fNbPols, j = 0; num_pol < e; ++num_pol) {
      ++j;
      Int_t segment_ind = pols[j] + j;
      Int_t segment_col = pols[j];
      Int_t seg1 = pols[segment_ind--];
      Int_t seg2 = pols[segment_ind--];
      Int_t np[] = {segs[seg1 * 3 + 1], segs[seg1 * 3 + 2], segs[seg2 * 3 + 1], segs[seg2 * 3 + 2]};
      Int_t n[] = {-1, -1, -1};
      Int_t normp[] = {0, 0, 0};

      np[0] != np[2] ?
               (np[0] != np[3] ?
                  (*n = *np, n[1] = np[1] == np[2] ?
                     n[2] = np[3], np[2] :
                        (n[2] = np[2], np[3])) :
                           (*n = np[1], n[1] = *np, n[2] = np[2] )) :
                              (*n = np[1], n[1] = *np, n[2] = np[3]);
      fPols.push_back(3);
      Int_t pol_size_ind = fPols.size() - 1;
      fPols.insert(fPols.end(), n, n + 3);
      Int_t check = CheckPoints(n, normp), ngood = check;

      if (check == 3)
         TMath::Normal2Plane(pnts + n[0] * 3, pnts + n[1] * 3, pnts + n[2] * 3, &fNormals[num_pol * 3]);

      while(segment_ind > j + 1) {
         seg2 = pols[segment_ind];
         np[0] = segs[seg2 * 3 + 1];
         np[1] = segs[seg2 * 3 + 2];
         if (np[0] == n[2]) {
            fPols.push_back(np[1]);
            if(check != 3)
               normp[ngood ++] = np[1];
         } else {
             fPols.push_back(np[0]);
             if (check != 3)
                normp[ngood ++] = np[0];
         }

         if(check != 3 && ngood == 3) {
            check = CheckPoints(normp, normp);

            if(check == 3)
               TMath::Normal2Plane(
                                   pnts + normp[0] * 3, pnts + normp[1] * 3,
                                   pnts + normp[2] * 3, &fNormals[num_pol * 3]
                                  );
            ngood = check;
         }
         ++fPols[pol_size_ind];
         --segment_ind;
      }
      j += segment_col + 1;
   }
}

//______________________________________________________________________________
Int_t TGLFaceSet::CheckPoints(const Int_t * source, Int_t * dest) const
{
   const Double_t * p1 = &fPnts[source[0] * 3];
   const Double_t * p2 = &fPnts[source[1] * 3];
   const Double_t * p3 = &fPnts[source[2] * 3];
   Int_t ret_val = 1;

   !Eq(p1, p2) ?
      !Eq(p1, p3) ?
         !Eq(p2, p3) ?
            ret_val = 3 :
               (ret_val = 2, *dest = *source, dest[1] = source[1]) :
                  (ret_val = 2, *dest = *source, dest[1] = source[1]) :
                     !Eq(p2, p3) ?
                        ret_val = 2, *dest = *source, dest[1] = source[2] :
                           *dest = *source;

   return ret_val;
}

//______________________________________________________________________________
void TGLFaceSet::GLDraw()const
{
   gVirtualGL->DrawFaceSet(&fPnts[0], &fPols[0], &fNormals[0], colors + fColorInd * 3, fNbPols);
}

class TGLPolyLine : public TGLPrimitive {
private:
   std::vector<Double_t>fVertices;

   TGLPolyLine(const TGLPolyLine &);
   TGLPolyLine & operator = (const TGLPolyLine &);
public:
   TGLPolyLine(const TBuffer3D & init_buffer, UInt_t name);
   void GLDraw()const;
};

//______________________________________________________________________________
TGLPolyLine::TGLPolyLine(const TBuffer3D & ib, UInt_t name)
                  :Base(name), fVertices(ib.fPnts, ib.fPnts + ib.fNbPnts * 3)
{
}

//______________________________________________________________________________
void TGLPolyLine::GLDraw()const
{
   //gVirtualGL//color
   gVirtualGL->PaintPolyLine(fVertices.size() / 3, const_cast<Double_t *>(&fVertices[0]),0);
}

class TGLPolyMarker : public TGLPrimitive {
private:
   Int_t fColorInd;
   Style_t fStyle;
   std::vector<Double_t>fVertices;
public:
   TGLPolyMarker(const TBuffer3D & init_buf, UInt_t name);
   void GLDraw()const;
};

//______________________________________________________________________________
TGLPolyMarker::TGLPolyMarker(const TBuffer3D & init_buf, UInt_t name)
                    :Base(name),fColorInd(init_buf.fSegs[0]),fStyle(7),
					      fVertices(init_buf.fPnts, init_buf.fPnts + 3 * init_buf.fNbPnts)
{
   //TAttMarker is not TObject descendant, so I need dynamic_cast
   if (const TAttMarker * ptr = dynamic_cast<const TAttMarker *>(init_buf.fId))
      fStyle = ptr->GetMarkerStyle();	
}

//______________________________________________________________________________
void TGLPolyMarker::GLDraw()const
{
   gVirtualGL->MaterialGL(kFRONT, colors + fColorInd * 3);
   gVirtualGL->PaintPolyMarker(&fVertices[0], fStyle, fVertices.size());
}

class TGLSelectionBox : public TObject {
private:
   typedef std::pair<Double_t, Double_t>PDD_t;
   PDD_t fXrange;
   PDD_t fYrange;
   PDD_t fZrange;
public:
   TObject * fBackPtr;
   TGLSelectionBox(const PDD_t & x_range, const PDD_t & y_range, const PDD_t & z_range, TObject * bp);
   void GLDraw()const;
private:
   TGLSelectionBox(const TGLSelectionBox &);
   TGLSelectionBox & operator = (const TGLSelectionBox &);
};

//______________________________________________________________________________
TGLSelectionBox::TGLSelectionBox(const PDD_t & x_range, const PDD_t & y_range, 
                                 const PDD_t & z_range, TObject * backPtr)
                     :fXrange(x_range), fYrange(y_range), 
                      fZrange(z_range), fBackPtr(backPtr)
{
}

//______________________________________________________________________________
void TGLSelectionBox::GLDraw()const
{
   Float_t material[] = {1., 1., 1., 1.};
   gVirtualGL->MaterialGL(kFRONT, material);
   gVirtualGL->MaterialGL(kFRONT, 80.);
   gVirtualGL->DrawSelectionBox(fXrange.first, fXrange.second, fYrange.first, fYrange.second, fZrange.first, fZrange.second);
}

class TGLWidget : public TGCompositeFrame {
private:
   TViewerOpenGL  *fViewer;
public:
   TGLWidget(TViewerOpenGL * c, Window_t id, const TGWindow * parent);

   Bool_t  HandleButton(Event_t * event)
   {
      return fViewer->HandleContainerButton(event);
   }
   Bool_t  HandleConfigureNotify(Event_t * event)
   {
      TGFrame::HandleConfigureNotify(event);
      return fViewer->HandleContainerConfigure(event);
   }
   Bool_t  HandleKey(Event_t * event)
   {
      return fViewer->HandleContainerKey(event);
   }
   Bool_t  HandleMotion(Event_t * event)
   {
      return fViewer->HandleContainerMotion(event);
   }
   Bool_t  HandleExpose(Event_t * event)
   {
      return fViewer->HandleContainerExpose(event);
   }
};

//______________________________________________________________________________
TGLWidget::TGLWidget(TViewerOpenGL * c, Window_t id, const TGWindow *p)
               :TGCompositeFrame(gClient, id, p)
{
   fViewer = c;
   gVirtualX->GrabButton(fId, kAnyButton, kAnyModifier, kButtonPressMask | kButtonReleaseMask, kNone, kNone);
   gVirtualX->SelectInput(fId, kKeyPressMask | kExposureMask | kPointerMotionMask | kStructureNotifyMask);
   gVirtualX->SetInputFocus(fId);
}


ClassImp(TViewerOpenGL)

//______________________________________________________________________________
TViewerOpenGL::TViewerOpenGL(TVirtualPad * vp)
                  :TVirtualViewer3D(vp), TGMainFrame(gClient->GetRoot(), 600, 600),
                   fCanvasWindow(0), fCanvasContainer(0), fCanvasLayout(0),
	                fMenuBarLayout(0), fMenuBarHelpLayout(0), fMenuBar(0), fHelpMenu(0),
                   fXc(0.), fYc(0.), fZc(0.), fRad(0.),
                   fCtx(0), fGLWin(0), fPressed(kFALSE),
                   fDList(0), fArcBall(0), fFrP(), fZoom(0.9),
	                fSelected(0), fNbShapes(0)
{
   fGLObjects.SetOwner(kTRUE);
   CreateViewer();
   Resize(600, 600);
   fArcBall = new TArcBall(600, 600);
   fGLBoxes.SetOwner(kTRUE);
   fDList = gVirtualGL->CreateGLLists(1);
   if (!fDList)   
      Error("TViewerOpenGL", "Could not create display list\n");
}

//______________________________________________________________________________
void TViewerOpenGL::CreateViewer()
{
   fHelpMenu = new TGPopupMenu(fClient->GetRoot());
   fHelpMenu->AddEntry("&About ROOT...", kGLHelpAbout);
   fHelpMenu->AddSeparator();
   fHelpMenu->AddEntry("Help on OpenGL Viewer...", kGLHelpOnViewer);
   fHelpMenu->Associate(this);
   
   // Create menubar layout hints
   fMenuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 0, 0, 1, 1);
   fMenuBarHelpLayout = new TGLayoutHints(kLHintsTop | kLHintsRight);

   // Create menubar
   fMenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
   fMenuBar->AddPopup("&Help",    fHelpMenu,    fMenuBarHelpLayout);
   AddFrame(fMenuBar, fMenuBarLayout);

   fCanvasWindow = new TGCanvas(this, GetWidth()+4, GetHeight()+4, kSunkenFrame | kDoubleBorder);
   InitGLWindow();

   fCanvasContainer = new TGLWidget(this, fGLWin, fCanvasWindow->GetViewPort());
   fCanvasWindow->SetContainer(fCanvasContainer);

   fCanvasLayout = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY);
   AddFrame(fCanvasWindow, fCanvasLayout);
   SetWindowName("OpenGL experimental viewer");
   SetClassHints("GLViewer", "GLViewer");
   SetMWMHints(kMWMDecorAll, kMWMFuncAll, kMWMInputModeless);
   MapSubwindows();
   Resize(GetDefaultSize());
   Show();
}

//______________________________________________________________________________
TViewerOpenGL::~TViewerOpenGL()
{
   DeleteContext();
   delete fHelpMenu;
   delete fMenuBar;
   delete fMenuBarLayout;
   delete fMenuBarHelpLayout;
   delete fArcBall;
   delete fCanvasContainer;
   delete fCanvasWindow;
   delete fCanvasLayout;
   //It's not dangerous to call glDeleteLists with wrong list, but I'm checking here
   if(fDList)
      gVirtualGL->DeleteGLLists(fDList, 1);
}

//______________________________________________________________________________
void TViewerOpenGL::InitGLWindow()
{
#ifdef GDK_WIN32
   new TGLKernel((TVirtualGLImp *)gROOT->ProcessLineFast("new TGWin32GL"));
#else
   new TGLKernel((TVirtualGLImp *)gROOT->ProcessLineFast("new TX11GL"));
#endif
   gVirtualGL->SetTrueColorMode();
   Window_t wind = fCanvasWindow->GetViewPort()->GetId();
   fGLWin = gVirtualGL->CreateGLWindow(wind);
   CreateContext();
   MakeCurrent();
}

//______________________________________________________________________________
void TViewerOpenGL::CreateContext()
{
   fCtx = gVirtualGL->CreateContext(fGLWin);
}

//______________________________________________________________________________
void TViewerOpenGL::DeleteContext()
{
   gVirtualGL->DeleteContext(fCtx);
}

//______________________________________________________________________________
void TViewerOpenGL::MakeCurrent()const
{
   gVirtualGL->MakeCurrent(fGLWin, fCtx);
}

//______________________________________________________________________________
void TViewerOpenGL::SwapBuffers()const
{
   gVirtualGL->SwapBuffers(fGLWin);
}

//______________________________________________________________________________
Bool_t TViewerOpenGL::HandleContainerButton(Event_t * event)
{
   if (event->fType == kButtonPress && event->fCode == kButton1) {
      TPoint pnt(event->fX, event->fY);
      fArcBall->Click(pnt);
      fPressed = kTRUE;
   } else if (event->fType == kButtonPress && event->fCode == kButton3)
      TestSelection(event);
   else if (event->fType == kButtonRelease && event->fCode == kButton1)
      fPressed = kFALSE;

   return kTRUE;
}

//______________________________________________________________________________
Bool_t TViewerOpenGL::HandleContainerConfigure(Event_t * event)
{
   gVirtualX->ResizeWindow(fGLWin, event->fWidth, event->fHeight);
   fArcBall->SetBounds(event->fWidth, event->fHeight);
   DrawObjects();
   return kTRUE;
}

//______________________________________________________________________________
Bool_t TViewerOpenGL::HandleContainerKey(Event_t * event)
{
   char tmp[10] = {0};
   UInt_t keysym = 0;
   
   gVirtualX->LookupString(event, tmp, sizeof(tmp), keysym);
   
   switch (keysym) {
   case kKey_Plus:
   case kKey_J:
   case kKey_j:
      fZoom /= 1.2;
      DrawObjects();
      break;
   case kKey_Minus:
   case kKey_K:
   case kKey_k:
      fZoom *= 1.2;
      DrawObjects();
      break;
   case kKey_R:
   case kKey_r: 
      gVirtualGL->PolygonGLMode(kFRONT, kFILL);
      gVirtualGL->EnableGL(kCULL_FACE);
      gVirtualGL->SetGLLineWidth(1.f);
      DrawObjects();
      break;
   case kKey_W:
   case kKey_w:
      gVirtualGL->DisableGL(kCULL_FACE);
      gVirtualGL->PolygonGLMode(kFRONT_AND_BACK, kLINE);
      gVirtualGL->SetGLLineWidth(1.5f);
      DrawObjects();
   } 
   
   return kTRUE;
}

//______________________________________________________________________________
Bool_t TViewerOpenGL::HandleContainerMotion(Event_t * event)
{
   if (fPressed) {
      TPoint pnt(event->fX, event->fY);
      fArcBall->Drag(pnt);
      DrawObjects();
   }

   return kTRUE;
}

//______________________________________________________________________________
Bool_t TViewerOpenGL::HandleContainerExpose(Event_t *)
{
   DrawObjects();
   return kTRUE;
}

//______________________________________________________________________________
void TViewerOpenGL::CreateScene(Option_t *)
{
   fGLObjects.Delete();
   TView * v = fPad->GetView();

   if (!v) {
      Error("CreateScene", "view not set");
      return;
   }

   TBuffer3D * buff = fPad->GetBuffer3D();
   TObjLink * lnk = fPad->GetListOfPrimitives()->FirstLink();
   buff->fOption = TBuffer3D::kOGL;
   while (lnk) {
      TObject * obj  = lnk->GetObject();
      if (obj->InheritsFrom(TAtt3D::Class()))
         obj->Paint("ogl");
      lnk = lnk->Next();
   }

   buff->fOption = TBuffer3D::kPAD;

   Double_t xdiff = fRangeX.second - fRangeX.first;
   Double_t ydiff = fRangeY.second - fRangeY.first;
   Double_t zdiff = fRangeZ.second - fRangeZ.first;
   Double_t max = xdiff > ydiff ? xdiff > zdiff ? xdiff : zdiff : ydiff > zdiff ? ydiff : zdiff;
   Float_t lmodel_amb[] = {0.5f, 0.5f, 1.f, 1.f};

   fRad = max * 1.7;
   fXc = fRangeX.first + xdiff / 2;
   fYc = fRangeY.first + ydiff / 2;
   fZc = fRangeZ.first + zdiff / 2;
   fFrP[0] = max / 1.9;
   fFrP[1] = max * 0.707;
   fFrP[2] = 3 * max;
   MakeCurrent();

   gVirtualGL->ClearGLColor(0.f, 0.f, 0.f, 1.f);
   gVirtualGL->ClearGLDepth(1.f);
   gVirtualGL->LightModel(kLIGHT_MODEL_AMBIENT, lmodel_amb);
   gVirtualGL->LightModel(kLIGHT_MODEL_TWO_SIDE, kFALSE);
   gVirtualGL->EnableGL(kLIGHTING);
   gVirtualGL->EnableGL(kLIGHT0);
   gVirtualGL->EnableGL(kLIGHT1);
   gVirtualGL->EnableGL(kLIGHT2);
   gVirtualGL->EnableGL(kLIGHT3);
   gVirtualGL->EnableGL(kDEPTH_TEST);
   gVirtualGL->EnableGL(kCULL_FACE);
   gVirtualGL->CullFaceGL(kBACK);
   gVirtualGL->PolygonGLMode(kFRONT, kFILL);
   BuildGLList();
   DrawObjects();
}

//______________________________________________________________________________
void TViewerOpenGL::UpdateScene(Option_t *)
{
   TBuffer3D * buff = fPad->GetBuffer3D();

   if (buff->fOption == buff->kOGL) {
      ++fNbShapes;
      fGLBoxes.AddLast(UpdateRange(buff));
      TGLPrimitive * add_obj = 0;
      
      switch (buff->fType) {
      case TBuffer3D::kLINE:
         add_obj = new TGLPolyLine(*buff, fNbShapes);
  	      break;
      case TBuffer3D::kMARKER:
         add_obj = new TGLPolyMarker(*buff, fNbShapes);
         break;
      default:
         add_obj = new TGLFaceSet(*buff, fNbShapes);
         break;
      }
      fGLObjects.AddLast(add_obj);
   }
}

//______________________________________________________________________________
void TViewerOpenGL::Show()
{
   MapRaised();
}

//______________________________________________________________________________
void TViewerOpenGL::CloseWindow()
{
   fPad->SetViewer3D(0);
   delete this;
}

//______________________________________________________________________________
void TViewerOpenGL::DrawObjects()const
{
   if(!fDList)
      return;
   if(!fGLObjects.GetSize())
      return;

   MakeCurrent();
   gVirtualGL->ClearGL(0);

   Int_t cx = GetWidth() / 2;
   Int_t cy = (GetHeight() - fMenuBar->GetHeight() - fMenuBarLayout->GetPadTop() - fMenuBarLayout->GetPadBottom() - fMenuBarHelpLayout->GetPadTop() - fMenuBarHelpLayout->GetPadBottom()) / 2;
   Int_t d = TMath::Min(cx, cy);
   Double_t frp = fFrP[0] * fZoom;

   gVirtualGL->ViewportGL(cx - d, cy - d, d * 2, d * 2);
   gVirtualGL->NewPRGL();
   gVirtualGL->FrustumGL(-frp, frp, -frp, frp, fFrP[1], fFrP[2]); 
   gVirtualGL->NewMVGL();
   gVirtualGL->PushGLMatrix();
   gVirtualGL->TranslateGL(0., 0., -fRad);
   gVirtualGL->MultGLMatrix(fArcBall->GetRotMatrix());
   gVirtualGL->RotateGL(-90., 1., 0., 0.);
   gVirtualGL->TranslateGL(-fXc, -fYc, -fZc);
   gVirtualGL->RunGLList(fDList);
   
   if (fSelected) {
      const TGLSelectionBox * selection_box = static_cast<const TGLSelectionBox *>(fGLBoxes.At(fSelected - 1));
	  
      selection_box->GLDraw();
   }
   
   gVirtualGL->PopGLMatrix();

   Float_t pos[] = {0.f, 0.f, 0.f, 1.f};
   Float_t lig_prop[] = {1.f, 1.f, 1.f, 1.f};

   gVirtualGL->GLLight(kLIGHT0, kPOSITION, pos);
   gVirtualGL->PushGLMatrix();
   gVirtualGL->TranslateGL(0., fRad + fYc, -fRad - fZc);
   gVirtualGL->GLLight(kLIGHT1, kPOSITION, pos);
   gVirtualGL->GLLight(kLIGHT1, kDIFFUSE, lig_prop);
   gVirtualGL->PopGLMatrix();
 
   gVirtualGL->PushGLMatrix();
   gVirtualGL->TranslateGL(fRad + fXc, 0., -fRad - fZc);
   gVirtualGL->GLLight(kLIGHT2, kPOSITION, pos);
   gVirtualGL->GLLight(kLIGHT2, kDIFFUSE, lig_prop);
   gVirtualGL->PopGLMatrix();

   gVirtualGL->TranslateGL(-fRad - fXc, 0., -fRad - fZc);
   gVirtualGL->GLLight(kLIGHT3, kPOSITION, pos);
   gVirtualGL->GLLight(kLIGHT3, kDIFFUSE, lig_prop);
   gVirtualGL->SwapBuffers(fGLWin);
}

//______________________________________________________________________________
TObject * TViewerOpenGL::UpdateRange(const TBuffer3D * buffer)
{
   Double_t xmin = buffer->fPnts[0], xmax = xmin, ymin = buffer->fPnts[1], ymax = ymin, zmin = buffer->fPnts[2], zmax = zmin;
   //calculate range
   for (Int_t i = 3, e = buffer->fNbPnts * 3; i < e; i += 3)
      xmin = TMath::Min(xmin, buffer->fPnts[i]), xmax = TMath::Max(xmax, buffer->fPnts[i]),
      ymin = TMath::Min(ymin, buffer->fPnts[i + 1]), ymax = TMath::Max(ymax, buffer->fPnts[i + 1]),
      zmin = TMath::Min(zmin, buffer->fPnts[i + 2]), zmax = TMath::Max(zmax, buffer->fPnts[i + 2]);

   TGLSelectionBox * ret_val = new TGLSelectionBox(std::make_pair(xmin, xmax), std::make_pair(ymin, ymax), std::make_pair(zmin, zmax), buffer->fId);

   if (!fGLObjects.GetSize()) {
      fRangeX.first = xmin, fRangeX.second = xmax;
      fRangeY.first = ymin, fRangeY.second = ymax;
      fRangeZ.first = zmin, fRangeZ.second = zmax;

      return ret_val;
   }

   if (fRangeX.first > xmin)
      fRangeX.first = xmin;
   if (fRangeX.second < xmax)
      fRangeX.second = xmax;
   if (fRangeY.first > ymin)
      fRangeY.first = ymin;
   if (fRangeY.second < ymax)
      fRangeY.second = ymax;
   if (fRangeZ.first > zmin)
      fRangeZ.first = zmin;
   if (fRangeZ.second < zmax)
      fRangeZ.second = zmax;

   return ret_val;
}

//______________________________________________________________________________
void TViewerOpenGL::BuildGLList()const
{
   gVirtualGL->NewGLList(fDList, kCOMPILE);
   TObjLink * lnk = fGLObjects.FirstLink();

   while (lnk) {
      TGLPrimitive * pobj = static_cast<TGLPrimitive *>(lnk->GetObject());
      gVirtualGL->GLLoadName(pobj->fName);
      pobj->GLDraw();
      lnk = lnk->Next();
   }

   gVirtualGL->EndGLList();
}

//______________________________________________________________________________
Bool_t TViewerOpenGL::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{
   switch (GET_MSG(msg)) {
   case kC_COMMAND:
      switch (GET_SUBMSG(msg)) {
      case kCM_BUTTON:
	   case kCM_MENU:
	      switch (parm1) {
         case kGLHelpAbout: {
            char str[32];
            sprintf(str, "About ROOT %s...", gROOT->GetVersion());
            TRootHelpDialog * hd = new TRootHelpDialog(this, str, 600, 400);
            hd->SetText(gHelpAbout);
            hd->Popup();
            break;
         }
         case kGLHelpOnViewer: {
            TRootHelpDialog * hd = new TRootHelpDialog(this, "Help on GL Viewer...", 600, 400);
            hd->SetText(gHelpViewerOpenGL);
            hd->Popup();
            break;
         }
	      default:
	         break;
	      }
	   default:
	      break;
      }
   default:
      break;
   }
   
   return kTRUE;
}

//______________________________________________________________________________
void TViewerOpenGL::TestSelection(Event_t * event)
{
   MakeCurrent();
   Double_t frp = fFrP[0] * fZoom;
   UInt_t buff[512] = {0};
   Int_t viewport[4] = {0};

   gVirtualGL->EnterSelectionMode(buff, 512, event, viewport);
   gVirtualGL->FrustumGL(-frp, frp, -frp, frp, fFrP[1], fFrP[2]);
   gVirtualGL->NewMVGL();
   gVirtualGL->TranslateGL(0., 0., -fRad);
   gVirtualGL->MultGLMatrix(fArcBall->GetRotMatrix());
   gVirtualGL->RotateGL(-90., 1., 0., 0.);
   gVirtualGL->TranslateGL(-fXc, -fYc, -fZc);
   gVirtualGL->RunGLList(fDList);
   
   Int_t res_hit = gVirtualGL->ExitSelectionMode();
   
   if (res_hit) {
      UInt_t choose = buff[3];
      UInt_t depth = buff[1];

      for (Int_t loop = 1; loop < res_hit; ++loop) {
         if (buff[loop * 4 + 1] < depth) {
            choose = buff[loop * 4 + 3];
            depth = buff[loop * 4 + 1];
         }
      }

      fSelected == choose ? fSelected = 0 : fSelected = choose;
      DrawObjects();
   } else if (fSelected) {
      fSelected = 0;
      DrawObjects();
   }
}
