/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here...

#include "Riostream.h" 

#include "RooTFnBinding.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include "TF3.h"

ClassImp(RooTFnBinding) 

RooTFnBinding::RooTFnBinding(const char *name, const char *title, TF1* _func, const RooArgList& _list) :
  RooAbsReal(name,title), 
  olist("obs","obs",this),
  func(_func)
{ 
  olist.add(_list) ;
} 


RooTFnBinding::RooTFnBinding(const char *name, const char *title, TF1* _func, const RooArgList& _obsList, const RooArgList& paramList) :
  RooAbsReal(name,title), 
  olist("obs","obs",this),
  plist("params","params",this),
  func(_func)
{ 
  olist.add(_obsList) ;
  plist.add(paramList) ;
} 


RooTFnBinding::RooTFnBinding(const RooTFnBinding& other, const char* name) :  
  RooAbsReal(other,name), 
  olist("obs",this,other.olist),
  plist("params",this,other.plist),
  func(other.func)
{ 
} 



Double_t RooTFnBinding::evaluate() const 
{ 
  Double_t x = olist.at(0) ? ((RooAbsReal*)olist.at(0))->getVal() : 0 ;
  Double_t y = olist.at(1) ? ((RooAbsReal*)olist.at(1))->getVal() : 0 ;
  Double_t z = olist.at(2) ? ((RooAbsReal*)olist.at(2))->getVal() : 0 ;
  for (Int_t i=0 ; i<func->GetNpar() ; i++) {
    func->SetParameter(i,plist.at(i)?((RooAbsReal*)plist.at(i))->getVal() : 0) ;
  }
  return func->Eval(x,y,z) ;
} 



void RooTFnBinding::printArgs(ostream& os) const 
{
  // Print object arguments and name/address of function pointer
  os << "[ TFn={" << func->GetName() << "=" << func->GetTitle() << "} " ;    
  for (Int_t i=0 ; i<numProxies() ; i++) {
    RooAbsProxy* p = getProxy(i) ;
    if (!TString(p->name()).BeginsWith("!")) {
      p->print(os) ;
      os << " " ;
    }
  }    
  os << "]" ;  
}


namespace RooFit {

  RooAbsReal* bindFunction(TF1* func,RooAbsReal& x) {
    return new RooTFnBinding(func->GetName(),func->GetTitle(),func,x) ;
  }

  RooAbsReal* bindFunction(TF2* func,RooAbsReal& x, RooAbsReal& y) {
    return new RooTFnBinding(func->GetName(),func->GetTitle(),func,RooArgList(x,y)) ;
  }

  RooAbsReal* bindFunction(TF3* func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) {
    return new RooTFnBinding(func->GetName(),func->GetTitle(),func,RooArgList(x,y,z)) ;
  }

  RooAbsReal* bindFunction(TF1* func,RooAbsReal& x, const RooArgList& params) {
    return new RooTFnBinding(func->GetName(),func->GetTitle(),func,x,params) ;
  }

  RooAbsReal* bindFunction(TF2* func,RooAbsReal& x, RooAbsReal& y, const RooArgList& params) {
    return new RooTFnBinding(func->GetName(),func->GetTitle(),func,RooArgList(x,y),params) ;
  }

  RooAbsReal* bindFunction(TF3* func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z, const RooArgList& params) {
    return new RooTFnBinding(func->GetName(),func->GetTitle(),func,RooArgList(x,y,z),params) ;
  }

}
