/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/ipc.h#1 $
 */

/* =================================================================== */
/*                                                                     */
/*                ipc.h -- interprocess communication                  */
/*                                                                     */
/* =================================================================== */
 
#ifndef _RTWORKS_IPC_H_
#define _RTWORKS_IPC_H_
 
#include <rtworks/common.h>
#include <rtworks/ipcstub.h>
#include <rtworks/sm.h>

/* 
 * T_IPC_SRV change reasons for change callbacks.
 */
typedef enum {
  T_IPC_SRV_CHANGE_REASON_CREATED,   /* when a T_IPC_SRV object is created   */
  T_IPC_SRV_CHANGE_REASON_DESTROYED  /* when a T_IPC_SRV object is destroyed */
} T_IPC_SRV_CHANGE_REASON;
 
/* 
 * RTserver connection status returned by TipcSrvGetConnStatus. 
 */
typedef enum {
  T_IPC_SRV_CONN_NONE,  /* not connected at all */
  T_IPC_SRV_CONN_WARM,  /* warm connection to RTserver */
  T_IPC_SRV_CONN_FULL   /* fully connected to RTserver */
} T_IPC_SRV_CONN_STATUS;
 
/* 
 * RTserver action to take when RTclient disconnects. 
 */
typedef enum {
  T_IPC_SRV_DISCONNECT_GMD_FAILURE,   /* failure on pending recv GMD */
  T_IPC_SRV_DISCONNECT_WARM,          /* keep client warm */
  T_IPC_SRV_DISCONNECT_GMD_SUCCESS    /* success on pending recv GMD */
} T_IPC_SRV_DISCONNECT_MODE;

/* 
 * RTserver message file logging categories (controlled by the options 
 * log_{in,out}_<category>) for TipcSrvLog{Add,Remove}Mt.              
 */
typedef enum {
  T_IPC_SRV_LOG_DATA = T_MT_LOG_DATA,     /* variable values and time */
  T_IPC_SRV_LOG_STATUS = T_MT_LOG_STATUS,   /* RThci status messages */
  T_IPC_SRV_LOG_INTERNAL = T_MT_LOG_INTERNAL /* all other standard message types */
} T_IPC_SRV_LOG_TYPE;
 
/* 
 * Modes for stopping RTserver. 
 */
typedef enum {
  T_IPC_SRV_STOP_NORMAL = 1,  /* just stop one RTserver */
  T_IPC_SRV_STOP_ALL,         /* stop all RTservers and RTclients */
  T_IPC_SRV_STOP_CLIENTS,     /* stop one RTserver and its RTclients */
  T_IPC_SRV_STOP_SERVERS      /* stop all RTservers */
} T_IPC_SRV_STOP_TYPE;
 
/* ---------------------------------------------------- */
/* Load balancing constants used in more than one place */
/* ---------------------------------------------------- */
#define T_IPC_SUBSCRIBE_LB_OFF    "load_balancing_off"

#define T_IPC_SRV_GMD_BASE_NAME   "rtserver" /* RTclient GMD files */
 
#define T_IPC_SUBJECT_SERVER      "_server"
#define T_IPC_SUBJECT_ALL         "_all"
#define T_IPC_SUBJECT_ARCHIVE     "_archive"
#define T_IPC_SUBJECT_CLIENT      "_client"
#define T_IPC_SUBJECT_DAQ         "_daq"
#define T_IPC_SUBJECT_HCI         "_hci"
#define T_IPC_SUBJECT_IE          "_ie"
#define T_IPC_SUBJECT_PLAYBACK    "_playback"
#define T_IPC_SUBJECT_TIME        "_time"
#define T_IPC_SUBJECT_DEFAULT     "_default"
#define T_IPC_SUBJECT_GROUP       "_group"
 
/* 
 * T_IPC_MON_ALL can be used in many TipcMon functions and T_MT_MON   
 * message types to specify a global action (e.g., for all subjects). 
 */
#define T_IPC_MON_ALL               "@"
#define T_TOK_IPC_MON_ALL           T_TOK_AT /* "@" */
#ifdef T_RTWORKS35_COMPAT
#define T_IPC_MON_ALL35             "*"
#define T_TOK_IPC_MON_ALL35         T_TOK_TIMES /* "*" */
#endif
 
/* 
 * This define probably belongs in ipc_dev.h, but it is placed 
 * here so it could be kept consistent with other defines      
 * based on T_LOBE_IPC_START.  RES 10/30/93.                   
 */
#define T_IPC_GUI_OPTION_CB_NUM     (T_LOBE_IPC_START - 20)
#define T_IPC_SRV_CREATE_CB_NUM     (T_LOBE_IPC_START - 30)
#define T_IPC_SRV_DESTROY_CB_NUM    (T_LOBE_IPC_START - 31)
#define T_IPC_SRV_SUBJECT_CB_NUM    (T_LOBE_IPC_START - 32)
#define T_IPC_SRV_TRAVERSE_CB_NUM   (T_LOBE_IPC_START - 33)
#define T_IPC_SRV_OPEN_CB_NUM       (T_LOBE_IPC_START - 34)
#define T_IPC_SRV_CLOSE_CB_NUM      (T_LOBE_IPC_START - 35)
#define T_IPC_SRV_CHANGE_CB_NUM     (T_LOBE_IPC_START - 36)

/*
 * T_IPC_SRV is the handle of a connection to a Talarian server.
 */
typedef struct T_IPC_SRV_STRUCT
               T_IPC_SRV_STRUCT,
              *T_IPC_SRV;

/*
 * T_IPC_EVENT events.
 */
typedef enum {
  T_IPC_EVENT_USER,
  T_IPC_EVENT_TIMER,
  T_IPC_EVENT_SOCKET,
  T_IPC_EVENT_CONN,
  T_IPC_EVENT_MSG,
  T_IPC_EVENT_MSG_TYPE
} T_IPC_EVENT_TYPE;

typedef struct T_IPC_EVENT_STRUCT
               T_IPC_EVENT_STRUCT,
              *T_IPC_EVENT;

typedef struct T_IPC_EVENT_DATA_STRUCT {
  T_IPC_EVENT_TYPE type;
  T_PTR            data;
  T_SOCK           socket;
  T_IO_CHECK_MODE  check_mode;
  T_REAL8          interval;
  T_IPC_CONN       conn;
  T_IPC_MSG        msg;
} T_IPC_EVENT_DATA_STRUCT, *T_IPC_EVENT_DATA;

/*
 * T_IPC_EVENT_FUNC dispatch function for events.
 */
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_EVENT_FUNC)
T_TYPEDEF((T_IPC_EVENT, T_IPC_EVENT_DATA, T_PTR));
 
/*
 * T_IPC_DISPATCHER is used to dispatch events.
 */
typedef struct T_IPC_DISPATCHER_STRUCT
               T_IPC_DISPATCHER_STRUCT,
              *T_IPC_DISPATCHER;

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_DISPATCHER_EVENT_TRAV_FUNC)
T_TYPEDEF((T_IPC_DISPATCHER, T_IPC_EVENT, T_PTR));

/*
 * T_IPC_SUBJECT is used for holding information about a subject to
 * which a client is subscribing
 */
typedef struct T_IPC_SUBJECT_STRUCT 
               T_IPC_SUBJECT_STRUCT, 
              *T_IPC_SUBJECT;
struct T_IPC_SUBJECT_STRUCT {
  T_STR   name;
  T_BOOL  lb_on; /* load balancing is ok for this subject */
};

typedef struct T_IPC_SRV_CREATE_CB_DATA_STRUCT 
               T_IPC_SRV_CREATE_CB_DATA_STRUCT,
              *T_IPC_SRV_CREATE_CB_DATA;
struct T_IPC_SRV_CREATE_CB_DATA_STRUCT {
  T_CB cb;
  T_IPC_SRV_CONN_STATUS old_conn_status; /* old connection status */
  T_IPC_SRV_CONN_STATUS new_conn_status; /* new connection status */
};

typedef struct T_IPC_SRV_DESTROY_CB_DATA_STRUCT
               T_IPC_SRV_DESTROY_CB_DATA_STRUCT, 
              *T_IPC_SRV_DESTROY_CB_DATA;
struct T_IPC_SRV_DESTROY_CB_DATA_STRUCT {
  T_CB cb;
  T_IPC_SRV_CONN_STATUS old_conn_status; /* old connection status */
  T_IPC_SRV_CONN_STATUS new_conn_status; /* new connection status */
};

typedef struct T_IPC_SRV_TRAVERSE_CB_DATA_STRUCT
               T_IPC_SRV_TRAVERSE_CB_DATA_STRUCT, 
              *T_IPC_SRV_TRAVERSE_CB_DATA;
struct T_IPC_SRV_TRAVERSE_CB_DATA_STRUCT {
  T_CB cb;
  T_STR server_name;    /* next server name in the list */
  T_BOOL stop_traverse; /* set by callback to stop traversing */
  T_IPC_SRV srv;        /* entity attempting connection to RTserver */
};

typedef T_IPC_CONN_MSG_CB_DATA_STRUCT 
        T_IPC_SRV_SUBJECT_CB_DATA_STRUCT,
       *T_IPC_SRV_SUBJECT_CB_DATA;

typedef struct T_IPC_SRV_OPEN_CB_DATA_STRUCT 
               T_IPC_SRV_OPEN_CB_DATA_STRUCT,
              *T_IPC_SRV_OPEN_CB_DATA;
struct T_IPC_SRV_OPEN_CB_DATA_STRUCT {
  T_CB cb;
  T_IPC_SRV_CONN_STATUS old_conn_status; /* old connection status */
  T_IPC_SRV_CONN_STATUS new_conn_status; /* new connection status */
};

typedef struct T_IPC_SRV_CLOSE_CB_DATA_STRUCT 
               T_IPC_SRV_CLOSE_CB_DATA_STRUCT,
              *T_IPC_SRV_CLOSE_CB_DATA;
struct T_IPC_SRV_CLOSE_CB_DATA_STRUCT {
  T_CB cb;
  T_IPC_SRV_CONN_STATUS old_conn_status; /* old connection status */
  T_IPC_SRV_CONN_STATUS new_conn_status; /* new connection status */
};

typedef struct T_IPC_SRV_CHANGE_CB_DATA_STRUCT 
               T_IPC_SRV_CHANGE_CB_DATA_STRUCT,
              *T_IPC_SRV_CHANGE_CB_DATA;
struct T_IPC_SRV_CHANGE_CB_DATA_STRUCT {
  T_CB cb;
  T_IPC_SRV_CHANGE_REASON reason;
};

/* 
 * T_IPC_SRV_SUBJECT_TRAV_FUNC is for traversing a list of subjects. 
 */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_SRV_SUBJECT_TRAV_FUNC)
T_TYPEDEF((T_STR, T_STR, T_PTR));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_SRV_CREATE_CB_FUNC)
T_TYPEDEF((T_IPC_CONN, T_IPC_SRV_CREATE_CB_DATA, T_CB_ARG));
 
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_SRV_DESTROY_CB_FUNC)
T_TYPEDEF((T_IPC_CONN, T_IPC_SRV_DESTROY_CB_DATA, T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_SRV_TRAVERSE_CB_FUNC)
T_TYPEDEF((T_IPC_CONN,          /* conn */
           T_IPC_SRV_TRAVERSE_CB_DATA, /* data */
           T_CB_ARG));          /* arg */

typedef T_IPC_CONN_MSG_CB_FUNC T_IPC_SRV_SUBJECT_CB_FUNC;
 
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_SRV_OPEN_CB_FUNC)
T_TYPEDEF((T_IPC_SRV, T_IPC_SRV_OPEN_CB_DATA, T_CB_ARG));
 
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_SRV_CLOSE_CB_FUNC)
T_TYPEDEF((T_IPC_SRV, T_IPC_SRV_CLOSE_CB_DATA, T_CB_ARG));
 
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_SRV_CHANGE_CB_FUNC)
T_TYPEDEF((T_IPC_SRV, T_IPC_SRV_CHANGE_CB_DATA, T_CB_ARG));
 
/* 
 * The callback arg of TipcDevRecvDoQuit is now an exit function 
 * DCG 11/23/93                                                  
 */
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_EXIT_FUNC)
T_TYPEDEF((void));
 
/*
 * These empty macros are used to control reflection. --KLC 6/13/98
 */
#ifndef T_NO_REFLECT
#define T_REFLECT_FILE(hfile) extern int eat_the_semicolon
#define T_REFLECT_STRUCT(name,tags) struct name
#define T_REFLECT_NA(decl) decl
#define T_REFLECT_RO(decl) decl
#define T_REFLECT_RW(decl) decl /* The default */
#define T_REFLECT_WO(decl) decl
#endif
 
#include <rtworks/mt.h> /* message types */
 
/*
 * Macros for compatibility with 3.5.  We wrote Tipc*21 functions for
 * 3.0 (which was a disaster), and wrote a few compat functions for
 * 3.5, and now try some easy macros so we don't even have to write
 * compat functions.  This will hopefully suffice (will probably not
 * work for Ada, Perl, or anything else that doesn't go through the C
 * preprocessor.  SMW 06/18/96
 */
#ifdef T_RTWORKS35_COMPAT
/*
 * Documented defines.
 */
#define T_MT_DG_SET_RECV  T_MT_SUBJECT_SET_SUBSCRIBE
#define T_MT_DG_RETRIEVE  T_MT_SUBJECT_RETRIEVE
 
#define T_MT_MON_APP_NAMES_SET_WATCH      T_MT_MON_PROJECT_NAMES_SET_WATCH
#define T_MT_MON_APP_NAMES_STATUS         T_MT_MON_PROJECT_NAMES_STATUS
#define T_MT_MON_DG_NAMES_SET_WATCH       T_MT_MON_SUBJECT_NAMES_SET_WATCH
#define T_MT_MON_DG_NAMES_STATUS          T_MT_MON_SUBJECT_NAMES_STATUS
#define T_MT_MON_DG_RECV_SET_WATCH        T_MT_MON_SUBJECT_SUBSCRIBE_SET_WATCH
#define T_MT_MON_DG_RECV_STATUS           T_MT_MON_SUBJECT_SUBSCRIBE_STATUS
#define T_MT_MON_CLIENT_RECV_SET_WATCH    T_MT_MON_CLIENT_SUBSCRIBE_SET_WATCH
#define T_MT_MON_CLIENT_RECV_STATUS       T_MT_MON_CLIENT_SUBSCRIBE_STATUS
                                          
#define T_MT_MON_APP_NAMES_POLL_RESULT    T_MT_MON_PROJECT_NAMES_POLL_RESULT
#define T_MT_MON_APP_NAMES_POLL_CALL      T_MT_MON_PROJECT_NAMES_POLL_CALL
#define T_MT_MON_DG_NAMES_POLL_RESULT     T_MT_MON_SUBJECT_NAMES_POLL_RESULT
#define T_MT_MON_DG_NAMES_POLL_CALL       T_MT_MON_SUBJECT_NAMES_POLL_CALL
#define T_MT_MON_DG_RECV_POLL_RESULT      T_MT_MON_SUBJECT_SUBSCRIBE_POLL_RESULT
#define T_MT_MON_DG_RECV_POLL_CALL        T_MT_MON_SUBJECT_SUBSCRIBE_POLL_CALL
#define T_MT_MON_CLIENT_RECV_POLL_RESULT  T_MT_MON_CLIENT_SUBSCRIBE_POLL_RESULT
#define T_MT_MON_CLIENT_RECV_NUM_POLL_RESULT T_MT_MON_CLIENT_SUBSCRIBE_NUM_POLL_RESULT
#define T_MT_MON_CLIENT_RECV_POLL_CALL    T_MT_MON_CLIENT_SUBSCRIBE_POLL_CALL
#define T_MT_MON_CLIENT_DG_POLL_RESULT    T_MT_MON_CLIENT_SUBJECT_POLL_RESULT
#define T_MT_MON_CLIENT_DG_POLL_CALL      T_MT_MON_CLIENT_SUBJECT_POLL_CALL
 
/*
 * Undocumented defines.
 */                             
#define T_IPC_DG_SERVER         T_IPC_SUBJECT_SERVER
#define T_IPC_DG_ALL            T_IPC_SUBJECT_ALL
#define T_IPC_DG_ARCHIVE        T_IPC_SUBJECT_ARCHIVE
#define T_IPC_DG_CLIENT         T_IPC_SUBJECT_CLIENT
#define T_IPC_DG_DAQ            T_IPC_SUBJECT_DAQ
#define T_IPC_DG_HCI            T_IPC_SUBJECT_HCI
#define T_IPC_DG_IE             T_IPC_SUBJECT_IE
#define T_IPC_DG_PLAYBACK       T_IPC_SUBJECT_PLAYBACK
#define T_IPC_DG_TIME           T_IPC_SUBJECT_TIME
#define T_MT_ARC_PB_DG_CREATE   T_MT_ARC_PB_SUBJECT_CREATE
 
/*
 * Documented typedefs.
 */
#define T_IPC_SRV_DG_TRAV_FUNC T_IPC_SRV_SUBJECT_TRAV_FUNC
 
/*
 * Documented API functions.
 */
#define TipcConnGetUniqueDatagroup  TipcConnGetUniqueSubject
#define TipcSrvGetUniqueDatagroup   TipcSrvGetUniqueSubject
 
#define TipcMonAppNamesGetWatch     TipcMonProjectNamesGetWatch
#define TipcMonAppNamesSetWatch     TipcMonProjectNamesSetWatch
#define TipcMonDgNamesGetWatch      TipcMonSubjectNamesGetWatch
#define TipcMonDgNamesSetWatch      TipcMonSubjectNamesSetWatch
#define TipcMonDgRecvGetWatch       TipcMonSubjectSubscribeGetWatch
#define TipcMonDgRecvSetWatch       TipcMonSubjectSubscribeSetWatch
#define TipcMonClientRecvGetWatch   TipcMonClientSubscribeGetWatch
#define TipcMonClientRecvSetWatch   TipcMonClientSubscribeSetWatch
 
#define TipcMonAppNamesPoll         TipcMonProjectNamesPoll
#define TipcMonDgNamesPoll          TipcMonSubjectNamesPoll
#define TipcMonDgRecvPoll           TipcMonSubjectSubscribePoll
#define TipcMonClientRecvPoll       TipcMonClientSubscribePoll
#define TipcMonClientRecvNumPoll    TipcMonClientSubscribeNumPoll
#define TipcMonClientDgPoll         TipcMonClientSubjectPoll
 
#define TipcSrvDgGetRecv            TipcSrvSubjectGetSubscribe
#define TipcSrvDgSetRecv            TipcSrvSubjectSetSubscribe
#define TipcSrvDgTraverseRecv       TipcSrvSubjectTraverseSubscribe
#define TipcSrvDgRetrieve           TipcSrvSubjectRetrieve
#define TipcSrvStdDgSetRecv         TipcSrvStdSubjectSetSubscribe
#define TipcSrvStdDgRetrieve        TipcSrvStdSubjectRetrieve
 
/*
 * Undocumented API functions.
 */
#define TipcMonGetDgRecvTable       TipcMonGetSubjectSubscribeTable
#define TipcMonGetClientRecvTable   TipcMonGetClientSubscribeTable
#define TipcSrvDgSortRecv           TipcSrvSubjectSortSubscribe
#define TipcSrvStdDgTraverse        TipcSrvStdSubjectTraverse
#endif
 
/* 
 * Added for MVS port - BDD 12/28/95 
 */
#ifdef T_OS_MVS
#include <rtworks/tutdefs.h>
#endif
 
/* Added For Cacheing */
#define T_SUBJ_NONE  0
#define T_SUBJ_CACHE 1
#define T_SUBJ_PERSIST 2

#define T_SUBJ_FLAG_SC_INITIAL_LOAD 0x00000001
#define T_SUBJ_FLAG_SC_LATEST_MSG   0x00000002
 
#define T_CACHE_ALL -1

/* 
 * IPC prototypes 
 */
#include <rtworks/ipcpr.h> 
#include <rtworks/ipclpr.h>

#ifndef MVS_KIT
/* 
 * TipcDev prototypes 
 */
#include <rtworks/ipcdevpr.h> 
#endif

#ifdef T_OS_WIN32
#include <rtworks/winipcpr.h>
#endif
 
/* 
 * Turn on the printf style error checking provided by gcc. 
 */
#if (__GNUC__ == 2 && !defined(__cplusplus))
T_BOOL TipcDevSendControl(T_STR fmt, ...)
  __attribute__ ((format (printf, 1, 2)));
#endif
 
#endif /* _RTWORKS_IPC_H_ */
