/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tiof/biof/dev/68/src/rtworks/edm.h#2 $
 */

#ifndef _RTWORKS_EDM_H_
#define _RTWORKS_EDM_H_

#include <rtworks/link.h>

/*
 * EDM trace bindings
 */
#define T_EDM_TRACE_LEVEL_OPTION    "_trace_level_edm"
#define T_EDM_TRACE_FILE_OPTION     "_trace_file_edm"

/*
 * EDM Managed Object
 *  TYPE - the type of object being managed
 *  OBJECT - the managed object wrapper
 */
typedef enum {
  T_EDM_OBJECT_LINK,
  T_EDM_OBJECT_CONN,
  T_EDM_OBJECT_SOURCE,
  T_EDM_OBJECT_TIMEOUT,
  T_EDM_OBJECT_EVENT
} T_EDM_OBJECT_TYPE;

typedef struct T_EDM_STRUCT 
               T_EDM_STRUCT, 
              *T_EDM;
typedef struct T_EDM_OBJECT_STRUCT 
               T_EDM_OBJECT_STRUCT,
              *T_EDM_OBJECT;
typedef struct T_EDM_TIMEOUT_STRUCT 
               T_EDM_TIMEOUT_STRUCT,
              *T_EDM_TIMEOUT;
typedef struct T_EDM_EVENT_STRUCT 
               T_EDM_EVENT_STRUCT,
              *T_EDM_EVENT;

typedef struct T_EDM_STATS_STRUCT {
  T_REAL8           create_ts;
  T_UINT4           num_events;               /* #user events queued          */
  T_UINT4           num_events_dispatched;    /* #user events dispatched      */
  T_REAL8           last_event_ts;            /* Last user event timestamp    */
  T_UINT4           num_ios;                  /* #i/o objects queued          */  
  T_UINT4           num_ios_dispatched;       /* #i/o events dispatched       */
  T_REAL8           last_io_ts;               /* Last i/o event timestamp     */
  T_UINT4           num_timeouts;             /* #timeout objects queued      */
  T_UINT4           num_timeouts_dispatched;  /* #timeout events dispatched   */
  T_REAL8           last_timeout_ts;          /* Last timeout event timestamp */
} T_EDM_STATS_STRUCT, *T_EDM_STATS;

typedef struct T_EDM_DISPATCH_DATA_STRUCT {
  T_EDM             edm;
  T_IO_CHECK_MODE   check_mode;
  T_EDM_OBJECT      object;
} T_EDM_DISPATCH_DATA_STRUCT, *T_EDM_DISPATCH_DATA;

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_EDM_DISPATCH_FUNC)
T_TYPEDEF((T_PTR object,
           T_EDM_DISPATCH_DATA data,
           T_PTR arg));

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_EDM_OBJECT_TRAV_FUNC)
T_TYPEDEF((T_EDM,
           T_EDM_OBJECT,
           T_PTR));

#include <rtworks/ipclpr.h>
#include <rtworks/edmpr.h>
#ifdef T_OS_WIN32
#include <rtworks/edmwinpr.h>
#endif
#if defined(T_OS_UNIX) || defined(T_OS_VMS)
#include <rtworks/edmuxpr.h>
#endif

#endif /* _RTWORKS_EDM_H_ */

