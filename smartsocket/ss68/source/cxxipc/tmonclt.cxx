/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tmonclt.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmonclt.hxx>

/* ========================================================================= */
/*..TipcMonClient::TipcMonClient -- TipcMonClient constructor */
TipcMonClient::TipcMonClient(T_STR clientName)
{
  _status = T_TRUE;

  _clientName = clientName;

  if (clientName == T_NULL) {
    _status = T_FALSE;
  }

}

/* ========================================================================= */
/*..TipcMonClient::~TipcMonClient -- TipcMonClient destructor */
TipcMonClient::~TipcMonClient()
{
}



#if 0
// -------------------------------------------------------------
// Assignment operator & copy constructor
// -------------------------------------------------------------
TipcMonClient& TipcMonClient::operator=(const TipcMonClient& m)
{
}

TipcMonClient::TipcMonClient(const TipcMonClient& msg)
{
}
#endif


// -------------------------------------------------------------------
// Watch functions
// -------------------------------------------------------------------

/* ========================================================================= */
/*..TipcMonClient::SubscribeWatch -- get whether or not this RTclient */
/*.. process is watching the subjects that an RTclient process is receiving*/
T_BOOL TipcMonClient::SubscribeWatch()
{
  T_BOOL result = T_FALSE;  // initialize variable

  _status = TipcMonClientSubscribeGetWatch(_clientName,&result);
  return result;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMonClient::RecvWatch -- get whether or not this RTclient process is */
/*..  watching the datagroups that an RTclient process is receiving */
T_BOOL TipcMonClient::RecvWatch()
{
  return SubscribeWatch();
}
#endif

/* ========================================================================= */
/*..TipcMonClient::SubscribeWatch -- start or stop watching the subjects */
/*.. that an RTclient process is receiving */
T_BOOL TipcMonClient::SubscribeWatch(T_BOOL watch_status)
{
  _status = TipcMonClientSubscribeSetWatch(_clientName, watch_status);
  return _status;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMonClient::RecvWatch -- start or stop watching the datagroups that */
/*..  an RTclient process is receiving */
T_BOOL TipcMonClient::RecvWatch(T_BOOL watch_status)
{
  return SubscribeWatch(watch_status);
}
#endif

/* ========================================================================= */
/*..TipcMonClient::TimeWatch -- get whether or not this RTclient process is */
/*..  watching time information in an RTclient process */
T_BOOL TipcMonClient::TimeWatch()
{
  T_BOOL result = T_FALSE;  // initialize variable

  _status = TipcMonClientTimeGetWatch(_clientName,&result);
  return result;
}

/* ========================================================================= */
/*..TipcMonClient::TimeWatch -- start or stop watching RTclinet time */
/*..  information */
T_BOOL TipcMonClient::TimeWatch(T_BOOL watch_status)
{
  _status = TipcMonClientTimeSetWatch(_clientName, watch_status);
  return _status;
}

/* ========================================================================= */
/*..TipcMonClient::BufferWatch -- get whether or not this RTclient process  */
/*..  is wathcing message-related buffer information in an RTclient process */
T_BOOL TipcMonClient::BufferWatch()
{
  T_BOOL result = T_FALSE;  // initialize variable

  _status = TipcMonClientBufferGetWatch(_clientName,&result);
  return result;
}

/* ========================================================================= */
/*..TipcMonClient::BufferWatch -- start or stop watching RTclient */
/*..  message-related buffer information */
T_BOOL TipcMonClient::BufferWatch(T_BOOL watch_status)
{
  _status = TipcMonClientBufferSetWatch(_clientName, watch_status);
  return _status;
}

/* ========================================================================= */
/*..TipcMonClient::MsgRecvWatch -- get whether or not this RTclient process */
/*..  is watching received messages in an RTclient process */
T_BOOL TipcMonClient::MsgRecvWatch(T_STR msg_type_name)
{
  T_BOOL result = T_FALSE; // initialize variable

  _status = TipcMonClientMsgRecvGetWatch(_clientName,
					 msg_type_name, &result);
  return result;
}

/* ========================================================================= */
/*..TipcMonClient::MsgRecvWatch -- start or stop watching RTclient */
/*..  received messages */
T_BOOL TipcMonClient::MsgRecvWatch(T_STR msg_type_name,
				   T_BOOL watch_status)
{
  _status = TipcMonClientMsgRecvSetWatch(_clientName, 
					 msg_type_name, watch_status);
  return _status;
}

/* ========================================================================= */
/*..TipcMonClient::MsgSendWatch -- get whether or not this RTclient process */
/*..  is watching sent messages in an RTclient process */
T_BOOL TipcMonClient::MsgSendWatch(T_STR msg_type_name)
{
  T_BOOL result = T_FALSE;  // initialize variable

  _status = TipcMonClientMsgSendGetWatch(_clientName, msg_type_name,
					 &result);
  return result;
}

/* ========================================================================= */
/*..TipcMonClient::MsgSendWatch -- start or stop watching RTclient sent */
/*..  messages */
T_BOOL TipcMonClient::MsgSendWatch(T_STR msg_type_name,
				   T_BOOL watch_status)
{
  _status = TipcMonClientMsgSendSetWatch(_clientName, 
					 msg_type_name, watch_status);
  return _status;
}

// -------------------------------------------------------------------
// Polling functions
// -------------------------------------------------------------------

/* ========================================================================= */
/*..TipcMonClient::SubscribePoll - poll once for the names of the datagroups */
/*..  that and RTclient process is receiving */
T_BOOL TipcMonClient::SubscribePoll()
{
  _status = TipcMonClientSubscribePoll(_clientName);
  return _status;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMonClient::RecvPoll -- poll once for the names of the datagroups */
/*..  that and RTclient process is receiving */
T_BOOL TipcMonClient::RecvPoll()
{
  return SubscribePoll();
}
#endif

/* ========================================================================= */
/*..TipcMonClient::GeneralPoll -- poll once for RTclient general information */
T_BOOL TipcMonClient::GeneralPoll()
{
  _status = TipcMonClientGeneralPoll(_clientName);
  return _status;
}

/* ========================================================================= */
/*..TipcMonClient::TimePoll -- poll once for RTclient time information */
T_BOOL TipcMonClient::TimePoll()
{
  _status = TipcMonClientTimePoll(_clientName);
  return _status;
}

/* ========================================================================= */
/*..TipcMonClient::BufferPoll -- poll once for RTclient message-related */
/*..  buffer information */
T_BOOL TipcMonClient::BufferPoll()
{
  _status = TipcMonClientBufferPoll(_clientName);
  return _status;
}

/* ========================================================================= */
/*..TipcMonClient::OptionPoll -- poll once for RTclient option information */
T_BOOL TipcMonClient::OptionPoll(T_STR option_name)
{
  _status = TipcMonClientOptionPoll(_clientName, option_name);
  return _status;
}

/* ========================================================================= */
/*..TipcMonClient::MsgTypePoll -- poll once for RTclient message type */
/*..  information */
T_BOOL TipcMonClient::MsgTypePoll(T_STR msg_type_name)
{
  _status = TipcMonClientMsgTypePoll(_clientName, msg_type_name);
  return _status;
}

/* ========================================================================= */
/*..TipcMonClient::CbPoll -- poll once for RTclient callback information */
T_BOOL TipcMonClient::CbPoll()
{
  _status = TipcMonClientCbPoll(_clientName);
  return _status;
}

/* ========================================================================= */
/*..TipcMonClient::MsgTrafficPoll -- poll once for RTclient message traffic */
/*..  information */
T_BOOL TipcMonClient::MsgTrafficPoll()
{
  _status = TipcMonClientMsgTrafficPoll(_clientName);
  return _status;
}

/* ========================================================================= */
/*..TipcMonClient::SubjectPoll -- poll once for RTclient subject information */
T_BOOL TipcMonClient::SubjectPoll(T_STR subject_name)
{
  _status = TipcMonClientSubjectPoll(_clientName, subject_name);
  return _status;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMonClient::DgPoll -- poll once for RTclient datagroup information */
T_BOOL TipcMonClient::DgPoll(T_STR dg_name)
{
  return SubjectPoll(dg_name);
}
#endif
