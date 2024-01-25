/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tconn.h#1 $
 */

#ifndef _T_CONN_HH_
#define _T_CONN_HH_

#include <rtworks/ipc.h>
#include <rtworks/tcallbck.h>

namespace SmartSockets {

  class T_IPCX_API TipcMsg;
  class T_IPCX_API TipcMt;

  /**
   * Inherits from TipcException so users can trap TipcConn related
   * exceptions. Offers no additional functionality.
   *
   * @see TipcException, TipcConn
   */
  class T_IPCX_API TipcConnException : public TipcException {

  public:
    /**
     * Constructor.
     */
    TipcConnException () {};

    /**
     * Destructor.
     */
    virtual ~TipcConnException () throw() {}

  };


  /**
   * Selector class to be used by TipcConn::search().
   */
  class T_IPCX_API TipcConnSearchSelector {
  public:

    /**
     * Signature for the search method.
     * @param conn - connection currently being searched.
     * @param msg -  current message message.
     * @return true to stop searching; false otherwise.
     */
    typedef bool (TipcConnSearchSelector::* Handler)
    (
    const TipcConn & conn,
    const TipcMsg& msg
    ) ;



    TipcConnSearchSelector();

    /**
     * Sets the current handler method; if this is not called, the default
     * method onSearch() is called
     */
    void setHandler(Handler cbMethod);

    /**
     * Gets the current handler method.
     */
    Handler getHandler();


    /**
     * Destructor.
     */
    virtual ~TipcConnSearchSelector() throw();

    /**
     * Default search method.
     * @param conn - connection currently being searched.
     * @param msg -  current message message.
     * @return true to stop searching; false otherwise.
     */
    virtual bool onSearch
    (
    const TipcConn & conn,
    const TipcMsg& msg
    )  throw () ;

    static  T_BOOL T_ENTRY searchFuncDelegator
    (
    T_IPC_CONN connection,
    T_IPC_MSG  msg,
    T_PTR arg
    );

  private:

    Handler _cbMethod;

  };

  /**
   * TipcConn - user class for a dummy connection and base class for all
   * other connection classes
   *
   * The TipcConn class calls the C API function TipcConnCreate at
   * construction. It manages the resulting T_IPC_CONN object internally.
   */
  class T_IPCX_API TipcConn {

  protected:
    ///C style T_IPC_CONN handle to the connection.
    T_IPC_CONN _connection;

    bool _destroy_flag;

    /**
     * Constructor
     * @param do_nothing - if this is FALSE, it creates a connection
     */
    TipcConn(bool do_nothing);


    /**
     * Delegator method for error callbacks.  Will be called when the
     * callback is triggered and will in turn call the callback's handler
     * method.
     *
     * @param connection the connection on which the callback was triggered
     * @param data the subject callback data (does not really apply for
     *             error callbacks, but all delegator methods need this
     *             same signature)
     * @param arg the callback object
     */
    static void T_ENTRY1 errorCbDelegator(T_IPC_CONN connection,
                                          T_IPC_CONN_ERROR_CB_DATA data,
                                          T_CB_ARG arg);


    /**
     * Delegator method for message queue callbacks.  Will be called when the
     * callback is triggered and will in turn call the callback's handler
     * method.
     *
     * @param connection the connection on which the callback was triggered
     * @param data the message queue callback data (contains the message
     *             for which the callback applies and the message's
     *             position and insertion information)
     * @param arg the callback object
     */
    static void T_ENTRY1 queueCbDelegator(T_IPC_CONN connection,
                                          T_IPC_CONN_QUEUE_CB_DATA data,
                                          T_CB_ARG arg);


    /**
     * Delegator method for message callbacks.  Will be called when the
     * callback is triggered and will in turn call the callback's handler
     * method.
     *
     * @param connection the connection on which the callback was triggered
     * @param data the message queue callback data (contains the message
     *             for which the callback applies)
     * @param arg the callback object
     */
    static void T_ENTRY1 messageCbDelegator(T_IPC_CONN connection,
                                            T_IPC_CONN_MSG_CB_DATA data,
                                            T_CB_ARG arg);


    /**
     * Delegator method for read callbacks.  Will be called when the
     * callback is triggered and will in turn call the callback's handler
     * method.
     *
     * @param connection the connection on which the callback was triggered
     * @param data the message queue callback data (contains the message
     *             for which the callback applies)
     * @param arg the callback object
     */
    static void T_ENTRY1 readCbDelegator(T_IPC_CONN connection,
                                         T_IPC_CONN_READ_CB_DATA data,
                                         T_CB_ARG arg);


    /**
     * Delegator method for encode callbacks.  Will be called when the
     * callback is triggered and will in turn call the callback's handler
     * method.
     *
     * @param connection the connection on which the callback was triggered
     * @param data the message queue callback data (contains the old and
     *             new packet information for which the callback applies)
     * @param arg the callback object
     */
    static void T_ENTRY1 encodeCbDelegator(T_IPC_CONN connection,
                                           T_IPC_CONN_ENCODE_CB_DATA data,
                                           T_CB_ARG arg);


    /**
     * Delegator method for decode callbacks.  Will be called when the
     * callback is triggered and will in turn call the callback's handler
     * method.
     *
     * @param connection the connection on which the callback was triggered
     * @param data the message queue callback data (contains the old and
     *             new packet information and the original message for
     *             which the callback applies)
     * @param arg the callback object
     */
    static void T_ENTRY1 decodeCbDelegator(T_IPC_CONN connection,
                                           T_IPC_CONN_DECODE_CB_DATA data,
                                           T_CB_ARG arg);

  public:
    /**
     * Constructors
     * @param connection - the C Style T_IPC_CONN handle
     * @param destroy_flag - flag for destroy
     * @throw TipcConnException
     */
    //@{
    TipcConn() throw (TipcException);
    TipcConn(T_IPC_CONN connection, bool destroy_flag=true)
    throw (TipcException);
    //@}

    ///Destructor
    virtual ~TipcConn() throw();


    ///operator
    operator T_IPC_CONN() const throw()  { return _connection;}

    // The following methods will have TipcSrv equivalents

    /**
     * Check if data can be read from or written to connection.
     * @param check_mode - value for the check mode, enum value
     * @param timeout - timeout
     * @throw TipcConnException
     */
    virtual bool check(T_IO_CHECK_MODE check_mode, real8 timeout)
    throw (TipcException);

    /**
     * Flush buffered outgoing messages on the connection.
     * @throw TipcConnException
     */
    virtual void flush() throw (TipcException);

    /**
     * Accessor methods; get and sets thhe auto_flush_size of a connection
     * to the RTserver
     * @param auto_flush_size - the value to be set
     * @return the auto flush size that is set.
     * @throw TipcConnException if the operation fails.
     */
    //@{
    virtual int4 getAutoFlushSize() const throw (TipcException);

    virtual void setAutoFlushSize(int4 auto_flush_size)
    throw (TipcException);
    //@}

    /**
     * Get/Set the block mode of a connection.
     * @param block_mode - flag for block mode
     * @return the value set for the block mode
     * @throw TipcConnException
     */
    //@{
    virtual long getBlockMode() const throw (TipcException);

    virtual void setBlockMode(bool block_mode)
    throw (TipcException);
    //@}


    /**
     * Accessor methods; Get/Set a timeout property from the connection.
     * @param auto_flush_size - the value to be set
     * @return the timeout which is set.
     * @throw TipcConnException if the operation fails.
     */
    //@{
    virtual real8 getTimeout(T_IPC_TIMEOUT timeout) const
    throw (TipcException);

    virtual void setTimeout(T_IPC_TIMEOUT timeout, real8  value)
    throw (TipcException);
    //@}

    /**
     * Determine the socket of the connection.
     * @return the socket information
     * @throw TipcConnException if the socket information could not be retrieved
     */
    virtual sock getSocket() const throw (TipcException);

    /**
     * Get number of queued messages from the connection.
     * @return the number of queued messages
     * @throw TipcConnException
     */
    virtual int4 getNumQueued() const throw (TipcException);

    /**
     * Get source for XtAppAddInput from the connection.
     * @return source for XtAppAddInput
     * @throw TipcConnException
     */
    virtual sock getXtSource() const throw (TipcException);

    /**
     * Check if the server connection is still alive.
     * @throw TipcConnException
     */
    virtual void keepAlive() throw (TipcException);

    /**
     * Read and process messages on the connection to the RTserver.
     * @param timeout - value that determines the timeout period
     * @throw TipcConnException
     */
    virtual bool mainLoop(real8 timeout = T_TIMEOUT_FOREVER)
    throw (TipcException);

    /**
     * Insert a message into the queue of the connection.
     * @param msg - message to be inserted
     * @param pos - position at which the message to be inserted
     * @throw TipcConnException if the insertion fails
     */
    virtual void insert(TipcMsg& msg, int4 pos)
    throw (TipcException);

    /**
     * Process a message in the connection.
     * @param msg - message to be processed
     * @throw TipcConnException
     */
    virtual void process(TipcMsg& msg) throw (TipcException);

    /**
     * Read all available data from the connection to RTserver and queue
     * messages in priority order.
     * @param timeout - timeout value, until which the read operation
     * can be performed.
     * @throw TipcConnException
     */
    void read(real8 timeout) throw (TipcException);

    /**
     * Retrieve the next message from the connection.
     * @param - msg place holder object for return value.
     * @param timeout - the value that determines the timeout period
     * @throw TipcConnException
     */
    virtual void next(TipcMsg& msg, real8 timeout)
    throw (TipcException);


    /**
     * Retrieve the next message from the connection.
     * @param msg - place holder object for return value.
     * @param timeout - the value that determines the timeout period
     * @throw TipcConnException if an error occurs.
     * @return false if the timeout is expired, true otherwise.
     */
    virtual bool nextEx(TipcMsg& msg, real8 timeout)
    throw (TipcException);


    /**
     * Search the message queue of the connection for a specific message
     * @param msg - TipcMsg object which is the result of the search
     * @param selObj - the object to be searched for
     * @param timeout - timeout value, until which the search can be performed
     * @throw TipcConnException
     */
    virtual void search(TipcMsg& msg, TipcConnSearchSelector* selObj,
                        real8 timeout) throw (TipcException);

    /**
     * Search the message queue of the connection for a message with
     * a specific type.
     * @param msg - TipcMsg object which is the result of the search
     * @param mt - TipcMt object; message type
     * @param timeout -  the timeout value until which the search can be performed
     * @throw TipcConnException
     */
    virtual void searchType(TipcMsg& msg, TipcMt& mt, real8 timeout)
    throw (TipcException);

    /**
     * Send a message through the connection. For connections, the
     * rtserver_check flag is defaulted and ignored. The TipcSrv derived
     * class version of this virtual fn will use it.
     * @param msg - message to sent it through the RTserver
     * @param rtserver_check  - flag for check server message send
     * @throw TipcConnException
     */
    virtual void send(const TipcMsg& msg, bool rtserver_check=FALSE)
    throw (TipcException);

    /**
     * Make a remote procedure call (RPC) with messages on the connection.
     * @param msg - message to sent it through the RTserver
     * @param call_msg - message to be sent through Rpc
     * @param timeout - timeout value, until which the operation to
     * be performed
     * @throw TipcConnException
     */
    virtual void sendRpc(TipcMsg& reply_msg, const TipcMsg& call_msg, real8 timeout)
    throw (TipcException);


#if defined(T_OS_WINDOWS)

    /*
     * The Microsoft Visual C++ compilers (4.2 and 5.0, and maybe earlier ones),
     * have a bug that makes it impossible to use references as the last
     * non-variable argument in a varargs list.  So, for Windows, we're just
     * passing the TipcMt object by value (since it's not modified [really
     * ought to be const, cie la vie...]).  Once this bug is fixed, or if
     * you're using some other compiler, this ifdef can be removed.
     */

    /**
     * Construct a message and send it through the connection.
     * @param mt - the message type corresponding to the message
     * @param var_arg_list - the va_list parameters
     * @throw TipcConnException
     */
    //@{
    virtual void     write(TipcMt mt, ...) throw (TipcException);
    virtual void     writeVa(TipcMt mt, va_list var_arg_list)
    throw (TipcException);
    //@}
#else
    void     write(TipcMt& mt, ...) throw (TipcException);
    void     writeVa(TipcMt& mt, va_list var_arg_list)
    throw (TipcException);

#endif //T_OS_WINDOWS


    /**
     * Print information about the connection.
     * @param func - the TutOut object containing the info to print
     * @throw TipcConnException
     */
    virtual void  print(T_OUT_FUNC func = (T_OUT_FUNC) TutOut) const
    throw (TipcException);

    // CALLBACKS
    /**
     * Creates a message queue callback.
     *
     * @param mt the message type for the callback
     * @param implementor a message queue callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcConnException if creation of the callback fails
     */
    virtual CallBack<MessageQueueCallBack> * queueCbCreate(const TipcMt& mt,
                                                           MessageQueueCallBack *,
                                                           void* arg = NULL)
    throw (TipcException);


    /**
     * Creates an error callback.
     *
     * @param implementor an error callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcConnException if creation of the callback fails
     */
    virtual CallBack <ErrorCallBack> * errorCbCreate(
                                                    ErrorCallBack * implementor,
                                                    void* arg = NULL)
    throw (TipcException);


    /**
     * Creates an encryption callback.
     *
     * @param mt the message type for the callback
     * @param implementor an encryption callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcConnException if creation of the callback fails
     */
    virtual CallBack <MessageCallBack> * encryptCbCreate(
                                                        const TipcMt & mt,
                                                        MessageCallBack * implementor,
                                                        void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a decryption callback.
     *
     * @param mt the message type for the callback
     * @param implementor a decryption callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcConnException if creation of the callback fails
     */
    virtual CallBack <MessageCallBack> * decryptCbCreate(
                                                        const TipcMt & mt,
                                                        MessageCallBack * implementor,
                                                        void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a default callback.
     *
     * @param implementor a messagen callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcConnException if creation of the callback fails
     */
    virtual CallBack <MessageCallBack> * defaultCbCreate(
                                                        MessageCallBack * implementor,
                                                        void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a process callback.
     *
     * @param mt the message type for the callback
     * @param implementor a messagen callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcConnException if creation of the callback fails
     */
    virtual CallBack <MessageCallBack> * processCbCreate(
                                                        const TipcMt & mt,
                                                        MessageCallBack * implementor,
                                                        void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a read callback.
     *
     * @param mt the message type for the callback
     * @param implementor a messagen callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcConnException if creation of the callback fails
     */
    virtual CallBack <MessageCallBack> * readCbCreate(
                                                     const TipcMt & mt,
                                                     MessageCallBack * implementor,
                                                     void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a write callback.
     *
     * @param mt the message type for the callback
     * @param implementor a messagen callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcConnException if creation of the callback fails
     */
    virtual CallBack <MessageCallBack> * writeCbCreate(
                                                      const TipcMt & mt,
                                                      MessageCallBack * implementor,
                                                      void* arg = NULL)
    throw (TipcException);


    /**
     * Creates an encode callback.
     *
     * @param implementor an encode callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcConnException if creation of the callback fails
     */
    virtual CallBack <EncodeCallBack> * encodeCbCreate(
                                                      EncodeCallBack * implementor,
                                                      void* arg = NULL)
    throw (TipcException);


    /**
     * Creates a decode callback.
     *
     * @param implementor an encode callback implementation
     * @param arg argument data for the callback
     * @return the constructed callback object
     * @throw TipcConnException if creation of the callback fails
     */
    virtual CallBack <DecodeCallBack> * decodeCbCreate(
                                                      DecodeCallBack * impleemntor,
                                                      void* arg = NULL)
    throw (TipcException);


    // ---------------------------------------------------------------------
    //  Starred methods have RTserver (TipcSrv*) overrides
    // ---------------------------------------------------------------------

    /**
     * Determine the unique subject of the connected server.
     * @return the unique subject
     * @throw TipcConnException
     */
    virtual const char * getPeerUniqueSubject() const throw (TipcException);

    /**
     * Determine the node name of the connected server.
     * @return the node information
     * @throw TipcConnException
     */
    virtual const char * getNode() const throw (TipcException);

    /**
     * Determine the user name of the connected server.
     * @return the user information
     * @throw TipcConnException
     */
    virtual const char * getUser() const throw (TipcException);

    /**
     * Determine the process ID of the connected server.
     * @return the Pid information
     * @throw TipcConnException
     */
    virtual int4 getPid() const throw (TipcException);

    /**
     * Determine the architecture name of the connected server.
     * @return the architecture name to the connected server
     * @throw TipcConnException
     */
    virtual const char * getArch() const throw (TipcException);

    /**
     * Acquire/Release exclusive access to the connection
     * @throw TipcConnException
     */
    //@{
    virtual void lock() throw (TipcException);

    virtual void unlock() throw (TipcException);
    //@}

    // -- GMD --

    /**
     * Create guaranteed message delivery area on the connection to RTserver.
     * @throw TipcConnException
     */
    virtual void gmdFileCreate() throw (TipcException);

    /**
     * Delete guaranteed message delivery files for the connection
     * to RTserver.
     * @throw TipcConnException
     */
    virtual void gmdFileDelete() throw (TipcException);

    /**
     * Resend all guaranteed messages after a delivery failure on the
     * connection toRTserver.
     * @throw TipcConnException
     */
    virtual void gmdResend() throw (TipcException);

    /**
     * Delete a message from the GMD area after a GMD failure on
     * the connection toRTserver.
     * @param msg - message to delete
     * @throw TipcConnException
     */
    virtual void gmdMsgDelete(TipcMsg& msg) throw (TipcException);

    /**
     * Resend a message after a GMD failure on the connection to RTserver.
     * @param msg - message to resend
     * @throw TipcConnException
     */
    virtual void gmdMsgResend(TipcMsg& msg) throw (TipcException);

    /**
     * Get/Set the GMD area maximum size of the connection to RTserver.
     * @param gmd_max_size - value to be set for the maximum GMD area
     * @return the maximum size of the GMD area
     * @throw TipcConnException
     */
    //@{
    virtual uint4 getGmdMaxSize() const throw (TipcException);
    virtual void setGmdMaxSize(uint4 gmd_max_size)
    throw (TipcException);
    //@}

    /**
     * Get name of directory where files are written for guaranteed
     * message delivery.
     * @return the directory name where the GMD files are written
     * @throw TipcConnException
     */
    const char * getGmdDir() const
    throw (TipcException);

    /**
     * Determine the number of outgoing GMD messages still pending on
     * the connection.
     * @return the number of GMD messages pending
     * @throw TipcConnException
     */
    virtual int4 getGmdNumPending() const throw (TipcException);

    /**
     * Gets the total number of bytes received on the connection.
     *
     * @return the total bytes received on the connection.
     * @throw TipcConnException
     *
     * @note The result is truncated to 31 bits, and may contain an incorrect
     * sign bit on some older clients. New clients should use
     * trafficGetBytesRecv8.
     * @see trafficGetBytesRecv8.
     * @deprecated This method is superseded by trafficGetBytesRecv8 as of
     * SmartSockets 6.7
     */
    virtual int4 trafficGetBytesRecv() const throw (TipcException);

    /**
     * Gets the total number of bytes received on the connection.
     *
     * @return the total bytes received on the connection.
     * @throw TipcConnException
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
     * @return the total bytes written to the write buffer on the
     *         connection.
     * @throw TipcConnException
     *
     * @note The result is truncated to 31 bits, and may contain an incorrect
     * sign bit on some older clients. New clients should use
     * trafficGetBytesSent8.
     * @see trafficGetBytesSent8.
     * @deprecated This method is superseded by trafficGetBytesSent8 as of
     * SmartSockets 6.7
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
     * @return the total bytes written to the write buffer on the
     *         connection.
     * @throw TipcConnException
     */
    virtual int8 trafficGetBytesSent8() const throw (TipcException);

    /**
     * Gets the total number of messages received on the connection.
     *
     * @return the total number of messages received on the connection.
     * @throw TipcConnException
     *
     * @note The result is truncated to 31 bits, and may contain an incorrect
     * sign bit on some older clients. New clients should use
     * trafficGetMsgsRecv8.
     * @see trafficGetMsgsRecv8.
     * @deprecated This method is superseded by trafficGetMsgsRecv8 as of
     * SmartSockets 6.7
     */
    virtual int4 trafficGetMsgsRecv() const throw (TipcException);

    /**
     * Gets the total number of messages received on the connection.
     *
     * @return the total number of messages received on the connection.
     * @throw TipcConnException
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
     * @return the total number of messages written to the write
     *         buffer on the connection.
     * @throw TipcConnException
     *
     * @note The result is truncated to 31 bits, and may contain an incorrect
     * sign bit on some older clients. New clients should use
     * trafficGetMsgsSent8.
     * @see trafficGetMsgsSent8.
     * @deprecated This method is superseded by trafficGetMsgsSent8 as of
     * SmartSockets 6.7
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
     * @return the total number of messages written to the write
     *         buffer on the connection.
     * @throw TipcConnException
     */
    virtual int8 trafficGetMsgsSent8() const throw (TipcException);

    /**
     * Gets the total number of bytes in the connection's read buffer.
     *
     * @return the total bytes in the connection's read buffer.
     * @throw TipcConnException
     */
    virtual int4 bufferGetReadSize() const throw (TipcException);

    /**
     * Gets the total number of bytes in the connection's write buffer.
     *
     * @return the total bytes in the connection's write buffer.
     * @throw TipcConnException
     */
    virtual int4 bufferGetWriteSize() const throw (TipcException);
  };


  /**
   * TipcConnServer - user class for a server connection
   * The TipcConnServer class calls the C API function TipcConnServerCreate
   * at construction, passing the resulting T_IPC_CONN handle up to the
   * TipcConn base class, where it is managed.
   *
   * @see the SmartSockets Application Programming Interface for information
   * on TipcConnCreate and TipcConnServerCreate
   */
  class T_IPCX_API TipcConnServer : public TipcConn {

  public:
    /**
     * Constructors
     * @param link_name - logical connection name
     * @param connection - the C Style T_IPC_CONN handle
     * @param destroy_flag - the flag for destroy
     * @throw TipcConnException
     */
    //@{
    TipcConnServer(const char * link_name) throw (TipcException);
    TipcConnServer(T_IPC_CONN connection, bool destroy_flag=TRUE)
    throw (TipcException);
    //@}

    ///Destructor
    virtual ~TipcConnServer()  throw();

    /**
     * Accepts a connection from a client and returns a pointer to a
     * new TipcConn object. It is your responsibility to delete this
     * object when finished.
     * @return the pointer to the TipcConn object
     * @throw TipcConnException
     */
    virtual TipcConn *accept() throw (TipcException);
  };

  /**
   * TipcConnClient - user class for a client connection
   * The TipcConnClient class calls the C API function TipcConnClientCreate
   * at construction,passing the resulting T_IPC_CONN handle up to the
   * TipcConn base class, where it is managed.
   * @see the SmartSockets Application Programming Interface for
   * information on TipcConnCreate and TipcConnClientCreate
   */
  class T_IPCX_API TipcConnClient : public TipcConn {

  public:
    /**
     * Constructors
     * @param link_name - logical connection name
     * @param connection - the C Style T_IPC_CONN handle
     * @param destroy_flag - the flag for destroy
     * @throw TipcConnException
     */
    //@{
    TipcConnClient() throw () {}
    TipcConnClient(const char * link_name) throw (TipcException);
    TipcConnClient(T_IPC_CONN connection, bool destroy_flag=true)
    throw (TipcException);
    //@}

    ///Destructor
    virtual ~TipcConnClient() throw();
  };

} // namespace SmartSockets
#endif
