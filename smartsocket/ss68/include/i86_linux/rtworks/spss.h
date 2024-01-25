/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/dpsi/dev/68/src/rtworks/spss.h#1 $
 */

#ifndef _RTWORKS_SPSS_H_
#define _RTWORKS_SPSS_H_

#include <rtworks/ipc.h>

/*
 * Error codes
 */
#define T_ERR_SPSS_STATE_INVALID                  (T_ERR_SPSS_START + 0)
#define T_ERRNM_SPSS_STATE_INVALID                "invalid SPSS state"
#define T_ERR_SPSS_OUTBOUND_MSGQ_FULL             (T_ERR_SPSS_START + 1) 
#define T_ERRNM_SPSS_OUTBOUND_MSGQ_FULL           "outbound SPSS msgQ full" 

/*                                              
 * Callback type numbers.                       
 */                                             
#define T_SPSS_EVENT_CB_NUM                       (T_LOBE_SPSS_START - 0)

/*
 * Message types 
 */

/*
 * SPSS trace bindings
 */
#define T_SPSS_TRACE_LEVEL_OPTION                 "_trace_level_spss"
#define T_SPSS_TRACE_FILE_OPTION                  "_trace_file_spss"

/*
 * SPSS states
 */
typedef enum {
  T_SPSSS_CLOSED,
  T_SPSSS_OPENING,
  T_SPSSS_OPEN
} T_SPSS_STATE;
 
/* 
 * SPSS callback events 
 */ 
typedef enum {
  T_SPSSE_OPEN,                /* 'Session open and ready' event   */
  T_SPSSE_READ,                /* 'Msg pending to be read' event   */
  T_SPSSE_WRITE,               /* 'Msg ready to write' event       */
  T_SPSSE_CLOSE                /* 'Session closing' event          */
} T_SPSS_EVENT;

/*
 * SPSS IOQ node types
 */
typedef enum {
  T_SPSS_QNT_SUBSCRIBE_REQUEST,
  T_SPSS_QNT_UNSUBSCRIBE_REQUEST,
  T_SPSS_QNT_MSG
} T_SPSS_IOQ_NODE_TYPE;

/*
 * Opaque data types.
 */
typedef struct T_SPSS_SUBJECT_STRUCT             T_SPSS_SUBJECT_STRUCT, 
                                                *T_SPSS_SUBJECT;
typedef struct T_SPSS_STAT_STRUCT                T_SPSS_STAT_STRUCT, 
                                                *T_SPSS_STAT;
typedef struct T_SPSS_IOQ_NODE_STRUCT            T_SPSS_IOQ_NODE_STRUCT, 
                                                *T_SPSS_IOQ_NODE;
typedef struct T_SPSS_STRUCT                     T_SPSS_STRUCT, 
                                                *T_SPSS;

/*                                              
 * Callback data fields.                        
 */                                             
typedef struct T_SPSS_EVENT_CB_DATA_STRUCT       T_SPSS_EVENT_CB_DATA_STRUCT, 
                                                *T_SPSS_EVENT_CB_DATA;
struct T_SPSS_EVENT_CB_DATA_STRUCT {            
  T_CB_DATA_STRUCT    cb_data;                  
  T_SPSS_EVENT        event;                    
};                                              

typedef struct T_SPSS_OPEN_EVENT_CB_DATA_STRUCT  T_SPSS_OPEN_EVENT_CB_DATA_STRUCT, 
                                                *T_SPSS_OPEN_EVENT_CB_DATA;
struct T_SPSS_OPEN_EVENT_CB_DATA_STRUCT {       
  T_SPSS_EVENT_CB_DATA_STRUCT     event_data;   
  T_BOOL                          status;       
};                                              
typedef T_SPSS_EVENT_CB_DATA_STRUCT              T_SPSS_READ_EVENT_CB_DATA_STRUCT, 
                                                *T_SPSS_READ_EVENT_CB_DATA;
typedef T_SPSS_EVENT_CB_DATA_STRUCT              T_SPSS_WRITE_EVENT_CB_DATA_STRUCT, 
                                                *T_SPSS_WRITE_EVENT_CB_DATA;
typedef T_SPSS_EVENT_CB_DATA_STRUCT              T_SPSS_CLOSE_EVENT_CB_DATA_STRUCT, 
                                                *T_SPSS_CLOSE_EVENT_CB_DATA;

/*
 * Callback function types.
 */
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SPSS_EVENT_CB_FUNC)
T_TYPEDEF((T_SPSS,
           T_SPSS_EVENT_CB_DATA,
           T_CB_ARG));

#include <rtworks/spsspr.h>

#endif /* _RTWORKS_SPSS_H_ */


