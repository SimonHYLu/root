// @(#)root/gviz3d:$Id$
// Author: Tomasz Sosnicki   18/09/09

/************************************************************************
* Copyright (C) 1995-2009, Rene Brun and Fons Rademakers.               *
* All rights reserved.                                                  *
*                                                                       *
* For the licensing terms see $ROOTSYS/LICENSE.                         *
* For the list of contributors see $ROOTSYS/README/CREDITS.             *
*************************************************************************/

#ifndef ROOT_TStructViewer
#define ROOT_TStructViewer

#include <TObject.h>
#include <TExMap.h>
#include <TObjArray.h>
#include <TList.h>
#include <TCanvas.h>

class TStructViewerGUI;
class TStructNode;

class TStructViewer : public TObject {

private:
   TObject          *fPointer;            // Main pointer to represented object
   TStructViewerGUI *fGUI;                // Pointer to GUI class
   TExMap            fLevelMembersCount;  // Contains number of objects on each level
   TExMap            fLevelSize;          // Contains total size in bytes of a level
   TExMap            fPointers;           // Map of pointer to avoid circulation
   TObjArray         fLevelArray;         // Array with pointers to nodes on each level
   TStructNode      *fTopNode;            // Master node for fPointer
   TList             fColors;             // List with properties used to color nodes


private:
   void     AddNode(TStructNode* node, ULong_t size);
   void     CountMembers(TClass* cl, TStructNode* parent);
   void     Prepare();
   void     Reset();

public:
   TStructViewer(TObject* ptr = NULL);
   ~TStructViewer();

   void     Draw(Option_t *option = "");
   TCanvas* GetCanvas();
   TExMap   GetLevelMembersCount() const;
   TExMap   GetLevelSize() const;
   Bool_t   GetLinksVisibility() const;
   TObject *GetPointer() const;
   void     SetColor(TString name, Int_t color);
   void     SetLinksVisibility(Bool_t val);
   void     SetPointer(TObject* ptr);

   ClassDef(TStructViewer, 1); // A 3D struct viewer
};

#endif
