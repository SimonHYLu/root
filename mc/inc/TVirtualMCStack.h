// @(#)root/mc:$Name:  $:$Id: TVirtualMCStack.h,v 1.4 2003/02/26 13:30:55 brun Exp $
// Authors: Ivana Hrivnacova 13/04/2002

#ifndef ROOT_TVirtualMCStack
#define ROOT_TVirtualMCStack

// Class TVirtualMCStack
// ---------------------
// Interface to a particles stack.
//

#include "TObject.h"
#include "TMCProcess.h"

class TParticle;

class TVirtualMCStack : public TObject {
  
public:
    // creators, destructors
    TVirtualMCStack();
    virtual ~TVirtualMCStack();

    // methods
    virtual void       SetTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  	                  Double_t px, Double_t py, Double_t pz, Double_t e,
  		          Double_t vx, Double_t vy, Double_t vz, Double_t tof,
		          Double_t polx, Double_t poly, Double_t polz,
		          TMCProcess mech, Int_t& ntr, Double_t weight,
		          Int_t is) = 0;
    virtual TParticle* GetNextTrack(Int_t& itrack)       = 0;
    virtual TParticle* GetCurrentTrack()                 = 0;    
    virtual TParticle* GetPrimaryForTracking(Int_t i)    = 0;    
    
    // set methods
    virtual void       SetCurrentTrack(Int_t track)      = 0;                           

    // get methods
    virtual Int_t      GetNtrack()    const = 0;
    virtual Int_t      GetNprimary()  const = 0;
    virtual Int_t      CurrentTrack() const = 0;
    virtual Int_t      CurrentTrackParent() const = 0;
    
  ClassDef(TVirtualMCStack,1) //Particles stack
};

#endif //ROOT_TVirtualMCStack
