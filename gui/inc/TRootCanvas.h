// @(#)root/gui:$Name:  $:$Id: TRootCanvas.h,v 1.9 2004/02/18 20:13:42 brun Exp $
// Author: Fons Rademakers   15/01/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/


#ifndef ROOT_TRootCanvas
#define ROOT_TRootCanvas

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRootCanvas                                                          //
//                                                                      //
// This class creates a main window with menubar, scrollbars and a      //
// drawing area. The widgets used are the new native ROOT GUI widgets.  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TCanvasImp
#include "TCanvasImp.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif

class TGCanvas;
class TGMenuBar;
class TGPopupMenu;
class TGLayoutHints;
class TGStatusBar;
class TRootContainer;
class TGToolBar;
class TGHorizontal3DLine;
class TVirtualPadEditor;

class TRootCanvas : public TGMainFrame, public TCanvasImp {

friend class TRootContainer;

private:
   TGCanvas            *fCanvasWindow;       // canvas widget
   TRootContainer      *fCanvasContainer;    // container in canvas widget
   TGMenuBar           *fMenuBar;            // menubar
   TGPopupMenu         *fFileMenu;           // file menu
   TGPopupMenu         *fEditMenu;           // edit menu
   TGPopupMenu         *fViewMenu;           // view menu
   TGPopupMenu         *fOptionMenu;         // option menu
   TGPopupMenu         *fInspectMenu;        // inspect menu
   TGPopupMenu         *fClassesMenu;        // classes menu
   TGPopupMenu         *fHelpMenu;           // help menu
   TGLayoutHints       *fMenuBarLayout;      // menubar layout hints
   TGLayoutHints       *fMenuBarItemLayout;  // layout hints for menu in menubar
   TGLayoutHints       *fMenuBarHelpLayout;  // layout hint for help menu in menubar
   TGLayoutHints       *fCanvasLayout;       // layout for canvas widget
   TGStatusBar         *fStatusBar;          // statusbar widget
   TGLayoutHints       *fStatusBarLayout;    // layout hints for statusbar
   
   TGCompositeFrame    *fEditorFrame;        // side farme for current pad editor 
   TGLayoutHints       *fEditorLayout;       // layout for editor frame
   TGCompositeFrame    *fMainFrame;          // main frame containing canvas and side frame
   TGLayoutHints       *fToolBarLayout;      // layout for toolbar widget
   TGToolBar           *fToolBar;            // icon button toolbar
   TGHorizontal3DLine  *fToolBarSep;         // toolbar separator
   TGLayoutHints       *fMainFrameLayout;    // layout for main frame
   
   TVirtualPadEditor   *fEditor;     // pointer to currently loaded pad editor
      
   Int_t                fCanvasID;   // index in fWindows array of TGX11
   Bool_t               fAutoFit;    // when true canvas container keeps same size as canvas
   Int_t                fButton;     // currently pressed button
   Long_t               fLockState;  // indicate drawing lock/unlock state

   void     CreateCanvas(const char *name);
   void     CreateEditor();

   Bool_t   HandleContainerButton(Event_t *ev);
   Bool_t   HandleContainerDoubleClick(Event_t *ev);
   Bool_t   HandleContainerConfigure(Event_t *ev);
   Bool_t   HandleContainerKey(Event_t *ev);
   Bool_t   HandleContainerMotion(Event_t *ev);
   Bool_t   HandleContainerExpose(Event_t *ev);
   Bool_t   HandleContainerCrossing(Event_t *ev);

   void     Lock();
   void     Unlock();
   Bool_t   IsLocked() { return fLockState; } 

public:
   TRootCanvas(TCanvas *c, const char *name, UInt_t width, UInt_t height);
   TRootCanvas(TCanvas *c, const char *name, Int_t x, Int_t y, UInt_t width, UInt_t height);
   virtual ~TRootCanvas();

   void     Close();
   void     ForceUpdate() { Layout(); }
   void     FitCanvas();
   void     GetWindowGeometry(Int_t &x, Int_t &y, UInt_t &w, UInt_t &h);
   UInt_t   GetCwidth() const;
   UInt_t   GetCheight() const;
   void     Iconify() { IconifyWindow(); }
   Int_t    InitWindow();
   void     SetWindowPosition(Int_t x, Int_t y);
   void     SetWindowSize(UInt_t w, UInt_t h);
   void     SetWindowTitle(const char *newTitle);
   void     SetCanvasSize(UInt_t w, UInt_t h);
   void     SetStatusText(const char *txt = 0, Int_t partidx = 0);

   void     Show() { MapRaised(); }
   void     ShowMenuBar(Bool_t show = kTRUE);
   void     ShowStatusBar(Bool_t show = kTRUE);

   void     ShowEditor(Bool_t show = kTRUE);
   void     ShowToolBar(Bool_t show = kTRUE);

   TGMenuBar     *GetMenuBar() const { return fMenuBar; }
   TGLayoutHints *GetMenuBarItemLayout() const { return fMenuBarItemLayout; }
   TGStatusBar   *GetStatusBar() const { return fStatusBar; }

   // overridden from TGMainFrame
   void     CloseWindow();
   Bool_t   ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
   void     ReallyDelete();

   ClassDef(TRootCanvas,0)  //ROOT native GUI version of main window with menubar and drawing area
};

#endif
