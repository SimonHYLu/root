// @(#)root/xml:$Name:  $:$Id: TXMLBuffer.h,v 1.2 2004/05/10 23:50:27 rdm Exp $
// Author: Sergey Linev  10.05.2004

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TXMLBuffer
#define ROOT_TXMLBuffer

#ifndef ROOT_TBuffer
#include "TBuffer.h"
#endif
#ifndef ROOT_TXMLSetup
#include "TXMLSetup.h"
#endif
#ifndef ROOT_TXMLEngine
#include "TXMLEngine.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif
#ifndef ROOT_TObjArray
#include "TObjArray.h"
#endif


class TExMap;
class TStreamerInfo;
class TStreamerElement;
class TObjArray;
class TMemberStreamer;
class TXMLDtdGenerator;
class TXMLFile;
class TXMLStackObj;


class TXMLBuffer : public TBuffer, public TXMLSetup {
   public:

      TXMLBuffer(TBuffer::EMode mode, const TXMLSetup& setup, TXMLFile* file = 0);
      virtual ~TXMLBuffer();

      xmlNodePointer XmlWrite(const TObject* obj);
      xmlNodePointer XmlWrite(const void* obj, const TClass* cl);

      TObject* XmlRead(xmlNodePointer node);
      void* XmlReadAny(xmlNodePointer node);

      void SetDtdGenerator(TXMLDtdGenerator* gen) { fDtdGener = gen; }

      // suppress class writing/reading

      virtual TClass*   ReadClass(const TClass* cl = 0, UInt_t* objTag = 0);
      virtual void      WriteClass(const TClass* cl);

      // redefined virtual functions of TBuffer

      virtual Int_t     CheckByteCount(UInt_t startpos, UInt_t bcnt, const TClass *clss); // SL
      virtual Int_t     CheckByteCount(UInt_t startpos, UInt_t bcnt, const char *classname); // SL
      virtual void      SetByteCount(UInt_t cntpos, Bool_t packInVersion = kFALSE);  // SL

      virtual Version_t ReadVersion(UInt_t *start = 0, UInt_t *bcnt = 0, const TClass *cl = 0);  // SL
      virtual UInt_t    WriteVersion(const TClass *cl, Bool_t useBcnt = kFALSE);  // SL

      virtual void*     ReadObjectAny(const TClass* clCast);

      virtual void      IncrementLevel(TStreamerInfo*);
      virtual void      SetStreamerElementNumber(Int_t);
      virtual void      DecrementLevel(TStreamerInfo*);

      // end of redefined virtual functions of XMLStyle1

      virtual void      WriteObject(const TObject *obj);


      // redefined virtual functions of TBuffer

      virtual Int_t    ReadArray(Bool_t    *&b);
      virtual Int_t    ReadArray(Char_t    *&c);
      virtual Int_t    ReadArray(UChar_t   *&c);
      virtual Int_t    ReadArray(Short_t   *&h);
      virtual Int_t    ReadArray(UShort_t  *&h);
      virtual Int_t    ReadArray(Int_t     *&i);
      virtual Int_t    ReadArray(UInt_t    *&i);
      virtual Int_t    ReadArray(Long_t    *&l);
      virtual Int_t    ReadArray(ULong_t   *&l);
      virtual Int_t    ReadArray(Long64_t  *&l);
      virtual Int_t    ReadArray(ULong64_t *&l);
      virtual Int_t    ReadArray(Float_t   *&f);
      virtual Int_t    ReadArray(Double_t  *&d);
      virtual Int_t    ReadArrayDouble32(Double_t  *&d);

      virtual Int_t    ReadStaticArray(Bool_t    *b);
      virtual Int_t    ReadStaticArray(Char_t    *c);
      virtual Int_t    ReadStaticArray(UChar_t   *c);
      virtual Int_t    ReadStaticArray(Short_t   *h);
      virtual Int_t    ReadStaticArray(UShort_t  *h);
      virtual Int_t    ReadStaticArray(Int_t     *i);
      virtual Int_t    ReadStaticArray(UInt_t    *i);
      virtual Int_t    ReadStaticArray(Long_t    *l);
      virtual Int_t    ReadStaticArray(ULong_t   *l);
      virtual Int_t    ReadStaticArray(Long64_t  *l);
      virtual Int_t    ReadStaticArray(ULong64_t *l);
      virtual Int_t    ReadStaticArray(Float_t   *f);
      virtual Int_t    ReadStaticArray(Double_t  *d);
      virtual Int_t    ReadStaticArrayDouble32(Double_t  *d);

      virtual void     ReadFastArray(Bool_t    *b, Int_t n);
      virtual void     ReadFastArray(Char_t    *c, Int_t n);
      virtual void     ReadFastArray(UChar_t   *c, Int_t n);
      virtual void     ReadFastArray(Short_t   *h, Int_t n);
      virtual void     ReadFastArray(UShort_t  *h, Int_t n);
      virtual void     ReadFastArray(Int_t     *i, Int_t n);
      virtual void     ReadFastArray(UInt_t    *i, Int_t n);
      virtual void     ReadFastArray(Long_t    *l, Int_t n);
      virtual void     ReadFastArray(ULong_t   *l, Int_t n);
      virtual void     ReadFastArray(Long64_t  *l, Int_t n);
      virtual void     ReadFastArray(ULong64_t *l, Int_t n);
      virtual void     ReadFastArray(Float_t   *f, Int_t n);
      virtual void     ReadFastArray(Double_t  *d, Int_t n);
      virtual void     ReadFastArrayDouble32(Double_t  *d, Int_t n);

      virtual void     WriteArray(const Bool_t    *b, Int_t n);
      virtual void     WriteArray(const Char_t    *c, Int_t n);
      virtual void     WriteArray(const UChar_t   *c, Int_t n);
      virtual void     WriteArray(const Short_t   *h, Int_t n);
      virtual void     WriteArray(const UShort_t  *h, Int_t n);
      virtual void     WriteArray(const Int_t     *i, Int_t n);
      virtual void     WriteArray(const UInt_t    *i, Int_t n);
      virtual void     WriteArray(const Long_t    *l, Int_t n);
      virtual void     WriteArray(const ULong_t   *l, Int_t n);
      virtual void     WriteArray(const Long64_t  *l, Int_t n);
      virtual void     WriteArray(const ULong64_t *l, Int_t n);
      virtual void     WriteArray(const Float_t   *f, Int_t n);
      virtual void     WriteArray(const Double_t  *d, Int_t n);
      virtual void     WriteArrayDouble32(const Double_t  *d, Int_t n);
      virtual void     ReadFastArray(void  *start , const TClass *cl, Int_t n=1, TMemberStreamer *s=0);
      virtual void     ReadFastArray(void **startp, const TClass *cl, Int_t n=1, Bool_t isPreAlloc=kFALSE, TMemberStreamer *s=0);

      virtual void     WriteFastArray(const Bool_t    *b, Int_t n);
      virtual void     WriteFastArray(const Char_t    *c, Int_t n);
      virtual void     WriteFastArray(const UChar_t   *c, Int_t n);
      virtual void     WriteFastArray(const Short_t   *h, Int_t n);
      virtual void     WriteFastArray(const UShort_t  *h, Int_t n);
      virtual void     WriteFastArray(const Int_t     *i, Int_t n);
      virtual void     WriteFastArray(const UInt_t    *i, Int_t n);
      virtual void     WriteFastArray(const Long_t    *l, Int_t n);
      virtual void     WriteFastArray(const ULong_t   *l, Int_t n);
      virtual void     WriteFastArray(const Long64_t  *l, Int_t n);
      virtual void     WriteFastArray(const ULong64_t *l, Int_t n);
      virtual void     WriteFastArray(const Float_t   *f, Int_t n);
      virtual void     WriteFastArray(const Double_t  *d, Int_t n);
      virtual void     WriteFastArrayDouble32(const Double_t  *d, Int_t n);
      virtual void     WriteFastArray(void  *start,  const TClass *cl, Int_t n=1, TMemberStreamer *s=0);
      virtual Int_t    WriteFastArray(void **startp, const TClass *cl, Int_t n=1, Bool_t isPreAlloc=kFALSE, TMemberStreamer *s=0);

      virtual void     StreamObject(void *obj, const type_info &typeinfo);
      virtual void     StreamObject(void *obj, const char *className);
      virtual void     StreamObject(void *obj, const TClass *cl);

      virtual TBuffer  &operator>>(Bool_t    &b);
      virtual TBuffer  &operator>>(Char_t    &c);
      virtual TBuffer  &operator>>(UChar_t   &c);
      virtual TBuffer  &operator>>(Short_t   &h);
      virtual TBuffer  &operator>>(UShort_t  &h);
      virtual TBuffer  &operator>>(Int_t     &i);
      virtual TBuffer  &operator>>(UInt_t    &i);
      virtual TBuffer  &operator>>(Long_t    &l);
      virtual TBuffer  &operator>>(ULong_t   &l);
      virtual TBuffer  &operator>>(Long64_t  &l);
      virtual TBuffer  &operator>>(ULong64_t &l);
      virtual TBuffer  &operator>>(Float_t   &f);
      virtual TBuffer  &operator>>(Double_t  &d);
      virtual TBuffer  &operator>>(Char_t    *c);

      virtual TBuffer  &operator<<(Bool_t    b);
      virtual TBuffer  &operator<<(Char_t    c);
      virtual TBuffer  &operator<<(UChar_t   c);
      virtual TBuffer  &operator<<(Short_t   h);
      virtual TBuffer  &operator<<(UShort_t  h);
      virtual TBuffer  &operator<<(Int_t     i);
      virtual TBuffer  &operator<<(UInt_t    i);
      virtual TBuffer  &operator<<(Long_t    l);
      virtual TBuffer  &operator<<(ULong_t   l);
      virtual TBuffer  &operator<<(Long64_t  l);
      virtual TBuffer  &operator<<(ULong64_t l);
      virtual TBuffer  &operator<<(Float_t   f);
      virtual TBuffer  &operator<<(Double_t  d);
      virtual TBuffer  &operator<<(const Char_t *c);

      // end of redefined virtual functions

   protected:
      // redefined protected virtual functions

      virtual void  WriteObject(const void *actualObjStart, const TClass *actualClass);

      // end redefined protected virtual functions

      TXMLBuffer();

      //TXMLBuffer(const TXMLBuffer&);

      TXMLStackObj* PushStack(xmlNodePointer current, Bool_t simple = kFALSE);
      void PopStack();
      void ShiftStack(const char* info = 0);

      xmlNodePointer StackNode();
      TXMLStackObj* Stack(Int_t depth);

      Bool_t VerifyNode(xmlNodePointer node, const char* name, const char* errinfo = 0);
      Bool_t VerifyStackNode(const char* name, const char* errinfo = 0);
      
      xmlNodePointer CreateItemNode(const char* name);
      Bool_t VerifyItemNode(const char* name, const char* errinfo = 0);

      Bool_t VerifyProp(xmlNodePointer node, const char* propname, const char* propvalue, const char* errinfo = 0);
      Bool_t VerifyStackProp(const char* propname, const char* propvalue, const char* errinfo = 0);

      void XmlWriteBlock(Bool_t force = kFALSE);
      void XmlReadBlock(xmlNodePointer node = 0);

      Bool_t ProcessPointer(const void* ptr, xmlNodePointer node);
      void RegisterPointer(const void* ptr, xmlNodePointer node);
      Bool_t ExtractPointer(xmlNodePointer node, void* &ptr);
      void ExtractObjectId(xmlNodePointer node, const void* ptr);

      xmlNodePointer XmlWriteBasic(Char_t value);
      xmlNodePointer XmlWriteBasic(Short_t value);
      xmlNodePointer XmlWriteBasic(Int_t value);
      xmlNodePointer XmlWriteBasic(Long_t value);
      xmlNodePointer XmlWriteBasic(Long64_t value);
      xmlNodePointer XmlWriteBasic(Float_t value);
      xmlNodePointer XmlWriteBasic(Double_t value);
      xmlNodePointer XmlWriteBasic(Bool_t value);
      xmlNodePointer XmlWriteBasic(UChar_t value);
      xmlNodePointer XmlWriteBasic(UShort_t value);
      xmlNodePointer XmlWriteBasic(UInt_t value);
      xmlNodePointer XmlWriteBasic(ULong_t value);
      xmlNodePointer XmlWriteBasic(ULong64_t value);
      xmlNodePointer XmlWriteValue(const char* value, const char* name);

      void XmlReadBasic(Char_t& value);
      void XmlReadBasic(Short_t& value);
      void XmlReadBasic(Int_t& value);
      void XmlReadBasic(Long_t& value);
      void XmlReadBasic(Long64_t& value);
      void XmlReadBasic(Float_t& value);
      void XmlReadBasic(Double_t& value);
      void XmlReadBasic(Bool_t& value);
      void XmlReadBasic(UChar_t& value);
      void XmlReadBasic(UShort_t& value);
      void XmlReadBasic(UInt_t& value);
      void XmlReadBasic(ULong_t& value);
      void XmlReadBasic(ULong64_t& value);
      const char* XmlReadValue(const char* name);

      TXMLFile* XmlFile();

      xmlNodePointer XmlWriteObject(const void* obj, const TClass* objClass);
      void* XmlReadObject(void* obj);

      void CreateElemNode(const TStreamerElement* elem, Int_t number = -1);
      Bool_t VerifyElemNode(const TStreamerElement* elem, Int_t number = -1);

      void              BeforeIOoperation();
      void              CheckVersionBuf();
      
      Int_t             fStoredBuffePos;       //!

      TObjArray         fStack;                //!

      Version_t         fVersionBuf;           //!

      TXMLDtdGenerator* fDtdGener;             //!
      TXMLFile*         fXmlFile;              //!

      TExMap*           fObjMap;               //!
      TObjArray*        fIdArray;              //!

      TString           fValueBuf;             //!

      Int_t             fErrorFlag;            //!

      Bool_t            fCanUseCompact;        //!
      Bool_t            fExpectedChain;        //!

   ClassDef(TXMLBuffer,1);
};

#endif


