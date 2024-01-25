/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tsrv.h#1 $
 */

#ifndef _T_SERVER_HH_
#define _T_SERVER_HH_

#include <rtworks/ipc.h>
#include <rtworks/types.h>
#include <rtworks/tex.h>
#include <rtworks/tconn.h>
#include <rtworks/tcallbck.h>
//#include <rtworks/txlcl.h>

namespace SmartSockets {

  /**
   * Inherits from TipcException so users can trap TipcSrv related
   * exceptions. Offers no additional functionality.
   *
   * @see TipcException, TipcSrv
   */
  class T_IPCX_API TipcSrvException : public TipcException {

  public:
    /**
     * Constructor.
     */
    TipcSrvException () {}

    /**
     * Destructor.
     */
    virtual ~TipcSrvException () throw() {}
  };

  /**
   * Traverser class to be used by TipcSrv::traverseSubj().
   */
  class T_IPCX_API TipcSrvSubjTraverser {
  public:

    /**
     * Signature for the traverse method.
     * @param subjectName the subject entry being traversed.
     * @return true to continue traversing; false otherwise.
     */
    typedef bool (TipcSrvSubjTraverser::* Handler)
    (
    const char* subjectName
    ) ;



    TipcSrvSubjTraverser();

    /**
     * Sets the current handler method; if this is not called, the default
     * method onTraverse() is called.
     */
    void setHandler(Handler cbMethod);

    /**
     * Gets the current handler method.
     */
    Handler getHandler();


    /**
     * Destructor.
     */
    virtual ~TipcSrvSubjTraverser() throw();

    /**
     * Default traverse method.
     * @param subjectName name of the subject being traversed.
     * @return true to continue traversing; false otherwise.
     */
    virtual bool onTraverse
    (
    const char*     subjectName
    )
    throw () ;

    static  T_PTR T_ENTRY traverseFuncDelegator
    (
    T_STR subj,
    T_STR dummy,
    T_PTR arg
    );

  private:

    Handler _cbMethod;

  };

  /**
   * Provides the interfaces for a client to connect to an RTserver process.
   * The constructors allow one to either explicitly choose some of the
   * connection
   * parameters, or to create a named connection, which picks up its settings
   * from
   * the SmartSockets command "setnopt"
   * (e.g. Utilities::parseCommandString("setnopt c1 server_names
   * tcp:_node:2222");
   * would set the server_names property of a connection whose name is c1).
   */
  class T_IPCX_API TipcSrv : public TipcConnClient {

  private:
    T_IPC_SRV _srv;

  protected:
    /**
     * Delegator method for subject callbacks.  Will be called when the
     * callback is triggered and will in turn call the callback's handler
     * method.
     *
     * @param connection the connection on which the callback was triggered
     * @param data the subject callback data (contains the message to which
     *             the callback applies)
     *
     * @param arg the callback object
     */
    static void T_ENTRY1 subjectCbDelegator(T_IPC_CONN connection,
                                            T_IPC_SRV_SUBJECT_CB_DATA data,
                                            T_CB_ARG arg);

    /**
     * Delegator method for open connection callbacks.  Will be called when
     * the callback is triggered and will in turn call the callback's handler
     * method.
     *
     * @param connection the connection on which the callback was triggered
     * @param data the connection callback data (contains the message to which
     *             connection status information for which the callback
     *             applies)
     * @param arg the callback object
     */
    static void T_ENTRY1 openCbDelegator(T_IPC_SRV connection,
                                         T_IPC_SRV_OPEN_CB_DATA data,
                                         T_CB_ARG arg);


    /**
     * Delegator method for close connection callbacks.  Will be called when
     * the callback is triggered and will in turn call the callback's handler
     * method.
     *
     * @param connection the connection on which the callback was triggered
     * @param data the connection callback data (contains the message to which
     *             connection status information for which the callback
     *             applies)
     * @param arg the callback object
     */
    static void T_ENTRY1 closeCbDelegator(T_IPC_SRV connection,
                                          T_IPC_SRV_CLOSE_CB_DATA data,
                                          T_CB_ARG arg);

  public:

    /**
     * Creates a connection object but doesn't establish a connection.  To
     * establish a connection, please see TipcSrv::open().
     * @param unique_subject is always used as the sender property of a message
     * sent to RTserver through a given connection. When a connection is first
     * opened to
     * RTserver, it automatically subscribes to its unique subject. RTservers
     * in a cloud
     * do not allow multiple processes to have the same unique subject.
     * @param server_names list of rtserver logical connection names following
     * the format: <protocol>:<node>:<port>.  This should match the RTserver's
     * conn_names option. If set to NULL, this option defaults to
     * 'tcp:_node:5101' on Windows and 'local:_node' on Solaris.
     * @param project is a self-contained unit or partition that prevents
     * unwanted messages from being sent to processes, in that connections in
     * different projects cannot
     * send messages to each other.  A connection belongs to only one project,
     * while an
     * RTserver can provide publish-subscribe routing services for one or more
     * projects.  If set to NULL, this option defaults to 'rtworks'.
     * @param subject_prefix specifies the qualifier to prepend to message
     * subject.
     * names that don't start with /. Subject names are organized in a
     * hierarchical namespace
     * where the components are delimited by /. A subject name that starts
     * with / is called an
     * absolute subject name. All non-absolute subject names have
     * Default_Subject_Prefix prepended to them.  If set to NULL, this option
     * defaults to '/'.
     * @throw TipcSrvException if an error occurs.
     * @par Example:
     * The following example creates two RTserver connections:
     * - # Connection 1 will connect to an RTserver (rt1) with its
     * conn_names option set to 'tcp:10.134.1.11:4444', with a fail-over
     * backup RTserver (rt2) with its conn_names option set to
     * 'tcp:10.134.1.12:3333'.  This connection will use the unique subject
     * '/foo' and the project 'bar', with a default subject prefix of '/'.
     * - # Connection 2 will connect to an RTserver running on the local
     * machine listening to a conn_names option set to 'tcp:_node'.  This
     * connection will use the default unique subject (_<machine>_<pid>), the
     * default project (rtworks), and a default_subject_prefix of '/leo/'.
     * @code
     * #include <rtworks/sscpp.h>
     * using namespace SmartSockets;
     * int main()
     * {
     *   try
     *   {
     *     TipcSrv c1 ("/foo",
     *                 "tcp:10.134.1.11:4444,tcp:10.134.1.12:3333",
     *                 "foo");
     *     TipcSrv c2 ((const char*)0,(const char*)0,(const char*)0,"/leo/");
     *     c1.open();
     *     c2.open();
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     * @endcode
     */
    TipcSrv(const char * unique_subject,
            const char * server_names,  /* comma delimited list */
            const char * project = (const char*) T_NULL,
            const char * subject_prefix = (const char*) T_NULL)
    throw (TipcException);

    /**
     * Creates a "named" connection object. This connection
     * will use "named" option values.
     * @par Example:
     * The following example creates two RTserver connections:
     * - # Connection 1 will connect to an RTserver (rt1) with its
     * conn_names option set to 'tcp:10.134.1.11:4444', with a fail-over
     * backup RTserver (rt2) with its conn_names option set to
     * 'tcp:10.134.1.12:3333'.  This connection will use the unique subject
     * '/foo' and the project 'bar', with a default subject prefix of '/'.
     * - # Connection 2 will connect to an RTserver running on the local
     * machine listening to a conn_names option set to 'tcp:_node'.  This
     * connection will use the default unique subject (_<machine>_<pid>), the
     * default project (rtworks), and a default_subject_prefix of '/leo/'.
     * @code
     * #include <rtworks/sscpp.h>
     * using namespace SmartSockets;
     * int main()
     * {
     *   try
     *   {
     *     Utilities::parseCommandString("setnopt c1 unique_subject /foo;"
     *      "setnopt c1 server_names tcp:10.134.1.11:4444,tcp:10.134.1.12:3333;"
     *                                   "setnopt c1 project bar");
     *
     *     Utilities::parseCommandString(
     *                             "setnopt c2 default_subject_prefix /leo/");
     *
     *     TipcSrv c1 ("c1");
     *     TipcSrv c2 ("c2");
     *     c1.open();
     *     c2.open();
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     * @endcode
     * @param name - named option value
     * @throw TipcSrvException if the operation fails.
     */
    TipcSrv(const char * name) throw (TipcException);

    /**
     * constructor from the C type T_IPC_SRV.  This constructor is used
     * internally by the API.
     * @param srv C structure created with TipcSrvConnCreate().
     * @param destroyFlag flag that determines whether or not the destructor
     * destroys the C structure.
     */
    TipcSrv (T_IPC_SRV srv,bool destroyFlag = false) throw (TipcException);

    /**
     * Gets the C style T_IPC_CONN handle to the connection.  Note that a valid
     * T_IPC_CONN is only returned if a connection has been opened.
     * @throw TipcSrvException
     */
    operator T_IPC_CONN() throw (TipcException);

    /**
     * Gets the C style T_IPC_SRV handle to the connection.
     * @throw TipcSrvException
     */
    operator T_IPC_SRV () const throw (TipcException);


    /**
     * Closes a connection.  There are two ways to close the
     * connection to RTserver. A full closure removes all the connection's
     * RTserver-related
     * information. Once this has occurred, the connection can continue as if
     * it had never been connected
     * at all to RTserver. The other mode of close is a warm closure. A
     * warm closure
     * keeps as much RTserver-related information as possible. Once this has
     * occurred, the connection
     * can easily reconnect to RTserver in the future and pick up where it left
     * off. After a warm
     * destroy, the connection can even partially operate as if it were still
     * connected to RTserver.
     * Messages can be sent, and they are buffered until a full connection to
     * RTserver is created
     * again.
     * @param close_status - whether to keep a warm connection to RTserver (use
     * T_IPC_SRV_CONN_WARM) or fully destroy the connection to RTserver (use
     * T_IPC_SRV_CONN_NONE)
     * throw TipcSrvException if the operation fails.
     */
    void close(T_IPC_SRV_CONN_STATUS close_status = T_IPC_SRV_CONN_NONE)
    throw (TipcException);

    ///Destructor
    virtual ~TipcSrv() throw();

    /**
     * Opens a connection. A connection can be openned as a full connection or
     * as a warm connection.  A full connection allows the application to send
     * or receive SmartSockets TipcMsg objects.  A warm connection allows
     * clients to buffer TipcMsg objects in memory before a full connection is
     * established.
     * @param create_status - determines whether to create a warm connection
     * (use T_IPC_SRV_CONN_WARM) or a full connection (use T_IPC_SRV_CONN_FULL).
     * The default is T_IPC_SRV_CONN_FULL
     * @throw TipcSrvException
     */
    void open(T_IPC_SRV_CONN_STATUS create_status = T_IPC_SRV_CONN_FULL)
    throw (TipcException);

    /**
     * Checks if data can be read from or written to connection.  This is a low
     * level method that is only used in advanced publish and subscribe
     * applications.  This method is normally called by flush() and read().
     * @warning This method may attempt to automatically create a connection
     * to RTserver.  If the connection has a warm state, check() sleeps for
     * timeout seconds and returns false.
     * @return whether or not the data is ready.
     * @param check_mode - value for the check mode.  Use  T_IO_CHECK_READ if
     * data can be read or T_IO_CHECK_WRITE to check if data can be written.
     * @param timeout - number of seconds to wait for a result.
     * @throw TipcSrvException if an error occurs.
     */
    virtual bool check(T_IO_CHECK_MODE check_mode, real8 timeout)
    throw (TipcException);

    /**
     * Flushes buffered outgoing messages on the connection.
     * @warning This method may attempt to automatically create a connection
     * to RTserver. If the connection has a warm state, an exception is thrown
     * immediately.
     * @throw TipcSrvException if no connection exists or if a network error
     * occurs.
     */
    virtual void flush() throw (TipcException);


    /**
     * Gets the auto_flush_size of a connection to the RTserver. The auto
     * flush size is used to control how much outgoing data
     * (in bytes) are buffered before being automatically flushed to RTserver.
     * The default for the auto flush size is 8192 bytes.
     * @return the auto flush size that is set.
     * @throw TipcSrvException if the operation fails.
     */
    virtual int4 getAutoFlushSize() const throw (TipcException);

    /**
     * Sets the auto_flush_size of a connection to the RTserver. The auto
     * flush size is used to control how much outgoing data
     * (in bytes) are buffered before being automatically flushed to RTserver.
     * The default for the auto flush size is 8192 bytes.  If set to
     * T_IPC_NO_AUTO_FLUSH, data will never be flushed.
     * @param auto_flush_size - the number of bytes after which data will be
     * sent down the wire.
     * @throw TipcSrvException if the operation fails.
     */
    virtual void setAutoFlushSize(int4 auto_flush_size)
    throw (TipcException);

    /**
     * Gets the total number of bytes received on a connection.\n
     *
     * @return the total number of bytes received on a connection.
     * @throw TipcSrvException if the operation fails.
     *
     * @note The result is truncated to 31 bits, and may contain an incorrect
     * sign bit on some older clients. New clients should use
     * trafficGetBytesRecv8.
     * @see trafficGetBytesRecv8.
     */
    virtual int4 trafficGetBytesRecv() const throw (TipcException);

    /**
     * Gets the total number of bytes received on a connection.
     *
     * @return the total number of bytes received on a connection.
     * @throw TipcSrvException if the operation fails.
     */
    virtual int8 trafficGetBytesRecv8() const throw (TipcException);

    /**
     * Gets the total number of bytes written to the write buffer
     * on a connection.  This number usually is the same as the
     * total number of bytes actually sent on the connection, but
     * sometimes the retrieved number is larger than the number
     * of bytes sent when the write buffer is not frequently
     * flushed.
     *
     * @return the total number of bytes written to the write buffer
     *         on a connection.
     * @throw TipcSrvException if the operation fails.
     *
     * @note The result is truncated to 31 bits, and may contain an incorrect
     * sign bit on some older clients. New clients should use
     * trafficGetBytesSent8.
     * @see trafficGetBytesSent8.
     */
    virtual int4 trafficGetBytesSent() const throw (TipcException);

    /**
     * Gets the total number of bytes written to the write buffer
     * on a connection.  This number usually is the same as the
     * total number of bytes actually sent on the connection, but
     * sometimes the retrieved number is larger than the number
     * of bytes sent when the write buffer is not frequently
     * flushed.
     *
     * @return the total number of bytes written to the write buffer
     *         on a connection.
     * @throw TipcSrvException if the operation fails.
     */
    virtual int8 trafficGetBytesSent8() const throw (TipcException);

    /**
     * Gets the total number of messages received on a connection.
     *
     * @return the total number of messages received on a connection.
     * @throw TipcSrvException if the operation fails.
     *
     * @note The result is truncated to 31 bits, and may contain an incorrect
     * sign bit on some older clients. New clients should use
     * trafficGetMsgsRecv8.
     * @see trafficGetMsgsRecv8.
     */
    virtual int4 trafficGetMsgsRecv() const throw (TipcException);

    /**
     * Gets the total number of messages received on a connection.
     *
     * @return the total number of messages received on a connection.
     * @throw TipcSrvException if the operation fails.
     */
    virtual int8 trafficGetMsgsRecv8() const throw (TipcException);

    /**
     * Gets the total number of messages written to the write buffer
     * on a connection.  This number usually is the same as the
     * total number of messages actually sent on the connection, but
     * sometimes the retrieved number is larger than the number
     * of messages sent when the write buffer is not frequently
     * flushed.
     *
     * @return the total number of messages written to the write buffer
     *         on a connection.
     * @throw TipcSrvException if the operation fails.
     *
     * @note The result is truncated to 31 bits, and may contain an incorrect
     * sign bit on some older clients. New clients should use
     * trafficGetMsgsSent8.
     * @see trafficGetMsgsSent8.
     */
    virtual int4 trafficGetMsgsSent() const throw (TipcException);

    /**
     * Gets the total number of messages written to the write buffer
     * on a connection.  This number usually is the same as the
     * total number of messages actually sent on the connection, but
     * sometimes the retrieved number is larger than the number
     * of messages sent when the write buffer is not frequently
     * flushed.
     *
     * @return the total number of messages written to the write buffer
     *         on a connection.
     * @throw TipcSrvException if the operation fails.
     */
    virtual int8 trafficGetMsgsSent8() const throw (TipcException);

    /**
     * Gets the total number of bytes in a connection's read buffer.
     *
     * @return the total number of bytes in a connection's read buffer.
     * @throw TipcSrvException if the operation fails.
     */
    virtual int4 bufferGetReadSize() const throw (TipcException);

    /**
     * Gets the total number of bytes in a connection's write buffer.
     *
     * @return the total number of bytes in a connection's write buffer.
     * @throw TipcSrvException if the operation fails.
     */
    virtual int4 bufferGetWriteSize() const throw (TipcException);

    /**
     * Gets a timeout property from the connection to RTserver.
     * The timeout properties control how often (in seconds) the connection
     * to RTserver is checked for possible network failures.
     * The timeout properties of the connection to RTserver are also available
     * in the options
     * Server_Read_Timeout, Server_Write_Timeout, Server_Keep_Alive_Timeout, and
     * Server_Delivery_Timeout. For the connection to RTserver, the default read
     * timeout is 30.0
     * seconds, the default write timeout is 30.0 seconds, the default keep
     * alive timeout is 15.0
     * seconds, and the default delivery timeout is 30.0 seconds.
     * @param timeout Determines the type of timeout to retrieve:
     * - If timeout is T_IPC_TIMEOUT_READ, the read timeout of the connection is
     *   obtained.  check() uses the read timeout to control how often data
     *   is expected to
     *   be available for reading on the connection. A read timeout property of
     *   0.0 disables checking for read timeouts.
     * - If timeout is T_IPC_TIMEOUT_WRITE, the write timeout of the connection
     *   is obtained.  A write timeout property of 0.0 disables
     *   checking for write timeouts.
     * - If timeout is T_IPC_TIMEOUT_KEEP_ALIVE, the keep alive timeout of the
     *   connection is obtained.  A keep alive timeout
     *   property of 0.0 disables keep alives.
     * - If timeout is T_IPC_TIMEOUT_DELIVERY, the delivery timeout of the
     *   connection is obtained. A delivery timeout
     *   property of 0.0 disables checking for delivery timeouts.
     * @throw TipcSrvException if the operation fails.
     * @return the timeout which is set.
     */
    virtual real8 getTimeout(T_IPC_TIMEOUT timeout) const
    throw (TipcException);
    /**
     * Sets a timeout property from the connection to RTserver.
     * The timeout properties control how often (in seconds) the connection
     * to RTserver is checked for possible network failures.
     * The timeout properties of the connection to RTserver are also available
     * in the options
     * Server_Read_Timeout, Server_Write_Timeout, Server_Keep_Alive_Timeout, and
     * Server_Delivery_Timeout. For the connection to RTserver, the default read
     * timeout is 30.0
     * seconds, the default write timeout is 30.0 seconds, the default keep
     * alive timeout is 15.0
     * seconds, and the default delivery timeout is 30.0 seconds.
     * @param timeout Determines the type of timeout to retrieve:
     * - If timeout is T_IPC_TIMEOUT_READ, the read timeout of the connection is
     *   obtained.  check() uses the read timeout to control how often data
     *   is expected to
     *   be available for reading on the connection. A read timeout property of
     *   0.0 disables checking for read timeouts.
     * - If timeout is T_IPC_TIMEOUT_WRITE, the write timeout of the connection
     *   is obtained.  A write timeout property of 0.0 disables
     *   checking for write timeouts.
     * - If timeout is T_IPC_TIMEOUT_KEEP_ALIVE, the keep alive timeout of the
     *   connection is obtained.  A keep alive timeout
     *   property of 0.0 disables keep alives.
     * - If timeout is T_IPC_TIMEOUT_DELIVERY, the delivery timeout of the
     *   connection is obtained. A delivery timeout
     *   property of 0.0 disables checking for delivery timeouts.
     * @param value the number of seconds to be set for the specific timeout.
     * @throw TipcSrvException if the operation fails.
     * @return the timeout which is set.
     */
    virtual void setTimeout(T_IPC_TIMEOUT timeout, real8 value)
    throw (TipcException);

    /**
     * Gets the socket handle of the connection.  A socket is an operating
     * system device which provides a
     * communication link to another process. All data sent to and received from
     * the connection is transmitted on this socket.
     * @return the socket information
     * throw TipcSrvException if the socket information could not be retrieved
     */
    virtual sock getSocket() const throw (TipcException);

    /**
     * Gets the number of queued messages from the connection.  A connection
     * has a priority queue of incoming messages. Messages are inserted into the
     * queue either explicitly or implicitly.  Explicit insertions can be made
     * by calling the insert() method.  Implicit insertions happen whenever
     * data is read using the read() or next() methods.
     * @return the number of queued messages
     * @throw TipcSrvException
     */
    virtual int4 getNumQueued() const throw (TipcException);

    /**
     * Gets an XtAppAddInput-compatible source from a connection, which
     * allows SmartSockets connections to be mixed with Motif or any other
     * Xt-based widget set. Both Xt
     * and TipcSrv have their own main loop functions: XtAppMainLoop for Xt and
     * mainLoop() for TipcSrv. Since only one main loop function can be
     * used, it is easiest to use XtAppAddInput for incorporating other
     * sources of input into Xt's event-
     * handling mechanism. See the SmartSockets User's Guide for more
     * information on integrating SmartSockets with Motif/Xt.
     * @return source for XtAppAddInput
     * @throw TipcSrvException
     */
    virtual sock getXtSource() const throw (TipcException);

    /**
     * Sets the username and password for the RTclient when connecting to an
     * RTserver with the Basic Security Driver installed. The password is
     * immediately encrypted and is never sent in plain text to the RTserver.
     * @param username - the username to use for authentication and
     * authorization
     * @param password - the password to use for authentication
     * @throw TipcSrvException if the operation fails.
     */
    virtual void setUsernamePassword(const char * username,
                                     const char * password)
    throw (TipcException);

    /**
     * Checks if the server connection is still alive.  This method performs a
     * remote procedure call to check if the process at the other
     * end of a connection is still alive. This explicit attempt to check the
     * health of a connection is
     * called a keep alive. keepAlive() is normally used only by
     * checki(), which automatically checks the connection based on the read
     * timeout property of the connection.
     * keepAlive() can be called explicitly, though, to check the health
     * of the connection.  If the other end of the connection is alive, it
     * receives the KEEP_ALIVE_CALL message,
     * processes it with TipcCbConnProcessKeepAliveCall, and sends a
     * KEEP_ALIVE_RESULT message back to this end.
     * @throw TipcSrvException
     */
    virtual void keepAlive() throw (TipcException);

    /**
     * Reads and processes messages on a connection. This method is a
     * convenience function that keeps calling TipcConnMsgNext with the time
     * remaining from
     * timeout until next() throws an exception or times out. For each TipcMsg
     * that mainLoop() gets, it processes the message with process() and then
     * destroys the message.
     * @param timeout - number of seconds to look for messages and process them.
     * Use 0.0 for timeout to process one message.  Use T_TIMEOUT_DRAIN to poll
     * the connection and catch up on all
     * pending messages that have accumulated without reading any new ones.
     * Use T_TIMEOUT_FOREVER for timeout to read and process messages
     * indefinitely.
     * @throw TipcSrvException
     */
    virtual bool mainLoop(real8 timeout)
    throw (TipcException);

    /**
     * Inserts a message into the queue of the connection.
     * @param msg - message to be inserted
     * @param pos - position at which the message to be inserted.  Defaults to
     * T_IPC_POS_PRIORITY, which inserts the message in priority order, after
     * all other * message with equal or higher priority. insert() also calls
     * the connection's queue callbacks.
     * @throw TipcSrvException if the insertion fails
     */
    virtual void insert(TipcMsg& msg, int4 pos = T_IPC_POS_PRIORITY)
    throw (TipcException);


    /**
     * Retrieves the next message from the connection.
     * @param timeout - number of seconds to look for a message.
     * Use 0.0 for timeout to poll the connection once and get the next message
     * available.
     * Use the default of T_TIMEOUT_FOREVER for timeout to wait indefinitelly
     * for a new message.
     * @param msg - place holder object for return value.
     * @throw TipcSrvException if an error occurs, or the timeout has expired.
     */
    virtual void next(TipcMsg& msg, real8 timeout)
    throw (TipcException);

    /**
     * Retrieves the next message from the connection.
     * @param timeout - number of seconds to look for a message.
     * Use 0.0 for timeout to poll the connection once and get the next message
     * available.
     * @param msg - place holder object for return value.
     * @throw TipcSrvException if an error occurs.
     * @return false if the timeout is expired, true otherwise.
     */
    virtual bool nextEx(TipcMsg& msg, real8 timeout)
    throw (TipcException);


    /**
     * Processes a message in a connection by calling the process, subject
     * or default callbacks. The default callbacks are only called if there are
     * no non-global process
     * callbacks for the type of message being processed.
     * Processing a message with two levels of callbacks allows for more
     * flexibility. For example,
     * RTserver creates a few process callbacks for the message types it needs
     * to process locally
     * (such as SUBJECT_SET_SUBSCRIBE), but the majority of the message types
     * (such as
     * NUMERIC_DATA) are simply routed to their destination by a default
     * callback.
     * Any message can be processed by process(), not just messages
     * read from a
     * connection. For example, a generic message file data source could be
     * implemented using the extractor operator to
     * read messages from a file then the messages created can be processed
     * by calling process().
     * @param msg - message to be processed
     * @throw TipcSrvException
     */
    virtual void process(TipcMsg& msg) throw (TipcException);

    /**
     * Searches the message queue of the connection for a specific message.
     * The connection has a priority queue of incoming messages.  This method
     * traverses the messages in this internal priority queue calling the
     * TipcConnSearchSelector object's onSearch() method until either
     * TipcConnSearchSelector::onSearch() returns true, or the timeout period
     * expires.  If TipcConnSearchSelector::onSearch() returns true, the
     * message is removed from the queue and put into the msg parameter.  If
     * the end of the queue is reached, read() is called with the remaining
     * timeout.  If any new messages arrive, the queue is traversed again.
     * @param msg place holder object for return value.
     * @param timeout - the number of seconds this method will block waiting
     * for the selector object selObj to select a TipcMsg.
     * Use the default of T_TIMEOUT_FOREVER for timeout to wait indefinitelly
     * for a new message.
     * @param selObj - the selector object with user-defined criteria for
     * finding a message.  See  TipcConnSearchSelector for more details.
     * @return the message found as a result of the search via the argument
     * msg.
     * @throw TipcSrvException if an error occurs, or the timeout is reached.
     * @par Example:
     * The following example shows a selector that searches for an info
     * message type with the second letter in the first field = to r:
     * @code
     * #include <rtworks/sscpp.h>
     * using namespace SmartSockets;
     * class MyTrav: public TipcConnSearchSelector
     * {
     *   public:
     *     bool onSearch(const TipcConn & conn, const TipcMsg& msg) throw ()
     *     {
     *       try
     *       {
     *         if (msg.getTypeNum() == T_MT_INFO)
     *         {
     *           const char* data = msg.nextString();
     *           if (strlen(data) >= 2)
     *           {
     *             if (data[1] == 'r')
     *             {
     *               return true;
     *             }
     *           }
     *         }
     *       }
     *       catch (...)
     *       {
     *       }
     *       return false;
     *     }
     * };
     * int main ()
     * {
     *   try
     *   {
     *     TipcSrv srv ((const char*) 0, (const char*) 0);
     *     srv.open();
     *     srv.setSubscribe("/foo");
     *     MyTrav* trav = new MyTrav();
     *     TipcMsg msg;
     *     srv.search(msg,trav);
     *     msg.print();
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     *
     * @endcode
     */
    virtual void search(TipcMsg& msg, TipcConnSearchSelector* selObj,
                        real8 timeout)
    throw (TipcException);

    /**
     * Searches the message queue of the connection for a message with
     * a specific type.  This is a convenience method that uses the same
     * technique as search() with a specific selector object that looks for
     * messages of a particular type.
     * @param msg place holder object for return value.
     * @param mt - TipcMt object; message type
     * @param timeout -  the timeout value until which the search can be
     * performed
     * Use the default of T_TIMEOUT_FOREVER for timeout to wait indefinitelly
     * for a new message.
     * @return the message found as a result of the search via the argument
     * msg.
     * @throw TipcSrvException
     */
    virtual void searchType(TipcMsg& msg, TipcMt& mt,
                            real8 timeout)
    throw (TipcException);


    /**
     * Reads all available data from the connection to RTserver and queues
     * messages in priority order.  If the connection's auto flush size is not
     * T_IPC_NO_AUTO_FLUSH,
     * then read() first calls flush() to %flush the connection's
     * write buffer (so that
     * any responses to the outgoing data are available sooner). read()
     * then reads data (in
     * the form of message packets) from a connection's socket and calls the
     * connection decode
     * callbacks. If necessary, the integers and strings in the message header
     * are converted to the
     * formats used by the receiving process. read() then converts the
     * message packets
     * into messages, and calls the connection's %read callbacks. read()
     * returns as soon as at
     * least one message is %read, so that high-data-rate applications do not
     * sit forever reading data.
     * @param timeout - timeout value, until which the read operation
     * can be performed.
     * Use the default of T_TIMEOUT_FOREVER for timeout to wait indefinitelly
     * for a new message.
     * @throw TipcSrvException thrown if an error occurs.
     */
    virtual void read(real8 timeout)
    throw (TipcException);

    /**
     * Sends a TipcMsg through the connection.  If check_server_msg_send is
     * set to true and the option
     * Server_Msg_Send is false, then send() does not %send the message,
     * but returns
     * immediately. Otherwise send() first temporarily sets the sender of the
     * message (using the value of the option Unique_Subject), then temporarily
     * prepends the
     * value of the option Default_Subject_Prefix to the message sender and
     * destination if they do
     * not start with slash (/), and calls the appropriate write callbacks.
     *
     * If the message has a delivery mode of either T_IPC_DELIVERY_SOME or
     * T_IPC_DELIVERY_ALL, then TipcConnMsgSend performs some extra steps for
     * guaranteed
     * message delivery. If the connection does not have a GMD area open, then
     * send() calls gmdFileCreate() to create the necessary GMD area.
     * send() increments an internal per-connection sequence number, sets the
     * sequence number of the message to the incremented value, and then saves a
     * copy of the
     * message to the GMD area so that the message can be easily resent. With
     * file-based GMD,
     * recovery can take place even if this process crashes and has to be
     * restarted.  If the message delivery timeout property is not set, then
     * the connection delivery timeout value is used for GMD.
     *
     * send() then converts the message into a message packet (checking the
     * message
     * header string encode property to see if header strings should be encoded
     * as integers to save
     * space), executes the connection encode callbacks, and appends the message
     * packet to the
     * connection's write buffer. TipcConnMsgSend then calls TipcConnFlush if
     * the number of
     * bytes in the connection's write buffer is greater than the connection's
     * auto flush size.
     * @param msg - message to sent it through the RTserver
     * @param check_server_msg_send - flag for check server message send
     * @throw TipcConnException, TipcSrvException if an error occurs.
     */
    virtual void send(const TipcMsg& msg,
                      bool check_server_msg_send=true)
    throw (TipcException);

    /**
     * Makes a remote procedure call (RPC) with TipcMsg objects.
     * One message is sent as the RPC call from the caller end of the
     * connection, and
     * another message is sent back as the RPC result to the caller. The callee
     * end of the connection
     * must be prepared to receive the call message and send back the result
     * message. The
     * relationship between the call and result message is that the message type
     * number of the
     * result message is always one greater than the message type number of the
     * call message.
     *
     * sendRpc() sends the message with send(), flushes all pending
     * outgoing data with flush(), and then waits for the result with
     * searchType().
     * @param reply_msg - message received as a result of call_msg.
     * @param call_msg - message to be sent through RPC.
     * @param timeout - number of seconds to wait for a reply message.
     * @return the reply message via the reply_msg argument.
     * @throw TipcSrvException if an error occurs.
     * @par Example:
     * The following example shows the sender side of an RPC call:
     * @code
     * #include <rtworks/sscpp.h>
     * #include <iostream>
     * using namespace SmartSockets;
     * using namespace std;
     * int main()
     * {
     *   try
     *   {
     *     // create message types with the numbers differing by one; whenever
     *     // using sendRpc, the request type number has to be one less than
     *     // the reply.
     *     TipcMt reqMt = TipcMt::create("requestMt",444,"");
     *     TipcMt replyMt = TipcMt::create("replyMt",445,"verbose");
     *
     *     // create the reply and request messages.
     *     TipcMsg replyMsg(replyMt);
     *     TipcMsg requestMsg(reqMt);
     *
     *     // set the destination of the request message to a subject that
     *     // another connection will listen.
     *     requestMsg.setDest("/requests");
     *
     *     // Create a server connection
     *     TipcSrv srv("/uniqueSubj1", "tcp:_node");
     *     srv.open();
     *
     *     // call this method with a timeout forever.
     *     srv.sendRpc(replyMsg,requestMsg);
     *
     *     // print the contents of the message to standard out.
     *     replyMsg.print();
     *   }
     *   catch (Exception& e)
     *   {
     *     cout << "exception caught; error = " << e.what() << endl;
     *   }
     *   return 0;
     * }
     * @endcode
     */
    virtual void sendRpc(TipcMsg& reply_msg,const TipcMsg& call_msg,
                         real8 timeout = T_TIMEOUT_FOREVER)
    throw (TipcException);

    /**
     * Constructs a message and sends it through the connection.  This is a
     * convenience method that takes a null-terminated variable number of
     * field type-value pairs. The field type part of the pair is T_IPC_FT, and
     * the value part of the
     * pair corresponds to the arguments for the related TipcMsg::append()
     * methods (e.g. if the type is T_IPC_FT_REAL8, then the value part is a *
     * REAL8 number) or TipcMsg::set* methods (if
     * the type part is T_IPC_PROP_DELIVERY_MODE, then the value part is a
     * T_IPC_DELIVERY_MODE setting). Some field types, such as arrays, use two
     * arguments for the
     * value part.
     * Write() performs the following steps:
     *  - Create a message of type mt
     *  - Append the fields to the message
     *  - Set the message sender to the value of the option Unique_Subject
     *  - Set the message destination to dest
     *  - Send the message with TipcSrvMsgSend
     *  - Destroy the message with TipcMsgDestroy
     * Write() is a short wrapper function that calls TipcSrvMsgWriteVa.
     * If check_server_msg_send is true and the option Server_Msg_Send is false,
     * then TipcSrvMsgWrite does not send the message, but returns immediately.
     * For normal usage, check_server_msg_send should always be true. This
     * allows the user to globally
     * turn off all normal outgoing messages by setting the option
     * Server_Msg_Send to false.
     * This is useful for running two RTclients, the primary one with
     * Server_Msg_Send set to true
     * and the backup one with Server_Msg_Send set to false.
     *
     * @param dest - destination, where the messages to be sent
     * @param mt - the message type corresponding to the message
     * @param check_server_msg_send - flag to determine whether or not the
     * message is really sent.
     * @param var_arg_list - the va_list parameters.  The field type part of
     * the pair is a T_IPC_FT and the
     * value part of the pair corresponds to one of these arguments:
     * - The argument for the related TipcMsgAppend* function. For example, if
     *   the type part is T_IPC_FT_REAL8, then the value part is a REAL8 number.
     * - The argument for the related TipcMsgSet* function. For example, if the
     *   type part is T_IPC_PROP_DELIVERY_MODE, then the value part is
     *   a T_IPC_DELIVERY_MODE setting.
     * @throw TipcSrvException if an error occurs.
     * @par Example:
     * The following shows a short example that builds and sends a numeric
     * data message to  the subject "/primary/control":
     * @code
     *  ...
     * TipcSrv srv ("/foo", "tcp:_node");
     * TipcMt mt (T_MT_NUMERIC_DATA);
     * srv.write("primary/control", mt, true,
     *           T_IPC_FT_STR, "voltage1",
     *           T_IPC_FT_REAL8, 65.0,
     *           T_IPC_FT_STR, "current1",
     *           T_IPC_FT_REAL8, 10.335,
     *           NULL);
     * @endcode
     */
    //@{
    void write(const char * dest, TipcMt& mt,
               bool check_server_msg_send, ...)
    throw (TipcException);

    void writeVa(const char * dest, TipcMt& mt,
                 bool check_server_msg_send,
                 va_list var_arg_list) throw (TipcException);
    //@}

    /**
     * Prints information about the connection to standard out.
     * The output can also be redirected if the parameter func is passed
     * @param func - the method used to print the connection information.
     * This method should match the following signature:
     * void T_ENTRY <func> (char*, ...);
     * @throw TipcSrvException
     */
    virtual void print(T_OUT_FUNC func = (T_OUT_FUNC) TutOut)  const
    throw (TipcException);

    /**
     * Determines the unique subject of the connected server.
     * @return the RTserver's unique subject
     * @throw TipcSrvException
     */
    virtual const char * getPeerUniqueSubject() const
    throw (TipcException);

    /**
     * Determines the node name of the connected server.
     * @return the node information
     * @throw TipcSrvException
     */
    virtual const char* getNode() const throw (TipcException);

    /**
     * Determines the user name of the connected server.
     * @return the user information
     * @throw TipcSrvException
     */
    virtual const char* getUser() const throw (TipcException);

    /**
     * Determines the process ID of the connected server.
     * @return the Pid information
     * @throw TipcSrvException
     */
    virtual int4 getPid() const throw (TipcException);

    /**
     * Determine the architecture name of the connected server. The string
     * returned is equivalent to the value of the environment variable RTARCH in
     * the machine where the RTserver is used (e.g. sun4_solaris on Solaris,
     * i86_w32 on Intel Windows 32 boxes).
     * @warning The string returned should not be changed; it belongs to this
     * object.
     * @return the architecture name to the connected server
     * @throw TipcSrvException
     */
    virtual const char* getArch() const throw (TipcException);

    /**
     * Determines the status of the connection.  A connection status of
     * T_IPC_SRV_CONN_FULL indicates that the connection has been opened to
     * RTserver.  A connection status of T_IPC_SRV_CONN_WARM indicates that
     * the connection has a warm state.
     * connection to RTserver. A connection status of T_IPC_SRV_CONN_NONE
     * indicates that the
     * TipcSrv object does not have a connection to RTserver.
     * @return the enum value of the T_IPC_SRV_CONN_STATUS.
     * @throw TipcSrvException
     */
    virtual T_IPC_SRV_CONN_STATUS getStatus() const
    throw (TipcException);

    /**
     * Acquires exclusive access to the connection.  This method locks all of
     * the internal synchronization objects within the connection to
     * RTserver. This insures that the calling thread in a multi-threaded
     * process has exclusive
     * access to the connection to RTserver. lock() may be called by a
     * thread recursively.
     * @warning Every call to lock() must be balanced by a call to unlock().
     * @throw TipcSrvException if the lock cannot be acquired.
     */
    virtual void lock() throw (TipcException);

    /**
     * Releases exclusive access to the connection.  This method unlocks all
     * of the internal synchronization objects within the connection to
     * RTserver.
     * @warning Every call to lock() must be balanced by a call to unlock().
     * @throw TipcSrvException if the lock cannot be acquired.
     */
    virtual void unlock() throw (TipcException);

    /**
     * Determines the number of outgoing GMD messages still pending on
     * the connection.  This method gets the number of GMD messages published
     * to RTserver
     * that have not been acknowledged nor explicitly deleted from the GMD area.
     * Messages are
     * deleted from the GMD area when a GMD_ACK message is received and when a
     * GMD_FAILURE message is processed. TipcSrvGetGmdNumPending may attempt to
     * automatically create a connection to RTserver. If the connection's
     * state is warm, this method
     * gets the number of pending GMD messages from the warm connection.
     * @return the number of GMD messages pending
     * @throw TipcSrvException if an error occurs.
     */
    virtual int4 getGmdNumPending() const throw (TipcException);

    /**
     * Determines if this object is subscribing to a subject.  The
     * subscribe status controls whether the RTclient receives messages
     * published (sent) to that
     * subject. If the subscribe status of subject is true, the connection
     * receives messages with
     * subject as their destination that are sent to RTserver. To start or stop
     * subscribing to a subject, use setSubscribe().  To find out if this
     * connection is subscribing to a subject including load balancing
     * information, use getSubscribeLb().
     * @param subject - subject name
     * @return a boolean whether the client is subscribed to the subject
     * @throw TipcSrvException if an error occurs.
     */
    bool getSubscribe(const char * subject) throw (TipcException);

    /**
     * Starts or stops subscribing to a subject.  This method sets whether or
     * not this object is subscribing to a subject. The
     * subscribe status controls whether or not the connection receives messages
     * published (sent) to
     * that subject. If the subscribe status of subject is true, then the
     * connection receives messages
     * with subject as their destination that are sent to RTserver. To
     * determine if an connection is subscribing to a subject, use
     * getSubscribe().
     * @param subject - subject name
     * @param recv_status - the receive status flag (true to start
     * subscribing, false to stop subscribing).
     * @throw TipcSrvException
     */
    void setSubscribe(const char * subject, bool recv_status = true)
    throw (TipcException);

    /**
     * Traverses the list of subjects that connection is subscribing.  This
     * method invokes the TipcSrvSubjTraverser::onTraverse() method for each
     * entry in the subscription table, until either
     * TipcSrvSubjTraverser::onTraverse() returns false, or the table ends.
     * @param travObj - the traversal object to traverse through subjects
     * @throw TipcSrvException if an error occurs.
     * @par Example:
     * The following code traverses through all the subjects that this object
     * has subscribed and prints each one to the screen:
     * @verbatim
       TAL-SS-00088-I Connecting to project <rtworks> on <_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>
         /bar1
         /foo
       @endverbatim
     * @code
     * #include <rtworks/sscpp.h>
     * #include <iostream>
     * using namespace SmartSockets;
     * using namespace std;
     *
     * class MyTrav: public TipcSrvSubjTraverser
     * {
     *   public:
     *     MyTrav()
     *     {
     *       cout << "Subscribing to the following subjects:" << endl;
     *     }
     *     virtual bool onTraverse ( const char* subjectName)
     *     {
     *       cout << "  " << subjectName << endl;
     *       return true;
     *     }
     * };
     * int main()
     * {
     *   try
     *   {
     *     MyTrav* trav = new MyTrav();
     *     TipcSrv srv("/foo",0);
     *     srv.setSubscribe("/bar1");
     *     srv.traverseSubscribe(trav);
     *     delete trav;
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     * @endcode
     */
    void traverseSubscribe(TipcSrvSubjTraverser* travObj)
    throw (TipcException);

    /**
     * Adds a message type to a message file logging type on the connection to
     * RTserver. Each logging type has two options that can be used to log
     * incoming and outgoing
     * messages in that logging type. When the option is set (the option value
     * should be the name
     * of a file), then read callbacks (for incoming) or write callbacks (for
     * outgoing) are created for
     * all the logging type's message types in the connection to RTserver. More
     * than one option can use the same file:
     * - The logging type T_IPC_SRV_LOG_DATA, which is for variable value
     * message types such as
     * TIME and NUMERIC_DATA, is controlled by the options Log_In_Data and
     * Log_Out_Data.
     * - The logging type T_IPC_SRV_LOG_STATUS, which is for RThci messages
     * types such ALERT
     * and POPUP, is controlled by the options Log_In_Status and Log_Out_Status.
     * - The logging
     * type T_IPC_SRV_LOG_INTERNAL, which is for all other message types such as
     * SUBJECT_SET_SUBSCRIBE and SERVER_STOP_CALL, is controlled by the options
     * Log_In_Internal and Log_Out_Internal.
     * All of the standard SmartSockets message types are added automatically to
     * one logging
     * type.
     *
     * Message types can be added to multiple logging types.
     *
     * @warning The message logging methods should be called before TipcSrv conn
     * is created, so they are static too.
     * @param log_type - the T_IPC_MT_LOG_TYPE value (either
     * T_IPC_SRV_LOG_DATA, T_IPC_SRV_LOG_STATUS, or T_IPC_SRV_LOG_INTERNAL).
     * @param mt - the message type
     * @throw TipcSrvException
     */
    static void logAddMt(T_IPC_MT_LOG_TYPE log_type,
                         TipcMt& mt) throw (TipcException);


    /**
     * Removes a message type from a message file logging type on
     * the connection to
     * RTserver. Each logging type has two options that can be used to log
     * incoming and outgoing
     * messages in that logging type. When the option is set (the option value
     * should be the name
     * of a file), then read callbacks (for incoming) or write callbacks (for
     * outgoing) are created for
     * all the logging type's message types in the connection to RTserver. More
     * than one option can use the same file:
     * - The logging type T_IPC_SRV_LOG_DATA, which is for variable value
     * message types such as
     * TIME and NUMERIC_DATA, is controlled by the options Log_In_Data and
     * Log_Out_Data.
     * - The logging type T_IPC_SRV_LOG_STATUS, which is for RThci messages
     * types such ALERT
     * and POPUP, is controlled by the options Log_In_Status and Log_Out_Status.
     * - The logging
     * type T_IPC_SRV_LOG_INTERNAL, which is for all other message types such as
     * SUBJECT_SET_SUBSCRIBE and SERVER_STOP_CALL, is controlled by the options
     * Log_In_Internal and Log_Out_Internal.
     * All of the standard SmartSockets message types are added automatically to
     * one logging
     * type.
     *
     * Message types can be added to multiple logging types.
     * @warning The message logging methods should be called before TipcSrv conn
     * is created, so they are static too.
     * @param log_type - the T_IPC_MT_LOG_TYPE value (either
     * T_IPC_SRV_LOG_DATA, T_IPC_SRV_LOG_STATUS, or T_IPC_SRV_LOG_INTERNAL).
     * @param mt - the message type
     * @throw TipcSrvException
     */
    static void logRemoveMt(T_IPC_MT_LOG_TYPE log_type,
                            TipcMt& mt) throw (TipcException);


    /**
     * Starts or stops subscribing to all standard subjects.  This method sets
     * the subscribe status of the standard subjects by calling
     * setSubscribe() for each standard subject.  setSubscribe() is
     * often used to subscribe to subjects listed in a command file (e.g. if a
     * command file has the following:  setopt subjects "/foo", "/bar").
     * The following are standard subjects:
     * - subjects listed in the option Subjects
     * - _<node> - the node-specific subject, such as _workstation1
     * - _all - common subject for all RTclients
     * As each standard subject subscribe status is changed,
     * setSubscribe() prints
     * out an informational message to standard out.
     * This method also flushes the retrieval messages with flush() when it
     * is done.
     * @param subscribe_status - flag set to start or stop subscribing
     * throw TipcSrvException
     */
    void setSubscribeStd(bool subscribe_status = true)
    throw (TipcException);


    /**
     * Determines if a connection is subscribing to a subject, including
     * load balancing information.
     * @paran subject - subject name
     * @param lb_status_return - pointer to boolean indicating the load
     * balancing status
     * @return a boolean, whether the client is subscribed to a subject or not
     * @throw TipcSrvException if an error occurs.
     */
    bool getSubscribeLb(const char * subject, bool *lb_status_return)
    throw (TipcException);

    /**
     * Starts or stops subscribing to a subject, with or without
     * load balancing.
     * @param subject - subject name
     * @param subject_status - status of the subject
     * @param lb_status - boolean value of the load balancing status
     * @throw TipcSrvException
     */
    void setSubscribeLb(const char *  subject, bool subject_status = true,
                        bool lb_status = true) throw (TipcException);

    /**
     * Initializes load balancing accounting in RTserver for a
     * subject to which messages will be published.
     * When the RTserver connected to the publishing connection routes a
     * message for load
     * balancing, it keeps track of which connection should receive the message.
     *  If this RTserver is
     * not already tracking load balancing for subject when the first
     * published message arrives
     * from a connection, the RTserver must start a monitoring-like watch
     * mechanism between the
     * RTservers of all subscribing connections. This increases the scalability
     * of load balancing
     * because only the relevant RTservers dynamically exchange load balancing
     * information.  The
     * load balancing accounting continues until no messages have been published
     * with GMD or
     * load balancing for an interval specified by the Gmd_Publish_Timeout
     * option. The default
     * for Gmd_Publish_Timeout is 300 seconds.
     * The starting of this load balancing accounting is very fast
     * but not instantaneous.
     * TipcSrvSubjectLbInit is intended to be used by advanced publish-
     * subscribe applications that
     * wish to ensure that RTserver is primed and immediately ready
     * for load balancing
     * publish-subscribe routing.
     * TipcSrvSubjectLbInit is usually only used at initialization, but it can
     * be called at any time by
     * advanced applications which wish to initialize load balancing publishing
     * accounting. The
     * use of TipcSrvSubjectLbInit is strictly optional for load balancing
     * publishing; if it is not called the load balancing setup occurs when
     * the first GMD or load balancing message is
     * published to subject.
     * If RTserver is already tracking load balancing for subject,
     * RTserver immediately returns a
     * GMD_INIT_RESULT message.
     * @param subject - subject name
     * @throw TipcSrvException
     */
    void subjectLbInit(const char *subject) throw (TipcException);

    /**
     * Creates a guaranteed message delivery (GMD) area on the connection to
     * RTserver.
     * This is a low level method that is automatically called from open()
     * if a GMD areal doesn't exist; only advanced applications that want to
     * speed up the connection process should call this.
     * @throw TipcSrvException
     */
    virtual void gmdFileCreate() throw (TipcException);

    /**
     * Deletes from disk guaranteed message delivery (GMD) files for a
     * connection. TipcConnGmdFileDelete is useful for processes that wish to
     * completely delete any obsolete GMD information.
     * When a process using GMD creates a connection, one of two scenarios is
     * possible:
     *
     * - the process does not care about GMD information from a previous
     * connection with the
     * same GMD configuration and should thus call gmdFileDelete() to
     * delete the old files
     * - the process is recreating the connection after a network failure and
     * should thus call gmdResend() to resend all previously sent messages
     * that may have been lost when the connection was destroyed.
     *
     * TipcConnGmdFileDelete should be used carefully, as deleting GMD files
     * removes all record of old messages that were supposed to be delivered.
     * to RTserver.
     *
     * @warning Old GMD files must be removed before any messages with
     * guaranteed delivery are sent or received on the connection.
     * gmdFileDelete() does not delete the GMD files if they
     * are already open by this process and in use for the connection. The GMD
     * files are opened by gmdFileCreate().
     * @throw TipcSrvException
     */
    virtual void gmdFileDelete() throw (TipcException);

    /**
     * Resends all guaranteed messages after a delivery failure on the
     * connection to RTserver.
     * @warning Be very careful when using this method, as it may result in
     * severe amounts of network traffic.  This method should only be called
     * when a network error occurs, and not under any other circunstances;
     * if used while a connection to RTserver is still alive, it could cause a
     * storm of resent GMD messages.
     * @throw TipcSrvException
     */
    virtual void gmdResend() throw (TipcException);

    /**
     * Deletes a message from the local GMD area after a GMD failure on
     * the connection to RTserver.  This method is was intended to be used from
     * a GMD_FAILURE connection process callback to terminate GMD for the
     * message.
     *
     * When a GMD_FAILURE message is processed by a sender RTclient, one of
     * two scenarios is possible:
     * - the sender RTclient does not want to resend the message, and thus uses
     * TipcSrvGmdMsgDelete and TipcSrvGmdMsgServerDelete to terminate GMD for
     * the message
     * - the sender RTclient does want to resend the message, and thus takes
     * some kind of user-defined action to correct the problem (possibly using
     * gmdMsgStatus() to
     * poll RTserver for GMD status) and then uses TipcSrvGmdMsgResend to resend
     * the message.
     * @param msg - message to delete
     * @throw TipcSrvException
     */
    virtual void gmdMsgDelete(TipcMsg& msg) throw (TipcException);

    /**
     * Gets the GMD area's maximum size of the connection to RTserver.
     * A GMD area holds guaranteed message delivery information for both
     * incoming and outgoing messages. There are two types of
     * guaranteed message delivery: file-based GMD and memory-only GMD.
     *
     * For file-based GMD this limit is the maximum file size, and for
     * memory-only GMD this
     * limit is the maximum amount of memory allowed. The default GMD area
     * maximum size is
     * 0, which means that no GMD area maximum size limit checking is performed.
     * The
     * connection GMD area maximum size and the connection delivery timeout can
     * be used to
     * constrain the amount of system resources used for GMD by space and by
     * time.
     *
     * If the connection GMD area maximum size is exceeded, then no further
     * messages can be
     * sent with guaranteed delivery (send() throws an exception with the
     * SmartSockets error number to set to T_ERR_VAL_TOO_LARGE) until some
     * unacknowledged
     * previously sent messages are acknowledged.
     *
     * @return the maximum size of the GMD area in bytes
     * @throw TipcSrvException if an error occurs.
     */
    virtual uint4 getGmdMaxSize() const throw (TipcException);

    /**
     * Sets the GMD area's maximum size of the connection to RTserver.
     * A GMD area holds guaranteed message delivery information for both
     * incoming and outgoing messages. There are two types of
     * guaranteed message delivery: file-based GMD and memory-only GMD.
     *
     * For file-based GMD this limit is the maximum file size, and for
     * memory-only GMD this
     * limit is the maximum amount of memory allowed. The default GMD area
     * maximum size is
     * 0, which means that no GMD area maximum size limit checking is performed.
     * The
     * connection GMD area maximum size and the connection delivery timeout can
     * be used to
     * constrain the amount of system resources used for GMD by space and by
     * time.
     *
     * If the connection GMD area maximum size is exceeded, then no further
     * messages can be
     * sent with guaranteed delivery (send() throws an exception with the
     * SmartSockets error number to set to T_ERR_VAL_TOO_LARGE) until some
     * unacknowledged
     * previously sent messages are acknowledged.
     *
     * @param gmd_max_size - value to be set for the maximum GMD area
     * @throw TipcSrvException if an error occurs.
     */
    virtual void setGmdMaxSize(uint4 gmd_max_size) throw (TipcException);

    /**
     * Deletes a message in RTserver after a GMD failure on the
     * connection to RTserver.  This method sends a GMD_DELETE message
     * to RTserver. When RTserver routes a message for GMD, it keeps the message
     * in memory so
     * that it can easily and quickly resend the message to any receiving
     * RTclient that disconnects
     * and reconnects. This method informs RTserver to terminate GMD for the
     * message, which allows RTserver to reclaim the memory for the message.
     * This method is intended to be used from a GMD_FAILURE connection
     * process callback to terminate GMD for the message.
     *
     * When a GMD_FAILURE message is processed by a sender RTclient, one of
     * two scenarios is possible:
     *
     * - the sender RTclient does not want to resend the message, and thus uses
     * TipcSrvGmdMsgDelete and TipcSrvGmdMsgServerDelete to terminate GMD for
     * the message
     * - the sender RTclient does want to resend the message, and thus takes
     * some kind of user-defined action to correct the problem (possibly using
     * gmdMsgStatus() to
     * poll RTserver for GMD status) and then uses TipcSrvGmdMsgResend to resend
     * the message.
     * @param msg - message to delete
     * @throw TipcSrvException
     */
    void gmdMsgServerDelete(TipcMsg& msg) throw (TipcException);

    /**
     * Polls RTserver for GMD status of a message.  This method polls for the
     * GMD status of msg by sending a GMD_STATUS_CALL
     * message to RTserver. RTserver responds by sending back a
     * GMD_STATUS_RESULT
     * message. The response should come back very quickly. The polling RTclient
     * can use
     * functions such as searchType() to get the response.
     *
     * When RTserver routes a message for GMD, it keeps track of which RTclients
     * should receive
     * the message and the status of GMD to each of these RTclients.
     * gmdMsgStatus() is
     * intended to be used from a GMD_FAILURE connection process callback to
     * query GMD status for the message.
     * The GMD_STATUS_RESULT message contains four fields:
     * - an INT4 integer field containing the sequence number property of the
     * message
     * - a STR_ARRAY string array field containing the names of the RTclients
     * that have acknowledged successful delivery of the message
     * - a STR_ARRAY string array field containing the names of the RTclients
     * where GMD has failed for the message
     * - a STR_ARRAY string array field containing the names of the RTclients
     * where delivery is still pending for the message.
     *
     * If RTserver does not have an in-memory GMD record of msg, RTserver
     * silently ignores the GMD_STATUS_CALL message and no GMD_STATUS_RESULT
     * message is returned.  This indicates that one of these has happened:
     * - RTserver has fully delivered msg
     * - TipcSrvGmdMsgServerDelete was used to terminate GMD for the message
     * - RTserver never routed msg
     *
     * When a GMD_FAILURE message is processed by a sender RTclient, one of
     * two scenarios is possible:
     * - the sender RTclient does not want to resend the message, and thus uses
     * gmdMsgDelete() and gmdMsgServerDelete() to terminate GMD for the message
     * - the sender RTclient does want to resend the message, and thus takes
     * some kind of user-defined action to correct the problem (possibly using
     * gmdMsgStatus() to poll RTserver for GMD status) the message.
     * @param msg - message for which the status is requested
     * @throw TipcSrvException
     */
    void gmdMsgStatus(TipcMsg& msg) throw (TipcException);

    /**
     * Initializes GMD accounting in RTserver for a subject to
     * which messages will be published.
     * @param subject - subject name
     * @throw TipcSrvException
     */
    void subjectGmdInit(const char * subject) throw (TipcException);

    /**
     * Set the connections project.
     * @param project - project name to be set
     * @throw TipcSrvException
     */
    void setProject(const char* project)
    throw (TipcException);


    /**
     * Get the connections project.
     * @return the project name
     * @throw TipcSrvException
     */
    const char* getProject() throw (TipcException);

    /**
     * Set the connections server names. This may be a comma
     * delimited list of server names.
     * @param server_names - server names comma
     * delimited list of server names
     * @throw TipcSrvException
     */
    void setServerNames(const char* server_names)
    throw (TipcException);

    /**
     * Get the connections server names.
     * @param server_names - the connections server names
     * @throw TipcSrvException
     */
    void setServerNamesList(T_STR_LIST server_names_list)
    throw (TipcException);

    /**
     * Get the connections server names list.
     * @return the connections server names list
     * @throw TipcSrvException
     */
    T_STR_LIST getServerNamesList()
    throw (TipcException);

    /**
     * Set the connections default subject prefix.
     * @param default_subject_prefix - default subject prefix to be set to
     * @throw TipcSrvException
     */
    void setDefaultSubjectPrefix(const char* default_subject_prefix)
    throw (TipcException);

    /**
     * Get the connections default subject prefix
     * return the default subject prefix of the connection
     * @throw TipcSrvException
     */
    const char* getDefaultSubjectPrefix()
    throw (TipcException);

    /**
     * Set the connections unique subject.
     * @param unique_subject - the unique subject name
     * @throw TipcSrvException
     */
    void setUniqueSubject(const char * unique_subject)
    throw (TipcException);

    /**
     * Get the connections unique subject.
     * @return the unique subject name
     * @throw TipcSrvException
     */
    const char* getUniqueSubject()
    throw (TipcException);


    /**
     * Subscribes to a subject obtaining the last cached value from SmartCache.
     * See SmartCache's documentation for more information on how to use this
     * method.
     * @param subj_name - subject name
     * @param status - True To Subscribe or False To Unsubscribe
     * @param flags - This field is used to determine the caching
     * behaviour:
     * - use T_SUBJ_FLAG_SC_LAST_VALUE for last value caching functionality.
     * - use T_SUBJ_FLAG_SC_INITIAL_LOAD for force initial load functionality.
     * This mode is used if the value retrieved from the cache is a full refresh
     * of data rather than an incremental update. When a client initially
     * subscribes to a subject, the first message to be received for the
     * particular subject is the reply message from the cache. Any messages
     * published to the subject in the interim will be forwarded to the
     * subscriber after the reply message.  The subscription will behave
     * normally from that point onwards.
     * - use T_SUBJ_FLAG_SC_LATEST_MSG to get the latest message in the
     * subject.  In this mode, the client will subscribe to the subject and
     * will then request the last value from the SmartCache utility for that
     * subject.  If the client receives a message on this subject before the
     * SmartCache reply message, the SmartCache reply message will be ignored
     * and not passed on to the client.
     * @throw TipcSrvException
     */
    void setSubscribeCache(const char * subj_name, bool status, int4 flags)
    throw (TipcException);

    /**
     * Sets the connection's credentials.  This method allows any credential
     * mechanisms (e.g. Kerberos, certificates, user name and password, etc.)
     * to be used to authenticate a client application before it joins an
     * RTserver cloud.  The data used here will be passed to the appropriate
     * Security manager in place, and it's up to that process to grant or deny
     * access to the RTserver cloud.
     * @param auth_policy_id - authorization policy Id
     * @param auth_data - authorization data
     * @param auth_data_len - authorization data length
     * @throw TipcSrvException
     */
    void setCredentials(int4 auth_policy_id, void * auth_data,
                        int4 auth_data_len) throw (TipcException);

    /**
     * Creates a message queue callback.  Queue callbacks are
     * called when a message is inserted into or deleted from a connection's
     * queue of incoming messages.
     * Queue callbacks are useful for watching the messages that have been
     * read in by a connection, but not yet processed. There is some overlap
     * between read callbacks and queue callbacks.
     *
     * A queue callback is usually created for a specific message type and
     * connection. If the
     * message type is null (e.g. TipcMt nullMt((T_IPC_MT)NULL), then a
     * global queue callback is created for all
     * message types on that connection.
     *
     * @param mt the message type for the callback
     * @param implementor a message queue callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcSrvException if creation of the callback fails
     * @par Example:
     * The following example shows a queue callback that gets triggered for
     * incoming and outgoing messages:
     * @code
     * #include <rtworks/sscpp.h>
     * #include <iostream>
     * using namespace SmartSockets;
     * using namespace std;
     *
     * class MyQueueCb : public MessageQueueCallBack
     * {
     *   public:
     *     void onMessage (CallBack <MessageQueueCallBack> * callBack,
     *                      TipcMsg & message,
     *                      TipcConn & connection,
     *                      uint4 queuePos,
     *                      bool is_insert)
     *     {
     *       const char* direction = is_insert? "inserted": "dequeued";
     *       cout << "in onMessage(); direction = " << direction << endl;
     *     }
     * };
     * int main ()
     * {
     *   try
     *   {
     *     TipcSrv srv("/foo","tcp:_node");
     *     MyQueueCb* myCb = new MyQueueCb();
     *     srv.open();
     *     CallBack<MessageQueueCallBack>* cb = srv.queueCbCreate(
     *                                  TipcMt::lookup(T_MT_INFO),myCb,"s1");
     *
     *     TipcMsg msg(T_MT_INFO,"/foo");
     *     msg << "test";
     *     srv.send(msg);
     *     srv.mainLoop(10.0);
     *     // This should disable the callback.
     *     cb->destroy();
     *
     *     delete myCb;
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     * @endcode
     * Here's the output of this code:
     * @verbatim
       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>
       in onMessage(); direction = inserted
       in onMessage(); direction = dequeued
       @endverbatim
     *
     */
    virtual CallBack<MessageQueueCallBack> * queueCbCreate(
                                                          const TipcMt& mt,
                                                          MessageQueueCallBack * implementor,
                                                          void* arg = NULL)
    throw (TipcException);


    /**
     * Creates an open connection callback.  Open callbacks are called when
     * this object creates a warm or full connection to RTserver.
     *
     * @param implementor a connection callback implementation
     * @param arg optional argument data for the callback
     * @return the constructed callback object
     * @throw TipcSrvException if creation of the callback fails
     * @par Example:
     * The following example shows how to create an open callback that is
     * invoked once when the server connection is opened; the connection is
     * then deactivated and the connection is opened and closed again.
     * @code
     * #include <rtworks/sscpp.h>
     * #include <iostream>
     * using namespace SmartSockets;
     * using namespace std;
     *
     * class MyConnCb : public ConnectionCallBack
     * {
     *   public:
     *     virtual void onConnection (
     *        CallBack < ConnectionCallBack > * callBack,
     *        TipcSrv & connection,
     *        ConnStatus oldConnStatus,
     *        ConnStatus newConnStatus)
     *     {
     *       const char* arg = (const char*) callBack->getArgument();
     *       cout << " in conn CB open() - " << arg << endl;
     *     }
     * };
     * int main ()
     * {
     *   try
     *   {
     *     TipcSrv srv("/foo","tcp:_node");
     *     MyConnCb* myCb = new MyConnCb();
     *     CallBack<ConnectionCallBack>* cb = srv.openCbCreate(myCb,"s1");
     *     srv.open();
     *
     *     srv.close();
     *
     *     // This should disable the callback.
     *     cb->destroy();
     *     srv.open();
     *     srv.close();
     *     delete myCb;
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     * @endcode
     * Here is the output for the code:
     * @verbatim

       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>
        in conn CB open() - s1
       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>

       @endverbatim
     */
    virtual CallBack <ConnectionCallBack> * openCbCreate(
                                                        ConnectionCallBack * implementor,
                                                        void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a close connection callback.  Close callbacks are called when
     * this object either loses or explicitly closes a connection to RTserver.
     *
     * @param implementor a connection callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcSrvException if creation of the callback fails
     * @par Example:
     * The following example shows how to create a close callback that is
     * invoked once when the server connection is closed, and then
     * deactivated.
     * @code
     * #include <rtworks/sscpp.h>
     * #include <iostream>
     * using namespace SmartSockets;
     * using namespace std;
     *
     * class MyConnCb : public ConnectionCallBack
     * {
     *   public:
     *     virtual void onConnection (
     *        CallBack < ConnectionCallBack > * callBack,
     *        TipcSrv & connection,
     *        ConnStatus oldConnStatus,
     *        ConnStatus newConnStatus)
     *     {
     *       const char* arg = (const char*) callBack->getArgument();
     *       cout << " in conn CB close() - " << arg << endl;
     *     }
     * };
     * int main ()
     * {
     *   try
     *   {
     *     TipcSrv srv("/foo","tcp:_node");
     *     MyConnCb* myCb = new MyConnCb();
     *     CallBack<ConnectionCallBack>* cb = srv.closeCbCreate(myCb,"s1");
     *     srv.open();
     *
     *     srv.close();
     *
     *     // This should disable the callback.
     *     cb->destroy();
     *     srv.open();
     *     srv.close();
     *     delete myCb;
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     * @endcode
     * Here is the output for the code:
     * @verbatim

       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>
        in conn CB close() - s1
       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>

       @endverbatim
     */
    virtual CallBack <ConnectionCallBack> * closeCbCreate(
                                                         ConnectionCallBack * implementor,
                                                         void* arg = NULL)
    throw (TipcException);


    /**
     * Creates an error callback.  ErrorCallBack objects are invoked
     * whenever this object loses a connection to RTserver.
     * @param implementor an error callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcSrvException if creation of the callback fails
     * @par Example:
     * The following example shows how to create an error callback that is
     * activated by killing the RTserver.
     * @code
     * #include <rtworks/sscpp.h>
     * #include <iostream>
     * using namespace SmartSockets;
     * using namespace std;
     *
     * class MyErrCb : public ErrorCallBack
     * {
     *   public:
     *     virtual void onError (
     *        CallBack < ErrorCallBack > * callBack,
     *        Exception& e,
     *        TipcConn &conn)
     *     {
     *       const char* arg = (const char*) callBack->getArgument();
     *       cout << " in error cb() - " << arg " " << e.what() << endl;
     *     }
     * };
     * int main ()
     * {
     *   try
     *   {
     *     TipcSrv srv("/foo","tcp:_node");
     *     MyErrCb* myCb = new MyErrCb();
     *     srv.open();
     *
     *     CallBack<ErrorCallBack>* cb = srv.errorCbCreate(myCb,"s1");
     *     System::runCommand("rtserver -stop > output");
     *     srv.mainLoop(10.0);
     *
     *     cb->destroy();
     *     delete myCb;
     *   }
     *   catch (Exception& e)
     *   {
     *     cout << "exception caught: " << e.what() << endl;
     *   }
     *   return 0;
     * }
     * @endcode
     * Here is the output for the code:
     * @verbatim

       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>
       TAL-SS-00104-W Lost connection to RTserver: error code = 10
       TAL-SS-00071-I Waiting before reconnecting
       TAL-SS-00097-I Attempting to reconnect to RTserver
       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00090-I Could not connect to <tcp:_node> RTserver
       TAL-SS-00093-I Skipping starting <tcp:_node> RTserver
       TAL-SS-00098-I Cannot reconnect to RTserver
        in error cb() - s1 server not connected
       exception caught: end of file

       @endverbatim
     */
    virtual CallBack <ErrorCallBack> * errorCbCreate(
                                                    ErrorCallBack * implementor,
                                                    void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a callback that is invoked before the TipcMtEncrypt encryption
     * callbacks and before the write callbacks are invoked.
     * EncryptionCallBack objects are invoked
     * when a message is buffered to be sent through the connection's socket,
     * which occurs in send(). These objects are invoked before the write
     * callback objects are invoked.
     *
     * After these objects are invoked, the message is encrypted using the
     * encryption object registered by TipcMt::setEncryptObj() and then the
     * write callbacks are invoked.
     *
     * Encrypt callbacks are useful for processing of messages before
     * they are encrypted and sent.
     * An encrypt callback is usually created for a specific message type and
     * connection. If the
     * message type is null (e.g. TipcMt nullMt((T_IPC_MT)NULL);), then global
     * encrypt callback is created for all message types on that
     * connection.
     *
     * @warning After a message has been encrypted, it is put into a
     * T_MT_ENCRYPT_MESSAGE as a binary payload; therefore, no write callback
     * objects will be triggered for that message type.
     * @param mt the message type for the callback
     * @param implementor an encryption callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcSrvException if creation of the callback fails
     * @par Example:
     * The following example shows an encrypt callback that gets triggered for
     * outgoing messages:
     * @code
     * #include <rtworks/sscpp.h>
     * #include <iostream>
     * using namespace SmartSockets;
     * using namespace std;
     *
     * class MyCb : public MessageCallBack
     * {
     *   public:
     *     void onMessage (CallBack <MessageCallBack> * cb,
     *                      TipcMsg & message,
     *                      TipcConn & connection)
     *     {
     *       cout << "in onMessage(); - (Encrypt Cb) " << endl;
     *     }
     * };
     * int main ()
     * {
     *   try
     *   {
     *     TipcSrv srv("/foo","tcp:_node");
     *     MyCb* myCb = new MyCb();
     *     srv.open();
     *     TipcMt mt(T_MT_INFO);
     *     mt.setKey((const void*)"a",2);
     *     CallBack<MessageCallBack>* cb = srv.encryptCbCreate(mt, myCb);
     *
     *     TipcMsg msg(T_MT_INFO,"/foo");
     *     msg << "test";
     *     srv.send(msg);
     *     srv.flush();
     *     // This should disable the callback.
     *     cb->destroy();
     *
     *     delete myCb;
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     * @endcode
     * Here's the output of this code:
     * @verbatim
       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>
       in onMessage(); - (Encrypt Cb)

       @endverbatim
     *
     */
    virtual CallBack <MessageCallBack> * encryptCbCreate(
                                                        const TipcMt & mt,
                                                        MessageCallBack * implementor,
                                                        void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a callback that is called after all the read callback objects are
     * called and after the TipcMtEncrypt object is called.  The Decrypt
     * callback objects are called when a new message is read from the
     * connection's socket, which occurs in the read(). First, the %read
     * callbacks are called. Then the message is decrypted. Finally,
     * the decrypt callbacks are called. Decrypt callbacks are useful for
     * processing messages, after
     * they have been decrypted, to do the exact opposite of what was done
     * during the encrypt
     * callbacks.
     * A decrypt callback is usually created for a specific message type and
     * connection. If the
     * message type is null, then global decrypt callback is created for all
     * message types on that
     * connection.
     *
     * @param mt the message type for the callback
     * @param implementor a decryption callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcSrvException if creation of the callback fails
     * @par Example:
     * The following example shows an encrypt callback that gets triggered for
     * outgoing messages:
     * @code
     * #include <rtworks/sscpp.h>
     * #include <iostream>
     * using namespace SmartSockets;
     * using namespace std;
     *
     * class MyCb : public MessageCallBack
     * {
     *   public:
     *     void onMessage (CallBack <MessageCallBack> * cb,
     *                      TipcMsg & message,
     *                      TipcConn & connection)
     *     {
     *       cout << "in onMessage(); - (Decrypt Cb) " << endl;
     *     }
     * };
     * int main ()
     * {
     *   try
     *   {
     *     TipcSrv srv("/foo","tcp:_node");
     *     MyCb* myCb = new MyCb();
     *     srv.open();
     *     TipcMt mt(T_MT_INFO);
     *     mt.setKey((const void*)"a",2);
     *     CallBack<MessageCallBack>* cb = srv.decryptCbCreate(mt, myCb);
     *
     *     TipcMsg msg(T_MT_INFO,"/foo");
     *     msg << "test";
     *     srv.send(msg);
     *     srv.mainLoop(10.0);
     *     // This should disable the callback.
     *     cb->destroy();
     *
     *     delete myCb;
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     * @endcode
     * Here's the output of this code:
     * @verbatim
       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>
       in onMessage(); - (Decrypt Cb)

       @endverbatim
     *
     */
    virtual CallBack <MessageCallBack> * decryptCbCreate(
                                                        const TipcMt & mt,
                                                        MessageCallBack * implementor,
                                                        void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a default callback that is invoked if no other process
     * callbacks are called.  Default callback objects are
     * called by process() when there are no process callbacks for the
     * type of message being processed.
     *
     * Default callbacks are useful for processing unexpected
     * message types or for generic processing of most message types.
     *
     * @param implementor a messagen callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcSrvException if creation of the callback fails
     * @par Example:
     * The following example shows a default, a process and a subject
     * callbacks.
     * outgoing messages:
     * @code
     * #include <rtworks/sscpp.h>
     * #include <iostream>
     * using namespace SmartSockets;
     * using namespace std;
     *
     * class MyCb : public MessageCallBack
     * {
     *   public:
     *     void onMessage (CallBack <MessageCallBack> * cb,
     *                      TipcMsg & msg,
     *                      TipcConn & connection)
     *     {
     *       const char* arg = (const char*) cb->getArgument();
     *       cout << "in onMessage(); - (" << arg << "); "
     *            << "message type = <" << msg.getType().getName()
     *            << "> " << endl;
     *     }
     * };
     * int main ()
     * {
     *   try
     *   {
     *     TipcSrv srv("/foo","tcp:_node");
     *     MyCb* myCb = new MyCb();
     *     srv.open();
     *     srv.setSubscribe("/bar");
     *     TipcMt mt(T_MT_INFO);
     *     CallBack<MessageCallBack>* cb1 = srv.defaultCbCreate(myCb,"default");
     *
     *     CallBack<MessageCallBack>* cb2 = srv.processCbCreate(mt,
     *                                                         myCb,"process");
     *
     *     TipcMt nullMt((T_IPC_MT)NULL);
     *     CallBack<MessageCallBack>* cb3 = srv.subjectCbCreate("/fo*",nullMt,
     *                                                         myCb,"subject");
     *
     *
     *     TipcMsg msg(T_MT_INFO,"/foo");
     *     msg << "test";
     *     // trigger the process and subject callbacks.
     *     srv.send(msg);
     *
     *     msg.setType(TipcMt::lookup(T_MT_NUMERIC_DATA));
     *     msg.setDest("/bar");
     *     // trigger the default callback.
     *     srv.send(msg);
     *
     *     srv.mainLoop(10.0);
     *     // This should disable the callback.
     *     cb1->destroy();
     *     cb2->destroy();
     *     cb3->destroy();
     *
     *     delete myCb;
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     * @endcode
     * Here's the output of this code:
     * @verbatim

       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>
       in onMessage(); - (process); message type = <info>
       in onMessage(); - (subject); message type = <info>
       in onMessage(); - (default); message type = <numeric_data>

       @endverbatim
     */
    virtual CallBack <MessageCallBack> * defaultCbCreate(
                                                        MessageCallBack * implementor,
                                                        void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a process callback that is triggered after a call to process().
     * Process callback
     * objects are triggered by a call to process() to perform the normal
     * processing of a message, such as
     * storing numeric data values and then acting on the variables.
     * A process callback is usually created for a specific message type and
     * connection. If the
     * message type is null (TipcMt nullMt((T_IPC_MT)NULL);), then a global
     * process callback is created for all message types on that connection.
     *
     * @param mt the message type for the callback
     * @param implementor a messagen callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcSrvException if creation of the callback fails
     * @see defaultCbCreate() for an example that shows this method being used.
     */
    virtual CallBack <MessageCallBack> * processCbCreate(
                                                        const TipcMt & mt,
                                                        MessageCallBack * implementor,
                                                        void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a read callback.  Read callback objects are called
     * when a new message is received from the connection's socket, which occurs
     * whenever read() is called. Connection read callbacks are useful for
     * message file logging and
     * preprocessing of messages before the normal processing with process().
     *
     * A read callback is usually created for a specific message type and
     * connection. If the message
     * type is null (e.g. TipcMt nullMt((T_IPC_MT) NULL) ), then a global read
     * callback is created for all message types on that connection.
     *
     * @param mt the message type for the callback
     * @param implementor a messagen callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcSrvException if creation of the callback fails
     * @par Example:
     * The following example shows an read callback that gets triggered for all
     * outgoing messages:
     * @code
     * #include <rtworks/sscpp.h>
     * #include <iostream>
     * using namespace SmartSockets;
     * using namespace std;
     *
     * class MyCb : public MessageCallBack
     * {
     *   public:
     *     void onMessage (CallBack <MessageCallBack> * cb,
     *                      TipcMsg & message,
     *                      TipcConn & connection)
     *     {
     *       cout << "in onMessage(); - (read Cb) "
     *            << message.getType().getName() << endl;
     *     }
     * };
     * int main ()
     * {
     *   try
     *   {
     *     TipcSrv srv("/foo","tcp:_node");
     *     MyCb* myCb = new MyCb();
     *     srv.open();
     *     TipcMt mt((T_IPC_MT)NULL);
     *     CallBack<MessageCallBack>* cb = srv.readCbCreate(mt, myCb);
     *
     *     TipcMsg msg(T_MT_INFO,"/foo");
     *     msg << "test";
     *     srv.send(msg);
     *     srv.mainLoop(10.0);
     *     // This should disable the callback.
     *     cb->destroy();
     *
     *     delete myCb;
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     * @endcode
     * Here's the output of this code:
     * @verbatim
       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>
       in onMessage(); - (read Cb) info

       @endverbatim
     *
     */

    virtual CallBack <MessageCallBack> * readCbCreate(
                                                     const TipcMt & mt,
                                                     MessageCallBack * implementor,
                                                     void* arg = NULL)
    throw (TipcException);

    /**
     * Creates a write callback.  Write callback objects are called
     * when a message is buffered to be sent through a connection's socket,
     * which occurs when send() is called. Connection write callbacks are
     * useful for message file logging and
     * processing of messages before they are actually sent.
     *
     * A write callback is usually created for a specific message type and
     * connection. If the
     * message type is null, then a global write callback is created for all
     * message types on that connection.
     *
     * @param mt the message type for the callback
     * @param implementor a messagen callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcSrvException if creation of the callback fails
     * @par Example:
     * The following example shows a write callback that gets triggered for all
     * outgoing messages:
     * @code
     * #include <rtworks/sscpp.h>
     * #include <iostream>
     * using namespace SmartSockets;
     * using namespace std;
     *
     * class MyCb : public MessageCallBack
     * {
     *   public:
     *     void onMessage (CallBack <MessageCallBack> * cb,
     *                      TipcMsg & message,
     *                      TipcConn & connection)
     *     {
     *       cout << "in onMessage(); - (write Cb) "
     *            << message.getType().getName() << endl;
     *     }
     * };
     * int main ()
     * {
     *   try
     *   {
     *     TipcSrv srv("/foo","tcp:_node");
     *     MyCb* myCb = new MyCb();
     *     srv.open();
     *     TipcMt mt((T_IPC_MT)NULL);
     *     CallBack<MessageCallBack>* cb = srv.writeCbCreate(mt, myCb);
     *
     *     TipcMsg msg(T_MT_INFO,"/foo");
     *     msg << "test";
     *     srv.send(msg);
     *     srv.mainLoop(10.0);
     *     // This should disable the callback.
     *     cb->destroy();
     *
     *     delete myCb;
     *   }
     *   catch (...)
     *   {
     *   }
     *   return 0;
     * }
     * @endcode
     * Here's the output of this code:
     * @verbatim
       TAL-SS-00088-I Connecting to project <rtworks> on <tcp:_node> RTserver
       TAL-SS-00089-I Using tcp protocol
       TAL-SS-00091-I Message from RTserver: Connection established.
       TAL-SS-00096-I Start subscribing to subject </foo>
       in onMessage(); - (write Cb) info

       @endverbatim
     *
     */
    virtual CallBack <MessageCallBack> * writeCbCreate(
                                                      const TipcMt & mt,
                                                      MessageCallBack * implementor,
                                                      void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a subject callback.  Subject callback objects are called by
     * a call to process() to %process a message when it is received by the
     * subject. If subject is
     * null, then func is called for all messages of type mt received by this
     * connection. If mt is null,
     * then the MessageCallBack object is called for every type of message
     * received by subj.
     * If both subject and mt are null (TipcMt nullMt((T_IPC_MT)NULL);),
     * the MessageCallBack object is called for every message received by this
     * connection.
     *
     * Subject wildcards (* or ...) are supported.
     *
     * @param subject the message subject for the callback
     * @param mt the message type for the callback
     * @param implementor a messagen callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcSrvException if creation of the callback fails
     * @see defaultCbCreate() for an example that shows this method being used.
     */
    virtual CallBack <MessageCallBack> * subjectCbCreate(
                                                        const char * subject,
                                                        const TipcMt & mt,
                                                        MessageCallBack * implementor,
                                                        void* arg = NULL)
    throw (TipcException);

  };

} // namespace SmartSockets

#endif //_T_SERVER_HH_
