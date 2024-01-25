/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/includes.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*                  includes.h -- included in all modules              */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_IPCX_INCLUDES_H_
#define _RTWORKS_IPCX_INCLUDES_H_

#include <rtworks/ipc.h>

namespace SmartSockets {

#define T_CXX_MUTEX_LOCK(mtx)         \
{               \
  if (T_INVALID_MUTEX != (mtx)) {       \
    T_BOOL status = TutMutexLock((mtx), T_TIMEOUT_FOREVER); \
    T_EXPECT(T_TRUE == status);         \
  }               \
}

#define T_CXX_MUTEX_UNLOCK(mtx)         \
{               \
  if (T_INVALID_MUTEX != (mtx)) {       \
    T_BOOL status = TutMutexUnlock((mtx));      \
    T_EXPECT(T_TRUE == status);         \
  }               \
}

#ifndef T_OS_WIN32
#define T_IPCX_API
#define T_IPCX_CB_API
#else
#ifndef T_STATIC_LINK
#define T_IPCX_API __declspec(dllexport)
#define T_IPCX_CB_API __declspec(dllexport)
#else
#define T_IPCX_API
#define T_IPCX_CB_API
#endif /* T_STATIC_LINK */
#endif /* T_OS_WIN32 */

} //namespace SmartSockets

#endif /* _RTWORKS_IPCX_INCLUDES_H_ */
