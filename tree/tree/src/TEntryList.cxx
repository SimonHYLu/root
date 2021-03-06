// @(#)root/tree:$Id$
// Author: Anna Kreshuk 27/10/2006

/*************************************************************************
 * Copyright (C) 1995-2006, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//______________________________________________________________________________
/* Begin_Html
<center><h2>TEntryList: a List of entry numbers in a TTree or TChain</h2></center>

There are two types of entry lists:
<ol>
<li> for a TTree (fBlocks data member is non-zero)
  Entry numbers are stored in TEntryListBlocks, which, in their turn, are stored
  in the TObjArray fBlocks. The range of the entry numbers is cut into intervals
  of kBlockSize entries (currently 64000), so that the first block contains
  information which entries out of the first 64000 pass the selection, the second
  block - which entries out of the 64000-127999 interval pass the selection, etc.
  Some blocks, obviously, might be empty. The internal representation of entry
  numbers in the blocks is described in the TEntryListBlock class description, and
  this representation might be changed by calling OptimizeStorage() function
  (when the list is filled via the Enter() function, this is done automatically,
  except for the last block).
  Individual entry lists can be merged (functions Merge() and Add())
  to make an entry list for a TChain of corresponding TTrees.
End_Html
Begin_Macro(source)
entrylist_figure1.C
End_Macro

Begin_Html
 
<li> for a TChain (fLists data member is non-zero)
  It contains a TList of sub-lists (TEntryList objects, corresponding to each TTree)
  Trees and lists are matched by the TTree name and its file name (full path). 
  All sub-lists are returned by the GetLists() function and individual lists are
  returned by GetEntryList() function. Such lists are no different from the lists for
  TTrees, described above.
End_Html
Begin_Macro(source)
entrylist_figure2.C
End_Macro

Begin_Html
</ol>
             <h4>Operations on entry lists</h4>

<ul>
<li> <b>Add</b>() - if the lists are for the same tree, adds all the entries of the second list
          to the first list. If the lists are for different trees, creates a TEntryList
          with 2 sublists for each TTree. If the lists are for TChains, merges the ones
          for the same trees and adds new sublists for the TTrees that were not included
          in the first TEntryList
<li> <b>Subtract</b>() - if the lists are for the same TTree, removes the entries of the second
               list from the first list. If the lists are for TChains, loops over all
               sub-lists
<li> <b>GetEntry(n)</b> - returns the n-th entry number 
<li> <b>Next</b>()      - returns next entry number. Note, that this function is 
                much faster than GetEntry, and it's called when GetEntry() is called
                for 2 or more indices in a row.
</ul>

                     <h4>TTree::Draw() and TChain::Draw()</h4>

  Use option <b>entrylist</b> to write the results of TTree::Draw and TChain::Draw into 
  an entry list. Example:
<pre>
     tree->Draw(">>elist", "x<0 && y>0", "entrylist");
     TEntryList *elist = (TEntryList*)gDirectory->Get("elist");
</pre>

                                <h4> Example of Loop en TEntryList with a TChain</h4>
<pre>
void loopChain() {
   TFile *fe = TFile::Open("myelist.root");
   TEntryList *myelist=(TEntryList*)fe->Get("myelist");
   TChain *ch = new TChain("ntuple");
   ch->Add("hsimple.root");
   ch->Add("hsimple2.root");
   Long64_t listEntries=myelist->GetN();
   Long64_t chainEntries = ch->GetEntries();
   Int_t treenum=0;
   ch->SetEntryList(myelist);
   for (Long64_t el =0;el<listEntries;el++) {
      Long64_t treeEntry = myelist->GetEntryAndTree(el,treenum);
      Long64_t chainEntry = treeEntry+ch->GetTreeOffset()[treenum];
      printf("el=%lld, treeEntry=%lld, chainEntry=%lld, treenum=%d\n",el,treeEntry,chainEntry,treenum);
   }
}
</pre>

                                <h4> TSelectors</h4>

  To fill an TEntryList from a TSelector correctly, one must add the TEntryList object
  to the output list of the selector (TSelector::fOutput). This is the only way to 
  make the sub-lists of the TEntryList switch when the current tree of the TChain is
  changed. 

   <h4> Using a TEntryList as input (TTree::SetEntryList() and TChain::SetEntryList())</h4>

  while the TTree::SetEntryList() function is only setting the TTree::fEntryList
  data member, the same function in TChain also finds correspondance between
  the TTrees of this TChain and the sub-lists of this TEntryList.

                       <h4>TEntryList and the current directory</h4>

  TEntryList objects are automatically added to the current directory (like TTrees).
  However, in case of a TEntryList for a chain, only the top-level entry list is added,
  not the sub-lists for specific trees. Placing entry lists in the current directory
  allows calling them as a part of a TTreeFormula expression, so if the user wants 
  to extract a sublist from a TChain entry list via the GetEntryList() or some other
  function, he has to add it to the current directory to be able to use it in 
  TTreeFormula expressions.

                      <h4>TEntryList and TEventList</h4>

  TTree::SetEventList() and TChain::SetEventList() transform a TEventList into a TEntryList
  See comments to those functions for more details
End_Html */


#include "TEntryList.h"
#include "TEntryListBlock.h"
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"

ClassImp(TEntryList)

//______________________________________________________________________________
TEntryList::TEntryList() : fEntriesToProcess(0)
{
   //default c-tor

   fLists = 0;
   fCurrent = 0;
   fBlocks = 0;
   fN = 0;
   fNBlocks = 0;
   fTreeName = "";
   fFileName = "";
   fStringHash = 0;
   fTreeNumber = -1;
   fDirectory = 0;
   fReapply = kFALSE;
   fLastIndexQueried = -1;
   fLastIndexReturned = 0;
   fShift = kFALSE;
}

//______________________________________________________________________________
TEntryList::TEntryList(const char *name, const char *title) : 
   TNamed(name, title),
   fEntriesToProcess(0)
{
   //c-tor with name and title

   fLists = 0;
   fCurrent = 0;
   fBlocks = 0;
   fN = 0;
   fNBlocks = 0;
   fTreeName = "";
   fFileName = "";
   fStringHash = 0;
   fTreeNumber = -1;
   fReapply = kFALSE;

   fDirectory  = gDirectory;
   if (fDirectory) fDirectory->Append(this);

   fLastIndexQueried = -1;
   fLastIndexReturned = 0;
   fShift = kFALSE;
}

//______________________________________________________________________________
TEntryList::TEntryList(const char *name, const char *title, const TTree *tree):TNamed(name, title)
{
   //constructor with name and title, which also sets the tree

   fLists = 0;
   fCurrent = 0;
   fBlocks = 0;
   fN = 0;
   fNBlocks = 0;
   fTreeNumber = -1;
   SetTree(tree);
   fReapply = kFALSE;

   fDirectory  = gDirectory;
   if (fDirectory) fDirectory->Append(this);

   fLastIndexQueried = -1;
   fLastIndexReturned = 0;
   fShift = kFALSE;
}

//______________________________________________________________________________
TEntryList::TEntryList(const char *name, const char *title, const char *treename, const char *filename) : TNamed(name, title),fEntriesToProcess(0)
{
   //c-tor with name and title, which also sets the treename and the filename

   fLists = 0;
   fCurrent = 0;
   fBlocks = 0;
   fNBlocks = 0;
   fN = 0;
   SetTree(treename, filename);
   fTreeNumber = -1;
   fReapply = kFALSE;

   fDirectory  = gDirectory;
   if (fDirectory) fDirectory->Append(this);

   fLastIndexQueried = -1;
   fLastIndexReturned = 0;
   fShift = kFALSE;
}

//______________________________________________________________________________
TEntryList::TEntryList(const TTree *tree) : fEntriesToProcess(0)
{
   //c-tor, which sets the tree

   fLists = 0;
   fCurrent = 0;
   fBlocks = 0;
   fNBlocks = 0;
   fN = 0;

   SetTree(tree);
   fTreeNumber = -1;

   fReapply = kFALSE;
   fDirectory  = gDirectory;
   if (fDirectory) fDirectory->Append(this);

   fLastIndexQueried = -1;
   fLastIndexReturned = 0;
   fShift = kFALSE;
}

//______________________________________________________________________________
TEntryList::TEntryList(const TEntryList &elist) : TNamed(elist)
{
   //copy c-tor

   fNBlocks = elist.fNBlocks;
   fTreeName = elist.fTreeName;
   fFileName = elist.fFileName;
   fStringHash = elist.fStringHash;
   fTreeNumber = elist.fTreeNumber;
   fLastIndexQueried = -1;
   fLastIndexReturned = 0;
   fN = elist.fN;
   fShift = elist.fShift;
   fLists = 0;
   fBlocks = 0;
   fReapply = elist.fReapply;
   fCurrent = 0;
   fEntriesToProcess = elist.fEntriesToProcess;
   if (elist.fLists){
      fLists = new TList();
      TEntryList *el1 = 0;
      TEntryList *el2 = 0;
      TIter next(elist.fLists);
      while((el1 = (TEntryList*)next())){
         el2 = new TEntryList(*el1);
         if (el1==elist.fCurrent)
            fCurrent = el2;
         fLists->Add(el2);
      }
   } else {
      if (elist.fBlocks){
         TEntryListBlock *block1 = 0;
         TEntryListBlock *block2 = 0;
         //or just copy it as a TObjArray??
         fBlocks = new TObjArray();
         for (Int_t i=0; i<fNBlocks; i++){
            block1 = (TEntryListBlock*)elist.fBlocks->UncheckedAt(i);
            block2 = new TEntryListBlock(*block1);
            fBlocks->Add(block2);
         }
      }
      fCurrent = this;
   }
   fDirectory  = 0;

}


//______________________________________________________________________________
TEntryList::~TEntryList()
{
   // Destructor.

   if (fBlocks){
      fBlocks->Delete();
      delete fBlocks;
   }
   fBlocks = 0;
   if (fLists){
      fLists->Delete();
      delete fLists;
   }
   
   fLists = 0;

   if (fDirectory) fDirectory->Remove(this);
   fDirectory  = 0;

}

//______________________________________________________________________________
void TEntryList::Add(const TEntryList *elist)
{
   //Add 2 entry lists

   if (fN==0){
      if (!fLists && fTreeName=="" && fFileName==""){
         //this list is empty. copy the other list completely
         fNBlocks = elist->fNBlocks;
         fTreeName = elist->fTreeName;
         fFileName = elist->fFileName;
         fStringHash = elist->fStringHash;
         fTreeNumber = elist->fTreeNumber;
         fLastIndexQueried = -1;
         fLastIndexReturned = 0;
         fN = elist->fN;
         if (elist->fLists){
            fLists = new TList();
            TEntryList *el1 = 0;
            TEntryList *el2 = 0;
            TIter next(elist->fLists);
            while((el1 = (TEntryList*)next())){
               el2 = new TEntryList(*el1);
               if (el1==elist->fCurrent)
                  fCurrent = el2;
               fLists->Add(el2);
            }
         } else {
            if (elist->fBlocks){
               TEntryListBlock *block1 = 0;
               TEntryListBlock *block2 = 0;
               fBlocks = new TObjArray();
               for (Int_t i=0; i<fNBlocks; i++){
                  block1 = (TEntryListBlock*)elist->fBlocks->UncheckedAt(i);
                  block2 = new TEntryListBlock(*block1);
                  fBlocks->Add(block2);
               }
            }
            fCurrent = 0;
         }
         return;
      }
   }

   if (!fLists){
      if (!elist->fLists){
         if (!strcmp(elist->fTreeName.Data(),fTreeName.Data()) && !strcmp(elist->fFileName.Data(),fFileName.Data())){
            //entry lists are for the same tree
            if (!elist->fBlocks)
               //the other list is empty list
               return;
            if (!fBlocks){
               //this entry list is empty
               TEntryListBlock *block1 = 0;
               TEntryListBlock *block2 = 0;
               fNBlocks = elist->fNBlocks;
               fN = elist->fN;
               fBlocks = new TObjArray();
               for (Int_t i=0; i<fNBlocks; i++){
                  block1 = (TEntryListBlock*)elist->fBlocks->UncheckedAt(i);
                  block2 = new TEntryListBlock(*block1);
               fBlocks->Add(block2);
               }
               return;
            }
            //both not empty, merge block by block
            TEntryListBlock *block1=0;
            TEntryListBlock *block2=0;
            Int_t i;
            Int_t nmin = TMath::Min(fNBlocks, elist->fNBlocks);
            Long64_t nnew, nold;
            for (i=0; i<nmin; i++){
               block1 = (TEntryListBlock*)fBlocks->UncheckedAt(i);
               block2 = (TEntryListBlock*)elist->fBlocks->UncheckedAt(i);
               nold = block1->GetNPassed();
               nnew = block1->Merge(block2);
               fN = fN - nold + nnew;
            }
            if (fNBlocks<elist->fNBlocks){
               Int_t nmax = elist->fNBlocks; 
               for (i=nmin; i<nmax; i++){
                  block2 = (TEntryListBlock*)elist->fBlocks->UncheckedAt(i);
                  block1 = new TEntryListBlock(*block2);
                  fBlocks->Add(block1);
                  fN+=block1->GetNPassed();
                  fNBlocks++;
               }
            }
            fLastIndexQueried = -1;
            fLastIndexReturned = 0;
         } else {
            //entry lists are for different trees. create a chain entry list with
            //2 sub lists for the first and second entry lists
            fLastIndexQueried = -1;
            fLastIndexReturned = 0;
            fLists = new TList();
            TEntryList *el = new TEntryList();
            el->fTreeName = fTreeName;
            el->fFileName = fFileName;
            el->fBlocks = fBlocks;
            fBlocks = 0;
            el->fNBlocks = fNBlocks;
            el->fN = fN;
            el->fLastIndexQueried = -1;
            el->fLastIndexReturned = 0;
            fLists->Add(el);
            el = new TEntryList(*elist);
            el->fLastIndexQueried = -1;
            el->fLastIndexReturned = 0;
            fLists->Add(el);
            fN+=el->GetN();
            fCurrent = 0;
         }
      } else {
         //second list already has sublists. add one by one
         TEntryList *el = 0;
         TIter next(elist->fLists);
         while ((el = (TEntryList*)next())){
            Add(el);
         }
         fCurrent = 0;
      }
   } else {
      //there are already some sublists in this list, just add another one
      if (!elist->fLists){
         //the other list doesn't have sublists
         TIter next(fLists);
         TEntryList *el = 0; 
         Bool_t found = kFALSE;
         while ((el = (TEntryList*)next())){
            if (!strcmp(el->fTreeName.Data(), elist->fTreeName.Data()) && 
                !strcmp(el->fFileName.Data(), elist->fFileName.Data())){
            // if (el->fStringHash == elist->fStringHash){
               //found a list for the same tree
               Long64_t oldn = el->GetN();
               el->Add(elist);
               found = kTRUE;
               fN = fN - oldn + el->GetN();
               break;
            }
         }
         if (!found){       
            el = new TEntryList(*elist);
            el->fLastIndexQueried = -1;
            el->fLastIndexReturned = 0;
            fLists->Add(el);
            fN+=el->GetN();
         }
      } else {
         //add all sublists from the other list
         TEntryList *el = 0;
         TIter next(elist->fLists);
         while ((el = (TEntryList*)next())){
            Add(el);
         }
         fCurrent = 0;
      }
      if (fCurrent){
         if (fCurrent->fBlocks){
            Int_t currentblock = (fCurrent->fLastIndexReturned)/kBlockSize;
            TEntryListBlock *block = (TEntryListBlock*)fCurrent->fBlocks->UncheckedAt(currentblock);
            block->ResetIndices();
            fCurrent->fLastIndexReturned = 0;
            fCurrent->fLastIndexQueried = -1;
         }
      }
      fCurrent = 0;
   }

}


//______________________________________________________________________________
Int_t TEntryList::Contains(Long64_t entry, TTree *tree)
{
//When tree = 0, returns from the current list
//When tree != 0, finds the list, corresponding to this tree
//When tree is a chain, the entry is assumed to be global index and the local
//entry is recomputed from the treeoffset information of the chain

   if (!tree){
      if (fBlocks) {
         //this entry list doesn't contain any sub-lists
         TEntryListBlock *block = 0;
         Int_t nblock = entry/kBlockSize;
         if (nblock >= fNBlocks) return 0;
         block = (TEntryListBlock*)fBlocks->UncheckedAt(nblock);
         return block->Contains(entry-nblock*kBlockSize);
      }
      if (fLists) {
         if (!fCurrent) fCurrent = (TEntryList*)fLists->First();
         return fCurrent->Contains(entry);
      }
      return 0;
   } else {
      Long64_t localEntry = tree->LoadTree(entry);
      SetTree(tree->GetTree());
      if (fCurrent)
         return fCurrent->Contains(localEntry);
   }
   return 0;

}

//______________________________________________________________________________
void TEntryList::DirectoryAutoAdd(TDirectory* dir)
{
   // Called by TKey and others to automatically add us to a directory when we are read from a file.
   
   SetDirectory(dir);
}

//________________________________________________________________________
Bool_t TEntryList::Enter(Long64_t entry, TTree *tree)
{
   //Add entry #entry to the list
   //When tree = 0, adds to the current list
   //When tree != 0, finds the list, corresponding to this tree
   //When tree is a chain, the entry is assumed to be global index and the local
   //entry is recomputed from the treeoffset information of the chain
   
   if (!tree){
      if (!fLists) {
         if (!fBlocks) fBlocks = new TObjArray();
         TEntryListBlock *block = 0;
         Long64_t nblock = entry/kBlockSize;
         if (nblock >= fNBlocks) {
            if (fNBlocks>0){
               block = (TEntryListBlock*)fBlocks->UncheckedAt(fNBlocks-1);
               if (!block) return 0;
               block->OptimizeStorage();
            }
            for (Int_t i=fNBlocks; i<=nblock; i++){
               block = new TEntryListBlock();
               fBlocks->Add(block);
            }
         fNBlocks = nblock+1;
         }
         block = (TEntryListBlock*)fBlocks->UncheckedAt(nblock);
         if (block->Enter(entry-nblock*kBlockSize)) {
            fN++;
            return 1;
         }
      } else {
         //the entry in the current entry list
         if (!fCurrent) fCurrent = (TEntryList*)fLists->First();
         if (fCurrent->Enter(entry)) {
            if (fLists)
               fN++;
            return 1;
         }
      }
   } else {
      Long64_t localentry = tree->LoadTree(entry);
      SetTree(tree->GetTree());
      if (fCurrent){
         if (fCurrent->Enter(localentry)) {
            if (fLists)
               fN++;
            return 1;
         }
      }
   }
   return 0;

}

//______________________________________________________________________________
Bool_t TEntryList::Remove(Long64_t entry, TTree *tree)
{
//Remove entry #entry from the list
//When tree = 0, removes from the current list
//When tree != 0, finds the list, corresponding to this tree
//When tree is a chain, the entry is assumed to be global index and the local
//entry is recomputed from the treeoffset information of the chain

   if (!tree){
      if (!fLists) {
         if (!fBlocks) return 0;
         TEntryListBlock *block = 0;
         Long64_t nblock = entry/kBlockSize;
         block = (TEntryListBlock*)fBlocks->UncheckedAt(nblock);
         if (!block) return 0;
         Long64_t blockindex = entry - nblock*kBlockSize;
         if (block->Remove(blockindex)){
            fN--;
            return 1;
         }
      } else {
         if (!fCurrent) fCurrent = (TEntryList*)fLists->First();
         if (fCurrent->Remove(entry)){
            if (fLists)
               fN--;
            return 1;
         }
      }
   } else {
      Int_t localentry = tree->LoadTree(entry);
      SetTree(tree->GetTree());
      if (fCurrent){
         if (fCurrent->Remove(localentry)) {
            if (fLists)
               fN--;
            return 1;
         }
      }
   }
   return 0;
}

//______________________________________________________________________________
Long64_t TEntryList::GetEntry(Int_t index)
{
   //return the number of the entry #index of this TEntryList in the TTree or TChain
   //See also Next().


   if (index>=fN){
      return -1;
   }
   if (index==fLastIndexQueried+1){
      //in a loop
      return Next();
   } else {
      if (fBlocks) {
         TEntryListBlock *block = 0;
         Long64_t total_passed = 0;
         Int_t i=0;
         while (total_passed<=index && i<fNBlocks){
            block=(TEntryListBlock*)fBlocks->UncheckedAt(i);
            total_passed+=block->GetNPassed();
            i++;
         }
         i--;
         total_passed-=block->GetNPassed();
         if (i!=fLastIndexReturned/kBlockSize){
            block = (TEntryListBlock*)fBlocks->UncheckedAt(fLastIndexReturned/kBlockSize);
            block->ResetIndices();
            block = (TEntryListBlock*)fBlocks->UncheckedAt(i);
         }

         Long64_t localindex = index - total_passed;
         Long64_t blockindex = block->GetEntry(localindex);
         if (blockindex < 0) return -1;
         Long64_t res = i*kBlockSize + blockindex;
         fLastIndexQueried = index;
         fLastIndexReturned = res;
         return res;
      } else {
         //find the corresponding list
         if (!fCurrent) fCurrent = (TEntryList*)fLists->First();
         TIter next(fLists);
         TEntryList *templist;
         Long64_t ntotal = 0;
         if (fCurrent){
            //reset all indices of the current list
            if (fCurrent->fBlocks){
               Int_t currentblock = (fCurrent->fLastIndexReturned)/kBlockSize;
               TEntryListBlock *block = (TEntryListBlock*)fCurrent->fBlocks->UncheckedAt(currentblock);
               block->ResetIndices();
               fCurrent->fLastIndexReturned = 0;
               fCurrent->fLastIndexQueried = -1;
            }
         }
         while ((templist = (TEntryList*)next())){
            if (!fShift){
               ntotal += templist->GetN();
            } else {
               if (templist->GetTreeNumber() >= 0)
                  ntotal += templist->GetN();
            }
            if (ntotal > index)
              break;
         }
         fCurrent = templist;
         if (!fCurrent) return -1;
         Long64_t localentry = index - (ntotal - fCurrent->GetN());
         fLastIndexQueried = index;
         fLastIndexReturned = fCurrent->GetEntry(localentry);
         return fLastIndexReturned;
      }

   }
   return -1;
}

//______________________________________________________________________________
Long64_t TEntryList::GetEntryAndTree(Int_t index, Int_t &treenum)
{
//return the index of "index"-th non-zero entry in the TTree or TChain
//and the # of the corresponding tree in the chain

//If shift is true, then when the requested entry is found in an entry list,
//for which there is no corresponding tree in the chain, this list is not
//taken into account, and entry from the next list with a tree is returned.
//Example:
//First sublist - 20 entries, second sublist - 5 entries, third sublist - 10 entries
//Second sublist doesn't correspond to any trees of the chain
//Then, when GetEntryAndTree(21, treenum, kTRUE) is called, first entry of the
//third sublist will be returned

   Long64_t result = GetEntry(index);
   if (fLists)
      treenum = fCurrent->fTreeNumber;
   else
      treenum = fTreeNumber;
   if (treenum<0) return -1;

   return result;
}

//______________________________________________________________________________
TEntryList *TEntryList::GetEntryList(const char *treename, const char *filename, Option_t *opt)
{
   //return the entry list, correspoding to treename and filename
   //By default, the filename is first tried as is, and then, if the corresponding list 
   //is not found, the filename is expanded to the absolute path, and compared again. 
   //To avoid it, use option "ne"

   if (gDebug > 1)
      Info("GetEntryList","tree: %s, file: %s",
                          (treename ? treename : "-"), (filename ? filename : "-"));

   if (!treename || !filename) return 0;
   TString option = opt;
   option.ToUpper();
   Bool_t nexp = option.Contains("NE");

   TString fn;
   TUrl u(filename, kTRUE);
   //fn = (!strcmp(u.GetProtocol(),"file")) ? u.GetFile() : filename;
   Bool_t local=kFALSE;
   if (!nexp){
      local = !strcmp(u.GetProtocol(), "file");
      if (local) fn = u.GetFile();
      else fn = u.GetUrl();
   } else {
      fn = filename;
   }

   if (!fLists){
      //there are no sublists
      if (!strcmp(treename, fTreeName.Data()) && !(strcmp(fn.Data(), fFileName.Data()))){
         return this;
      } else {
         //if the file is local, try the full name, unless "ne" option was specified
         if (!nexp && local){
            gSystem->ExpandPathName(fn);
            if (!gSystem->IsAbsoluteFileName(fn))
               gSystem->PrependPathName(gSystem->pwd(), fn);
            fn = gSystem->UnixPathName(fn);
            if (!strcmp(treename, fTreeName.Data()) && !(strcmp(fn.Data(), fFileName.Data())))
               return this;
         }
         return 0;
      }
   }

   TString stotal = treename;
   stotal.Append(fn);
   ULong_t newhash = stotal.Hash();

   TIter next(fLists);
   TEntryList *templist;
   while ((templist = (TEntryList*)next())){
      if (templist->fStringHash==0){
         stotal = templist->fTreeName + templist->fFileName;
         templist->fStringHash = stotal.Hash();
      }
      if (newhash == templist->fStringHash){
         if (!strcmp(templist->GetTreeName(), treename) && !strcmp(templist->GetFileName(), fn.Data())){
            return templist;
         }
      }
   }

   //didn't find anything for this filename, try the full name too
   if (!nexp && local){
      TString longname = fn;
      gSystem->ExpandPathName(longname);
      if (!gSystem->IsAbsoluteFileName(longname))
         gSystem->PrependPathName(gSystem->pwd(), longname);
      longname = gSystem->UnixPathName(longname);
      stotal = treename;
      stotal.Append(longname);
      newhash = stotal.Hash();
      next.Reset();
      while ((templist = (TEntryList*)next())){
         if (templist->fStringHash==0){
            stotal = templist->fTreeName + templist->fFileName;
            templist->fStringHash = stotal.Hash();
         }
         if (newhash == templist->fStringHash){
            if (!strcmp(templist->GetTreeName(), treename) && !strcmp(templist->GetFileName(), longname)){
               return templist;
            }
         }
      }
   }
   return 0;
}
      
//______________________________________________________________________________
Int_t TEntryList::Merge(TCollection *list)
{
   //Merge this list with the lists from the collection

   if (!list) return -1;
   TIter next(list);
   TEntryList *elist = 0;
   while ((elist = (TEntryList*)next())) {
      if (!elist->InheritsFrom(TEntryList::Class())) {
         Error("Add","Attempt to add object of class: %s to a %s",elist->ClassName(),this->ClassName());
         return -1;
      }
      Add(elist);
   }
   return 0;   
}

//______________________________________________________________________________
Long64_t TEntryList::Next()
{
   //return the next non-zero entry index (next after fLastIndexQueried)
   //this function is faster than GetEntry()

   Long64_t result;
   if (fN == fLastIndexQueried+1 || fN==0){
      return -1;
   }
   if (fBlocks){
      Int_t iblock = fLastIndexReturned/kBlockSize;
      TEntryListBlock *current_block = (TEntryListBlock*)fBlocks->UncheckedAt(iblock);
      result = current_block->Next();
      if (result>=0) {
         fLastIndexQueried++;
         fLastIndexReturned = result+kBlockSize*iblock;
         return fLastIndexReturned;
      }
      else {
         while (result<0 && iblock<fNBlocks-1) {
            current_block->ResetIndices();
            iblock++;
            current_block = (TEntryListBlock*)fBlocks->UncheckedAt(iblock);
            current_block->ResetIndices();
            result = current_block->Next();
         }
         if (result<0) {
            fLastIndexQueried = -1;
            fLastIndexReturned = 0;
            return -1;
         }
         fLastIndexQueried++;
         fLastIndexReturned = result+kBlockSize*iblock;

         return fLastIndexReturned;
      }
   } else {
      if (!fCurrent) {
         fCurrent = (TEntryList*)fLists->First();
         if (!fCurrent) return 0;
         if (fShift) {
            while (fCurrent->GetTreeNumber()<0) {
               fCurrent = (TEntryList*)fLists->After(fCurrent);
               if (!fCurrent) return 0;
            }
         }
      }
      result = fCurrent->Next();
      if (result>=0) {
         fLastIndexQueried++;
         fLastIndexReturned = result;
         return result;
      } else {
         if (fCurrent){
            //reset all indices of the current list
            if (fCurrent->fBlocks){
               Int_t currentblock = (fCurrent->fLastIndexReturned)/kBlockSize;
               TEntryListBlock *block = (TEntryListBlock*)fCurrent->fBlocks->UncheckedAt(currentblock);
               block->ResetIndices();
               fCurrent->fLastIndexReturned = 0;
               fCurrent->fLastIndexQueried = -1;
            }
         }

         //find the list with the next non-zero entry
         while (result<0 && fCurrent!=((TEntryList*)fLists->Last())){
            if (!fCurrent) return 0;
            fCurrent->fLastIndexQueried = -1;
            fCurrent->fLastIndexReturned = 0;
            fCurrent = (TEntryList*)fLists->After(fCurrent);
            // fCurrent is guarantee to be non-zero because it is not the 'last' 
            // element of the list.
            if (!fCurrent) return 0;
            if (!fShift)
               result = fCurrent->Next();
            else {
               if (fCurrent->GetTreeNumber() >= 0)
                  result = fCurrent->Next();
            }
         }
         fLastIndexQueried++;
         fLastIndexReturned = result;
         return result;
      }
   }
}


//______________________________________________________________________________
void TEntryList::OptimizeStorage()
{
   //Checks if the array representation is more economical and if so, switches to it

   if (fBlocks){
      TEntryListBlock *block = 0;
      for (Int_t i=0; i<fNBlocks; i++){
         block = (TEntryListBlock*)fBlocks->UncheckedAt(i);
         block->OptimizeStorage();
      }
   }
}


//______________________________________________________________________________
void TEntryList::Print(const Option_t* option) const
{
   //Print this list
   //option = "" - default - print the name of the tree and file
   //option = "all" - print all the entry numbers

   TString opt = option;
   opt.ToUpper();
   if (fBlocks) {
      printf("%s %s\n", fTreeName.Data(), fFileName.Data());
      if (opt.Contains("A")){
         TEntryListBlock* block = 0;
         for (Int_t i=0; i<fNBlocks; i++){
            block = (TEntryListBlock*)fBlocks->UncheckedAt(i);
            Int_t shift = i*kBlockSize;
            block->PrintWithShift(shift);
         }
      }
   }
   else {
      TEntryList *elist = 0;
      if (fN>0){
         TIter next(fLists);
         while((elist = (TEntryList*)next())){
            elist->Print(option);
         }
      } else {
         if (!fLists) printf("%s %s\n", fTreeName.Data(), fFileName.Data());
         else {
            TIter next(fLists);
            while ((elist = (TEntryList*)next())){
               printf("%s %s\n", elist->GetTreeName(), elist->GetFileName());
            }
         }
      }
   }
}

//______________________________________________________________________________
void TEntryList::Reset()
{
   //Reset this list

   //Maybe not delete, but just reset the number of blocks to 0????

   if (fBlocks){
      fBlocks->Delete();
      delete fBlocks;
      fBlocks = 0;
   }
   if (fLists){
      if (!((TEntryList*)fLists->First())->GetDirectory()){
         fLists->Delete();
      }
      delete fLists;
      fLists = 0;
   }
   fCurrent = 0;
   fBlocks = 0;
   fNBlocks = 0;
   fN = 0;
   fTreeName = "";
   fFileName = "";
   fStringHash = 0;
   fTreeNumber = -1;
   fLastIndexQueried = -1;
   fLastIndexReturned = 0;
   fReapply = kFALSE;
}

//______________________________________________________________________________
void TEntryList::SetDirectory(TDirectory *dir)
{
   //Add reference to directory dir. dir can be 0.

   if (fDirectory == dir) return;
   if (fDirectory) fDirectory->Remove(this);
   fDirectory = dir;
   if (fDirectory) fDirectory->Append(this);
}

//______________________________________________________________________________
void TEntryList::SetTree(const char *treename, const char *filename)
{
   //If a list for a tree with such name and filename exists, sets it as the current sublist
   //If not, creates this list and sets it as the current sublist
   //
   // ! the filename is taken as provided, no extensions to full path or url !

   TEntryList *elist = 0;

   TString stotal = treename;
   stotal.Append(filename);
   //printf("setting tree %s\n", stotal.Data());
   ULong_t newhash = stotal.Hash();
   if (fLists) {
      //find the corresponding entry list and make it current
      if (!fCurrent) fCurrent = (TEntryList*)fLists->First();
      if (fCurrent->fStringHash == 0){
         stotal = fCurrent->fTreeName + fCurrent->fFileName;
         fCurrent->fStringHash = stotal.Hash();
      }
      if (newhash == fCurrent->fStringHash){
         //this list is current
         if (!strcmp(fCurrent->fTreeName, treename) && !strcmp(fCurrent->fFileName, filename)){
            return;
         }
      } 
      TIter next(fLists);
      while ((elist = (TEntryList*)next())){
         if (newhash == elist->fStringHash){
            if (!strcmp(elist->GetTreeName(), treename) && !strcmp(elist->GetFileName(), filename)){
               //the current entry list was changed. reset the fLastIndexQueried,
               //so that Next() doesn't start with the wrong current list
               //Also, reset those indices in the previously current list
               if (fCurrent->fBlocks){
                  Int_t currentblock = (fCurrent->fLastIndexReturned)/kBlockSize;
                  TEntryListBlock *block = (TEntryListBlock*)fCurrent->fBlocks->UncheckedAt(currentblock);
                  block->ResetIndices();
                  fCurrent->fLastIndexReturned = 0;
                  fCurrent->fLastIndexQueried = -1;
               }
               fCurrent = elist;
               fLastIndexQueried = -3;
               return;
            }
         }
      }
      //didn't find an entry list for this tree, create a new one
      elist = new TEntryList("", "", treename, filename);
      if (elist->GetDirectory()) {
         //sub lists are not added to the current directory
         elist->GetDirectory()->Remove(elist);
         elist->SetDirectory(0);
      }
      fLists->Add(elist);
      fCurrent = elist;
      return;
   } else {
      if (fN==0 && fTreeName=="" && fFileName==""){
         //this is the first tree set to this list
         fTreeName = treename;
         fFileName = filename;
         stotal = fTreeName + fFileName;
         //fStringHash = stotal.Hash();
         fStringHash = newhash;
         fCurrent = this;
      } else {
         if (fStringHash == 0){
            stotal = fTreeName + fFileName;
            fStringHash = stotal.Hash();
         }
         if (newhash != fStringHash){
            //we have a chain and already have an entry list for the first tree
            //move the first entry list to the fLists
            fLists = new TList();
            elist = new TEntryList();
            elist->fTreeName = fTreeName;
            elist->fFileName = fFileName;
            elist->fStringHash = fStringHash;
            elist->fN = fN;
            elist->fTreeNumber = fTreeNumber;
            elist->fBlocks = fBlocks;
            fBlocks = 0;
            elist->fNBlocks = fNBlocks;
            fLists->Add(elist);
            elist = new TEntryList("", "", treename, filename);
            if (elist->GetDirectory()) {
               //sub lists are not added to the current directory
               elist->GetDirectory()->Remove(elist);
               elist->SetDirectory(0);
            }
            fLists->Add(elist);
            fCurrent = elist;
            //the current entry list was changed. reset the fLastIndexQueried,
            //so that Next() doesn't start with the wrong current list
            fLastIndexQueried = -3;
            
         }
         else {
            //same tree as in the current entry list, don't do anything
            return;
         }
      }
   }
}

//______________________________________________________________________________
void TEntryList::SetTree(const TTree *tree)
{
   //If a list for a tree with such name and filename exists, sets it as the current sublist
   //If not, creates this list and sets it as the current sublist
   //The name of the file, where the tree is, is taken as 
   //tree->GetTree()->GetCurrentFile()->GetName(), and then expanded either to the absolute path,
   //or to full url. If, for some reason, you want to provide 
   //the filename in a different format, use SetTree(const char *treename, const char *filename),
   //where the filename is taken "as is".

   if (!tree) return;
   TString treename = tree->GetTree()->GetName();
   TString filename;
   if (tree->GetTree()->GetCurrentFile()){
      filename = tree->GetTree()->GetCurrentFile()->GetName();
      TUrl url(filename.Data(), kTRUE);
      if (!strcmp(url.GetProtocol(), "file")){
         gSystem->ExpandPathName(filename);
         if (!gSystem->IsAbsoluteFileName(filename))
            gSystem->PrependPathName(gSystem->pwd(), filename);
         filename = gSystem->UnixPathName(filename);
      } else {
         filename = url.GetUrl();
      }
   } else {
      //memory-resident
      filename = "";
   }
   SetTree(treename, filename);

}

//______________________________________________________________________________
void TEntryList::Subtract(const TEntryList *elist)
{
   //remove all the entries of this entry list, that are contained in elist

   TEntryList *templist = 0;
   if (!fLists){
      if (!fBlocks) return;
      //check if lists are for the same tree
      if (!elist->fLists){
         //second list is also only for 1 tree
         if (!strcmp(elist->fTreeName.Data(),fTreeName.Data()) && 
             !strcmp(elist->fFileName.Data(),fFileName.Data())){
            //same tree
            Long64_t n2 = elist->GetN();
            Long64_t entry;
            for (Int_t i=0; i<n2; i++){
               entry = (const_cast<TEntryList*>(elist))->GetEntry(i);
               Remove(entry);
            }
         } else {
            //different trees
            return;
         }
      } else {
         //second list has sublists, try to find one for the same tree as this list
         TIter next1(elist->GetLists());
         templist = 0;
         Bool_t found = kFALSE;
         while ((templist = (TEntryList*)next1())){
            if (!strcmp(templist->fTreeName.Data(),fTreeName.Data()) && 
                !strcmp(templist->fFileName.Data(),fFileName.Data())){
               found = kTRUE;
               break;
            }
         }
         if (found) {
            Subtract(templist);
         }
      } 
   } else {
      //this list has sublists
      TIter next2(fLists);
      templist = 0;
      Long64_t oldn=0;
      while ((templist = (TEntryList*)next2())){
         oldn = templist->GetN();
         templist->Subtract(elist);
         fN = fN - oldn + templist->GetN();
      }
   }
   return;


}

//______________________________________________________________________________
TEntryList operator||(TEntryList &elist1, TEntryList &elist2)
{
   TEntryList eresult = elist1;
   //eresult = elist1;
   // printf("internal in operator1\n");
   eresult.Print("all");
   eresult.Add(&elist2);
   // printf("internal in operator2\n");
   eresult.Print("all");

   return eresult;
}


