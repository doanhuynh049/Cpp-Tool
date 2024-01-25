/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tiof/biof/dev/68/src/rtworks/conn.h#1 $
 */

#ifndef _RTWORKS_CONN_H_
#define _RTWORKS_CONN_H_

#include <rtworks/link.h>
#include <rtworks/ioctl.h>
#include <rtworks/msg.h>

/*
 * CONN trace bindings
 */
#define T_CONN_TRACE_LEVEL_OPTION                  "_trace_level_conn"
#define T_CONN_TRACE_FILE_OPTION                   "_trace_file_conn"

typedef enum {
  T_CONN_TIMEOUT_READ,
  T_CONN_TIMEOUT_WRITE,
  T_CONN_TIMEOUT_KEEP_ALIVE,
  T_CONN_TIMEOUT_DELIVERY /* guaranteed message delivery */
} T_CONN_TIMEOUT;

/*
 * T_CONN_POS_PRIORITY is used to insert a message in priority order in
 * the message queue of a connection.
 */
#define T_CONN_POS_PRIORITY (-1)

/*
 * T_CONN_NO_AUTO_FLUSH completely disables any automatic flushing of outgoing
 * messages on a connection.
 */
#define T_CONN_NO_AUTO_FLUSH (-1)

/*
 * GMD-related defines.
 */

#define T_CONN_GMD_DIR_NAME  "gmd"       /* tacked onto Ipc_Gmd_Directory */
#define T_CONN_SPOOL_MSG_EXT ".msg"      /* msg file component of spool */
#define T_CONN_SPOOL_MAP_EXT ".map"      /* map file component of spool */
#define T_CONN_HSN_EXT       ".hsn"      /* highest sequence numbers of incoming msgs */
#define T_CONN_GMD_MEMORY    "memory"    /* special name for memory-only GMD */
#define T_CONN_GMD_FILE      "file"      /* special name for file-only GMD */

/*
 * Connection properties and IOCTL commands
 */

#define T_CONN_PROP_NO_CONN_INIT "NO_CONN_INIT"           /* no conn_init required */
#define T_CONN_PROP_GROUP_NAME   "GROUP_NAME"             /* Groupname from the conn */
#define T_CONN_IOCTL_EXEC_START_DATA "START_DATA_CHANNEL" /* start data channel */

/*
 * Opaue data types
 */
typedef struct T_CONN_STRUCT
               T_CONN_STRUCT,
              *T_CONN;
typedef struct T_CONN_SPOOL_STRUCT
               T_CONN_SPOOL_STRUCT,
              *T_CONN_SPOOL;
typedef struct T_CONN_HSN_STRUCT
               T_CONN_HSN_STRUCT,
              *T_CONN_HSN;
typedef struct T_CONN_HSN_ENTRY_STRUCT
               T_CONN_HSN_ENTRY_STRUCT,
              *T_CONN_HSN_ENTRY;
typedef struct T_CONN_ACK_STRUCT
               T_CONN_ACK_STRUCT,
              *T_CONN_ACK;

/*
 * Callback related defintions
 */
#define T_CONN_ERROR_CB_NUM                   (T_LOBE_IPC_START - 0)
#define T_CONN_ACCEPT_CB_NUM                  (T_LOBE_IPC_START - 1)
#define T_CONN_READ_CB_NUM                    (T_LOBE_IPC_START - 2)
#define T_CONN_WRITE_CB_NUM                   (T_LOBE_IPC_START - 3)
#define T_CONN_PROCESS_CB_NUM                 (T_LOBE_IPC_START - 4)
#define T_CONN_QUEUE_CB_NUM                   (T_LOBE_IPC_START - 5)
#define T_CONN_ENCODE_CB_NUM                  (T_LOBE_IPC_START - 6)
#define T_CONN_DECODE_CB_NUM                  (T_LOBE_IPC_START - 7)
#define T_IPC_ST_CHANGE_CB_NUM                (T_LOBE_IPC_START - 8)
#define T_CONN_ENCRYPT_CB_NUM                 (T_LOBE_IPC_START - 9)
#define T_CONN_DECRYPT_CB_NUM                 (T_LOBE_IPC_START - 10)
#define T_CONN_CONNECT_CB_NUM                 (T_LOBE_IPC_START - 11)
#define T_CONN_WRITE_BUFFER_CHANGE_CB_NUM     (T_LOBE_IPC_START - 12)


typedef struct T_CONN_MSG_CB_DATA_STRUCT
               T_CONN_MSG_CB_DATA_STRUCT,
              *T_CONN_MSG_CB_DATA;
struct T_CONN_MSG_CB_DATA_STRUCT {
  T_CB  cb;
  T_MSG msg;
};

typedef struct T_CONN_ERROR_CB_DATA_STRUCT
               T_CONN_ERROR_CB_DATA_STRUCT,
              *T_CONN_ERROR_CB_DATA;
struct T_CONN_ERROR_CB_DATA_STRUCT {
  T_CB    cb;
  T_INT4  err_num;          /* global error code */
#ifdef T_RTWORKS35_COMPAT   /* compat with 3.5 */
  T_INT4  unix_errno;       /* UNIX-style error code (from errno) */
#ifdef T_OS_VMS
  T_INT4  vms_errno;        /* VMS-style error code (from vaxc$errno) */
#endif
#endif
  T_INT4  c_err_num;        /* C error number (errno) */
  T_INT4  os_err_num;       /* OS error number (vaxc$errno, GetLastError(), etc.) */
  T_INT4  socket_err_num;   /* socket error number (WSAGetLastError(), sock_errno(), etc.) */
};

typedef struct T_CONN_ACCEPT_CB_DATA_STRUCT
               T_CONN_ACCEPT_CB_DATA_STRUCT,
              *T_CONN_ACCEPT_CB_DATA;
struct T_CONN_ACCEPT_CB_DATA_STRUCT {
  T_CB        cb;
  T_CONN  client;
};

typedef struct T_CONN_CONNECT_CB_DATA_STRUCT
               T_CONN_CONNECT_CB_DATA_STRUCT,
              *T_CONN_CONNECT_CB_DATA;
struct T_CONN_CONNECT_CB_DATA_STRUCT {
  T_CB        cb;
  T_BOOL  status;
  T_INT4 err_num;
};


typedef struct T_CONN_READ_CB_DATA_STRUCT
               T_CONN_READ_CB_DATA_STRUCT,
              *T_CONN_READ_CB_DATA;
struct T_CONN_READ_CB_DATA_STRUCT {
  T_CB    cb;
  T_MSG   msg;
  T_INT4  packet_size;
};

typedef struct T_CONN_MSG_CB_DATA_STRUCT
               T_CONN_WRITE_CB_DATA_STRUCT,
              *T_CONN_WRITE_CB_DATA;

typedef struct T_CONN_MSG_CB_DATA_STRUCT
               T_CONN_PROCESS_CB_DATA_STRUCT,
              *T_CONN_PROCESS_CB_DATA;

typedef struct T_CONN_MSG_CB_DATA_STRUCT
               T_CONN_DEFAULT_CB_DATA_STRUCT,
              *T_CONN_DEFAULT_CB_DATA;

typedef struct T_CONN_QUEUE_CB_DATA_STRUCT
               T_CONN_QUEUE_CB_DATA_STRUCT,
              *T_CONN_QUEUE_CB_DATA;
struct T_CONN_QUEUE_CB_DATA_STRUCT {
  T_CB    cb;
  T_MSG   msg;          /* message being inserted into or deleted from queue */
  T_BOOL  insert_flag;  /* TRUE if inserted, FALSE if deleted */
  T_INT4  position;     /* position of message in queue (0 is front) */
};

typedef struct T_CONN_WRITE_BUFFER_CHANGE_CB_DATA_STRUCT
               T_CONN_WRITE_BUFFER_CHANGE_CB_DATA_STRUCT,
              *T_CONN_WRITE_BUFFER_CHANGE_CB_DATA;
struct T_CONN_WRITE_BUFFER_CHANGE_CB_DATA_STRUCT {
  T_CB    cb;
  T_MSG   msg;          /* message being written into the write buffer */
  T_BOOL  write_flag;   /* TRUE if written, FALSE if flushed */
};

typedef struct T_CONN_ENCODE_CB_DATA_STRUCT
               T_CONN_ENCODE_CB_DATA_STRUCT,
              *T_CONN_ENCODE_CB_DATA;
struct T_CONN_ENCODE_CB_DATA_STRUCT {
  T_CB    cb;               /* must be first field in all CB_DATA structures */
  T_PTR   packet;           /* packet minus counter */
  T_INT4  packet_size;      /* number of bytes in packet */
  T_PTR   orig_packet;      /* for reference */
  T_INT4  orig_packet_size; /* for reference */
  T_MSG   orig_msg;         /* make available for monitoring.  SMW 10/30/96 */
};

typedef struct T_CONN_DECODE_CB_DATA_STRUCT
               T_CONN_DECODE_CB_DATA_STRUCT,
              *T_CONN_DECODE_CB_DATA;
struct T_CONN_DECODE_CB_DATA_STRUCT {
  T_CB    cb;               /* must be first field in all CB_DATA structures */
  T_PTR   packet;           /* packet minus counter */
  T_INT4  packet_size;      /* number of bytes in packet */
  T_PTR   orig_packet;      /* for reference */
  T_INT4  orig_packet_size; /* for reference */
};

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CONN_MSG_CB_FUNC)
T_TYPEDEF((T_CONN,
               T_CONN_MSG_CB_DATA,
               T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CONN_ERROR_CB_FUNC)
T_TYPEDEF((T_CONN,
               T_CONN_ERROR_CB_DATA,
               T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CONN_ACCEPT_CB_FUNC)
T_TYPEDEF((T_CONN,
               T_CONN_ACCEPT_CB_DATA,
               T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CONN_READ_CB_FUNC)
T_TYPEDEF((T_CONN,
               T_CONN_READ_CB_DATA,
               T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CONN_CONNECT_CB_FUNC)
T_TYPEDEF((T_CONN,
               T_CONN_CONNECT_CB_DATA,
               T_CB_ARG));


typedef T_CONN_MSG_CB_FUNC T_CONN_WRITE_CB_FUNC;

typedef T_CONN_MSG_CB_FUNC T_CONN_PROCESS_CB_FUNC;

typedef T_CONN_MSG_CB_FUNC T_CONN_DEFAULT_CB_FUNC;

typedef T_CONN_MSG_CB_FUNC T_CONN_ENCRYPT_CB_FUNC;

typedef T_CONN_MSG_CB_FUNC T_CONN_DECRYPT_CB_FUNC;

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CONN_QUEUE_CB_FUNC)
T_TYPEDEF((T_CONN,
               T_CONN_QUEUE_CB_DATA,
               T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CONN_WRITE_BUFFER_CHANGE_CB_FUNC)
T_TYPEDEF((T_CONN,
               T_CONN_WRITE_BUFFER_CHANGE_CB_DATA,
               T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CONN_ENCODE_CB_FUNC)
T_TYPEDEF((T_CONN,
               T_CONN_ENCODE_CB_DATA,
               T_CB_ARG));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CONN_DECODE_CB_FUNC)
T_TYPEDEF((T_CONN,
               T_CONN_DECODE_CB_DATA,
               T_CB_ARG));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CONN_MSG_SEARCH_FUNC)
T_TYPEDEF((T_CONN,
               T_MSG,
               T_PTR));
typedef struct T_CONN_LOG_INFO_STRUCT T_CONN_LOG_INFO_STRUCT, *T_CONN_LOG_INFO;

/*
 * CONN filter function
 */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_CONN_MSG_FILTER_FUNC)
T_TYPEDEF((T_CONN, T_MSG, T_PTR));

#include <rtworks/connpr.h>

#endif /* _RTWORKS_CONN_H_ */

