/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/msg.h#1 $
 */

#ifndef _RTWORKS_MSG_H_
#define _RTWORKS_MSG_H_

/*
 * Protocol version
 */
#define T_MSG_PROTOCOL_VERSION              78
#define T_MSG_PROTOCOL_VERSION_BASELINE     65 /* baseline for compat */
#define T_MSG_PROTOCOL_VERSION_ORIG_ALPHA   65
#define T_MSG_PROTOCOL_VERSION_LB           67 /* first load balancing version */
#define T_MSG_PROTOCOL_VERSION_UNIQUE       68 /* version to fix bugs 3502 & 3503 */
#define T_MSG_PROTOCOL_VERSION_HDR_STR      69 /* turn hdr str encoding on/off */
#define T_MSG_PROTOCOL_VERSION_GMD_INIT     69 /* gmd init convenience func */
#define T_MSG_PROTOCOL_VERSION_40_FINAL     69 /* final 4.0 version */
#define T_MSG_PROTOCOL_VERSION_50_BETA      70 /* first 5.0 version */
#define T_MSG_PROTOCOL_VERSION_WC           71 /* first weighted conn ver. */
#define T_MSG_PROTOCOL_VERSION_50_FINAL     71 /* final 5.0 version */
#define T_MSG_PROTOCOL_VERSION_LOCAL        72 /* local delivery mode for mcast */
#define T_MSG_PROTOCOL_VERSION_51_FINAL     72 /* final 5.1 version */
#define T_MSG_PROTOCOL_VERSION_52_FINAL     73 /* final 5.2 version */
#define T_MSG_PROTOCOL_VERSION_CONN_CAP     74 /* version to cap + usr/pw */
#define T_MSG_PROTOCOL_VERSION_GMD_STREAM   75 /* subscribe-side gmd buffering */
#define T_MSG_PROTOCOL_VERSION_SOD          76 /* ordered delivery mode */
#define T_MSG_PROTOCOL_VERSION_AUTH         77 /* authentication + authorization */
#define T_MSG_PROTOCOL_VERSION_SERVER_AUTH  78 /* server-server authentication */

/*
 * Current broadcast version
 */
#define T_MSG_BROADCAST_VERSION 46

/*
 * T_MSG_FT is for a message field type.
 *
 * NOTE: if you add/remove values, update ipcFtStr in ipc/ipcmsg1.c.
 */
typedef enum {
  T_MSG_FT_INVALID = 0,
  T_MSG_FT_CHAR,
  T_MSG_FT_BINARY,
  T_MSG_FT_STR,
  T_MSG_FT_STR_ARRAY,
  T_MSG_FT_INT2,
  T_MSG_FT_INT2_ARRAY,
  T_MSG_FT_INT4,
  T_MSG_FT_INT4_ARRAY,
  T_MSG_FT_INT8,
  T_MSG_FT_INT8_ARRAY,
  T_MSG_FT_REAL4,
  T_MSG_FT_REAL4_ARRAY,
  T_MSG_FT_REAL8,
  T_MSG_FT_REAL8_ARRAY,
  T_MSG_FT_REAL16,
  T_MSG_FT_REAL16_ARRAY,
  T_MSG_FT_MSG,
  T_MSG_FT_MSG_ARRAY,
  T_MSG_FT_INTERNAL_GROUP,      /* used internally for message files          */
  T_MSG_FT_INTERNAL_BOOL,       /* used internally for message files          */
  T_MSG_FT_INTERNAL_ID,         /* used internally for message files          */
  T_MSG_FT_INTERNAL_VERBOSE,    /* used internally for message files          */
  T_MSG_PROP_DELIVERY_MODE,     /* access a property via Tipc*Msg{Read,Write} */
  T_MSG_PROP_DEST,              /* access property via Tipc*Msg{Read,Write}   */
  T_MSG_PROP_PRIORITY,          /* access property via Tipc*Msg{Read,Write}   */
  T_MSG_PROP_SENDER,            /* access property via Tipc*Msg{Read,Write}   */
  T_MSG_PROP_TYPE,              /* access property via Tipc*Msg{Read,Write}   */
  T_MSG_PROP_USER_PROP,         /* access property via Tipc*Msg{Read,Write}   */
  T_MSG_PROP_DEST_SERVER,       /* undocumented                               */
  T_MSG_PROP_SCOPE,             /* undocumented                               */
  T_MSG_PROP_DELIVERY_TIMEOUT,  /* access property via Tipc*Msg{Read,Write}   */
  T_MSG_PROP_LB_MODE,           /* access property via Tipc*Msg{Read,Write}   */
  T_MSG_PROP_HEADER_STR_ENCODE, /* access property via Tipc*Msg{Read,Write}   */
  T_MSG_PROP_LOCAL_DELIVERY,    /* local delivery mode                        */
  T_MSG_PROP_COMPRESSION,       /* access property via Tipc*Msg{Read,Write}   */
  /* 
   * New field types added at IPC protocol version 71 --KLC 8/10/98 
   */
  T_MSG_FT_GUID = 64,           /* Globally unique ID                         */
  T_MSG_FT_GUID_ARRAY,          /* Array of globally unique IDs               */
  T_MSG_FT_MSGID,               /* GUID + 4-byte ts + 4-byte seq              */
  T_MSG_FT_MSGID_ARRAY,         /* Array of GUID + 4-byte ts + 4-byte seq     */
  T_MSG_FT_WSTR,                /* Wide (Unicode) string                      */
  T_MSG_FT_WSTR_ARRAY,          /* Array of wide (Unicode) strings            */
  T_MSG_FT_XML,                 /* XML                                        */
  T_MSG_FT_TIMESTAMP,           /* UT-based timestamp                         */
  T_MSG_FT_TIMESTAMP_ARRAY,     /* Array of UT-based timestamps               */
  T_MSG_FT_UTF8,                /* UTF-8 encoded string                       */
  T_MSG_FT_UTF8_ARRAY,          /* Array of UTF-8 encoded strings             */
  T_MSG_FT_BOOL,                /* Boolean value                              */
  T_MSG_FT_BOOL_ARRAY,          /* Array of Boolean values                    */
  T_MSG_FT_BYTE                 /* One byte                                   */
} T_MSG_FT;
#define T_MSG_PROP_HEADER_ENCODE T_MSG_PROP_HEADER_STR_ENCODE /* compatability from rev 0 to 1 */
 
/* 
 * Message delivery modes. 
 */
typedef enum {
  T_MSG_DELIVERY_BEST_EFFORT, /* will deliver unless network failure occurs         */
  T_MSG_DELIVERY_SOME,        /* guaranteed to deliver to at least one process      */
  T_MSG_DELIVERY_ALL,          /* guaranteed to deliver to all destination processes */
  T_MSG_DELIVERY_ORDERED,     /* source ordered delivery                            */
  T_MSG_DELIVERY_PERSISTENT /* persistent delivery using RMS */
} T_MSG_DELIVERY_MODE;
#define T_MSG_DELIVERY_GMD(mode)  ((T_MSG_DELIVERY_SOME == (mode)) || (T_MSG_DELIVERY_ALL == (mode)))

/* 
 * Message load balancing modes. 
 */
typedef enum {
  T_MSG_LB_NONE,        /* normal publish subscribe delivery mode             */
  T_MSG_LB_ROUND_ROBIN, /* Round Robin load balancing                         */
  T_MSG_LB_WEIGHTED,    /* weighted Round Robin load balancing (based on acks)*/
  T_MSG_LB_SORTED       /* sorted load balancing                              */
} T_MSG_LB_MODE;
#define T_MSG_LB_FIFO T_MSG_LB_WEIGHTED /* compatability from rev 0 to 1 */

/* 
 * Message file creation modes
 */
typedef enum {
  T_MSG_FILE_CREATE_READ,         /* read (auto-detect text or binary)    */
  T_MSG_FILE_CREATE_WRITE,        /* write in text mode                   */
  T_MSG_FILE_CREATE_WRITE_BINARY, /* write in binary mode                 */
  T_MSG_FILE_CREATE_APPEND        /* append (auto-detect text or binary)  */
} T_MSG_FILE_CREATE_MODE;

/*
 * Opaque data types
 */
typedef struct T_MSG_MT_STRUCT 
               T_MSG_MT_STRUCT, 
              *T_MSG_MT;

typedef struct T_MSG_MT_ENCRYPT_INFO_STRUCT 
               T_MSG_MT_ENCRYPT_INFO_STRUCT,
              *T_MSG_MT_ENCRYPT_INFO;

typedef struct T_MSG_STRUCT 
               T_MSG_STRUCT, 
              *T_MSG;

typedef struct T_MSG_WIRE_STRUCT 
               T_MSG_WIRE_STRUCT, 
              *T_MSG_WIRE;
 
typedef struct T_MSG_FIELD_STRUCT 
               T_MSG_FIELD_STRUCT, 
              *T_MSG_FIELD;
 
typedef struct T_MSG_FILE_STRUCT 
               T_MSG_FILE_STRUCT, 
              *T_MSG_FILE;

typedef struct T_BUF_STRUCT 
               T_MSG_BUF_STRUCT, 
              *T_MSG_BUF;

typedef struct T_MSG_TRAV_STRUCT 
               T_MSG_TRAV_STRUCT, 
              *T_MSG_TRAV;
struct T_MSG_TRAV_STRUCT {
  T_INT2         *type_ptr;       /* stored as 2 bytes in message             */
  T_BOOL          unknown;        /* Is null message field? SVM 09/04/98      */
  T_PTR           value_ptr;      /* can be cast to appropriate type          */
  T_INT4         *array_size_ptr; /* NULL if not applicable                   */
  T_MSG_FIELD     field;          /* actual field                             */
  T_INT4          index;          /* numeric index for this field JAM 05/12/99*/
  T_STR           name;           /* name of field JAM 05/18/99               */
};
 
typedef struct T_MSG_NAME_NUM_MAP_STRUCT
               T_MSG_NAME_NUM_MAP_STRUCT,
              *T_MSG_NAME_NUM_MAP;
struct T_MSG_NAME_NUM_MAP_STRUCT {
  T_HASH_TABLE incoming_num_to_name; /* Maps incoming num to name */
  T_HASH_TABLE outgoing_name_to_num; /* Maps outgoing name to num */
  T_HASH_TABLE outgoing_num_to_name; /* Maps outgoing num to name */
  T_INT4      *outgoing_count;
};

/*
 * Function types
 */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_MSG_MT_TRAV_FUNC)
    T_TYPEDEF((T_STR, T_MSG_MT, T_PTR));
 
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_MSG_TRAV_FUNC)
    T_TYPEDEF((T_MSG, T_MSG_TRAV, T_PTR));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_MSG_ENCRYPT_FUNC)
    T_TYPEDEF((T_BUF, T_PTR, T_INT4, T_PTR));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_MSG_DECRYPT_FUNC)
    T_TYPEDEF((T_BUF, T_PTR, T_INT4, T_PTR));

#include <rtworks/msgmt.h>
#include <rtworks/msgpr.h>

#endif /* _RTWORKS_MSG_H_ */

