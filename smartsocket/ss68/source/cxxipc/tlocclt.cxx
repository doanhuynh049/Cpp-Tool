/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tlocclt.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tlocclt.hxx>

/* ========================================================================= */
/*..TipcLocalClient::TipcLocalClient -- TipcLocalClient constructor */
TipcLocalClient::TipcLocalClient(T_STR file_name) 
#ifndef T_SS35_HIERARCHY
  : TipcConnWrapper(T_TRUE)
#endif
{
  _status = T_FALSE;

  _destroy_flag = T_TRUE;
  _connection = TipcConnCreateClientLocal(file_name);

  if (T_NULL != _connection) {
    _status = T_TRUE;
  }
}


/* ========================================================================= */
/*..TipcLocalClient::TipcLocalClient -- TipcLocalClient constructor */
TipcLocalClient::TipcLocalClient(T_IPC_CONN connection, T_BOOL destroy_flag) 
#ifndef T_SS35_HIERARCHY
  : TipcConnWrapper(T_TRUE)
#endif
{
  _status = T_FALSE;

  _destroy_flag = destroy_flag;
  _connection = connection;

  if (T_NULL != _connection) {
    _status = T_TRUE;
  }
}

/* ========================================================================= */
/*..TipcLocalClient::~TipcLocalClient -- TipcLocalClient destructor */
TipcLocalClient::~TipcLocalClient() 
{
}
