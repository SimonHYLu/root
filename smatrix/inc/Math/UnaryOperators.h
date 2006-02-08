// @(#)root/smatrix:$Name:  $:$Id: UnaryOperators.h,v 1.1 2005/11/24 16:03:42 brun Exp $
// Authors: T. Glebe, L. Moneta    2005  

#ifndef  ROOT_Math_UnaryOperators
#define  ROOT_Math_UnaryOperators
//======================================================
//
// ATTENTION: This file was automatically generated,
//            do not edit!
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
//======================================================

#include <cmath>

namespace ROOT { 

  namespace Math { 



template <class T, unsigned int D> class SVector;
template <class T, unsigned int D1, unsigned int D2, class R> class SMatrix;


//==============================================================================
// Minus
//==============================================================================
template <class T>
class Minus {
public:
  static inline T apply(const T& rhs) {
    return -(rhs);
  }
};

//==============================================================================
// operator- (Expr, unary)
//==============================================================================
template <class A, class T, unsigned int D>
inline VecExpr<UnaryOp<Minus<T>, VecExpr<A,T,D>, T>, T, D>
 operator-(const VecExpr<A,T,D>& rhs) {
  typedef UnaryOp<Minus<T>, VecExpr<A,T,D>, T> MinusUnaryOp;

  return VecExpr<MinusUnaryOp,T,D>(MinusUnaryOp(Minus<T>(),rhs));
}


//==============================================================================
// operator- (SVector, unary)
//==============================================================================
template <class T, unsigned int D>
inline VecExpr<UnaryOp<Minus<T>, SVector<T,D>, T>, T, D>
 operator-(const SVector<T,D>& rhs) {
  typedef UnaryOp<Minus<T>, SVector<T,D>, T> MinusUnaryOp;

  return VecExpr<MinusUnaryOp,T,D>(MinusUnaryOp(Minus<T>(),rhs));
}

//==============================================================================
// operator- (MatrixExpr, unary)
//==============================================================================
template <class A, class T, unsigned int D, unsigned int D2, class R>
inline Expr<UnaryOp<Minus<T>, Expr<A,T,D,D2,R>, T>, T, D, D2,R>
 operator-(const Expr<A,T,D,D2,R>& rhs) {
  typedef UnaryOp<Minus<T>, Expr<A,T,D,D2,R>, T> MinusUnaryOp;

  return Expr<MinusUnaryOp,T,D,D2,R>(MinusUnaryOp(Minus<T>(),rhs));
}


//==============================================================================
// operator- (SMatrix, unary)
//==============================================================================
template <class T, unsigned int D, unsigned int D2, class R>
inline Expr<UnaryOp<Minus<T>, SMatrix<T,D,D2,R>, T>, T, D, D2,R>
 operator-(const SMatrix<T,D,D2,R>& rhs) {
  typedef UnaryOp<Minus<T>, SMatrix<T,D,D2,R>, T> MinusUnaryOp;

  return Expr<MinusUnaryOp,T,D,D2,R>(MinusUnaryOp(Minus<T>(),rhs));
}


//==============================================================================
// Fabs
//==============================================================================
template <class T>
class Fabs {
public:
  static inline T apply(const T& rhs) {
    return std::fabs(rhs);
  }
};

//==============================================================================
// fabs (Expr, unary)
//==============================================================================
template <class A, class T, unsigned int D>
inline VecExpr<UnaryOp<Fabs<T>, VecExpr<A,T,D>, T>, T, D>
 fabs(const VecExpr<A,T,D>& rhs) {
  typedef UnaryOp<Fabs<T>, VecExpr<A,T,D>, T> FabsUnaryOp;

  return VecExpr<FabsUnaryOp,T,D>(FabsUnaryOp(Fabs<T>(),rhs));
}


//==============================================================================
// fabs (SVector, unary)
//==============================================================================
template <class T, unsigned int D>
inline VecExpr<UnaryOp<Fabs<T>, SVector<T,D>, T>, T, D>
 fabs(const SVector<T,D>& rhs) {
  typedef UnaryOp<Fabs<T>, SVector<T,D>, T> FabsUnaryOp;

  return VecExpr<FabsUnaryOp,T,D>(FabsUnaryOp(Fabs<T>(),rhs));
}

//==============================================================================
// fabs (MatrixExpr, unary)
//==============================================================================
template <class A, class T, unsigned int D, unsigned int D2, class R>
inline Expr<UnaryOp<Fabs<T>, Expr<A,T,D,D2,R>, T>, T, D, D2, R>
 fabs(const Expr<A,T,D,D2,R>& rhs) {
  typedef UnaryOp<Fabs<T>, Expr<A,T,D,D2,R>, T> FabsUnaryOp;

  return Expr<FabsUnaryOp,T,D,D2,R>(FabsUnaryOp(Fabs<T>(),rhs));
}


//==============================================================================
// fabs (SMatrix, unary)
//==============================================================================
template <class T, unsigned int D, unsigned int D2, class R>
inline Expr<UnaryOp<Fabs<T>, SMatrix<T,D,D2,R>, T>, T, D, D2, R>
 fabs(const SMatrix<T,D,D2,R>& rhs) {
  typedef UnaryOp<Fabs<T>, SMatrix<T,D,D2,R>, T> FabsUnaryOp;

  return Expr<FabsUnaryOp,T,D,D2,R>(FabsUnaryOp(Fabs<T>(),rhs));
}


//==============================================================================
// Sqr
//==============================================================================
template <class T>
class Sqr {
public:
  static inline T apply(const T& rhs) {
    return square(rhs);
  }
};

//==============================================================================
// sqr (Expr, unary)
//==============================================================================
template <class A, class T, unsigned int D>
inline VecExpr<UnaryOp<Sqr<T>, VecExpr<A,T,D>, T>, T, D>
 sqr(const VecExpr<A,T,D>& rhs) {
  typedef UnaryOp<Sqr<T>, VecExpr<A,T,D>, T> SqrUnaryOp;

  return VecExpr<SqrUnaryOp,T,D>(SqrUnaryOp(Sqr<T>(),rhs));
}


//==============================================================================
// sqr (SVector, unary)
//==============================================================================
template <class T, unsigned int D>
inline VecExpr<UnaryOp<Sqr<T>, SVector<T,D>, T>, T, D>
 sqr(const SVector<T,D>& rhs) {
  typedef UnaryOp<Sqr<T>, SVector<T,D>, T> SqrUnaryOp;

  return VecExpr<SqrUnaryOp,T,D>(SqrUnaryOp(Sqr<T>(),rhs));
}

//==============================================================================
// sqr (MatrixExpr, unary)
//==============================================================================
template <class A, class T, unsigned int D, unsigned int D2, class R>
inline Expr<UnaryOp<Sqr<T>, Expr<A,T,D,D2,R>, T>, T, D, D2, R>
 sqr(const Expr<A,T,D,D2,R>& rhs) {
  typedef UnaryOp<Sqr<T>, Expr<A,T,D,D2,R>, T> SqrUnaryOp;

  return Expr<SqrUnaryOp,T,D,D2,R>(SqrUnaryOp(Sqr<T>(),rhs));
}


//==============================================================================
// sqr (SMatrix, unary)
//==============================================================================
template <class T, unsigned int D, unsigned int D2, class R>
inline Expr<UnaryOp<Sqr<T>, SMatrix<T,D,D2,R>, T>, T, D, D2, R>
 sqr(const SMatrix<T,D,D2,R>& rhs) {
  typedef UnaryOp<Sqr<T>, SMatrix<T,D,D2,R>, T> SqrUnaryOp;

  return Expr<SqrUnaryOp,T,D,D2,R>(SqrUnaryOp(Sqr<T>(),rhs));
}


//==============================================================================
// Sqrt
//==============================================================================
template <class T>
class Sqrt {
public:
  static inline T apply(const T& rhs) {
    return std::sqrt(rhs);
  }
};

//==============================================================================
// sqrt (VecExpr, unary)
//==============================================================================
template <class A, class T, unsigned int D>
inline VecExpr<UnaryOp<Sqrt<T>, VecExpr<A,T,D>, T>, T, D>
 sqrt(const VecExpr<A,T,D>& rhs) {
  typedef UnaryOp<Sqrt<T>, VecExpr<A,T,D>, T> SqrtUnaryOp;

  return VecExpr<SqrtUnaryOp,T,D>(SqrtUnaryOp(Sqrt<T>(),rhs));
}


//==============================================================================
// sqrt (SVector, unary)
//==============================================================================
template <class T, unsigned int D>
inline VecExpr<UnaryOp<Sqrt<T>, SVector<T,D>, T>, T, D>
 sqrt(const SVector<T,D>& rhs) {
  typedef UnaryOp<Sqrt<T>, SVector<T,D>, T> SqrtUnaryOp;

  return VecExpr<SqrtUnaryOp,T,D>(SqrtUnaryOp(Sqrt<T>(),rhs));
}

//==============================================================================
// sqrt (MatrixExpr, unary)
//==============================================================================
template <class A, class T, unsigned int D, unsigned int D2, class R>
inline Expr<UnaryOp<Sqrt<T>, Expr<A,T,D,D2,R>, T>, T, D, D2, R>
 sqrt(const Expr<A,T,D,D2,R>& rhs) {
  typedef UnaryOp<Sqrt<T>, Expr<A,T,D,D2,R>, T> SqrtUnaryOp;

  return Expr<SqrtUnaryOp,T,D,D2,R>(SqrtUnaryOp(Sqrt<T>(),rhs));
}


//==============================================================================
// sqrt (SMatrix, unary)
//==============================================================================
template <class T, unsigned int D, unsigned int D2, class R>
inline Expr<UnaryOp<Sqrt<T>, SMatrix<T,D,D2,R>, T>, T, D, D2, R>
 sqrt(const SMatrix<T,D,D2,R>& rhs) {
  typedef UnaryOp<Sqrt<T>, SMatrix<T,D,D2,R>, T> SqrtUnaryOp;

  return Expr<SqrtUnaryOp,T,D,D2,R>(SqrtUnaryOp(Sqrt<T>(),rhs));
}


  }  // namespace Math

}  // namespace ROOT
          


#endif  /* ROOT_Math_UnaryOperators */ 
