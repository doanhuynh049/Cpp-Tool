/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmonclt.hxx#1 $
 */

#ifndef _T_MON_CLIENT_HH_
#define _T_MON_CLIENT_HH_

#include <rtworks/ipc.h>
#include <rtworks/tobj.hxx>

class T_IPCX_API TipcMonClient : public Tobj {

private:
  T_STR        _clientName;

public:
  TipcMonClient(T_STR clientName);

  //  TipcMonClient(const TipcMonClient& msg);
  //  TipcMonClient& operator=(const TipcMonClient& m);

  virtual ~TipcMonClient();

public:
  // Watch functions

  T_BOOL        SubscribeWatch();                    // Get it
  T_BOOL        SubscribeWatch(T_BOOL watch_status); // Set it
#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
  T_BOOL        RecvWatch();                         // Get it
  T_BOOL        RecvWatch(T_BOOL watch_status);      // Set it
#endif

  T_BOOL        TimeWatch();                         // Get it
  T_BOOL        TimeWatch(T_BOOL watch_status);      // Set it

  T_BOOL        BufferWatch();                       // Get it
  T_BOOL        BufferWatch(T_BOOL watch_status);    // Set it

  T_BOOL        MsgRecvWatch(T_STR msg_type_name);   // Get it
  T_BOOL        MsgRecvWatch(T_STR msg_type_name,
			     T_BOOL watch_status);   // Set it

  T_BOOL        MsgSendWatch(T_STR msg_type_name);   // Get it
  T_BOOL        MsgSendWatch(T_STR msg_type_name,
			     T_BOOL watch_status);   // Set it

  // Polling functions
  T_BOOL        SubscribePoll();
#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
  T_BOOL        RecvPoll();
#endif
  T_BOOL        GeneralPoll();
  T_BOOL        TimePoll();
  T_BOOL        BufferPoll();
  T_BOOL        OptionPoll(T_STR option_name);
  T_BOOL        MsgTypePoll(T_STR msg_type_name);
  T_BOOL        CbPoll();
  T_BOOL        MsgTrafficPoll();
  T_BOOL        SubjectPoll(T_STR subject_name);
#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
  T_BOOL        DgPoll(T_STR dg_name);
#endif

};
#endif



