/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmon.h#1 $
 */

#ifndef _T_MONITOR_HH_
#define _T_MONITOR_HH_

#include <rtworks/ipc.h>
#include <rtworks/tex.h>
#include <rtworks/tsrv.h>
#include <rtworks/tmon.h>

namespace SmartSockets {

  /**
   * Inherits from TipcException so users can trap TipcMon related
   * exceptions. Offers no additional functionality.
   * @see TipcException, Monitor
   */
  class T_IPCX_API TipcMonException : public TipcException {

  public:
    /**
     * Constructor.
     */
    TipcMonException() {}

    /**
     * Destructor.
     */
    virtual ~TipcMonException () throw() {}

  };


  /**
   * Enables users to send monitoring requests to a specified
   * RTserver connection.  RTclient processes have monitoring capabilities
   * to gather information about your project, as well as determining where
   * processes are located in your network.
   *
   * From within an RTclient, hundreds of pieces of information can be gathered
   * in real time about all parts of a running SmartSockets project. These are
   * some of the kinds of information available:
   * - Information about RTclients:\n
   *   - names\n
   *   - message buffers\n
   *   - message traffic statistics\n
   *   - message types\n
   *   - options\n
   *   - CPU usage\n
   *   - memory usage\n
   *   - node the RTClient is on\n
   *   - options\n
   *   - current time\n
   *   - subjects the RTClient is subscribed to\n
   * - Information about RTservers:\n
   *   - names \n
   *   - message buffers \n
   *   - message traffic statistics \n
   *   - connections \n
   *   - options \n
   *   - CPU usage \n
   *   - memory usage \n
   *   - node the RTServer is on \n
   *   - options \n
   *   - current time \n
   * - Information about Subjects: \n
   *   - names \n
   *   - RTclients that are subscribing \n
   * - Information about Projects: \n
   *   - names \n
   *
   * This information can be either polled once to provide a one-time snapshot
   * of information or watched to provide asynchronous updates when changes
   * occur. A monitoring request can specify either a specific object or all
   * objects matching a wildcard scope filter. The information is delivered to
   * the requesting program in standard message types, prefixed with T_MT_MON_.
   * (For brevity, the T_MT_ portion is omitted from this point on.) The
   * fields of these monitoring message types contain the information the
   * RTclient is interested in.  In other words, this class does not parse the
   * results of the monitoring requests; it simply builds the requests and
   * sends them to the RTserver.  It's up to the application to write a
   * callback that receives a monitoring reply message and parses out the
   * desired information.
   *
   * There is no monitoring available for peer-to-peer connections or for
   * programs that do not use the SmartSockets API or C++ Class Library.
   *
   * Monitoring is built into SmartSockets. No user-defined code is needed to
   * support monitoring in an RTclient.
   *
   */
  class T_IPCX_API TipcMon {

    T_IPC_SRV _srv;

  public:
    /**
     * Constructor
     * @param tsrv server connection used to send the monitoring request.  All
     * resuls received from the monitoring requests should be trapped using this
     * object (either by calling TipcSrv::searchType() or by creating a callback
     * on the appropriate message type.
     * @exception TipcMonException thrown if the connection to RTserver is
     * invalid.
     */
    TipcMon(const TipcSrv &tsrv) throw (TipcException);

    /**
     * Destructor
     */
    virtual ~TipcMon() throw();

  public:
    /**
     * Gets the monitoring identification string of this TipcSrv connection;
     * this identification string is used as a descriptive name for the
     * connection when it is being monitored. It is used as part of a field in
     * these message types:
     * - MON_CLIENT_NAMES_STATUS
     * - MON_SERVER_NAMES_STATUS
     * - MON_CLIENT_NAMES_POLL_RESULT
     * - MON_CLIENT_NAMES_NUM_POLL_RESULT
     * - MON_SERVER_NAMES_POLL_RESULT
     *
     * In these message types, the full field has the form "ident: user@node"
     * (e.g. "Primary_RTclient: ssuser@workstation1"). The monitoring
     * identification string is used as ident. By default the monitoring
     * identification string is set to 'RTclient'.  This string can be set with
     * the method setIdentStr().
     */
    const char *  getIdentStr() const throw (TipcException);

    /**
     * Sets the monitoring identification string of the TipcSrv connection
     * passed as an argument to the constructor; this identification string
     * is used as a descriptive name for the connection
     * when it is being monitored. It is used as part of a field in these
     * message types:
     * - MON_CLIENT_NAMES_STATUS
     * - MON_SERVER_NAMES_STATUS
     * - MON_CLIENT_NAMES_POLL_RESULT
     * - MON_CLIENT_NAMES_NUM_POLL_RESULT
     * - MON_SERVER_NAMES_POLL_RESULT
     *
     * In these message types, the full field has the form "ident: user@node"
     * (e.g. "Primary_RTclient: ssuser@workstation1"). The monitoring
     * identification string is used as ident.
     */
    void setIdentStr(const char * type_str) throw (TipcException);

    // Watch methods

    /// Print all monitoring categories being watched.
    void printWatch(T_OUT_FUNC out_func = (T_OUT_FUNC) TutOut)
    throw (TipcException);

    /**
     * Determines whether RTclient is watching project name
     */
    bool projectNamesGetWatch() const throw (TipcException);

    /**
     * Start or stop watching project names.  This method also saves the watch
     * status internally and sends a MON_PROJECT_NAMES_SET_WATCH message
     * to RTserver. The watch status controls whether or not the RTclient
     * receives a MON_PROJECT_NAMES_STATUS message each time a project is
     * created or destroyed. A project is considered created when the first
     * RTclient in that project connects to RTserver. A project is considered
     * destroyed when the last RTclient in that project disconnects from
     * RTserver. The watch status is saved internally so that the RTclient can
     * automatically rewatch all appropriate categories if RTclient has to
     * reconnect to RTserver.
     *
     * Each MON_PROJECT_NAMES_STATUS message contains three fields:\n
     * - a STR_ARRAY field containing the current project names\n
     * - a STR field containing the name of the created project (or "" if
     *   not applicable)\n
     * - a STR field containing the name of the destroyed project (or "" if
     *   not applicable)\n
     *
     * In addition to sending a MON_PROJECT_NAMES_STATUS message each time a
     * project is created or destroyed, RTserver also sends an initial
     * MON_PROJECT_NAMES_STATUS message when the watch status is set to TRUE,
     * so that the RTclient does not have to wait for the next project name
     * change to receive the first MON_PROJECT_NAMES_STATUS message.
     * To determine if this RTclient is watching project names, use
     * projectNamesGetWatch(). To poll once for project names, use
     * projectNamesPoll().
     *
     * @throw TipcMonException if this watch has already been set, or if a
     * problem occurs with the RTserver connection.
     */
    void projectNamesSetWatch(bool watch_status) throw (TipcException);

    /**
     * Determines whether RTclient is watching RTclient names.
     * @see clientNamesSetWatch() for more details on watching RTclient names.
     */
    bool clientNamesGetWatch() const throw (TipcException);

    /**
     * Starts or stops watching any RTclient names in the current project that
     * match the value of the Monitor_Scope option.  This method saves the
     * watch status and sends a MON_CLIENT_NAMES_SET_WATCH message to RTserver.      * The watch status controls whether or not the RTclient receives a
     * MON_CLIENT_NAMES_STATUS message each time an RTclient is created or
     * destroyed. An RTclient is considered created when it connects to
     * RTserver. An RTclient is considered destroyed when it disconnects from
     * RTserver. The watch status is saved internally so that the RTclient can
     * automatically rewatch all appropriate categories if RTclient has to
     * reconnect to RTserver.
     *
     * Each MON_CLIENT_NAMES_STATUS message contains five fields:\n
     * - a STR_ARRAY field containing the current RTclient names (unique
     *   subjects)\n
     * - a STR_ARRAY field containing RTclient info strings of the form
     *   "ident: user@node" (such as "RTclient: ssuser@workstation1")\n
     * - a STR_ARRAY field containing the name of the RTserver each RTclient
     *   is connected to a STR field containing the name of the created
     *   RTclient (or "" if not applicable)\n
     * - a STR field containing the name of the destroyed RTclient (or "" if
     *   not applicable)\n
     *
     * In addition to sending a MON_CLIENT_NAMES_STATUS message each time an
     * RTclient is created or destroyed, RTserver also sends an initial
     * MON_CLIENT_NAMES_STATUS message when the watch status is set to TRUE,
     * so that the RTclient does not have to wait for the next RTclient name
     * change to receive the first MON_CLIENT_NAMES_STATUS message.
     *
     * To determine if this RTclient is watching RTclient names, use
     * clientNamesGetWatch(). To poll once for the RTclient names, use
     * clientNamesPoll().
     */
    void clientNamesSetWatch(bool watch_status) throw (TipcException);

    /**
     * Determines whether RTclient is watching RTserver names
     * @see serverNamesSetWatch() for more details on watching RTserver names.
     */
    bool serverNamesGetWatch() const throw (TipcException);

    /**
     * Start or stop watching RTserver names that match the value of the
     * Monitor_Scope option. This method saves the watch status and sends a
     * MON_SERVER_NAMES_SET_WATCH message to RTserver. The watch status
     * controls whether or not the RTclient receives a MON_SERVER_NAMES_STATUS
     * message each time an RTserver is created or destroyed.  An RTserver is
     * considered created when it starts up (and usually connects to other
     * RTservers). An RTserver is considered destroyed when it terminates or
     * disconnects from other RTservers. The watch status is saved internally
     * so that the RTclient can automatically rewatch all appropriate
     * categories if RTclient has to reconnect to RTserver.
     *
     * Each MON_SERVER_NAMES_STATUS message contains four fields:\n
     * - a STR_ARRAY field containing the current RTserver names\n
     * - a STR_ARRAY field containing RTserver info strings of the form
     *   "ident: user@node" (for example, "RTserver: ssuser@workstation1")\n
     * - a STR field containing the name of the created RTserver (or "" if
     *   not applicable)\n
     * - a STR field containing the name of the destroyed RTserver (or "" if
     * not applicable)
     *
     * In addition to sending a MON_SERVER_NAMES_STATUS message each time an
     * RTserver is created or destroyed, RTserver also sends an initial
     * MON_SERVER_NAMES_STATUS message when the watch status is set to TRUE,
     * so that the RTclient does not have to wait for the next RTserver name
     * change to receive the first MON_SERVER_NAMES_STATUS message.
     *
     * To determine if this RTclient is watching RTserver names, use
     * serverNamesGetWatch(). To poll once for RTserver names, use
     * serverNamesPoll() .
     */
    void serverNamesSetWatch(bool watch_status) throw (TipcException);

    /// Determines whether RTclient is watching subject names
    bool subjectNamesGetWatch() const throw (TipcException);

    /**
     * Start or stop watching subject names in the current project that match
     * the value of the Monitor_Scope option.  This method saves the watch
     * status and sends a MON_SUBJECT_NAMES_SET_WATCH message to RTserver.
     * The watch status controls whether or not the RTclient receives a
     * MON_SUBJECT_NAMES_STATUS message each time a subject is created or
     * destroyed. A subject is considered created when the first RTclient
     * starts subscribing to that subject. A subject is considered destroyed
     * when the last RTclient stops subscribing to that subject. The watch
     * status is saved internally so that the RTclient can automatically
     * rewatch all appropriate categories if RTclient has to reconnect to
     * RTserver.
     *
     * Each MON_SUBJECT_NAMES_STATUS message contains three fields:
     * - a STR_ARRAY field containing the current subject names\n
     * - a STR field containing the name of the created subject (or "" if not
     *   applicable)\n
     * - a STR field containing the name of the destroyed subject (or "" if
     *   not applicable)\n
     * In addition to sending a MON_SUBJECT_NAMES_STATUS message each time a
     * subject is created or destroyed, RTserver also sends an initial
     * MON_SUBJECT_NAMES_STATUS message when the watch status is set to TRUE,
     * so that the RTclient does not have to wait for the next subject name
     * change to receive the first MON_SUBJECT_NAMES_STATUS message.
     *
     * To determine if this RTclient is watching subject names, use
     * subjectNamesGetWatch(). To poll once for subject names, use
     * subjectNamesPoll().
     */
    void subjectNamesSetWatch(bool watch_status) throw (TipcException);

    /**
     * Determines whether this RTclient is watching the Rtclients
     * that are subscribing to a subject.
     * @see subjectSubscribeSetWatch
     */
    bool subjectSubscribeGetWatch(const char * subject_name) const
    throw (TipcException);

    /**
     * Start or stop watching the RTclients that are subscribing to a subject.
     * This method saves the watch status and sends a
     * MON_SUBJECT_SUBSCRIBE_SET_WATCH message to RTserver.  The watch status
     * controls whether or not the RTclient receives a
     * MON_SUBJECT_SUBSCRIBE_STATUS message each time an RTclient starts or
     * stops subscribing to the subject. The watch status is saved internally
     * so that the RTclient can automatically rewatch all appropriate
     * categories if RTclient has to reconnect to RTserver. If subject_name
     * is T_IPC_MON_ALL then all subjects in the current project that match
     * the value of the Monitor_Scope option are watched.
     *
     * Each MON_SUBJECT_SUBSCRIBE_STATUS message contains four fields:
     * - a STR field containing the name of the subject\n
     * - a STR_ARRAY field containing the names of the current RTclients
     *   that are subscribing to the subject\n
     * - a STR field containing the name of the RTclient that just started
     *   subscribing to the subject (or "" if not applicable)\n
     * - a STR field containing the name of the RTclient that just stopped
     *   subscribing to the subject (or "" if not applicable)\n
     *
     * In addition to sending a MON_SUBJECT_SUBSCRIBE_STATUS message each time
     * an RTclient starts or stops subscribing to the subject, RTserver also
     * sends an initial MON_SUBJECT_SUBSCRIBE_STATUS message (or many messages
     * if T_IPC_MON_ALL is used) when the watch status is set to TRUE, so that
     * the RTclient does not have to wait for the next subject subscriber
     * change to receive the first MON_SUBJECT_SUBSCRIBE_STATUS message.
     * To determine if this RTclient is watching the RTclients that are
     * subscribing to a subject, use subjectSubscribeGetWatch(). To poll once
     * for the RTclients that are subscribing to a subject, use
     * subjectSubscribePoll().
     *
     */
    void subjectSubscribeSetWatch(const char * subject_name, bool watch_status)
    throw (TipcException);

    /**
     * Polls once for project names by sending a MON_PROJECT_NAMES_POLL_CALL
     * message to RTserver. RTserver responds by sending back a
     * MON_PROJECT_NAMES_POLL_RESULT message. The response should
     * come back quickly. The polling RTclient can use methods such as
     * TipcSrv::searchType() or TipcSrv::mainLoop() to get the response. The
     * MON_PROJECT_NAMES_POLL_RESULT message contains one field, a STR_ARRAY
     * field containing the current project names.
     *
     * To watch for created and destroyed project names, use
     * projectNamesSetWatch().
     */
    void projectNamesPoll() throw (TipcException);

    /**
     * Polls for the subject names in the current project that match the
     * value of the Monitor_Scope option by sending a
     * MON_SUBJECT_NAMES_POLL_CALL
     * message to RTserver. RTserver responds by sending back a
     * MON_SUBJECT_NAMES_POLL_RESULT message. The response should come back
     * quickly. The polling RTclient can use methods such as
     * TipcSrv::searchType() or TipcSrv::mainLoop() to get the response.
     * The MON_SUBJECT_NAMES_POLL_RESULT
     * message contains one field, a STR_ARRAY field containing the current
     * subject names.
     *
     * To watch for created and destroyed subject names, use
     * subjectNamesSetWatch()
     */
    void subjectNamesPoll() throw (TipcException);

    /**
     * Polls once for RTclients subscribing to a subject by sending
     * a MON_SUBJECT_SUBSCRIBE_POLL_CALL message to RTserver. If
     * subject_name is T_IPC_MON_ALL then all subjects in the current project
     * that match the value of the
     * Monitor_Scope option are polled. RTserver responds by sending back a
     * MON_SUBJECT_SUBSCRIBE_POLL_RESULT message. The response should come back
     * quickly.
     * The polling RTclient can use methods such as TipcSrv::searchType() or
     * TipcSrv::mainLoop() to get the response.
     * The MON_SUBJECT_SUBSCRIBE_POLL_RESULT message contains groups of two
     * fields:
     * - a STR field containing the name of the subject\n
     * - a STR_ARRAY field containing the names of the current RTclients that
     *   are subscribing to the subject\n
     *
     * The group of two fields is repeated multiple times only if subject_name
     * is T_IPC_MON_ALL.
     * To watch for subject subscribers, use subjectSubscribeSetWatch()
     */
    void subjectSubscribePoll(const char * dg_name) throw (TipcException);

    /**
     * Polls for the RTclient names in the current project that match the
     * value of the Monitor_Scope option by sending a MON_CLIENT_NAMES_POLL_CALL
     * message to RTserver. RTserver responds by sending back a
     * MON_CLIENT_NAMES_POLL_RESULT message. The response should come back
     * quickly.  The polling RTclient can use methods such as
     * TipcSrv::searchType() or TipcSrv::mainLoop()
     * to get the response. The MON_CLIENT_NAMES_POLL_RESULT message contains
     * three fields:
     * - a STR_ARRAY field containing the current RTclient names\n
     * - a STR_ARRAY field containing RTclient info strings of the form
     * "ident: user@node" (such as "RTclient: ssuser@workstation1")\n
     * - a STR_ARRAY field containing the name of the RTserver each RTclient
     * is connected to\n
     * To watch for created and destroyed RTclient names, use
     * clientNamesSetWatch()
     */
    void clientNamesPoll() throw (TipcException);

    /**
     * Polls for the RTserver names that match the value of the
     * Monitor_Scope option by sending a MON_SERVER_NAMES_POLL_CALL message to
     * RTserver. RTserver responds by sending back a
     * MON_SERVER_NAMES_POLL_RESULT message. The response should come back
     * quickly. The polling RTclient can use methods
     * such as TipcSrv::searchType or TipcSrv::mainLoop() to get the response.
     * The MON_SERVER_NAMES_POLL_RESULT message contains two fields:
     * - a STR_ARRAY field containing the current RTserver names\n
     * - a STR_ARRAY field containing RTserver info strings of the form
     *   "ident: user@node" (for example, "RTserver: ssuser@workstation1")\n
     *
     * To watch for created and destroyed RTserver names, use
     * serverNamesSetWatch();
     */
    void serverNamesPoll() throw (TipcException);

    /**
     * Determine whether this RTclient is watching RTserver connections.
     * @see serverConnSetWatch() for more details on watching RTserver conns.
     */
    bool serverConnGetWatch() const throw (TipcException);

    /**
     * Sets whether or not this RTclient is watching RTserver
     * connections of the RTservers that match the value of
     * the Monitor_Scope option.  This method saves the watch status and sends a
     * MON_SERVER_CONN_SET_WATCH message to RTserver. The watch status controls
     * whether or not the RTclient receives a MON_SERVER_CONN_STATUS
     * message each time a
     * connection between RTservers is created or destroyed. The
     * watch status is saved internally
     * so that the RTclient can automatically rewatch all appropriate
     * categories if RTclient has to reconnect to RTserver.
     * Each MON_SERVER_CONN_STATUS message contains eight fields
     * (all the array fields have the same number of elements):
     * - a STR_ARRAY field containing the names of the first RTservers
     *   on the connections\n
     * - a STR_ARRAY field containing the names of the second RTservers
     *   on the connections\n
     * - a STR_ARRAY field containing the logical connection names of
     *   the connections\n
     * - an INT4_ARRAY field containing the weights of the
     *   connections (always one)\n
     * - a STR field containing the name of the first RTserver on
     *   the created connection (or "" if not applicable)\n
     * - a STR field containing the name of the second RTserver on
     *   the created connection (or "" if not applicable)\n
     * - a STR field containing the name of the first RTserver on the
     *   destroyed connection (or "" if not applicable)\n
     * - a STR field containing the name of the second RTserver on
     *   the destroyed connection (or "" if not applicable)\n
     *
     * In addition to sending a MON_SERVER_CONN_STATUS message each time an
     * RTserver
     * connection is created or destroyed, RTserver also sends an initial
     * MON_SERVER_CONN_STATUS message when the watch status is set to TRUE,
     * so that theRTclient does not have to wait for the next
     * RTserver connection change to receive the first
     * MON_SERVER_CONN_STATUS message.
     *
     * To determine if this RTclient is watching RTserver connections, use
     * serverConnGetWatch(). To poll once for RTserver connections, use
     * serverConnPoll().
     */
    void serverConnSetWatch(bool watch_status) throw (TipcException);

    /**
     * Polls for RTserver connections of the RTservers that match the
     * value of the Monitor_Scope option by sending a MON_SERVER_CONN_POLL_CALL
     * message to RTserver. RTserver responds by sending back a
     * MON_SERVER_CONN_POLL_RESULT message. The response should
     * come back quickly.
     * The polling RTclient can use methods such as
     * TipcSrv::searchType() or TipcSrv::mainLoop() to get the response.
     * The MON_SERVER_CONN_POLL_RESULT message contains four fields (all the
     * array fields have the same number of elements):
     * - a STR_ARRAY field containing the names of the first RTservers on
     *   the connections
     * - a STR_ARRAY field containing the names of the second RTservers on
     *   the connections
     * - a STR_ARRAY field containing the logical connection names of
     *   the connections
     * - an INT4_ARRAY field containing the weights of the connections
     *   (always one)
     * To watch for created and destroyed RTserver connections, use
     * serverConnSetWatch.
     */
    void serverConnPoll() throw (TipcException);

    /**
     * Polls for RTserver general information by sending a
     * MON_SERVER_GENERAL_POLL_CALL message to the server_name RTserver. The
     * server_name argument can be the unique subject name of a single
     * RTserver, a wildcarded
     * subject name to match many RTservers, or T_IPC_MON_ALL to poll all
     * RTservers that
     * match the value of the Monitor_Scope option. The polled RTserver
     * responds by sending
     * back a MON_SERVER_GENERAL_POLL_RESULT message. The response should come
     * back quickly. The polling RTclient can use methods such as
     * TipcSrv::searchType() or
     * TipcSrv::mainLoop() to get the response.
     * Each MON_SERVER_GENERAL_POLL_RESULT message contains seventeen fields:
     * - a STR field containing the name of the RTserver
     * - a STR field containing the process monitoring identifier (defaults to
     * - a major type of license, such as RTserver)
     * - a STR field containing the node name the RTserver is running on
     * - a STR field containing the user name the RTserver is running under
     * - an INT4 field containing the PID of the RTserver
     * - a STR field containing the name of the SmartSockets architecture
     *   (such as sun4_solaris)
     * - an INT4 field containing the current operating system sbrk value
     *   (the edge of the RTserver's virtual memory address space)
     * - an INT4 field containing the amount of change in the current
     *   operating system sbrk value (the edge of the process's virtual
     *   memory address space) since the RTserver
     *   started (actually, since the process's first call to
     *   any SmartSockets function)
     * - an INT2 field containing the integer number format of
     *   the RTserver (really of type T_INT_FORMAT, but stored in
     *   an INT2 field to save space)
     * - an INT2 field containing the real number format of
     *   the RTserver (really of type T_REAL_FORMAT, but stored in
     *   an INT2 field to save space)
     * - a STR field containing the name of the startup command file
     *   that the RTserver used at initialization
     * - an INT4 field containing whether the RTserver was started
     *   with the -no_daemon command-line argument (really of type BOOL)
     * - an INT4 field containing whether the RTserver was started
     *   with the -cmd_mode command-line argument (really of type BOOL)
     * - a STR_ARRAY field containing the names of the RTclients in
     *   the same project as the polling RTclient which are directly
     *   connected to the RTserver
     * - a STR_ARRAY field containing the names of the RTservers
     *   which are directly connected to the RTserver
     * - a STR_ARRAY field containing the names of the subjects in the
     *   same project as the
     *   polling RTclient to which the RTserver is subscribing
     *   as the result of an RTserver subscribe command
     * - a STR_ARRAY field containing the names of the subjects in
     *   the same project as the
     *   polling RTclient to which the RTserver is subscribing as the
     *   result of direct RTclients subscriptions
     *
     * There are multiple responses only if server_name is a
     * wildcard or T_IPC_MON_ALL.
     *
     * To poll once for RTclient general information, use clientGeneralPoll().
     *
     * For an example of how MON_SERVER_GENERAL_POLL_RESULT messages
     * can be used,
     * see the RTmon GDI Server Information window.
     */
    void serverGeneralPoll(const char * serverName) throw (TipcException);

    /**
     * Polls for RTserver time information by sending a
     * MON_SERVER_TIME_POLL_CALL message to the server_name RTserver. The
     * server_name argument can be the unique subject name of
     * a single RTserver, a wildcarded
     * subject name to match many RTservers, or T_IPC_MON_ALL
     * to poll all RTservers that
     * match the value of the Monitor_Scope option. The polled RTserver
     * responds by sending
     * back a MON_SERVER_TIME_POLL_RESULT message. The response should come back
     * quickly. The polling RTclient can use methods
     * such as TipcSrv::searchType() or TipcSrv::mainLoop() to get the response.
     *
     * Each MON_SERVER_TIME_POLL_RESULT message contains four fields:
     * - a STR field containing the name of the RTserver
     * - a REAL8 field containing the wall time (the time from TutGetWallTime)
     * - a STR field containing the wall time converted to a string
     *   using the full time format
     * - a REAL8 field containing the amount of CPU time used
     *
     * There are multiple responses only if server_name is a wildcard
     * or T_IPC_MON_ALL.
     * To poll once for RTclient time information, use clientTimePoll().
     *
     */
    void serverTimePoll(const char * serverName) throw (TipcException);

    /**
     * Polls for RTserver message-related buffer information by sending
     * a MON_SERVER_BUFFER_POLL_CALL message to the server_name RTserver. The
     * server_name argument can be the unique subject name of a single
     * RTserver, a wildcarded
     * subject name to match many RTservers, or T_IPC_MON_ALL to
     * poll all RTservers that match
     * the value of the Monitor_Scope option. The buffer information of
     * connected_process_name (which can be either an RTserver or
     * another RTclient in the same
     * project) is polled. The polled RTserver responds by sending back a
     * MON_SERVER_BUFFER_POLL_RESULT message. The
     * response should come back quickly.
     * If connected_process_name is T_IPC_MON_ALL then the buffers
     * in the polled RTserver of
     * all connected processes are polled. The polling RTclient
     * can use methods such as
     * TipcSrv::searchType() or TipcSrv::mainLoop() to get the response.
     * Each MON_SERVER_BUFFER_POLL_RESULT message
     * contains one fixed field followed
     * by one or more groups of five fields. The fixed field is a STR field
     * containing the name of the
     * RTserver. The group of five fields is:
     * - a STR field containing the name of the process RTserver is connected to
     * - an INT4 field containing the number of queued incoming message in
     * the RTserver's connection to the connected process
     * - an INT4 field containing the number of bytes of data in the
     * queued incoming messages in the RTserver's connection
     * to the connected process
     * - an INT4 field containing the number of bytes of data in
     * the read buffer in the RTserver's connection to the connected process
     * - an INT4 field containing the number
     * of bytes of data in the write
     * buffer in the RTserver's connection to the connected process
     * There are multiple responses only if server_name
     * is a wildcard or T_IPC_MON_ALL.
     * To poll once for RTclient buffer information, use clientBufferPoll.
     * For an example of how MON_SERVER_BUFFER_POLL_RESULT messages
     * can be used, see
     * the RTmon GDI Server Buffers window.
     */
    void serverBufferPoll(const char * serverName,
                          const char * connected_process_name)
    throw (TipcException);

    /**
     * Polls for RTserver option information by sending a
     * MON_SERVER_OPTION_POLL_CALL message to the server_name RTserver. The
     * server_name argument can be the unique subject name of a single
     * RTserver, a wildcarded
     * subject name to match many RTservers, or T_IPC_MON_ALL to poll all
     * RTservers that
     * match the value of the Monitor_Scope option. The polled RTserver
     * responds by sending
     * back a MON_SERVER_OPTION_POLL_RESULT message. The response should come
     * back
     * quickly. If option_name is T_IPC_MON_ALL then all options
     * in the polled RTserver are
     * polled. The polling RTclient can use methods such as
     * TipcSrv::searchType() or TipcSrv::mainLoop() to get the response.
     * Each MON_SERVER_OPTION_POLL_RESULT message contains one fixed field
     * followed
     * by one or more groups of five fields. The fixed field is a STR field
     * containing the name of the RTserver. The group of five fields is:
     * - a STR field containing the name of the option\n
     * - an INT2 field containing the option type (really T_OPT_TYPE)\n
     * - a STR field containing the value of the option, converted to a string
     *   like the setopt command displays\n
     * - an INT4 field containing whether or not the option is required to
     *   have a value (that is, cannot be UNKNOWN)\n
     * - a STR_ARRAY field containing the legal values for the option; this
     *   array is empty if not applicable.\n
     *
     * There are multiple responses only if server_name is a wildcard or
     * T_IPC_MON_ALL.
     * To poll once for RTclient option information, use clientOptionPoll.
     */
    void serverOptionPoll(const char * serverName, const char * option_name)
    throw (TipcException);

    /**
     * Polls for RTserver message traffic information by sending a
     * MON_SERVER_MSG_TRAFFIC_POLL_CALL message to the server_name RTserver. The
     * server_name argument can be the unique subject name of a
     * single RTserver, a wildcarded
     * subject name to match many RTservers, or T_IPC_MON_ALL
     * to poll all RTservers that
     * match the value of the Monitor_Scope option.
     * The message traffic information of
     * connected_process_name (which can be either an RTserver or
     * another RTclient in the same
     * project) is polled. The polled RTserver responds by sending back a
     * MON_SERVER_MSG_TRAFFIC_POLL_RESULT message. The response should come back
     * quickly. If connected_process_name is T_IPC_MON_ALL then
     * the message traffic in the
     * polled RTserver of all connected processes are polled. The
     * polling RTclient can use methods
     * such as TipcSrv::searchType() or TipcSrv::mainLoop() to get the response.
     * Each MON_SERVER_MSG_TRAFFIC_POLL_RESULT message contains one fixed field
     * followed by one or more groups of five fields. The fixed field is
     * a STR field containing the
     * name of the RTserver.
     * The group of five fields is:
     * - a STR field containing the name of the process RTserver is connected to
     * - an INT4 field containing the number of messages the polled
     * RTserver has sent on its
     * connection to the connected process
     * - an INT4 field containing the number of messages the polled
     * RTserver has received on its
     * connection to the connected process
     * - an INT4 field containing the number of bytes the polled
     * RTserver has sent on its
     * connection to the connected process
     * - an INT4 field containing the number of bytes the polled
     * RTserver has received on its
     * connection to the connected process
     * There are multiple responses only if server_name
     * is a wildcard or T_IPC_MON_ALL.
     * To poll once for RTclient message
     * traffic information, use clientMsgTrafficPoll.
     * For an example of how MON_SERVER_MSG_TRAFFIC_POLL_RESULT messages can be
     * used, see the RTmon GDI Watch Server Connections window.
     *
     * @note The traffic statistics presented in the result message from this
     * poll are truncated to 31 bits, and may contain an incorrect sign bit on
     * some older servers. This method is provided for compatibility with
     * pre-6.7 servers. When polling newer servers, you should use
     * serverMsgTrafficExPoll.
     * @see serverMsgTrafficExPoll
     */

    void serverMsgTrafficPoll(const char * serverName,
                              const char * connected_process_name)
    throw (TipcException);

    // New in 6.7 (this poll only)
    /**
     * Polls for RTserver message traffic information by sending a
     * MON_SERVER_MSG_TRAFFIC_EX_POLL_CALL message to the server_name RTserver.
     * The server_name argument can be the unique subject name of a
     * single RTserver, a wildcarded
     * subject name to match many RTservers, or T_IPC_MON_ALL
     * to poll all RTservers that
     * match the value of the Monitor_Scope option.
     * The message traffic information of
     * connected_process_name (which can be either an RTserver or
     * another RTclient in the same
     * project) is polled. The polled RTserver responds by sending back a
     * MON_SERVER_MSG_TRAFFIC_EX_POLL_RESULT message. The response should come
     * back quickly. If connected_process_name is T_IPC_MON_ALL then
     * the message traffic in the
     * polled RTserver of all connected processes are polled. The
     * polling RTclient can use methods
     * such as TipcSrv::searchType() or TipcSrv::mainLoop() to get the response.
     * Each MON_SERVER_MSG_TRAFFIC_EX_POLL_RESULT message contains one fixed
     * field followed by five field arrays. The fixed field is a STR field
     * containing the name of the RTserver.
     * The five arrays are:
     * - a STR_ARRAY field containing the names of the process RTserver is
     * connected to
     * - an INT8_ARRAY field containing the number of messages the polled
     * RTserver has sent on its
     * connection to the connected processes
     * - an INT8_ARRAY field containing the number of messages the polled
     * RTserver has received on its
     * connection to the connected processes
     * - an INT8_ARRAY field containing the number of bytes the polled
     * RTserver has sent on its
     * connection to the connected processes
     * - an INT8_ARRAY field containing the number of bytes the polled
     * RTserver has received on its
     * connection to the connected processes
     * There are multiple responses only if server_name
     * is a wildcard or T_IPC_MON_ALL.
     * To poll once for RTclient message
     * traffic information, use clientMsgTrafficPoll.
     * @since SmartSockets 6.7.0
     */
    void serverMsgTrafficExPoll(const char * serverName,
                                const char * connected_process_name)
    throw (TipcException);

    /**
     * Polls for RTserver route information by sending a
     * MON_SERVER_ROUTE_POLL_CALL message to the server_name RTserver. The
     * server_name argument can be the unique subject name of a
     * single RTserver, a wildcarded
     * subject name to match many RTservers, or T_IPC_MON_ALL
     * to poll all RTservers that
     * match the value of the Monitor_Scope option. The
     * route information to the RTserver
     * dest_server_name is polled. The polled
     * RTserver responds by sending back a
     * MON_SERVER_ROUTE_POLL_RESULT message. The
     * response should come back quickly.
     * If dest_server_name is T_IPC_MON_ALL then the routes in
     * the polled RTserver to all
     * other RTservers are polled. The polling RTclient can use methods such as
     * TipcSrv::searchType() or TipcSrv::mainLoop() to get the response.
     * Each MON_SERVER_ROUTE_POLL_RESULT message contains
     * one fixed field followed by
     * one or more groups of four fields. The fixed field is a STR field
     * containing the name of the
     * RTserver. The group of four fields is:
     * - a STR field containing the name of the destination RTserver
     * - a STR_ARRAY field containing the names of the
     * RTservers which are directly connected
     * to the destination RTserver
     * - an INT4 field containing the distance (in connections or hops
     * ) from the polled RTserver
     * to the destination RTserver
     * - a STR_ARRAY field containing the names of the RTservers
     * to follow for the shortest
     * path from the polled RTserver to the destination RTserver
     * There are multiple responses only if server_name
     * is a wildcard or T_IPC_MON_ALL.
     */
    void serverRoutePoll(const char * serverName, const char * dest_server_name)
    throw (TipcException);


    /// Determines whether this RTclient is watching the subjects
    /// that an RTclient is subscribing to
    bool clientSubscribeGetWatch(const char * clientName) const
    throw (TipcException);

    /**
     * Sets whether or not this RTclient is watching the subjects
     * that the client_name RTclient is subscribing to. The
     * client_name argument can be the
     * unique subject name of a single RTclient, a wildcarded
     * subject name to match many
     * RTclients, or T_IPC_MON_ALL to poll all RTclients in the project that
     * match the value of the
     * Monitor_Scope option. clientSubscribeSetWatch saves
     * the watch status and sends
     * a MON_CLIENT_SUBSCRIBE_SET_WATCH message to RTserver. The watch status
     * controls whether or not
     * the RTclient receives a MON_CLIENT_SUBSCRIBE_STATUS
     * message each time the watched RTclient starts or stops subscribing to
     * a subject. The watch
     * status is saved internally so that the RTclient
     * can automatically rewatch all appropriate
     * categories if RTclient has to reconnect to RTserver.
     * Each MON_CLIENT_SUBSCRIBE_STATUS message contains four fields:
     * - a STR field containing the name of the RTclient
     * - a STR_ARRAY field containing the names of the current subjects that
     * the RTclient is subscribing to
     * - a STR field containing the name of the subject that the RTclient
     * has just started subscribing to (
     * or "" if not applicable)
     * - a STR field containing the name of the subject that the RTclient
     * has just stopped subscribing to (
     * or "" if not applicable)
     * In addition to sending a
     * MON_CLIENT_SUBSCRIBE_STATUS message each time the
     * watched RTclient starts or stops subscribing to a subject,
     * RTserver also sends an initial
     * MON_CLIENT_SUBSCRIBE_STATUS message (or many messages if a wildcard or
     * T_IPC_MON_ALL is used) when the watch status is set to TRUE
     * , so that the RTclient
     * does not have to wait
     * for the next subscribed subject
     * change to receive the first
     * MON_CLIENT_SUBSCRIBE_STATUS message.
     * To determine if this RTclient is watching the subjects that an RTclient
     * is subscribing to, use
     * clientSubscribeGetWatch. To poll once for the
     * subjects that an RTclient is
     * subscribing to, use clientSubscribePoll.
     * For an example of how MON_CLIENT_SUBSCRIBE_STATUS messages can
     * be used, see the
     * RTmon GDI Watch Client Subjects window.
     */
    void clientSubscribeSetWatch(const char * clientName,
                                 bool watch_status) throw (TipcException);

    /// Determines whether this RTclient is watching time information
    /// for an RTclient.
    bool clientTimeGetWatch(const char * clientName)  const
    throw (TipcException);

    /**
     * Sets whether or not this RTclient is watching time information
     * in the client_name RTclient. The client_name argument can be
     * the unique subject name of
     * a single RTclient, a wildcarded subject name to match
     * many RTclients, or T_IPC_MON_ALL
     * to poll all RTclients in the project that match the
     * value of the Monitor_Scope option.
     * clientTimeSetWatch saves the watch status and sends a
     * MON_CLIENT_TIME_SET_WATCH message to the client_name
     * RTclient. The watch status
     * controls whether or not the RTclient
     * receives a MON_CLIENT_TIME_STATUS message each
     * time the current time changes in the watched RTclient. The
     * watch status is saved internally
     * so that the RTclient can automatically rewatch all appropriate
     * categories if RTclient has to
     * reconnect to RTserver.
     * Each MON_CLIENT_TIME_STATUS message contains seven fields:
     * - a STR field containing the name of the RTclient
     * - a REAL8 field containing the current time
     * (the time from TutGetCurrentTime)
     * - a STR field containing the current time converted to a
     * string using the format specified
     * in the Time_Format option; the time format from the watched
     * RTclient, not this RTclient,
     * is used
     * - a REAL8 field containing the wall time (the time from TutGetWallTime)
     * - a STR field containing the wall time converted to a string
     * using the full time format
     * - a REAL8 field containing the amount of CPU time used
     * - an INT4 field containing the number of frames processed
     * In addition to sending a MON_CLIENT_TIME_STATUS message
     * each time the current time
     * changes, the watched RTclient also
     * sends an initial MON_CLIENT_TIME_STATUS message
     * when the watch status is set to TRUE, so that the RTclient does not have
     * to wait for the next
     * time change to receive the first MON_CLIENT_TIME_STATUS message.
     * To determine if this RTclient is watching RTclient time information, use
     * clientTimeGetWatch. To poll once for RTclient time information, use
     * clientTimePoll.
     * For an example of how MON_CLIENT_TIME_STATUS messages can
     * be used, see the
     * RTmon GDI Watch Client Time window.
     */
    void clientTimeSetWatch(const char * clientName,
                            bool watch_status) throw (TipcException);

    /// Determines whether this RTclient is watching message-related
    /// buffer information in an RTclient
    bool clientBufferGetWatch(const char * clientName) const
    throw (TipcException);

    /**
     * Sets whether or not this RTclient is watching message-related
     * buffer information in the client_name RTclient. The
     * client_name argument can be the
     * unique subject name of a single RTclient, a wildcarded
     * subject name to match many
     * RTclients, or T_IPC_MON_ALL to watch all RTclients in the project
     * that match the value of
     * the Monitor_Scope option. clientBufferSetWatch saves
     * the watch status and sends
     * a MON_CLIENT_BUFFER_SET_WATCH message to the
     * client_name RTclient. The watch
     * status controls whether or not
     * the RTclient receives a MON_CLIENT_BUFFER_STATUS
     * message each time the message queue for the connection
     * to RTserver changes in the
     * watched RTclient. The watch status is saved internally so
     * that the RTclient can automatically
     * rewatch all appropriate categories if RTclient
     * has to reconnect to RTserver.
     * Each MON_CLIENT_BUFFER_STATUS message contains five fields:
     * - a STR field containing the name of the RTclient
     * - an INT4 field containing the number of queued incoming
     *   message in the RTclient's
     * connection to RTserver
     * - an INT4 field containing the number of bytes of data
     *   in the queued incoming messages in the RTclient's connection
     *   to RTserver
     * - an INT4 field containing the number of bytes of data in the read
     *   buffer in the RTclient's connection to RTserver
     * - an INT4 field containing the number of bytes of data in the write
     *   buffer in the RTclient's connection to RTserver
     * In addition to sending a MON_CLIENT_BUFFER_STATUS message each time
     * the message
     * queue changes, the watched RTclient
     * also sends an initial MON_CLIENT_BUFFER_STATUS
     * message when the watch status is set to TRUE, so that the RTclient does
     * not have to wait for
     * the next message queue change
     * to receive the first MON_CLIENT_BUFFER_STATUS
     * message.
     * To determine if this RTclient is watching
     * RTclient buffer information, use
     * clientBufferGetWatch. To poll once for RTclient buffer information, use
     * clientBufferPoll.
     * For an example of how MON_CLIENT_BUFFER_STATUS messages can
     * be used, see the
     * RTmon GDI Watch Client Buffer window.
     */
    void clientBufferSetWatch(const char * clientName,
                              bool watch_status) throw (TipcException);

    /// Determines whether this RTclient is watching received
    /// messages in an RTclient.
    bool clientMsgRecvGetWatch(const char * clientName,
                               const char * msg_type_name) const
    throw (TipcException);

    /**
     * Sets whether or not this RTclient is watching received
     * messages in the client_name RTclient. The client_name
     * argument can be the unique
     * subject name of a single RTclient, a wildcarded subject name to
     * match many RTclients, or
     * T_IPC_MON_ALL to watch all RTclients in the project that
     * match the value of the
     * Monitor_Scope option. clientMsgRecvSetWatch saves
     * the watch status and sends a
     * MON_CLIENT_MSG_RECV_SET_WATCH message to the client_name RTclient. The
     * watch status controls whether or not the RTclient receives a
     * MON_CLIENT_MSG_RECV_STATUS message each time a
     * received message is inserted or
     * deleted from the message queue for the connection to
     * RTserver in the watched RTclient.
     * The watch status is saved internally so the
     * RTclient can automatically rewatch all
     * appropriate categories if RTclient has to reconnect to
     * RTserver. If msg_type_name is
     * T_IPC_MON_ALL, then all message types in
     * the watched RTclient are watched.
     * Each MON_CLIENT_MSG_RECV_STATUS message contains six fields:
     * - a STR field containing the name of the RTclient
     * - a MSG field containing the received message
     * - an INT4 field containing whether or not the message was
     * just inserted into or deleted
     * from the connection's message queue (really of type T_BOOL);
     * a value of TRUE means
     * the message was inserted, a value of FALSE means the message was deleted
     * - an INT4 field containing the position in the queue where
     * the received message was just
     * inserted or deleted (a value of 0 means the front of the queue)
     * - a BINARY field containing a unique 8-byte identifier for the
     * received message; this field
     * is useful for processes that need to accurately track
     * the entire contents of the
     * connection's message queue, such as the RTmon GDI
     * - a BINARY field containing an array of unique 8-byte identifiers
     * for all messages in the
     * connection's message queue; this field is useful for
     * processes that need to accurately
     * track the entire contents of the connection's message queue,
     * such as the RTmon GDI
     * In addition to sending a MON_CLIENT_MSG_RECV_STATUS
     * message each time there is a
     * message queue change, the watched RTclient also sends an initial
     * MON_CLIENT_MSG_RECV_STATUS message when the watch status is set
     * to TRUE, so that
     * the RTclient does not have to wait for the next message queue
     * change to receive the first
     * MON_CLIENT_MSG_RECV_STATUS message.
     * To see if this RTclient is watching RTclient received messages, use
     * clientMsgRecvGetWatch.
     * For an example of how MON_CLIENT_MSG_RECV_STATUS messages can
     * be used, see the
     * RTmon GDI Watch Messages Received window.
     */
    void clientMsgRecvSetWatch(const char * clientName,
                               const char * msg_type_name,bool watch_status)
    throw (TipcException);

    /// Determines whether this RTclient is watching sent messages
    /// in an RTclient.
    bool clientMsgSendGetWatch(const char * clientName,
                               const char * msg_type_name) const
    throw (TipcException);

    /**
     * Sets whether or not this RTclient is watching sent
     * messages in the client_name RTclient. The client_name
     * argument can be the unique
     * subject name of a single RTclient, a wildcarded subject name to
     * match many RTclients, or
     * T_IPC_MON_ALL to watch all RTclients in the project that
     * match the value of the
     * Monitor_Scope option. clientMsgSendSetWatch saves
     * the watch status and sends a
     * MON_CLIENT_MSG_SEND_SET_WATCH message to the client_name RTclient. The
     * watch status controls whether or not the RTclient receives a
     * MON_CLIENT_MSG_SEND_STATUS message each time a message
     * is sent to RTserver in
     * the watched RTclient. The watch status is saved internally
     * so that the RTclient can
     * automatically rewatch all appropriate categories if RTclient has to
     * reconnect to RTserver. If
     * msg_type_name is T_IPC_MON_ALL then all message types
     * in the watched RTclient are
     * watched.
     * Each MON_CLIENT_MSG_SEND_STATUS message contains two fields:
     * - a STR field containing the name of the RTclient
     * - a MSG field containing the sent message
     * Unlike most of the other watch categories, the watched RTclient does
     * not also send an initial
     * MON_CLIENT_MSG_SEND_STATUS message when the watch status is set
     * to TRUE. This is
     * because the status message is only sent when a message is sent, and
     * thus there is no current
     * status to use for initialization.
     * To determine if this RTclient is watching RTclient sent messages, use
     * clientMsgSendGetWatch.
     * For an example of how MON_CLIENT_MSG_SEND_STATUS messages can
     * be used, see the
     * RTmon GDI Watch Messages Sent window.
     */

    void clientMsgSendSetWatch(const char * clientName,
                               const char * msg_type_name,
                               bool watch_status) throw (TipcException);


    /**
     * Polls for subjects that the client_name RTclient is subscribing to
     * by sending a MON_CLIENT_SUBSCRIBE_POLL_CALL message to RTserver. The
     * client_name argument can be the unique subject name of a
     * single RTclient, a wildcarded
     * subject name to match many RTclients, or T_IPC_MON_ALL to
     * poll all RTclients in the
     * project that match the value of the Monitor_Scope option.
     * RTserver responds by sending
     * back a MON_CLIENT_SUBSCRIBE_POLL_RESULT message. The response should come
     * back quickly. The polling RTclient can use methods such as
     * TipcSrv::searchType() or
     * TipcSrv::mainLoop() to get the response.
     * The MON_CLIENT_SUBSCRIBE_POLL_RESULT message contains
     * one or more groups of
     * two fields:
     * - a STR field containing the name of the RTclient
     * - a STR_ARRAY field containing the names of the current
     * subjects that the RTclient is
     * subscribing to
     * This group of two fields is repeated multiple times only if
     * client_name is a wildcard or
     * T_IPC_MON_ALL.
     * To watch for subjects that an RTclient is subscribing to, use
     * clientSubscribeSetWatch.
     */
    void clientSubscribePoll(const char * clientName)
    throw (TipcException);

    /**
     * Polls for RTclient general information by sending a
     * MON_CLIENT_GENERAL_POLL_CALL message to the client_name RTclient. The
     * client_name argument can be the unique subject name of a
     * single RTclient, a wildcarded
     * subject name to match many RTclients, or T_IPC_MON_ALL to
     * poll all RTclients in the
     * project that match the value of the Monitor_Scope option.
     * The polled RTclient responds by
     * sending back a MON_CLIENT_GENERAL_POLL_RESULT message. The
     * response may or
     * may not come back quickly, depending on what the polled RTclient
     * is doing. The polling
     * RTclient can use methods such as TipcSrv::searchType() or TipcSrv::
     * mainLoop() to get the
     * response.
     *
     * Each MON_CLIENT_GENERAL_POLL_RESULT message contains sixteen fields:
     * - a STR field containing the name of the RTclient
     * - a STR field containing the process monitoring identifier (defaults
     * to a major type of
     * license, such as Development_RTmon)
     * - a STR field containing the node name the RTclient is running on
     * - a STR field containing the user name the RTclient is running under
     * - an INT4 field containing the PID of the RTclient
     * - a STR field containing the project the RTclient is a part of
     * - a STR field containing the name of the RTserver
     * the RTclient is connected to
     * - a STR field containing the name of the SmartSockets
     * architecture (such as sun4_solaris)
     * - an INT4 field containing the current operating system sbrk value (
     * the edge of the RTclient'
     * s virtual memory address space)
     * - an INT4 field containing the amount of change in the
     * current operating system sbrk value (the edge of the process
     * 's virtual memory address space) since the RTclient
     * started (actually, since the process's first call to any
     * SmartSockets methods)
     * - an INT2 field containing the integer number format of the RTclient
     * (really of type T_INT_FORMAT
     * , but stored in an
     * INT2 field to save space)
     * - an INT2 field containing the real number format of the RTclient
     * (really of type T_REAL_FORMAT , but stored in an
     * INT2 field to save space)
     * - a STR field containing the fully expanded logical connection name
     * of the connection to RTserver
     * - an INT4_ARRAY field containing the counted licenses that
     * the RTclient has checked out ( really an array of type
     * T_LICENSE, but stored in an INT4_ARRAY field for portability)
     * - a STR_ARRAY field containing the extra licenses that the
     * RTclient has checked out (such as snmp)
     * - a STR_ARRAY field containing the names of the subjects that
     * the RTclient is subscribing to
     * There are multiple responses only if client_name
     * is a wildcard or T_IPC_MON_ALL.
     * To poll once for RTserver general information, use serverGeneralPoll.
     * For an example of how
     * MON_CLIENT_GENERAL_POLL_RESULT messages can be used,
     * see the RTmon GDI Client Information window.
     */
    void clientGeneralPoll(const char * clientName)
    throw (TipcException);

    /**
     * Polls for RTclient time information by sending a
     * MON_CLIENT_TIME_POLL_CALL message to the client_name RTclient. The
     * client_name argument can be the unique subject name of a
     * single RTclient, a wildcarded
     * subject name to match many RTclients, or T_IPC_MON_ALL to
     * poll all RTclients in the
     * project that match the value of the Monitor_Scope option.
     * The polled RTclient responds by
     * sending back a MON_CLIENT_TIME_POLL_RESULT message.
     * The response may or may
     * not come back quickly, depending on what the polled RTclient is
     * doing. The polling RTclient
     * can use methods such as TipcSrv::searchType() or TipcSrv::mainLoop()
     * to get the response.
     * Each MON_CLIENT_TIME_POLL_RESULT message contains seven fields:
     * - a STR field containing the name of the RTclient
     * - a REAL8 field containing the current time (the time from
     *   TutGetCurrentTime)
     * - a STR field containing the current time converted to
     *   a string using the format specified in the Time_Format option;
     *   the time format from the polled RTclient, not
     *   this RTclient, is used
     * - a REAL8 field containing the wall time (the time from TutGetWallTime)
     * - a STR field containing the wall time converted to a string using
     *   the full time format
     * - a REAL8 field containing the amount of CPU time used
     * - an INT4 field containing the number of frames processed
     * There are multiple responses only if client_name is a wildcard or
     * T_IPC_MON_ALL.
     * To watch RTclient time information, use clientTimeSetWatch.
     */
    void clientTimePoll(const char * clientName)
    throw (TipcException);

    /**
     * Polls for RTclient message-related buffer information by sending a
     * MON_CLIENT_BUFFER_POLL_CALL message to the client_name RTclient. The
     * client_name argument can be the unique subject name of a
     * single RTclient, a wildcarded
     * subject name to match many RTclients, or T_IPC_MON_ALL to
     * poll all RTclients in the
     * project that match the value of the Monitor_Scope option.
     * The polled RTclient responds by
     * sending back a MON_CLIENT_BUFFER_POLL_RESULT message. The response may or
     * may not come back quickly, depending on what the polled RTclient
     * is doing. The polling
     * RTclient can use methods such as TipcSrv::searchType() or TipcSrv::
     * mainLoop() to get the
     * response.
     * Each MON_CLIENT_BUFFER_POLL_RESULT message contains five fields:
     * - a STR field containing the name of the RTclient
     * - an INT4 field containing the number of queued
     * incoming message in the RTclient's connection to RTserver
     * - an INT4 field containing the number of bytes of data in the queued
     * incoming messages in the RTclient's connection to RTserver
     * - an INT4 field containing the number of bytes of data in the
     * read buffer in the RTclient's connection to RTserver
     * - an INT4 field containing the number of bytes of data in the
     * write buffer in the RTclient's connection to RTserver
     * There are multiple responses only if client_name is a wildcard or
     * T_IPC_MON_ALL.
     * To watch RTclient buffer information, use clientBufferSetWatch.
     */
    void clientBufferPoll(const char * clientName)
    throw (TipcException);

    /**
     * Polls for RTclient option information by sending a
     * MON_CLIENT_OPTION_POLL_CALL message to the client_name RTclient. The
     * client_name argument can be the unique subject name of a
     * single RTclient, a wildcarded
     * subject name to match many RTclients, or T_IPC_MON_ALL to
     * poll all RTclients in the
     * project that match the value of the Monitor_Scope option.
     * The polled RTclient responds by
     * sending back a MON_CLIENT_OPTION_POLL_RESULT message. The response may or
     * may not come back quickly, depending on what the polled RTclient
     * is doing. If option_name
     * is T_IPC_MON_ALL, then all options in the polled RTclient are
     * polled. The polling RTclient
     * can use methods such as TipcSrv::searchType() or TipcSrv::mainLoop
     * () to get the response.
     * Each MON_CLIENT_OPTION_POLL_RESULT message
     * contains one fixed field followed
     * by one or more groups of five fields. The fixed field is a STR field
     * containing the name of the
     * RTclient. The group of five fields is:
     * - a STR field containing the name of the option
     * - an INT2 field containing the option type (really T_OPT_TYPE)
     * - a STR field containing the value of the option, converted to a string
     * like the setopt command displays
     * - an INT4 field containing whether or not the option is required to have
     * a value (that is, it cannot be UNKNOWN)
     * - a STR_ARRAY field containing the legal values for the option;
     * this array is empty if not applicable
     * There are multiple responses only if client_name
     * is a wildcard or T_IPC_MON_ALL.
     * To poll once for RTserver option information, use serverOptionPoll.
     */
    void clientOptionPoll(const char * clientName,
                          const char * option_name)
    throw (TipcException);

    /**
     * Polls for RTclient message type information by sending a
     * MON_CLIENT_MSG_TYPE_POLL_CALL message to the client_name RTclient. The
     * client_name argument can be the unique subject name of a
     * single RTclient, a wildcarded
     * subject name to match many RTclients, or T_IPC_MON_ALL to
     * poll all RTclients in the
     * project that match the value of the Monitor_Scope option.
     * The polled RTclient responds by
     * sending back a MON_CLIENT_MSG_TYPE_POLL_RESULT message. The response
     * may or
     * may not come back quickly, depending on what the polled
     * RTclient is doing. If
     * msg_type_name is T_IPC_MON_ALL, then all message types
     * in the polled RTclient are
     * polled. The polling RTclient can use methods such as
     * TipcSrv::searchType() or
     * TipcSrv::mainLoop() to get the response.
     * Each MON_CLIENT_MSG_TYPE_POLL_RESULT message contains
     * one fixed field and one
     * or more groups of fifteen fields. The fixed field is a STR field
     * containing the name of the
     * RTclient. The group of fifteen fields is:
     * - a STR field containing the name of the message type
     * - an INT4 field containing the message type number
     * - a STR field
     * containing the message type grammar
     * - an INT4 field containing whether or not the
     * message type priority is known
     * (really of type T_BOOL)
     * - an INT2 field containing the message type priority (not applicable if
     * the previous field is FALSE)
     * - an INT4 field containing the message type delivery
     * mode (really of type T_IPC_DELIVERY_MODE)
     * - an INT4 field containing the message type user-defined property
     * - an INT4 field containing the number of read callbacks the polled
     * RTclient has for this message type on its connection to RTserver
     * - an INT4 field containing the number of
     * write callbacks the polled RTclient
     * has for this message type
     * on its connection to RTserver
     * - an INT4 field containing the number of
     * process callbacks the polled RTclient
     * has for this message type
     * on its connection to RTserver
     * - an INT4 field containing the number of
     * queue callbacks the polled RTclient
     * has for this message type
     * on its connection to RTserver
     * - an INT4 field containing the number of messages the polled RTclient has
     * sent for this message type
     * on its connection to RTserver
     * - an INT4 field containing the number of messages the polled RTclient
     *  has received for this message type on its connection to RTserver
     * - an INT4 field containing the number of bytes the polled RTclient has
     * sent for this message type
     * on its connection to RTserver
     * - an INT4 field containing the number of bytes the polled RTclient has
     * received for this message type
     * on its connection to RTserver
     * There are multiple responses only if client_name is a wildcard or
     * T_IPC_MON_ALL.
     * To poll once for global
     * message traffic information, use clientMsgTrafficPoll. To
     * poll once for message traffic information
     * by subject, use clientSubjectPoll. To poll
     * once for global callback information, use clientCbPoll.
     *
     * @note The traffic statistics presented in the result message from this
     * poll are truncated to 31 bits, and may contain an incorrect sign bit on
     * some older servers. This method is provided for compatibility with
     * pre-6.7 servers. When polling newer servers, you should use
     * clientMsgTypeExPoll.
     * @see clientMsgTypeExPoll, clientSubjectPoll, clientCbPoll
     */
    void clientMsgTypePoll(const char * clientName,
                           const char * msg_type_name)
    throw (TipcException);

    // New in 6.7 (this poll only)
    /**
     * Polls for RTclient message type information by sending a
     * MON_CLIENT_MSG_TYPE_EX_POLL_CALL message to the client_name RTclient. The
     * client_name argument can be the unique subject name of a
     * single RTclient, a wildcarded
     * subject name to match many RTclients, or T_IPC_MON_ALL to
     * poll all RTclients in the
     * project that match the value of the Monitor_Scope option.
     * The polled RTclient responds by
     * sending back a MON_CLIENT_MSG_TYPE_EX_POLL_RESULT message. The response
     * may or may not come back quickly, depending on what the polled
     * RTclient is doing. If
     * msg_type_name is T_IPC_MON_ALL, then all message types
     * in the polled RTclient are
     * polled. The polling RTclient can use methods such as
     * TipcSrv::searchType() or
     * TipcSrv::mainLoop() to get the response.
     * Each MON_CLIENT_MSG_TYPE_EX_POLL_RESULT message contains
     * one fixed field and fifteen field arrays. The fixed field is a STR field
     * containing the name of the RTclient. The fifteen arrays are:
     * - a STR_ARRAY field containing the name of each message type
     * - an INT4_ARRAY field containing each message type number
     * - a STR_ARRAY field
     * containing theach message type grammar
     * - an INT4_ARRAY field containing whether or not each
     * message type priority is known
     * (really of type T_BOOL)
     * - an INT2_ARRAY field containing the message type priorities (not
     * applicable if the corresponding element in the previous field is FALSE)
     * - an INT4_ARRAY field containing the message type delivery
     * modes (really of type T_IPC_DELIVERY_MODE)
     * - an INT4_ARRAY field containing the message type user-defined properties
     * - an INT4_ARRAY field containing the number of read callbacks the polled
     * RTclient has for each message type on its connection to RTserver
     * - an INT4_ARRAY field containing the number of
     * write callbacks the polled RTclient
     * has for each message type
     * on its connection to RTserver
     * - an INT4_ARRAY field containing the number of
     * process callbacks the polled RTclient
     * has for each message type
     * on its connection to RTserver
     * - an INT4_ARRAY field containing the number of
     * queue callbacks the polled RTclient
     * has for each message type
     * on its connection to RTserver
     * - an INT8_ARRAY field containing the number of messages the polled
     * RTclient has sent for each message type
     * on its connection to RTserver
     * - an INT8_ARRAY field containing the number of messages the polled
     *  RTclient has received for each message type on its connection to
     * RTserver
     * - an INT8_ARRAY field containing the number of bytes the polled RTclient
     * has sent for each message type
     * on its connection to RTserver
     * - an INT8_ARRAY field containing the number of bytes the polled RTclient
     * has received for each message type on its connection to RTserver
     * There are multiple responses only if client_name is a wildcard or
     * T_IPC_MON_ALL.
     * To poll once for global
     * message traffic information, use clientMsgTrafficPoll. To
     * poll once for message traffic information
     * by subject, use clientSubjectPoll. To poll
     * once for global callback information, use clientCbPoll.
     * @see clientSubjectPoll, clientCbPoll
     * @since SmartSockets 6.7.0
     */
    void clientMsgTypeExPoll(const char * clientName,
                             const char * msg_type_name)
    throw (TipcException);

    /**
     * Polls for RTclient callback information by sending a
     * MON_CLIENT_CB_POLL_CALL message to the
     * client_name RTclient. The client_name
     * argument can be the unique subject name of a single RTclient,
     * a wildcarded subject name to
     * match many RTclients, or T_IPC_MON_ALL to poll all RTclients
     * in the project that match
     * the value of the Monitor_Scope option. The polled
     * RTclient responds by sending back a
     * MON_CLIENT_CB_POLL_RESULT message. The response may or may not come back
     * quickly, depending on what the polled RTclient is doing.
     * The polling RTclient can use
     * methods such as TipcSrv::searchType() or TipcSrv::mainLoop
     * () to get the response.
     * Each MON_CLIENT_CB_POLL_RESULT message contains nine fields:
     * - a STR field containing the name of the RTclient
     * - an INT4 field containing the number of global read callbacks the
     * polled RTclient has on its connection to RTserver
     * - an INT4 field containing the number of global write callbacks the
     * polled RTclient has on its connection to RTserver
     * - an INT4 field containing the number
     * of global process callbacks the polled RTclient has
     * on its connection to RTserver
     * - an INT4 field containing the number of global queue callbacks the
     * polled RTclient has on its connection to RTserver
     * - an INT4 field containing the number of default callbacks the
     * polled RTclient has on its connection to RTserver
     * - an INT4 field containing the number of error callbacks the
     * polled RTclient has on its connection to RTserver
     * - an INT4 field containing the number
     * of server create callbacks the polled RTclient has
     * - an INT4 field containing the number
     * of server destroy callbacks the polled RTclient has
     * There are multiple responses
     * only if client_name is a wildcard or T_IPC_MON_ALL.
     */
    void clientCbPoll(const char * clientName) throw (TipcException);

    /**
     * Polls for RTclient message traffic information by sending a
     * MON_CLIENT_MSG_TRAFFIC_POLL_CALL message to the client_name RTclient. The
     * client_name argument can be the unique subject name of a single
     * RTclient, a wildcarded
     * subject name to match many RTclients, or T_IPC_MON_ALL to poll all
     * RTclients in the
     * project that match the value of the Monitor_Scope option. The polled
     * RTclient responds by
     * sending back a MON_CLIENT_MSG_TRAFFIC_POLL_RESULT message. The response
     * may or may not come back quickly, depending on what the polled
     * RTclient is doing. The
     * polling RTclient can use methods such as TipcSrv::searchType() or
     * TipcSrv::mainLoop() to
     * get the response.
     *
     * Each MON_CLIENT_MSG_TRAFFIC_POLL_RESULT message contains five fields:
     * - a STR field containing the name of the RTclient\n
     * - an INT4 field containing the number of messages the polled RTclient
     *   has sent on its connection to RTserver\n
     * - an INT4 field containing the number of messages the polled RTclient
     *   has received on its connection to RTserver\n
     * - an INT4 field containing the number of bytes the polled RTclient has
     *   sent on its connection to RTserver\n
     * - an INT4 field containing the number of bytes the polled RTclient has
     *   received on its connection to RTserver\n
     *
     * There are multiple responses only if client_name is a wildcard or
     * T_IPC_MON_ALL.
     * To poll once for message traffic information by message type, use
     * clientMsgTypePoll. To poll once for message traffic information by
     * subject, use
     * clientSubjectPoll.
     * To poll once for RTserver message traffic information, use
     * serverMsgTrafficPoll().
     * @see clientSubjectPoll, serverMsgTrafficPoll
     */
    void clientMsgTrafficPoll(const char * clientName)
    throw (TipcException);

    /**
     * Polls for RTclient subject information by sending a
     * MON_CLIENT_SUBJECT_POLL_CALL message to the client_name RTclient. The
     * client_name argument can be the unique subject name of a single
     * RTclient, a wildcarded subject name to match many RTclients, or
     * T_IPC_MON_ALL to poll all RTclients in the project that match the value
     * of the Monitor_Scope option.  The polled RTclient responds by
     * sending back a MON_CLIENT_SUBJECT_POLL_RESULT message. The response may
     * or may not come back quickly, depending on what the polled RTclient is
     * doing. If subject_name is T_IPC_MON_ALL, then all subjects ever used
     * in the polled RTclient are polled. The polling RTclient can
     * use methods such as TipcSrv::searchType() or
     * TipcSrv::mainLoop() to get the response.
     * Each MON_CLIENT_SUBJECT_POLL_RESULT message contains one fixed
     * field followed by one or more groups of five fields. The fixed
     * field is a STR field containing the name of the
     * RTclient. The group of five fields is:
     * - a STR field containing the name of the subject
     * - an INT4 field containing the number of messages the polled RTclient
     *   has sent to this subject on its connection to RTserver
     * - an INT4 field containing the number of messages the polled RTclient
     *   has received for this subject on its connection to RTserver
     * - an INT4 field containing the number of bytes the polled RTclient
     *   has sent to this subject on its connection to RTserver
     * - an INT4 field containing the number of bytes the polled RTclient has
     *   received for this subject on its connection to RTserver
     *
     * There are multiple responses only if client_name is a wildcard or
     * T_IPC_MON_ALL.  To poll once for global message traffic information,
     * use clientMsgTrafficPoll(). To poll once for message traffic
     * information by message type, use clientMsgTypePoll().
     *
     * @note The traffic statistics presented in the result message from this
     * poll are truncated to 31 bits, and may contain an incorrect sign bit on
     * some older servers. This method is provided for compatibility with
     * pre-6.7 servers. When polling newer servers, you should use
     * clientSubjectExPoll.
     * @see clientSubjectExPoll, clientMsgTrafficPoll, clientMsgTypePoll
     */
    void clientSubjectPoll(const char * clientName,
                           const char * subject_name) throw (TipcException);

    // New in 6.7 (this poll only)
    /**
     * Polls for RTclient subject information by sending a
     * MON_CLIENT_SUBJECT_POLL_CALL message to the client_name RTclient. The
     * client_name argument can be the unique subject name of a single
     * RTclient, a wildcarded subject name to match many RTclients, or
     * T_IPC_MON_ALL to poll all RTclients in the project that match the value
     * of the Monitor_Scope option.  The polled RTclient responds by
     * sending back a MON_CLIENT_SUBJECT_EX_POLL_RESULT message. The response
     * may or may not come back quickly, depending on what the polled RTclient
     * is doing. If subject_name is T_IPC_MON_ALL, then all subjects ever used
     * in the polled RTclient are polled. The polling RTclient can
     * use methods such as TipcSrv::searchType() or
     * TipcSrv::mainLoop() to get the response.
     * Each MON_CLIENT_SUBJECT_POLL_RESULT message contains one fixed
     * field followed by five array fields. The fixed
     * field is a STR field containing the name of the
     * RTclient. The five arrays are:
     * - a STR_ARRAY field containing the name of the subjects
     * - an INT8_ARRAY field containing the number of messages the polled
     *   RTclient has sent to each subject on its connection to RTserver
     * - an INT8_ARRAY field containing the number of messages the polled
     *   RTclient has received for each subject on its connection to RTserver
     * - an INT8_ARRAY field containing the number of bytes the polled RTclient
     *   has sent to each subject on its connection to RTserver
     * - an INT8_ARRAY field containing the number of bytes the polled RTclient
     *   has received for each subject on its connection to RTserver
     *
     * There are multiple responses only if client_name is a wildcard or
     * T_IPC_MON_ALL.  To poll once for global message traffic information,
     * use clientMsgTrafficPoll(). To poll once for message traffic
     * information by message type, use clientMsgTypePoll().
     * @see clientMsgTrafficPoll, clientMsgTypePoll
     * @since SmartSockets 6.7.0
     */
    void clientSubjectExPoll(const char * clientName,
                             const char * subject_name) throw (TipcException);

    // new APIs implemented in SS 6.4

    /**
     * clientNamesNumPoll polls for the number of RTclients in the
     * current project that match the value of the monitor_scope option by
     * sending a MON_CLIENT_NAMES_NUM_POLL_CALL message to the RTserver.
     * The RTserver responds by sending back a MON_CLIENT_NAMES_NUM_POLL_RESULT
     * message.
     *
     * The MON_CLIENT_NAMES_NUM_POLL_RESULT message will have the
     * following grammar:
     *
     * int4 - number of clients
     *
     * The resulting message may have 0 for the number of clients, if no
     * clients match the value of monitor_scope.
     */
    void clientNamesNumPoll() throw (TipcException);

    /**
     * clientSubscribeNumPoll polls for the number of subjects
     * subscribed to by an RTclient by sending a
     * MON_CLIENT_SUBSCRIBE_NUM_POLL_CALL message to the RTserver. The
     * client_name argument can be the unique subject name of a single
     * RTclient, a wildcarded subject name to match many RTclients, or
     * T_IPC_MON_ALL to poll all RTclients in the project that match
     * the value of the monitor_scope option.  The RTserver responds by
     * sending back a MON_CLIENT_SUBSCRIBE_NUM_POLL_RESULT message.
     *
     * The MON_CLIENT_SUBSCRIBE_NUM_POLL_RESULT message will have the
     * following grammar:
     *
     * str_array - list of client names
     * int4_array - number of subscribes per client
     *
     * The resulting message may contain arrays of zero length if no
     * clients match the client_name argument.
     * @throw TipcMonException if an error occurs.
     */
    void clientSubscribeNumPoll(const char * client_name)
    throw (TipcException);

    /**
     * clientCpuPoll polls for an RTclient's CPU utilization by
     * sending a MON_CLIENT_CPU_POLL_CALL message to the client_name
     * RTclient. The client_name argument can be the unique subject name
     * of a single RTclient, a wildcarded subject name to match many
     * RTclients, or T_IPC_MON_ALL to poll all RTclients in the project
     * that match the value of the monitor_scope option. The polled
     * RTclient responds by sending back a MON_CLIENT_CPU_POLL_RESULT message.
     *
     * The MON_CLIENT_CPU_POLL_RESULT will have the following grammar:
     *
     * str - Client name
     * real4 - Percent CPU utilization (between 0.0 and 100.0)
     *
     * There may be multiple responses if client_name is wildcarded or
     * T_IPC_MON_ALL.  If there are no clients that match client_name then
     * the poll request is dropped by the RTserver and no responses will be
     * received.
     *
     * A value contained in the percent CPU utilization field is less than
     * zero indicates that the client cannot query its OS for this
     * information, or that the client has not yet implemented this
     * functionality.
     * @throw TipcMonException if an error occurs.
     */
    void clientCpuPoll(const char * client_name)
    throw (TipcException);

    /**
     * serverCpuPoll polls for a RTserver's CPU utilization by sending
     * a MON_SERVER_CPU_POLL_CALL message to the server_name RTserver. The
     * server_name argument can be the unique subject name of a single
     * RTserver, a wildcarded subject name to match many RTservers, or
     * T_IPC_MON_ALL to poll all RTservers that match the value of the
     * monitor_scope option. The polled RTservers respond by sending back a
     * MON_SERVER_CPU_POLL_RESULT message.
     *
     * The MON_SERVER_CPU_POLL_RESULT will have the following grammar:
     *
     * str - Server name
     * real4 - Percent CPU utilization (between 0.0 and 100.0)
     *
     * There may be multiple responses if server_name is wildcarded or
     * T_IPC_MON_ALL.  If there are no servers that match server_name then
     * the poll request is dropped by the RTserver and no responses will be
     * received.
     * A value contained in the percent CPU utilization field is less than
     * zero indicates that the server cannot query its OS for this
     * information, or that the server has not yet implemented this
     * functionality.
     * @throw TipcMonException if an error occurs.
     */
    void serverCpuPoll(const char * server_name)
    throw (TipcException);

    /**
     * clientInfoPoll polls for RTclient information by sending a
     * MON_CLIENT_INFO_POLL_CALL message to the RTserver. The client_name
     * argument can be the unique subject name of a single RTclient, a
     * wildcarded subject name to match many RTclients, or T_IPC_MON_ALL to
     * poll all RTclients in the project that match the value of the
     * monitor_scope option.  The RTserver responds by sending back a
     * MON_CLIENT_INFO_POLL_RESULT message.
     * The MON_CLIENT_INFO_POLL_RESULT message will have the following grammar:
     *
     * str - client_name
     * str -   ident
     * str -   node_name
     * str -   user_name
     * int4 -   pid
     * str -   project
     * str -   server_name
     * str -   arch
     * int4 -   current_sbrk
     * int4 -   sbrk_delta_since_startup
     * int2 -   int_format
     * int2 -   real_format
     * str  -   logical_conn_name_to_server
     * int4 -  number of subscribes for client
     * real4 -  Percent CPU utilization (between 0.0 and 100.0) for client
     * @throw TipcMonException if an error occurs.
     */
    void clientInfoPoll(const char * client_name)
    throw (TipcException);


    /**
     * clientCongestionSetWatch is used to initiate and terminate the
     * T_MT_MON_CLIENT_BUFFER_HIGH_WATER and T_MT_MON_CLIENT_BUFFER_LOW_WATER
     * watches.  The watch_status argument indicates whether the watch is being
     * initiated or terminated.  If the watch is being initiated, the
     * client_name argument indicates the name of the client whose write
     * buffer is being watched.  If the watch is being terminated, the
     * client_name argument indicates the name of the client whose write buffer
     * is to no longer be watched.  The high_water and low_water arguments
     * specify the high and low water marks for the buffers being monitored.
     * The srv argument in clientCongestionSetWatch indicates the connection
     * on which the watch is to be set.
     * @throw TipcMonException if an error occurs.
     */
    void clientCongestionSetWatch(const char * client_name, int4 high_water,
                                  int4 low_water,  bool watch_status) throw (TipcException);


    /**
     * serverCongestionSetWatch is used to initiate and terminate the
     * T_MT_MON_SERVER_BUFFER_HIGH_WATER and T_MT_MON_SERVER_BUFFER_LOW_WATER
     * watches.  The watch_status argument indicates whether the watch is
     * being initiated or terminated.  If the watch is being initiated,
     * the server_name argument indicates the name of the server to set the
     * watch, and the connected_process_name argument indicates the name of
     * the process whose write buffer is to be watched.  If the watch is
     * being terminated, the connected_process_name argument indicates the
     * name of the process whose write buffer is to no longer be watched.
     * The high_water and low_water arguments specify the high and low water
     * marks for the buffers being monitored.  The srv argument in
     * serverCongestionSetWatch indicates the connection on which the watch
     * is to be set.
     * @throw TipcMonException if an error occurs.
     */
    void serverCongestionSetWatch(const char * server_name,
                                  const char * connected_process_name, int4 high_water,
                                  int4 low_water,  bool watch_status) throw (TipcException);

    /**
     * serverMaxClientLicensesSetWatch is the method that will be
     * called to initiate and terminate the T_MT_MON_MAX_CLIENT_LICENSES
     * watch.  The server_name argument indicates the name of the server
     * to set the watch.  The watch_status argument indicates whether the
     * watch is being initiated or terminated. The srv argument in
     * serverMaxClientLicensesSetWatch indicates the connection on which
     * the watch is to be set.
     * @throw TipcMonException if an error occurs.
     */
    void serverMaxClientLicensesSetWatch(const char * server_name,
                                         bool watch_status) throw (TipcException);


    /**
     * clientCongestionGetWatchis used to determine if a
     * T_MT_MON_CLIENT_BUFFER_HIGH_WATER and T_MT_MON_CLIENT_BUFFER_LOW_WATER
     * watch is currently active for a given server process.  The client_name
     * argument indicates the client for which the watch is being checked.
     * The srv argument in clientCongestionGetWatch indicates the connection
     * on which the watch is to be checked.
     * @return true if the watch is currently active, false if not.
     * @throw TipcMonException if an error occurs.
     */
    bool clientCongestionGetWatch(const char * client_name)
    throw (TipcException);

    /**
     * serverCongestionGetWatch is used to determine if a
     * T_MT_MON_SERVER_BUFFER_HIGH_WATER and T_MT_MON_SERVER_BUFFER_LOW_WATER
     * watch is currently active for a given server process.  The server_name
     * argument indicates the server for which the watch is being checked.
     * The connected_process_name argument indicates for which process the
     * watch status is being determined.
     * The srv argument in serverCongestionGetWatch indicates the connection
     * on which the watch is to be checked.
     *
     * @return true if the watch is currently active, false if not.
     * @throw TipcMonException if an error occurs.
     */
    bool serverCongestionGetWatch(const char * server_name,
                                  const char * connected_process_name) throw (TipcException);

    /**
     * serverMaxClientLicensesGetWatchis used to determine if a
     * T_MT_MON_MAX_CLIENT_LICENSES watch is currently active for a given
     * server.  The server_name argument indicates the server for which the
     * watch is being checked.
     * The srv argument in TipcSrvMonServerMaxClientLicensesGetWatch indicates
     * the connection on which the watch is to be checked.
     *
     *
     * @return true if the watch is currently active, false if not.
     * @throw TipcMonException if an error occurs.
     */
    bool serverMaxClientLicensesGetWatch(const char * server_name)
    throw (TipcException);

    /**
     * clientExtPoll polls for RTclient extension information by sending a
     * MON_CLIENT_EXT_POLL_CALL message to the RTserver. The client_name
     * argument can be the unique subject name of a single RTclient, a
     * wildcarded subject name to match many RTclients, or T_IPC_MON_ALL to
     * poll all RTclients in the project that match the value of the
     * monitor_scope option.  The RTserver responds by sending back a
     * MON_CLIENT_EXT_POLL_RESULT message.
     *
     * @throw TipcMonException if an error occurs.
     */
    void clientExtPoll(const char * client_name)
    throw (TipcException);

    // new APIs implemented in SS 6.7

    /**
     * Polls for RTserver start time information by sending a
     * MON_SERVER_START_TIME_POLL_CALL message to the server_name RTserver. The
     * server_name argument can be the unique subject name of
     * a single RTserver, a wildcarded
     * subject name to match many RTservers, or T_IPC_MON_ALL
     * to poll all RTservers that
     * match the value of the Monitor_Scope option. The polled RTserver
     * responds by sending
     * back a MON_SERVER_START_TIME_POLL_RESULT message. The response should come back
     * quickly. The polling RTclient can use methods
     * such as TipcSrv::searchType() or TipcSrv::mainLoop() to get the response.
     *
     * Each MON_SERVER_START_TIME_POLL_RESULT message contains three fields:
     * - a STR field containing the name of the RTserver
     * - a REAL8 field containing the start time of the RTserver
     * - a REAL8 field containing the number of seconds elapsed since RTserver was started
     *
     * There are multiple responses only if server_name is a wildcard
     * or T_IPC_MON_ALL.
     *
     * @since SmartSockets 6.7.0
     */
    void serverStartTimePoll(const char * serverName)
    throw (TipcException);

    /**
     * serverVersionPoll polls for a RTserver's version by sending
     * a MON_SERVER_VERSION_POLL_CALL message to the server_name RTserver. The
     * server_name argument can be the unique subject name of a single
     * RTserver, a wildcarded subject name to match many RTservers, or
     * T_IPC_MON_ALL to poll all RTservers that match the value of the
     * monitor_scope option. The polled RTservers respond by sending back a
     * MON_SERVER_SERVER_POLL_RESULT message.
     *
     * The MON_SERVER_VERSION_POLL_RESULT will have the following grammar:
     *
     * str - Server name
     * int4 - Version number
     *
     * There may be multiple responses if server_name is wildcarded or
     * T_IPC_MON_ALL.  If there are no servers that match server_name then
     * the poll request is dropped by the RTserver and no responses will be
     * received.
     * @throw TipcMonException if an error occurs.
     * @since SmartSockets 6.7.0
     */
    void serverVersionPoll(const char * server_name)
    throw (TipcException);

    /**
     * clientVersionPoll polls for an RTclient's version by
     * sending a MON_CLIENT_VERSION_POLL_CALL message to the client_name
     * RTclient. The client_name argument can be the unique subject name
     * of a single RTclient, a wildcarded subject name to match many
     * RTclients, or T_IPC_MON_ALL to poll all RTclients in the project
     * that match the value of the monitor_scope option. The polled
     * RTclient responds by sending back a MON_CLIENT_VERSION_POLL_RESULT message.
     *
     * The MON_CLIENT_VERSION_POLL_RESULT will have the following grammar:
     *
     * str - Client name
     * int4 - Client version
     *
     * There may be multiple responses if client_name is wildcarded or
     * T_IPC_MON_ALL.  If there are no clients that match client_name then
     * the poll request is dropped by the RTserver and no responses will be
     * received.
     *
     * @throw TipcMonException if an error occurs.
     * @since SmartSockets 6.7.0
     */
    void clientVersionPoll(const char * client_name)
    throw (TipcException);

  };

} //namespace SmartSockets

#endif // _T_MONITOR_HH_
