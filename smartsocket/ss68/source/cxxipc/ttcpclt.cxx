/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/ttcpclt.cxx#1 $
 */

#include "includes.h"
#include <rtworks/ttcpclt.hxx>

/* ========================================================================= */
/*..TipcTcpClient::TipcTcpClient -- TipcTcpClient constructor */
TipcTcpClient::TipcTcpClient(T_STR node_name, T_INT4 port_number)
#ifndef T_SS35_HIERARCHY
  : TipcConnWrapper(T_TRUE)
#endif
{
  _status = T_FALSE;

  _destroy_flag = T_TRUE;
  _connection = TipcConnCreateClientTcp(node_name, port_number);

  if (T_NULL != _connection) {
    _status = T_TRUE;
  }

}


/* ========================================================================= */
/*..TipcTcpClient::TipcTcpClient -- TipcTcpClient constructor */
TipcTcpClient::TipcTcpClient(T_IPC_CONN connection, T_BOOL destroy_flag)
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
/*..TipcTcpClient::~TipcTcpClient -- TipcTcpClient destructor */
TipcTcpClient::~TipcTcpClient() 
{
}









