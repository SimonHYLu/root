/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooFitCore
 *    File: $Id: RooAbsIntegrator.cc,v 1.5 2001/05/16 07:41:06 verkerke Exp $
 * Authors:
 *   DK, David Kirkby, Stanford University, kirkby@hep.stanford.edu
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu
 * History:
 *   07-Mar-2001 WV Created initial version
 *
 * Copyright (C) 2001 University of California
 *****************************************************************************/

// -- CLASS DESCRIPTION --
// RooAbsIntegrator is the abstract interface for numerical integration engines


#include "RooFitCore/RooAbsIntegrator.rdl"

ClassImp(RooAbsIntegrator)
;


RooAbsIntegrator::RooAbsIntegrator(const RooAbsPdf& function, Int_t mode) : 
  _function((RooAbsPdf*)&function), _mode(mode) 
{
}


RooAbsIntegrator::RooAbsIntegrator(const RooAbsIntegrator& other) : _function(other._function), _mode(other._mode)
{
}


RooAbsIntegrator::~RooAbsIntegrator()
{
}
