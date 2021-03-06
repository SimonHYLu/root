// @(#)root/postscript:$Id$
// Author: Rene Brun, Olivier Couet, Pierre Juillot, Oleksandr Grebenyuk

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/


//______________________________________________________________________________
/* Begin_Html
<center><h2>TPostScript: Graphics interface to PostScript</h2></center>
This code was initially developed in the context of HIGZ and PAW
by Olivier Couet and Pierre Juillot. It has been converted to a C++ class by
Rene Brun.

<P>To generate a Postscript (or encapsulated ps) file corresponding to
a single image in a canvas, you can:

<UL>
<LI>Select the <B>Print PostScript</B> item in the canvas <B>File</B> menu.
By default, a Postscript file with the name of the canvas.ps is generated.

<br>
<LI>Click in the canvas area, near the edges, with the right mouse button
and select the <B>Print</B> item. You can select the name of the Postscript
file. If the file name is xxx.ps, you will generate a Postscript file named
xxx.ps. If the file name is xxx.eps, you generate an encapsulated Postscript
file instead.
<br>

<LI>In your program (or macro), you can type:

<P><TT><B>
c1-&gt;Print(&quot;xxx.ps&quot;)</B> or <B>c1-&gt;Print(&quot;xxx.eps&quot;)
</B></TT>

<P>This will generate a file corresponding to the picture in the canvas
pointed by <tt><B>c1</B></tt>.

<P><TT><B>pad1-&gt;Print(&quot;xxx.ps&quot;)</B></TT>

<P>prints only the picture in the pad pointed by <tt><B>pad1</B></tt>. The size
of the Postscript picture, by default, is computed to keep the aspect ratio
of the picture on the screen, where the size along x is always 20cm. You
can set the size of the PostScript picture before generating the picture
with a command such as:

<PRE>
   TPostScript myps(&quot;myfile.ps&quot;,111)
   myps.Range(xsize,ysize);
   object-&gt;Draw();
   myps.Close();
</PRE>

<P>You can set the default paper size with:
<PRE>
   gStyle-&gt;SetPaperSize(xsize,ysize);
</PRE>
<P>You can resume writing again in this file with <tt><B>myps.Open();</B></tt>.
Note that you may have several Postscript files opened simultaneously.
</UL>

<H2>Output type</H2>

The output type allows to define how the PostScript output will looks like.
It allows to define the page format (A4, Legal etc..), the orientation
(Portrait, Landscape) and the number of images (zones) per page.
The output type has the following form:
<pre>
                        [Format][Nx][Ny][Type]
</pre>
Where:
<ul>
   <li> Format : Is an integer between 0 and 99 defining the page format.
        Example:
   <br> Format = 3 the paper is in the standard A3 format.
   <br> Format = n (1<n<98) is an An format.
   <br> Format = 4 and Format=0 are the same and define an A4 page.
   <br> The A0 format is selected by Format=99.
   <br> The US format Letter is selected by Format = 100.
   <br> The US format Legal is selected by Format = 200.
   <br> The US format Ledger is selected by Format = 300.

   <li> Nx, Ny : Specify respectively the number of zones on the x and y axis.
   Nx and Ny are integers between 1 and 9.

   <li> Type : Can be equal to:
   <ul>
   <li> 1 : Portrait mode with a small margin at the bottom of the page.
   <li> 2 : Landscape mode with a small margin at the bottom of the page.
   <li> 4 : Portrait mode with a large margin at the bottom of the page.
   <li> 5 : Landscape mode with a large margin at the bottom of the page.
            The large margin is useful for some PostScript printers (very often
            for the colour printers) as they need more space to grip the paper
            for mechanical reasons. Note that some PostScript colour printers
            can also use the so called special A4 format permitting the full
            usage of the A4 area; in this case larger margins are not necessary
            and Type=1 or 2 can be used.
   <li> 3 : Encapsulated PostScript. This Type permits the generation of files
            which can be included in other documents, for example in LaTeX
            files.
   </ul>
</ul>

<H2>Making several pictures in the same Postscript file: case 1</H2>
<P>The following macro is an example illustrating how to open a Postscript
file and draw several pictures. The generation of a new Postscript page
is automatic when <tt><B>TCanvas::Clear</B></tt> is called by
<tt><b>object-&gt;Draw()</b></tt>.
<PRE>
{
   TFile f(&quot;hsimple.root&quot;);
   TCanvas c1(&quot;c1&quot;,&quot;canvas&quot;,800,600);

   <b>// select postscript output type</b>
   <b>// type = 111   portrait  ps</b>
   <b>// type = 112   landscape ps</b>
   <b>// type = 113   eps</b>
   Int_t type = 111;

   <b>// create a postscript file and set the paper size</b>
   TPostScript ps(&quot;test.ps&quot;,type);
   ps.Range(16,24);  //set x,y of printed page

   <b>// draw 3 histograms from file hsimple.root on separate pages</b>
   hpx-&gt;Draw();
   c1.Update();      //force drawing in a macro
   hprof-&gt;Draw();
   c1.Update();
   hpx-&gt;Draw(&quot;lego1&quot;);
   c1.Update();
   ps.Close();
}
</PRE>

<H2>Making several pictures in the same Postscript file: case 2</H2>
<P>This example shows 2 pages. The canvas is divided.
<tt><B>TPostScript::NewPage</B></tt> must be called before starting a new
picture.<tt><b>object-&gt;Draw</b></tt> does not clear the canvas in this case
because we clear only the pads and not the main canvas.
Note that <tt><b>c1-&gt;Update</b></tt> must be called at the end of the first
picture.
<PRE>
{
   TFile *f1 = new TFile("hsimple.root");
   TCanvas *c1 = new TCanvas("c1");
   TPostScript *ps = new TPostScript("file.ps",112);
   c1-&gt;Divide(2,1);
<b>// picture 1</b>
   ps-&gt;NewPage();
   c1-&gt;cd(1);
   hpx-&gt;Draw();
   c1-&gt;cd(2);
   hprof-&gt;Draw();
   c1-&gt;Update();

<b>// picture 2</b>
   ps-&gt;NewPage();
   c1-&gt;cd(1);
   hpxpy-&gt;Draw();
   c1-&gt;cd(2);
   ntuple-&gt;Draw("px");
   c1-&gt;Update();
   ps-&gt;Close();

<b>// invoke Postscript viewer</b>
   gSystem-&gt;Exec("gs file.ps");
}
</PRE>

<H2>Making several pictures in the same Postscript file: case 3</H2>
<b>This is the recommended way</b>. If the Postscript file name finishes with
"(", the file remains opened (it is not closed). If the Postscript file name
finishes with ")" and the file has been opened with "(", the file is closed.
<P>Example:
<PRE>
{
   TCanvas c1("c1");
   h1.Draw();
   c1.Print("c1.ps(");  // write canvas and keep the ps file open
   h2.Draw();
   c1.Print("c1.ps");   // canvas is added to "c1.ps"
   h3.Draw();
   c1.Print("c1.ps)");  // canvas is added to "c1.ps" and ps file is closed
}
</PRE>
The <tt>TCanvas::Print("file.ps(")</tt> mechanism is very useful, but it can
be a little inconvenient to have the action of opening/closing a file being
atomic with printing a page. Particularly if pages are being generated in some
loop one needs to detect the special cases of first and last page and then
munge the argument to Print() accordingly.
<BR>The "[" and "]" can be used instead of "(" and ")" as shown below.
<P>Example:
<PRE>
   c1.Print("file.ps[");        // No actual print, just open file.ps

   for (int i=0; i<10; ++i) {
      // fill canvas for context i
      // ...

      c1.Print("file.ps");      // Actually print canvas to the file
   }

   c1.Print("file.ps]");        // No actual print, just close the file
</PRE>

<H2>Color Model</H2>
TPostScript support two color model RGB and CMYK. CMY and CMYK models are
subtractive color models unlike RGB which is an additive. They are mainly
used for printing purposes. CMY means Cyan Magenta Yellow to convert RGB
to CMY it is enough to do: C=1-R, M=1-G and Y=1-B. CMYK has one more
component K (black). The conversion from RGB to CMYK is:
<PRE>
 Double_t Black   = TMath::Min(TMath::Min(1-Red,1-Green),1-Blue);
 Double_t Cyan    = (1-Red-Black)/(1-Black);
 Double_t Magenta = (1-Green-Black)/(1-Black);
 Double_t Yellow  = (1-Blue-Black)/(1-Black);
</PRE>
CMYK add the black component which allows to have a better quality for black
printing. PostScript support the CMYK model.
<br>
To change the color model use gStyle->SetColorModelPS(c).
<ul>
<li> c = 0 means TPostScript will use RGB color model (default)
<li> c = 1 means TPostScript will use CMYK color model
</ul>
End_Html */

#ifdef WIN32
#pragma optimize("",off)
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Riostream.h"
#include "TROOT.h"
#include "TColor.h"
#include "TVirtualPad.h"
#include "TPoints.h"
#include "TPostScript.h"
#include "TStyle.h"
#include "TMath.h"
#include "TText.h"
#include "TSystem.h"

// to scale fonts to the same size as the old TT version
const Float_t kScale = 0.93376068;

Int_t TPostScript::fgLineJoin = 0;

ClassImp(TPostScript)


//______________________________________________________________________________
TPostScript::TPostScript() : TVirtualPS()
{
   // Default PostScript constructor

   fStream          = 0;
   fType            = 0;
   gVirtualPS       = this;
   fBlue            = 0.;
   fBoundingBox     = kFALSE;
   fClear           = kFALSE;
   fClip            = 0;
   fClipStatus      = kFALSE;
   fCurrentColor    = 0;
   fDXC             = 0.;
   fDYC             = 0.;
   fFX              = 0.;
   fFY              = 0.;
   fGreen           = 0.;
   fIXzone          = 0;
   fIYzone          = 0;
   fLastCellBlue    = 0;
   fLastCellGreen   = 0;
   fLastCellRed     = 0;
   fLineJoin        = 0;
   fLineScale       = 0.;
   fMarkerSizeCur   = 0.;
   fMaxLines        = 0;
   fMaxsize         = 0;
   fMode            = 0;
   fNBSameColorCell = 0;
   fNXzone          = 0;
   fNYzone          = 0;
   fNbCellLine      = 0;
   fNbCellW         = 0;
   fNbinCT          = 0;
   fNpages          = 0;
   fRange           = kFALSE;
   fRed             = 0.;
   fSave            = 0;
   fX1v             = 0.;
   fX1w             = 0.;
   fX2v             = 0.;
   fX2w             = 0.;
   fXC              = 0.;
   fXVP1            = 0.;
   fXVP2            = 0.;
   fXVS1            = 0.;
   fXVS2            = 0.;
   fXsize           = 0.;
   fY1v             = 0.;
   fY1w             = 0.;
   fY2v             = 0.;
   fY2w             = 0.;
   fYC              = 0.;
   fYVP1            = 0.;
   fYVP2            = 0.;
   fYVS1            = 0.;
   fYVS2            = 0.;
   fYsize           = 0.;
   fZone            = kFALSE;
   for (Int_t i=0; i<32; i++) fPatterns[i]=0;
}


//______________________________________________________________________________
TPostScript::TPostScript(const char *fname, Int_t wtype)
            :TVirtualPS(fname, wtype)
{
   // Initialize the PostScript interface
   //
   //  fname : PostScript file name
   //  wtype : PostScript workstation type
   //
   //
   //  The possible workstation types are:
   //     111 ps  Portrait
   //     112 ps  Landscape
   //     113 eps

   fStream = 0;
   Open(fname, wtype);
}


//______________________________________________________________________________
void TPostScript::Open(const char *fname, Int_t wtype)
{
   // Open a PostScript file
   if (fStream) {
      Warning("Open", "postscript file already open");
      return;
   }

   fMarkerSizeCur = 0;
   fCurrentColor  = 0;
   fRed           = -1;
   fGreen         = -1;
   fBlue          = -1;
   fLenBuffer     = 0;
   fClip          = 0;
   fType          = abs(wtype);
   fClear         = kTRUE;
   fZone          = kFALSE;
   fSave          = 0;
   SetLineScale(gStyle->GetLineScalePS());
   gStyle->GetPaperSize(fXsize, fYsize);
   fMode          = fType%10;
   Float_t xrange, yrange;
   if (gPad) {
      Double_t ww = gPad->GetWw();
      Double_t wh = gPad->GetWh();
      if (fType == 113) {
         ww *= gPad->GetWNDC();
         wh *= gPad->GetHNDC();
      }
      Double_t ratio = wh/ww;
      if (fType == 112) {
         xrange = fYsize;
         yrange = xrange*ratio;
         if (yrange > fXsize) { yrange = fXsize; xrange = yrange/ratio;}
      } else {
         xrange = fXsize;
         yrange = fXsize*ratio;
         if (yrange > fYsize) { yrange = fYsize; xrange = yrange/ratio;}
      }
      fXsize = xrange; fYsize = yrange;
   }

   // open OS file
   fStream = new ofstream(fname,ios::out);
   if (fStream == 0 || gSystem->AccessPathName(fname,kWritePermission)) {
      printf("ERROR in TPostScript::Open: Cannot open file:%s\n",fname);
      return;
   }
   gVirtualPS = this;

   for (Int_t i=0;i<fSizBuffer;i++) fBuffer[i] = ' ';
   if( fType == 113) {
      fBoundingBox = kFALSE;
      PrintStr("%!PS-Adobe-2.0 EPSF-2.0@");
   } else {
      fBoundingBox = kTRUE;
      PrintStr("%!PS-Adobe-2.0@");
      Initialize();
   }

   fClipStatus = kFALSE;
   fRange      = kFALSE;

   // Set a default range
   Range(fXsize, fYsize);

   fPrinted    = kFALSE;
   if (fType == 113) NewPage();
}


//______________________________________________________________________________
TPostScript::~TPostScript()
{
   // Default PostScript destructor

   Close();
}


//______________________________________________________________________________
void TPostScript::Close(Option_t *)
{
   // Close a PostScript file

   if (!gVirtualPS) return;
   if (!fStream) return;
   if (gPad) gPad->Update();
   if( fMode != 3) {
      SaveRestore(-1);
      if( fPrinted ) { PrintStr("showpage@"); SaveRestore(-1);}
      PrintStr("@");
      PrintStr("%%Trailer@");
      PrintStr("%%Pages: ");
      WriteInteger(fNpages);
      PrintStr("@");
      while (fSave > 0) { SaveRestore(-1); }
   } else {
      PrintStr("@");
      while (fSave > 0) { SaveRestore(-1); }
      PrintStr("showpage@");
      PrintStr("end@");
   }
   PrintStr("@");
   PrintStr("%%EOF@");

   // Close file stream

   if (fStream) { fStream->close(); delete fStream; fStream = 0;}

   gVirtualPS = 0;
}


//______________________________________________________________________________
void TPostScript::On()
{
   // Activate an already open PostScript file

   if (!fType) {
      Error("On", "no postscript file open");
      Off();
      return;
   }
   gVirtualPS = this;
}


//______________________________________________________________________________
void TPostScript::Off()
{
   // Deactivate an already open PostScript file

   gVirtualPS = 0;
}


//______________________________________________________________________________
void TPostScript::CellArrayBegin(Int_t W, Int_t /*H*/, Double_t x1, Double_t x2,
                                 Double_t y1, Double_t y2)
{
   // Draw a Cell Array
   //
   // Drawing a PostScript Cell Array is in fact done thanks to three
   // procedures: CellArrayBegin, CellArrayFill, and CellArrayEnd.
   //
   // CellArrayBegin: Initiate the Cell Array by writing the necessary
   //                 PostScript procedures and the initial values of the
   //                 required parameters. The input parameters are:
   //                 W: number of boxes along the width.
   //                 H: number of boxes along the height
   //                 x1,x2,y1,y2: First box coordinates.
   // CellArrayFill:  Is called for each box of the Cell Array. The first
   //                 box is the top left one and the last box is the
   //                 bottom right one. The input parameters are the Red,
   //                 Green, and Blue components of the box colour. These
   //                 Levels are between 0 and 255.
   // CellArrayEnd:   Finishes the Cell Array.
   //
   // PostScript cannot handle arrays larger than 65535. So the Cell Array
   // is drawn in several pieces.

   Int_t ix1 = XtoPS(x1);
   Int_t iy1 = YtoPS(y1);

   Float_t wt = (288/2.54)*gPad->GetAbsWNDC()*
                fXsize*((x2 - x1)/(gPad->GetX2()-gPad->GetX1()));
   Float_t ht = (288/2.54)*gPad->GetAbsHNDC()*
                fYsize*((y2 - y1)/(gPad->GetY2()-gPad->GetY1()));

   fLastCellRed     = 300;
   fLastCellGreen   = 300;
   fLastCellBlue    = 300;
   fNBSameColorCell = 0;

   fNbinCT = 0;
   fNbCellW = W;
   fNbCellLine = 0;
   fMaxLines = 40000/(3*fNbCellW);

   // Define some paremeters
   PrintStr("@/WT"); WriteReal(wt)          ; PrintStr(" def"); // Cells width
   PrintStr(" /HT"); WriteReal(ht)          ; PrintStr(" def"); // Cells height
   PrintStr(" /XS"); WriteInteger(ix1)      ; PrintStr(" def"); // X start
   PrintStr(" /YY"); WriteInteger(iy1)      ; PrintStr(" def"); // Y start
   PrintStr(" /NX"); WriteInteger(W)        ; PrintStr(" def"); // Number of columns
   PrintStr(" /NY"); WriteInteger(fMaxLines); PrintStr(" def"); // Number of lines

   // This PS procedure draws one cell.
   PrintStr(" /DrawCell ");
   PrintStr(   "{WT HT XX YY bf");
   PrintStr(   " /NBBD NBBD 1 add def");
   PrintStr(   " NBBD NBB eq {exit} if");
   PrintStr(   " /XX WT XX add def");
   PrintStr(   " IX NX eq ");
   PrintStr(      "{/YY YY HT sub def");
   PrintStr(      " /XX XS def");
   PrintStr(      " /IX 0 def} if");
   PrintStr(   " /IX IX 1 add def} def");

   // This PS procedure draws fMaxLines line. It takes care of duplicated
   // colors. Values "n" greater than 300 mean than the previous color
   // should be duplicated n-300 times.
   PrintStr(" /DrawCT ");
   PrintStr(   "{/NBB NX NY mul def");
   PrintStr(   " /XX XS def");
   PrintStr(   " /IX 1 def");
   PrintStr(   " /NBBD 0 def");
   PrintStr(   " /RC 0 def /GC 1 def /BC 2 def");
   PrintStr(   " 1 1 NBB ");
   PrintStr(      "{/NB CT RC get def");
   PrintStr(      " NB 301 ge ");
   PrintStr(         "{/NBL NB 300 sub def");
   PrintStr(         " 1 1 NBL ");
   PrintStr(            "{DrawCell}");
   PrintStr(         " for");
   PrintStr(         " /RC RC 1 add def");
   PrintStr(         " /GC RC 1 add def");
   PrintStr(         " /BC RC 2 add def}");
   PrintStr(         "{CT RC get 255 div CT GC get 255 div CT BC get 255 div setrgbcolor");
   PrintStr(         " DrawCell");
   PrintStr(         " /RC RC 3 add def");
   PrintStr(         " /GC GC 3 add def");
   PrintStr(         " /BC BC 3 add def} ifelse NBBD NBB eq {exit} if} for");
   PrintStr(         " /YY YY HT sub def clear} def");

   PrintStr(" /CT [");
}


//______________________________________________________________________________
void TPostScript::CellArrayFill(Int_t r, Int_t g, Int_t b)
{
   // Paint the Cell Array

   if (fLastCellRed == r && fLastCellGreen == g && fLastCellBlue == b) {
      fNBSameColorCell++;
   } else {
      if (fNBSameColorCell != 0 ) {
         WriteInteger(fNBSameColorCell+300);
         fNBSameColorCell = 0;
      }
      WriteInteger(r);
      WriteInteger(g);
      WriteInteger(b);
      fLastCellRed = r;
      fLastCellGreen = g;
      fLastCellBlue = b;
   }

   fNbinCT++;
   if (fNbinCT == fNbCellW) {
      fNbCellLine++;
      fNbinCT = 0;
   }

   if (fNbCellLine == fMaxLines) {
      if (fNBSameColorCell != 0) WriteInteger(fNBSameColorCell+300);
      PrintStr("] def DrawCT /CT [");
      fNbCellLine = 0;
      fLastCellRed = 300;
      fLastCellGreen = 300;
      fLastCellBlue = 300;
      fNBSameColorCell = 0;
      fNbinCT = 0;
   }
}


//______________________________________________________________________________
void TPostScript::CellArrayEnd()
{
   // End the Cell Array painting

   if (fNBSameColorCell != 0 ) WriteInteger(fNBSameColorCell+300);
   PrintStr("] def /NY");
   WriteInteger(fNbCellLine);
   PrintStr(" def DrawCT ");
}


//______________________________________________________________________________
void TPostScript::DefineMarkers()
{
   // Define the markers

   PrintStr("/mp {newpath /y exch def /x exch def} def@");
   PrintStr("/side {[w .77 mul w .23 mul] .385 w mul sd w 0 l currentpoint t -144 r} def@");
   PrintStr("/mr {mp x y w2 0 360 arc} def /m24 {mr s} def /m20 {mr f} def@");
   PrintStr("/mb {mp x y w2 add m w2 neg 0 d 0 w neg d w 0 d 0 w d cl} def@");
   PrintStr("/mt {mp x y w2 add m w2 neg w neg d w 0 d cl} def@");
   PrintStr("/m21 {mb f} def /m25 {mb s} def /m22 {mt f} def /m26{mt s} def@");
   PrintStr("/m23 {mp x y w2 sub m w2 w d w neg 0 d cl f} def@");
   PrintStr("/m27 {mp x y w2 add m w3 neg w2 neg d w3 w2 neg d w3 w2 d cl s} def@");
   PrintStr("/m28 {mp x w2 sub y w2 sub w3 add m w3 0 d ");
   PrintStr(" 0 w3 neg d w3 0 d 0 w3 d w3 0 d ");
   PrintStr(" 0 w3 d w3 neg 0 d 0 w3 d w3 neg 0 d");
   PrintStr(" 0 w3 neg d w3 neg 0 d cl s } def@");
   PrintStr("/m29 {mp gsave x w2 sub y w2 add w3 sub m currentpoint t");
   PrintStr(" 4 {side} repeat cl fill gr} def@");
   PrintStr("/m30 {mp gsave x w2 sub y w2 add w3 sub m currentpoint t");
   PrintStr(" 4 {side} repeat cl s gr} def@");
   PrintStr("/m31 {mp x y w2 sub m 0 w d x w2 sub y m w 0 d");
   PrintStr(" x w2 sub y w2 add m w w neg d x w2 sub y w2");
   PrintStr(" sub m w w d s} def@");
   PrintStr("/m32 {mp x y w2 sub m w2 w d w neg 0 d cl s} def@");
   PrintStr("/m33 {mp x y w2 add m w3 neg w2 neg d w3 w2 neg d w3 w2 d cl f} def@");
   PrintStr("/m34 {mp x w2 sub y w2 sub w3 add m w3 0 d ");
   PrintStr(" 0 w3 neg d w3 0 d 0 w3 d w3 0 d ");
   PrintStr(" 0 w3 d w3 neg 0 d 0 w3 d w3 neg 0 d");
   PrintStr(" 0 w3 neg d w3 neg 0 d cl f } def@");
   PrintStr("/m2 {mp x y w2 sub m 0 w d x w2 sub y m w 0 d s} def@");
   PrintStr("/m5 {mp x w2 sub y w2 sub m w w d x w2 sub y w2 add m w w neg d s} def@");
}


//______________________________________________________________________________
void TPostScript::DrawBox(Double_t x1, Double_t y1, Double_t x2, Double_t  y2)
{
   // Draw a Box

   static Double_t x[4], y[4];
   Int_t ix1 = XtoPS(x1);
   Int_t ix2 = XtoPS(x2);
   Int_t iy1 = YtoPS(y1);
   Int_t iy2 = YtoPS(y2);
   Int_t fillis = fFillStyle/1000;
   Int_t fillsi = fFillStyle%1000;

   if (fillis == 3 || fillis == 2) {
      if (fillsi > 99) {
         x[0] = x1;   y[0] = y1;
         x[1] = x2;   y[1] = y1;
         x[2] = x2;   y[2] = y2;
         x[3] = x1;   y[3] = y2;
         return;
      }
      if (fillsi > 0 && fillsi < 26) {
         x[0] = x1;   y[0] = y1;
         x[1] = x2;   y[1] = y1;
         x[2] = x2;   y[2] = y2;
         x[3] = x1;   y[3] = y2;
         DrawPS(-4, &x[0], &y[0]);
      }
      if (fillsi == -3) {
         SetColor(5);
         WriteInteger(ix2 - ix1);
         WriteInteger(iy2 - iy1);
         WriteInteger(ix1);
         WriteInteger(iy1);
         PrintFast(3," bf");
      }
   }
   if (fillis == 1) {
      SetColor(fFillColor);
      WriteInteger(ix2 - ix1);
      WriteInteger(iy2 - iy1);
      WriteInteger(ix1);
      WriteInteger(iy1);
      PrintFast(3," bf");
   }
   if (fillis == 0) {
      SetColor(fLineColor);
      WriteInteger(ix2 - ix1);
      WriteInteger(iy2 - iy1);
      WriteInteger(ix1);
      WriteInteger(iy1);
      PrintFast(3," bl");
   }
}


//______________________________________________________________________________
void TPostScript::DrawFrame(Double_t xl, Double_t yl, Double_t xt, Double_t  yt,
                            Int_t mode, Int_t border, Int_t dark, Int_t light)
{
   // Draw a Frame around a box
   //
   // mode = -1  box looks as it is behind the screen
   // mode =  1  box looks as it is in front of the screen
   // border is the border size in already precomputed PostScript units
   // dark  is the color for the dark part of the frame
   // light is the color for the light part of the frame

   static Int_t xps[7], yps[7];
   Int_t i, ixd0, iyd0, idx, idy, ixdi, iydi, ix, iy;

   // Draw top&left part of the box
   if (mode == -1) SetColor(dark);
   else            SetColor(light);
   Int_t bordPS = 4*border;
   xps[0] = XtoPS(xl);          yps[0] = YtoPS(yl);
   xps[1] = xps[0] + bordPS;    yps[1] = yps[0] + bordPS;
   xps[2] = xps[1];             yps[2] = YtoPS(yt) - bordPS;
   xps[3] = XtoPS(xt) - bordPS; yps[3] = yps[2];
   xps[4] = XtoPS(xt);          yps[4] = YtoPS(yt);
   xps[5] = xps[0];             yps[5] = yps[4];
   xps[6] = xps[0];             yps[6] = yps[0];

   ixd0 = xps[0];
   iyd0 = yps[0];
   WriteInteger(ixd0);
   WriteInteger(iyd0);

   PrintFast(2," m");
   idx = 0;
   idy = 0;
   for (i=1;i<7;i++) {
      ixdi = xps[i];
      iydi = yps[i];
      ix   = ixdi - ixd0;
      iy   = iydi - iyd0;
      ixd0 = ixdi;
      iyd0 = iydi;
      if( ix && iy) {
         if( idx ) { MovePS(idx,0); idx = 0; }
         if( idy ) { MovePS(0,idy); idy = 0; }
         MovePS(ix,iy);
         continue;
      }
      if ( ix ) {
         if( idy )  { MovePS(0,idy); idy = 0; }
         if( !idx ) { idx = ix; continue;}
         if( ix*idx > 0 )       idx += ix;
         else { MovePS(idx,0);  idx  = ix; }
         continue;
      }
      if( iy ) {
         if( idx ) { MovePS(idx,0); idx = 0; }
         if( !idy) { idy = iy; continue;}
         if( iy*idy > 0 )         idy += iy;
         else { MovePS(0,idy);    idy  = iy; }
      }
   }
   if( idx ) MovePS(idx,0);
   if( idy ) MovePS(0,idy);
   PrintFast(2," f");

   // Draw bottom&right part of the box
   if (mode == -1) SetColor(light);
   else            SetColor(dark);
   xps[0] = XtoPS(xl);          yps[0] = YtoPS(yl);
   xps[1] = xps[0] + bordPS;    yps[1] = yps[0] + bordPS;
   xps[2] = XtoPS(xt) - bordPS; yps[2] = yps[1];
   xps[3] = xps[2];             yps[3] = YtoPS(yt) - bordPS;
   xps[4] = XtoPS(xt);          yps[4] = YtoPS(yt);
   xps[5] = xps[4];             yps[5] = yps[0];
   xps[6] = xps[0];             yps[6] = yps[0];

   ixd0 = xps[0];
   iyd0 = yps[0];
   WriteInteger(ixd0);
   WriteInteger(iyd0);

   PrintFast(2," m");
   idx = 0;
   idy = 0;
   for (i=1;i<7;i++) {
      ixdi = xps[i];
      iydi = yps[i];
      ix   = ixdi - ixd0;
      iy   = iydi - iyd0;
      ixd0 = ixdi;
      iyd0 = iydi;
      if( ix && iy) {
         if( idx ) { MovePS(idx,0); idx = 0; }
         if( idy ) { MovePS(0,idy); idy = 0; }
         MovePS(ix,iy);
         continue;
      }
      if ( ix ) {
         if( idy )  { MovePS(0,idy); idy = 0; }
         if( !idx ) { idx = ix; continue;}
         if( ix*idx > 0 )       idx += ix;
         else { MovePS(idx,0);  idx  = ix; }
         continue;
      }
      if( iy ) {
         if( idx ) { MovePS(idx,0); idx = 0; }
         if( !idy) { idy = iy; continue;}
         if( iy*idy > 0 )         idy += iy;
         else { MovePS(0,idy);    idy  = iy; }
      }
   }
   if( idx ) MovePS(idx,0);
   if( idy ) MovePS(0,idy);
   PrintFast(2," f");
}


//______________________________________________________________________________
void TPostScript::DrawPolyLine(Int_t nn, TPoints *xy)
{
   // Draw a PolyLine
   //
   //  Draw a polyline through  the points xy.
   //  If nn=1 moves only to point x,y.
   //  If nn=0 the x,y are  written  in the PostScript file
   //          according to the current transformation.
   //  If nn>0 the line is clipped as a line.
   //  If nn<0 the line is clipped as a fill area.

   Int_t  i, n, ixd0, iyd0, idx, idy, ixdi, iydi, ix, iy;
   Style_t linestylesav = fLineStyle;
   Width_t linewidthsav = fLineWidth;
   if (nn > 0) {
      n = nn;
      SetLineStyle(fLineStyle);
      SetLineWidth(fLineWidth);
      SetColor(Int_t(fLineColor));
   } else {
      n = -nn;
      SetLineStyle(1);
      SetLineWidth(1);
      SetColor(Int_t(fLineColor));
   }

   ixd0 = XtoPS(xy[0].GetX());
   iyd0 = YtoPS(xy[0].GetY());
   WriteInteger(ixd0);
   WriteInteger(iyd0);
   if( n <= 1) {
      if( n == 0) goto END;
      PrintFast(2," m");
      goto END;
   }

   PrintFast(2," m");
   idx = 0;
   idy = 0;
   for (i=1;i<n;i++) {
      ixdi = XtoPS(xy[i].GetX());
      iydi = YtoPS(xy[i].GetY());
      ix   = ixdi - ixd0;
      iy   = iydi - iyd0;
      ixd0 = ixdi;
      iyd0 = iydi;
      if( ix && iy) {
         if( idx ) { MovePS(idx,0); idx = 0; }
         if( idy ) { MovePS(0,idy); idy = 0; }
         MovePS(ix,iy);
         continue;
      }
      if ( ix ) {
         if( idy )  { MovePS(0,idy); idy = 0; }
         if( !idx ) { idx = ix; continue;}
         if( ix*idx > 0 )       idx += ix;
         else { MovePS(idx,0);  idx  = ix; }
         continue;
      }
      if( iy ) {
         if( idx ) { MovePS(idx,0); idx = 0; }
         if( !idy) { idy = iy; continue;}
         if( iy*idy > 0 )         idy += iy;
         else { MovePS(0,idy);    idy  = iy; }
      }
   }
   if( idx ) MovePS(idx,0);
   if( idy ) MovePS(0,idy);

   if (nn > 0 ) {
      if (xy[0].GetX() == xy[n-1].GetX() && xy[0].GetY() == xy[n-1].GetY()) PrintFast(3," cl");
      PrintFast(2," s");
   } else {
      PrintFast(2," f");
   }
END:
   if (nn < 0) {
      SetLineStyle(linestylesav);
      SetLineWidth(linewidthsav);
   }
}


//______________________________________________________________________________
void TPostScript::DrawPolyLineNDC(Int_t nn, TPoints *xy)
{
   // Draw a PolyLine in NDC space
   //
   //  Draw a polyline through the points xy.
   //  If nn=1 moves only to point x,y.
   //  If nn=0 the x,y are  written  in the PostScript file
   //          according to the current transformation.
   //  If nn>0 the line is clipped as a line.
   //  If nn<0 the line is clipped as a fill area.

   Int_t  i, n, ixd0, iyd0, idx, idy, ixdi, iydi, ix, iy;
   Style_t linestylesav = fLineStyle;
   Width_t linewidthsav = fLineWidth;
   if (nn > 0) {
      n = nn;
      SetLineStyle(fLineStyle);
      SetLineWidth(fLineWidth);
      SetColor(Int_t(fLineColor));
   } else {
      n = -nn;
      SetLineStyle(1);
      SetLineWidth(1);
      SetColor(Int_t(fLineColor));
   }

   ixd0 = UtoPS(xy[0].GetX());
   iyd0 = VtoPS(xy[0].GetY());
   WriteInteger(ixd0);
   WriteInteger(iyd0);
   if( n <= 1) {
      if( n == 0) goto END;
      PrintFast(2," m");
      goto END;
   }

   PrintFast(2," m");
   idx = 0;
   idy = 0;
   for (i=1;i<n;i++) {
      ixdi = UtoPS(xy[i].GetX());
      iydi = VtoPS(xy[i].GetY());
      ix   = ixdi - ixd0;
      iy   = iydi - iyd0;
      ixd0 = ixdi;
      iyd0 = iydi;
      if( ix && iy) {
         if( idx ) { MovePS(idx,0); idx = 0; }
         if( idy ) { MovePS(0,idy); idy = 0; }
         MovePS(ix,iy);
         continue;
      }
      if ( ix ) {
         if( idy )  { MovePS(0,idy); idy = 0; }
         if( !idx ) { idx = ix; continue;}
         if( ix*idx > 0 )       idx += ix;
         else { MovePS(idx,0);  idx  = ix; }
         continue;
      }
      if( iy ) {
         if( idx ) { MovePS(idx,0); idx = 0; }
         if( !idy) { idy = iy; continue;}
         if( iy*idy > 0 )         idy += iy;
         else { MovePS(0,idy);    idy  = iy; }
      }
   }
   if( idx ) MovePS(idx,0);
   if( idy ) MovePS(0,idy);

   if (nn > 0 ) {
      if (xy[0].GetX() == xy[n-1].GetX() && xy[0].GetY() == xy[n-1].GetY()) PrintFast(3," cl");
      PrintFast(2," s");
   } else {
      PrintFast(2," f");
   }
END:
   if (nn < 0) {
      SetLineStyle(linestylesav);
      SetLineWidth(linewidthsav);
   }
}


//______________________________________________________________________________
void TPostScript::DrawPolyMarker(Int_t n, Float_t *x, Float_t *y)
{
   // Draw markers at the n WC points x, y

   Int_t i, np, markerstyle;
   Float_t markersize;
   static char chtemp[10];

   if (!fMarkerSize) return;
   Style_t linestylesav = fLineStyle;
   Width_t linewidthsav = fLineWidth;
   SetLineStyle(1);
   SetLineWidth(1);
   SetColor(Int_t(fMarkerColor));
   markerstyle = abs(fMarkerStyle);
   if (markerstyle <= 0) strlcpy(chtemp, " m20",10);
   if (markerstyle == 1) strlcpy(chtemp, " m20",10);
   if (markerstyle == 2) strlcpy(chtemp, " m2",10);
   if (markerstyle == 3) strlcpy(chtemp, " m31",10);
   if (markerstyle == 4) strlcpy(chtemp, " m24",10);
   if (markerstyle == 5) strlcpy(chtemp, " m5",10);
   if (markerstyle >= 6 && markerstyle <= 19) strlcpy(chtemp, " m20",10);
   if (markerstyle >= 20 && markerstyle <= 34 ) snprintf(chtemp,10," m%d", markerstyle);
   if (markerstyle >= 35) strlcpy(chtemp, " m20",10);

   // Set the PostScript marker size
   if (markerstyle == 1) {
      markersize = 2.;
   } else if (markerstyle == 6) {
      markersize = 4.;
   } else if (markerstyle == 7) {
      markersize = 8.;
   } else {
      Float_t symbolsize  = fMarkerSize;
      const Int_t kBASEMARKER = 8;
      Float_t sbase = symbolsize*kBASEMARKER;
      Float_t s2x = sbase / Float_t(gPad->GetWw() * gPad->GetAbsWNDC());
      markersize = this->UtoPS(s2x) - this->UtoPS(0);
   }

   if (fMarkerSizeCur != markersize) {
      fMarkerSizeCur = markersize;
      PrintFast(3," /w");
      WriteInteger(Int_t(markersize+0.5));
      PrintFast(40," def /w2 {w 2 div} def /w3 {w 3 div} def");
   }

   WriteInteger(XtoPS(x[0]));
   WriteInteger(YtoPS(y[0]));
   if (n == 1) {
      PrintStr(chtemp);
      SetLineStyle(linestylesav);
      SetLineWidth(linewidthsav);
      return;
   }
   np = 1;
   for (i=1;i<n;i++) {
      WriteInteger(XtoPS(x[i]));
      WriteInteger(YtoPS(y[i]));
      np++;
      if (np == 100 || i == n-1) {
         WriteInteger(np);
         PrintFast(2," {");
         PrintStr(chtemp);
         PrintFast(3,"} R");
         np = 0;
      }
   }
   SetLineStyle(linestylesav);
   SetLineWidth(linewidthsav);
}


//______________________________________________________________________________
void TPostScript::DrawPolyMarker(Int_t n, Double_t *x, Double_t *y)
{
   // Draw markers at the n WC points x, y

   Int_t i, np, markerstyle;
   Float_t markersize;
   static char chtemp[10];

   if (!fMarkerSize) return;
   Style_t linestylesav = fLineStyle;
   Width_t linewidthsav = fLineWidth;
   SetLineStyle(1);
   SetLineWidth(1);
   SetColor(Int_t(fMarkerColor));
   markerstyle = abs(fMarkerStyle);
   if (markerstyle <= 0) strlcpy(chtemp, " m20",10);
   if (markerstyle == 1) strlcpy(chtemp, " m20",10);
   if (markerstyle == 2) strlcpy(chtemp, " m2",10);
   if (markerstyle == 3) strlcpy(chtemp, " m31",10);
   if (markerstyle == 4) strlcpy(chtemp, " m24",10);
   if (markerstyle == 5) strlcpy(chtemp, " m5",10);
   if (markerstyle >= 6 && markerstyle <= 19) strlcpy(chtemp, " m20",10);
   if (markerstyle >= 20 && markerstyle <= 34 ) snprintf(chtemp,10," m%d", markerstyle);
   if (markerstyle >= 35) strlcpy(chtemp, " m20",10);

   // Set the PostScript marker size
   if (markerstyle == 1) {
      markersize = 2.;
   } else if (markerstyle == 6) {
      markersize = 4.;
   } else if (markerstyle == 7) {
      markersize = 8.;
   } else {
      Float_t symbolsize  = fMarkerSize;
      const Int_t kBASEMARKER = 8;
      Float_t sbase = symbolsize*kBASEMARKER;
      Float_t s2x = sbase / Float_t(gPad->GetWw() * gPad->GetAbsWNDC());
      markersize = this->UtoPS(s2x) - this->UtoPS(0);
   }

   if (fMarkerSizeCur != markersize) {
      fMarkerSizeCur = markersize;
      PrintFast(3," /w");
      WriteInteger(Int_t(markersize+0.5));
      PrintFast(40," def /w2 {w 2 div} def /w3 {w 3 div} def");
   }

   WriteInteger(XtoPS(x[0]));
   WriteInteger(YtoPS(y[0]));
   if (n == 1) {
      PrintStr(chtemp);
      SetLineStyle(linestylesav);
      SetLineWidth(linewidthsav);
      return;
   }
   np = 1;
   for (i=1;i<n;i++) {
      WriteInteger(XtoPS(x[i]));
      WriteInteger(YtoPS(y[i]));
      np++;
      if (np == 100 || i == n-1) {
         WriteInteger(np);
         PrintFast(2," {");
         PrintStr(chtemp);
         PrintFast(3,"} R");
         np = 0;
      }
   }
   SetLineStyle(linestylesav);
   SetLineWidth(linewidthsav);
}


//______________________________________________________________________________
void TPostScript::DrawPS(Int_t nn, Float_t *xw, Float_t *yw)
{
   // Draw a PolyLine
   //
   //  Draw a polyline through the points xw,yw.
   //  If nn=1 moves only to point xw,yw.
   //  If nn=0 the XW(1) and YW(1) are  written  in the PostScript file
   //          according to the current NT.
   //  If nn>0 the line is clipped as a line.
   //  If nn<0 the line is clipped as a fill area.

   static Float_t dyhatch[24] = {.0075,.0075,.0075,.0075,.0075,.0075,.0075,.0075,
                                 .01  ,.01  ,.01  ,.01  ,.01  ,.01  ,.01  ,.01  ,
                                 .015 ,.015 ,.015 ,.015 ,.015 ,.015 ,.015 ,.015};
   static Float_t anglehatch[24] = {180, 90,135, 45,150, 30,120, 60,
                                    180, 90,135, 45,150, 30,120, 60,
                                    180, 90,135, 45,150, 30,120, 60};
   Int_t  i, n, ixd0, iyd0, idx, idy, ixdi, iydi, ix, iy, fais, fasi;
   fais = fasi = n = 0;
   Int_t jxd0 = XtoPS(xw[0]);
   Int_t jyd0 = YtoPS(yw[0]);
   Style_t linestylesav = fLineStyle;
   Width_t linewidthsav = fLineWidth;

   if (nn > 0) {
      n = nn;
      SetLineStyle(fLineStyle);
      SetLineWidth(fLineWidth);
      SetColor(Int_t(fLineColor));
   }
   if (nn < 0) {
      n = -nn;
      SetLineStyle(1);
      SetLineWidth(1);
      SetColor(Int_t(fFillColor));
      fais = fFillStyle/1000;
      fasi = fFillStyle%1000;
      if (fais == 3 || fais == 2) {
         if (fasi > 100 && fasi <125) {
            DrawHatch(dyhatch[fasi-101],anglehatch[fasi-101], n, xw, yw);
            goto END;
         }
         if (fasi > 0 && fasi < 26) {
            SetFillPatterns(fasi, Int_t(fFillColor));
         }
      }
   }

   ixd0 = jxd0;
   iyd0 = jyd0;
   WriteInteger(ixd0);
   WriteInteger(iyd0);
   if( n <= 1) {
      if( n == 0) goto END;
      PrintFast(2," m");
      goto END;
   }

   PrintFast(2," m");
   idx = idy = 0;
   for (i=1;i<n;i++) {
      ixdi = XtoPS(xw[i]);
      iydi = YtoPS(yw[i]);
      ix   = ixdi - ixd0;
      iy   = iydi - iyd0;
      ixd0 = ixdi;
      iyd0 = iydi;
      if( ix && iy) {
         if( idx ) { MovePS(idx,0); idx = 0; }
         if( idy ) { MovePS(0,idy); idy = 0; }
         MovePS(ix,iy);
      } else if ( ix ) {
         if( idy )  { MovePS(0,idy); idy = 0;}
         if( !idx ) { idx = ix;}
         else if( TMath::Sign(ix,idx) == ix )       idx += ix;
         else { MovePS(idx,0);  idx  = ix;}
      } else if( iy ) {
         if( idx ) { MovePS(idx,0); idx = 0;}
         if( !idy) { idy = iy;}
         else if( TMath::Sign(iy,idy) == iy)         idy += iy;
         else { MovePS(0,idy);    idy  = iy;}
      }
   }
   if (idx) MovePS(idx,0);
   if (idy) MovePS(0,idy);

   if (nn > 0 ) {
      if (xw[0] == xw[n-1] && yw[0] == yw[n-1]) PrintFast(3," cl");
      PrintFast(2," s");
   } else {
      if (fais == 0) {PrintFast(5," cl s"); goto END;}
      if (fais == 3 || fais == 2) {
         if (fasi > 0 && fasi < 26) {
            PrintFast(3," FA");
            fRed   = -1;
            fGreen = -1;
            fBlue  = -1;
         }
         goto END;
      }
      PrintFast(2," f");
   }
END:
   if (nn < 0) {
      SetLineStyle(linestylesav);
      SetLineWidth(linewidthsav);
   }
}


//______________________________________________________________________________
void TPostScript::DrawPS(Int_t nn, Double_t *xw, Double_t *yw)
{
   // Draw a PolyLine
   //
   // Draw a polyline through  the points xw,yw.
   // If nn=1 moves only to point xw,yw.
   // If nn=0 the xw(1) and YW(1) are  written  in the PostScript file
   //         according to the current NT.
   // If nn>0 the line is clipped as a line.
   // If nn<0 the line is clipped as a fill area.

   static Float_t dyhatch[24] = {.0075,.0075,.0075,.0075,.0075,.0075,.0075,.0075,
                                 .01  ,.01  ,.01  ,.01  ,.01  ,.01  ,.01  ,.01  ,
                                 .015 ,.015 ,.015 ,.015 ,.015 ,.015 ,.015 ,.015};
   static Float_t anglehatch[24] = {180, 90,135, 45,150, 30,120, 60,
                                    180, 90,135, 45,150, 30,120, 60,
                                    180, 90,135, 45,150, 30,120, 60};
   Int_t  i, n, ixd0, iyd0, idx, idy, ixdi, iydi, ix, iy, fais, fasi;
   fais = fasi = n = 0;
   Int_t jxd0 = XtoPS(xw[0]);
   Int_t jyd0 = YtoPS(yw[0]);
   Style_t linestylesav = fLineStyle;
   Width_t linewidthsav = fLineWidth;

   if (nn > 0) {
      n = nn;
      SetLineStyle(fLineStyle);
      SetLineWidth(fLineWidth);
      SetColor(Int_t(fLineColor));
   }
   if (nn < 0) {
      n = -nn;
      SetLineStyle(1);
      SetLineWidth(1);
      SetColor(Int_t(fFillColor));
      fais = fFillStyle/1000;
      fasi = fFillStyle%1000;
      if (fais == 3 || fais == 2) {
         if (fasi > 100 && fasi <125) {
            DrawHatch(dyhatch[fasi-101],anglehatch[fasi-101], n, xw, yw);
            goto END;
         }
         if (fasi > 0 && fasi < 26) {
            SetFillPatterns(fasi, Int_t(fFillColor));
         }
      }
   }

   ixd0 = jxd0;
   iyd0 = jyd0;
   WriteInteger(ixd0);
   WriteInteger(iyd0);
   if( n <= 1) {
      if( n == 0) goto END;
      PrintFast(2," m");
      goto END;
   }

   PrintFast(2," m");
   idx = idy = 0;
   for (i=1;i<n;i++) {
      ixdi = XtoPS(xw[i]);
      iydi = YtoPS(yw[i]);
      ix   = ixdi - ixd0;
      iy   = iydi - iyd0;
      ixd0 = ixdi;
      iyd0 = iydi;
      if( ix && iy) {
         if( idx ) { MovePS(idx,0); idx = 0; }
         if( idy ) { MovePS(0,idy); idy = 0; }
         MovePS(ix,iy);
      } else if ( ix ) {
         if( idy )  { MovePS(0,idy); idy = 0;}
         if( !idx ) { idx = ix;}
         else if( TMath::Sign(ix,idx) == ix )       idx += ix;
         else { MovePS(idx,0);  idx  = ix;}
      } else if( iy ) {
         if( idx ) { MovePS(idx,0); idx = 0;}
         if( !idy) { idy = iy;}
         else if( TMath::Sign(iy,idy) == iy)         idy += iy;
         else { MovePS(0,idy);    idy  = iy;}
      }
   }
   if (idx) MovePS(idx,0);
   if (idy) MovePS(0,idy);

   if (nn > 0 ) {
      if (xw[0] == xw[n-1] && yw[0] == yw[n-1]) PrintFast(3," cl");
      PrintFast(2," s");
   } else {
      if (fais == 0) {PrintFast(5," cl s"); goto END;}
      if (fais == 3 || fais == 2) {
         if (fasi > 0 && fasi < 26) {
            PrintFast(3," FA");
            fRed   = -1;
            fGreen = -1;
            fBlue  = -1;
         }
         goto END;
      }
      PrintFast(2," f");
   }
END:
   if (nn < 0) {
      SetLineStyle(linestylesav);
      SetLineWidth(linewidthsav);
   }
}


//______________________________________________________________________________
void TPostScript::DrawHatch(Float_t, Float_t, Int_t, Float_t *, Float_t *)
{
   // Draw Fill area with hatch styles

   Warning("DrawHatch", "hatch fill style not yet implemented");
}


//______________________________________________________________________________
void TPostScript::DrawHatch(Float_t, Float_t, Int_t, Double_t *, Double_t *)
{
   // Draw Fill area with hatch styles

   Warning("DrawHatch", "hatch fill style not yet implemented");
}


//______________________________________________________________________________
void TPostScript::FontEncode()
{
   // Font Reencoding

   PrintStr("/reEncode ");
   PrintStr("{exch findfont");
   PrintStr(" dup length dict begin");
   PrintStr(" {1 index /FID eq ");
   PrintStr(" {pop pop}");
   PrintStr(" {def} ifelse");
   PrintStr(" } forall");
   PrintStr(" /Encoding exch def");
   PrintStr(" currentdict end");
   PrintStr(" dup /FontName get exch");
   PrintStr(" definefont pop");
   PrintStr(" } def");
   PrintStr(" [/Times-Bold /Times-Italic /Times-BoldItalic /Helvetica");
   PrintStr(" /Helvetica-Oblique /Helvetica-Bold /Helvetica-BoldOblique");
   PrintStr(" /Courier /Courier-Oblique /Courier-Bold /Courier-BoldOblique");
   PrintStr(" /Times-Roman /AvantGarde-Book /AvantGarde-BookOblique");
   PrintStr(" /AvantGarde-Demi /AvantGarde-DemiOblique /Bookman-Demi");
   PrintStr(" /Bookman-DemiItalic /Bookman-Light /Bookman-LightItalic");
   PrintStr(" /Helvetica-Narrow /Helvetica-Narrow-Bold /Helvetica-Narrow-BoldOblique");
   PrintStr(" /Helvetica-Narrow-Oblique /NewCenturySchlbk-Roman /NewCenturySchlbk-Bold");
   PrintStr(" /NewCenturySchlbk-BoldItalic /NewCenturySchlbk-Italic");
   PrintStr(" /Palatino-Bold /Palatino-BoldItalic /Palatino-Italic /Palatino-Roman");
   PrintStr(" ] {ISOLatin1Encoding reEncode } forall");
}


//______________________________________________________________________________
void TPostScript::Initialize()
{
   // PostScript Initialisation
   //
   // This routine initialize the following PostScript procedures:
   //
   // +------------+------------------+-----------------------------------+
   // | Macro Name | Input parameters |            Explanation            |
   // +------------+------------------+-----------------------------------+
   // |     l      | x y              | Draw a line to the x y position   |
   // +------------+------------------+-----------------------------------+
   // |     m      | x y              | Move to the position x y          |
   // +------------+------------------+-----------------------------------+
   // |     box    | dx dy x y        | Define a box                      |
   // +------------+------------------+-----------------------------------+
   // |     bl     | dx dy x y        | Draw a line box                   |
   // +------------+------------------+-----------------------------------+
   // |     bf     | dx dy x y        | Draw a filled box                 |
   // +------------+------------------+-----------------------------------+
   // |     t      | x y              | Translate                         |
   // +------------+------------------+-----------------------------------+
   // |     r      | angle            | Rotate                            |
   // +------------+------------------+-----------------------------------+
   // |     rl     | i j              | Roll the stack                    |
   // +------------+------------------+-----------------------------------+
   // |     d      | x y              | Draw a relative line to x y       |
   // +------------+------------------+-----------------------------------+
   // |     X      | x                | Draw a relative line to x (y=0)   |
   // +------------+------------------+-----------------------------------+
   // |     Y      | y                | Draw a relative line to y (x=0)   |
   // +------------+------------------+-----------------------------------+
   // |     rm     | x y              | Move relatively to x y            |
   // +------------+------------------+-----------------------------------+
   // |     gr     |                  | Restore the graphic context       |
   // +------------+------------------+-----------------------------------+
   // |     lw     | lwidth           | Set line width to lwidth          |
   // +------------+------------------+-----------------------------------+
   // |     sd     | [] 0             | Set dash line define by []        |
   // +------------+------------------+-----------------------------------+
   // |     s      |                  | Stroke mode                       |
   // +------------+------------------+-----------------------------------+
   // |     c      | r g b            | Set rgb color to r g b            |
   // +------------+------------------+-----------------------------------+
   // |     cl     |                  | Close path                        |
   // +------------+------------------+-----------------------------------+
   // |     f      |                  | Fill the last describe path       |
   // +------------+------------------+-----------------------------------+
   // |     mXX    | x y              | Draw the marker type XX at (x,y)  |
   // +------------+------------------+-----------------------------------+
   // |     Zone   | ix iy            | Define the current zone           |
   // +------------+------------------+-----------------------------------+
   // |     black  |                  | The color is black                |
   // +------------+------------------+-----------------------------------+
   // |     C      | dx dy x y        | Clipping on                       |
   // +------------+------------------+-----------------------------------+
   // |     NC     |                  | Clipping off                      |
   // +------------+------------------+-----------------------------------+
   // |     R      |                  | repeat                            |
   // +------------+------------------+-----------------------------------+
   // |     ita    |                  | Used to make the symbols italic   |
   // +------------+------------------+-----------------------------------+
   // |     K      |                  | kshow                             |
   // +------------+------------------+-----------------------------------+

   Double_t rpxmin, rpymin, width, heigth;
   rpxmin = rpymin = width = heigth = 0;
   Int_t format;
   fNpages=1;
   for (Int_t i=0;i<32;i++) fPatterns[i]=0;

   // Mode is last digit of PostScript Workstation type
   //    mode=1,2 for portrait/landscape black and white
   //    mode=3 for Encapsulated PostScript File
   //    mode=4 for portrait colour
   //    mode=5 for lanscape colour
   Int_t atype = abs(fType);
   fMode       = atype%10;
   if( fMode <= 0 || fMode > 5) {
      Error("Initialize", "invalid file type %d", fMode);
      return;
   }

   // fNXzone (fNYzone) is the total number of windows in x (y)
   fNXzone = (atype%1000)/100;
   fNYzone = (atype%100)/10;
   if( fNXzone <= 0 ) fNXzone = 1;
   if( fNYzone <= 0 ) fNYzone = 1;
   fIXzone = 1;
   fIYzone = 1;

   // format = 0-99 is the European page format (A4,A3 ...)
   // format = 100 is the US format  8.5x11.0 inch
   // format = 200 is the US format  8.5x14.0 inch
   // format = 300 is the US format 11.0x17.0 inch
   format = atype/1000;
   if( format == 0 )  format = 4;
   if( format == 99 ) format = 0;

   PrintStr("%%Title: ");
   const char *pstitle = gStyle->GetTitlePS();
   if (gPad && !strlen(pstitle)) pstitle = gPad->GetMother()->GetTitle();
   if (strlen(GetName())<=80) PrintStr(GetName());
   if(!strlen(pstitle) && fMode != 3) {;
      PrintFast(2," (");
      if ( format <= 99 ) {;
         PrintFast(2," A");
         WriteInteger(format);
         PrintFast(1,")");
      }
      else {
         if ( format == 100 ) PrintFast(8," Letter)");
         if ( format == 200 ) PrintFast(7," Legal)");
         if ( format == 300 ) PrintFast(8," Ledger)");
      }
      PrintStr("@");
      PrintStr("%%Pages: (atend)@");
   }
   else {
         if (!strchr(pstitle,'\n')) {
         PrintFast(2,": ");
         PrintStr(pstitle);
      }
      PrintStr("@");
   }

   PrintFast(24,"%%Creator: ROOT Version ");
   PrintStr(gROOT->GetVersion());
   PrintStr("@");
   PrintFast(16,"%%CreationDate: ");
   TDatime t;
   PrintStr(t.AsString());
   PrintStr("@");

   if ( fMode == 1 || fMode == 4) PrintStr("%%Orientation: Portrait@");
   if ( fMode == 2 || fMode == 5) PrintStr("%%Orientation: Landscape@");

   PrintStr("%%EndComments@");
   PrintStr("%%BeginProlog@");

   if( fMode == 3)PrintStr("80 dict begin@");

   // Initialization of PostScript procedures
   PrintStr("/s {stroke} def /l {lineto} def /m {moveto} def /t {translate} def@");
   PrintStr("/r {rotate} def /rl {roll}  def /R {repeat} def@");
   PrintStr("/d {rlineto} def /rm {rmoveto} def /gr {grestore} def /f {eofill} def@");
   if (gStyle->GetColorModelPS()) {
      PrintStr("/c {setcmykcolor} def /black {0 0 0 1 setcmykcolor} def /sd {setdash} def@");
   } else {
      PrintStr("/c {setrgbcolor} def /black {0 setgray} def /sd {setdash} def@");
   }
   PrintStr("/cl {closepath} def /sf {scalefont setfont} def /lw {setlinewidth} def@");
   PrintStr("/box {m dup 0 exch d exch 0 d 0 exch neg d cl} def@");
   PrintStr("/NC{systemdict begin initclip end}def/C{NC box clip newpath}def@");
   PrintStr("/bl {box s} def /bf {box f} def /Y { 0 exch d} def /X { 0 d} def @");
   PrintStr("/K {{pop pop 0 moveto} exch kshow} bind def@");
   PrintStr("/ita {/ang 15 def gsave [1 0 ang dup sin exch cos div 1 0 0] concat} def @");

   DefineMarkers();
   FontEncode();

   // mode=1 for portrait black/white
   if (fMode == 1)  {
      rpxmin = 0.7;
      rpymin = TMath::Sqrt(2.)*rpxmin;
      switch (format) {
         case 100 :
            width  = (8.5*2.54)-2.*rpxmin;
            heigth = (11.*2.54)-2.*rpymin;
            break;
         case 200 :
            width  = (8.5*2.54)-2.*rpxmin;
            heigth = (14.*2.54)-2.*rpymin;
            break;
         case 300 :
            width  = (11.*2.54)-2.*rpxmin;
            heigth = (17.*2.54)-2.*rpymin;
            break;
         default  :
            width  = 21.0-2.*rpxmin;
            heigth = 29.7-2.*rpymin;
      };
   }

   // mode=2 for landscape black/white
   if (fMode == 2)  {
      rpymin = 0.7;
      rpxmin = TMath::Sqrt(2.)*rpymin;
      switch (format) {
         case 100 :
            width  = (11.*2.54)-2.*rpxmin;
            heigth = (8.5*2.54)-2.*rpymin;
         case 200 :
            width  = (14.*2.54)-2.*rpxmin;
            heigth = (8.5*2.54)-2.*rpymin;
         case 300 :
            width  = (17.*2.54)-2.*rpxmin;
            heigth = (11.*2.54)-2.*rpymin;
         default  :
            width  = 29.7-2.*rpxmin;
            heigth = 21-2.*rpymin;
      };
   }

   // mode=3 encapsulated PostScript
   if (fMode == 3)  {
      width   = 20;
      heigth  = 20;
      format  = 4;
      fNXzone = 1;
      fNYzone = 1;
   }

   // mode=4 for portrait colour
   if (fMode == 4)  {
      rpxmin = 0.7;
      rpymin = 3.4;
      switch (format) {
         case 100 :
            width  = (8.5*2.54)-2.*rpxmin;
            heigth = (11.*2.54)-2.*rpymin;
         case 200 :
            width  = (8.5*2.54)-2.*rpxmin;
            heigth = (14.*2.54)-2.*rpymin;
         case 300 :
            width  = (11.*2.54)-2.*rpxmin;
            heigth = (17.*2.54)-2.*rpymin;
         default  :
            width  = (21.0-2*rpxmin);
            heigth = (29.7-2.*rpymin);
      };
   }

   // mode=5 for lanscape colour
   if (fMode == 5)  {
      rpxmin = 3.4;
      rpymin = 0.7;
      switch (format) {
         case 100 :
            width  = (11.*2.54)-2.*rpxmin;
            heigth = (8.5*2.54)-2.*rpymin;
         case 200 :
            width  = (14.*2.54)-2.*rpxmin;
            heigth = (8.5*2.54)-2.*rpymin;
         case 300 :
            width  = (17.*2.54)-2.*rpxmin;
            heigth = (11.*2.54)-2.*rpymin;
         default  :
            width  = (29.7-2*rpxmin);
            heigth = (21-2.*rpymin);
      };
   }

   Double_t value = 0;
   if       (format <  100) value = 21*TMath::Power(TMath::Sqrt(2.), 4-format);
   else  if (format == 100) value = 8.5*2.54;
   else  if (format == 200) value = 8.5*2.54;
   else  if (format == 300) value = 11.*2.54;
   if       (format >= 100) format = 4;

   // Compute size (in points) of the window for each picture = f(fNXzone,fNYzone)
   Double_t sizex = width/Double_t(fNXzone)*TMath::Power(TMath::Sqrt(2.), 4-format);
   Double_t sizey = heigth/Double_t(fNYzone)*TMath::Power(TMath::Sqrt(2.), 4-format);
   Int_t npx      = 4*CMtoPS(sizex);
   Int_t npy      = 4*CMtoPS(sizey);
   if (sizex > sizey) fMaxsize = CMtoPS(sizex);
   else               fMaxsize = CMtoPS(sizey);

   // Procedure Zone
   if (fMode != 3)  {
      PrintFast(33,"/Zone {/iy exch def /ix exch def ");
      PrintFast(10," ix 1 sub ");
      WriteInteger(npx);
      PrintFast(5," mul ");
      WriteReal(Float_t(fNYzone));
      PrintFast(8," iy sub ");
      WriteInteger(npy);
      PrintStr(" mul t} def@");
   } else {
      PrintStr("@");
   }

   PrintStr("%%EndProlog@");
   PrintStr("%%BeginSetup@");
   PrintStr("%%EndSetup@");
   PrintFast(8,"newpath ");
   SaveRestore(1);
   if (fMode == 1 || fMode == 4)  {
      WriteInteger(CMtoPS(rpxmin));
      WriteInteger(CMtoPS(rpymin));
      PrintFast(2," t");
   }
   if (fMode == 2 || fMode == 5)  {
      PrintFast(7," 90 r 0");
      WriteInteger(CMtoPS(-value));
      PrintFast(3," t ");
      WriteInteger(CMtoPS(rpxmin));
      WriteInteger(CMtoPS(rpymin));
      PrintFast(2," t");
   }

   PrintFast(15," .25 .25 scale ");
   if (fMode != 3) {
      SaveRestore(1);
      PrintStr("@");
      PrintStr("%%Page: 1 1@");
      SaveRestore(1);
   }

   //Check is user has defined a special header in the current style
   Int_t nh = strlen(gStyle->GetHeaderPS());
   if (nh) {
      PrintFast(nh,gStyle->GetHeaderPS());
      if (fMode != 3) SaveRestore(1);
   }
}


//______________________________________________________________________________
void TPostScript::MovePS(Int_t ix, Int_t iy)
{
   // Move to a new position

   if (ix != 0 && iy != 0)  {
      WriteInteger(ix);
      WriteInteger(iy);
      PrintFast(2," d");
   } else if (ix != 0)  {
      WriteInteger(ix);
      PrintFast(2," X");
   } else if (iy != 0)  {
      WriteInteger(iy);
      PrintFast(2," Y");
   }
}


//______________________________________________________________________________
void TPostScript::NewPage()
{
   // Move to a new PostScript page

   //   Compute pad conversion coefficients
   if (gPad) {
//     if (!gPad->GetPadPaint()) gPad->Update();
      Double_t ww   = gPad->GetWw();
      Double_t wh   = gPad->GetWh();
      fYsize       = fXsize*wh/ww;
   } else fYsize = 27;

   if(fType  == 113 && !fBoundingBox) {
      Bool_t psave = fPrinted;
      PrintStr("@%%BoundingBox: ");
      Double_t xlow=0, ylow=0, xup=1, yup=1;
      if (gPad) {
         xlow = gPad->GetAbsXlowNDC();
         xup  = xlow + gPad->GetAbsWNDC();
         ylow = gPad->GetAbsYlowNDC();
         yup  = ylow + gPad->GetAbsHNDC();
      }
      WriteInteger(CMtoPS(fXsize*xlow));
      WriteInteger(CMtoPS(fYsize*ylow));
      WriteInteger(CMtoPS(fXsize*xup));
      WriteInteger(CMtoPS(fYsize*yup));
      PrintStr("@");
      Initialize();
      fBoundingBox  = kTRUE;
      fPrinted      = psave;
   }
   if (fPrinted) {
      if (fSave) SaveRestore(-1);
      fClear    = kTRUE;
      fPrinted  = kFALSE;
   }
   Zone();
}


//______________________________________________________________________________
void TPostScript::Range(Float_t xsize, Float_t ysize)
{
   // Set the range for the paper in centimeters

   Float_t xps=0, yps=0, xncm=0, yncm=0, dxwn=0, dywn=0, xwkwn=0, ywkwn=0, xymax=0;

   fXsize = xsize;
   fYsize = ysize;
   if( fType != 113) { xps = fXsize;  yps = fYsize; }
   else {              xps = xsize;   yps = ysize; }

   if( xsize <= xps && ysize < yps) {
      if ( xps > yps ) xymax = xps;
      else             xymax = yps;
      xncm  = xsize/xymax;
      yncm  = ysize/xymax;
      dxwn  = ((xps/xymax)-xncm)/2;
      dywn  = ((yps/xymax)-yncm)/2;
   } else {
      if (xps/yps < 1) xwkwn = xps/yps;
      else             xwkwn = 1;
      if (yps/xps < 1) ywkwn = yps/xps;
      else             ywkwn = 1;

      if (xsize < ysize)  {
         xncm = ywkwn*xsize/ysize;
         yncm = ywkwn;
         dxwn = (xwkwn-xncm)/2;
         dywn = 0;
         if( dxwn < 0) {
            xncm = xwkwn;
            dxwn = 0;
            yncm = xwkwn*ysize/xsize;
            dywn = (ywkwn-yncm)/2;
         }
      } else {
         xncm = xwkwn;
         yncm = xwkwn*ysize/xsize;
         dxwn = 0;
         dywn = (ywkwn-yncm)/2;
         if( dywn < 0) {
            yncm = ywkwn;
            dywn = 0;
            xncm = ywkwn*xsize/ysize;
            dxwn = (xwkwn-xncm)/2;
         }
      }
   }
   fXVP1  = dxwn;
   fXVP2  = xncm+dxwn;
   fYVP1  = dywn;
   fYVP2  = yncm+dywn;
   fRange = kTRUE;
}


//______________________________________________________________________________
void TPostScript::SaveRestore(Int_t flag)
{
   // Compute number of gsaves for restore
   // This allows to write the correct number of grestore at the
   // end of the PS file.

   if (flag == 1) { PrintFast(7," gsave ");  fSave++; }
   else           { PrintFast(4," gr ");     fSave--; }
}


//______________________________________________________________________________
void TPostScript::SetFillColor( Color_t cindex )
{
   // Set color index for fill areas

   fFillColor = cindex;
   if (gStyle->GetFillColor() <= 0) cindex = 0;
   SetColor(Int_t(cindex));
}


//______________________________________________________________________________
void TPostScript::SetFillPatterns(Int_t ipat, Int_t color)
{
   // Patterns definition
   //
   // Define the pattern ipat in the current PS file. ipat can vary from
   // 1 to 25. Together with the pattern, the color (color) in which the
   // pattern has to be drawn is also required. A pattern is defined in the
   // current PS file only the first time it is used. Some level 2
   // Postscript functions are used, so on level 1 printers, patterns will
   // not work. This is not a big problem because patterns are
   // defined only if they are used, so if they are not used a PS level 1
   // file will not be polluted by level 2 features, and in any case the old
   // patterns used a lot of memory which made them almost unusable on old
   // level 1 printers. Finally we should say that level 1 devices are
   // becoming very rare. The official PostScript is now level 3 !

   char cdef[28];
   char cpat[5];
   snprintf(cpat,5," P%2.2d", ipat);

   // fPatterns is used as an array of chars. If fPatterns[ipat] != 0 the
   // pattern number ipat as already be defined is this file and it
   // is not necessary to redefine it. fPatterns is set to zero in Initialize.
   // The pattern number 26 allows to know if the macro "cs" has already
   // been defined in the current file (see label 200).
   if (fPatterns[ipat] == 0) {

   // Define the Patterns. Line width must be 1
   // Setting fLineWidth to -1 will force the line width definition next time
   // TPostScript::SetLineWidth will be called.
      fLineWidth = -1;
      PrintFast(5," 1 lw");
      PrintStr(" << /PatternType 1 /PaintType 2 /TilingType 1");
      switch (ipat) {
         case 1 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 98 /YStep 4");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" [1] 0 sd 2 4 m 99 4 l s 1 3 m 98 3 l s");
            PrintStr(" 2 2 m 99 2 l s 1 1 m 98 1 l s");
            PrintStr(" gr end } >> [ 4.0 0 0 4.0 0 0 ]");
            break;
         case 2 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 96 /YStep 4");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" [1 3] 0 sd 2 4 m 98 4 l s 0 3 m 96 3 l s");
            PrintStr(" 2 2 m 98 2 l s 0 1 m 96 1 l s");
            PrintStr(" gr end } >> [ 3.0 0 0 3.0 0 0 ]");
            break;
         case 3 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 96 /YStep 16");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" [1 3] 0 sd 2 13 m 98 13 l s 0 9 m 96 9 l s");
            PrintStr(" 2 5 m 98 5 l s 0 1 m 96 1 l s");
            PrintStr(" gr end } >> [ 2.0 0 0 2.0 0 0 ]");
            break;
         case 4 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 0 m 100 100 l s");
            PrintStr(" gr end } >> [ 0.24 0 0 0.24 0 0 ]");
            break;
         case 5 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 100 m 100 0 l s");
            PrintStr(" gr end } >> [ 0.24 0 0 0.24 0 0 ]");
            break;
         case 6 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 50 0 m 50 100 l s");
            PrintStr(" gr end } >> [ 0.12 0 0 0.12 0 0 ]");
            break;
         case 7 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 50 m 100 50 l s");
            PrintStr(" gr end } >> [ 0.12 0 0 0.12 0 0 ]");
            break;
         case 8 :
            PrintStr(" /BBox [ 0 0 101 101 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 0 m 0 30 l 30 0 l f 0 70 m 0 100 l 30 100 l f");
            PrintStr(" 70 100 m 100 100 l 100 70 l f 70 0 m 100 0 l");
            PrintStr(" 100 30 l f 50 20 m 20 50 l 50 80 l 80 50 l f");
            PrintStr(" 50 80 m 30 100 l s 20 50 m 0 30 l s 50 20 m");
            PrintStr(" 70 0 l s 80 50 m 100 70 l s");
            PrintStr(" gr end } >> [ 0.24 0 0 0.24 0 0 ]");
            break;
         case 9 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 50 m 50 50 50 180 360 arc");
            PrintStr(" 0 50 m 0 100 50 270 360 arc");
            PrintStr(" 50 100 m 100 100 50 180 270 arc s");
            PrintStr(" gr end } >> [ 0.24 0 0 0.24 0 0 ]");
            break;
         case 10 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 50 m 100 50 l 1 1 m 100 1 l");
            PrintStr(" 0 0 m 0 50 l 100 0 m 100 50 l");
            PrintStr(" 50 50 m 50 100 l s");
            PrintStr(" gr end } >> [ 0.24 0 0 0.24 0 0 ]");
            break;
         case 11 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 0 m 0 20 l 50 0 m 50 20 l");
            PrintStr(" 100 0 m 100 20 l 0 80 m 0 100 l");
            PrintStr(" 50 80 m 50 100 l 100 80 m 100 100 l");
            PrintStr(" 25 30 m 25 70 l 75 30 m 75 70 l");
            PrintStr(" 0 100 m 20 85 l 50 100 m 30 85 l");
            PrintStr(" 50 100 m 70 85 l 100 100 m 80 85 l");
            PrintStr(" 0 0 m 20 15 l 50 0 m 30 15 l");
            PrintStr(" 50 0 m 70 15 l 100 0 m 80 15 l");
            PrintStr(" 5 35 m 45 65 l 5 65 m 45 35 l");
            PrintStr(" 55 35 m 95 65 l 55 65 m 95 35 l s");
            PrintStr(" gr end } >> [ 0.5 0 0 0.5 0 0 ]");
            break;
         case 12 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 80 m 0 100 20 270 360 arc");
            PrintStr(" 30 100 m 50 100 20 180 360 arc");
            PrintStr(" 80 100 m 100 100 20 180 270 arc");
            PrintStr(" 20 0 m 0 0 20 0 90 arc");
            PrintStr(" 70 0 m 50 0 20 0 180 arc");
            PrintStr(" 100 20 m 100 0 20 90 180 arc");
            PrintStr(" 45 50 m 25 50 20 0 360 arc");
            PrintStr(" 95 50 m 75 50 20 0 360 arc s");
            PrintStr(" gr end } >> [ 0.5 0 0 0.5 0 0 ]");
            break;
         case 13 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 0 m 100 100 l 0 100 m 100 0 l s");
            PrintStr(" gr end } >> [ 0.24 0 0 0.24 0 0 ]");
            break;
         case 14 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 80 /YStep 80");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 20 m 100 20 l 20 0 m 20 100 l");
            PrintStr(" 0 80 m 100 80 l 80 0 m 80 100 l");
            PrintStr(" 20 40 m 60 40 l 60 20 m 60 60 l");
            PrintStr(" 40 40 m 40 80 l 40 60 m 80 60 l s");
            PrintStr(" gr end } >> [ 0.60 0 0 0.60 0 0 ]");
            break;
         case 15 :
            PrintStr(" /BBox [ 0 0 60 60 ]");
            PrintStr(" /XStep 60 /YStep 60");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 55 m 0 60 5 270 360 arc");
            PrintStr(" 25 60 m 30 60 5 180 360 arc");
            PrintStr(" 55 60 m 60 60 5 180 270 arc");
            PrintStr(" 20 30 m 15 30 5 0 360 arc");
            PrintStr(" 50 30 m 45 30 5 0 360");
            PrintStr(" arc 5 0 m 0 0 5 0 90 arc");
            PrintStr(" 35 0 m 30 0 5 0 180 arc");
            PrintStr(" 60 5 m 60 0 5 90 180 arc s");
            PrintStr(" gr end } >> [ 0.41 0 0 0.41 0 0 ]");
            break;
         case 16 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 50 50 m 25 50 25 0 180 arc s");
            PrintStr(" 50 50 m 75 50 25 180 360 arc s");
            PrintStr(" gr end } >> [ 0.4 0 0 0.2 0 0 ]");
            break;
         case 17 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" [24] 0 setdash 0 0 m 100 100 l s");
            PrintStr(" gr end } >> [ 0.24 0 0 0.24 0 0 ]");
            break;
         case 18 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" [24] 0 setdash 0 100 m 100 0 l s");
            PrintStr(" gr end } >> [ 0.24 0 0 0.24 0 0 ]");
            break;
         case 19 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 90 50 m 50 50 40 0 360 arc");
            PrintStr(" 0 50 m 0 100 50 270 360 arc");
            PrintStr(" 50 0 m 0 0 50 0 90 arc");
            PrintStr(" 100 50 m 100 0 50 90 180 arc");
            PrintStr(" 50 100 m 100 100 50 180 270 arc s");
            PrintStr(" gr end } >> [ 0.47 0 0 0.47 0 0 ]");
            break;
         case 20 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 50 50 m 50 75 25 270 450 arc s");
            PrintStr(" 50 50 m 50 25 25 90  270 arc s");
            PrintStr(" gr end } >> [ 0.2 0 0 0.4 0 0 ]");
            break;
         case 21 :
            PrintStr(" /BBox [ 0 0 101 101 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 1 1 m 25 1 l 25 25 l 50 25 l 50 50 l");
            PrintStr(" 75 50 l 75 75 l 100 75 l 100 100 l");
            PrintStr(" 50 1 m 75 1 l 75 25 l 100 25 l 100 50 l");
            PrintStr(" 0 50 m 25 50 l 25 75 l 50 75 l 50 100 l s");
            PrintStr(" gr end } >> [ 0.5 0 0 0.5 0 0 ]");
            break;
         case 22 :
            PrintStr(" /BBox [ 0 0 101 101 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 1 100 m 25 100 l 25 75 l 50 75 l 50 50 l");
            PrintStr(" 75 50 l 75 25 l 100 25 l 100 1 l");
            PrintStr(" 50 100 m 75 100 l 75 75 l 100 75 l 100 50 l");
            PrintStr(" 0 50 m 25 50 l 25 25 l 50 25 l 50 1 l s");
            PrintStr(" gr end } >> [ 0.5 0 0 0.5 0 0 ]");
            break;
         case 23 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" [1 7] 0 sd 0 8 50 { dup dup m 2 mul 0 l s } for");
            PrintStr(" 0 8 50 { dup dup 2 mul 100 m 50 add exch 50");
            PrintStr(" add l s } for 100 0 m 100 100 l 50 50 l f");
            PrintStr(" gr end } >> [ 0.24 0 0 0.24 0 0 ]");
            break;
         case 24 :
            PrintStr(" /BBox [ 0 0 100 100 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 100 100 m 100 36 l 88 36 l 88 88 l f");
            PrintStr(" 100 0 m 100 12 l 56 12 l 50 0 l f");
            PrintStr(" 0 0 m 48 0 l 48 48 l 50 48 l 56 60 l");
            PrintStr(" 36 60 l 36 12 l 0 12 l f [1 7] 0 sd");
            PrintStr(" 61 8 87 { dup dup dup 12 exch m 88 exch l s");
            PrintStr(" 16 exch 4 sub m 88 exch 4 sub l s } for");
            PrintStr(" 13 8 35 { dup dup dup 0 exch m 36 exch l s");
            PrintStr(" 4 exch 4 sub m 36 exch 4 sub l s } for");
            PrintStr(" 37 8 59 { dup dup dup 12 exch m 36 exch l s");
            PrintStr(" 16 exch 4 sub m 36 exch 4 sub l s } for");
            PrintStr(" 13 8 60 { dup dup dup 56 exch m 100 exch l s");
            PrintStr(" 60 exch 4 sub m 100 exch 4 sub l s } for");
            PrintStr(" gr end } >> [ 0.5 0 0 0.5 0 0 ]");
            break;
         case 25 :
            PrintStr(" /BBox [ 0 0 101 101 ]");
            PrintStr(" /XStep 100 /YStep 100");
            PrintStr(" /PaintProc { begin gsave");
            PrintStr(" 0 0 m 30 30 l 70 30 l 70 70 l 100 100 l 100 0 l");
            PrintStr(" f 30 30 m 30 70 l 70 70 l f");
            PrintStr(" gr end } >> [ 0.5 0 0 0.5 0 0 ]");
      };
      snprintf(cdef,28," makepattern /%s exch def",&cpat[1]);
      PrintStr(cdef);
      fPatterns[ipat] = 1;
   }

   // Define the macro cs and FA if they are not yet defined.
   if (fPatterns[26] == 0) {
      if (gStyle->GetColorModelPS()) {
         PrintStr(" /cs {[/Pattern /DeviceCMYK] setcolorspace} def");
         PrintStr(" /FA {f [/DeviceCMYK] setcolorspace} def");
      } else {
         PrintStr(" /cs {[/Pattern /DeviceRGB] setcolorspace} def");
         PrintStr(" /FA {f [/DeviceRGB] setcolorspace} def");
      }
      fPatterns[26] = 1;
   }

   // Activate the pattern.
   PrintFast(3," cs");
   TColor *col = gROOT->GetColor(color);
   if (col) {
      Double_t colRed   = col->GetRed();
      Double_t colGreen = col->GetGreen();
      Double_t colBlue  = col->GetBlue();
      if (gStyle->GetColorModelPS()) {
         Double_t colBlack = TMath::Min(TMath::Min(1-colRed,1-colGreen),1-colBlue);
         if (colBlack==1) {
            WriteReal(0);
            WriteReal(0);
            WriteReal(0);
            WriteReal(colBlack);
         } else {
            Double_t colCyan    = (1-colRed-colBlack)/(1-colBlack);
            Double_t colMagenta = (1-colGreen-colBlack)/(1-colBlack);
            Double_t colYellow  = (1-colBlue-colBlack)/(1-colBlack);
            WriteReal(colCyan);
            WriteReal(colMagenta);
            WriteReal(colYellow);
            WriteReal(colBlack);
         }
      } else {
         WriteReal(colRed);
         WriteReal(colGreen);
         WriteReal(colBlue);
      }
   }
   PrintFast(4,cpat);
   PrintFast(9," setcolor");
}


//______________________________________________________________________________
void TPostScript::SetLineColor( Color_t cindex )
{
   // Set color index for lines

   fLineColor = cindex;
   SetColor(Int_t(cindex));
}


//______________________________________________________________________________
void TPostScript::SetLineJoin( Int_t linejoin )
{
   // Set the value of the global parameter TPostScript::fgLineJoin.
   // This parameter determines the appearance of joining lines in a PostScript
   // output.
   // It takes one argument which may be:
   //   - 0 (miter join)
   //   - 1 (round join)
   //   - 2 (bevel join)
   // The default value is 0 (miter join).
   //
   //Begin_Html
   /*
   <img src="gif/linejoin.gif">
   */
   //End_Html
   //
   // To change the line join behaviour just do:
   // TPostScript::SetLineJoin(2); // Set the PS line join to bevel.

   fgLineJoin = linejoin;
}


//______________________________________________________________________________
void TPostScript::SetLineStyle(Style_t linestyle)
{
   // Change the line style
   //
   //   linestyle = 2 dashed
   //             = 3  dotted
   //             = 4  dash-dotted
   //              else = solid
   //
   // See TStyle::SetLineStyleString for style definition

   if ( linestyle == fLineStyle) return;
   fLineStyle = linestyle;
   const char *st = gStyle->GetLineStyleString(linestyle);
   PrintFast(1,"[");
   Int_t nch = strlen(st);
   PrintFast(nch,st);
   PrintFast(6,"] 0 sd");
}


//______________________________________________________________________________
void TPostScript::SetLineWidth(Width_t linewidth)
{
   // Change the line width

   if ( linewidth == fLineWidth) return;
   fLineWidth = linewidth;
   WriteInteger(Int_t(fLineScale*fLineWidth));
   PrintFast(3," lw");
}


//______________________________________________________________________________
void TPostScript::SetMarkerColor( Color_t cindex )
{
   // Set color index for markers

   fMarkerColor = cindex;
   SetColor(Int_t(cindex));
}


//______________________________________________________________________________
void TPostScript::SetColor(Int_t color)
{
   // Set the current color.

   if (color < 0) color = 0;
   fCurrentColor = color;
   TColor *col = gROOT->GetColor(color);
   if (col)
      SetColor(col->GetRed(), col->GetGreen(), col->GetBlue());
   else
      SetColor(1., 1., 1.);
}


//______________________________________________________________________________
void TPostScript::SetColor(Float_t r, Float_t g, Float_t b)
{
   // Set directly current color (don't go via TColor).

   if (r == fRed && g == fGreen && b == fBlue) return;

   fRed   = r;
   fGreen = g;
   fBlue  = b;

   if (fRed <= 0 && fGreen <= 0 && fBlue <= 0 ) {
      PrintFast(6," black");
   } else {
      if (gStyle->GetColorModelPS()) {
         Double_t colBlack   = TMath::Min(TMath::Min(1-fRed,1-fGreen),1-fBlue);
         Double_t colCyan    = (1-fRed-colBlack)/(1-colBlack);
         Double_t colMagenta = (1-fGreen-colBlack)/(1-colBlack);
         Double_t colYellow  = (1-fBlue-colBlack)/(1-colBlack);
         WriteReal(colCyan);
         WriteReal(colMagenta);
         WriteReal(colYellow);
         WriteReal(colBlack);
      } else {
         WriteReal(fRed);
         WriteReal(fGreen);
         WriteReal(fBlue);
      }
      PrintFast(2," c");
   }
}


//______________________________________________________________________________
void TPostScript::SetTextColor( Color_t cindex )
{
   // Set color index for text

   fTextColor = cindex;

   SetColor( Int_t(cindex) );
}


//______________________________________________________________________________
void TPostScript::Text(Double_t xx, Double_t yy, const char *chars)
{
   // Write a string of characters
   //
   // This routine writes the string chars into a PostScript file
   // at position xx,yy in world coordinates.

   static const char *psfont[] = {
    "/Times-Italic"         , "/Times-Bold"         , "/Times-BoldItalic",
    "/Helvetica"            , "/Helvetica-Oblique"  , "/Helvetica-Bold"  ,
    "/Helvetica-BoldOblique", "/Courier"            , "/Courier-Oblique" ,
    "/Courier-Bold"         , "/Courier-BoldOblique", "/Symbol"          ,
    "/Times-Roman"          , "/ZapfDingbats"       , "/Symbol"};

   const Double_t kDEGRAD = TMath::Pi()/180.;
   Double_t x = xx;
   Double_t y = yy;
   if (!gPad) return;

   // Compute the font size. Exit if it is 0
   // The font size is computed from the TTF size to get exactly the same
   // size on the screen and in the PostScript file.
   Double_t wh = (Double_t)gPad->XtoPixel(gPad->GetX2());
   Double_t hh = (Double_t)gPad->YtoPixel(gPad->GetY1());
   Float_t tsize, ftsize;

   if (wh < hh) {
      tsize         = fTextSize*wh;
      Int_t sizeTTF = (Int_t)(tsize*kScale+0.5); // TTF size
      ftsize        = (sizeTTF*fXsize*gPad->GetAbsWNDC())/wh;
   } else {
      tsize         = fTextSize*hh;
      Int_t sizeTTF = (Int_t)(tsize*kScale+0.5); // TTF size
      ftsize        = (sizeTTF*fYsize*gPad->GetAbsHNDC())/hh;
   }
   Double_t fontsize = 4*(72*(ftsize)/2.54);
   if( fontsize <= 0) return;

   Float_t tsizex = gPad->AbsPixeltoX(Int_t(tsize))-gPad->AbsPixeltoX(0);
   Float_t tsizey = gPad->AbsPixeltoY(0)-gPad->AbsPixeltoY(Int_t(tsize));

   Int_t font = abs(fTextFont)/10;
   if( font > 15 || font < 1) font = 1;

   // Text color.
   SetColor(Int_t(fTextColor));

   // Text alignment.
   Int_t txalh = fTextAlign/10;
   if (txalh <1) txalh = 1; if (txalh > 3) txalh = 3;
   Int_t txalv = fTextAlign%10;
   if (txalv <1) txalv = 1; if (txalv > 3) txalv = 3;
   if (txalv == 3) {
      y -= 0.8*tsizey*TMath::Cos(kDEGRAD*fTextAngle);
      x += 0.8*tsizex*TMath::Sin(kDEGRAD*fTextAngle);
   } else if (txalv == 2) {
      y -= 0.4*tsizey*TMath::Cos(kDEGRAD*fTextAngle);
      x += 0.4*tsizex*TMath::Sin(kDEGRAD*fTextAngle);
   }

   UInt_t w = 0, w0 = 0;
   Bool_t kerning;
   // In order to measure the precise character positions we need to trick
   // FreeType into rendering high-resolution characters otherwise it will
   // stick to the screen pixel grid, which is far worse than we can achieve
   // on print.
   const Float_t scale = 16.0;
   // Save current text attributes.
   TText saveAttText;
   saveAttText.TAttText::operator=(*this);
   const Int_t len=strlen(chars);
   Int_t *charWidthsCumul = 0;
   TText t;
   t.SetTextSize(fTextSize * scale);
   t.SetTextFont(fTextFont);
   t.GetTextAdvance(w, chars);
   t.GetTextAdvance(w0, chars, kFALSE);
   t.TAttText::Modify();
   if (w0-w != 0) kerning = kTRUE;
   else        kerning = kFALSE;
   if (kerning) {
      // Calculate the individual character placements.
      charWidthsCumul = new Int_t[len];
      for (Int_t i = len - 1;i >= 0;i--) {
         UInt_t ww = 0;
         t.GetTextAdvance(ww, chars + i);
         Double_t wwl = (gPad->AbsPixeltoX(ww)-gPad->AbsPixeltoX(0));
         charWidthsCumul[i] = (Int_t)((XtoPS(wwl) - XtoPS(0)) / scale);
      }
   }
   // Restore text attributes.
   saveAttText.TAttText::Modify();

   Double_t charsLength = gPad->AbsPixeltoX(w)-gPad->AbsPixeltoX(0);
   Int_t psCharsLength = (Int_t)((XtoPS(charsLength)-XtoPS(0)) / scale);

   // Text angle.
   Int_t psangle = Int_t(0.5 + fTextAngle);

   // Save context.
   PrintStr("@");
   SaveRestore(1);

   // Clipping
   Int_t xc1 = XtoPS(gPad->GetX1());
   Int_t xc2 = XtoPS(gPad->GetX2());
   Int_t yc1 = YtoPS(gPad->GetY1());
   Int_t yc2 = YtoPS(gPad->GetY2());
   WriteInteger(xc2 - xc1);
   WriteInteger(yc2 - yc1);
   WriteInteger(xc1);
   WriteInteger(yc1);
   PrintStr(" C");

   // Output text position and angle. The text position is computed
   // using Double_t to avoid precision problems.
   Double_t vx  = (x - gPad->GetX1())/(gPad->GetX2()-gPad->GetX1());
   Double_t cmx = fXsize*(gPad->GetAbsXlowNDC()+vx*gPad->GetAbsWNDC());
   WriteReal((288.*cmx)/2.54);
   Double_t vy  = (y - gPad->GetY1())/(gPad->GetY2()-gPad->GetY1());
   Double_t cmy = fYsize*(gPad->GetAbsYlowNDC()+vy*gPad->GetAbsHNDC());
   WriteReal((288.*cmy)/2.54);
   PrintStr(Form(" t %d r ", psangle));
   if(txalh == 2) PrintStr(Form(" %d 0 t ", -psCharsLength/2));
   if(txalh == 3) PrintStr(Form(" %d 0 t ", -psCharsLength));
   PrintStr(psfont[font-1]);
   if (font != 15) {
     PrintStr(Form(" findfont %g sf 0 0 m ",fontsize));
   } else {
     PrintStr(Form(" findfont %g sf 0 0 m ita ",fontsize));
   }

   if (kerning) {
      PrintStr("@");
      // Output individual character placements
      for (Int_t i = len-1; i >= 1; i--) {
         WriteInteger(charWidthsCumul[0] - charWidthsCumul[i]);
      }
      delete [] charWidthsCumul;
      PrintStr("@");
   }

   // Output text.
   PrintStr("(");

   // Inside a PostScript string, the new line (if needed to break up long lines) must be escaped by a backslash.
   const char *crsave = fImplicitCREsc;
   fImplicitCREsc = "\\";

   char str[8];
   for (Int_t i=0; i<len;i++) {
      if (chars[i]!='\n') {
         if (chars[i]=='(' || chars[i]==')' || chars[i]=='\\') {
            snprintf(str,8,"\\%c",chars[i]);
            PrintStr(str);
         } else if ((chars[i]=='-') && (font != 12)) {
            PrintStr("\\255");
         } else {
            snprintf(str,8,"%c",chars[i]);
            PrintFast(1,str);
         }
      }
   }
   PrintStr(")");
   fImplicitCREsc = crsave;

   if (kerning) {
      if (font != 15) PrintStr(" K NC");
      else            PrintStr(" K gr NC");
   } else {
      if (font != 15) PrintStr(" show NC");
      else            PrintStr(" show gr NC");
   }

   SaveRestore(-1);
}


//______________________________________________________________________________
void TPostScript::TextNDC(Double_t u, Double_t v, const char *chars)
{
   // Write a string of characters in NDC

   Double_t x = gPad->GetX1() + u*(gPad->GetX2() - gPad->GetX1());
   Double_t y = gPad->GetY1() + v*(gPad->GetY2() - gPad->GetY1());
   Text(x, y, chars);
}


//______________________________________________________________________________
Int_t TPostScript::UtoPS(Double_t u)
{
   // Convert U from NDC coordinate to PostScript

   Double_t cm = fXsize*(gPad->GetAbsXlowNDC() + u*gPad->GetAbsWNDC());
   return Int_t(0.5 + 288*cm/2.54);
}


//______________________________________________________________________________
Int_t TPostScript::VtoPS(Double_t v)
{
   // Convert V from NDC coordinate to PostScript

   Double_t cm = fYsize*(gPad->GetAbsYlowNDC() + v*gPad->GetAbsHNDC());
   return Int_t(0.5 + 288*cm/2.54);
}


//______________________________________________________________________________
Int_t TPostScript::XtoPS(Double_t x)
{
   // Convert X from world coordinate to PostScript

   Double_t u = (x - gPad->GetX1())/(gPad->GetX2() - gPad->GetX1());
   return  UtoPS(u);
}


//______________________________________________________________________________
Int_t TPostScript::YtoPS(Double_t y)
{
   // Convert Y from world coordinate to PostScript

   Double_t v = (y - gPad->GetY1())/(gPad->GetY2() - gPad->GetY1());
   return  VtoPS(v);
}


//______________________________________________________________________________
void TPostScript::Zone()
{
   // Initialize the PostScript page in zones

   if( !fClear )return;
   fClear = kFALSE;

   // When Zone has been called, fZone is TRUE
   fZone = kTRUE;

   if( fIYzone > fNYzone) {
      fIYzone=1;
      if( fMode != 3) {
         PrintStr("@showpage");
         SaveRestore(-1);
         fNpages++;
         PrintStr("@%%Page:");
         WriteInteger(fNpages);
         WriteInteger(fNpages);
         PrintStr("@");
      } else {
         PrintFast(9," showpage");
         SaveRestore(-1);
      }
   }

   // No grestore the first time
   if( fMode != 3) {
      if( fIXzone != 1 || fIYzone != 1) SaveRestore(-1);
      SaveRestore(1);
      PrintStr("@");
      WriteInteger(fIXzone);
      WriteInteger(fIYzone);
      PrintFast(5," Zone");
      PrintStr("@");
      fIXzone++;
      if( fIXzone > fNXzone) { fIXzone=1; fIYzone++; }
   }

   // Picture Initialisation
   SaveRestore(1);
   if (fgLineJoin) {
      WriteInteger(fgLineJoin);
      PrintFast(12," setlinejoin");
   }
   PrintFast(6," 0 0 t");
   fRed     = -1;
   fGreen   = -1;
   fBlue    = -1;
   fPrinted = kFALSE;
   fLineColor  = -1;
   fLineStyle  = -1;
   fLineWidth  = -1;
   fFillColor  = -1;
   fFillStyle  = -1;
   fMarkerSizeCur = -1;
}
