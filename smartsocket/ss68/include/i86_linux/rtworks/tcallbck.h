/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tcallbck.h#1 $
 */

#ifndef _T_CALLBACK_HH_
#define _T_CALLBACK_HH_

#include <rtworks/types.h>
#include <rtworks/tex.h>



namespace SmartSockets {

  class T_IPCX_API TipcConn;
  class T_IPCX_API TipcMsg;
  class T_IPCX_API TipcSrv;


  /**
   * Inherits from Exception so users can trap callback related
   * exceptions.  Offers no additional functionality.
   *
   * @see Exception, CallBack
   */
  class T_IPCX_CB_API CallBackException : public Exception 
  {
  public:
    /**
     * Constructor.
     */
    CallBackException() {}

/* Anticipation of a more functional exception class
    CallBackException( int4 ssErrNum = 0, 
                       int4 osErrNum = 0,
                       int4 socketErrNum = 0,
                       int4 cErrNum = 0)
                       : Exception (ssErrNum, osErrNum, socketErrNum, cErrNum)
    {
    }
*/

    /**
     * Destructor
     */
    virtual ~CallBackException() throw()
    {
    }
  };


  /**
   * The CallBack class provides a template for callback objects.
   *
   * The T_CB struct underlies the callback object and the creation,
   * destruction, setting and getting of priority and print operations
   * are performed on this struct.
   */
  template <class Implementor>
  class T_IPCX_CB_API CallBack 
  {
    friend class T_IPCX_API TipcSrv;
    friend class T_IPCX_API TipcConn;
    friend class T_IPCX_API Option;

  public:

    /**
     * Destroys the callback
     *
     * @throw CallBackException if the desctruction of the callback
     *        fails
     */
    void destroy() throw (CallBackException) 
    {
      if (is_created()) 
      {
        if (!TutCbDestroy(m_cb))
        {
          throw CallBackException();
        }

        m_cb = 0;
      }

      delete this;
    }

   
    /**
     * Returns whether this callback object has been successfully created
     *
     * @return whether this callback object has been successfully created
     */
    bool is_created() throw () 
    {
      return 0 != m_cb;
    }


    /**
     * Returns the underlying callback structure of this object.
     *
     * @return the underlying callback structure of this object.
     */
    operator T_CB () throw () 
    {
      return m_cb;
    }


    /**
     * Returns this callback object's argument data.
     *
     * @return this callback object's argument data.
     */
    void * getArgument () const throw ()
    {
      return m_argument;
    }


    /**
     * Sets this callback object's argument data.
     *
     * @param data this callback object's argument data.
     */
    void setArgument (void* data) throw ()
    {
      m_argument = data;
    }


    /**
     * Returns the priority of this callback.
     *
     * @return the priority of this callback.
     */
    T_CB_PRIORITY getPriority () const throw (CallBackException) 
    {
      T_CB_PRIORITY result;

      if (!TutCbGetPriority (m_cb, &result)) 
      {
        throw CallBackException ();
      }

      return result;
    }

    
    /**
     * Sets the priority of this callback.
     *
     * @param priority the priority of this callback.
     */
    void setPriority (T_CB_PRIORITY priority) throw (CallBackException) 
    {
      if (!TutCbSetPriority (m_cb, priority)) 
      {
        throw CallBackException ();
      }
    }


    /**
     * Calls the callback print method.
     *
     * @return TRUE
     */
    virtual bool print (T_OUT_FUNC func = (T_OUT_FUNC)TutOut) 
                                                throw (CallBackException) 
    {
      TutCbPrint(m_cb, func);
      return T_TRUE;
    }

  protected:
    /* 
     * The callback structure underlying this object
     */
    T_CB m_cb; 

    /*
     * The call back implementing class.
     */
    Implementor * m_implementor;  
   
    /*
     * The callback handling method within the implementing class.
     */
    typename Implementor::Handler m_handler; 
                                      
    /*
     * The argument data for this object.
     */
    void * m_argument;


    /**
     * Sets the underlying callback structure of this object.
     *
     * @param handle the underlying calllback structure of this object.
     */
    void setTCB (T_CB handle)
    {
      m_cb = handle;
    }

  private:
    /**
     * Constructor
     * 
     * @param implemntor the callback implemting object
     * @param handler the handling method in the implementing object
     */
    CallBack (Implementor * implementor,
              typename Implementor::Handler handler)
      : m_cb (0)
      , m_implementor (implementor)
      , m_handler (handler)
      , m_argument(0)
    { 
    }

    /**
     * Destructor
     */
    virtual ~CallBack () throw()
    {
    }

    /**
     * Disallows copy construction by being private
     *
     * @param callback the source of the copy
     */
    CallBack (const CallBack & callback)
    {
    }
  };


  /**
   * The connection callback class impelements the CallBack template
   * It can be triggered for connection creations and destructions.
   *
   * Upon triggering, onConnection is the default method is executed
   */
  class T_IPCX_CB_API ConnectionCallBack
  {      
  public:
    enum ConnStatus
    {
      none,/* not connected at all */
      warm,/* warm connection to RTserver */
      full /* full connection to RTserver */
    };

    typedef void (ConnectionCallBack::* Handler) (
      CallBack < ConnectionCallBack > * callBack,
      TipcSrv & connection,
      ConnStatus oldConnStatus,
      ConnStatus newConnStatus);

    /**
     * Constructor
     */
    ConnectionCallBack () {}

    /**
     * Destructor
     */
    virtual ~ConnectionCallBack ()  throw() {}


    /**
     * Default method called when a connection callback is triggered.
     * Prints relevent callback and connection information.
     *
     * @param callBack the callback that was triggered
     * @param connection the connection on which the callback was triggered
     * @param oldConnStatus the former connection status
     * @param newConnStatus the current connection status
     */
    virtual void onConnection (
      CallBack < ConnectionCallBack > * callBack,
      TipcSrv & connection,
      ConnStatus oldConnStatus,
      ConnStatus newConnStatus); 

  };


  /**
   * The message callback class impelements the CallBack template
   * It can be triggered for read, write, encrypt, decrypt, default,
   * process and subject callbacks.
   *
   * Upon triggering, onConnection is the default method is executed
   */
  class T_IPCX_CB_API MessageCallBack
  {
  public:
    typedef void (MessageCallBack::* Handler) (
                              CallBack <MessageCallBack> * callBack,
                              TipcMsg & message,
                              TipcConn & connection);

    /**
     * Constructor
     */
    MessageCallBack () {}
                       

    /**
     * Destructor
     */
    virtual ~MessageCallBack () throw() {}


    /**
     * Default method called when a message callback is triggered.
     * Prints the relevent callback, connection and message information
     *
     * @param callBack the callback that was triggered
     * @param conn the connection on which the callback was triggered
     * @param msg the msg for which the callback was triggered
     */
    virtual void onMessage (CallBack<MessageCallBack>* callback,
                            TipcMsg & msg,
                            TipcConn & conn); 

/*
    static void dispatchCreateMsgDelegator (T_IPC_DISPATCHER_EVENT event,
                                            void * data,
                                            void * arg);
*/
/*
    static void connMsgDelegator (T_IPC_CONN conn, 
                                  T_IPC_CONN_MSG_CB_DATA data, 
                                  T_CB_ARG arg);
*/

  };


  /**
   * The error callback class impelements the CallBack template
   * It can be triggered for error callbacks.
   *
   * Upon triggering, onError is the default method executed.
   */
  class T_IPCX_CB_API ErrorCallBack
  {      
  public:
    typedef void (ErrorCallBack::* Handler) (
      CallBack < ErrorCallBack > * callBack,
      Exception & error,
      TipcConn & conn);


    /**
     * Constructor
     */
    ErrorCallBack() {}


    /**
     * Destructor
     */
    virtual ~ErrorCallBack() throw() {}


    /**
     * Default method called when an error callback is triggered.
     * Prints the relevent callback, connection and exception information.
     *
     * @param callBack the callback that was triggered
     * @param conn the connection on which the callback was triggered
     * @param error the exception that triggered the callback.
     */
    virtual void onError (
      CallBack <ErrorCallBack> * callBack,
      Exception & error,
      TipcConn & conn); 
  };


  /**
   * The message queue callback class impelements the CallBack template
   * It can be triggered for queue callbacks.
   *
   * Upon triggering, onMessage is the default method executed.
   */
  class T_IPCX_CB_API MessageQueueCallBack
  {
  public:
    typedef void (MessageQueueCallBack::* Handler) (
                                 CallBack <MessageQueueCallBack> * callBack,
                                 TipcMsg & message,
                                 TipcConn & connection,
                                 uint4 queuePos,
                                 bool is_insert);

    /**
     * Constructor
     */
    MessageQueueCallBack() {}

    /**
     * Destructor
     */
    virtual ~MessageQueueCallBack () throw() {}

    /**
     * Default method called when a message queue callback is triggered.
     * Prints the relevent callback, connection, message and queue
     * information.
     *
     * @param callBack the callback that was triggered
     * @param conn the connection on which the callback was triggered
     * @param message the message that triggered the callback.
     * @param queuePos the position in the queue of the affected message
     * @param is_insert whether the messge was inserted
     */
    virtual void onMessage (CallBack <MessageQueueCallBack> * callBack,
                            TipcMsg & message,
                            TipcConn & connection,
                            uint4 queuePos,
                            bool is_insert); 
  };



  /**
   * The encode callback class impelements the CallBack template
   * It can be triggered for encode callbacks.
   *
   * Upon triggering, onMessage is the default method executed.
   */
  class T_IPCX_CB_API EncodeCallBack
  {
  public:
    typedef void (EncodeCallBack::* Handler) ( 
                                      CallBack <EncodeCallBack> * callBack,
                                      void * packet,
                                      int4 packetSize,
                                      void * origPacket,
                                      int4 origPacketSize,
                                      TipcMsg & origMessage,
                                      TipcConn & connection);

    /**
     * Constructor
     */
    EncodeCallBack() {};

    /**
     * Destructor
     */
    virtual ~EncodeCallBack () throw() {}

    /**
     * Default method called when an encode callback is triggered.
     * Prints the relevent callback, connection, message and packet
     * information.
     *
     * @param callBack the callback that was triggered
     * @param conn the connection on which the callback was triggered
     * @param origMessage the original message whose encoding triggered 
     *                    the callback
     * @param origPacket the former packet
     * @param packet the current packet
     * @param origPacketSize the former packet size
     * @param packetSize the current packet size
     */
    virtual void onMessage (CallBack <EncodeCallBack> * callBack, 
                            void * packet,
                            int4 packetSize,
                            void * origPacket,
                            int4 origPacketSize,
                            TipcMsg & origMessage,
                            TipcConn & connection);
  };



  /**
   * The decode callback class impelements the CallBack template
   * It can be triggered for decode callbacks.
   *
   * Upon triggering, onMessage is the default method executed.
   */
  class T_IPCX_CB_API DecodeCallBack
  {
  public:
    typedef void (DecodeCallBack::* Handler) (
                                CallBack < DecodeCallBack > * callBack,
                                void * packet,
                                int4 packetSize,
                                void * origPacket,
                                int4 origPacketSize,
                                TipcConn & connection);

    /**
     * Constructor
     */
    DecodeCallBack() {};

    /**
     * Destructor
     */
    virtual ~DecodeCallBack () throw() {};

    /**
     * Default method called when an encode callback is triggered.
     * Prints the relevent callback, connection, message and packet
     * information.
     *
     * @param callBack the callback that was triggered
     * @param conn the connection on which the callback was triggered
     * @param origPacket the former packet
     * @param packet the current packet
     * @param origPacketSize the former packet size
     */
    virtual void onMessage (CallBack <DecodeCallBack> * callBack,
                            void * packet,
                            int4 packetSize,
                            void * origPacket,
                            int4 origPacketSize,
                            TipcConn & connection);
  };

}
#endif
