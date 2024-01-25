/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tlocsrv.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tlocsrv.hxx>


/* ========================================================================= */
/*..TipcLocalServer::TipcLocalServer -- TipcLocalServer constructor */
TipcLocalServer::TipcLocalServer(T_STR file_name) 
#ifndef T_SS35_HIERARCHY
  : TipcConnWrapper(T_TRUE)
#endif
{
  _status = T_FALSE;

  _destroy_flag = T_TRUE;
  _connection = TipcConnCreateServerLocal(file_name);

  if (T_NULL != _connection) {
    _status = T_TRUE;
  }

}


/* ========================================================================= */
/*..TipcLocalServer::TipcLocalServer -- TipcLocalServer Constructor */
TipcLocalServer::TipcLocalServer(T_IPC_CONN connection, T_BOOL destroy_flag)
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
/*..TipcLocalServer::~TipcLocalServer -- TipcLocalServer Destructor */
TipcLocalServer::~TipcLocalServer() 
{
}


/* ========================================================================= */
/*..TipcLocalServer* TipcLocalServer -- accept a local client connection */
TipcLocalServer* TipcLocalServer::Accept()
{
  T_IPC_CONN accept_conn = TipcConnAccept(_connection);

  if (T_NULL == accept_conn) {
    _status = T_FALSE;
    return (TipcLocalServer *)T_NULL;
  }
  else {
    _status = T_TRUE;
  }

  TipcLocalServer* result = new TipcLocalServer(accept_conn);
  
  return result;
}

