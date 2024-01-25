/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tre/dev/68/src/rtworks/smr.h#1 $
 */

#ifndef _RTWORKS_SMR_H_
#define _RTWORKS_SMR_H_

#include <rtworks/ipc.h>
#include <rtworks/server.h>
#include <rtworks/sm.h>
#include <rtworks/sdbasic.h>

/*
 * Error codes
 */
#define T_ERR_SMR_STATE_INVALID                   (T_ERR_SMR_START + 0)
#define T_ERRNM_SMR_STATE_INVALID                 "invalid SMR state"

/*
 * Client modes
 */
typedef enum {
  T_SMR_FULL_CLIENT,               /* Normal client                    */
  T_SMR_WARM_CLIENT,               /* Warm client                      */
  T_SMR_DEAD_CLIENT                /* Dead client                      */
} T_SMR_CLIENT_MODE;

/*
 * Interface IDs for server extensions
 */
typedef enum {
  T_SMR_IID_XF_FCTRY_ID = 0,       /* Interface factory interface id   */
  T_SMR_IID_CLIENT_FCTRY_XF_ID,    /* Client factory interface id      */
  T_SMR_IID_CLIENT_XF_ID,          /* Client interface id              */
  T_SMR_IID_CLIENT_XF_ID_2         /* Client interface id              */
} T_SMR_INTERFACE_ID;

/*
 * Msg direction during in filter function call
 */
typedef enum {
  T_SMR_MFD_IN,                    /* Message received from client    */
  T_SMR_MFD_OUT                    /* Message being sent to client    */
} T_SMR_MSG_FILTER_DIRECTION;

/*
 * Opaque types.
 */
typedef struct T_SMR_PROJECT_STRUCT
               T_SMR_PROJECT_STRUCT,
              *T_SMR_PROJECT;
typedef struct T_SMR_SUBJECT_STRUCT
               T_SMR_SUBJECT_STRUCT,
              *T_SMR_SUBJECT;
typedef struct T_SMR_SUBJECT_LIST_STRUCT
               T_SMR_SUBJECT_LIST_STRUCT,
              *T_SMR_SUBJECT_LIST;
typedef struct T_SMR_SUBJECT_ROUTING_STRUCT
               T_SMR_SUBJECT_ROUTING_STRUCT,
              *T_SMR_SUBJECT_ROUTING;
typedef struct T_SMR_SERVER_RECV_ACK_STRUCT
               T_SMR_SERVER_RECV_ACK_STRUCT,
              *T_SMR_SERVER_RECV_ACK;
typedef struct T_SMR_SERVER_RECV_ACK_PDQ_STRUCT
               T_SMR_SERVER_RECV_ACK_PDQ_STRUCT,
              *T_SMR_SERVER_RECV_ACK_PDQ;
typedef struct T_SMR_SERVER_SEND_ACK_DATA_STRUCT
               T_SMR_SERVER_SEND_ACK_DATA_STRUCT,
              *T_SMR_SERVER_SEND_ACK_DATA;
typedef struct T_SMR_SERVER_SEND_ACK_STRUCT
               T_SMR_SERVER_SEND_ACK_STRUCT,
              *T_SMR_SERVER_SEND_ACK;
typedef struct T_SMR_SERVER_MON_WATCH_STRUCT
               T_SMR_SERVER_MON_WATCH_STRUCT,
              *T_SMR_SERVER_MON_WATCH;
typedef struct T_SMR_SERVER_STRUCT
               T_SMR_SERVER_STRUCT,
              *T_SMR_SERVER;
typedef struct T_SMR_SERVER_PRIME_STRUCT
               T_SMR_SERVER_PRIME_STRUCT,
              *T_SMR_SERVER_PRIME;
typedef struct T_SMR_CLIENT_SEND_ACK_STRUCT
               T_SMR_CLIENT_SEND_ACK_STRUCT,
              *T_SMR_CLIENT_SEND_ACK;
typedef struct T_SMR_CLIENT_RECV_ACK_STRUCT
               T_SMR_CLIENT_RECV_ACK_STRUCT,
              *T_SMR_CLIENT_RECV_ACK;
typedef struct T_SMR_CLIENT_MON_WATCH_STRUCT
               T_SMR_CLIENT_MON_WATCH_STRUCT,
              *T_SMR_CLIENT_MON_WATCH;
typedef struct T_SMR_CLIENT_STRUCT
               T_SMR_CLIENT_STRUCT,
              *T_SMR_CLIENT;
typedef struct T_SMR_CLIENT_PRIME_STRUCT
               T_SMR_CLIENT_PRIME_STRUCT,
              *T_SMR_CLIENT_PRIME;
typedef struct T_SMR_GMD_CLIENT_STRUCT
               T_SMR_GMD_CLIENT_STRUCT,
              *T_SMR_GMD_CLIENT;
typedef struct T_SMR_PUBLISH_CLIENT_STRUCT
               T_SMR_PUBLISH_CLIENT_STRUCT,
              *T_SMR_PUBLISH_CLIENT;
typedef struct T_SMR_GROUP_STRUCT
               T_SMR_GROUP_STRUCT,
              *T_SMR_GROUP;
typedef struct T_SMR_GROUP_PREOPEN_MEMBER_STRUCT
               T_SMR_GROUP_PREOPEN_MEMBER_STRUCT,
              *T_SMR_GROUP_PREOPEN_MEMBER;
typedef struct T_SMR_IOCHM_XF_STRUCT
               T_SMR_IOCHM_XF_STRUCT,
              *T_SMR_IOCHM_XF;
typedef struct T_SMR_IO_DIRECTOR_STRUCT
               T_SMR_IO_DIRECTOR_STRUCT,
              *T_SMR_IO_DIRECTOR;
typedef struct T_SMR_IO_TRAFFIC_STRUCT
               T_SMR_IO_TRAFFIC_STRUCT,
              *T_SMR_IO_TRAFFIC;
typedef struct T_SMR_IO_BUFFER_STRUCT
               T_SMR_IO_BUFFER_STRUCT,
              *T_SMR_IO_BUFFER;
typedef struct T_SMR_STRUCT
               T_SMR_STRUCT,
              *T_SMR;
typedef struct T_SMR_CLIENT_FCTRY_XF_STRUCT
               T_SMR_CLIENT_FCTRY_XF_STRUCT,
              *T_SMR_CLIENT_FCTRY_XF;
typedef struct T_SMR_CLIENT_XF_STRUCT
               T_SMR_CLIENT_XF_STRUCT,
              *T_SMR_CLIENT_XF;
typedef struct T_SMR_CLIENT_XF_2_STRUCT
               T_SMR_CLIENT_XF_2_STRUCT,
              *T_SMR_CLIENT_XF_2;
typedef struct T_SMR_XFFCTRY_XF_STRUCT
               T_SMR_XFFCTRY_XF_STRUCT,
              *T_SMR_XFFCTRY_XF;
typedef struct T_SMR_EXT_LIBRARY_INFO_STRUCT
               T_SMR_EXT_LIBRARY_INFO_STRUCT,
              *T_SMR_EXT_LIBRARY_INFO;
typedef struct T_SMR_OPEN_PROC_TRAV_CTX_STRUCT
               T_SMR_OPEN_PROC_TRAV_CTX_STRUCT,
              *T_SMR_OPEN_PROC_TRAV_CTX;
typedef struct T_SMR_TOPOLOGY_EXPLORER_DATA_STRUCT
               T_SMR_TOPOLOGY_EXPLORER_DATA_STRUCT,
              *T_SMR_TOPOLOGY_EXPLORER_DATA;
typedef struct T_SMR_CONGESTION_WATCH_CONN_STRUCT
               T_SMR_CONGESTION_WATCH_CONN_STRUCT,
              *T_SMR_CONGESTION_WATCH_CONN;
typedef struct T_SMR_CONGESTION_WATCH_SERVER_STRUCT
               T_SMR_CONGESTION_WATCH_SERVER_STRUCT,
              *T_SMR_CONGESTION_WATCH_SERVER;
typedef struct T_SMR_REMOTE_CONGESTION_WATCH_STRUCT
               T_SMR_REMOTE_CONGESTION_WATCH_STRUCT,
              *T_SMR_REMOTE_CONGESTION_WATCH;
typedef struct T_SMR_DRAIN_DF_ARGS_STRUCT
               T_SMR_DRAIN_DF_ARGS_STRUCT,
              *T_SMR_DRAIN_DF_ARGS;

/*
 * Traversal function types.
 */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_PROJECT_TRAV_FUNC)
T_TYPEDEF((T_STR,
           T_SMR_PROJECT,
           T_PTR));

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CLIENT_TRAV_FUNC)
T_TYPEDEF((T_STR,
           T_SMR_CLIENT,
           T_PTR));

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_SUBJECT_TRAV_FUNC)
T_TYPEDEF((T_SUBJECT_TABLE_TRAV_STATUS,
           T_SMR_SUBJECT,
           T_PTR));

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_SERVER_TRAV_FUNC)
T_TYPEDEF((T_STR,
           T_SMR_SERVER,
           T_PTR));

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_GROUP_TRAV_FUNC)
T_TYPEDEF((T_STR,
           T_SMR_GROUP,
           T_PTR));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_WC_SUBJECT_TRAV_FUNC)
T_TYPEDEF((T_SMR_SUBJECT,
           T_SMR_SUBJECT,
           T_PTR,
           T_PTR));

/*
 * Callback type numbers.
 */
#define T_SMR_PROJECT_CREATE_CB_NUM                  (T_LOBE_SMR_START - 0)
#define T_SMR_PROJECT_DESTROY_CB_NUM                 (T_LOBE_SMR_START - 1)
#define T_SMR_CLIENT_CREATE_CB_NUM                   (T_LOBE_SMR_START - 2)
#define T_SMR_CLIENT_OPEN_CB_NUM                     (T_LOBE_SMR_START - 3)
#define T_SMR_CLIENT_CLOSE_CB_NUM                    (T_LOBE_SMR_START - 4)
#define T_SMR_CLIENT_DESTROY_CB_NUM                  (T_LOBE_SMR_START - 5)
#define T_SMR_CLIENT_SUBSCRIBE_CB_NUM                (T_LOBE_SMR_START - 6)
#define T_SMR_SUBJECT_CREATE_CB_NUM                  (T_LOBE_SMR_START - 7)
#define T_SMR_SUBJECT_DESTROY_CB_NUM                 (T_LOBE_SMR_START - 8)
#define T_SMR_SUBJECT_SUBSCRIBE_CB_NUM               (T_LOBE_SMR_START - 9)
#define T_SMR_SERVER_CREATE_CB_NUM                   (T_LOBE_SMR_START - 10)
#define T_SMR_SERVER_DESTROY_CB_NUM                  (T_LOBE_SMR_START - 11)
#define T_SMR_SERVER_SUBSCRIBE_CB_NUM                (T_LOBE_SMR_START - 12)
#define T_SMR_SERVER_MAX_CLIENT_LICENSES_CB_NUM      (T_LOBE_SMR_START - 13)
#define T_SMR_GROUP_CREATE_CB_NUM                    (T_LOBE_SMR_START - 14)

/*
 * Callback data fields.
 */
typedef struct T_CB_DATA_STRUCT
               T_SMR_PROJECT_CREATE_CB_DATA_STRUCT,
              *T_SMR_PROJECT_CREATE_CB_DATA;

typedef struct T_CB_DATA_STRUCT
               T_SMR_PROJECT_DESTROY_CB_DATA_STRUCT,
              *T_SMR_PROJECT_DESTROY_CB_DATA;

/*
typedef struct T_SMR_CLIENT_MAX_LICENSES_CB_DATA_STRUCT
               T_SMR_CLIENT_MAX_LICENSES_CB_DATA_STRUCT,
              *T_SMR_CLIENT_MAX_LICENSES_CB_DATA;
struct T_SMR_CLIENT_MAX_LICENSES_CB_DATA_STRUCT {
  T_CB          cb;
  T_STR         client_name;
  T_BOOL        license_flag;
};
*/

typedef struct T_CB_DATA_STRUCT
               T_SMR_CLIENT_CREATE_CB_DATA_STRUCT,
              *T_SMR_CLIENT_CREATE_CB_DATA;

typedef struct T_CB_DATA_STRUCT
               T_SMR_CLIENT_OPEN_CB_DATA_STRUCT,
              *T_SMR_CLIENT_OPEN_CB_DATA;

typedef struct T_CB_DATA_STRUCT
               T_SMR_CLIENT_CLOSE_CB_DATA_STRUCT,
              *T_SMR_CLIENT_CLOSE_CB_DATA;

typedef struct T_CB_DATA_STRUCT
               T_SMR_CLIENT_DESTROY_CB_DATA_STRUCT,
              *T_SMR_CLIENT_DESTROY_CB_DATA;

typedef struct T_SMR_CLIENT_SUBSCRIBE_CB_DATA_STRUCT
               T_SMR_CLIENT_SUBSCRIBE_CB_DATA_STRUCT,
              *T_SMR_CLIENT_SUBSCRIBE_CB_DATA;
struct T_SMR_CLIENT_SUBSCRIBE_CB_DATA_STRUCT {
  T_CB          cb;
  T_SMR_SUBJECT dg;
  T_BOOL        recv_flag;
};

typedef struct T_CB_DATA_STRUCT
               T_SMR_SUBJECT_CREATE_CB_DATA_STRUCT,
              *T_SMR_SUBJECT_CREATE_CB_DATA;

typedef struct T_CB_DATA_STRUCT
               T_SMR_SUBJECT_DESTROY_CB_DATA_STRUCT,
              *T_SMR_SUBJECT_DESTROY_CB_DATA;

typedef struct T_SMR_SUBJECT_SUBSCRIBE_CB_DATA_STRUCT
               T_SMR_SUBJECT_SUBSCRIBE_CB_DATA_STRUCT,
              *T_SMR_SUBJECT_SUBSCRIBE_CB_DATA;
struct T_SMR_SUBJECT_SUBSCRIBE_CB_DATA_STRUCT {
  T_CB          cb;
  T_SMR_CLIENT  client;
  T_BOOL        recv_flag;
};

typedef struct T_SMR_SERVER_CREATE_CB_DATA_STRUCT
               T_SMR_SERVER_CREATE_CB_DATA_STRUCT,
              *T_SMR_SERVER_CREATE_CB_DATA;
struct T_SMR_SERVER_CREATE_CB_DATA_STRUCT {
  T_CB    cb;
  T_BOOL  client_flag;
};

typedef struct T_SMR_SERVER_MAX_CLIENT_LICENSES_CB_DATA_STRUCT
               T_SMR_SERVER_MAX_CLIENT_LICENSES_CB_DATA_STRUCT,
              *T_SMR_SERVER_MAX_CLIENT_LICENSES_CB_DATA;
struct T_SMR_SERVER_MAX_CLIENT_LICENSES_CB_DATA_STRUCT {
  T_CB    cb;
  T_STR   client_name;
  T_INT4  allowed;
};

typedef struct T_CB_DATA_STRUCT
               T_SMR_SERVER_DESTROY_CB_DATA_STRUCT,
              *T_SMR_SERVER_DESTROY_CB_DATA;

typedef struct T_CB_DATA_STRUCT
               T_SMR_GROUP_CREATE_CB_DATA_STRUCT,
              *T_SMR_GROUP_CREATE_CB_DATA;

typedef struct T_SMR_SERVER_SUBSCRIBE_CB_DATA_STRUCT
               T_SMR_SERVER_SUBSCRIBE_CB_DATA_STRUCT,
              *T_SMR_SERVER_SUBSCRIBE_CB_DATA;
struct T_SMR_SERVER_SUBSCRIBE_CB_DATA_STRUCT {
  T_CB            cb;
  T_SMR_SUBJECT   subject;
  T_BOOL          subscribe_flag;
};

typedef struct T_SMR_MSG_HANDLER_DATA_STRUCT
               T_SMR_MSG_HANDLER_DATA_STRUCT,
              *T_SMR_MSG_HANDLER_DATA;
struct T_SMR_MSG_HANDLER_DATA_STRUCT {
  T_MSG         msg;
  T_SIOC_MSG_IQ iq;
  T_STR        *dest_server_list;
  T_INT4        num_dest_servers;
  T_STR        *dest_client_list;
  T_INT4        num_dest_clients;
  T_INT4       *dest_mid_list;
  T_INT4        num_dest_mids;
};

typedef struct T_SMR_MSG_DISPATCH_DATA_STRUCT
               T_SMR_MSG_DISPATCH_DATA_STRUCT,
              *T_SMR_MSG_DISPATCH_DATA;
struct T_SMR_MSG_DISPATCH_DATA_STRUCT {
  T_MSG msg;
};

typedef struct T_SMR_MSG_FILTER_DATA_STRUCT
               T_SMR_MSG_FILTER_DATA_STRUCT,
              *T_SMR_MSG_FILTER_DATA;
struct T_SMR_MSG_FILTER_DATA_STRUCT {
  T_SMR_MSG_FILTER_DIRECTION  dir;
  T_MSG                       msg;
};

/*
 * Callback function types.
 */
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_PROJECT_CREATE_CB_FUNC)
T_TYPEDEF((T_SMR_PROJECT,
           T_SMR_PROJECT_CREATE_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_PROJECT_DESTROY_CB_FUNC)
T_TYPEDEF((T_SMR_PROJECT,
           T_SMR_PROJECT_DESTROY_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CLIENT_CREATE_CB_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_SMR_CLIENT_CREATE_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CLIENT_OPEN_CB_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_SMR_CLIENT_OPEN_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CLIENT_CLOSE_CB_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_SMR_CLIENT_CLOSE_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CLIENT_DESTROY_CB_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_SMR_CLIENT_DESTROY_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CLIENT_SUBSCRIBE_CB_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_SMR_CLIENT_SUBSCRIBE_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_SUBJECT_CREATE_CB_FUNC)
T_TYPEDEF((T_SMR_SUBJECT,
           T_SMR_SUBJECT_CREATE_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_SUBJECT_DESTROY_CB_FUNC)
T_TYPEDEF((T_SMR_SUBJECT,
           T_SMR_SUBJECT_DESTROY_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_SUBJECT_SUBSCRIBE_CB_FUNC)
T_TYPEDEF((T_SMR_SUBJECT,
           T_SMR_SUBJECT_SUBSCRIBE_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT
              T_SMR_SERVER_MAX_CLIENT_LICENSES_CB_FUNC)
T_TYPEDEF((T_SMR,
           T_SMR_SERVER_MAX_CLIENT_LICENSES_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_SERVER_CREATE_CB_FUNC)
T_TYPEDEF((T_SMR_SERVER,
           T_SMR_SERVER_CREATE_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_SERVER_DESTROY_CB_FUNC)
T_TYPEDEF((T_SMR_SERVER,
           T_SMR_SERVER_DESTROY_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_SERVER_SUBSCRIBE_CB_FUNC)
T_TYPEDEF((T_SMR_SERVER,
           T_SMR_SERVER_SUBSCRIBE_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_GROUP_CREATE_CB_FUNC)
T_TYPEDEF((T_SMR_GROUP,
           T_SMR_GROUP_CREATE_CB_DATA,
           T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_MSG_HANDLER_FUNC)
T_TYPEDEF((T_SMR,
           T_PTR,
           T_SMR_MSG_HANDLER_DATA));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_MSG_DISPATCH_FUNC)
T_TYPEDEF((T_PTR,
           T_SMR_MSG_DISPATCH_DATA,
           T_CB_ARG));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_MSG_FILTER_FUNC)
T_TYPEDEF((T_SMR_MSG_FILTER_DATA,
           T_CB_ARG));

/*
 * SMR extensions related defintions
 */
#define T_SMR_EXT_LIBRARY_OPEN_FUNC  "TsrvExtOpenFunc"
#define T_SMR_EXT_LIBRARY_CLOSE_FUNC "TsrvExtCloseFunc"

/*
 * Client factory interface method definitions
 */
typedef T_CB (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_FCTRY_CREATE_CB_FUNC)
T_TYPEDEF((T_SMR_CLIENT_CREATE_CB_FUNC,
           T_CB_ARG));

typedef T_CB (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_FCTRY_CREATE_CB_LOOKUP_FUNC)
T_TYPEDEF((T_SMR_CLIENT_CREATE_CB_FUNC,
           T_CB_ARG));

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_FCTRY_TRAVERSE_FUNC)
T_TYPEDEF((T_SMR_CLIENT_TRAV_FUNC,
           T_PTR));

/*
 * Client interface method definitions
 */
typedef T_CB (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_DESTROY_CB_CREATE_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_SMR_CLIENT_DESTROY_CB_FUNC,
           T_CB_ARG));

typedef T_CB (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_DESTROY_CB_LOOKUP_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_SMR_CLIENT_DESTROY_CB_FUNC,
           T_CB_ARG));

typedef T_SMR_MSG_FILTER_FUNC (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_FILTER_CREATE_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_INT4,
           T_SMR_MSG_FILTER_FUNC));

typedef T_SMR_MSG_DISPATCH_FUNC (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_DISPATCH_CREATE_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_INT4,
           T_SMR_MSG_DISPATCH_FUNC));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_GET_NAME_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_STR *));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_GET_DISCONNECT_MODE_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_IPC_SRV_DISCONNECT_MODE *));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_GET_IDENT_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_STR *));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_GET_PROJECT_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_SMR_PROJECT *));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_SEND_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_MSG));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_DESTROY_FUNC)
T_TYPEDEF((T_SMR_CLIENT));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_PRINT_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_OUT_FUNC));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CL_XF_GET_PROPERTIES_FUNC)
T_TYPEDEF((T_SMR_CLIENT,
           T_PROPERTIES));


/*
 * Interface factory interface method definitions
 */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_CREATE_INTERFACE_FUNC)
T_TYPEDEF((T_SMR_INTERFACE_ID,
           T_PTR *,
           T_PTR));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_DESTROY_INTERFACE_FUNC)
T_TYPEDEF((T_SMR_INTERFACE_ID,
           T_PTR));

/*
 * Server extension library exports definitions
 */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_EXT_OPEN_FUNC)
T_TYPEDEF((T_EDM,
           T_SMR_XFFCTRY_XF));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SMR_EXT_CLOSE_FUNC)
T_TYPEDEF((void));

/*
 * Interface factory interface definition
 */
struct T_SMR_XFFCTRY_XF_STRUCT {
  T_PTR                                 ptr;
  T_SMR_CREATE_INTERFACE_FUNC           create_xface;
  T_SMR_DESTROY_INTERFACE_FUNC          destroy_xface;
};

/*
 * Client factory interface definition
 */
struct T_SMR_CLIENT_FCTRY_XF_STRUCT {
  T_PTR                                 ptr;
  T_SMR_CL_FCTRY_CREATE_CB_FUNC         create_cb_create;
  T_SMR_CL_FCTRY_CREATE_CB_LOOKUP_FUNC  create_cb_lookup;
  T_SMR_CL_FCTRY_TRAVERSE_FUNC          trav;
};

/*
 * Client interface definition
 */
struct T_SMR_CLIENT_XF_STRUCT {
  T_SMR_CLIENT                          client;
  T_SMR_CL_XF_DESTROY_CB_CREATE_FUNC    destroy_cb_create;
  T_SMR_CL_XF_DESTROY_CB_LOOKUP_FUNC    destroy_cb_lookup;
  T_SMR_CL_XF_DISPATCH_CREATE_FUNC      dispatch_create;
  T_SMR_CL_XF_FILTER_CREATE_FUNC        filter_create;
  T_SMR_CL_XF_GET_NAME_FUNC             get_name;
  T_SMR_CL_XF_GET_DISCONNECT_MODE_FUNC  get_disconnectmode;
  T_SMR_CL_XF_GET_IDENT_FUNC            get_ident;
  T_SMR_CL_XF_GET_PROJECT_FUNC          get_project;
  T_SMR_CL_XF_SEND_FUNC                 send;
  T_SMR_CL_XF_DESTROY_FUNC              destroy;
  T_SMR_CL_XF_PRINT_FUNC                print;
};

/*
 * Client 2 interface definition
 */
struct T_SMR_CLIENT_XF_2_STRUCT {
  T_SMR_CLIENT                          client;
  T_SMR_CL_XF_DESTROY_CB_CREATE_FUNC    destroy_cb_create;
  T_SMR_CL_XF_DESTROY_CB_LOOKUP_FUNC    destroy_cb_lookup;
  T_SMR_CL_XF_DISPATCH_CREATE_FUNC      dispatch_create;
  T_SMR_CL_XF_FILTER_CREATE_FUNC        filter_create;
  T_SMR_CL_XF_GET_NAME_FUNC             get_name;
  T_SMR_CL_XF_GET_DISCONNECT_MODE_FUNC  get_disconnectmode;
  T_SMR_CL_XF_GET_IDENT_FUNC            get_ident;
  T_SMR_CL_XF_GET_PROJECT_FUNC          get_project;
  T_SMR_CL_XF_SEND_FUNC                 send;
  T_SMR_CL_XF_DESTROY_FUNC              destroy;
  T_SMR_CL_XF_PRINT_FUNC                print;
  T_SMR_CL_XF_GET_PROPERTIES_FUNC       get_properties;

};

#include <rtworks/smrpr.h>

#endif /* _RTWORKS_SMR_H_ */


