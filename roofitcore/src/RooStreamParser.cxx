/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooFitCore
 *    File: $Id$
 * Authors:
 *   DK, David Kirkby, Stanford University, kirkby@hep.stanford.edu
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu
 * History:
 *   07-Mar-2001 WV Created initial version
 *
 * Copyright (C) 2001 University of California
 *****************************************************************************/

#include <iostream.h>
#include <stdlib.h>
#include <ctype.h>
#include "RooFitCore/RooStreamParser.hh"


ClassImp(RooStreamParser)

RooStreamParser::RooStreamParser(istream& is) : 
  _is(is), _prefix("")
{
}

RooStreamParser::RooStreamParser(istream& is, TString errorPrefix) : 
  _is(is), _prefix(errorPrefix)
{
}


RooStreamParser::~RooStreamParser()
{
}


TString RooStreamParser::readToken() 
{
  // Smart tokenizer. Absorb white space and token must be either punctuation or alphanum
  Bool_t isPunct(kFALSE), first(kTRUE) ;
  char buffer[1024], c, cnext, cprev=' ' ;
  Int_t bufptr=0 ;

  while(1) {
    _is.get(c) ;
    if (!_is.good() || c=='\n') break ;
    if (isspace(c)) {
      if (first) 
	continue ; 
      else 
	break ;
    }

    // If '-' or '/' see what the next character is
    if (c=='-' || c=='/') {
      _is.get(cnext) ;
      _is.putback(cnext) ;
    }
    
    // Stop if begin of comments is encountered
    if (c=='/' && cnext=='/') {
      zapToEnd() ;
      break ;
    }
    
    // Decide if token is punctuation or alphanumeric
    if (isalnum(c) || c=='.' || c=='*' || (c=='-' && (isdigit(cnext) || cnext=='.'))) {
      if (!first && isPunct) {
	// Punct -> Alpha, end token
	_is.putback(c) ;
	break ;
      }
    } else {
      if (first)
	isPunct=kTRUE ;
      else if (!isPunct) {
	// Alpha -> Punct, end token
	_is.putback(c) ;
	break ;
      }
    }

    // Store in buffer
    buffer[bufptr++]=c ;
    first=kFALSE ;
    cprev=c ;
  }  

  // Zero terminate buffer and convert to TString
  buffer[bufptr]=0 ;
  return TString(buffer) ;
}


TString RooStreamParser::readLine() 
{
  char buffer[1024] ;

  // Read till end of line
  _is.getline(buffer,1024,'\n') ;

  // Chop eventual comments
  char *pcomment = strstr(buffer,"//") ;
  if (pcomment) *pcomment=0 ;

  // Chop leading and trailing space
  char *pstart=buffer ;
  while (isspace(*pstart)) {
    pstart++ ;
  }
  char *pend=buffer+strlen(buffer)-1 ;
  if (pend>pstart)
    while (isspace(*pend)) { *pend--=0 ; }
  
  // Convert to TString
  return TString(pstart) ;
}




void RooStreamParser::zapToEnd() 
{
  _is.ignore(1000,'\n') ;
}

Bool_t RooStreamParser::expectToken(TString expected, Bool_t zapOnError) 
{
  TString token=readToken() ;

  Bool_t error=token.CompareTo(expected) ;
  if (error && !_prefix.IsNull()) {
    cout << _prefix << ": parse error, expected '" 
	 << expected << "'" << ", got '" << token << "'" << endl ;
    if (zapOnError) zapToEnd() ;
  }
  return error ;
}


Bool_t RooStreamParser::readDouble(Double_t& value, Bool_t zapOnError) 
{
  TString token=readToken() ;
  return convertToDouble(token,value) ;
  
}


Bool_t RooStreamParser::convertToDouble(TString token, Double_t& value) 
{
  char* endptr(0) ;
  const char* data=token ;
  value = strtod(data,&endptr) ;
  Bool_t error = (endptr-data!=token.Length()) ;

  if (error && !_prefix.IsNull()) {
    cout << _prefix << ": parse error, cannot convert '" 
	 << token << "'" << " to double precision" <<  endl ;
  }
  return error ;
}


Bool_t RooStreamParser::readInteger(Int_t value, Bool_t zapOnError) 
{
  TString token=readToken() ;
  return convertToInteger(token,value) ;
}


Bool_t RooStreamParser::convertToInteger(TString token, Int_t& value) 
{
  char* endptr(0) ;
  const char* data=token ;
  value = strtol(data,&endptr,10) ;
  Bool_t error = (endptr-data!=token.Length()) ;

  if (error && !_prefix.IsNull()) {
    cout << _prefix << ": parse error, cannot convert '" 
	 << token << "'" << " to integer" <<  endl ;
  }
  return error ;
}


Bool_t RooStreamParser::readString(TString& value, Bool_t zapOnError) 
{
  TString token=readToken() ;
  return convertToString(token,value) ;
}


Bool_t RooStreamParser::convertToString(TString token, TString& string) 
{
  string=token ;
  return kFALSE ;
}





