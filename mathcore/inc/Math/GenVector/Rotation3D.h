// @(#)root/mathcore:$Name:  $:$Id: Rotation3D.h,v 1.3 2005/10/27 18:00:01 moneta Exp $
// Authors: W. Brown, M. Fischler, L. Moneta    2005  

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2005 , LCG ROOT FNAL MathLib Team                    *
  *                                                                    *
  *                                                                    *
  **********************************************************************/

// Header file for class Rotation in 3 dimensions, represented by 3x3 matrix
//
// Created by: Mark Fischler Thurs June 9  2005
//
// Last update: $Id: Rotation3D.h,v 1.3 2005/10/27 18:00:01 moneta Exp $
//
#ifndef ROOT_Math_GenVector_Rotation3D 
#define ROOT_Math_GenVector_Rotation3D  1


#include "Math/GenVector/Cartesian3D.h"
#include "Math/GenVector/DisplacementVector3D.h"
#include "Math/GenVector/PositionVector3D.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/3DConversions.h"
#include "Math/GenVector/3DDistances.h"

#include "Math/GenVector/Rotation3Dfwd.h"
#include "Math/GenVector/AxisAnglefwd.h"
#include "Math/GenVector/EulerAnglesfwd.h"
#include "Math/GenVector/Quaternionfwd.h"
#include "Math/GenVector/RotationXfwd.h"
#include "Math/GenVector/RotationYfwd.h"
#include "Math/GenVector/RotationZfwd.h"
#include "Math/GenVector/AxisAnglefwd.h"

#include <algorithm>
#include <cassert>
#include <iostream>


namespace ROOT {
namespace Math {


  /**
     Rotation class with the (3D) rotation represented by
     a 3x3 orthogonal matrix.
     This is the optimal representation for application to vectors.
     See also AxisAngle, EulerAngles, and Quaternion for classes
     which have conversion operators to Rotation3D.

     @ingroup GenVector
  */

class Rotation3D {

public:

  typedef double Scalar;

  enum Rotation3DMatrixIndex {
      XX = 0, XY = 1, XZ = 2
    , YX = 3, YY = 4, YZ = 5
    , ZX = 6, ZY = 7, ZZ = 8
    };

  // ========== Constructors and Assignment =====================

  /**
      Default constructor (identity rotation)
  */
  Rotation3D();

  /**
     Construct given a pair of pointers or iterators defining the
     beginning and end of an array of nine Scalars
   */
  template<class IT>
  Rotation3D(IT begin, IT end) { SetComponents(begin,end); }

  /**
     Construct from an AxisAngle
  */
  explicit Rotation3D( AxisAngle const   & a ) { gv_detail::convert(a, *this); }

  /**
     Construct from EulerAngles
  */
  explicit Rotation3D( EulerAngles const & e ) { gv_detail::convert(e, *this); }

  /**
     Construct from a Quaternion
  */
  explicit Rotation3D( Quaternion const  & q ) { gv_detail::convert(q, *this); }

  /**
     Construct from an axial rotation
  */
  explicit Rotation3D( RotationZ const & r ) { gv_detail::convert(r, *this); }
  explicit Rotation3D( RotationY const & r ) { gv_detail::convert(r, *this); }
  explicit Rotation3D( RotationX const & r ) { gv_detail::convert(r, *this); }

  /**
     Construct from a linear algebra matrix of size at least 3x3,
     which must support operator()(i,j) to obtain elements (0,0) thru (2,2).
     Precondition:  The matrix is assumed to be orthonormal.  NO checking
     or re-adjusting is performed.
  */
  template<class ForeignMatrix>
  explicit Rotation3D(const ForeignMatrix & m) { SetComponents(m); }

  /**
     Construct from three orthonormal vectors (which must have methods
     x(), y() and z()) which will be used as the columns of the rotation
     matrix.  The orthonormality will be checked, and values adjusted
     so that the result will always be a good rotation matrix.
  */
  template<class ForeignVector>
  Rotation3D(const ForeignVector& v1,
             const ForeignVector& v2,
             const ForeignVector& v3 ) { SetComponents(v1, v2, v3); }

  // The compiler-generated copy ctor, copy assignment, and dtor are OK.

  /**
     Raw constructor from nine Scalar components (without any checking)
  */
  Rotation3D(Scalar  xx, Scalar  xy, Scalar  xz,
             Scalar  yx, Scalar  yy, Scalar  yz,
             Scalar  zx, Scalar  zy, Scalar  zz)
 {
    SetComponents (xx, xy, xz, yx, yy, yz, zx, zy, zz);
 }

  /**
     Assign from an AxisAngle
  */
  Rotation3D &
  operator=( AxisAngle const   & a ) { return operator=(Rotation3D(a)); }

  /**
     Assign from EulerAngles
  */
  Rotation3D &
  operator=( EulerAngles const & e ) { return operator=(Rotation3D(e)); }

  /**
     Assign from a Quaternion
  */
  Rotation3D &
  operator=( Quaternion const  & q ) {return operator=(Rotation3D(q)); }

  /**
     Assign from an axial rotation
  */
  Rotation3D &
  operator=( RotationZ const & r ) { return operator=(Rotation3D(r)); }
  Rotation3D &
  operator=( RotationY const & r ) { return operator=(Rotation3D(r)); }
  Rotation3D &
  operator=( RotationX const & r ) { return operator=(Rotation3D(r)); }

  /**
     Assign from an orthonormal linear algebra matrix of size 3x3,
     which must support operator()(i,j) to obtain elements (0,0) thru (2,2).
  */
  template<class ForeignMatrix>
  Rotation3D &
  operator=(const ForeignMatrix & m) { SetComponents(m); return *this; }

  /**
     Re-adjust components to eliminate small deviations from perfect
     orthonormality.
   */
  void Rectify();

  // ======== Components ==============

  /**
     Set components from three orthonormal vectors (which must have methods
     x(), y() and z()) which will be used as the columns of the rotation
     matrix.  The orthonormality will be checked, and values adjusted
     so that the result will always be a good rotation matrix.
  */
  template<class ForeignVector>
  void
  SetComponents (const ForeignVector& v1,
                 const ForeignVector& v2,
                 const ForeignVector& v3 ) {
    fM[XX]=v1.x();  fM[XY]=v2.x();  fM[XZ]=v3.x();
    fM[YX]=v1.y();  fM[YY]=v2.y();  fM[YZ]=v3.y();
    fM[ZX]=v1.z();  fM[ZY]=v2.z();  fM[ZZ]=v3.z();
    Rectify();
  }

  /**
     Get components into three vectors which will be the (orthonormal) 
     columns of the rotation matrix.  (The vector class must have a 
     constructor from 3 Scalars.) 
  */
  template<class ForeignVector>
  void
  GetComponents ( ForeignVector& v1,
                  ForeignVector& v2,
                  ForeignVector& v3 ) {
    v1 = ForeignVector ( fM[XX], fM[XY], fM[XZ] );
    v2 = ForeignVector ( fM[YX], fM[YY], fM[YZ] );
    v3 = ForeignVector ( fM[ZX], fM[ZY], fM[ZZ] );
  }

  /**
     Set the 9 matrix components given an iterator to the start of
     the desired data, and another to the end (9 past start).
   */
  template<class IT>
  void SetComponents(IT begin, IT end) {
    assert (end==begin+9);
    std::copy ( begin, end, fM );
  }

  /**
     Get the 9 matrix components into data specified by an iterator begin
     and another to the end of the desired data (9 past start).
   */
  template<class IT>
  void GetComponents(IT begin, IT end) const {
    assert (end==begin+9);
    std::copy ( fM, fM+9, begin );
  }

  /**
     Set components from a linear algebra matrix of size at least 3x3,
     which must support operator()(i,j) to obtain elements (0,0) thru (2,2).
     Precondition:  The matrix is assumed to be orthonormal.  NO checking
     or re-adjusting is performed.
  */
  template<class ForeignMatrix>
  void
  SetComponents (const ForeignMatrix & m) {
    fM[XX]=m(0,0);  fM[XY]=m(0,1);  fM[XZ]=m(0,2);
    fM[YX]=m(1,0);  fM[YY]=m(1,1);  fM[YZ]=m(1,2);
    fM[ZX]=m(2,0);  fM[ZY]=m(2,1);  fM[ZZ]=m(2,2);
  }

  /**
     Get components into a linear algebra matrix of size at least 3x3,
     which must support operator()(i,j) for write access to elements
     (0,0) thru (2,2).
  */
  template<class ForeignMatrix>
  void
  GetComponents (ForeignMatrix & m) const {
    m(0,0)=fM[XX];  m(0,1)=fM[XY];  m(0,2)=fM[XZ];
    m(1,0)=fM[YX];  m(1,1)=fM[YY];  m(1,2)=fM[YZ];
    m(2,0)=fM[ZX];  m(2,1)=fM[ZY];  m(2,2)=fM[ZZ];
  }

  /**
     Set the components from nine scalars -- UNCHECKED for orthonormaility
   */
  void
  SetComponents (Scalar  xx, Scalar  xy, Scalar  xz,
                 Scalar  yx, Scalar  yy, Scalar  yz,
                 Scalar  zx, Scalar  zy, Scalar  zz) {
                 fM[XX]=xx;  fM[XY]=xy;  fM[XZ]=xz;
                 fM[YX]=yx;  fM[YY]=yy;  fM[YZ]=yz;
                 fM[ZX]=zx;  fM[ZY]=zy;  fM[ZZ]=zz;
  }

  /**
     Get the nine components into nine scalars
   */
  void
  GetComponents (Scalar &xx, Scalar &xy, Scalar &xz,
                 Scalar &yx, Scalar &yy, Scalar &yz,
                 Scalar &zx, Scalar &zy, Scalar &zz) const {
                 xx=fM[XX];  xy=fM[XY];  xz=fM[XZ];
                 yx=fM[YX];  yy=fM[YY];  yz=fM[YZ];
                 zx=fM[ZX];  zy=fM[ZY];  zz=fM[ZZ];
  }

  // =========== operations ==============

  /**
     Rotation operation on a cartesian vector
   */
    DisplacementVector3D< ROOT::Math::Cartesian3D<double> >
    operator() (const DisplacementVector3D< ROOT::Math::Cartesian3D<double> > & v) const;

  /**
     Rotation operation on a displacement vector in any coordinate system
   */
  template <class CoordSystem>
  DisplacementVector3D<CoordSystem>
  operator() (const DisplacementVector3D<CoordSystem> & v) const {
    DisplacementVector3D< Cartesian3D<double> > xyz(v);
    DisplacementVector3D< Cartesian3D<double> > Rxyz = operator()(xyz);
    return DisplacementVector3D<CoordSystem> ( Rxyz );
  }

  /**
     Rotation operation on a position vector in any coordinate system
   */
  template <class CoordSystem>
  PositionVector3D<CoordSystem>
  operator() (const PositionVector3D<CoordSystem> & v) const {
    DisplacementVector3D< Cartesian3D<double> > xyz(v);
    DisplacementVector3D< Cartesian3D<double> > Rxyz = operator()(xyz);
    return PositionVector3D<CoordSystem> ( Rxyz );
  }

  /**
     Rotation operation on a Lorentz vector in any spatial coordinate system
   */
  template <class CoordSystem>
  LorentzVector<CoordSystem>
  operator() (const LorentzVector<CoordSystem> & v) const {
    DisplacementVector3D< Cartesian3D<double> > xyz(v.Vect());
    xyz = operator()(xyz);
    LorentzVector< PxPyPzE4D<double> > xyzt (xyz.X(), xyz.Y(), xyz.Z(), v.E());
    return LorentzVector<CoordSystem> ( xyzt );
  }

  /**
     Rotation operation on an arbitrary vector v.
     Preconditions:  v must implement methods x(), y(), and z()
     and the arbitrary vector type must have a constructor taking (x,y,z)
   */
  template <class ForeignVector>
  ForeignVector
  operator() (const  ForeignVector & v) const {
    DisplacementVector3D< Cartesian3D<double> > xyz(v);
    DisplacementVector3D< Cartesian3D<double> > Rxyz = operator()(xyz);
    return ForeignVector ( Rxyz.X(), Rxyz.Y(), Rxyz.Z() );
  }

  /**
     Overload operator * for rotation on a vector
   */
  template <class AVector>
  inline
  AVector operator* (const AVector & v) const
  {
    return operator()(v);
  }

  /**
      Invert a rotation in place
   */
  void Invert();

  /**
      Return inverse of  a rotation
   */
  Rotation3D Inverse() const { Rotation3D t(*this); t.Invert(); return t; }

  // ========= Multi-Rotation Operations ===============

  /**
     Multiply (combine) two rotations
   */
  Rotation3D operator * (const Rotation3D  & r) const;
  Rotation3D operator * (const AxisAngle   & a) const;
  Rotation3D operator * (const EulerAngles & e) const;
  Rotation3D operator * (const Quaternion  & q) const;
  Rotation3D operator * (const RotationX  & rx) const;
  Rotation3D operator * (const RotationY  & ry) const;
  Rotation3D operator * (const RotationZ  & rz) const;

  /**
     Post-Multiply (on right) by another rotation :  T = T*R
   */
  template <class R>
  Rotation3D & operator *= (const R & r) { return *this = (*this)*r; }

  /**
     Equality/inequality operators
   */
  bool operator == (const Rotation3D & rhs) {
    if( fM[0] != rhs.fM[0] )  return false;
    if( fM[1] != rhs.fM[1] )  return false;
    if( fM[2] != rhs.fM[2] )  return false;
    if( fM[3] != rhs.fM[3] )  return false;
    if( fM[4] != rhs.fM[4] )  return false;
    if( fM[5] != rhs.fM[5] )  return false;
    if( fM[6] != rhs.fM[6] )  return false;
    if( fM[7] != rhs.fM[7] )  return false;
    if( fM[8] != rhs.fM[8] )  return false;
    return true;
  }
  bool operator != (const Rotation3D & rhs) {
    return ! operator==(rhs);
  }

private:

  Scalar fM[9];

};  // Rotation3D

// ============ Class Rotation3D ends here ============

/**
   Distance between two rotations
 */
template <class R>
inline
typename Rotation3D::Scalar
Distance ( const Rotation3D& r1, const R & r2) {return gv_detail::dist(r1,r2);}

/**
   Multiplication of an axial rotation by a Rotation3D 
 */
Rotation3D operator* (RotationX const & r1, Rotation3D const & r2);
Rotation3D operator* (RotationY const & r1, Rotation3D const & r2);
Rotation3D operator* (RotationZ const & r1, Rotation3D const & r2);

/**
   Multiplication of an axial rotation by another axial Rotation 
 */
Rotation3D operator* (RotationX const & r1, RotationY const & r2);
Rotation3D operator* (RotationX const & r1, RotationZ const & r2);

Rotation3D operator* (RotationY const & r1, RotationX const & r2);
Rotation3D operator* (RotationY const & r1, RotationZ const & r2);

Rotation3D operator* (RotationZ const & r1, RotationX const & r2);
Rotation3D operator* (RotationZ const & r1, RotationY const & r2);

/**
   Stream Output and Input
 */
  // TODO - I/O should be put in the manipulator form 

std::ostream & operator<< (std::ostream & os, const Rotation3D & r);
  
} // namespace Math
} // namespace ROOT

#endif // ROOT_Math_GenVector_Rotation3D 
