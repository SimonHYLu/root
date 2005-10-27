// @(#)root/mathcore:$Name:  $:$Id: DisplacementVector3D.h,v 1.4 2005/09/23 14:00:54 moneta Exp $
// Authors: W. Brown, M. Fischler, L. Moneta    2005  

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2005 , LCG ROOT MathLib Team and                     *
  *                      FNAL LCG ROOT MathLib Team                    *
  *                                                                    *
  *                                                                    *
  **********************************************************************/

// Header source file for class DisplacementVector3D
//
// Created by: Lorenzo Moneta  at Mon May 30 12:21:43 2005
// Major rewrite: M. FIschler  at Wed Jun  8  2005
//
// Last update: $Id: DisplacementVector3D.h,v 1.4 2005/09/23 14:00:54 moneta Exp $
//

#ifndef ROOT_Math_GenVector_DisplacementVector3D 
#define ROOT_Math_GenVector_DisplacementVector3D  1

#include "Math/GenVector/Cartesian3D.h"

#include "Math/GenVector/PositionVector3Dfwd.h"
#include "Math/GenVector/GenVectorIO.h"
#include "Math/GenVector/BitReproducible.h"

#include <cassert>

//doxygen tag
/**
   @defgroup GenVector 3D and 4D Vectors
 */


namespace ROOT {

  namespace Math {


    /**
              Class describing a generic displacement vector in 3 dimensions.
              This class is templated on the type of Coordinate system.
              One example is the XYZVector which is a vector based on
              double precision x,y,z data members by using the
              Cartesian3D<double> Coordinate system.

	      @ingroup GenVector
    */

    template <class CoordSystem>
    class DisplacementVector3D {

    public:

      typedef typename CoordSystem::Scalar Scalar;
      typedef CoordSystem CoordinateType;

      // ------ ctors ------

      /**
          Default constructor. Construct an empty object with zero values
      */
      DisplacementVector3D ( ) :   fCoordinates()  { }

      /**
         Construct from three values of type <em>Scalar</em>.
         In the case of a XYZVector the values are x,y,z
         In the case of  a polar vector they are r,theta, phi
      */
      DisplacementVector3D(Scalar a, Scalar b, Scalar c) :
        fCoordinates ( a , b,  c )  { }

     /**
          Construct from a displacement vector expressed in different
          coordinates, or using a different Scalar type
      */
      template <class T>
      explicit DisplacementVector3D( const DisplacementVector3D<T> & v) :
        fCoordinates ( v.Coordinates() ) { }

      /**
          Construct from a foreign 3D vector type, for example, Hep3Vector
          Precondition: v must implement methods x(), y() and z()
      */
      template <class ForeignVector>
      explicit DisplacementVector3D( const ForeignVector & v) :
        fCoordinates ( Cartesian3D<Scalar>( v.x(), v.y(), v.z() ) ) { }


#ifdef LATER
      /**
         construct from a generic linear algebra  vector of at least size 3
         implementing operator [].
         \par v  LAVector
         \par index0   index where coordinates starts (typically zero)
         It works for all Coordinates types,
         ( x= v[index0] for Cartesian and r=v[index0] for Polar )
      */
      template <class LAVector>
      DisplacementVector3D(const LAVector & v, size_t index0 ) {
        fCoordinates = CoordSystem ( v[index0], v[index0+1], v[index0+2] );
      }
#endif

      // compiler-generated copy ctor and dtor are fine.

      // ------ assignment ------

      /**
          Assignment operator from a displacement vector of arbitrary type
      */
      template <class OtherCoords>
      DisplacementVector3D & operator=
                        ( const DisplacementVector3D<OtherCoords> & v) {
        fCoordinates = v.Coordinates();
        return *this;
      }

      /**
         Assignment operator from a position vector
         (not necessarily efficient unless one or the other is Cartesian)
      */
      template <class OtherCoords>
      DisplacementVector3D & operator=
                        ( const PositionVector3D<OtherCoords> & rhs) {
        SetXYZ(rhs.x(), rhs.y(), rhs.z());
	return *this;
      }

      /**
          Assignment from a foreign 3D vector type, for example, Hep3Vector
          Precondition: v must implement methods x(), y() and z()
      */
      template <class ForeignVector>
      DisplacementVector3D & operator= ( const ForeignVector & v) {
        SetXYZ( v.x(),  v.y(), v.z() );
        return *this;
      }


#ifdef LATER
      /**
         assign from a generic linear algebra  vector of at least size 3
         implementing operator []. This could be also a C array
         \par v  LAVector
         \par index0   index where coordinates starts (typically zero)
         It works for all Coordinates types,
         ( x= v[index0] for Cartesian and r=v[index0] for Polar )
      */
      template <class LAVector>
      DisplacementVector3D & assignFrom(const LAVector & v, size_t index0 = 0) {
        fCoordinates = CoordSystem  ( v[index0], v[index0+1], v[index0+2] );
        return *this;
      }
#endif

      // ------ Set, Get, and access coordinate data ------

      /**
          Retrieve a copy of the coordinates object
      */
      CoordSystem Coordinates() const {
        return fCoordinates;
      }

      /**
         Set internal data based on a C-style array of 3 Scalar numbers
       */
      void SetCoordinates( const Scalar src[] )
                            { fCoordinates.SetCoordinates(src);  }

      /**
         Set internal data based on 3 Scalar numbers
       */
      void SetCoordinates( Scalar a, Scalar b, Scalar c )
                            { fCoordinates.SetCoordinates(a, b, c);  }

      /**
         Set internal data based on 3 Scalars at *begin to *end
       */
      template <class IT>
      void SetCoordinates( IT begin, IT end  )
      { IT a = begin; IT b = ++begin; IT c = ++begin;
        assert (++begin==end);
        SetCoordinates (*a,*b,*c);
      }

      /**
         get internal data into 3 Scalar numbers
       */
      void GetCoordinates( Scalar& a, Scalar& b, Scalar& c ) const
                            { fCoordinates.GetCoordinates(a, b, c);  }

      /**
         get internal data into a C-style array of 3 Scalar numbers
       */
      void GetCoordinates( Scalar dest[] ) const
                            { fCoordinates.GetCoordinates(dest);  }

      /**
         get internal data into 3 Scalars at *begin to *end
       */
      template <class IT>
      void GetCoordinates( IT begin, IT end ) const
      { IT a = begin; IT b = ++begin; IT c = ++begin;
        assert (++begin==end);
        GetCoordinates (*a,*b,*c);
      }

      /**
         set the values of the vector from the cartesian components (x,y,z)
         (if the vector is held in polar or cylindrical eta coordinates,
         then (x, y, z) are converted to that form)
       */
      void SetXYZ (Scalar x, Scalar y, Scalar z) {
            fCoordinates =  Cartesian3D<Scalar> (x,y,z);
      }

      // ------------------- Equality -----------------

      /**
        Exact equality
       */
      bool operator==(const DisplacementVector3D & rhs) const {
        return fCoordinates==rhs.fCoordinates;
      }
      bool operator!= (const DisplacementVector3D & rhs) const {
        return !(operator==(rhs));
      }

      // ------ Individual element access, in various coordinate systems ------

      /**
          Cartesian X, converting if necessary from internal coordinate system.
      */
      Scalar X() const { return fCoordinates.X(); }

      /**
          Cartesian Y, converting if necessary from internal coordinate system.
      */
      Scalar Y() const { return fCoordinates.Y(); }

      /**
          Cartesian Z, converting if necessary from internal coordinate system.
      */
      Scalar Z() const { return fCoordinates.Z(); }

      /**
          Polar R, converting if necessary from internal coordinate system.
      */
      Scalar R() const { return fCoordinates.R(); }

      /**
          Polar theta, converting if necessary from internal coordinate system.
      */
      Scalar Theta() const { return fCoordinates.Theta(); }

      /**
          Polar phi, converting if necessary from internal coordinate system.
      */
      Scalar Phi() const { return fCoordinates.Phi(); }

      /**
          Polar eta, converting if necessary from internal coordinate system.
      */
      Scalar Eta() const { return fCoordinates.Eta(); }

      /**
          Cylindrical transverse component rho
      */
      Scalar Rho() const { return fCoordinates.Rho(); }

      // ----- Other fundamental properties -----

      /**
          Magnitute squared ( r^2 in spherical coordinate)
      */
      Scalar Mag2() const { return fCoordinates.Mag2();}

      /**
         Transverse component squared (rho^2 in cylindrical coordinates.
      */
      Scalar Perp2() const { return fCoordinates.Perp2();}

      /**
         return unit vector parallel to this
      */
      DisplacementVector3D  Unit() const {
        Scalar tot = R();
        return tot == 0 ? *this : DisplacementVector3D(*this) / tot;
      }

      // ------ Setting individual elements present in coordinate system ------

      /**
         Change X - Cartesian3D coordinates only
      */
      void SetX (Scalar x) { fCoordinates.SetX(x); }

      /**
         Change Y - Cartesian3D coordinates only
      */
      void SetY (Scalar y) { fCoordinates.SetY(y); }

      /**
         Change Z - Cartesian3D coordinates only
      */
      void SetZ (Scalar z) { fCoordinates.SetZ(z); }

      /**
         Change R - Polar3D coordinates only
      */
      void SetR (Scalar r) { fCoordinates.SetR(r); }

      /**
         Change Theta - Polar3D coordinates only
      */
      void SetTheta (Scalar theta) { fCoordinates.SetTheta(theta); }

      /**
         Change Phi - Polar3D or CylindricalEta3D coordinates
      */
      void SetPhi (Scalar phi) { fCoordinates.SetPhi(phi); }

      /**
         Change Rho - CylindricalEta3D coordinates only
      */
      void SetRho (Scalar rho) { fCoordinates.SetRho(rho); }

      /**
         Change Eta - CylindricalEta3D coordinates only
      */
      void SetEta (Scalar eta) { fCoordinates.SetEta(eta); }


      // ------ Operations combining two vectors ------

      /**
          Return the scalar (dot) product of two vectors.
          It is possible to perform the product for any classes
          implementing X(), Y() and Z() member functions
      */
      template< class OtherVector >
      Scalar Dot( const  OtherVector & v) const {
        return X()*v.x() + Y()*v.y() + Z()*v.z();
      }

      /**
         Return vector (cross) product of two displacement vectors,
         as a vector in the coordinate system of this class.
      */
      template <class OtherVector>
      DisplacementVector3D Cross( const OtherVector & v) const {
        DisplacementVector3D  result;
        result.SetXYZ (  Y()*v.z() - v.y()*Z(),
                         Z()*v.x() - v.z()*X(),
                         X()*v.y() - v.x()*Y() );
        return result;
      }


      //#ifndef __CINT__

      /**
          Self Addition with a displacement vector.
      */
      template <class OtherCoords>
      DisplacementVector3D & operator+=
                        (const  DisplacementVector3D<OtherCoords> & v) {
        SetXYZ(  X() + v.X(), Y() + v.Y(), Z() + v.Z() );
        return *this;
      }

      /**
          Self Difference with a displacement vector.
      */
      template <class OtherCoords>
      DisplacementVector3D & operator-=
                        (const  DisplacementVector3D<OtherCoords> & v) {
        SetXYZ(  x() - v.x(), y() - v.y(), z() - v.z() );
        return *this;
      }

      //#endif //not CINT
#ifdef OLDCINT

      /**
          Self Addition with a displacement vector.
          Careful - if a position vector is added in this way,
          the result should be a position vector, but in CINT this
          operation will succeed and modify this displacement vector.
       */
      template<class V>
      DisplacementVector3D & operator+= (const  V & v) {
        SetXYZ(  x() + v.x(), y() + v.y(), z() + v.z() );
        return *this;
      }

      /**
          Self Difference with a displacement vector.
          Careful - if a position vector is subtracted in this way,
          in CINT this operation (which is phsyically meaningless)
          will not be caught as an error.
       */
      template<class V>
      DisplacementVector3D & operator-= (const  V & v) {
        SetXYZ(  x() - v.x(), y() - v.y(), z() - v.z() );
        return *this;
      }
      //#endif
      //#if defined(__MAKECINT__) || defined(G__DICTIONARY) 

      /**
          Addition of DisplacementVector3D vectors.
          The (coordinate system) type of the returned vector is defined to
          be identical to that of the first vector, which is passed by value.
          Careful - if a position vector is added in this way,
          the result should be a position vector, but in CINT this
          operation will return a displacement vector.

       */
      template <class V2>
      DisplacementVector3D operator+(const V2  & v2) const{
        DisplacementVector3D tmp(*this);
        return tmp += v2;
      }

      /**
          Difference between two DisplacementVector3D vectors.
          Careful - if a position vector is subtracted in this way,
          in CINT this operation (which is phsyically meaningless)
          will not be caught as an error.
      */
      template <class V2>
      DisplacementVector3D operator-(const V2  & v2) const {
        DisplacementVector3D tmp(*this);
        return tmp -= v2;
      }

#endif //  G_DICTIONARY

      /**
         multiply this vector by a scalar quantity
      */
      DisplacementVector3D & operator*= (Scalar a) {
        fCoordinates.Scale(a);
        return *this;
      }

      /**
         divide this vector by a scalar quantity
      */
      DisplacementVector3D & operator/= (Scalar a) {
        fCoordinates.Scale(1/a);
        return *this;
      }

      // The following methods (v*a and v/a) could instead be free functions.
      // They were moved into the class to solve a problem on AIX.
      /**
        Multiply a vector by a real number
      */
      DisplacementVector3D operator * ( Scalar a ) const {
        DisplacementVector3D tmp(*this);
        tmp *= a;
        return tmp;
      }

      /**
         Negative of the vector
       */
      DisplacementVector3D operator - ( ) const {
        //DisplacementVector3D tmp(*this);
        //tmp.Negate();
        return operator*( Scalar(-1) );
      }

      DisplacementVector3D operator + ( ) const {return *this;}

      /**
         Division of a vector with a real number
       */
      DisplacementVector3D operator/ (Scalar a) const {
        DisplacementVector3D tmp(*this);
        tmp /= a;
        return tmp;
      }


      // Limited backward name compatibility with CLHEP

      Scalar x()     const { return X();     }
      Scalar y()     const { return Y();     }
      Scalar z()     const { return Z();     }
      Scalar r()     const { return R();     }
      Scalar theta() const { return Theta(); }
      Scalar phi()   const { return Phi();   }
      Scalar eta()   const { return Eta();   }
      Scalar rho()   const { return Rho();   }
      Scalar mag2()  const { return Mag2();  }
      Scalar perp2() const { return Perp2(); }
      DisplacementVector3D unit() const {return Unit();}

    private:

      CoordSystem fCoordinates;

#ifdef NOT_SURE_THIS_SHOULD_BE_FORBIDDEN
      /**
         Cross product involving a position vector is inappropriate
      */
      template <class T2>
      DisplacementVector3D Cross( const PositionVector3D<T2> & ) const;
#endif

    };

// ---------- DisplacementVector3D class template ends here ------------
// ---------------------------------------------------------------------

//#ifndef __CINT__


   /**
        Addition of DisplacementVector3D vectors.
        The (coordinate system) type of the returned vector is defined to
        be identical to that of the first vector, which is passed by value
    */
    template <class CoordSystem1, class CoordSystem2>
    inline
    DisplacementVector3D<CoordSystem1>
    operator+(       DisplacementVector3D<CoordSystem1> v1,
               const DisplacementVector3D<CoordSystem2>  & v2) {
      return v1 += v2;
    }

    /**
        Difference between two DisplacementVector3D vectors.
        The (coordinate system) type of the returned vector is defined to
        be identical to that of the first vector.
    */
    template <class CoordSystem1, class CoordSystem2>
    inline
    DisplacementVector3D<CoordSystem1>
    operator-( DisplacementVector3D<CoordSystem1> v1,
               DisplacementVector3D<CoordSystem2> const & v2) {
      return v1 -= v2;
    }

    //#endif // not __CINT__

    /**
       Multiplication of a displacement vector by real number  a*v
    */
    template <class CoordSystem>
    inline
    DisplacementVector3D<CoordSystem>
    operator * ( typename DisplacementVector3D<CoordSystem>::Scalar a,
                 DisplacementVector3D<CoordSystem> v) {
      return v *= a;
      // Note - passing v by value and using operator *= may save one
      // copy relative to passing v by const ref and creating a temporary.
    }


    // v1*v2 notation for Cross product of two vectors is omitted,
    // since it is always confusing as to whether dot product is meant.



    // ------------- I/O to/from streams -------------

    template< class char_t, class traits_t, class T >
      inline
      std::basic_ostream<char_t,traits_t> &
      operator << ( std::basic_ostream<char_t,traits_t> & os
                  , DisplacementVector3D<T> const & v
                  )
    {
      if( !os )  return os;

      typename T::Scalar a, b, c;
      v.GetCoordinates(a, b, c);

      if( detail::get_manip( os, detail::bitforbit ) )  {
        detail::set_manip( os, detail::bitforbit, '\00' );
        typedef GenVector_detail::BitReproducible BR;
        BR::Output(os, a);
        BR::Output(os, b);
        BR::Output(os, c);
      }
      else  {
        os << detail::get_manip( os, detail::open  ) << a
           << detail::get_manip( os, detail::sep   ) << b
           << detail::get_manip( os, detail::sep   ) << c
           << detail::get_manip( os, detail::close );
      }

      return os;

    }  // op<< <>()


    template< class char_t, class traits_t, class T >
      inline
      std::basic_istream<char_t,traits_t> &
      operator >> ( std::basic_istream<char_t,traits_t> & is
                  , DisplacementVector3D<T> & v
                  )
    {
      if( !is )  return is;

      typename T::Scalar a, b, c;

      if( detail::get_manip( is, detail::bitforbit ) )  {
        detail::set_manip( is, detail::bitforbit, '\00' );
        typedef GenVector_detail::BitReproducible BR;
        BR::Input(is, a);
        BR::Input(is, b);
        BR::Input(is, c);
      }
      else  {
        detail::require_delim( is, detail::open  );  is >> a;
        detail::require_delim( is, detail::sep   );  is >> b;
        detail::require_delim( is, detail::sep   );  is >> c;
        detail::require_delim( is, detail::close );
      }

      if( is )
        v.SetCoordinates(a, b, c);
      return is;

    }  // op>> <>()

  }  // namespace Math

}  // namespace ROOT


#endif /* ROOT_Math_GenVector_DisplacementVector3D  */

