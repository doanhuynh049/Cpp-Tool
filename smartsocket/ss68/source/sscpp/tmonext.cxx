/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tmonext.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmonext.h>

namespace SmartSockets {

  /* ======================================================================= */
  /*..TipcMonExt::TipcMonExt -- TipcMonExt constructor */
  TipcMonExt::TipcMonExt(const TipcSrv &tsrv) throw (TipcException)
  {

    _srv = (T_IPC_SRV) tsrv;

    if (NULL == _srv)
      throw TipcMonExtException();
  }

  /* ======================================================================= */
  /*..TipcMonExt::~TipcMonExt -- TipcMonExt destructor */
  TipcMonExt::~TipcMonExt() throw()
  {
  }

  /* ======================================================================== */
  /*..TipcMonExt::setBool -- set a boolean field in the extension  */
  void TipcMonExt::set(const char * name, 
                       bool value)  throw (TipcMonExtException)
  {
    if (!TipcSrvMonExtSetBool(_srv, const_cast<char *> (name), value))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setBoolArray -- set a boolean array field in the extension */
  void TipcMonExt::set(const char * name, 
                       const bool *value, 
                       int4 size)  throw (TipcMonExtException)
  {
    T_BOOL* tempData = new T_BOOL[size];

    if (tempData) {
      for (int i = 0; i < size; i++) {
        tempData[i] = value[i];
      }
    }

    if (!TipcSrvMonExtSetBoolArray(_srv, const_cast<char *> (name), tempData, size)) {
      delete [] tempData;
      throw TipcMonExtException();
    }

    delete [] tempData;
  }

  /* ======================================================================== */
  /*..TipcMonExt::setStr -- set a str  field in the extension  */
  void TipcMonExt::set(const char * name, 
                       const char * value)  throw (TipcMonExtException)
  {
    if (!TipcSrvMonExtSetStr(_srv, 
                             const_cast<char *> (name), 
                             const_cast<char *>(value)))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setStrArray -- set a str array field in the extension */
  void TipcMonExt::set(const char * name, 
                       const char ** value, 
                       int4 size)  throw (TipcMonExtException)
  {

    if (!TipcSrvMonExtSetStrArray(_srv, const_cast<char *> (name), 
                                  const_cast<char **> (value), size))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setInt4 -- set a int4 field in the extension  */
  void TipcMonExt::set(const char * name, 
                       const int4 value)  throw (TipcMonExtException)
  {
    if (!TipcSrvMonExtSetInt4(_srv, 
                             const_cast<char *> (name), 
                             (value)))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setInt4Array -- set a int4 array field in the extension */
  void TipcMonExt::set(const char * name, 
                       const int4 * value, 
                       int4 size)  throw (TipcMonExtException)
  {

    if (!TipcSrvMonExtSetInt4Array(_srv, const_cast<char *> (name), 
                                  const_cast<int4 *> (value), size))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setInt2 -- set a int2 field in the extension  */
  void TipcMonExt::set(const char * name, 
                       const int2 value)  throw (TipcMonExtException)
  {
    if (!TipcSrvMonExtSetInt2(_srv, 
                             const_cast<char *> (name), 
                             (value)))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setInt2Array -- set a int2 array field in the extension */
  void TipcMonExt::set(const char * name, 
                       const int2 * value, 
                       int4 size)  throw (TipcMonExtException)
  {

    if (!TipcSrvMonExtSetInt2Array(_srv, const_cast<char *> (name), 
                                  const_cast<int2 *> (value), size))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setInt8 -- set a int8 field in the extension  */
  void TipcMonExt::set(const char * name, 
                       const int8 value)  throw (TipcMonExtException)
  {
    if (!TipcSrvMonExtSetInt8(_srv, 
                             const_cast<char *> (name), 
                             (value)))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setInt8Array -- set a int8 array field in the extension */
  void TipcMonExt::set(const char * name, 
                       const int8 * value, 
                       int4 size)  throw (TipcMonExtException)
  {

    if (!TipcSrvMonExtSetInt8Array(_srv, const_cast<char *> (name), 
                                  const_cast<int8 *> (value), size))
      throw TipcMonExtException();
  }


  /* ======================================================================== */
  /*..TipcMonExt::setReal8 -- set a real8 field in the extension  */
  void TipcMonExt::set(const char * name, 
                       const real8 value)  throw (TipcMonExtException)
  {
    if (!TipcSrvMonExtSetReal8(_srv, 
                             const_cast<char *> (name), 
                             (value)))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setReal8Array -- set a real8 array field in the extension */
  void TipcMonExt::set(const char * name, 
                       const real8 * value, 
                       int4 size)  throw (TipcMonExtException)
  {

    if (!TipcSrvMonExtSetReal8Array(_srv, const_cast<char *> (name), 
                                  const_cast<real8 *> (value), size))
      throw TipcMonExtException();
  }


  /* ======================================================================== */
  /*..TipcMonExt::setReal16 -- set a real16 field in the extension  */
  void TipcMonExt::set(const char * name, 
                       const real16 value)  throw (TipcMonExtException)
  {
    if (!TipcSrvMonExtSetReal16(_srv, 
                             const_cast<char *> (name), 
                             (value)))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setReal16Array -- set a real16 array field in the extension */
  void TipcMonExt::set(const char * name, 
                       const real16 * value, 
                       int4 size)  throw (TipcMonExtException)
  {

    if (!TipcSrvMonExtSetReal16Array(_srv, const_cast<char *> (name), 
                                  const_cast<real16 *> (value), size))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setReal4 -- set a real4 field in the extension  */
  void TipcMonExt::set(const char * name, 
                       const real4 value)  throw (TipcMonExtException)
  {
    if (!TipcSrvMonExtSetReal4(_srv, 
                             const_cast<char *> (name), 
                             (value)))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setReal4Array -- set a real4 array field in the extension */
  void TipcMonExt::set(const char * name, 
                       const real4 * value, 
                       int4 size)  throw (TipcMonExtException)
  {

    if (!TipcSrvMonExtSetReal4Array(_srv, const_cast<char *> (name), 
                                  const_cast<real4 *> (value), size))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setUtf8 -- set a Utf8 field in the extension  */
  void TipcMonExt::setUtf8(const char * name, 
                       const char * value)  throw (TipcMonExtException)
  {
    if (!TipcSrvMonExtSetUtf8(_srv, 
                             const_cast<char *> (name), 
                             const_cast<char *> (value)))
      throw TipcMonExtException();
  }

  /* ======================================================================== */
  /*..TipcMonExt::setUtf8Array -- set a utf8 array field in the extension */
  void TipcMonExt::setUtf8Array(const char * name, 
                                const char ** value, 
                                int4 size)  throw (TipcMonExtException)
  {

    if (!TipcSrvMonExtSetUtf8Array(_srv, const_cast<char *> (name), 
                                  const_cast<char **> (value), size))
      throw TipcMonExtException();
  }


  /* ======================================================================== */
  /*..TipcMonExt::set -- set a binary field in the extension */
  void TipcMonExt::set(const char * name, 
                       const void * value, 
                       int4 size)  throw (TipcMonExtException)
  {

    if (!TipcSrvMonExtSetBinary(_srv, const_cast<char *> (name), 
                                const_cast<void *> (value), size))
      throw TipcMonExtException();
  }


  /* ======================================================================== */
  /*..TipcMonExt::clear -- clear (delete) a named field in the extension */
  void TipcMonExt::clear(const char * name) throw (TipcMonExtException)
  {

    if (!TipcSrvMonExtDelete(_srv, const_cast<char *> (name)))
      throw TipcMonExtException();
  }





} //namespace SmartSockets
