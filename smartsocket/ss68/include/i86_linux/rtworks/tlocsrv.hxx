/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tlocsrv.hxx#1 $
 */

#ifndef _T_LOCAL_SERVER_HH_
#define _T_LOCAL_SERVER_HH_

#include <rtworks/ipc.h>
#include <rtworks/tipccw.hxx>

class T_IPCX_API TipcLocalServer : public TipcConnWrapper {

public:
  TipcLocalServer(T_STR file_name);
  TipcLocalServer(T_IPC_CONN connection, T_BOOL destroy_flag=TRUE);

  virtual ~TipcLocalServer();

  TipcLocalServer* Accept();

};

#endif
