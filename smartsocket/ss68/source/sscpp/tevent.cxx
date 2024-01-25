/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tevent.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tevent.h>
#include <rtworks/tdisp.h>

namespace SmartSockets {


  /* Get an event's dispatcher. */
  TipcDispatcher TipcEvent::getDispatcher()
  throw (TipcException)
  {
    T_IPC_DISPATCHER dispatcher_return = NULL;

    if (!TipcEventGetDispatcher(_event, &dispatcher_return))
      throw TipcEventException();

    return TipcDispatcher(dispatcher_return, false);
  }



  /* Get an event's type. */
  T_IPC_EVENT_TYPE TipcEvent::getType() const 
  throw (TipcException)
  {
    T_IPC_EVENT_TYPE type_return;
    if (!TipcEventGetType(_event, &type_return))
      throw TipcEventException();

    return type_return;
  }

} //namespace SmartSockets
