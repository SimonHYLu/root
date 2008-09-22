// @(#)root/fitpanel:$Id$
// Author: Ilka Antcheva, Lorenzo Moneta 10/08/2006

/*************************************************************************
 * Copyright (C) 1995-2006, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TFitEditor
#define ROOT_TFitEditor

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TFitEditor                                                           //
//                                                                      //
// Allows to explore and compare various fits.                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif

#include "Foption.h"
#include "Math/MinimizerOptions.h"

#include <vector>

//--- Object types
enum EObjectType {
   kObjectHisto,
   kObjectGraph,
   kObjectGraph2D,
   kObjectHStack,
   kObjectTree,
   kObjectMultiGraph
};


class TGTab;
class TVirtualPad;
class TCanvas;
class TGLabel;
class TGComboBox;
class TGTextEntry;
class TGNumberEntry;
class TGDoubleHSlider;
class TGNumberEntry;
class TGNumberEntryField;
class TGStatusBar;
class TAxis;
class TF1;


class TFitEditor : public TGMainFrame {

protected:
   TGTab               *fTab;              // tab widget holding the editor
   TGCompositeFrame    *fTabContainer;     // main tab container
   TGCompositeFrame    *fGeneral;          // general tab
   TGCompositeFrame    *fMinimization;     // minimization tab
   TGTextButton        *fFitButton;        // performs fitting
   TGTextButton        *fResetButton;      // resets fit parameters
   TGTextButton        *fCloseButton;      // close the fit panel
   TGHorizontalFrame   *fObjLabelParent;   // parent of fObjLabel
   TGLabel             *fObjLabel;         // contains fitted object name
   TGLabel             *fSelLabel;         // contains selected fit function
   TGComboBox          *fFuncList;         // contains function list
   TGTextEntry         *fEnteredFunc;      // contains user function file name
   TGTextButton        *fUserButton;       // opens a dialog for user-defined fit method
   TGRadioButton       *fNone;             // set no operation mode
   TGRadioButton       *fAdd;              // set addition mode
   TGRadioButton       *fConv;             // set convolution mode
   TGLayoutHints       *fLayoutNone;       // layout hints of fNone radio button
   TGLayoutHints       *fLayoutAdd;        // layout hints of fAdd radio button
   TGLayoutHints       *fLayoutConv;       // layout hints of fConv radio button
   TGTextButton        *fSetParam;         // open set parameters dialog
   TGCheckButton       *fIntegral;         // switch on/off option 'integral'
   TGCheckButton       *fBestErrors;       // switch on/off option 'improve errors'
   TGCheckButton       *fUseRange;         // switch on/off option 'use function range'
   TGCheckButton       *fAdd2FuncList;     // switch on/off option 'add to list'
   TGCheckButton       *fUseGradient ;     // switch on/off option 'use gradient'
   TGCheckButton       *fAllWeights1;      // switch on/off option 'all weights=1'
   TGCheckButton       *fImproveResults;   // switch on/off option 'improve fit results'
   TGCheckButton       *fEmptyBinsWghts1;  // switch on/off option 'include empry bins'
   TGComboBox          *fMethodList;       // contains method list
   TGCheckButton       *fLinearFit;        // switch on/off linear fit option
   TGCheckButton       *fNoChi2;           // switch on/off option 'No Chi-square'
   TGCheckButton       *fNoStoreDrawing;   // switch on/off 'no store/drwing' option
   TGCheckButton       *fNoDrawing;        // switch on/off 'no drawing' option
   TGCheckButton       *fDrawSame;         // switch on/off fit function drawing
   TGTextButton        *fDrawAdvanced;     // opens a dialog for advanced draw options
   TGDoubleHSlider     *fSliderX;          // slider to set fit range along x-axis
   TGDoubleHSlider     *fSliderY;          // slider to set fit range along y-axis
   TGDoubleHSlider     *fSliderZ;          // slider to set fit range along z-axis
   TGHorizontalFrame   *fSliderXParent;    // parent of fSliderX
   TGHorizontalFrame   *fSliderYParent;    // parent of fSliderY
   TGHorizontalFrame   *fSliderZParent;    // parent of fSliderZ
   TGNumberEntry       *fRobustValue;      // contains robust value for linear fit
   TGRadioButton       *fOptDefault;       // set default printing mode
   TGRadioButton       *fOptVerbose;       // set printing mode to 'Verbose'
   TGRadioButton       *fOptQuiet;         // set printing mode to 'Quiet'
   TCanvas             *fCanvas;           // canvas containing the object
   TVirtualPad         *fParentPad;        // pad containing the object
   TObject             *fFitObject;        // selected object to fit
   EObjectType          fType;             // object type info
   Int_t                fDim;              // object dimension
   TAxis               *fXaxis;            // x-axis
   TAxis               *fYaxis;            // y-axis
   TAxis               *fZaxis;            // z-axis
//    are not  these bin numbers ? should be integer  ? 
   Float_t              fXrange;           // x-range 
   Float_t              fXmin;             // x-min
   Float_t              fXmax;             // x-max
   Float_t              fYrange;           // y-range
   Float_t              fYmin;             // y-min
   Float_t              fYmax;             // y-max
   Float_t              fZrange;           // z-range
   Float_t              fZmin;             // z-min
   Float_t              fZmax;             // z-max

   // structure holding parameter value and limits
   struct FuncParamData_t { 
      FuncParamData_t() {
         fP[0] = 0; fP[1] = 0; fP[2] = 0; 
      }
      Double_t & operator[](UInt_t i) { return fP[i];}
      Double_t fP[3];
   };
   std::vector<FuncParamData_t >  fFuncPars;         // function parameters (value + limits)

   Int_t                fPx1old,
                        fPy1old,
                        fPx2old,
                        fPy2old;

   Double_t             fFuncXmin;       // fit function range (min and max) values    
   Double_t             fFuncXmax;      
   Double_t             fFuncYmin;      
   Double_t             fFuncYmax;      
   Double_t             fFuncZmin;      
   Double_t             fFuncZmax;      

   TGRadioButton       *fLibMinuit;        // set default minimization library (Minuit)
   TGRadioButton       *fLibMinuit2;       // set Minuit2 as minimization library
   TGRadioButton       *fLibFumili;        // set Fumili as minimization library
   TGRadioButton       *fMigrad;           // set default minimization method (MIGRAD)
   TGRadioButton       *fSimplex;          // set Simplex as minimization method
   TGRadioButton       *fFumili;           // set Fumili as minimization method
   TGRadioButton       *fCombination;      // set Combination as minimization method
   TGRadioButton       *fScan;             // set Scan as minimization method
   TGNumberEntryField  *fErrorScale;       // contains error scale set for minimization
   TGNumberEntryField  *fTolerance;        // contains tolerance set for minimization
   TGNumberEntryField  *fIterations;       // contains maximum number of iterations

   TGStatusBar         *fStatusBar;        // statusbar widget
   
   static TFitEditor *fgFitDialog;         // singleton fit panel

   TGComboBox *BuildFunctionList(TGFrame *parent, Int_t id);
   TGComboBox *BuildMethodList(TGFrame *parent, Int_t id);
   Int_t       CheckFunctionString(const char* str);
   void        CreateGeneralTab();
   void        CreateMinimizationTab();
   void        MakeTitle(TGCompositeFrame *parent, const char *title);
   TF1*        HasFitFunction(TObject *obj);
   void        GetFunctionsFromList(TList *list);
   void        CheckRange(TF1 *f1);
   void        SetEditable(Bool_t);

private:
   TFitEditor(const TFitEditor&);              // not implemented
   TFitEditor& operator=(const TFitEditor&);   // not implemented

   void RetrieveOptions(Foption_t&, TString&, ROOT::Math::MinimizerOptions&, Int_t);

public:
   TFitEditor(TVirtualPad* pad, TObject *obj);
   virtual ~TFitEditor();

//   static TFitEditor *&GetFP();
   static  TFitEditor *GetInstance(TVirtualPad* pad, TObject *obj);
   virtual Option_t  *GetDrawOption() const;
   virtual void       Hide();
   virtual void       Show(TVirtualPad* pad, TObject *obj);

           void       ShowObjectName(TObject* obj);
           Bool_t     SetObjectType(TObject* obj);
   virtual void       Terminate();
           void       UpdateGUI();

   virtual void   CloseWindow();
   virtual void   ConnectSlots();
   virtual void   ConnectToCanvas();
   virtual void   DisconnectSlots();
   virtual void   RecursiveRemove(TObject* obj);
   virtual void   SetCanvas(TCanvas *c);
   virtual void   SetFitObject(TVirtualPad *pad, TObject *obj, Int_t event);
   virtual void   SetFunction(const char *function);

   // slot methods 'General' tab
   virtual void   DoAddition(Bool_t on);
   virtual void   DoAdvancedOptions();
   virtual void   DoAllWeights1();
   virtual void   DoClose();
   virtual void   DoEmptyBinsAllWeights1();
   virtual void   DoEnteredFunction();
   virtual void   DoFit();
   virtual void   DoMaxIterations();
   virtual void   DoFunction(Int_t sel);
   virtual void   DoLinearFit();
   virtual void   DoNoChi2();
   virtual void   DoNoSelection();
   virtual void   DoNoStoreDrawing();
   virtual void   DoReset();
   virtual void   DoSetParameters();
   virtual void   DoSliderXMoved();
   virtual void   DoSliderYMoved();
   virtual void   DoSliderZMoved();
   virtual void   DoUserDialog();

   // slot methods 'Minimization' tab
   virtual void   DoLibrary(Bool_t on);
   virtual void   DoMinMethod(Bool_t on);
   virtual void   DoPrintOpt(Bool_t on);
   
   typedef std::vector<FuncParamData_t > FuncParams_t; 

   
   ClassDef(TFitEditor,0)  //Fit Panel interface
};

#endif
