/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tlocclt.hxx#1 $
 */

#ifndef _T_LOCAL_CLIENT_HH_
#define _T_LOCAL_CLIENT_HH_

#include <rtworks/ipc.h>
#include <rtworks/tipccw.hxx>

class T_IPCX_API TipcLocalClient : public TipcConnWrapper {

public:
  TipcLocalClient(T_STR file_name);
  TipcLocalClient(T_IPC_CONN connection, T_BOOL destroy_flag=TRUE);

  virtual ~TipcLocalClient();

};

#endif
