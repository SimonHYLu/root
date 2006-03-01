// @(#)root/proofx:$Name:  $:$Id: TXProofMgr.cxx,v 1.4 2006/02/26 16:09:57 rdm Exp $
// Author: Gerardo Ganis  12/12/2005

/*************************************************************************
 * Copyright (C) 1995-2005, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TXProofMgr                                                           //
//                                                                      //
// The PROOF manager interacts with the PROOF server coordinator to     //
// create or destroy a PROOF session, attach to or detach from          //
// existing one, and to monitor any client activity on the cluster.     //
// At most one manager instance per server is allowed.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TList.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TProof.h"
#include "TXProofMgr.h"
#include "TXSocket.h"

ClassImp(TXProofMgr)

//______________________________________________________________________________
TXProofMgr::TXProofMgr(const char *url, Int_t dbg, const char *alias)
          : TVirtualProofMgr(url)
{
   // Create a PROOF manager for the standard (old) environment.

   // Correct URL protocol
   if (!strcmp(fUrl.GetProtocol(), TUrl("a").GetProtocol()))
      fUrl.SetProtocol("proof");
   // Check port
  if (fUrl.GetPort() == TUrl("a").GetPort()) {
      // For the time being we use 'rootd' service as default.
      // This will be changed to 'proofd' as soon as XRD will be able to
      // accept on multiple ports
      Int_t port = gSystem->GetServiceByName("rootd");
      if (port < 0) {
         if (gDebug > 0)
            Info("TXProofMgr","service 'rootd' not found by GetServiceByName"
                              ": using default IANA assigned tcp port 1094");
         port = 1094;
      } else {
         if (gDebug > 1)
            Info("TXProofMgr","port from GetServiceByName: %d", port);
      }
      fUrl.SetPort(port);
   }

   // Check and save the host FQDN ...
   TInetAddress addr = gSystem->GetHostByName(fUrl.GetHost());
   if (addr.IsValid()) {
      fUrl.SetHost(addr.GetHostName());
      if (!strcmp(fUrl.GetProtocol(),"UnNamedHost"))
         fUrl.SetHost(addr.GetHostAddress());
   }

   SetName(fUrl.GetUrl(kTRUE));
   if (alias)
      SetAlias(alias);
   else
      SetAlias(fUrl.GetHost());

   // Initialize
   if (Init(dbg) != 0) {
      // Failure: make sure the socket is deleted so that its lack of
      // validity is correctly transmitted
      SafeDelete(fSocket);
   }
}

//______________________________________________________________________________
Int_t TXProofMgr::Init(Int_t)
{
   // Do real initialization: open the connection and set the relevant
   // variables.
   // Login and authentication are dealt with at this level, if required.
   // Return 0 in case of success, 1 if the remote server is a 'proofd',
   // -1 in case of error.

   // Here we make sure that the port is explicitly specified in the URL,
   // even when it matches the default value
   TString u = fUrl.GetUrl(kTRUE);

   if (!(fSocket = new TXSocket(u,'M')) || !(fSocket->IsValid())) {
      if (!fSocket || !(fSocket->IsServProofd()))
         Error("Init", "while opening the connection to %s - exit", u.Data());
      if (fSocket && fSocket->IsServProofd())
         fServType = TVirtualProofMgr::kProofd;
      return -1;
   }

   // We add the manager itself fro correct destruction
   {  R__LOCKGUARD2(gROOTMutex);
      gROOT->GetListOfSockets()->Remove(fSocket);
   }

   // We are done
   return 0;
}

//______________________________________________________________________________
TXProofMgr::~TXProofMgr()
{
   // Destructor: close the connection

   if (fSocket)
      fSocket->Close("P");
   SafeDelete(fSocket);
}

//______________________________________________________________________________
TVirtualProof *TXProofMgr::AttachSession(Int_t id, Bool_t gui)
{
   // Dummy version provided for completeness. Just returns a pointer to
   // existing session 'id' (as shown by TProof::QuerySessions) or 0 if 'id' is
   // not valid. The boolena 'gui' should be kTRUE when invoked from the GUI.

   TVirtualProofDesc *d = GetProofDesc(id);
   if (d) {
      if (d->GetProof())
         // Nothing to do if already in contact with proofserv
         return d->GetProof();

      // Re-compose url
      TString u(Form("%s/?%d", fUrl.GetUrl(kTRUE), d->GetRemoteId()));

      // We need this to set correctly the kUsingSessionGui bit before the first
      // feedback messages arrive
      if (gui)
         u += "GUI";

      // Attach
      TVirtualProof *p = new TProof(u);
      if (p && p->IsValid()) {

         // Set reference manager
         p->SetManager(this);

         // Save record about this session
         d->SetIdle(p->IsIdle());
         d->SetProof(p);

         // Set session tag
         p->SetName(d->GetName());

      } else {
         // Session creation failed
         Error("AttachSession", "attaching to PROOF session");
      }
      return p;
   }

   Info("AttachSession","invalid proofserv id (%d)", id);
   return 0;
}

//______________________________________________________________________________
void TXProofMgr::DetachSession(Int_t id, Option_t *opt)
{
   // Detach session with 'id' from its proofserv. The 'id' is the number
   // shown by QuerySessions.

   if (id > 0) {
      // Single session request
      TVirtualProofDesc *d = GetProofDesc(id);
      if (d) {
         if (fSocket)
            fSocket->DisconnectSession(d->GetRemoteId(), opt);
         fSessions->Remove(d);
         delete d;
      }
   } else if (id == 0) {

      // Requesto to destroy all sessions
      if (fSocket) {
         TString o = Form("%sA",opt);
         fSocket->DisconnectSession(-1, o);
      }
      if (fSessions)
         fSessions->Delete();
   }

   return;
}

//______________________________________________________________________________
Bool_t TXProofMgr::MatchUrl(const char *url)
{
   // Checks if 'url' refers to the same 'user@host:port' entity as the URL
   // in memory. TVirtualProofMgr::MatchUrl cannot be used here because of the
   // 'double' default port, implying an additional check on teh port effectively
   // open.

   TUrl u(url);

   // Correct URL protocol
   if (!strcmp(u.GetProtocol(), TUrl("a").GetProtocol()))
      u.SetProtocol("proof");

   if (u.GetPort() == TUrl("a").GetPort()) {
      // Set default port
      Int_t port = gSystem->GetServiceByName("rootd");
      if (port < 0)
         port = 1094;
      u.SetPort(port);
   }

   // Get the url host FQDN ...
   TInetAddress addr = gSystem->GetHostByName(u.GetHost());
   if (addr.IsValid()) {
      u.SetHost(addr.GetHostName());
      if (!strcmp(u.GetProtocol(),"UnNamedHost"))
         u.SetHost(addr.GetHostAddress());
   }

   // Now we can check
   if (!strcmp(u.GetHost(), fUrl.GetHost()))
      if (u.GetPort() == fUrl.GetPort() ||
          u.GetPort() == fSocket->GetPort())
         if (strlen(u.GetUser()) <= 0 || !strcmp(u.GetUser(),fUrl.GetUser()))
            return kTRUE;

   // Match failed
   return kFALSE;
}

//______________________________________________________________________________
TList *TXProofMgr::QuerySessions(Option_t *opt)
{
   // Get list of sessions accessible to this manager

   if (opt && !strncasecmp(opt,"L",1))
      // Just return the existing list
      return fSessions;

   // Nothing to do if not in contact with proofserv
   if (!IsValid()) return 0;

   // Create list if not existing
   if (!fSessions) {
      fSessions = new TList();
      fSessions->SetOwner();
   }

   // Send the request
   TList *ocl = new TList;
   TObjString *os = fSocket->SendCoordinator(TXSocket::kQuerySessions);
   if (os) {
      TObjArray *oa = TString(os->GetName()).Tokenize(TString("|"));
      if (oa) {
         TVirtualProofDesc *d = 0;
         TObjString *to = (TObjString *) oa->First();
         while ((to = (TObjString *) oa->After(to))) {
            // Now parse them ...
            char al[256];
            char tg[256];
            Int_t id = -1, st = -1, nc = 0;
            sscanf(to->GetName(),"%d %s %s %d %d", &id, tg, al, &st, &nc);
            // Add to the list, if not already there
            if (!(d = (TVirtualProofDesc *) fSessions->FindObject(tg))) {
               Int_t locid = fSessions->GetSize() + 1;
               d = new TVirtualProofDesc(tg, al, GetUrl(), locid, id, st, 0);
               fSessions->Add(d);
            } else {
               // Set missing / update info
               d->SetIdle(st);
               d->SetRemoteId(id);
               d->SetTitle(al);
            }
            // Add to the list for final garbage collection
            ocl->Add(new TObjString(tg));
         }
         SafeDelete(oa);
      }
      SafeDelete(os);
   }

   // Printout and Garbage collection
   if (fSessions->GetSize() > 0) {
      TIter nxd(fSessions);
      TVirtualProofDesc *d = 0;
      while ((d = (TVirtualProofDesc *)nxd())) {
         if (ocl->FindObject(d->GetName())) {
            if (opt && !strncasecmp(opt,"S",1))
               d->Print("");
         } else {
            fSessions->Remove(d);
            SafeDelete(d);
         }
      }
   }

   // We are done
   return fSessions;
}
