// @(#)root/pyroot:$Name:  $:$Id: PyCallable.h,v 1.68 2005/01/28 05:45:41 brun Exp $
// Author: Wim Lavrijsen, Aug 2004

#ifndef PYROOT_PYCALLABLE_H
#define PYROOT_PYCALLABLE_H


namespace PyROOT {

/** Python callable object interface
      @author  WLAV
      @date    08/10/2004
      @version 2.0
 */

   class ObjectProxy;

   class PyCallable {
   public:
      virtual ~PyCallable() {}

   public:
      virtual PyObject* GetDocString() = 0;

   public:
      virtual PyObject* operator()( ObjectProxy* self, PyObject* args, PyObject* kwds ) = 0;
   };

} // namespace PyROOT

#endif // !PYROOT_PYCALLABLE_H
