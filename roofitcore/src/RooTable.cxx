/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooTable.cc,v 1.6 2002/09/05 04:34:01 verkerke Exp $
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
// RooTable is the abstract interface for table objects.
// Table objects are the category equivalent of RooPlot objects
// (which are used for real-valued objects)

#include "RooFitCore/RooTable.hh"

ClassImp(RooTable)


RooTable::RooTable(const char *name, const char *title) : TNamed(name,title)
{
}


RooTable::RooTable(const RooTable& other) : TNamed(other)
{
}


RooTable::~RooTable()
{
}


void RooTable::printToStream(ostream& os, PrintOption opt, TString indent) const
{
  os << indent << "RooTable" << endl ;
}
