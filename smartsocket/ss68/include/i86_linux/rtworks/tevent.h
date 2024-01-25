/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tevent.h#1 $
 */

#ifndef _T_EVENT_HH
#define _T_EVENT_HH

#include <rtworks/ipc.h>
#include <rtworks/tex.h>

namespace SmartSockets {

  /**
   * Inherits from TipcException so users can trap TipcEvent related
   * exceptions. Offers no additional functionality.
   *
   * @see TipcException, TipcEvent
   */
  class T_IPCX_API TipcEventException : public TipcException {

  public:
    /**
     * Constructor.
     */
    TipcEventException () {};

    /**
     * Destructor.
     */
    virtual ~TipcEventException () throw() {
    }
  };

  class T_IPCX_API MessageEvent;
  class T_IPCX_API SocketEvent;
  class T_IPCX_API UserEvent;
  class T_IPCX_API ConnEvent;
  class T_IPCX_API TimerEvent;
  class T_IPCX_API TipcMsg;
  class T_IPCX_API TipcConn;
  class T_IPCX_API TipcSrv;
  class T_IPCX_API TipcDispatcher;

  /**
   * Base class that has the interface and default
   * implementation for the MessageEvent handlers.
   * Classes inheriting from this will have either
   * an overloaded version of onMessage(), or a
   * method matching the Handler prototype invoked
   * by the dispatcher whenever a MessageEvent is
   * triggered.
   */
  class T_IPCX_API MessageEventHandler {
  public:

    /**
     * Signature for the message event handler
     * methods.
     * @param event the message event that registered this
     * handler to the dispatcher.
     * @param msg the message that triggered this event
     * handler.
     */
    typedef void (MessageEventHandler::*Handler) (
                                                 MessageEvent& event,
                                                 TipcMsg& msg);


    /**
     * Default method for handling messsage events.
     * @param event the message event that registered this
     * handler to the dispatcher.
     * @param msg the message that triggered this event
     * handler.
     */
    virtual void onMessage(MessageEvent& event,
                           TipcMsg& msg)
    {
    }
  };


  /**
   * Base class that has the interface and default
   * implementation for the SocketEvent handlers.
   * Classes inheriting from this will have either
   * an overloaded version of onSocket(), or a
   * method matching the Handler prototype invoked
   * by the dispatcher whenever a SocketEvent is
   * triggered.
   */
  class T_IPCX_API SocketEventHandler {
  public:
    /**
     * Signature for the socket event handler
     * methods.
     * @param event the socket event that registered this
     * handler with the TipcDispatcher.
     * @param socketFd the socket file descriptor that
     * triggered this event handler.
     * @param checkMode variable that determines whether
     * there's data to be read from (T_IO_CHECK_READ) or written
     * to a socket (T_IO_CHECK_WRITE).
     */
    typedef void (SocketEventHandler::*Handler) (
                                                SocketEvent& event,
                                                sock socketFd,
                                                T_IO_CHECK_MODE checkMode);

    /**
     * Default implementation for the socket event handler
     * methods.
     * @param event the socket event that registered this
     * handler with the TipcDispatcher.
     * @param socketFd the socket file descriptor that
     * triggered this event handler.
     * @param checkMode variable that determines whether
     * there's data to be read from (T_IO_CHECK_READ) or written
     * to a socket (T_IO_CHECK_WRITE).
     */

    virtual void onSocket(SocketEvent& event,
                          sock socketFd,
                          T_IO_CHECK_MODE checkMode)
    {
    }
  };


  /**
   * Base class that has the interface and default
   * implementation for the UserEvent handlers.
   * Classes inheriting from this will have either
   * an overloaded version of onUserEvent(), or a
   * method matching the Handler prototype invoked
   * by the dispatcher whenever a UserEvent is
   * triggered.
   */
  class T_IPCX_API UserEventHandler {
  public:

    /**
     * Signature for the UserEvent handler
     * methods.
     * @param event the user event that registered this
     * handler with the TipcDispatcher.
     * @param data the data set when registering this event
     * with the Dispatcher.
     */
    typedef void (UserEventHandler::*Handler) (
                                              UserEvent& event,
                                              void* data);


    /**
     * Default implementation for the UserEvent handler
     * methods.
     * @param event the user event that registered this
     * handler with the TipcDispatcher.
     * @param data the data set when registering this event
     * with the Dispatcher.
     */
    virtual void onUserEvent(UserEvent& event,
                             void* data)
    {
    }
  };


  /**
   * Base class that has the interface and default
   * implementation for the ConnEvent handlers.
   * Classes inheriting from this will have either
   * an overloaded version of onConnEvent(), or a
   * method matching the Handler prototype invoked
   * by the dispatcher whenever a ConnEvent is
   * triggered.
   */
  class T_IPCX_API ConnEventHandler {
  public:
    /**
     * Signature for the ConnEvent handler
     * methods.
     * @param event the ConnEvent that registered this
     * handler with the TipcDispatcher.
     * @param conn the peer-to-peer connection  that
     * triggered this event handler.
     * @param checkMode variable that determines whether
     * there's data to be read from (T_IO_CHECK_READ) or written
     * to a connection (T_IO_CHECK_WRITE).
     */
    typedef void (ConnEventHandler::*Handler) (
                                              ConnEvent& event,
                                              TipcConn& conn,
                                              T_IO_CHECK_MODE checkMode);

    /**
     * Default implementation for the ConnEvent handler
     * methods.
     * @param event the ConnEvent that registered this
     * handler with the TipcDispatcher.
     * @param conn the peer-to-peer connection  that
     * triggered this event handler.
     * @param checkMode variable that determines whether
     * there's data to be read from (T_IO_CHECK_READ) or written
     * to a connection (T_IO_CHECK_WRITE).
     */
    virtual void onConnEvent (ConnEvent& event,
                              TipcConn& conn,
                              T_IO_CHECK_MODE checkMode)
    {
#if 0
      if (checkMode == T_IO_CHECK_READ) {
        TipcMsg* msg = conn.next(T_TIMEOUT_FOREVER);
        if (msg != NULL) {
          conn.process(*msg);
          delete msg;
        }
      }
#endif
    }
  };


  /**
   * Base class that has the interface and default
   * implementation for the TimerEvent handlers.
   * Classes inheriting from this will have either
   * an overloaded version of onTimer(), or a
   * method matching the Handler prototype invoked
   * by the dispatcher whenever a TimerEvent is
   * triggered.
   */
  class T_IPCX_API TimerEventHandler {
  public:
    /**
     * Signature for the TimerEvent handler
     * methods.
     * @param event the TimerEvent that registered this
     * handler with the TipcDispatcher.
     * @param timerInterval the number of seconds until this
     * event handler is next triggered by the Dispatcher.
     */
    typedef void (TimerEventHandler::*Handler) (
                                               TimerEvent& event,
                                               real8 timerInterval);

    /**
     * Default implementation for the TimerEvent handler
     * methods.
     * @param event the TimerEvent that registered this
     * handler with the TipcDispatcher.
     * @param timerInterval the number of seconds until this
     * event handler is next triggered by the Dispatcher.
     */
    virtual void onTimer(TimerEvent& event,
                         real8 timerInterval)
    {
      printf ("Current time = %f\n", TutGetWallTime());
    }
  };


  /**
   * Base class to all events handled by the Dispatcher.  This
   * class allows users to either destroy an event (e.g. stop triggering
   * the event handlers), or to get the type of an event, or even to
   * get the Dipatcher that created the object.  The first use (destroying
   * events is particularly useful to remove events from within event
   * handlers.  Note that only the TipcDispatcher and TipcDispatcher
   * traverser are allowed to instantiate or delete objects of this class.
   *
   * The TipcEvent class and its children are used to contain the
   * handler objects and pointers called when an event is triggered.
   * @warning Do not delete this object using the delete operator;
   * call either the TipcEvent::destroy() method, or the
   * TipcDispatcher::destroyEvent() method.
   */
  class T_IPCX_API TipcEvent {

  public:
    /// friend class
    //@{
    friend class T_IPCX_API TipcDispatcher;
    friend class T_IPCX_API TipcDispatcherTraverser;
    //@}

  protected:

    /**
     * Protected Constructor; use the Dispatcher to create TipcEvent child
     * classes.
     */
    TipcEvent()
    : _event (NULL)
    {
    }

    /**
     * Destructor; use  the Dispatcher::destroyEvent() to delete this class.
     */
    virtual ~TipcEvent() throw()
    {
    }



    /**
     * Allows child classes to set the underlying C event structure.
     */
    void setEvent(T_IPC_EVENT event)
    {
      _event = event;
    }
  public:


    /**
     * Unregisters the events with the
     * TipcDispatcher objects.
     */
    void destroy() throw (TipcException)
    {
      if (_event != NULL) {
        if (!TipcEventDestroy(_event))
          throw TipcEventException();
      }
      delete this;
    }


    /**
     * Gets the Dispatcher that created this event.
     */
    TipcDispatcher getDispatcher()
    throw (TipcException);

    /**
     * Gets the underlying C structure for compatibility with
     * the C API.
     */
    operator T_IPC_EVENT() const
    throw ()
    {
      return _event;
    }

    /**
     * Gets the type of the event.
     * @return one of the following:
     * - T_IPC_EVENT_USER - this pointer is a UserEvent.
     * - T_IPC_EVENT_TIMER - this pointer is a TimerEvent.
     * - T_IPC_EVENT_SOCKET - this pointer is a SocketEvent.
     * - T_IPC_EVENT_CONN - this pointer is a ConnEvent.
     * - T_IPC_EVENT_MSG - this pointer is a MessageEvent created
     * with TipcDispatcher::createMsgSubjEvent().
     * - T_IPC_EVENT_MSG_TYPE - this pointer is a MessageEvent
     * created with TipcDispatcher::createMsgTypeEvent().
     */
    T_IPC_EVENT_TYPE getType() const
    throw (TipcException);


  protected:
    /**
     * The actual C style handle we are wrapping.
     */
    T_IPC_EVENT _event;

  };



  /**
   * Class that represents a user event; user events are only useful
   * for inter thread communication.  Events of this type will be
   * immediately queued in the Dispatcher.  If the dispatcher is running
   * in detatched mode, this allows one thread to communicate to the
   * dispatcher's thread in a thread safe manner.
   *
   * This class contains the UserEventHandler object and method that are
   * triggered when a user event is dispatched.
   * @warning Do not delete this object using the delete operator;
   * call either the TipcEvent::destroy() method, or the
   * TipcDispatcher::destroyEvent() method.
   */
  class T_IPCX_API UserEvent: public TipcEvent {
  public:
    /// friend class
    //@{
    friend class T_IPCX_API TipcDispatcher;
    friend class T_IPCX_API TipcDispatcherTraverser;
    //@}

    /**
     * Allows users to get the user event data associated with
     * this event.  The data can be any arbitrary user specified data.
     * Note that the memory returned by this method belongs to the
     * UserEvent object, and therefore, should not be deleted.
     */
    const void* getData() throw (TipcException)
    {
      void* data = NULL;
      if (!TipcEventGetData(_event,&data))
        throw TipcEventException();
      return const_cast <const void*> (data);
    }

  private:
    /**
     * Private constructor to ensure that only TipcDispatcher objects
     * can instantiate this class.
     */
    UserEvent(UserEventHandler* handlerObj,
              UserEventHandler::Handler handlerMethod)
    : _handlerObj(handlerObj)
    , _handlerMethod(handlerMethod)
    {
    }

    virtual ~UserEvent() throw()
    {
    }

    UserEventHandler* _handlerObj;
    UserEventHandler::Handler _handlerMethod;
  };

  /**
   * Class that represents a message event.  Message events
   * are triggered whenever a message matching a message type or
   * a subject arrive in a given server connection.
   *
   * Note that MessageEvents provide very similar functionality
   * to callbacks.  It is not recommended to mix the MessageEvent
   * class with MessageCallBack classes as these are different
   * programming paradigms.  If MessageEvents are used in conjunction
   * with MessageCallbacks, all callbacks the server connection will be
   * triggered before the message events.
   *
   * Caution:
   *
   * When an event is created or destroyed from a thread other than
   * the thread dispatching the events, a user event is added to
   * the dispatcher to perform the actual creation or destruction.
   * To ensure that event data, such as a socket or a connection,
   * is not destroyed before the event is removed from the
   * dispatcher, create a user event to destroy the event and to
   * perform any necessary cleanup.
   *
   *
   * This class contains the MessageEventHandler object and method
   * that is called by the TipcDispatcher when this event is triggered.
   * @warning Do not delete this object using the delete operator;
   * call either the TipcEvent::destroy() method, or the
   * TipcDispatcher::destroyEvent() method.
   */
  class T_IPCX_API MessageEvent: public TipcEvent {
  public:
    /// friend class
    //@{
    friend class T_IPCX_API TipcDispatcher;
    friend class T_IPCX_API TipcDispatcherTraverser;
    //@}
  private:
    /**
     * Private constructor to ensure that only TipcDispatcher objects
     * can instantiate this class.
     */
    MessageEvent(MessageEventHandler* handlerObj,
                 MessageEventHandler::Handler handlerMethod)
    : _handlerObj(handlerObj)
    , _handlerMethod(handlerMethod)
    {
    }
    virtual ~MessageEvent() throw()
    {
    }

    MessageEventHandler* _handlerObj;
    MessageEventHandler::Handler _handlerMethod;
  };


  /**
   * Class that represents a socket event.  Socket events
   * are triggered whenever the given socket is ready to be written
   * to or read from.
   *
   * Caution:
   *
   * When an event is created or destroyed from a thread other than
   * the thread dispatching the events, a user event is added to
   * the dispatcher to perform the actual creation or destruction.
   * To ensure that event data, such as a socket or a connection,
   * is not destroyed before the event is removed from the
   * dispatcher, create a user event to destroy the event and to
   * perform any necessary cleanup.
   *
   * This class contains the SocketEventHandler object and method
   * that is called by the TipcDispatcher when this event is triggered.
   * @warning Do not delete this object using the delete operator;
   * call either the TipcEvent::destroy() method, or the
   * TipcDispatcher::destroyEvent() method.
   */

  class T_IPCX_API SocketEvent: public TipcEvent {
  public:
    /// friend class
    //@{
    friend class T_IPCX_API TipcDispatcher;
    friend class T_IPCX_API TipcDispatcherTraverser;

    /**
     * Gets the socket associated with the event.
     * @return socket file descriptor.
     */
    sock getSocket() throw (TipcException)
    {
      sock socketFd = 0;
      if (!TipcEventGetSocket(_event,&socketFd))
        throw TipcEventException();
      return socketFd;
    }

    /**
     * Gets the IO operation associated with the event.
     * @return T_IO_CHECK_READ or T_IO_CHECK_WRITE.
     */
    T_IO_CHECK_MODE getCheckMode() throw (TipcException)
    {
      T_IO_CHECK_MODE checkMode;
      if (!TipcEventGetCheckMode(_event,&checkMode))
        throw TipcEventException();
      return checkMode;
    }

  private:
    /**
     * Private constructor to ensure that only TipcDispatcher objects
     * can instantiate this class.
     */
    SocketEvent(SocketEventHandler* handlerObj,
                SocketEventHandler::Handler handlerMethod)
    : _handlerObj(handlerObj)
    , _handlerMethod(handlerMethod)
    {
    }

    virtual ~SocketEvent() throw()
    {
    }
    SocketEventHandler* _handlerObj;
    SocketEventHandler::Handler _handlerMethod;
  };


  /**
   * Class that represents a peer-to-peer connection event.
   * Connection events are triggered whenever the given
   * TipcConn object is ready to be written
   * to or read from.
   *
   * Caution:
   *
   * When an event is created or destroyed from a thread other than
   * the thread dispatching the events, a user event is added to
   * the dispatcher to perform the actual creation or destruction.
   * To ensure that event data, such as a socket or a connection,
   * is not destroyed before the event is removed from the
   * dispatcher, create a user event to destroy the event and to
   * perform any necessary cleanup.
   *
   * This class contains the ConnEventHandler object and method
   * that is called by the TipcDispatcher when this event is triggered.
   * @warning Do not delete this object using the delete operator;
   * call either the TipcEvent::destroy() method, or the
   * TipcDispatcher::destroyEvent() method.
   */
  class T_IPCX_API ConnEvent: public TipcEvent {
  public:
    /// friend class
    //@{
    friend class T_IPCX_API TipcDispatcher;
    friend class T_IPCX_API TipcDispatcherTraverser;
    //@}
    /**
     * Gets the IO operation associated with the event.
     * @return T_IO_CHECK_READ or T_IO_CHECK_WRITE.
     */
    T_IO_CHECK_MODE getCheckMode() throw (TipcException)
    {
      T_IO_CHECK_MODE checkMode;
      if (!TipcEventGetCheckMode(_event,&checkMode))
        throw TipcEventException();
      return checkMode;
    }

  private:
    /**
     * Private constructor to ensure that only TipcDispatcher objects
     * can instantiate this class.
     */
    ConnEvent(ConnEventHandler* handlerObj,
              ConnEventHandler::Handler handlerMethod)
    : _handlerObj(handlerObj)
    , _handlerMethod(handlerMethod)
    {
    }

    virtual ~ConnEvent() throw()
    {
    }

    ConnEventHandler* _handlerObj;
    ConnEventHandler::Handler _handlerMethod;
  };

  /**
   * Class that represents a timer event that is invoked every
   * interval seconds on the specified TipcDispatcher.  If the
   * timer is to be invoked only once then destroy the event
   * in the event_func.
   *
   * This class contains the TimerEventHandler object and method
   * that is called by the TipcDispatcher when this event is triggered.
   * @warning Do not delete this object using the delete operator;
   * call either the TipcEvent::destroy() method, or the
   * TipcDispatcher::destroyEvent() method.
   */

  class T_IPCX_API TimerEvent: public TipcEvent {
  public:
    /// friend class
    //@{
    friend class T_IPCX_API TipcDispatcher;
    friend class T_IPCX_API TipcDispatcherTraverser;
    //@}
    /**
     * Gets the current timer interval (in seconds) for this
     * event.
     */
    real8 getTimerInterval() throw (TipcException)
    {
      real8 interval;
      if (!TipcEventGetInterval(_event,&interval))
        throw TipcEventException();
      return interval;
    }

    /**
     * Sets the  timer interval (in seconds) for this
     * event.  Subsequent events of this type will start
     * occurring interval number of seconds from this call.
     */
    void setTimerInterval(real8 interval) throw (TipcException)
    {
      if (!TipcEventSetInterval(_event,interval))
        throw TipcEventException();
    }

  private:
    /**
     * Private constructor to ensure that only TipcDispatcher objects
     * can instantiate this class.
     */
    TimerEvent(TimerEventHandler* handlerObj,
               TimerEventHandler::Handler handlerMethod)
    : _handlerObj(handlerObj)
    , _handlerMethod(handlerMethod)
    {
    }
    virtual ~TimerEvent() throw()
    {
    }

    TimerEventHandler* _handlerObj;
    TimerEventHandler::Handler _handlerMethod;
  };

} //namespace SmartSockets

#endif //_T_EVENT_HH
