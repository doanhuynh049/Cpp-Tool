/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/sscpp.h#1 $
 */

/** 
 * @file sscpp.h
 * This file is a convenience that includes all header files needed to use the
 * SmartSockets C++ class library. 
 */


#ifndef _T_SSCPP_HH_
#define _T_SSCPP_HH_

#include <rtworks/ipc.h>

#ifndef T_IPCX_API

#ifndef T_OS_WIN32
#define T_IPCX_API
#define T_IPCX_CB_API
#else

#ifndef T_STATIC_LINK
#define T_IPCX_API __declspec(dllimport)
#ifdef T_OS_WINDOWS_VC6
#define T_IPCX_CB_API __declspec(dllimport)
#else
#define T_IPCX_CB_API
#endif /* T_OS_WINDOWS_VC6 */

#else
#define T_IPCX_API
#define T_IPCX_CB_API
#endif /* T_STATIC_LINK */
#endif /* T_OS_WIN32 */
#endif /* T_IPCX_API */

#include <rtworks/tcallbck.h>
#include <rtworks/tconn.h>
#include <rtworks/tdisp.h>
#include <rtworks/tevent.h>
#include <rtworks/tex.h>
#include <rtworks/texnames.h>
#include <rtworks/tmon.h>
#include <rtworks/tmonext.h>
#include <rtworks/tmsg.h>
#include <rtworks/tmsgfile.h>
#include <rtworks/tmsgname.h>
#include <rtworks/tmsgsize.h>
#include <rtworks/tmt.h>
#include <rtworks/toption.h>
#include <rtworks/tsrv.h>
#include <rtworks/tsystem.h>
#include <rtworks/tutil.h>
#include <rtworks/types.h>
#include <rtworks/txml.h>
#include <rtworks/tscache.h>

#endif //_T_SSCPP_HH_

