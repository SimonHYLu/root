#include "BaBar/BaBar.hh"
/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooCurve.cc,v 1.38 2004/08/09 00:00:53 bartoldu Exp $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2004, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

// -- CLASS DESCRIPTION [PLOT] --
// A RooCurve is a one-dimensional graphical representation of a real-valued function.
// A curve is approximated by straight line segments with endpoints chosen to give
// a "good" approximation to the true curve. The goodness of the approximation is
// controlled by a precision and a resolution parameter. To view the points where
// a function y(x) is actually evaluated to approximate a smooth curve, use:
//
//  RooPlot *p= y.plotOn(x.frame());
//  p->getAttMarker("curve_y")->SetMarkerStyle(20);
//  p->setDrawOptions("curve_y","PL");
//  p->Draw();

// #include "BaBar/BaBar.hh"

#include "RooFitCore/RooCurve.hh"
#include "RooFitCore/RooHist.hh"
#include "RooFitCore/RooAbsReal.hh"
#include "RooFitCore/RooArgSet.hh"
#include "RooFitCore/RooRealVar.hh"
#include "RooFitCore/RooRealIntegral.hh"
#include "RooFitCore/RooRealBinding.hh"
#include "RooFitCore/RooScaledFunc.hh"

#include <iostream>
#include <iomanip>
#include <math.h>
#include <assert.h>
using std::cout;
using std::endl;
using std::ostream;
using std::setw;

ClassImp(RooCurve)

RooCurve::RooCurve() {
  initialize();
}

RooCurve::RooCurve(const RooAbsReal &f, RooAbsRealLValue &x, Double_t xlo, Double_t xhi, Int_t xbins,
		   Double_t scaleFactor, const RooArgSet *normVars, Double_t prec, Double_t resolution,
		   Bool_t shiftToZero, WingMode wmode) {
  // Create a 1-dim curve of the value of the specified real-valued expression
  // as a function of x. Use the optional precision parameter to control
  // how precisely the smooth curve is rasterized. Use the optional argument set
  // to specify how the expression should be normalized. Use the optional scale
  // factor to rescale the expression after normalization.
  // If shiftToZero is set, the entire curve is shift down to make the lowest
  // point in of the curve go through zero.

  // grab the function's name and title
  TString name("curve_");
  name.Append(f.GetName());
  SetName(name.Data());
  TString title(f.GetTitle());
  SetTitle(title.Data());
  // append " ( [<funit> ][/ <xunit> ])" to our y-axis label if necessary
  if(0 != strlen(f.getUnit()) || 0 != strlen(x.getUnit())) {
    title.Append(" ( ");
    if(0 != strlen(f.getUnit())) {
      title.Append(f.getUnit());
      title.Append(" ");
    }
    if(0 != strlen(x.getUnit())) {
      title.Append("/ ");
      title.Append(x.getUnit());
      title.Append(" ");
    }
    title.Append(")");
  }
  setYAxisLabel(title.Data());

  RooAbsFunc *funcPtr = 0;
  RooAbsFunc *rawPtr  = 0;
  funcPtr= f.bindVars(x,normVars,kTRUE);

  // apply a scale factor if necessary
  if(scaleFactor != 1) {
    rawPtr= funcPtr;
    funcPtr= new RooScaledFunc(*rawPtr,scaleFactor);
  }
  assert(0 != funcPtr);

  // calculate the points to add to our curve
  Double_t prevYMax = getYAxisMax() ;
  addPoints(*funcPtr,xlo,xhi,xbins+1,prec,resolution,wmode);
  initialize();

  // cleanup
  delete funcPtr;
  if(rawPtr) delete rawPtr;
  if (shiftToZero) shiftCurveToZero(prevYMax) ;

  // Adjust limits
  Int_t i ;
  for (i=0 ; i<GetN() ; i++) {    
    Double_t x,y ;
    GetPoint(i,x,y) ;
    updateYAxisLimits(y);
  }
}



RooCurve::RooCurve(const char *name, const char *title, const RooAbsFunc &func,
		   Double_t xlo, Double_t xhi, UInt_t minPoints, Double_t prec, Double_t resolution,
		   Bool_t shiftToZero, WingMode wmode) {
  SetName(name);
  SetTitle(title);
  Double_t prevYMax = getYAxisMax() ;
  addPoints(func,xlo,xhi,minPoints+1,prec,resolution,wmode);  
  initialize();
  if (shiftToZero) shiftCurveToZero(prevYMax) ;

  // Adjust limits
  Int_t i ;
  for (i=0 ; i<GetN() ; i++) {    
    Double_t x,y ;
    GetPoint(i,x,y) ;
    updateYAxisLimits(y);
  }
}



RooCurve::~RooCurve() 
{
}



void RooCurve::initialize() 
{
  // Perform initialization that is common to all constructors.

  // set default line width in pixels
  SetLineWidth(3);
  // set default line color
  SetLineColor(kBlue);
}


void RooCurve::shiftCurveToZero(Double_t prevYMax) 
  // Find lowest point in curve and move all points in curve so that
  // lowest point will go exactly through zero
{
  Int_t i ;
  Double_t minVal(1e30) ;
  Double_t maxVal(-1e30) ;

  // First iteration, find current lowest point
  for (i=1 ; i<GetN()-1 ; i++) {
    Double_t x,y ;
    GetPoint(i,x,y) ;
    if (y<minVal) minVal=y ;
    if (y>maxVal) maxVal=y ;
  }

  // Second iteration, lower all points by minVal
  for (i=1 ; i<GetN()-1 ; i++) {
    Double_t x,y ;
    GetPoint(i,x,y) ;
    SetPoint(i,x,y-minVal) ;
  }

  // Check if y-axis range needs readjustment
  if (getYAxisMax()>prevYMax) {
    Double_t newMax = maxVal - minVal ;
    setYAxisLimits(getYAxisMin(), newMax<prevYMax ? prevYMax : newMax) ;
  }
}



void RooCurve::addPoints(const RooAbsFunc &func, Double_t xlo, Double_t xhi,
			 Int_t minPoints, Double_t prec, Double_t resolution, WingMode wmode) {
  // Add points calculated with the specified function, over the range (xlo,xhi).
  // Add at least minPoints equally spaced points, and add sufficient points so that
  // the maximum deviation from the final straight-line segements is prec*(ymax-ymin),
  // down to a minimum horizontal spacing of resolution*(xhi-xlo).

  // check the inputs
  if(!func.isValid()) {
    cout << fName << "::addPoints: input function is not valid" << endl;
    return;
  }
  if(minPoints <= 0 || xhi <= xlo) {
    cout << fName << "::addPoints: bad input (nothing added)" << endl;
    return;
  }

  // Perform a coarse scan of the function to estimate its y range.
  // Save the results so we do not have to re-evaluate at the scan points.
  Double_t *yval= new Double_t[minPoints];
  assert(0 != yval);
  Double_t x,dx= (xhi-xlo)/(minPoints-1.);

  Int_t step;
  Double_t ymax(-1e30), ymin(1e30) ;
  for(step= 0; step < minPoints; step++) {
    x= xlo + step*dx;
    if (step==minPoints-1) x-=1e-15 ;
    yval[step]= func(&x);
    if (yval[step]>ymax) ymax=yval[step] ;
    if (yval[step]<ymin) ymin=yval[step] ;
  }
  Double_t yrangeEst=(ymax-ymin) ;

  // store points of the coarse scan and calculate any refinements necessary
  Double_t minDx= resolution*(xhi-xlo);
  Double_t x1,x2= xlo;

  if (wmode==Extended) {
    addPoint(xlo-dx,0) ;
    addPoint(xlo-dx,yval[0]) ;
  } else if (wmode==Straight) {
    addPoint(xlo,0) ;
  }

  addPoint(xlo,yval[0]);
  for(step= 1; step < minPoints; step++) {
    x1= x2;
    x2= xlo + step*dx;
    addRange(func,x1,x2,yval[step-1],yval[step],prec*yrangeEst,minDx);
  }
  addPoint(xhi,yval[minPoints-1]) ;

  if (wmode==Extended) {
    addPoint(xhi+dx,yval[minPoints-1]) ;
    addPoint(xhi+dx,0) ;
  } else if (wmode==Straight) {
    addPoint(xhi,0) ;
  }

  // cleanup
  delete [] yval;
}

void RooCurve::addRange(const RooAbsFunc& func, Double_t x1, Double_t x2,
			Double_t y1, Double_t y2, Double_t minDy, Double_t minDx) {
  // Fill the range (x1,x2) with points calculated using func(&x). No point will
  // be added at x1, and a point will always be added at x2. The density of points
  // will be calculated so that the maximum deviation from a straight line
  // approximation is prec*(ymax-ymin) down to the specified minimum horizontal spacing.

  // calculate our value at the midpoint of this range
  Double_t xmid= 0.5*(x1+x2);
  Double_t ymid= func(&xmid);
  // test if the midpoint is sufficiently close to a straight line across this interval
  Double_t dy= ymid - 0.5*(y1+y2);
  if((xmid - x1 >= minDx) && fabs(dy)>0 && fabs(dy) >= minDy) {
    // fill in each subrange
    addRange(func,x1,xmid,y1,ymid,minDy,minDx);
    addRange(func,xmid,x2,ymid,y2,minDy,minDx);
  }
  else {
    // add the endpoint
    addPoint(x2,y2);
  }
}

void RooCurve::addPoint(Double_t x, Double_t y) {
  // Add a point with the specified coordinates. Update our y-axis limits.

  // cout << "RooCurve("<< GetName() << ") adding point at (" << x << "," << y << ")" << endl ;
  Int_t next= GetN();
  SetPoint(next, x, y);
}

Double_t RooCurve::getFitRangeNEvt() const {
  return 1;
}

Double_t RooCurve::getFitRangeBinW() const {
  return 1;
}

void RooCurve::printToStream(ostream& os, PrintOption opt, TString indent) const {
  // Print info about this histogram to the specified output stream.
  //
  //   Standard: number of entries
  //    Verbose: print points on curve

  oneLinePrint(os,*this);
  RooPlotable::printToStream(os,opt,indent);
  if(opt >= Standard) {
    os << indent << "--- RooCurve ---" << endl;
    Int_t n= GetN();
    os << indent << "  Contains " << n << " points" << endl;
    if(opt >= Verbose) {
      os << indent << "  Graph points:" << endl;
      for(Int_t i= 0; i < n; i++) {
	os << indent << setw(3) << i << ") x = " << fX[i] << " , y = " << fY[i] << endl;
      }
    }
  }
}


Double_t RooCurve::chiSquare(const RooHist& hist) const 
{
  Int_t i,np = hist.GetN() ;
  Double_t x,y,eyl,eyh ;
  Double_t hbinw2 = hist.getNominalBinWidth()/2 ;

  Double_t chisq(0) ;
  for (i=0 ; i<np ; i++) {    
    // Retrieve histogram contents
    ((RooHist&)hist).GetPoint(i,x,y) ;
    eyl = hist.GetEYlow()[i] ;
    eyh = hist.GetEYhigh()[i] ;

    // Integrate function over this bin
    Double_t avg = average(x-hbinw2,x+hbinw2) ;

    // Add pull^2 to chisq
    if (y!=0) {      
      Double_t pull = (y>avg) ? ((y-avg)/eyl) : ((y-avg)/eyh) ;
      chisq += pull*pull ;
    }
  }

  // Return chisq/nDOF 
  return chisq / np ;
}



Double_t RooCurve::average(Double_t lo, Double_t hi) const
{
  // Find points corresponding to first and last point
  Double_t tolerance = fabs(hi-lo)*1e-3 ;
  Int_t ifirst = findPoint(lo,tolerance) ;
  Int_t ilast  = findPoint(hi,tolerance) ;

  if (ilast<=ifirst) {
    cout << "RooCurve::average(" << GetName() 
	 << ") invalid range (" << lo << "," << hi << ")" << endl ;
    return 0 ;
  }

  // Trapezoid integration
  Int_t i ;
  Double_t sum(0),x1,y1,x2,y2 ;
  for (i=ifirst ; i<ilast ; i++) {
    ((RooCurve&)*this).GetPoint(i,x1,y1) ;
    ((RooCurve&)*this).GetPoint(i+1,x2,y2) ;
    sum += (x2-x1)*(y1+y2)/2 ;
  }

  // Return trapezoid sum devided by integration range
  ((RooCurve&)*this).GetPoint(ifirst,x1,y1) ;
  ((RooCurve&)*this).GetPoint(ilast,x2,y2) ;
  return sum/(x2-x1) ;
}



Int_t RooCurve::findPoint(Double_t xvalue, Double_t tolerance) const
{
  Double_t delta(999.),x,y ;
  Int_t i,n = GetN() ;
  Int_t ibest(-1) ;
  for (i=0 ; i<n ; i++) {
    ((RooCurve&)*this).GetPoint(i,x,y) ;
    if (fabs(xvalue-x)<delta) {
      delta = fabs(xvalue-x) ;
      ibest = i ;
    }
  }

  return (delta<tolerance)?ibest:-1 ;
}
