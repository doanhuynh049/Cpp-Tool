/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tmon.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmon.h>

namespace SmartSockets {

  /* ======================================================================= */
  /*..TipcMon::TipcMon -- TipcMon constructor */
  TipcMon::TipcMon(const TipcSrv &tsrv) throw (TipcException)
  {

    _srv = (T_IPC_SRV) tsrv;

    if (NULL == _srv)
      throw TipcMonException();


  }

  /* ======================================================================= */
  /*..TipcMon::~TipcMon -- TipcMon destructor */
  TipcMon::~TipcMon() throw()
  {
  }

  // -------------------------------------------------------------
  // Ident functions
  // -------------------------------------------------------------

  /* ======================================================================= */
  /*..TipcMon::IdentStr -- get the monitoring identification string of */
  /*..  this process */
  const char * TipcMon::getIdentStr() const throw (TipcException)
  {
    char * result = (char *)"";  // initialize variable

    bool status = TipcMonGetIdentStr(&result);
    if (false == status)
      throw TipcMonException();

    return result;
  }

  /* ======================================================================= */
  /*..TipcMon::IdentStr -- set the monitoring identification string of */
  /*..  this process */
  void TipcMon::setIdentStr(const char * type_str) throw (TipcException)
  {
    if (!TipcMonSetIdentStr(const_cast<char *> (type_str)))
      throw TipcMonException();
  }

  // -------------------------------------------------------------
  // Watch functions
  // -------------------------------------------------------------

  /* ======================================================================= */
  /*..TipcMon::printWatch -- print all monitoring categories being watched */
  void TipcMon::printWatch(T_OUT_FUNC out_func) throw (TipcException)
  {
    if (!TipcSrvMonPrintWatch(_srv, out_func))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::projectNamesGetWatch --get whether or not this RTclient process is */
  /*..  watching project names */
  bool TipcMon::projectNamesGetWatch() const throw (TipcException)
  {
    T_BOOL result = T_FALSE;  // initialize variable

    bool status = TipcSrvMonProjectNamesGetWatch(_srv, &result);
    if (false == status)
      throw TipcMonException();

    return(result?true:false);
  }

  /* ======================================================================= */
  /*..TipcMon::projectNamesSetWatch -- start or stop watching project names    */
  void TipcMon::projectNamesSetWatch(bool watch_status) throw (TipcException)
  {
    if (!TipcSrvMonProjectNamesSetWatch(_srv, watch_status))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::clientNamesGetWatch -- get whether or not this RTclient process */
  /*..  is watching RTclient names */
  bool TipcMon::clientNamesGetWatch() const throw (TipcException)
  {
    T_BOOL result = T_FALSE;  // initialize variable

    bool status = TipcSrvMonClientNamesGetWatch(_srv, &result);
    if (false == status)
      throw TipcMonException();

    return(result?true:false);
  }

  /* ======================================================================= */
  /*..TipcMon::clientNamesSetWatch -- start or stop watching RTclient names */
  void TipcMon::clientNamesSetWatch(bool watch_status) throw (TipcException)
  {
    if (!TipcSrvMonClientNamesSetWatch(_srv, watch_status))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::serverNamesGetWatch -- get whether or not this RTclient process */
  /*..  is watching RTserver names */
  bool TipcMon::serverNamesGetWatch() const throw (TipcException)
  {
    T_BOOL result = T_FALSE;  // initialize variable

    bool status = TipcSrvMonServerNamesGetWatch(_srv, &result);
    if (false == status)
      throw TipcMonException();

    return(result?true:false);
  }

  /* ======================================================================= */
  /*..TipcMon::serverNamesSetWatch -- start or stop watching RTserver names */
  void TipcMon::serverNamesSetWatch(bool watch_status) throw (TipcException)
  {
    if (!TipcSrvMonServerNamesSetWatch(_srv, watch_status))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::subjectNamesGetWatch -- get whether or not this RTclient process */
  /*..  is watching subject names */
  bool TipcMon::subjectNamesGetWatch() const throw (TipcException)
  {
    T_BOOL result = T_FALSE;  // initialize variable

    bool status = TipcSrvMonSubjectNamesGetWatch(_srv, &result);
    if (false == status)
      throw TipcMonException();

    return(result?true:false);
  }

  /* ======================================================================= */
  /*..TipcMon::subjectNamesSetWatch -- start or stop watching subject names */
  void TipcMon::subjectNamesSetWatch(bool watch_status) throw (TipcException)
  {
    if (!TipcSrvMonSubjectNamesSetWatch(_srv, watch_status))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::subscribeGetWatch -get whether or not this RTclient process */
  /*..  is watching the RTclient processes that are receiving a subject */
  bool TipcMon::subjectSubscribeGetWatch(const char * subject_name) const
  throw (TipcException)
  {
    T_BOOL result = T_FALSE; // initialize variable

    bool status = TipcSrvMonSubjectSubscribeGetWatch(_srv,
                                                    const_cast<char *> (subject_name), &result);

    if (false == status)
      throw TipcMonException();

    return(result?true:false);
  }


  /* ======================================================================= */
  /*..TipcMon::subscribeSetWatch start or stop watching RTclient processes */
  /*..  that are receiving a subject */
  void TipcMon::subjectSubscribeSetWatch(const char * subject_name,
                                         bool watch_status) throw (TipcException)
  {
    if (!TipcSrvMonSubjectSubscribeSetWatch(_srv,
                                           const_cast<char *> (subject_name), watch_status))
      throw TipcMonException();
  }

  // -------------------------------------------------------------------
  // Poll functions
  // -------------------------------------------------------------------

  /* ======================================================================= */
  /*..TipcMon::projectNamesPoll -- poll once for project names */
  void TipcMon::projectNamesPoll() throw (TipcException)
  {
    if (!TipcSrvMonProjectNamesPoll(_srv))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::clientNamesPoll -- poll once for RTclient names */
  void TipcMon::clientNamesPoll() throw (TipcException)
  {
    if (!TipcSrvMonClientNamesPoll(_srv))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::ServerNamesPoll -- poll once for RTserver names */
  void TipcMon::serverNamesPoll() throw (TipcException)
  {
    if (!TipcSrvMonServerNamesPoll(_srv))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::subjectNamesPoll -- poll once for subject names */
  void TipcMon::subjectNamesPoll() throw (TipcException)
  {
    if (!TipcSrvMonSubjectNamesPoll(_srv))
      throw TipcMonException();
  }


  /* ======================================================================= */
  /*..TipcMon::subscribePoll -- poll once for the names of the RTclient */
  /*..  processes that are receiving a subject */
  void TipcMon::subjectSubscribePoll(const char * subject_name)
  throw (TipcException)
  {
    if (!TipcSrvMonSubjectSubscribePoll(_srv,
                                       const_cast<char *> (subject_name)))
      throw TipcMonException();
  }


  /* ======================================================================= */
  /*.. TipcMon::serverConnGetWatch -- get watch on server connection */
  bool TipcMon::serverConnGetWatch() const throw (TipcException)
  {
    T_BOOL status_return = T_FALSE;

    if (!TipcSrvMonServerConnGetWatch(_srv, &status_return))
      throw TipcMonException();

    return(status_return?true:false);
  }

  /* ======================================================================= */
  /*.. TipcMon::serverConnSetWatch -- set watch on server connections */
  void TipcMon::serverConnSetWatch(bool watch_status) throw (TipcException)
  {
    if (!TipcSrvMonServerConnSetWatch(_srv, watch_status))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*.. TipcMon::serverConnPoll -- poll once on server connections */
  void TipcMon::serverConnPoll() throw (TipcException)
  {
    if (!TipcSrvMonServerConnPoll(_srv))
      throw TipcMonException();
  }

  // CLIENT WATCH FUNCTIONS...
  /* ======================================================================= */
  /*..TipcMon::SubscribeWatch -- get whether or not this RTclient */
  /*..process is watching the subjects that an RTclient process is receiving*/
  bool TipcMon::clientSubscribeGetWatch(const char * clientName) const
  throw (TipcException)
  {
    T_BOOL result = T_FALSE;  // initialize variable

    bool status = TipcSrvMonClientSubscribeGetWatch(_srv,
                                                    const_cast<char *> (clientName), &result);

    if (false == status)
      throw TipcMonException();

    return(result?true:false);
  }


  /* ======================================================================= */
  /*..TipcMon::SubscribeWatch -- start or stop watching the subjects */
  /*.. that an RTclient process is receiving */
  void TipcMon::clientSubscribeSetWatch(const char * clientName,
                                        bool watch_status) throw (TipcException)
  {
    if (!TipcSrvMonClientSubscribeSetWatch(_srv,
                                           const_cast<char *> (clientName), watch_status))
      throw TipcMonException();
  }


  /* ======================================================================= */
  /*..TipcMon::TimeWatch -- get whether or not this RTclient process is */
  /*..  watching time information in an RTclient process */
  bool TipcMon::clientTimeGetWatch(const char * clientName) const
  throw (TipcException)
  {
    T_BOOL result = T_FALSE;  // initialize variable

    bool status = TipcSrvMonClientTimeGetWatch(_srv,
                                               const_cast<char *> (clientName), &result);

    if (false == status)
      throw TipcMonException();

    return(result?true:false);
  }

  /* ======================================================================= */
  /*..TipcMon::TimeWatch -- start or stop watching RTclient time */
  /*..  information */
  void TipcMon::clientTimeSetWatch(const char * clientName, bool watch_status)
  throw (TipcException)
  {
    if (!TipcSrvMonClientTimeSetWatch(_srv,
                                      const_cast<char *> (clientName), watch_status))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::BufferWatch -- get whether or not this RTclient process  */
  /*..  is wathcing message-related buffer information in an RTclient process */
  bool TipcMon::clientBufferGetWatch(const char * clientName) const
  throw (TipcException)
  {
    T_BOOL result = T_FALSE;  // initialize variable

    bool status = TipcSrvMonClientBufferGetWatch(_srv,
                                                 const_cast<char *> (clientName),&result);

    if (false == status)
      throw TipcMonException();

    return(result?true:false);
  }

  /* ======================================================================= */
  /*..TipcMon::BufferWatch -- start or stop watching RTclient */
  /*..  message-related buffer information */
  void TipcMon::clientBufferSetWatch(const char * clientName, bool watch_status)
  throw (TipcException)
  {
    if (!TipcSrvMonClientBufferSetWatch(_srv,
                                        const_cast<char *> (clientName), watch_status))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::cleintMsgRecvGetWatch -- get whether or not this RTclient process */
  /*..  is watching received messages in an RTclient process */
  bool TipcMon::clientMsgRecvGetWatch(const char * clientName,
                                      const char * msg_type_name) const throw (TipcException)
  {
    T_BOOL result = T_FALSE; // initialize variable

    bool status = TipcSrvMonClientMsgRecvGetWatch(_srv,
                                                  const_cast<char *> (clientName),
                                                  const_cast<char *> (msg_type_name), &result);
    if (false == status)
      throw TipcMonException();

    return(result?true:false);
  }

  /* ======================================================================= */
  /*..TipcMon::clientMsgRecvSetWatch -- start or stop watching RTclient */
  /*..  received messages */
  void TipcMon::clientMsgRecvSetWatch(const char * clientName,
                                      const char * msg_type_name,
                                      bool watch_status) throw (TipcException)
  {
    if (!TipcSrvMonClientMsgRecvSetWatch(_srv, const_cast<char *> (clientName),
                                         const_cast<char *> (msg_type_name), watch_status))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::MsgSendWatch -- get whether or not this RTclient process */
  /*..  is watching sent messages in an RTclient process */
  bool TipcMon::clientMsgSendGetWatch(const char * clientName,
                                      const char * msg_type_name) const throw (TipcException)
  {
    T_BOOL result = T_FALSE;  // initialize variable

    bool status = TipcSrvMonClientMsgSendGetWatch(_srv,
                                                  const_cast<char *> (clientName),
                                                  const_cast<char *> (msg_type_name), &result);
    if (false == status)
      throw TipcMonException();

    return(result?true:false);
  }

  /* ======================================================================= */
  /*..TipcMon::MsgSendWatch -- start or stop watching RTclient sent */
  /*..  messages */
  void TipcMon::clientMsgSendSetWatch(const char * clientName,
                                      const char * msg_type_name,
                                      bool watch_status) throw (TipcException)
  {
    if (!TipcSrvMonClientMsgSendSetWatch(_srv, const_cast<char *> (clientName),
                                         const_cast<char *> (msg_type_name), watch_status))
      throw TipcMonException();
  }

  // -------------------------------------------------------------------
  // Polling functions (Client)
  // -------------------------------------------------------------------

  /* ======================================================================= */
  /*..TipcMon::SubscribePoll - poll once for the names of the datagroups */
  /*..  that and RTclient process is receiving */
  void TipcMon::clientSubscribePoll(const char * clientName) throw (TipcException)
  {
    if (!TipcSrvMonClientSubscribePoll(_srv, const_cast<char *> (clientName)))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::GeneralPoll -- poll once for RTclient general information */
  void TipcMon::clientGeneralPoll(const char * clientName) throw (TipcException)
  {
    if (!TipcSrvMonClientGeneralPoll(_srv, const_cast<char *> (clientName)))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::TimePoll -- poll once for RTclient time information */
  void TipcMon::clientTimePoll(const char * clientName) throw (TipcException)
  {
    if (!TipcSrvMonClientTimePoll(_srv, const_cast<char *> (clientName)))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::BufferPoll -- poll once for RTclient message-related */
  /*..  buffer information */
  void TipcMon::clientBufferPoll(const char * clientName) throw (TipcException)
  {
    if (!TipcSrvMonClientBufferPoll(_srv, const_cast<char *> (clientName)))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::OptionPoll -- poll once for RTclient option information */
  void TipcMon::clientOptionPoll(const char * clientName, const char * option_name)
  throw (TipcException)
  {
    if (!TipcSrvMonClientOptionPoll(_srv, const_cast<char *> (clientName),
                                    const_cast<char *> (option_name)))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::MsgTypePoll -- poll once for RTclient message type */
  /*..  information */
  void TipcMon::clientMsgTypePoll(const char * clientName, const char * msg_type_name)
  throw (TipcException)
  {
    if (!TipcSrvMonClientMsgTypePoll(_srv, const_cast<char *> (clientName),
                                     const_cast<char *> (msg_type_name)))
      throw TipcMonException();
  }
  void TipcMon::clientMsgTypeExPoll(const char * clientName, const char * msg_type_name)
  throw (TipcException)
  {
    if (!TipcSrvMonClientMsgTypeExPoll(_srv, const_cast<char *> (clientName),
                                       const_cast<char *> (msg_type_name)))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::CbPoll -- poll once for RTclient callback information */
  void TipcMon::clientCbPoll(const char * clientName) throw (TipcException)
  {
    if (!TipcSrvMonClientCbPoll(_srv, const_cast<char *> (clientName)))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::MsgTrafficPoll -- poll once for RTclient message traffic */
  /*..  information */
  void TipcMon::clientMsgTrafficPoll(const char * clientName) throw (TipcException)
  {
    if (!TipcSrvMonClientMsgTrafficPoll(_srv, const_cast<char *> (clientName)))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::SubjectPoll -- poll once for RTclient subject information */
  void TipcMon::clientSubjectPoll(const char * clientName, const char * subject_name)
  throw (TipcException)
  {
    if (!TipcSrvMonClientSubjectPoll(_srv, const_cast<char *> (clientName),
                                     const_cast<char *> (subject_name)))
      throw TipcMonException();
  }
  void TipcMon::clientSubjectExPoll(const char * clientName, const char * subject_name)
  throw (TipcException)
  {
    if (!TipcSrvMonClientSubjectExPoll(_srv, const_cast<char *> (clientName),
                                       const_cast<char *> (subject_name)))
      throw TipcMonException();
  }


  // SERVER POLL FUNCTIONS...
  /* ======================================================================= */
  /*..TipcMon::serverGeneralPoll -- poll once for RTserver general information */
  void TipcMon::serverGeneralPoll(const char * serverName) throw (TipcException)
  {
    if (!TipcSrvMonServerGeneralPoll(_srv, const_cast<char *> (serverName)))
      throw TipcMonException();
  }


  /* ======================================================================= */
  /*..TipcMon::TimePoll -- poll once for RTserver time information */
  void TipcMon::serverTimePoll(const char * serverName) throw (TipcException)
  {
    if (!TipcSrvMonServerTimePoll(_srv, const_cast<char *> (serverName)))
      throw TipcMonException();
  }


  /* ======================================================================= */
  /*..TipcMon::serverBufferPoll -- poll once for RTserver message-related */
  /*..  buffer information */
  void TipcMon::serverBufferPoll(const char * serverName,
                                 const char * connected_process_name) throw (TipcException)
  {
    if (!TipcSrvMonServerBufferPoll(_srv,
                                    const_cast<char *> (serverName),
                                    const_cast<char *> (connected_process_name)))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::serverOptionPoll -- poll once for RTserver option information */
  void TipcMon::serverOptionPoll(const char * serverName,
                                 const char * option_name) throw (TipcException)
  {
    if (!TipcSrvMonServerOptionPoll(_srv,
                                    const_cast<char *> (serverName),
                                    const_cast<char *> (option_name)))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*..TipcMon::serverMsgTrafficPoll -- poll msg traffic */
  void TipcMon::serverMsgTrafficPoll(const char * serverName,
                                     const char * connected_process_name) throw (TipcException)
  {
    if (!TipcSrvMonServerMsgTrafficPoll(_srv,
                                        const_cast<char *> (serverName),
                                        const_cast<char *> (connected_process_name)))
      throw TipcMonException();
  }
  void TipcMon::serverMsgTrafficExPoll(const char * serverName,
                                       const char * connected_process_name) throw (TipcException)
  {
    if (!TipcSrvMonServerMsgTrafficExPoll(_srv,
                                          const_cast<char *> (serverName),
                                          const_cast<char *> (connected_process_name)))
      throw TipcMonException();
  }

  /* ======================================================================= */
  /*.. TipcMon::serverRoutePoll -- poll once on route between servers */
  void TipcMon::serverRoutePoll(const char * serverName,
                                const char * dest_server_name) throw (TipcException)
  {
    if (!TipcSrvMonServerRoutePoll(_srv,
                                   const_cast<char *> (serverName),
                                   const_cast<char *> (dest_server_name)))
      throw TipcMonException();
  }


  // new APIs implemented in SS 6.4

  /* ======================================================================= */
  /*..TipcMon::clientNamesNumPoll -- polls the number of RTclients in the  */
  /*..current project that match the value of the monitor_scope option */
  void TipcMon::clientNamesNumPoll() throw (TipcException)
  {
    if (!TipcSrvMonClientNamesNumPoll(_srv)) {
      throw TipcMonException();
    }
  }

  /* ======================================================================= */
  /*..TipcMon::clientSubscribeNumPoll -- poll the number of subjects */
  /*..subscribed to by an RTclient */
  void TipcMon::clientSubscribeNumPoll(const char * client_name)
  throw (TipcException)
  {
    if (!TipcSrvMonClientSubscribeNumPoll(_srv,
                                          const_cast<char *> (client_name))) {
      throw TipcMonException();
    }
  }

  /* ======================================================================= */
  /*..TipcMon::clientCpuPoll -- poll RTclient's CPU utilization*/
  void TipcMon::clientCpuPoll(const char * client_name) throw (TipcException)
  {
    if (!TipcSrvMonClientCpuPoll(_srv, const_cast<char *> (client_name))) {
      throw TipcMonException();
    }
  }

  /* ======================================================================= */
  /*.. TipcMon::serverCpuPoll -- polls for the RTserver's CPU utilization */
  void TipcMon::serverCpuPoll(const char * serverName) throw (TipcException)
  {
    if (!TipcSrvMonServerCpuPoll(_srv, const_cast<char *> (serverName))) {
      throw TipcMonException();
    }
  }

  /* ======================================================================= */
  /*..TipcMon::clientInfoPoll -- polls for RTclient's information */
  void TipcMon::clientInfoPoll(const char * client_name) throw (TipcException)
  {
    if (!TipcSrvMonClientInfoPoll(_srv, const_cast<char *> (client_name))) {
      throw TipcMonException();
    }
  }

  /* ======================================================================= */
  /*..TipcMon::serverCongestionSetWatch -- methods that will be called to */
  /*..initiate and terminate the T_MT_MON_CLIENT_BUFFER_HIGH_WATER and */
  /*..T_MT_MON_CLIENT_BUFFER_LOW_WATER watches.  */
  void TipcMon::clientCongestionSetWatch(const char * client_name,
                                         int4 high_water, int4 low_water, bool watch_status)
  throw (TipcException)
  {
    if (!TipcSrvMonClientCongestionSetWatch(_srv, const_cast<char *> (client_name),
                                            high_water, low_water, watch_status)) {
      throw TipcMonException();
    }
  }


  /* ======================================================================= */
  /*.. TipcMon::serverCongestionSetWatch -- methods that will be called to */
  /*..initiate and terminate the T_MT_MON_SERVER_BUFFER_HIGH_WATER and */
  /*..T_MT_MON_SERVER_BUFFER_LOW_WATER watches. */
  void TipcMon::serverCongestionSetWatch(const char * server_name,
                                         const char * connected_process_name, int4 high_water,
                                         int4 low_water, bool watch_status) throw (TipcException)
  {
    if (!TipcSrvMonServerCongestionSetWatch(_srv, const_cast<char *> (server_name),
                                            const_cast<char *> (connected_process_name), high_water, low_water,
                                            watch_status)) {
      throw TipcMonException();
    }
  }

  /* ======================================================================= */
  /*..TipcMon::serverMaxClientLicensesSetWatch -- used to initiate and */
  /*..terminate the T_MT_MON_MAX_CLIENT_LICENSES watch */
  void TipcMon::serverMaxClientLicensesSetWatch(const char * server_name,
                                                bool watch_status) throw (TipcException)
  {
    if (!TipcSrvMonServerMaxClientLicensesSetWatch(_srv, const_cast<char *> (server_name),
                                                   watch_status)) {
      throw TipcMonException();
    }
  }

  /* ======================================================================= */
  /*..TipcMon::clientCongestionGetWatch --  methods that will be called to */
  /*..determine if a T_MT_MON_CLIENT_BUFFER_HIGH_WATER and */
  /*..T_MT_MON_CLIENT_BUFFER_LOW_WATER watch is currently active for a */
  /*..given server process. */
  bool TipcMon::clientCongestionGetWatch(const char * client_name)
  throw (TipcException)
  {
    T_BOOL result = T_FALSE;

    if (!TipcSrvMonClientCongestionGetWatch(_srv,
                                            const_cast<char *> (client_name), &result)) {
      throw TipcMonException();
    }

    return(result?true:false);
  }

  /* ======================================================================= */
  /*..TipcMon::serverBufferGetWatch --  methods that will be called to */
  /*..determine if a T_MT_MON_SERVER_BUFFER_HIGH_WATER and */
  /*..T_MT_MON_SERVER_BUFFER_LOW_WATER watch is currently active for */
  /*..a given server process. */
  bool TipcMon::serverCongestionGetWatch(const char * server_name,
                                         const char * connected_process_name) throw (TipcException)
  {
    T_BOOL result = T_FALSE;

    if (!TipcSrvMonServerCongestionGetWatch(_srv, const_cast<char *> (server_name),
                                            const_cast<char *> (connected_process_name), &result)) {
      throw TipcMonException();
    }

    return(result?true:false);
  }

  /* ======================================================================= */
  /*..TipcMon::serverMaxClientLicensesGetWatch -- determine if a */
  /*..T_MT_MON_MAX_CLIENT_LICENSES watch is currently active */
  bool TipcMon::serverMaxClientLicensesGetWatch(const char * server_name)
  throw (TipcException)
  {
    T_BOOL result = T_FALSE;

    if (!TipcSrvMonServerMaxClientLicensesGetWatch(_srv,
                                                   const_cast<char *> (server_name), &result)) {
      throw TipcMonException();
    }

    return(result?true:false);
  }

  /* ======================================================================= */
  /*..TipcMon::clientExtPoll -- polls for RTclient's extension information */
  void TipcMon::clientExtPoll(const char * client_name) throw (TipcException)
  {
    if (!TipcSrvMonClientExtPoll(_srv, const_cast<char *> (client_name))) {
      throw TipcMonException();
    }
  }

  // new APIs implemented in SS 7.0

  /* ======================================================================= */
  /*..TipcMon::serverStartTimePoll -- poll once for RTserver start time information */
  void TipcMon::serverStartTimePoll(const char * serverName) throw (TipcException)
  {
    if (!TipcSrvMonServerStartTimePoll(_srv, const_cast<char *> (serverName))) {
      throw TipcMonException();
    }
  }

  /* ======================================================================= */
  /*.. TipcMon::serverVersionPoll -- polls for the RTserver's version */
  void TipcMon::serverVersionPoll(const char * serverName) throw (TipcException)
  {
    if (!TipcSrvMonServerVersionPoll(_srv, const_cast<char *> (serverName))) {
      throw TipcMonException();
    }
  }

  /* ======================================================================= */
  /*..TipcMon::clientVersionPoll -- polls for RTclient's version */
  void TipcMon::clientVersionPoll(const char * client_name) throw (TipcException)
  {
    if (!TipcSrvMonClientVersionPoll(_srv, const_cast<char *> (client_name))) {
      throw TipcMonException();
    }
  }



} //namespace SmartSockets
