// @(#)root/mathcore:$Name:  $:$Id: BoostX.h,v 1.1 2005/11/24 14:45:50 moneta Exp $
// Authors: W. Brown, M. Fischler, L. Moneta    2005  

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2005 ROOT FNAL MathLib Team                          *
  *                                                                    *
  *                                                                    *
  **********************************************************************/

// Header file for BoostX
// 
// Created by: Mark Fischler  Mon Nov 1  2005
// 
// Last update: $Id: BoostX.h,v 1.1 2005/11/24 14:45:50 moneta Exp $
// 
#ifndef ROOT_Math_GenVector_BoostX
#define ROOT_Math_GenVector_BoostX 1

#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "Math/GenVector/DisplacementVector3D.h"
#include "Math/GenVector/Cartesian3D.h"

namespace ROOT {

  namespace Math {

  /**
     Lorentz boost class with the (4D) transformation represented internally
     by a 4x4 orthosymplectic matrix.
     Also, the 3-D rotation classes can be considered to be special Lorentz
     transformations which do not mix space and time components.

     @ingroup GenVector

  */

class BoostX {

public:

  typedef double Scalar;

  enum LorentzRotationMatrixIndex {
      LXX =  0, LXY =  1, LXZ =  2, LXT =  3
    , LYX =  4, LYY =  5, LYZ =  6, LYT =  7
    , LZX =  8, LZY =  9, LZZ = 10, LZT = 11
    , LTX = 12, LTY = 13, LTZ = 14, LTT = 15
  };

  enum BoostMatrixIndex {
      XX =  0, XY =  1, XZ =  2, XT =  3
    	     , YY =  4, YZ =  5, YT =  6
    		      , ZZ =  7, ZT =  8
    			       , TT =  9
  };

  // ========== Constructors and Assignment =====================

  /**
      Default constructor (identity transformation)
  */
  BoostX();

  /**
     Construct given a Scalar beta_x
   */
  BoostX(Scalar beta_x) { SetComponents(beta_x); }


  // The compiler-generated copy ctor, copy assignment, and dtor are OK.

  /**
     Re-adjust components to eliminate small deviations from a perfect
     orthosyplectic matrix.
   */
  void Rectify();

  // ======== Components ==============

  /**
     Set components from a Scalar beta_x
  */
  void
  SetComponents (Scalar beta_x);

  /**
     Get components into a Scalar beta_x
  */
  void
  GetComponents (Scalar& beta_x) const;


  /** 
      Retrieve the beta of the Boost 
   */ 
  Scalar Beta() const { return fBeta; }

  /** 
      Retrieve the gamma of the Boost 
   */ 
  Scalar Gamma() const { return fGamma; }

  /** 
      Set the given beta of the Boost 
   */ 
  void  SetBeta(Scalar beta) { SetComponents(beta); }
   
  /**
     The beta vector for this boost
   */
  DisplacementVector3D< ROOT::Math::Cartesian3D<double> > BetaVector() const;

  /**
     Get elements of internal 4x4 symmetric representation, into a data
     array suitable for direct use as the components of a LorentzRotation
     Note -- 16 Scalars will be written into the array; if the array is not
     that large, then this will lead to undefined behavior.
  */
  void 
  GetLorentzRotation (Scalar r[]) const; 
  
  // =========== operations ==============

  /**
     Lorentz transformation operation on a Minkowski ('Cartesian') 
     LorentzVector
  */
  LorentzVector< ROOT::Math::PxPyPzE4D<double> >
  operator() (const LorentzVector< ROOT::Math::PxPyPzE4D<double> > & v) const;
  
  /**
     Lorentz transformation operation on a LorentzVector in any 
     coordinate system
   */
  template <class CoordSystem>
  LorentzVector<CoordSystem>
  operator() (const LorentzVector<CoordSystem> & v) const {
    LorentzVector< PxPyPzE4D<double> > xyzt(v);
    LorentzVector< PxPyPzE4D<double> > Rxyzt = operator()(xyzt);
    return LorentzVector<CoordSystem> ( Rxyzt );
  }

  /**
     Lorentz transformation operation on an arbitrary 4-vector v.
     Preconditions:  v must implement methods x(), y(), z(), and t()
     and the arbitrary vector type must have a constructor taking (x,y,z,t)
   */
  template <class Foreign4Vector>
  Foreign4Vector
  operator() (const Foreign4Vector & v) const {
    LorentzVector< PxPyPzE4D<double> > xyzt(v);
    LorentzVector< PxPyPzE4D<double> > Rxyzt = operator()(xyzt);
    return Foreign4Vector ( Rxyzt.X(), Rxyzt.Y(), Rxyzt.Z(), Rxyzt.T() );
  }

  /**
     Overload operator * for rotation on a vector
   */
  template <class A4Vector>
  inline
  A4Vector operator* (const A4Vector & v) const
  {
    return operator()(v);
  }

  /**
      Invert a BoostX in place
   */
  void Invert();

  /**
      Return inverse of  a rotation
   */
  BoostX Inverse() const;

  /**
     Equality/inequality operators
   */
  bool operator == (const BoostX & rhs) {
    if( fBeta  != rhs.fBeta  )  return false;
    if( fGamma != rhs.fGamma )  return false;
    return true;
  }

  bool operator != (const BoostX & rhs) {
    return ! operator==(rhs);
  }

private:

  Scalar fBeta;
  Scalar fGamma;

};  // BoostX

// ============ Class BoostX ends here ============


/**
   Stream Output and Input
 */
  // TODO - I/O should be put in the manipulator form 

std::ostream & operator<< (std::ostream & os, const BoostX & b);


} //namespace Math
} //namespace ROOT







#endif /* ROOT_Math_GenVector_BoostX  */
