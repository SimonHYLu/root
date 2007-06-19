/* @(#)root/base:$Name:  $:$Id: LinkDef2.h,v 1.44 2007/03/05 14:24:48 rdm Exp $ */

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifdef __CINT__

#ifndef G__KCC 	 
#include "cint/lib/dll_stl/str.h" 	 
#endif 	 

#pragma extra_include "vector";
#pragma extra_include "string";
#pragma extra_include "iostream";

#pragma link C++ class vector<string>;
#pragma link C++ class vector<TString>;
#pragma create TClass string;

#include <vector>

#pragma link C++ global gTQSender;
#pragma link C++ global gTQSlotParams;

#pragma link C++ enum EAccessMode;
#pragma link C++ enum ESignals;
#pragma link C++ enum ESysConstants;
#pragma link C++ enum EFpeMask;
#pragma link C++ enum EFileModeMask;

#pragma link C++ function operator+(const TTime&,const TTime&);
#pragma link C++ function operator-(const TTime&,const TTime&);
#pragma link C++ function operator*(const TTime&,const TTime&);
#pragma link C++ function operator/(const TTime&,const TTime&);

#pragma link C++ function operator==(const TTime&,const TTime&);
#pragma link C++ function operator!=(const TTime&,const TTime&);
#pragma link C++ function operator<(const TTime&,const TTime&);
#pragma link C++ function operator<=(const TTime&,const TTime&);
#pragma link C++ function operator>(const TTime&,const TTime&);
#pragma link C++ function operator>=(const TTime&,const TTime&);

#pragma link C++ function operator==(const TMD5&,const TMD5&);
#pragma link C++ function operator!=(const TMD5&,const TMD5&);
#pragma link C++ function operator>>(TBuffer&,TMD5&);
#pragma link C++ function operator<<(TBuffer&,const TMD5&);

#pragma link C++ function operator==(const TUUID&,const TUUID&);
#pragma link C++ function operator!=(const TUUID&,const TUUID&);
#pragma link C++ function operator>>(TBuffer&,TUUID&);
#pragma link C++ function operator<<(TBuffer&,const TUUID&);

#pragma link C++ function operator==(const TRef&,const TRef&);
#pragma link C++ function operator!=(const TRef&,const TRef&);

#pragma link C++ function ConnectCINT(TQObject*,char*,char*);

#pragma link C++ function R_ISDIR(Int_t);
#pragma link C++ function R_ISCHR(Int_t);
#pragma link C++ function R_ISBLK(Int_t);
#pragma link C++ function R_ISREG(Int_t);
#pragma link C++ function R_ISLNK(Int_t);
#pragma link C++ function R_ISFIFO(Int_t);
#pragma link C++ function R_ISSOCK(Int_t);
#pragma link C++ function R_ISOFF(Int_t);

#pragma link C++ struct FileStat_t;
#pragma link C++ struct UserGroup_t;
#pragma link C++ struct SysInfo_t;
#pragma link C++ struct CpuInfo_t;
#pragma link C++ struct MemInfo_t;
#pragma link C++ struct ProcInfo_t;

#pragma link C++ class TExec+;
#pragma link C++ class TFolder+;
#pragma link C++ class TMacro+;
#pragma link C++ class TMD5+;
#pragma link C++ class TMemberInspector;
#pragma link C++ class TMessageHandler+;
#pragma link C++ class TNamed+;
#pragma link C++ class TObjString+;
#pragma link C++ class TObject-;
#pragma link C++ class TRemoteObject-;
#pragma link C++ class TPoint;
#pragma link C++ class TProcessID+;
#pragma link C++ class TProcessUUID+;
#pragma link C++ class TProcessEventTimer;
#pragma link C++ class TRef-;
#pragma link C++ class TROOT;
#pragma link C++ class TRegexp;
#pragma link C++ class TPRegexp;
#pragma link C++ class TRefCnt;
#pragma link C++ class TSignalHandler;
#pragma link C++ class TStopwatch+;
#pragma link C++ class TStorage;
#pragma link C++ class TString-!;
#pragma link C++ class TStringRef-!;
#pragma link C++ class TStringLong-;
#pragma link C++ class TSubString;
#pragma link C++ class TSysEvtHandler;
#pragma link C++ class TSystem+;
#pragma link C++ class TSystemFile+;
#pragma link C++ class TSystemDirectory+;
#pragma link C++ class TTask+;
#pragma link C++ class TTime;
#pragma link C++ class TTimer;
#pragma link C++ class TQObject-;
#pragma link C++ class TQObjSender;
#pragma link C++ class TQClass;
#pragma link C++ class TQConnection;
#pragma link C++ class TQCommand;
#pragma link C++ class TQUndoManager;
#pragma link C++ class TUUID+;
#pragma link C++ class TPluginHandler;
#pragma link C++ class TPluginManager;

#endif
