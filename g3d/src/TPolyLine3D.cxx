// @(#)root/g3d:$Name:  $:$Id: TPolyLine3D.cxx,v 1.4 2000/12/13 15:13:48 brun Exp $
// Author: Nenad Buncic   17/08/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include <fstream.h>
#include <iostream.h>

#include "TROOT.h"
#include "TPolyLine3D.h"
#include "TVirtualPad.h"
#include "TVirtualGL.h"
#include "TVirtualX.h"
#include "TPoint.h"
#include "TView.h"
#include "TPadView3D.h"

ClassImp(TPolyLine3D)

//______________________________________________________________________________
// PolyLine3D is a 3-dimensional polyline. It has 4 different constructors.
//
//   First one, without any parameters TPolyLine3D(), we call 'default
// constructor' and it's used in a case that just an initialisation is
// needed (i.e. pointer declaration).
//
//       Example:
//                 TPolyLine3D *pl1 = new TPolyLine3D;
//
//
//   Second one is 'normal constructor' with, usually, one parameter
// n (number of points), and it just allocates a space for the points.
//
//       Example:
//                 TPolyLine3D pl1(150);
//
//
//   Third one allocates a space for the points, and also makes
// initialisation from the given array.
//
//       Example:
//                 TPolyLine3D pl1(150, pointerToAnArray);
//
//
//   Fourth one is, almost, similar to the constructor above, except
// initialisation is provided with three independent arrays (array of
// x coordinates, y coordinates and z coordinates).
//
//       Example:
//                 TPolyLine3D pl1(150, xArray, yArray, zArray);
//





//______________________________________________________________________________
TPolyLine3D::TPolyLine3D()
{
//*-*-*-*-*-*-*-*-*-*-*-*-*3-D PolyLine default constructor*-*-*-*-*-*-*-*-*-*-*
//*-*                      ================================

   fN = 0;
   fP = 0;
   fLastPoint = -1;
}


//______________________________________________________________________________
TPolyLine3D::TPolyLine3D(Int_t n, Option_t *option)
{
//*-*-*-*-*-*3-D PolyLine normal constructor without initialisation*-*-*-*-*-*-*
//*-*        ======================================================
//*-*  If n < 0 the default size (2 points) is set
//*-*
   fLastPoint = -1;
   if (n < 1) fN = 2;  // Set the default size for this object
   else fN = n;

   fP = new Float_t[3*fN];
   for (Int_t i=0; i<3*fN; i++) fP[i] = 0;
   fOption = option;
}

//______________________________________________________________________________
TPolyLine3D::TPolyLine3D(Int_t n, Float_t *p, Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*-*-*3-D PolyLine normal constructor*-*-*-*-*-*-*-*-*-*-*-*
//*-*                      ===============================
//*-*  If n < 0 the default size (2 points) is set
//*-*

   if (n < 1) fN = 2;  // Set the default size for this object
   else fN = n;

   fP = new Float_t[3*fN];
   if (n > 0) {
     for (Int_t i=0; i<3*n; i++) {
        fP[i] = p[i];
     }
    fLastPoint = fN-1;
   }
   else {
     for (Int_t i=0; i<3*fN; i++) {
        fP[i] = 0;
     }
    fLastPoint = -1;
   }
   fOption = option;

}

//______________________________________________________________________________
TPolyLine3D::TPolyLine3D(Int_t n, Double_t *p, Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*-*-*3-D PolyLine normal constructor*-*-*-*-*-*-*-*-*-*-*-*
//*-*                      ===============================
//*-*  If n < 0 the default size (2 points) is set
//*-*

   if (n < 1) fN = 2;  // Set the default size for this object
   else fN = n;

   fP = new Float_t[3*fN];
   if (n > 0) {
     for (Int_t i=0; i<3*n; i++) {
        fP[i] = p[i];
     }
    fLastPoint = fN-1;
   }
   else {
     for (Int_t i=0; i<3*fN; i++) {
        fP[i] = 0;
     }
    fLastPoint = -1;
   }
   fOption = option;

}


//______________________________________________________________________________
TPolyLine3D::TPolyLine3D(Int_t n, Float_t *x, Float_t *y, Float_t *z, Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*-*-*3-D PolyLine normal constructor*-*-*-*-*-*-*-*-*-*-*-*
//*-*                      ===============================
//*-*  If n < 0 the default size (2 points) is set
//*-*

   fLastPoint = -1;
   if (n < 1) fN = 2;  // Set the default size for this object
   else fN = n;

   fP = new Float_t[3*fN];
   Int_t j = 0;
   if (n > 0) {
       for (Int_t i=0; i<n;i++) {
           fP[j]   = x[i];
           fP[j+1] = y[i];
           fP[j+2] = z[i];
           j += 3;
       }
       fLastPoint = fN-1;
   }
   else {
     for (Int_t i=0; i<3*fN; i++) {
        fP[i] = 0;
     }
   }
   fOption = option;
}


//______________________________________________________________________________
TPolyLine3D::TPolyLine3D(Int_t n, Double_t *x, Double_t *y, Double_t *z, Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*-*-*3-D PolyLine normal constructor*-*-*-*-*-*-*-*-*-*-*-*
//*-*                      ===============================
//*-*  If n < 0 the default size (2 points) is set
//*-*

   fLastPoint = -1;
   if (n < 1) fN = 2;  // Set the default size for this object
   else fN = n;

   fP = new Float_t[3*fN];
   Int_t j = 0;
   if (n > 0) {
       for (Int_t i=0; i<n;i++) {
           fP[j]   = x[i];
           fP[j+1] = y[i];
           fP[j+2] = z[i];
           j += 3;
       }
       fLastPoint = fN-1;
   }
   else {
     for (Int_t i=0; i<3*fN; i++) {
        fP[i] = 0;
     }
   }
   fOption = option;
}


//______________________________________________________________________________
TPolyLine3D::~TPolyLine3D()
{
//*-*-*-*-*-*-*-*-*-*-*-*-*3-D PolyLine default destructor*-*-*-*-*-*-*-*-*-*-*-*
//*-*                      ===============================

   if (fP) delete [] fP;

}


//______________________________________________________________________________
TPolyLine3D::TPolyLine3D(const TPolyLine3D &polyline)
{
   ((TPolyLine3D&)polyline).Copy(*this);
}


//______________________________________________________________________________
void TPolyLine3D::Copy(TObject &obj)
{
//*-*-*-*-*-*-*-*-*-*-*-*-*Copy this polyline to polyline*-*-*-*-*-*-*-*-*-*-*-*
//*-*                      ==============================

   TObject::Copy(obj);
   TAttLine::Copy(((TPolyLine3D&)obj));
   ((TPolyLine3D&)obj).fN = fN;
   if (((TPolyLine3D&)obj).fP)
      delete [] ((TPolyLine3D&)obj).fP;
   ((TPolyLine3D&)obj).fP = new Float_t[3*fN];
   for (Int_t i=0; i<3*fN;i++)  {((TPolyLine3D&)obj).fP[i] = fP[i];}
   ((TPolyLine3D&)obj).fOption = fOption;
   ((TPolyLine3D&)obj).fLastPoint = fLastPoint;
}


//______________________________________________________________________________
Int_t TPolyLine3D::DistancetoPrimitive(Int_t px, Int_t py)
{
//*-*-*-*-*-*-*-*Compute distance from point px,py to a 3-D polyline*-*-*-*-*-*-*
//*-*            ===================================================
//*-*
//*-*  Compute the closest distance of approach from point px,py to each segment
//*-*  of the polyline.
//*-*  Returns when the distance found is below DistanceMaximum.
//*-*  The distance is computed in pixels units.
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

   const Int_t inaxis = 7;
   Int_t dist = 9999;

   Int_t puxmin = gPad->XtoAbsPixel(gPad->GetUxmin());
   Int_t puymin = gPad->YtoAbsPixel(gPad->GetUymin());
   Int_t puxmax = gPad->XtoAbsPixel(gPad->GetUxmax());
   Int_t puymax = gPad->YtoAbsPixel(gPad->GetUymax());

//*-*- return if point is not in the user area
   if (px < puxmin - inaxis) return dist;
   if (py > puymin + inaxis) return dist;
   if (px > puxmax + inaxis) return dist;
   if (py < puymax - inaxis) return dist;

   TView *view = gPad->GetView();
   if (!view) return dist;

   Int_t i, dsegment;
   Double_t x1,y1,x2,y2;
   Float_t xndc[3];
   for (i=0;i<Size()-1;i++) {
      view->WCtoNDC(&fP[3*i], xndc);
      x1 = xndc[0];
      y1 = xndc[1];
      view->WCtoNDC(&fP[3*i+3], xndc);
      x2 = xndc[0];
      y2 = xndc[1];
      dsegment = DistancetoLine(px,py,x1,y1,x2,y2);
      if (dsegment < dist) dist = dsegment;
   }
   return dist;
}


//______________________________________________________________________________
void TPolyLine3D::Draw(Option_t *option)
{
//*-*-*-*-*-*-*-*Draw this 3-D polyline with its current attributes*-*-*-*-*-*-*
//*-*            ==================================================

   AppendPad(option);

}

//______________________________________________________________________________
void TPolyLine3D::DrawOutlineCube(TList *outline, Double_t *rmin, Double_t *rmax)
{
//*-*-*-*-*-*-*-*Draw cube outline with 3d polylines*-*-*-*-*-*-*
//*-*            ===================================
//*-*                                                                    *
//*-*      x = fRmin[0]        X = fRmax[0]                              *
//*-*      y = fRmin[1]        Y = fRmax[1]                              *
//*-*      z = fRmin[2]        Z = fRmax[2]                              *
//*-*                                                                    *
//*-*                                                                    *
//*-*            (x,Y,Z) +---------+ (X,Y,Z)                             *
//*-*                   /         /|                                     *
//*-*                  /         / |                                     *
//*-*                 /         /  |                                     *
//*-*        (x,y,Z) +---------+   |                                     *
//*-*                |         |   + (X,Y,z)                             *
//*-*                |         |  /                                      *
//*-*                |         | /                                       *
//*-*                |         |/                                        *
//*-*                +---------+                                         *
//*-*             (x,y,z)   (X,y,z)                                      *
//*-*                                                                    *
//*-*                                                                    *
//*-*                                                                    *
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**

   Double_t x = rmin[0];     Double_t X = rmax[0];
   Double_t y = rmin[1];     Double_t Y = rmax[1];
   Double_t z = rmin[2];     Double_t Z = rmax[2];

   TPolyLine3D *pl3d = (TPolyLine3D *)outline->First();
   if (!pl3d) {
      TView *view = gPad->GetView();
      TPolyLine3D *p1 = new TPolyLine3D(4);
      TPolyLine3D *p2 = new TPolyLine3D(4);
      TPolyLine3D *p3 = new TPolyLine3D(4);
      TPolyLine3D *p4 = new TPolyLine3D(4);
      p1->SetLineColor(view->GetLineColor());
      p1->SetLineStyle(view->GetLineStyle());
      p1->SetLineWidth(view->GetLineWidth());
      p1->Copy(*p2);
      p1->Copy(*p3);
      p1->Copy(*p4);
      outline->Add(p1);
      outline->Add(p2);
      outline->Add(p3);
      outline->Add(p4);
   }

   pl3d = (TPolyLine3D *)outline->First();

   pl3d->SetPoint(0, x, y, z);
   pl3d->SetPoint(1, X, y, z);
   pl3d->SetPoint(2, X, Y, z);
   pl3d->SetPoint(3, x, Y, z);

   pl3d = (TPolyLine3D *)outline->After(pl3d);

   pl3d->SetPoint(0, X, y, z);
   pl3d->SetPoint(1, X, y, Z);
   pl3d->SetPoint(2, X, Y, Z);
   pl3d->SetPoint(3, X, Y, z);

   pl3d = (TPolyLine3D *)outline->After(pl3d);

   pl3d->SetPoint(0, X, y, Z);
   pl3d->SetPoint(1, x, y, Z);
   pl3d->SetPoint(2, x, Y, Z);
   pl3d->SetPoint(3, X, Y, Z);

   pl3d = (TPolyLine3D *)outline->After(pl3d);

   pl3d->SetPoint(0, x, y, Z);
   pl3d->SetPoint(1, x, y, z);
   pl3d->SetPoint(2, x, Y, z);
   pl3d->SetPoint(3, x, Y, Z);
}


//______________________________________________________________________________
void TPolyLine3D::DrawPolyLine(Int_t n, Float_t *p, Option_t *option)
{
//*-*-*-*-*-*-*-*-*Draw this 3-D polyline with new coordinates*-*-*-*-*-*-*-*-*-*
//*-*              ============================================

   TPolyLine3D *newpolyline = new TPolyLine3D();
   Int_t size = 3*Size();
   newpolyline->fN =n;
   newpolyline->fP = new Float_t[size];
   for (Int_t i=0; i<size;i++) { newpolyline->fP[i] = p[i];}
   TAttLine::Copy(*newpolyline);
   newpolyline->fOption = fOption;
   newpolyline->fLastPoint = fLastPoint;
   newpolyline->SetBit(kCanDelete);
   newpolyline->AppendPad(option);
}

//______________________________________________________________________________
void TPolyLine3D::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
//*-*-*-*-*-*-*-*-*-*Execute action corresponding to one event*-*-*-*-*-*-*-*-*-*
//*-*                =========================================


        if (gPad->GetView())
                gPad->GetView()->ExecuteRotateView(event, px, py);

}

//______________________________________________________________________________
void TPolyLine3D::ls(Option_t *option) const
{
//*-*-*-*-*-*-*-*-*-*List this 3-D polyline with its attributes*-*-*-*-*-*-*
//*-*                ==========================================

   TROOT::IndentLevel();
   cout <<"PolyLine3D  N=" <<fN<<" Option="<<option<<endl;

}

//______________________________________________________________________________
void TPolyLine3D::Paint(Option_t *option)
{
//*-*-*-*-*-*-*-*-*Paint this 3-D polyline with its current attributes*-*-*-*-*
//*-*              ===================================================
//*-*
//*-* Option could be 'x3d', and it means that output
//*-* will be performed by X3D package.
//*-*
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

    //*-* Check whether there is some 3D view class for this TPad
    TPadView3D *view3D = (TPadView3D*)gPad->GetView3D();
    if (view3D) view3D->PaintPolyLine(this,option);

    //*-* Check if option is 'x3d'.      NOTE: This is a simple checking
    //                                         but since there is no other
    //                                         options yet, this works fine.
    Int_t size =Size();
    if ((*option != 'x') && (*option != 'X')) {
        PaintPolyLine(size, fP, option);
    }
    else {
        X3DBuffer *buff = new X3DBuffer;
        if (buff) {
            buff->numPoints = size;
            buff->numSegs   = size-1;
            buff->numPolys  = 0;        //NOTE: Because of different structure, our
            buff->polys     = NULL;     //      TPolyLine3D can't use polygons
            buff->points    = fP;
        }

        Int_t c = ((GetLineColor() % 8) - 1) * 4;     // Basic colors: 0, 1, ... 8
        if (c < 0) c = 0;

    //*-* Allocate memory for segments *-*
        buff->segs = new Int_t[buff->numSegs*3];
        if (buff->segs) {
            for (Int_t i = 0; i < buff->numSegs; i++) {
                buff->segs[3*i  ] = c;
                buff->segs[3*i+1] = i;
                buff->segs[3*i+2] = i+1;
            }
        }


        if (buff && buff->points && buff->segs) //If everything seems to be OK ...
            FillX3DBuffer(buff);
        else {                            // ... something very bad was happened
            gSize3D.numPoints -= buff->numPoints;
            gSize3D.numSegs   -= buff->numSegs;
            gSize3D.numPolys  -= buff->numPolys;
        }

        if (buff->segs)     delete [] buff->segs;
        if (buff->polys)    delete [] buff->polys;
        if (buff)           delete    buff;
    }
}


//______________________________________________________________________________
void TPolyLine3D::PaintPolyLine(Int_t n, Float_t *p, Option_t *)
{
//*-*-*-*-*-*-*-*-*Draw this 3-D polyline with new coordinates*-*-*-*-*-*-*-*-*-*
//*-*              ===========================================

   if (n < 2) return;

   TAttLine::Modify();  //Change line attributes only if necessary

//*-*- Loop on each individual line

   for (Int_t i=1;i<n;i++) {
      gPad->PaintLine3D(&p[3*i-3], &p[3*i]);
   }
}


//______________________________________________________________________________
void TPolyLine3D::PaintPolyLine(Int_t n, Double_t *p, Option_t *)
{
//*-*-*-*-*-*-*-*-*Draw this 3-D polyline with new coordinates*-*-*-*-*-*-*-*-*-*
//*-*              ===========================================

   if (n < 2) return;

   TAttLine::Modify();  //Change line attributes only if necessary

//*-*- Loop on each individual line

   for (Int_t i=1;i<n;i++) {
      gPad->PaintLine3D(&p[3*i-3], &p[3*i]);
   }
}

//______________________________________________________________________________
void TPolyLine3D::Print(Option_t *option) const
{
//*-*-*-*-*-*-*-*-*-*Dump this 3-D polyline with its attributes*-*-*-*-*-*-*-*-*
//*-*                ==========================================

   printf("    TPolyLine3D N=%d, Option=%s\n",fN,option);
   TString opt = option;
   opt.ToLower();
   if (opt.Contains("all")) {
      for (Int_t i=0;i<Size();i++) {
        printf(" x[%d]=%g, y[%d]=%g, z[%d]=%g\n",i,fP[3*i],i,fP[3*i+1],i,fP[3*i+2]);
      }
   }
}

//______________________________________________________________________________
void TPolyLine3D::SavePrimitive(ofstream &out, Option_t *)
{
    // Save primitive as a C++ statement(s) on output stream out

   char quote = '"';
   out<<"   "<<endl;
   if (gROOT->ClassSaved(TPolyLine3D::Class())) {
       out<<"   ";
   } else {
       out<<"   TPolyLine3D *";
   }
   Int_t size=Size();
   out<<"pline3D = new TPolyLine3D("<<fN<<","<<quote<<fOption<<quote<<");"<<endl;

   SaveLineAttributes(out,"pline3D",1,1,1);

   if (size > 0) {
    for (Int_t i=0;i<size;i++) {
       out<<"   pline3D->SetPoint("<<i<<","<<fP[3*i]<<","<<fP[3*i+1]<<","<<fP[3*i+2]<<");"<<endl;
    }
   }
   out<<"   pline3D->Draw();"<<endl;
}

//______________________________________________________________________________
Int_t TPolyLine3D::SetNextPoint(Double_t x, Double_t y, Double_t z)
{
// Set point following LastPoint

    fLastPoint++;
    SetPoint(fLastPoint, x, y, z);
    return fLastPoint;
}
//______________________________________________________________________________
void TPolyLine3D::SetPoint(Int_t n, Double_t x, Double_t y, Double_t z)
{
//*-*-*-*-*-*-*-*-*-*Initialize one point of the 3-D polyline*-*-*-*-*-*-*-*-*-*
//*-*                ========================================
//*-*  if n is more then the current TPolyLine3D size (n > fN) - re-allocate this
//*-*

   if (n < 0) return;
   if (!fP || n >= fN) {
   // re-allocate the object
      Float_t *savepoint = new Float_t [3*(n+1)];
      if (fP && fN){
         memcpy(savepoint,fP,3*fN*sizeof(Float_t));
        delete [] fP;
      }
      fP = savepoint;
      fN = n+1;
   }
   fP[3*n  ] = x;
   fP[3*n+1] = y;
   fP[3*n+2] = z;
   fLastPoint = TMath::Max(fLastPoint,n);
}


//______________________________________________________________________________
void TPolyLine3D::SetPolyLine(Int_t n, Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*Set new values for this 3-D polyline*-*-*-*-*-*-*-*-*-*-*
//*-*                  ====================================

   fN =n;
   if (fP) delete [] fP;
   fP = new Float_t[3*fN];
   memset(fP,0,3*fN*sizeof(Float_t));
   fOption = option;
   fLastPoint = fN-1;

}


//______________________________________________________________________________
void TPolyLine3D::SetPolyLine(Int_t n, Float_t *p, Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*Set new values for this 3-D polyline*-*-*-*-*-*-*-*-*-*-*
//*-*                  ====================================

   fN =n;
   if (fP) delete [] fP;
   fP = new Float_t[3*fN];
   if (p) {
      for (Int_t i=0; i<fN;i++) {
         fP[3*i]   = p[3*i];
         fP[3*i+1] = p[3*i+1];
         fP[3*i+2] = p[3*i+2];
      }
   } else {
       memset(fP,0,3*fN*sizeof(Float_t));
   }
   fOption = option;
   fLastPoint = fN-1;
}


//______________________________________________________________________________
void TPolyLine3D::SetPolyLine(Int_t n, Double_t *p, Option_t *option)
{
//*-*-*-*-*-*-*-*-*-*-*Set new values for this 3-D polyline*-*-*-*-*-*-*-*-*-*-*
//*-*                  ====================================

   fN =n;
   if (fP) delete [] fP;
   fP = new Float_t[3*fN];
   if (p) {
      for (Int_t i=0; i<fN;i++) {
         fP[3*i]   = p[3*i];
         fP[3*i+1] = p[3*i+1];
         fP[3*i+2] = p[3*i+2];
      }
   } else {
       memset(fP,0,3*fN*sizeof(Float_t));
   }
   fOption = option;
   fLastPoint = fN-1;

}

//______________________________________________________________________________
void TPolyLine3D::Sizeof3D() const
{
//*-*-*-*-*-*Return total X3D size of this shape with its attributes*-*-*-*-*-*-*
//*-*        =======================================================

    gSize3D.numPoints += Size();
    gSize3D.numSegs   += (Size()-1);
    gSize3D.numPolys  += 0;

}


//_______________________________________________________________________
void TPolyLine3D::Streamer(TBuffer &b)
{
//*-*-*-*-*-*-*-*-*Stream a class object*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*              =========================================
   UInt_t R__s, R__c;
   if (b.IsReading()) {
      b.ReadVersion(&R__s, &R__c);
      TObject::Streamer(b);
      TAttLine::Streamer(b);
      b >> fN;
      if (fN) {
         fP = new Float_t[3*fN];
         b.ReadFastArray(fP,3*fN);
      }
      fOption.Streamer(b);
      fLastPoint = fN-1;
      b.CheckByteCount(R__s, R__c, TPolyLine3D::IsA());
   } else {
      R__c = b.WriteVersion(TPolyLine3D::IsA(), kTRUE);
      TObject::Streamer(b);
      TAttLine::Streamer(b);
      Int_t size = Size();
      b << size;
      if (size) b.WriteFastArray(fP, 3*size);
      fOption.Streamer(b);
      b.SetByteCount(R__c, kTRUE);
   }
}
