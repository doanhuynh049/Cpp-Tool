/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tdisp.h#1 $
 */

#ifndef _T_DISPATCHER_HH
#define _T_DISPATCHER_HH

#include <rtworks/ipc.h>
#include <rtworks/tex.h>
#include <rtworks/tevent.h>
#include <rtworks/tsrv.h>
#include <rtworks/tmsg.h>
namespace SmartSockets {

  /**
   * Inherits from TipcException so users can trap TipcDispatcher related
   * exceptions. Offers no additional functionality.
   *
   * @see TipcException, TipcDispatcher
   */
  class T_IPCX_API TipcDispatcherException : public TipcException {

  public:
    /**
     * Constructor.
     */
    TipcDispatcherException ()
    {
    }

    TipcDispatcherException (int4 errorNum)
    :TipcException(errorNum)
    {
    }

    /**
     * Destructor.
     */
    virtual ~TipcDispatcherException () throw()
    {
    }
  };

  class T_IPCX_API TipcDispatcher;
  class T_IPCX_API TipcEvent;


  /**
   * Traverser class used with TipcDispatcher::eventTraverse()
   */
  class T_IPCX_API TipcDispatcherTraverser {
  public:

    /**
     * Signature for the search method.
     * @param disp  dispatcher object.
     * @param event  current event.
     * @return true to continue searching; false otherwise.
     */
    typedef bool (TipcDispatcherTraverser::* Handler)
    (
    TipcDispatcher& disp,
    const TipcEvent* event
    ) ;



    /**
     * Constructor.
     */
    TipcDispatcherTraverser();

    /**
     * Sets the current handler method; if this is not called, the default
     * method onTraverse() is called
     */
    void setHandler(Handler cbMethod);

    /**
     * Gets the current handler method.
     */
    Handler getHandler();


    /**
     * Destructor.
     */
    virtual ~TipcDispatcherTraverser() throw();


    /**
     * Default search method.
     * @param disp  dispatcher object.
     * @param event  current event.
     * @return true to continue searching; false otherwise.
     */
    virtual bool onTraverse (TipcDispatcher& disp, const TipcEvent* event);

    static  T_PTR T_ENTRY traverseFuncDelegator
    (
    T_IPC_DISPATCHER disp,
    T_IPC_EVENT  event,
    T_PTR arg
    );

  private:

    Handler _cbMethod;

  };

  /**
   * The Dispatcher allows one to create events, and trigger the event handlers
   * associated with them.  Currently five types of events are supported:
   *
   * - Timer events - make the dispatcher invoke TimerEventHandlers at a
   * specific interval. All timers are repeating. To simulate a one-shot timer
   * the associated callback function should be coded to remove the  event.
   *
   * - Socket events - make the dispatcher invoke SocketEventHandlers whenever
   * there's data to be read or written from/to a socket.  Socket events give
   * programmers a very similar functionality to the C select() function with
   * an object-oriented abstraction.
   *
   * - Connection events - make the dispatcher invoke ConnectionEventHandlers
   * for read or write availability on peer-to-peer connections.
   *
   * - User events - make the dispatcher invoke UserEventHandlers.  User events
   * are only useful for dispatchers running in detatched mode.  It allows
   * users to invoke methods in the Dispatcher thread.  When a user event is
   * added to a dispatcher, it is immediately queued to be invoked. There is
   * not a trigger per se for user events.
   *
   * - Message events - make the dispatcher invoke MessageEventHandlers either
   * based on a message type or on a subject.  Message events provide the same
   * functionality as process callbacks and subject callbacks in the TipcSrv
   * class.
   *
   * Message events and TipcSrv callbacks, as stated above, have overlap in
   * functionality.  Callbacks are useful for applications that only have a
   * single or few connections to RTserver.  It's not advisable to mix callbacks
   * with events, as they are different paradigms of programming.  If they are
   * mixed, however, the dispatcher will actually invoke all TipcSrv callbacks
   * before invoking the message event handlers associated with a connection.
   *
   * If a programmer chooses to use callbacks rather than events, the
   * dispatcher is still useful; it can be used to service the callbacks of
   * different connections all within the same thread.  In order to achieve this,
   * a call to addInboundConnection() should be made.
   *
   * Multiple dispatchers are allowed within a process. Any number of
   * connections and events may be added to a dispatcher. A dispatcher is
   * thread-safe and may be operated on from any thread.
   */

  class T_IPCX_API TipcDispatcher {
  private:
    bool _isDetached;
    bool _destroyFlag;
    T_IPC_DISPATCHER _dispatcher;

  public:
    /**
     * Constructor compatible with the C API.
     * @param dispatcher C structure
     * @param destroyFlag determines whether or not the destructor destroys
     * the T_IPC_DISPATCHER C structure.
     * @param isDetached determines whether or not the dispatcher structure
     * passed in is running in detatched mode.
     * @throw TipcDispatcherException if the dispatcher structure is NULL.
     */
    TipcDispatcher (T_IPC_DISPATCHER dispatcher,
                    bool destroyFlag = false,
                    bool isDetached = false)
    throw (TipcException)
    :_dispatcher (dispatcher),
    _isDetached(isDetached),
    _destroyFlag(destroyFlag)
    {
      if (_dispatcher == NULL) {
        throw TipcDispatcherException(T_ERR_NULL_PTR);
      }
    }

    /**
     * Creates a dispatcher
     * @param detached - determines whether or not the dispatcher starts
     * processing events in a separate thread
     * @throw TipcDispatcher exception if an error occurs creating the
     * dispatcher object; it's important to note that an exception with
     * the value T_ERR_VAL_INVALID is thrown if a detached dispatcher
     * is started without calling Utilities::initThreads().
     */
    TipcDispatcher (bool isDetached = false)
    throw (TipcException)
    : _isDetached(isDetached)
    , _destroyFlag(true)
    {
      if (_isDetached) {
        if (!TipcThreadEnabled()) {
          throw (TipcDispatcherException(T_ERR_VAL_INVALID));
        }
        _dispatcher = TipcDispatcherCreateDetached();
        if (NULL == _dispatcher) {
          throw TipcDispatcherException();
        }
      }
      else {
        _dispatcher = TipcDispatcherCreate();
        if (NULL == _dispatcher) {
          throw TipcDispatcherException();
        }
      }
    }

    /**
     * Destructor.  Note that the destructor will destroy the C dispatcher
     * structure unless the _destroyFlag is set to false.
     */
    virtual ~TipcDispatcher () throw ()
    {
      if (_destroyFlag) {
        TipcDispatcherDestroy(_dispatcher);
      }
    }

    /**
     * Extracts underlying T_IPC_DISPATCHER handle.
     */
    operator T_IPC_DISPATCHER ()
    const throw ()
    {
      return _dispatcher;
    }

    /**
     * Dispatches one event if there aren't any events available, it waits
     * timeout number of seconds  for one.
     * @param timeout number of seconds to wait for an event to be dispatched.
     */
    bool dispatch(real8 timeout = T_TIMEOUT_FOREVER)
    throw (TipcException);

    /**
     * Dispatch events continuously for timeout seconds.
     * @param timeout number of seconds to dispatch (defaults to infinite)
     * @throw TipcDispatcherException if an error occurred dispatching an
     * event.
     */
    bool mainLoop(real8 timeout = T_TIMEOUT_FOREVER)
    throw (TipcException);


    /**
     * Traverse the events waiting to be dispatched by the dispatcher.
     * @param travObj traversal object derived from the
     * TipcDispatcherTraverse parent class.
     * @throw TipcDispatcherException if a NULL travObj is passed, or
     * if an error occurs during the traversal process.
     */
    void eventTraverse
    (
    TipcDispatcherTraverser* travObj
    ) throw (TipcException);


    /**
     * Creates an event that will be triggered if a message
     * matching the given type reaches the given server connection.  Note
     * that in order for this event to be triggered, at least one Dispatcher
     * in the process should have added the server connection to it; this can
     * be done by  calling the addInboundConnection() method.
     * Note that there's currently a limitation in the C API that mandates that a
     * server connection must be added to at most one dispatcher in the process.
     * @param srv the server connection that will be receiving data.
     * @param mt the message type selector that will determine when the event handler
     * will be triggered.
     * @param handlerObj instance of a child class implementing a message handler method.
     * handler; defaults to the MessageEventHandler::onMessage() method.
     * @see MessageEventHandler::Handler for more details on the signature of the
     * message handler method.
     */
    MessageEvent* createMsgTypeEvent
    (
    const TipcSrv& srv,
    const TipcMt& mt,
    MessageEventHandler* handlerObj
    ) throw (TipcException)
    {
      MessageEventHandler::Handler hndlrMethod =
      &MessageEventHandler::onMessage;
      MessageEvent* event = new MessageEvent(handlerObj,hndlrMethod);

      if (!event)
        throw TipcDispatcherException(T_ERR_NOMEM);

      T_IPC_EVENT cEvent = TipcEventCreateMsgType(_dispatcher,
                                                  (T_IPC_SRV) srv,
                                                  (T_IPC_MT) mt,
                                                  &TipcDispatcher::messageEventDelegator,
                                                  event);
      if (!cEvent)
        throw TipcDispatcherException();

      event->setEvent(cEvent);

      return event;

    }

#if 0
    /**
     * Creates an event that will be triggered if a message that doesn't trigger
     * any other events reaches the given server connection.  Note
     * that in order for this event to be triggered, at least one Dispatcher
     * in the process should have added the server connection to it; this can
     * be done by  calling the addInboundConnection() method.
     * Note that there's currently a limitation in the C API that mandates that a
     * server connection must be added to at most one dispatcher in the process.
     * @param srv the server connection that will be receiving data.
     * @param handlerObj instance of a child class implementing a message handler method.
     * @param handlerMethod method matching the function signature of the message event
     * handler; defaults to the MessageEventHandler::onMessage() method.
     * @see MessageEventHandler::Handler for more details on the signature of the
     * message handler method.
     */
    MessageEvent* createMsgDefaultEvent
    (
    const TipcSrv& srv,
    MessageEventHandler* handlerObj,
    MessageEventHandler::Handler handlerMethod = &MessageEventHandler::onMessage
    ) throw (TipcException)
    {
      MessageEvent* event = new MessageEvent(handlerObj,handlerMethod);

      if (!event)
        throw TipcDispatcherException(T_ERR_NOMEM);

      T_IPC_EVENT cEvent = TipcEventCreateMsgDefault(_dispatcher,
                                                     (T_IPC_SRV) srv,
                                                     messageEventDelegator,
                                                     event);
      if (!cEvent)
        throw TipcDispatcherException();

      event->setEvent(cEvent);

      return event;

    }
#endif

    /**
     * Creates an event that will be triggered if a message that matches the
     * given subject (wildcards will also work) reaches the given server connection.
     * Note that in order for this event to be triggered, at least one Dispatcher
     * in the process should have added the server connection to it; this can
     * be done by  calling the addInboundConnection() method.
     * Note that there's currently a limitation in the C API that mandates that a
     * server connection must be added to at most one dispatcher in the process.
     * @param srv the server connection that will be receiving data.
     * @param handlerObj instance of a child class implementing a message handler method.
     * @see MessageEventHandler::Handler for more details on the signature of the
     * message handler method.
     */
    MessageEvent* createMsgSubjEvent
    (
    const TipcSrv& srv,
    const char* subject,
    MessageEventHandler* handlerObj
    ) throw (TipcException)
    {
      MessageEventHandler::Handler handlerMethod =
      &MessageEventHandler::onMessage;
      MessageEvent* event = new MessageEvent(handlerObj,handlerMethod);

      if (!event)
        throw TipcDispatcherException(T_ERR_NOMEM);

      T_IPC_EVENT cEvent = TipcEventCreateMsg(_dispatcher,
                                              (T_IPC_SRV) srv,
                                              const_cast <char*>(subject),
                                              messageEventDelegator,
                                              event);
      if (!cEvent)
        throw TipcDispatcherException();

      event->setEvent(cEvent);

      return event;

    }


    /**
     * Creates an event that will be periodically triggered every interval.
     * number of seconds.  To simulate a one time event, call the
     * TipcEvent::destroy() method once the event has been triggered once.
     * @param interval the number of seconds between events.
     * @param handlerObj instance of a child class implementing a timer event
     * handler method.
     * @param handlerMethod method matching the function signature of the
     * timer event handler; defaults to the TimerEventHandler::onTimer() method.
     */
    TimerEvent* createTimerEvent
    (
    real8 interval,
    TimerEventHandler* handlerObj
    ) throw (TipcException)
    {
      TimerEventHandler::Handler handlerMethod = &TimerEventHandler::onTimer;
      TimerEvent* event = new TimerEvent(handlerObj,handlerMethod);

      if (!event)
        throw TipcDispatcherException(T_ERR_NOMEM);

      T_IPC_EVENT cEvent = TipcEventCreateTimer(_dispatcher,
                                                interval,
                                                timerEventDelegator,
                                                event);
      if (!cEvent)
        throw TipcDispatcherException();

      event->setEvent(cEvent);

      return event;
    }

    /**
     * Creates an event that will be triggered whenever there's data ready
     * to be read or written to a TipcConn object.  Note that unlike the
     * Message events, there's no need to add a peer to peer connection to
     * the dispatcher.
     * @param connection the peer-to-peer object to be monitored for read or
     * write events.
     * @param checkMode if set to T_IO_CHECK_READ, triggers the ConnEventHandler
     * object if data is ready to be read from the connection; if set to
     * T_IO_CHECK_WRITE, triggers the ConnEventHandler object if data is ready
     * to be written to the connection.
     * @param handlerObj instance of a child class implementing a conn event
     * handler method.
     * @see ConnEventHandler::Handler for more details on the signature of the
     * conn event handler method.
     */
    ConnEvent* createConnEvent
    (
    const TipcConn& connection,
    T_IO_CHECK_MODE checkMode,
    ConnEventHandler* handlerObj
    ) throw (TipcException)
    {
      ConnEventHandler::Handler handlerMethod =
      &ConnEventHandler::onConnEvent;
      ConnEvent* event = new ConnEvent(handlerObj,handlerMethod);

      if (!event)
        throw TipcDispatcherException(T_ERR_NOMEM);

      T_IPC_EVENT cEvent = TipcEventCreateConn(_dispatcher,
                                               (T_IPC_CONN)connection,
                                               checkMode,
                                               &TipcDispatcher::connEventDelegator,
                                               event);
      if (!cEvent)
        throw TipcDispatcherException();

      event->setEvent(cEvent);

      return event;
    }

    /**
     * Creates an event that will be triggered whenever there's data ready
     * to be read or written to a Socket.  Note that unlike the
     * Message events, there's no need to add a socket to
     * the dispatcher.
     * @param socketFd the file descriptor of the socket to be monitored for
     * read or write events.
     * @param checkMode if set to T_IO_CHECK_READ, triggers the ConnEventHandler
     * object if data is ready to be read from the socket; if set to
     * T_IO_CHECK_WRITE, triggers the ConnEventHandler object if data is ready
     * to be written to the socket.
     * @param handlerObj instance of a child class implementing a socket event
     * handler method.
     * @see SocketEventHandler::Handler for more details on the signature of the
     * socket event handler method.
     */
    SocketEvent* createSocketEvent
    (
    sock socketFd,
    T_IO_CHECK_MODE checkMode,
    SocketEventHandler* handlerObj
    ) throw (TipcException)
    {
      SocketEventHandler::Handler handlerMethod =
      &SocketEventHandler::onSocket;
      SocketEvent* event = new SocketEvent(handlerObj,handlerMethod);

      if (!event)
        throw TipcDispatcherException(T_ERR_NOMEM);

      T_IPC_EVENT cEvent = TipcEventCreateSocket(_dispatcher,
                                                 socketFd,
                                                 checkMode,
                                                 &TipcDispatcher::socketEventDelegator,
                                                 event);
      if (!cEvent)
        throw TipcDispatcherException();

      event->setEvent(cEvent);

      return event;
    }

    /**
     * Creates an event that will be triggered immediately.  User events
     * are only useful for dispatchers running in detatched mode.  It allows
     * users to invoke methods in the Dispatcher thread.  When a user event is
     * added to a dispatcher, it is immediately queued to be invoked. There is
     * not a trigger per se for user events.
     * @param data to be retrieved in the UserEventHandler::Handler method.
     * @param handlerObj instance of a child class implementing a socket event
     * handler method.
     */
    UserEvent* createUserEvent
    (
    void* data,
    UserEventHandler* handlerObj
    ) throw (TipcException)
    {
      UserEventHandler::Handler handlerMethod= &UserEventHandler::onUserEvent;
      UserEvent* event = new UserEvent(handlerObj,handlerMethod);

      if (!event)
        throw TipcDispatcherException(T_ERR_NOMEM);

      T_IPC_EVENT cEvent = TipcEventCreate(_dispatcher,
                                           data,
                                           &TipcDispatcher::userEventDelegator,
                                           event);
      if (!cEvent)
        throw TipcDispatcherException();

      event->setEvent(cEvent);

      return event;
    }


    /**
     * Unregisters and destroys a event created by one the create methods.
     */
    void destroyEvent (TipcEvent* event)
    throw (TipcException)
    {
      try {
        event->destroy();

        event = NULL;
      }
      catch (Exception& e) {
        throw TipcDispatcherException(e.getErrNum());
      }

    }


    /**
     * Adds a connection to the dispatcher.  The connection added will behave
     * as a source for messages.  Messages coming from this connection will
     * be read and processed by invoking the approriate connection callback
     * (e.g. a ProcessCallback, a SubjectCallback, etc.  Note that currently
     * there's a limitation in the C API that enforces the fact that one
     * connection can only belong to one dispatcher.
     * @param srv TipcSrv connection object to be added.
     * @throw TipcDispatcherException if the connection is already present
     * or if there's a failure adding it to the dispatcher.
     */
    void addInboundConnection(const TipcSrv& srv)
    throw (TipcException)
    {
      T_IPC_DISPATCHER disp;
      if (!TipcSrvConnGetDispatcher((T_IPC_SRV)srv,&disp)) {
        throw TipcDispatcherException();
      }
      if (disp != NULL) {
        throw(TipcDispatcherException(T_ERR_ALREADY_EXISTS));
      }
      if (!TipcSrvConnSetDispatcher((T_IPC_SRV)srv,_dispatcher)) {
        throw TipcDispatcherException();
      }
    }

    /**
     * Removes a connection from the dispatcher.
     * @param srv reference to the TipcSrv connection object to be removed.
     * @throw TipcDispatcher exception if the server connection is not
     * registered with this dispatcher, or if there's a failure in removing
     * the connection.
     */
    void removeInboundConnection(const TipcSrv& srv)
    throw (TipcException)
    {
      T_IPC_DISPATCHER disp;
      if (!TipcSrvConnGetDispatcher((T_IPC_SRV)srv,&disp)) {
        if (disp != _dispatcher) {
          throw(TipcDispatcherException(T_ERR_DOESNT_EXIST));
        }
      }
      if (!TipcSrvConnSetDispatcher((T_IPC_SRV)srv,NULL)) {
        throw TipcDispatcherException();
      }
    }

  private:

    static void T_ENTRY messageEventDelegator (T_IPC_EVENT eventParam,
                                               T_IPC_EVENT_DATA eventData,
                                               T_PTR arg)
    {
      MessageEvent* event = (MessageEvent*) arg;
      TipcMsg msg(eventData->msg);

      ((event->_handlerObj)->*(event->_handlerMethod)) (*event,msg);

    }


    static void T_ENTRY socketEventDelegator (T_IPC_EVENT eventParam,
                                              T_IPC_EVENT_DATA eventData,
                                              T_PTR arg)
    {
      SocketEvent* event = (SocketEvent*) arg;
      sock socketFd = eventData->socket;
      T_IO_CHECK_MODE checkMode = eventData->check_mode;

      ((event->_handlerObj)->*(event->_handlerMethod)) (*event,
                                                        socketFd,
                                                        checkMode);

    }


    static void T_ENTRY userEventDelegator (T_IPC_EVENT eventParam,
                                            T_IPC_EVENT_DATA eventData,
                                            T_PTR arg)
    {
      UserEvent* event = (UserEvent*) arg;
      void* data = eventData->data;

      ((event->_handlerObj)->*(event->_handlerMethod)) (*event,
                                                        data);

    }



    static void T_ENTRY connEventDelegator (T_IPC_EVENT eventParam,
                                            T_IPC_EVENT_DATA eventData,
                                            T_PTR arg)
    {
      ConnEvent* event = (ConnEvent*) arg;
      TipcConn conn (eventData->conn,false);
      T_IO_CHECK_MODE checkMode = eventData->check_mode;

      ((event->_handlerObj)->*(event->_handlerMethod)) (*event,
                                                        conn,
                                                        checkMode);

    }



    static void T_ENTRY timerEventDelegator (T_IPC_EVENT eventParam,
                                             T_IPC_EVENT_DATA eventData,
                                             T_PTR arg)
    {
      TimerEvent* event = (TimerEvent*) arg;
      real8 interval = eventData->interval;

      ((event->_handlerObj)->*(event->_handlerMethod)) (*event,
                                                        interval);

    }

  };
} // namespace SmartSockets


#endif //_T_DISPATCHER_HH

