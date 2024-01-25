/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/uttrace.h#1 $
 */

#ifndef _RTWORKS_TUT_H_
#define _RTWORKS_TUT_H_

/*
 * Opaque data types
 */
typedef struct T_UT_TRACE_STRUCT T_UT_TRACE_STRUCT, *T_UT_TRACE;

/*
 * Settable flags within a trace object.
 */
#define T_UT_TRACE_FLAGS_UNDEFINED                    0x00
#define T_UT_TRACE_FLAG_TIMESTAMP                     0x01
#define T_UT_TRACE_FLAG_PREFIX                        0x02
#define T_UT_TRACE_FLAG_LEVEL                         0x04

/*
 * Default trace bindings
 */
#define T_UT_TRACE_LEVEL_OPTION             "trace_level"
#define T_UT_TRACE_FILE_OPTION              "trace_file"
#define T_UT_TRACE_FLAGS_OPTION             "trace_flags"
#define T_UT_TRACE_FILE_SIZE_OPTION         "trace_file_size"

/*
 * Allowable trace levels.
 */
typedef enum {
  T_UT_TRACE_LEVEL_DEFAULT = -2,  /* Default Trace Level. Can be overwritten  */
  T_UT_TRACE_LEVEL_ALWAYS,        /* Display messsages like copyright strings */
  T_UT_TRACE_LEVEL_NEVER,         /* Display no messages                      */
  T_UT_TRACE_LEVEL_ERR,           /* Display error messages                   */
  T_UT_TRACE_LEVEL_WARN,          /* Display warning messages              */
  T_UT_TRACE_LEVEL_INFO,          /* Display information messages                  */
  T_UT_TRACE_LEVEL_INFO_1,        /* Display level 1 information msgs         */
  T_UT_TRACE_LEVEL_INFO_2,        /* Display level 2 information msgs         */ 
  T_UT_TRACE_LEVEL_VERBOSE,       /* Display verbose messages              */ 
  T_UT_TRACE_LEVEL_VERBOSE_1,     /* Display level 1 verbose messages         */ 
  T_UT_TRACE_LEVEL_VERBOSE_2,     /* Display level 2 verbose messages         */ 
  T_UT_TRACE_LEVEL_DBG            /* Displays debug messages                  */
} T_UT_TRACE_LEVEL;


#endif /* _RTWORKS_TUT_H_ */
