// @(#)root/base:$Name:  $:$Id: TSystemDirectory.h,v 1.6 2005/05/30 10:21:14 rdm Exp $
// Author: Christian Bormann  13/10/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
#ifndef ROOT_TSystemDirectory
#define ROOT_TSystemDirectory


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSystemDirectory                                                     //
//                                                                      //
// Describes an Operating System directory for the browser.             //
//                                                                      //
// Author: Christian Bormann  30/09/97                                  //
//         http://www.ikf.physik.uni-frankfurt.de/~bormann/             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TSystemFile
#include "TSystemFile.h"
#endif

class TOrdCollection;
class TList;


class TSystemDirectory : public TSystemFile {

protected:
   TOrdCollection *fDirsInBrowser;
   TOrdCollection *fFilesInBrowser;

   Bool_t             IsDirectory(const char *name) const;
   TSystemDirectory  *FindDirObj(const char *name);
   TSystemFile       *FindFileObj(const char *name, const char *dir);

public:
   TSystemDirectory();
   TSystemDirectory(const char *dirname, const char *path);

   virtual ~TSystemDirectory();

   virtual Bool_t IsFolder() const { return kTRUE; }
   virtual Bool_t IsDirectory() const { return kTRUE; }

   virtual void   Browse(TBrowser *b);
   virtual void   Edit() { }
   virtual TList *GetListOfFiles() const;
   virtual void   SetDirectory(const char *name);
   virtual void   Delete() {}
   virtual void   Copy(const char *) {}
   virtual void   Move(const char *) {}

   // dummy methods from TObject
   void        DrawClass() const { }
   TObject    *DrawClone(Option_t *) const { return 0; }
   void        SetDrawOption(Option_t *) { }
   void        SetName(const char *) { }
   void        SetTitle(const char *) { }
   void        Delete(Option_t *) { }
   void        Copy(TObject & ) const { }
   ClassDef(TSystemDirectory,0)  //A system directory
};

#endif

