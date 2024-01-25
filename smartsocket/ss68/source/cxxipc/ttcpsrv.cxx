/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/ttcpsrv.cxx#1 $
 */

#include "includes.h"
#include <rtworks/ttcpsrv.hxx>

/* ========================================================================= */
/*..TipcTcpServer::TipcTcpServer -- TipcTcpServer constructor */
TipcTcpServer::TipcTcpServer(T_INT4 port_number)
#ifndef T_SS35_HIERARCHY
  : TipcConnWrapper(T_TRUE)
#endif
{
  _status = T_FALSE;

  _destroy_flag = T_TRUE;
  _connection = TipcConnCreateServerTcp(port_number);

  if (T_NULL != _connection) {
    _status = T_TRUE;
  }

}


/* ========================================================================= */
/*..TipcTcpServer::TipcTcpServer -- TipcTcpServer constructor */
TipcTcpServer::TipcTcpServer(T_IPC_CONN connection, T_BOOL destroy_flag)
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
/*..TipcTcpServer::~TipcTcpServer -- TipcTcpServer destructor */
TipcTcpServer::~TipcTcpServer() 
{
}


/* ========================================================================= */
/*..TipcTcpServer::Accept -- accept a tcp client connection */
TipcTcpServer* TipcTcpServer::Accept()
{
  T_IPC_CONN accept_conn = TipcConnAccept(_connection);

  if (T_NULL == accept_conn){
    _status = T_FALSE;
    return (TipcTcpServer *)T_NULL;
  }
  else {
    _status = T_TRUE;
  }

  TipcTcpServer* result = new TipcTcpServer(accept_conn);
  return result;
}
