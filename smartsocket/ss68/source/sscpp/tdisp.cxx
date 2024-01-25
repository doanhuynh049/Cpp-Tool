/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tdisp.cxx#2 $
 */

#include "includes.h"
#include <rtworks/tdisp.h>

namespace SmartSockets {

#define CALL_HANDLER(object,ptrToMember)  ((object).*(ptrToMember))

  TipcDispatcherTraverser::TipcDispatcherTraverser()
  {
#if !defined(T_OS_SOLARIS) || defined(__GNUC__)
    _cbMethod = &TipcDispatcherTraverser::onTraverse;
#else
    _cbMethod = (Handler)this->onTraverse;
#endif
  }

  void TipcDispatcherTraverser::setHandler(TipcDispatcherTraverser::Handler cbMethod)
  {
    _cbMethod = cbMethod;
  }

  TipcDispatcherTraverser::Handler TipcDispatcherTraverser::getHandler()
  {
    return _cbMethod;
  }

  TipcDispatcherTraverser::~TipcDispatcherTraverser() throw()
  {
  }

  bool TipcDispatcherTraverser::onTraverse
  (
  TipcDispatcher& disp, const TipcEvent* event
  )
  {
    return false;
  }

  T_PTR T_ENTRY TipcDispatcherTraverser:: traverseFuncDelegator
  (
  T_IPC_DISPATCHER disp,
  T_IPC_EVENT  event,
  T_PTR arg
  )
  {
    if (arg == NULL)
      throw TipcDispatcherException();

    TipcDispatcherTraverser* cb = (TipcDispatcherTraverser*) arg;

    TipcDispatcherTraverser::Handler method = (cb->getHandler());

    if (method == T_NULL_FUNC)
      throw TipcDispatcherException(T_ERR_NULL_PTR);


    bool retVal = false;


    T_IPC_EVENT_TYPE eventType;

    TipcEventGetType(event,&eventType);

    TipcEvent* eventArg = NULL;
    switch (eventType) {
      case T_IPC_EVENT_USER:
        eventArg = new UserEvent(NULL,NULL);
        break;

      case T_IPC_EVENT_TIMER:
        eventArg = new TimerEvent(NULL,NULL);
        break;

      case T_IPC_EVENT_SOCKET:
        eventArg = new SocketEvent(NULL,NULL);
        break;

      case T_IPC_EVENT_CONN:
        eventArg = new ConnEvent(NULL,NULL);
        break;

      case T_IPC_EVENT_MSG:
        eventArg = new MessageEvent(NULL,NULL);
        break;

      case T_IPC_EVENT_MSG_TYPE:
        eventArg = new MessageEvent(NULL,NULL);
        break;
    }

    eventArg->setEvent(event);

    TipcDispatcher dispArg(disp,false);

    retVal = CALL_HANDLER(*cb,method) (dispArg,eventArg);

    delete eventArg;

    return retVal?NULL:arg;
  }



  /* Dispatch events once for timeout seconds. */
  bool TipcDispatcher::dispatch(real8 timeout) throw (TipcException)
  {
    if (!TipcDispatcherDispatch(_dispatcher, timeout)) {
      if (T_ERR_TIMEOUT_REACHED != TutErrNumGet()) {
        throw TipcDispatcherException();
      }
      return false;
    }

    return true;
  }

  /* Dispatch events continuously for timeout seconds. */
  bool TipcDispatcher::mainLoop(real8 timeout) throw (TipcException)
  {
    // Don't allow mainLoop to be called when started in thread mode.
    if (_isDetached) {
      throw TipcDispatcherException(T_ERR_TYPE_INVALID);
    }
    if (!TipcDispatcherMainLoop(_dispatcher,timeout)) {
      if (T_ERR_TIMEOUT_REACHED==TutErrNumGet()) {
        return false;
      }
      throw TipcDispatcherException();
    }
    return true;
  }

  void TipcDispatcher::eventTraverse
  (
  TipcDispatcherTraverser* travObj
  ) throw (TipcException)
  {
    if (travObj == NULL) {
      throw TipcDispatcherException(T_ERR_NULL_PTR);
    }

    TipcDispatcherTraverser::Handler travMethod = NULL;

    if (travMethod != T_NULL_FUNC) {
      travObj->setHandler(travMethod);
    }

    TutErrNumSet(0);
    if (!TipcDispatcherEventTraverse(_dispatcher,
                                     TipcDispatcherTraverser::traverseFuncDelegator,
                                     (T_PTR)travObj)) {
      int4 errNum = TutErrNumGet();
      if ((errNum!= 0) && (errNum != T_ERR_END_OF_TRAVERSAL)) {
        throw TipcDispatcherException();
      }
    }
  }

} //namespace SmartSockets
