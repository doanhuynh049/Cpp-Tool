/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmonsrv.hxx#1 $
 */

#ifndef _T_MON_SERVER_HH_
#define _T_MON_SERVER_HH_

#include <rtworks/ipc.h>
#include <rtworks/tobj.hxx>

class T_IPCX_API TipcMonServer : public Tobj {

private:
  T_STR       _serverName;

public:
  TipcMonServer(T_STR serverName);

  //  TipcMonServer(const TipcMonServer& msg);
  //  TipcMonServer& operator=(const TipcMonServer& m);

  virtual ~TipcMonServer();

public:
  // Polling functions
  T_BOOL        GeneralPoll();
  T_BOOL        TimePoll();
  T_BOOL        BufferPoll(T_STR connected_process_name);
  T_BOOL        OptionPoll(T_STR option_name);
#ifndef T_SS35_HIERARCHY
  T_BOOL MsgTrafficPoll(T_STR connected_process_name);
  T_BOOL RoutePoll(T_STR dest_server_name);
#endif
};

#endif
