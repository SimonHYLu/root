// @(#)root/graf:$Name:  $:$Id: TGraphAsymmErrors.cxx,v 1.24 2002/11/16 15:17:47 brun Exp $
// Author: Rene Brun   03/03/99

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include <string.h>

#include "Riostream.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"
#include "TStyle.h"
#include "TMath.h"
#include "TArrow.h"
#include "TVirtualPad.h"
#include "TF1.h"

ClassImp(TGraphAsymmErrors)

//______________________________________________________________________________
//
//   A TGraphAsymmErrors is a TGraph with assymetric error bars.
//   The various format options to draw a TGraphAsymmErrors are explained in
//     TGraphAsymmErrors::Paint.
//
//  The picture below has been generated by the following macro:
//------------------------------------------------------------------------
//{
//   gROOT->Reset();
//   c1 = new TCanvas("c1","A Simple Graph with error bars",200,10,700,500);
//
//   c1->SetFillColor(42);
//   c1->SetGrid();
//   c1->GetFrame()->SetFillColor(21);
//   c1->GetFrame()->SetBorderSize(12);
//
//   Int_t n = 10;
//   Double_t x[n]  = {-0.22, 0.05, 0.25, 0.35, 0.5, 0.61,0.7,0.85,0.89,0.95};
//   Double_t y[n]  = {1,2.9,5.6,7.4,9,9.6,8.7,6.3,4.5,1};
//   Double_t exl[n] = {.05,.1,.07,.07,.04,.05,.06,.07,.08,.05};
//   Double_t eyl[n] = {.8,.7,.6,.5,.4,.4,.5,.6,.7,.8};
//   Double_t exh[n] = {.02,.08,.05,.05,.03,.03,.04,.05,.06,.03};
//   Double_t eyh[n] = {.6,.5,.4,.3,.2,.2,.3,.4,.5,.6};
//   gr = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
//   gr->SetTitle("TGraphAsymmErrors Example");
//   gr->SetMarkerColor(4);
//   gr->SetMarkerStyle(21);
//   gr->Draw("ALP");
//
//   c1->Update();
//}
//Begin_Html
/*
<img src="gif/gaerrors.gif">
*/
//End_Html
//

//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(): TGraph()
{
//*-*-*-*-*-*-*-*-*-*-*TGraphAsymmErrors default constructor*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  =====================================
   fEXlow       = 0;
   fEYlow       = 0;
   fEXhigh      = 0;
   fEYhigh      = 0;
}


//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(Int_t n)
       : TGraph(n)
{
//*-*-*-*-*-*-*-*-*-*-*TGraphAsymmErrors normal constructor*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ====================================
//
//  the arrays are preset to zero

   if (n <= 0) {
      Error("TGraphAsymmErrors", "illegal number of points (%d)", n);
      return;
   }

   fEXlow       = new Double_t[n];
   fEYlow       = new Double_t[n];
   fEXhigh      = new Double_t[n];
   fEYhigh      = new Double_t[n];

   for (Int_t i=0;i<n;i++) {
      fEXlow[i]  = 0;
      fEXhigh[i] = 0;
      fEYlow[i]  = 0;
      fEYhigh[i] = 0;
   }
}


//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(Int_t n, const Float_t *x, const Float_t *y, const Float_t *exl, const Float_t *exh, const Float_t *eyl, const Float_t *eyh)
       : TGraph(n,x,y)
{
//*-*-*-*-*-*-*-*-*-*-*TGraphAsymmErrors normal constructor*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ====================================
//
//  if exl,h or eyl,h are null, the corresponding arrays are preset to zero

   if (n <= 0) {
      Error("TGraphAsymmErrors", "illegal number of points (%d)", n);
      return;
   }

   fEXlow       = new Double_t[n];
   fEYlow       = new Double_t[n];
   fEXhigh      = new Double_t[n];
   fEYhigh      = new Double_t[n];

   for (Int_t i=0;i<n;i++) {
      if (exl) fEXlow[i]  = exl[i];
      else     fEXlow[i]  = 0;
      if (exh) fEXhigh[i] = exh[i];
      else     fEXhigh[i] = 0;
      if (eyl) fEYlow[i]  = eyl[i];
      else     fEYlow[i]  = 0;
      if (eyh) fEYhigh[i] = eyh[i];
      else     fEYhigh[i] = 0;
   }
}

//______________________________________________________________________________
TGraphAsymmErrors::TGraphAsymmErrors(Int_t n, const Double_t *x, const Double_t *y, const Double_t *exl, const Double_t *exh, const Double_t *eyl, const Double_t *eyh)
       : TGraph(n,x,y)
{
//*-*-*-*-*-*-*-*-*-*-*TGraphAsymmErrors normal constructor*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ====================================
//
//  if exl,h or eyl,h are null, the corresponding arrays are preset to zero

   if (n <= 0) {
      Error("TGraphAsymmErrors", "illegal number of points (%d)", n);
      return;
   }

   fEXlow       = new Double_t[n];
   fEYlow       = new Double_t[n];
   fEXhigh      = new Double_t[n];
   fEYhigh      = new Double_t[n];

   for (Int_t i=0;i<n;i++) {
      if (exl) fEXlow[i]  = exl[i];
      else     fEXlow[i]  = 0;
      if (exh) fEXhigh[i] = exh[i];
      else     fEXhigh[i] = 0;
      if (eyl) fEYlow[i]  = eyl[i];
      else     fEYlow[i]  = 0;
      if (eyh) fEYhigh[i] = eyh[i];
      else     fEYhigh[i] = 0;
   }
}

//______________________________________________________________________________
TGraphAsymmErrors::~TGraphAsymmErrors()
{
//*-*-*-*-*-*-*-*-*-*-*TGraphAsymmErrors default destructor*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ===============================

   delete [] fEXlow;
   delete [] fEXhigh;
   delete [] fEYlow;
   delete [] fEYhigh;
}

//______________________________________________________________________________
void TGraphAsymmErrors::Apply(TF1 *f)
{
  // apply a function to all data points
  // y = f(x,y)
  //
  // Errors are calculated as eyh = f(x,y+eyh)-f(x,y) and
  // eyl = f(x,y)-f(x,y-eyl)
  //
  // Special treatment has to be applied for the functions where the
  // role of "up" and "down" is reversed.
  // function suggested/implemented by Miroslav Helbich <helbich@mail.desy.de>

  Double_t x,y,exl,exh,eyl,eyh,eyl_new,eyh_new,fxy;

  for (Int_t i=0;i<GetN();i++) {
     GetPoint(i,x,y);
     exl=GetEXlow()[i];
     exh=GetEXhigh()[i];
     eyl=GetEYlow()[i];
     eyh=GetEYhigh()[i];

     fxy = f->Eval(x,y);
     SetPoint(i,x,fxy);

     // in the case of the functions like y-> -1*y the roles of the
     // upper and lower error bars is reversed
     if (f->Eval(x,y-eyl)<f->Eval(x,y+eyh)) {
        eyl_new = TMath::Abs(fxy - f->Eval(x,y-eyl));
        eyh_new = TMath::Abs(f->Eval(x,y+eyh) - fxy);
     }
     else {
        eyh_new = TMath::Abs(fxy - f->Eval(x,y-eyl));
        eyl_new = TMath::Abs(f->Eval(x,y+eyh) - fxy);
     }

     //error on x doesn't change
     SetPointError(i,exl,exh,eyl_new,eyh_new);
   }
}

//______________________________________________________________________________
void TGraphAsymmErrors::ComputeRange(Double_t &xmin, Double_t &ymin, Double_t &xmax, Double_t &ymax) const
{
  for (Int_t i=0;i<fNpoints;i++) {
     if (fX[i] -fEXlow[i] < xmin) {
        if (gPad->GetLogx()) {
           if (fEXlow[i] < fX[i]) xmin = fX[i]-fEXlow[i];
           else                   xmin = fX[i]/3;
        } else {
          xmin = fX[i]-fEXlow[i];
        }
     }
     if (fX[i] +fEXhigh[i] > xmax) xmax = fX[i]+fEXhigh[i];
     if (fY[i] -fEYlow[i] < ymin) {
        if (gPad->GetLogy()) {
           if (fEYlow[i] < fY[i]) ymin = fY[i]-fEYlow[i];
           else                   ymin = fY[i]/3;
        } else {
          ymin = fY[i]-fEYlow[i];
        }
     }
     if (fY[i] +fEYhigh[i] > ymax) ymax = fY[i]+fEYhigh[i];
  }
}

//______________________________________________________________________________
Double_t TGraphAsymmErrors::GetErrorX(Int_t i) const
{
//    This function is called by GraphFitChisquare.
//    It returns the quadratic error along X at point i.

   if (i < 0 || i >= fNpoints) return -1;
   if (!fEXlow && !fEXhigh) return -1;
   Double_t elow=0, ehigh=0;
   if (fEXlow)  elow  = fEXlow[i];
   if (fEXhigh) ehigh = fEXhigh[i];
   return TMath::Sqrt(0.5*(elow*elow + ehigh*ehigh));
}

//______________________________________________________________________________
Double_t TGraphAsymmErrors::GetErrorY(Int_t i) const
{
//    This function is called by GraphFitChisquare.
//    It returns the quadratic error along Y at point i.

   if (i < 0 || i >= fNpoints) return -1;
   if (!fEYlow && !fEYhigh) return -1;
   Double_t elow=0, ehigh=0;
   if (fEYlow)  elow  = fEYlow[i];
   if (fEYhigh) ehigh = fEYhigh[i];
   return TMath::Sqrt(0.5*(elow*elow + ehigh*ehigh));
}

//______________________________________________________________________________
Int_t TGraphAsymmErrors::InsertPoint()
{
// Insert a new point at the mouse position

   Int_t ipoint = TGraph::InsertPoint();

   Double_t *newEXlow  = new Double_t[fNpoints];
   Double_t *newEYlow  = new Double_t[fNpoints];
   Double_t *newEXhigh = new Double_t[fNpoints];
   Double_t *newEYhigh = new Double_t[fNpoints];
   Int_t i;
   for (i=0;i<ipoint;i++) {
      newEXlow[i]  = fEXlow[i];
      newEYlow[i]  = fEYlow[i];
      newEXhigh[i] = fEXhigh[i];
      newEYhigh[i] = fEYhigh[i];
  }
   newEXlow[ipoint]  = 0;
   newEYlow[ipoint]  = 0;
   newEXhigh[ipoint] = 0;
   newEYhigh[ipoint] = 0;
   for (i=ipoint+1;i<fNpoints;i++) {
      newEXlow[i]  = fEXlow[i-1];
      newEYlow[i]  = fEYlow[i-1];
      newEXhigh[i] = fEXhigh[i-1];
      newEYhigh[i] = fEYhigh[i-1];
   }
   delete [] fEXlow;
   delete [] fEYlow;
   delete [] fEXhigh;
   delete [] fEYhigh;
   fEXlow  = newEXlow;
   fEYlow  = newEYlow;
   fEXhigh = newEXhigh;
   fEYhigh = newEYhigh;
   return ipoint;
}

//______________________________________________________________________________
void TGraphAsymmErrors::Paint(Option_t *option)
{
   // Paint this TGraphAsymmErrors with its current attributes
   //
   // by default horizonthal and vertical small lines are drawn at
   // the end of the error bars. if option "z" or "Z" is specified,
   // these lines are not drawn.
   //
   // if option contains ">" an arrow is drawn at the end of the error bars
   // if option contains "|>" a full arrow is drawn at the end of the error bars
   // the size of the arrow is set to 2/3 of the marker size.
   //
   // By default, error bars are drawn. If option "X" is specified,
   // the errors are not drawn (TGraph::Paint equivalent).
   //
   // if option "[]" is specified only the end vertical/horizonthal lines
   // of the error bars are drawn. This option is interesting to superimpose
   // systematic errors on top of a graph with statistical errors.

   const Int_t BASEMARKER=8;
   Double_t s2x, s2y, symbolsize, sbase;
   Double_t x, y, xl1, xl2, xr1, xr2, yup1, yup2, ylow1, ylow2, tx, ty;
   static Float_t cxx[11] = {1,1,0.6,0.6,1,1,0.6,0.5,1,0.6,0.6};
   static Float_t cyy[11] = {1,1,1,1,1,1,1,1,1,0.5,0.6};

   if (strchr(option,'X') || strchr(option,'x')) {TGraph::Paint(option); return;}
   Bool_t brackets = kFALSE;
   if (strstr(option,"[]")) brackets = kTRUE;
   Bool_t endLines = kTRUE;
   if (strchr(option,'z')) endLines = kFALSE;
   if (strchr(option,'Z')) endLines = kFALSE;
   const char *arrowOpt = 0;
   if (strchr(option,'>'))  arrowOpt = ">";
   if (strstr(option,"|>")) arrowOpt = "|>";

   Bool_t axis = kFALSE;
   if (strchr(option,'a')) axis = kTRUE;
   if (strchr(option,'A')) axis = kTRUE;
   if (axis) TGraph::Paint(option);

   TAttLine::Modify();

   TArrow arrow;
   arrow.SetLineWidth(GetLineWidth());
   arrow.SetLineColor(GetLineColor());
   arrow.SetFillColor(GetFillColor());

   symbolsize  = GetMarkerSize();
   sbase       = symbolsize*BASEMARKER;
   Int_t mark  = GetMarkerStyle();
   Double_t cx  = 0;
   Double_t cy  = 0;
   if (mark >= 20 && mark < 31) {
      cx = cxx[mark-20];
      cy = cyy[mark-20];
   }

//*-*-      define the offset of the error bars due to the symbol size
   s2x  = gPad->PixeltoX(Int_t(0.5*sbase)) - gPad->PixeltoX(0);
   s2y  =-gPad->PixeltoY(Int_t(0.5*sbase)) + gPad->PixeltoY(0);
   tx   = 0.50*s2x;
   ty   = 0.50*s2y;
   Float_t asize = 0.6*symbolsize*BASEMARKER/gPad->GetWh();

   gPad->SetBit(kClipFrame, TestBit(kClipFrame));
   for (Int_t i=0;i<fNpoints;i++) {
      x  = gPad->XtoPad(fX[i]);
      y  = gPad->YtoPad(fY[i]);
      if (x < gPad->GetUxmin()) continue;
      if (x > gPad->GetUxmax()) continue;
      if (y < gPad->GetUymin()) continue;
      if (y > gPad->GetUymax()) continue;
      xl1 = x - s2x*cx;
      xl2 = gPad->XtoPad(fX[i] - fEXlow[i]);
      if (xl1 > xl2) {
         if (arrowOpt) {
            arrow.PaintArrow(xl1,y,xl2,y,asize,arrowOpt);
         } else {
            if (!brackets) gPad->PaintLine(xl1,y,xl2,y);
            if (endLines)  gPad->PaintLine(xl2,y-ty,xl2,y+ty);
         }
      }
      xr1 = x + s2x*cx;
      xr2 = gPad->XtoPad(fX[i] + fEXhigh[i]);
      if (xr1 < xr2) {
         if (arrowOpt) {
            arrow.PaintArrow(xr1,y,xr2,y,asize,arrowOpt);
         } else {
            if (!brackets) gPad->PaintLine(xr1,y,xr2,y);
            if (endLines)  gPad->PaintLine(xr2,y-ty,xr2,y+ty);
         }
      }
      yup1 = y + s2y*cy;
      yup2 = gPad->YtoPad(fY[i] + fEYhigh[i]);
      if (yup2 > gPad->GetUymax()) yup2 =  gPad->GetUymax();
      if (yup2 > yup1) {
         if (arrowOpt) {
            arrow.PaintArrow(x,yup1,x,yup2,asize,arrowOpt);
         } else {
            if (!brackets) gPad->PaintLine(x,yup1,x,yup2);
            if (endLines)  gPad->PaintLine(x-tx,yup2,x+tx,yup2);
         }
      }
      ylow1 = y - s2y*cy;
      ylow2 = gPad->YtoPad(fY[i] - fEYlow[i]);
      if (ylow2 < gPad->GetUymin()) ylow2 =  gPad->GetUymin();
      if (ylow2 < ylow1) {
         if (arrowOpt) {
            arrow.PaintArrow(x,ylow1,x,ylow2,asize,arrowOpt);
         } else {
            if (!brackets) gPad->PaintLine(x,ylow1,x,ylow2);
            if (endLines)  gPad->PaintLine(x-tx,ylow2,x+tx,ylow2);
         }
      }
   }
   if (!brackets && !axis) TGraph::Paint(option);
   gPad->ResetBit(kClipFrame);
}


//______________________________________________________________________________
void TGraphAsymmErrors::Print(Option_t *) const
{
//*-*-*-*-*-*-*-*-*-*-*Print graph and errors values*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  =============================
//

   for (Int_t i=0;i<fNpoints;i++) {
      printf("x[%d]=%g, y[%d]=%g, exl[%d]=%g, exh[%d]=%g, eyl[%d]=%g, eyh[%d]=%g\n"
         ,i,fX[i],i,fY[i],i,fEXlow[i],i,fEXhigh[i],i,fEYlow[i],i,fEYhigh[i]);
   }
}

//______________________________________________________________________________
Int_t TGraphAsymmErrors::RemovePoint()
{
// Delete point close to the mouse position

   Int_t ipoint = TGraph::RemovePoint();
   if (ipoint < 0) return ipoint;

   Double_t *newEXlow  = new Double_t[fNpoints];
   Double_t *newEYlow  = new Double_t[fNpoints];
   Double_t *newEXhigh = new Double_t[fNpoints];
   Double_t *newEYhigh = new Double_t[fNpoints];
   Int_t i, j = -1;
   for (i=0;i<fNpoints+1;i++) {
      if (i == ipoint) continue;
      j++;
      newEXlow[j]  = fEXlow[i];
      newEYlow[j]  = fEYlow[i];
      newEXhigh[j] = fEXhigh[i];
      newEYhigh[j] = fEYhigh[i];
   }
   delete [] fEXlow;
   delete [] fEYlow;
   delete [] fEXhigh;
   delete [] fEYhigh;
   fEXlow  = newEXlow;
   fEYlow  = newEYlow;
   fEXhigh = newEXhigh;
   fEYhigh = newEYhigh;
   return ipoint;
}

//______________________________________________________________________________
Int_t TGraphAsymmErrors::RemovePoint(Int_t ipnt)
{
// Delete point number ipnt

   Int_t ipoint = TGraph::RemovePoint(ipnt);
   if (ipoint < 0) return ipoint;

   Double_t *newEXlow  = new Double_t[fNpoints];
   Double_t *newEYlow  = new Double_t[fNpoints];
   Double_t *newEXhigh = new Double_t[fNpoints];
   Double_t *newEYhigh = new Double_t[fNpoints];
   Int_t i, j = -1;
   for (i=0;i<fNpoints+1;i++) {
      if (i == ipoint) continue;
      j++;
      newEXlow[j]  = fEXlow[i];
      newEYlow[j]  = fEYlow[i];
      newEXhigh[j] = fEXhigh[i];
      newEYhigh[j] = fEYhigh[i];
   }
   delete [] fEXlow;
   delete [] fEYlow;
   delete [] fEXhigh;
   delete [] fEYhigh;
   fEXlow  = newEXlow;
   fEYlow  = newEYlow;
   fEXhigh = newEXhigh;
   fEYhigh = newEYhigh;
   return ipoint;
}

//______________________________________________________________________________
void TGraphAsymmErrors::SavePrimitive(ofstream &out, Option_t *option)
{
    // Save primitive as a C++ statement(s) on output stream out

   char quote = '"';
   out<<"   "<<endl;
   if (gROOT->ClassSaved(TGraphAsymmErrors::Class())) {
       out<<"   ";
   } else {
       out<<"   TGraphAsymmErrors *";
   }
   out<<"grae = new TGraphAsymmErrors("<<fNpoints<<");"<<endl;
   out<<"   grae->SetName("<<quote<<GetName()<<quote<<");"<<endl;
   out<<"   grae->SetTitle("<<quote<<GetTitle()<<quote<<");"<<endl;

   SaveFillAttributes(out,"grae",0,1001);
   SaveLineAttributes(out,"grae",1,1,1);
   SaveMarkerAttributes(out,"grae",1,1,1);

   for (Int_t i=0;i<fNpoints;i++) {
      out<<"   grae->SetPoint("<<i<<","<<fX[i]<<","<<fY[i]<<");"<<endl;
      out<<"   grae->SetPointError("<<i<<","<<fEXlow[i]<<","<<fEXhigh[i]<<","<<fEYlow[i]<<","<<fEYhigh[i]<<");"<<endl;
   }
   
   static Int_t frameNumber = 0;
   if (fHistogram) {
      frameNumber++;
      TString hname = fHistogram->GetName();
      hname += frameNumber;
      fHistogram->SetName(hname.Data());
      fHistogram->SavePrimitive(out,"nodraw");
      out<<"   grae->SetHistogram("<<fHistogram->GetName()<<");"<<endl;
      out<<"   "<<endl;
   }

   // save list of functions
   TIter next(fFunctions);
   TObject *obj;
   while ((obj=next())) {
      obj->SavePrimitive(out,"nodraw");
      out<<"   grae->GetListOfFunctions()->Add("<<obj->GetName()<<");"<<endl;
      if (obj->InheritsFrom("TPaveStats")) {
         out<<"   ptstats->SetParent(grae->GetListOfFunctions());"<<endl;
      }
   }

   if (strstr(option,"multigraph")) {
      out<<"   multigraph->Add(grae);"<<endl;
      return;
   }
   out<<"   grae->Draw("
      <<quote<<option<<quote<<");"<<endl;
}

//______________________________________________________________________________
void TGraphAsymmErrors::SetPoint(Int_t i, Double_t x, Double_t y)
{
//*-*-*-*-*-*-*-*-*-*-*Set x and y values for point number i*-*-*-*-*-*-*-*-*
//*-*                  =====================================

   if (i < 0) return;
   if (i >= fNpoints) {
   // re-allocate the object
      Double_t *savex   = new Double_t[i+1];
      Double_t *savey   = new Double_t[i+1];
      Double_t *saveexl = new Double_t[i+1];
      Double_t *saveeyl = new Double_t[i+1];
      Double_t *saveexh = new Double_t[i+1];
      Double_t *saveeyh = new Double_t[i+1];
      if (fNpoints > 0) {
         memcpy(savex,  fX,     fNpoints*sizeof(Double_t));
         memcpy(savey,  fY,     fNpoints*sizeof(Double_t));
         memcpy(saveexl,fEXlow, fNpoints*sizeof(Double_t));
         memcpy(saveeyl,fEYlow, fNpoints*sizeof(Double_t));
         memcpy(saveexh,fEXhigh,fNpoints*sizeof(Double_t));
         memcpy(saveeyh,fEYhigh,fNpoints*sizeof(Double_t));
      }
      if (fX)      delete [] fX;
      if (fY)      delete [] fY;
      if (fEXlow)  delete [] fEXlow;
      if (fEYlow)  delete [] fEYlow;
      if (fEXhigh) delete [] fEXhigh;
      if (fEYhigh) delete [] fEYhigh;
      fX  = savex;
      fY  = savey;
      fEXlow  = saveexl;
      fEYlow  = saveeyl;
      fEXhigh = saveexh;
      fEYhigh = saveeyh;
      fNpoints = i+1;
   }
   fX[i] = x;
   fY[i] = y;
   if (fHistogram) {
      delete fHistogram;
      fHistogram = 0;
   }
}

//______________________________________________________________________________
void TGraphAsymmErrors::SetPointError(Double_t exl, Double_t exh, Double_t eyl, Double_t eyh)
{
//*-*-*-*-*-*-*Set ex and ey values for point pointed by the mouse*-*-*-*
//*-*          ===================================================

   Int_t px = gPad->GetEventX();
   Int_t py = gPad->GetEventY();

   //localize point to be deleted
   Int_t ipoint = -2;
   Int_t i;
   // start with a small window (in case the mouse is very close to one point)
   for (i=0;i<fNpoints;i++) {
      Int_t dpx = px - gPad->XtoAbsPixel(gPad->XtoPad(fX[i]));
      Int_t dpy = py - gPad->YtoAbsPixel(gPad->YtoPad(fY[i]));
      if (dpx*dpx+dpy*dpy < 25) {ipoint = i; break;}
   }
   if (ipoint == -2) return;

   fEXlow[ipoint]  = exl;
   fEYlow[ipoint]  = eyl;
   fEXhigh[ipoint] = exh;
   fEYhigh[ipoint] = eyh;
   gPad->Modified();
}

//______________________________________________________________________________
void TGraphAsymmErrors::SetPointError(Int_t i, Double_t exl, Double_t exh, Double_t eyl, Double_t eyh)
{
//*-*-*-*-*-*-*-*-*-*-*Set ex and ey values for point number i*-*-*-*-*-*-*-*
//*-*                  =======================================

   if (i < 0) return;
   if (i >= fNpoints) {
   // re-allocate the object
      TGraphAsymmErrors::SetPoint(i,0,0);
   }
   fEXlow[i]  = exl;
   fEYlow[i]  = eyl;
   fEXhigh[i] = exh;
   fEYhigh[i] = eyh;
}

//______________________________________________________________________________
void TGraphAsymmErrors::Streamer(TBuffer &b)
{
   // Stream an object of class TGraphAsymmErrors.

   if (b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = b.ReadVersion(&R__s, &R__c);
      if (R__v > 2) {
         TGraphAsymmErrors::Class()->ReadBuffer(b, this, R__v, R__s, R__c);
         return;
      }
      //====process old versions before automatic schema evolution
      TGraph::Streamer(b);
      fEXlow  = new Double_t[fNpoints];
      fEYlow  = new Double_t[fNpoints];
      fEXhigh = new Double_t[fNpoints];
      fEYhigh = new Double_t[fNpoints];
      if (R__v < 2) {
         Float_t *exlow  = new Float_t[fNpoints];
         Float_t *eylow  = new Float_t[fNpoints];
         Float_t *exhigh = new Float_t[fNpoints];
         Float_t *eyhigh = new Float_t[fNpoints];
         b.ReadFastArray(exlow,fNpoints);
         b.ReadFastArray(eylow,fNpoints);
         b.ReadFastArray(exhigh,fNpoints);
         b.ReadFastArray(eyhigh,fNpoints);
         for (Int_t i=0;i<fNpoints;i++) {
            fEXlow[i]  = exlow[i];
            fEYlow[i]  = eylow[i];
            fEXhigh[i] = exhigh[i];
            fEYhigh[i] = eyhigh[i];
         }
         delete [] eylow;
         delete [] exlow;
         delete [] eyhigh;
         delete [] exhigh;
      } else {
         b.ReadFastArray(fEXlow,fNpoints);
         b.ReadFastArray(fEYlow,fNpoints);
         b.ReadFastArray(fEXhigh,fNpoints);
         b.ReadFastArray(fEYhigh,fNpoints);
      }
      b.CheckByteCount(R__s, R__c, TGraphAsymmErrors::IsA());
      //====end of old versions

   } else {
      TGraphAsymmErrors::Class()->WriteBuffer(b,this);
   }
}
