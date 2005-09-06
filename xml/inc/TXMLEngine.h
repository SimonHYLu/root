// @(#)root/xml:$Name:  $:$Id: TXMLEngine.h,v 1.7 2004/12/22 16:50:08 rdm Exp $
// Author: Sergey Linev  10.05.2004

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TXMLEngine
#define ROOT_TXMLEngine

#ifndef ROOT_TObject
#include "TObject.h"
#endif

typedef void* XMLNodePointer_t;
typedef void* XMLNsPointer_t;
typedef void* XMLAttrPointer_t;
typedef void* XMLDocPointer_t;

class TXMLInputStream;
class TXMLOutputStream;

class TXMLEngine : public TObject {
   public:
      TXMLEngine();
      virtual ~TXMLEngine();
      
      Bool_t            HasAttr(XMLNodePointer_t xmlnode, const char* name);
      const char*       GetAttr(XMLNodePointer_t xmlnode, const char* name);
      Int_t             GetIntAttr(XMLNodePointer_t node, const char* name);
      XMLAttrPointer_t  NewAttr(XMLNodePointer_t xmlnode, XMLNsPointer_t,
                                const char* name, const char* value);
      XMLAttrPointer_t  NewIntAttr(XMLNodePointer_t xmlnode, const char* name, Int_t value);
      void              FreeAttr(XMLNodePointer_t xmlnode, const char* name);
      XMLNodePointer_t  NewChild(XMLNodePointer_t parent, XMLNsPointer_t ns,
                                 const char* name, const char* content = 0);
      XMLNsPointer_t    NewNS(XMLNodePointer_t xmlnode, const char* reference, const char* name = 0);
      void              AddChild(XMLNodePointer_t parent, XMLNodePointer_t child);
      void              UnlinkNode(XMLNodePointer_t node);
      void              FreeNode(XMLNodePointer_t xmlnode);
      void              UnlinkFreeNode(XMLNodePointer_t xmlnode);
      const char*       GetNodeName(XMLNodePointer_t xmlnode);
      const char*       GetNodeContent(XMLNodePointer_t xmlnode);
      XMLNodePointer_t  GetChild(XMLNodePointer_t xmlnode);
      XMLNodePointer_t  GetParent(XMLNodePointer_t xmlnode);
      XMLNodePointer_t  GetNext(XMLNodePointer_t xmlnode);
      void              ShiftToNext(XMLNodePointer_t &xmlnode);
      Bool_t            IsEmptyNode(XMLNodePointer_t) { return kFALSE; } // obsolete, should not be used
      void              SkipEmpty(XMLNodePointer_t &) {}                 // obsolete, should not be used
      void              CleanNode(XMLNodePointer_t xmlnode);
      XMLDocPointer_t   NewDoc(const char* version = 0);
      void              AssignDtd(XMLDocPointer_t xmldoc, const char* dtdname, const char* rootname);
      void              FreeDoc(XMLDocPointer_t xmldoc);
      void              SaveDoc(XMLDocPointer_t xmldoc, const char* filename, Int_t layout = 1);
      void              DocSetRootElement(XMLDocPointer_t xmldoc, XMLNodePointer_t xmlnode);
      XMLNodePointer_t  DocGetRootElement(XMLDocPointer_t xmldoc);
      XMLDocPointer_t   ParseFile(const char* filename);
      Bool_t            ValidateDocument(XMLDocPointer_t, Bool_t = kFALSE) { return kFALSE; } // obsolete
   protected:
      char*             Makestr(const char* str);   
      char*             Makenstr(const char* start, int len);
      XMLNodePointer_t  AllocateNode(int namelen, XMLNodePointer_t parent);
      XMLAttrPointer_t  AllocateAttr(int namelen, int valuelen, XMLNodePointer_t xmlnode);
      XMLNsPointer_t    FindNs(XMLNodePointer_t xmlnode, const char* nsname);
      void              TruncateNsExtension(XMLNodePointer_t xmlnode);
      void              UnpackSpecialCharacters(char* target, const char* source, int srclen);
      void              OutputValue(Char_t* value, TXMLOutputStream* out);
      void              SaveNode(XMLNodePointer_t xmlnode, TXMLOutputStream* out, Int_t layout, Int_t level);
      XMLNodePointer_t  ReadNode(XMLNodePointer_t xmlparent, TXMLInputStream* inp, Int_t& resvalue);
  
   ClassDef(TXMLEngine,1);
};

#endif

