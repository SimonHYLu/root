// @(#)root/sql:$Name:  $:$Id: TSQLObjectData.cxx,v 1.2 2005/11/22 20:42:36 pcanal Exp $
// Author: Sergey Linev  20/11/2005

/*************************************************************************
 * Copyright (C) 1995-2005, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//________________________________________________________________________
//
// TSQLObjectData is used in TBufferSQL2 class in reading procedure.
// It contains data, request from database table for one specifc
// object for one specific class. For instance, when data for
// class TH1 required, requests will be done to
// TH1_ver4 and TH1_streamer_ver4 tables and result of these reuests
// will be kept in single TSQLObjectData instance.
//
//________________________________________________________________________

#include "TSQLObjectData.h"

#include "TObjArray.h"
#include "TNamed.h"
#include "TSQLRow.h"
#include "TSQLResult.h"
#include "TSQLClassInfo.h"

ClassImp(TSQLObjectData)

//________________________________________________________________________
   TSQLObjectData::TSQLObjectData() :
      TObject(),
      fInfo(0),
      fObjId(0),
      fClassData(0),
      fBlobData(0),
      fLocatedColumn(-1),
      fClassRow(0),
      fBlobRow(0),
      fLocatedField(0),
      fLocatedValue(0),
      fCurrentBlob(kFALSE),
      fBlobName1(),
      fBlobName2(),
      fUnpack(0)
{
   // default contrsuctor
}

//______________________________________________________________________________
TSQLObjectData::TSQLObjectData(TSQLClassInfo* sqlinfo,
                               Int_t          objid,
                               TSQLResult*    classdata,
                               TSQLResult*    blobdata) :
   TObject(),
   fInfo(sqlinfo),
   fObjId(objid),
   fClassData(classdata),
   fBlobData(blobdata),
   fLocatedColumn(-1),
   fClassRow(0),
   fBlobRow(0),
   fLocatedField(0),
   fLocatedValue(0),
   fCurrentBlob(kFALSE),
   fBlobName1(),
   fBlobName2(),
   fUnpack(0)
{
   // normal contrsuctor,

   if (fClassData!=0)
      fClassRow = fClassData->Next();
   if (fBlobData!=0)
      fBlobRow = fBlobData->Next();

}

//______________________________________________________________________________
TSQLObjectData::~TSQLObjectData()
{
   // destructor of TSQLObjectData object

   if (fClassRow!=0) delete fClassRow;
   if (fBlobRow!=0) delete fBlobRow;
   if (fClassData!=0) delete fClassData;
   if (fBlobData!=0) delete fBlobData;
   if (fUnpack!=0) { fUnpack->Delete(); delete fUnpack; }
}

//______________________________________________________________________________
Int_t TSQLObjectData::GetNumClassFields()
{
   // return number of columns in class table result

   if (fClassData!=0) return fClassData->GetFieldCount();
   return 0;
}

//______________________________________________________________________________
const char* TSQLObjectData::GetClassFieldName(Int_t n)
{
   // get name of class table column

   if (fClassData!=0) return fClassData->GetFieldName(n);
   return 0;
}

//______________________________________________________________________________
Bool_t TSQLObjectData::LocateColumn(const char* colname, Bool_t isblob)
{
   // locate column of that name in results

   if (fUnpack!=0) {
      fUnpack->Delete();
      delete fUnpack;
      fUnpack = 0;
   }

   fLocatedField = 0;
   fLocatedValue = 0;
   fCurrentBlob = kFALSE;

   if ((fClassData==0) || (fClassRow==0)) return kFALSE;

   Int_t numfields = GetNumClassFields();

   for (Int_t ncol=1;ncol<numfields;ncol++) {
      const char* fieldname = GetClassFieldName(ncol);
      if (strcmp(colname, fieldname)==0) {
         fLocatedColumn = ncol;
         fLocatedField = fieldname;
         fLocatedValue = fClassRow->GetField(ncol);
         break;
      }
   }

   if (fLocatedField==0) return kFALSE;

   if (!isblob) return kTRUE;

   if (fBlobRow==0) return kFALSE;

   fCurrentBlob = kTRUE;

   ExtractBlobValues();

   return kTRUE;
}

//______________________________________________________________________________
Bool_t TSQLObjectData::ExtractBlobValues()
{
   // extract from blob table value and names

   if (fBlobRow==0) return kFALSE;

   fLocatedValue = fBlobRow->GetField(1);

   const char* name = fBlobRow->GetField(0);
   const char* separ = strstr(name, ":"); //SQLNameSeparator()

   if (separ==0) {
      fBlobName1 = "";
      fBlobName2 = name;
   } else {
      fBlobName1 = "";
      fBlobName1.Append(name, separ-name);
      separ+=strlen(":"); //SQLNameSeparator()
      fBlobName2 = separ;
   }

   return kTRUE;
}

//______________________________________________________________________________
void TSQLObjectData::AddUnpack(const char* tname, const char* value)
{
   // add emulated data
   // this used to place version or TObject raw data, read from normal tables

   TNamed* str = new TNamed(tname, value);
   if (fUnpack==0) {
      fUnpack = new TObjArray();
      fBlobName1 = "";
      fBlobName2 = str->GetName();
      fLocatedValue = str->GetTitle();
   }

   fUnpack->Add(str);
}

//______________________________________________________________________________
void TSQLObjectData::AddUnpackInt(const char* tname, Int_t value)
{
   // emulate integer value in raw data

   TString sbuf;
   sbuf.Form("%d", value);
   AddUnpack(tname, sbuf.Data());
}

//______________________________________________________________________________
void TSQLObjectData::ShiftToNextValue()
{
   // shift to next column or next row in blob data

   Bool_t doshift = kTRUE;

   if (fUnpack!=0) {
      TObject* prev = fUnpack->First();
      fUnpack->Remove(prev);
      delete prev;
      fUnpack->Compress();
      if (fUnpack->GetLast()>=0) {
         TNamed* curr = (TNamed*) fUnpack->First();
         fBlobName1 = "";
         fBlobName2 = curr->GetName();
         fLocatedValue = curr->GetTitle();
         return;
      }
      delete fUnpack;
      fUnpack = 0;
      doshift = kFALSE;
   }

   if (fCurrentBlob>0) {
      if (doshift) {
         delete fBlobRow;
         fBlobRow = fBlobData->Next();
      }
      ExtractBlobValues();
   } else
      if (fClassData!=0) {
         if (doshift) fLocatedColumn++;
         if (fLocatedColumn<GetNumClassFields()) {
            fLocatedField = GetClassFieldName(fLocatedColumn);
            fLocatedValue = fClassRow->GetField(fLocatedColumn);
         } else {
            fLocatedField = 0;
            fLocatedValue = 0;
         }
      }
}

//______________________________________________________________________________
Bool_t TSQLObjectData::VerifyDataType(const char* tname, Bool_t errormsg)
{
   // checks if data type corresponds to that stored in raw table

   if (tname==0) {
      if (errormsg)
         Error("VerifyDataType","Data type not specified");
      return kFALSE;
   }

   // here maybe type of column can be checked
   if (!IsBlobData()) return kTRUE;

   if (fBlobName2!=tname) {
      if (errormsg)
         Error("VerifyDataType","Data type meissmatch %s - %s", fBlobName2.Data(), tname);
      return kFALSE;
   }

   return kTRUE;
}

//______________________________________________________________________________
Bool_t TSQLObjectData::PrepareForRawData()
{
   // prepare to read data from raw table

   if (!ExtractBlobValues()) return kFALSE;

   fCurrentBlob = kTRUE;

   return kTRUE;
}

