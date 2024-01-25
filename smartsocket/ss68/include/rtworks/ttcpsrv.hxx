/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/ttcpsrv.hxx#1 $
 */

#ifndef _T_TCP_SERVER_HH_
#define _T_TCP_SERVER_HH_

#include <rtworks/ipc.h>
#include <rtworks/tipccw.hxx>

class T_IPCX_API TipcTcpServer : public TipcConnWrapper {

public:
  TipcTcpServer(T_INT4 port_number);
  TipcTcpServer(T_IPC_CONN connection, T_BOOL destroy_flag=TRUE);

  virtual ~TipcTcpServer();

  TipcTcpServer* Accept();

};

#endif
