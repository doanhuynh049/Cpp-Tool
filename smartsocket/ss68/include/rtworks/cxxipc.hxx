/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/cxxipc.hxx#2 $
 */

#ifndef _T_CXX_IPC_HH_
#define _T_CXX_IPC_HH_

#include <rtworks/ipc.h>

#ifndef T_IPCX_API
#ifndef T_OS_WIN32
#define T_IPCX_API
#else
#ifndef T_STATIC_LINK
#define T_IPCX_API __declspec(dllimport)
#else
#define T_IPCX_API
#endif /* T_STATIC_LINK */
#endif /* T_OS_WIN32 */
#endif

#include <rtworks/tconn.hxx>
#include <rtworks/tipccw.hxx>
#include <rtworks/tmsgfile.hxx>
#include <rtworks/tmsg.hxx>
#include <rtworks/tmsgmanp.hxx>
#include <rtworks/tmt.hxx>
#include <rtworks/tobj.hxx>
#include <rtworks/tsrv.hxx>
#include <rtworks/tlocclt.hxx>
#include <rtworks/tlocsrv.hxx>
#include <rtworks/ttcpclt.hxx>
#include <rtworks/ttcpsrv.hxx>
#include <rtworks/tmon.hxx>
#include <rtworks/tmonclt.hxx>
#include <rtworks/tmonsrv.hxx>

#endif
