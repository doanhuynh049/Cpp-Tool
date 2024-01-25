/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tmonsrv.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmonsrv.hxx>

/* ========================================================================= */
/*..TipcMonServer::TipcMonServer -- TipcMonServer constructor */
TipcMonServer::TipcMonServer(T_STR serverName)
{
  _status = T_TRUE;

  _serverName = serverName;

  if ( serverName == T_NULL ) {
    _status = T_FALSE;
  }

}


/* ========================================================================= */
/*..TipcMonServer::~TipcMonServer -- TipcMonServer destructor */
TipcMonServer::~TipcMonServer()
{
}

#if 0
// -------------------------------------------------------------
// Assignment operator & copy constructor
// -------------------------------------------------------------
TipcMonServer& TipcMonServer::operator=(const TipcMonServer& m)
{
}

TipcMonServer::TipcMonServer(const TipcMonServer& msg)
{
}
#endif

// -------------------------------------------------------------
// Polling methods
// -------------------------------------------------------------

/* ========================================================================= */
/*..TipcMonServer::GeneralPoll -- poll once for RTserver general information */
T_BOOL TipcMonServer::GeneralPoll()
{
  _status = TipcMonServerGeneralPoll(_serverName);
  return _status;
}


/* ========================================================================= */
/*..TipcMonServer::TimePoll -- poll once for RTserver time information */
T_BOOL TipcMonServer::TimePoll()
{
  _status = TipcMonServerTimePoll(_serverName);
  return _status;
}


/* ========================================================================= */
/*..TipcMonServer::BufferPoll -- poll once for RTserver message-related */
/*..  buffer information */
T_BOOL TipcMonServer::BufferPoll(T_STR connected_process_name)
{
  _status = TipcMonServerBufferPoll(_serverName, connected_process_name);
  return _status;
}

#ifndef T_SS35_HIERARCHY

/* ========================================================================= */
/*..TipcMonServer::OptionPoll -- poll once for RTserver option information */
T_BOOL TipcMonServer::OptionPoll(T_STR option_name)
{
  _status = TipcMonServerOptionPoll(_serverName, option_name);
  return _status;
}

/* ========================================================================= */
/*..TipcMonServer::MsgTrafficPoll -- poll msg traffic */
T_BOOL TipcMonServer::MsgTrafficPoll(T_STR connected_process_name)
{
  _status = TipcMonServerMsgTrafficPoll(_serverName, connected_process_name);
  return _status;
}

/* ========================================================================= */
/*.. TipcMonServer::RoutePoll -- poll once on route between servers */
T_BOOL TipcMonServer::RoutePoll(T_STR dest_server_name)
{
  _status = TipcMonServerRoutePoll(_serverName, dest_server_name);
  return _status;
}

#endif
