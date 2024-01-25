/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/ttcpclt.hxx#1 $
 */

#ifndef _T_TCP_CLIENT_HH_
#define _T_TCP_CLIENT_HH_

#include <rtworks/ipc.h>
#include <rtworks/tipccw.hxx>

class T_IPCX_API TipcTcpClient : public TipcConnWrapper {

public:
  TipcTcpClient(T_STR node_name, T_INT4 port_number);
  TipcTcpClient(T_IPC_CONN connection, T_BOOL destroy_flag=TRUE);

  virtual ~TipcTcpClient();

};

#endif
