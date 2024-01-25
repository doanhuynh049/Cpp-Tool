/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/error.h#3 $
 */

/* =================================================================== */
/*                                                                     */
/*                    error.h -- error handling                        */
/*                                                                     */
/* =================================================================== */

/*
 * If you add an error code, please add a row to the error code table
 * in the UTIL man pages chapter.  SMW 06/24/95
 *
 * Also make sure you add a reasonable error name (English, all lower
 * case), and add a line to utErrStrInit so it gets added to the hash
 * table. JAM 03/20/97
 *
 * For binary compatibility reasons, existing error codes can no longer
 * be renumbered.  SMW 01/01/97
 */

#ifndef _RTWORKS_ERROR_H_
#define _RTWORKS_ERROR_H_

/*
 * UTIL errors.  Note that the UTIL and IPC error codes must be globally
 * available to everyone, since everyone must be able to set and unset
 * them.
 */

#define T_ERR_UTIL_START   0

/* ZERO IS NOT A VALID ERROR CODE!!! SMW 3/21/93 */
/* ...but use it for 'no error' - MW 2005-09-26 */
#define T_ERR_NONE 0

#define T_ERR_C  (T_ERR_UTIL_START + 1)
#define T_ERRNM_C "C"
#define T_ERR_OS  (T_ERR_UTIL_START + 2)
#define T_ERRNM_OS "OS"
#define T_ERR_NULL_PTR  (T_ERR_UTIL_START + 3)
#define T_ERRNM_NULL_PTR "NULL pointer"
#define T_ERR_VAL_TOO_SMALL  (T_ERR_UTIL_START + 4)
#define T_ERRNM_VAL_TOO_SMALL "value too small"
#define T_ERR_VAL_TOO_LARGE  (T_ERR_UTIL_START + 5)
#define T_ERRNM_VAL_TOO_LARGE "value too large"
#define T_ERR_ALREADY_EXISTS  (T_ERR_UTIL_START + 6)
#define T_ERRNM_ALREADY_EXISTS "already exists"
#define T_ERR_VAL_INVALID  (T_ERR_UTIL_START + 7)
#define T_ERRNM_VAL_INVALID "value invalid"
#define T_ERR_DOESNT_EXIST  (T_ERR_UTIL_START + 8)
#define T_ERRNM_DOESNT_EXIST "doesn't exist"
#define T_ERR_TIMEOUT_REACHED  (T_ERR_UTIL_START + 9)
#define T_ERRNM_TIMEOUT_REACHED "timeout reached"
#define T_ERR_EOF  (T_ERR_UTIL_START + 10)
#define T_ERRNM_EOF "end of file"
/* TODO: merge with T_ERR_SAME_VALUE from ie.h.  SMW 01/25/94 */
/*  T_ERR_SAME_VALUE has been removed from ie.h use */
/*  T_ERR_VAL_SAME instead   jeh 06/30/95 */
#define T_ERR_VAL_SAME  (T_ERR_UTIL_START + 11)
#define T_ERRNM_VAL_SAME "value is the same"
#define T_ERR_VAL_UNKNOWN  (T_ERR_UTIL_START + 12)
#define T_ERRNM_VAL_UNKNOWN "value unknown"
#define T_ERR_VAL_REQUIRED  (T_ERR_UTIL_START + 13)
#define T_ERRNM_VAL_REQUIRED "value required"
#define T_ERR_TYPE_INVALID  (T_ERR_UTIL_START + 14)
#define T_ERRNM_TYPE_INVALID "type invalid"
#define T_ERR_TYPE_MISMATCH  (T_ERR_UTIL_START + 15)
#define T_ERRNM_TYPE_MISMATCH "type mismatch"
#define T_ERR_INTERRUPTED  (T_ERR_UTIL_START + 16)
#define T_ERRNM_INTERRUPTED "interrupted"
#define T_ERR_VERIFY_FAILED  (T_ERR_UTIL_START + 17)
#define T_ERRNM_VERIFY_FAILED "verify failed"
#define T_ERR_END_OF_TRAVERSAL  (T_ERR_UTIL_START + 18)
#define T_ERRNM_END_OF_TRAVERSAL "end of traversal"
#define T_ERR_EMPTY  (T_ERR_UTIL_START + 19)
#define T_ERRNM_EMPTY "empty"
#define T_ERR_VAL_MISMATCH  (T_ERR_UTIL_START + 20)
#define T_ERRNM_VAL_MISMATCH "value mismatch"
#define T_ERR_TYPE_UNKNOWN  (T_ERR_UTIL_START + 21)
#define T_ERRNM_TYPE_UNKNOWN "type unknown"
#define T_ERR_FILE_CORRUPT  (T_ERR_UTIL_START + 24)
#define T_ERRNM_FILE_CORRUPT "file corrupt"
#define T_ERR_WOULD_BLOCK (T_ERR_UTIL_START + 25)
#define T_ERRNM_WOULD_BLOCK "would block"
#define T_ERR_READ_ONLY (T_ERR_UTIL_START + 28)
#define T_ERRNM_READ_ONLY "read only"
#define T_ERR_SOCKET  (T_ERR_UTIL_START + 29)
#define T_ERRNM_SOCKET "socket"
#define T_ERR_NOMEM  (T_ERR_UTIL_START + 30)
#define T_ERRNM_NOMEM "no memory"
#define T_ERR_UNSUPPORTED  (T_ERR_UTIL_START + 31)
#define T_ERRNM_UNSUPPORTED "unsupported"
#define T_ERR_OBJECT_IN_USE (T_ERR_UTIL_START + 32)
#define T_ERRNM_OBJECT_IN_USE "object in use"
#define T_ERR_MUX_MISMATCH (T_ERR_UTIL_START + 33)
#define T_ERRNM_MUX_MISMATCH "mux mismatch"
#define T_ERR_SESSION_ABANDONED (T_ERR_UTIL_START + 34)
#define T_ERRNM_SESSION_ABANDONED "session abandoned"

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
#define T_ERR_UNIX  T_ERR_C
#define T_ERR_VMS  T_ERR_OS
#define T_ERR_WIN32  T_ERR_OS
#define T_ERR_WINSOCK  T_ERR_SOCKET
#endif

/* no separate T_ERR_OS and T_ERR_SOCKET on UNIX */
#if defined(T_OS_UNIX)
#undef T_ERR_OS
#define T_ERR_OS T_ERR_C
#undef T_ERR_SOCKET
#define T_ERR_SOCKET T_ERR_C
#endif

#ifdef T_OS_VMS /* no separate T_ERR_SOCKET on VMS */
#undef T_ERR_SOCKET
#define T_ERR_SOCKET T_ERR_C
#endif

/*
 * IPC errors.
 */

#define T_ERR_IPC_START  500

#define T_ERR_CONN_INIT_FAILED (T_ERR_IPC_START + 0)
#define T_ERRNM_CONN_INIT_FAILED "connection initialization failed"
#define T_ERR_SRV_NOT_CONNECTED (T_ERR_IPC_START + 2)
#define T_ERRNM_SRV_NOT_CONNECTED "server not connected"
#define T_ERR_SRV_ALREADY_CONNECTED (T_ERR_IPC_START + 3)
#define T_ERRNM_SRV_ALREADY_CONNECTED "server already connected"
#define T_ERR_SRV_ACCESS_DENIED (T_ERR_IPC_START + 4)
#define T_ERRNM_SRV_ACCESS_DENIED "server denied access"
#define T_ERR_MSG_CORRUPT (T_ERR_IPC_START + 5)
#define T_ERRNM_MSG_CORRUPT "message corrupt"
#define T_ERR_MSG_INVALID (T_ERR_IPC_START + 7)
#define T_ERRNM_MSG_INVALID "message invalid"
#define T_ERR_MSG_EOM (T_ERR_IPC_START + 8)
#define T_ERRNM_MSG_EOM "end of message"
#define T_ERR_FT_INVALID (T_ERR_IPC_START + 9)
#define T_ERRNM_FT_INVALID "field type invalid"
#define T_ERR_FT_MISMATCH (T_ERR_IPC_START + 10)
#define T_ERRNM_FT_MISMATCH "field type mismatch"
#define T_ERR_IN_PROGRESS (T_ERR_IPC_START + 12)
#define T_ERRNM_IN_PROGRESS "in progress"
#define T_ERR_FAILURE_DETECTED (T_ERR_IPC_START + 14)
#define T_ERRNM_FAILURE_DETECTED "failure detected"
#define T_ERR_LICENSE_DENIED (T_ERR_IPC_START + 15)
#define T_ERRNM_LICENSE_DENIED "license denied"
#define T_ERR_IN_USE (T_ERR_IPC_START + 17)
#define T_ERRNM_IN_USE "in use"
#define T_ERR_GMD_SENDER_TIMEOUT (T_ERR_IPC_START + 18)
#define T_ERRNM_GMD_SENDER_TIMEOUT "GMD sender timeout"
#define T_ERR_GMD_RECEIVER_TIMEOUT (T_ERR_IPC_START + 19)
#define T_ERRNM_GMD_RECEIVER_TIMEOUT "GMD receiver timeout"
#define T_ERR_GMD_RECEIVER_EXIT (T_ERR_IPC_START + 20)
#define T_ERRNM_GMD_RECEIVER_EXIT "GMD receiver exit"
#define T_ERR_GMD_NO_RECEIVERS (T_ERR_IPC_START + 21)
#define T_ERRNM_GMD_NO_RECEIVERS "GMD no receivers"
#define T_ERR_PROTOCOL_MISMATCH (T_ERR_IPC_START + 22)
#define T_ERRNM_PROTOCOL_MISMATCH "protocol mismatch"
#define T_ERR_SEND_QUEUED_IN_DRIVER (T_ERR_IPC_START + 23)
#define T_ERRNM_SEND_QUEUED_IN_DRIVER "send queued in driver"
#define T_ERR_RECV_QUEUED_IN_DRIVER (T_ERR_IPC_START + 24)
#define T_ERRNM_RECV_QUEUED_IN_DRIVER "recv queued in driver"
#define T_ERR_SRV_CTLMSG_REJECTED (T_ERR_IPC_START + 25)
#define T_ERRNM_SRV_CTLMSG_REJECTED "server rejected control message"

#ifdef T_RTWORKS35_COMPAT /* leave around forever?  SMW 05/26/96 */
#define T_ERR_CONN_EOF T_ERR_EOF /* for compat with 3.5 */
#define T_ERR_MSG_READ_ONLY T_ERR_READ_ONLY /* for compat with 3.5 */
#endif

/* ----------------------------------------------------- */
/* I guess the IE/DAQ/HCI start could overlap, but don't */
/* bother for now.                                       */
/* ----------------------------------------------------- */

/* ----------------------------------------------------- */
/* RTie errors.  See <rtworks/ie.h> for the error codes. */
/* ----------------------------------------------------- */
#define T_ERR_IE_START   1000

/* ------------------------------------------------------- */
/* RTdaq errors.  See <rtworks/daq.h> for the error codes. */
/* ------------------------------------------------------- */
#define T_ERR_DAQ_START  2000

/* ------------------------------------------------------- */
/* RThci errors.  See <rtworks/hci.h> for the error codes. */
/* ------------------------------------------------------- */
#define T_ERR_HCI_START  3000

#define T_ERR_SRV_START  4000
/* --------------------- */
/* RTserver error codes. */
/* --------------------- */
#define T_ERR_PENDING (T_ERR_SRV_START + 1)
#define T_ERRNM_PENDING "operation pending completion"
#define T_ERR_STATE_INVALID (T_ERR_SRV_START + 2)
#define T_ERRNM_STATE_INVALID "invalid state"
#define T_ERR_EVENT_INVALID (T_ERR_SRV_START + 3)
#define T_ERRNM_EVENT_INVALID "invalid event"
#define T_ERR_MAX_BUF_EXCEEDED (T_ERR_SRV_START + 4)
#define T_ERRNM_MAX_BUF_EXCEEDED "max buffer exceeded"
#define T_ERR_MODE_INVALID (T_ERR_SRV_START + 5)
#define T_ERRNM_MODE_INVALID "invalid mode"
#define T_ERR_MAX_CLIENTS_EXCEEDED (T_ERR_SRV_START + 6)
#define T_ERRNM_MAX_CLIENTS_EXCEEDED "max clients exceeded"
#define T_ERR_CONN_REQUEST_REJECTED (T_ERR_SRV_START + 7)
#define T_ERRNM_CONN_REQUEST_REJECTED "connection request rejected"
#define T_ERR_MAX_SERVERS_EXCEEDED (T_ERR_SRV_START + 8)
#define T_ERRNM_MAX_SERVERS_EXCEEDED "max servers exceeded"
#define T_ERR_DISCONNECT_MODE_INVALID (T_ERR_SRV_START + 9)
#define T_ERRNM_DISCONNECT_MODE_INVALID "invalid disconnect mode"
#define T_ERR_AUTHENTICATION_FAILED (T_ERR_SRV_START + 10)
#define T_ERRNM_AUTHENTICATION_FAILED "authentication failed"
#define T_ERR_CLIENT_NOT_LOCAL (T_ERR_SRV_START + 11)
#define T_ERRNM_CLIENT_NOT_LOCAL "client is not local"
#define T_ERR_MAX_CLIENT_LICENSES_EXCEEDED (T_ERR_SRV_START + 12)
#define T_ERRNM_MAX_CLIENT_LICENSES_EXCEEDED "max client licenses exceeded"
#define T_ERR_CLIENT_CREATE_REJECTED (T_ERR_SRV_START + 13)
#define T_ERRNM_CLIENT_CREATE_REJECTED "client creation rejected"
#define T_ERR_CLIENT_NAME_NOT_UNIQUE (T_ERR_SRV_START + 14)
#define T_ERRNM_CLIENT_NAME_NOT_UNIQUE "client name is not unique"
#define T_ERR_CLIENT_UNIQUENESS_CHECK_IN_PROGRESS (T_ERR_SRV_START + 15)
#define T_ERRNM_CLIENT_UNIQUENESS_CHECK_IN_PROGRESS "client uniqueness check in progress"
#define T_ERR_AUTHORIZATION_DENIED (T_ERR_SRV_START + 16)
#define T_ERRNM_AUTHORIZATION_DENIED "authorization denied"
#define T_ERR_SUBJECT_INVALID (T_ERR_SRV_START + 17)
#define T_ERRNM_SUBJECT_INVALID "invalid subject"
#define T_ERR_SUBJECT_ALREADY_SUBSCRIBED (T_ERR_SRV_START + 18)
#define T_ERRNM_SUBJECT_ALREADY_SUBSCRIBED "subject already subscribed to"
#define T_ERR_SUBJECT_NOT_SUBSCRIBED (T_ERR_SRV_START + 19)
#define T_ERRNM_SUBJECT_NOT_SUBSCRIBED "subject not subscribed to"
#define T_ERR_DUP_CLIENT_MID (T_ERR_SRV_START + 20)
#define T_ERRNM_DUP_CLIENT_MID "duplicate member id"
#define T_ERR_SERVER_NAME_NOT_UNIQUE (T_ERR_SRV_START + 21)
#define T_ERRNM_SERVER_NAME_NOT_UNIQUE "server name is not unique"
#define T_ERR_SERVER_CREATE_REJECTED (T_ERR_SRV_START + 22)
#define T_ERRNM_SERVER_CREATE_REJECTED "server creation rejected"

/* ------------------------------------------------------------- */
/* CHAR_FORMAT errors. See <rtworks/util.h> for the error codes. */
/* ------------------------------------------------------------- */
#define T_ERR_CF_START  5000

/* --------------------------------------------------------------- */
/* Tmqi (SmartQueue) errors. See <rtworks/sqerror.h>               */
/* for the error codes.                                            */
/* --------------------------------------------------------------- */
#define T_ERR_MQI_START  6000

/* --------------------------------------------------------------- */
/* SmartQueue server errors.                                       */
/* --------------------------------------------------------------- */
#define T_ERR_SQ_START  7000

/* --------------------------------------------------------------- */
/* Encrypt/Decrypt Api errors.                                     */
/* --------------------------------------------------------------- */
#define T_ERR_ENCRYPT_START 8000

#define T_ERR_NOT_ENCRYPTED (T_ERR_ENCRYPT_START + 1)
#define T_ERRNM_NOT_ENCRYPTED "msg not encrypted"

#define T_ERR_NO_ENCRYPT_KEY (T_ERR_ENCRYPT_START + 2)
#define T_ERRNM_NO_ENCRYPT_KEY "no encrypt key"

/*
 * SUT errors
 */
#define T_ERR_SUT_START     9000    /* 9000  -> 9999  */

/*
 * SPC errors
 */
#define T_ERR_SPC_START     10000   /* 10000 -> 10999 */

/*
 * SVC errors
 */
#define T_ERR_SVC_START     11000   /* 11000 -> 11999 */

/*
 * SPSS errors
 */
#define T_ERR_SPSS_START    12000   /* 12000 -> 12999 */

/*
 * Gateway errors
 */
#define T_ERR_GW_START      13000   /* 13000 -> 13999 */

/*
 * Gateway MSL (and drivers) errors
 */
#define T_ERR_GW_MSL_START  14000   /* 13000 -> 17999 */

/*
 * Gateway CI (and drivers) errors
 */
#define T_ERR_GW_CI_START   18000   /* 18000 -> 22999 */

/* --------------------------------------------------------------- */
/* SSL protocol driver errors.                                     */
/* --------------------------------------------------------------- */
#define T_ERR_SSL_START     23000   /* 23000 -> 23999 */
/*
 * new SSL error codes should be added to ipcLinkSslErrorRegister()
 * and ipcLinkSslMapErrors in lnkssl.c
 */
#define T_ERR_SSL_ERROR (T_ERR_SSL_START + 1)
#define T_ERRNM_SSL_ERROR "SSL error"
#define T_ERR_SSL_PROTOCOL_ERROR (T_ERR_SSL_START + 2)
#define T_ERRNM_SSL_PROTOCOL_ERROR "invalid certificate chain"
#define T_ERR_SSL_CERT_CHAIN_INVALID (T_ERR_SSL_START + 3)
#define T_ERRNM_SSL_CERT_CHAIN_INVALID "invalid certificate chain"
#define T_ERR_SSL_CERT_EXPIRED (T_ERR_SSL_START + 4)
#define T_ERRNM_SSL_CERT_EXPIRED "certificate expired"
#define T_ERR_SSL_CERT_CHAIN_INCOMPLETE (T_ERR_SSL_START + 5)
#define T_ERRNM_SSL_CERT_CHAIN_INCOMPLETE "certificate chain incomplete"
#define T_ERR_SSL_DATA_NOT_FOUND (T_ERR_SSL_START + 6)
#define T_ERRNM_SSL_DATA_NOT_FOUND "data not found"
#define T_ERR_SSL_UNAUTHORIZED_CA (T_ERR_SSL_START + 7)
#define T_ERRNM_SSL_UNAUTHORIZED_CA "unauthorized ca"
#define T_ERR_SSL_UNKNOWN_CRITICAL_EXTENSION (T_ERR_SSL_START + 8)
#define T_ERRNM_SSL_UNKNOWN_CRITICAL_EXTENSION "unknown critical extension"
#define T_ERR_SSL_UNAUTHORIZED_CERT (T_ERR_SSL_START + 9)
#define T_ERRNM_SSL_UNAUTHORIZED_CERT "unauthorized certificate"
#define T_ERR_SSL_SIGNATURE_INVALID (T_ERR_SSL_START + 10)
#define T_ERRNM_SSL_SIGNATURE_INVALID "invalid signature"

/* --------------------------------------------------------------- */
/* proxy protocol driver errors.                                   */
/* --------------------------------------------------------------- */
#define T_ERR_PROXY_START 24000   /* 24000 -> 24999 */

#define T_ERR_PROXY_INVALID_RESPONSE (T_ERR_PROXY_START + 0)
#define T_ERRNM_PROXY_INVALID_RESPONSE "invalid response from proxy"
#define T_ERR_PROXY_UNAUTHORIZED (T_ERR_PROXY_START + 1)
#define T_ERRNM_PROXY_UNAUTHORIZED "unauthorized"
#define T_ERR_PROXY_FORBIDDEN (T_ERR_PROXY_START + 2)
#define T_ERRNM_PROXY_FORBIDDEN "forbidden"
#define T_ERR_PROXY_SERVER_ERROR (T_ERR_PROXY_START + 3)
#define T_ERRNM_PROXY_SERVER_ERROR "proxy server error"


/* --------------------------------------------------------------- */
/* web server errors.                                              */
/* --------------------------------------------------------------- */
#define T_ERR_WEB_SERVER_START 25000   /* 25000 -> 25999 */


/* --------------------------------------------------------------- */
/* HTTP/HTTPS link driver errors.                                  */
/* --------------------------------------------------------------- */
#define T_ERR_LINK_HTTP_START 26000   /* 26000 -> 26999 */

/*
 * Security Manager (and drivers) errors
 */
#define T_ERR_SM_START        27000   /* 27000 -> 27999 */

/*
 * Assured Message Delivery errors (AMD)
 */
#define T_ERR_AMD_START       50000   /* 50000 -> 50099 */

/*
 * T_ERR_SAVE is used to save error codes when an error occurs and then
 * restore them after some cleanup code has been executed (the cleanup
 * code may overwrite the error codes).
 */
#define T_ERR_MAX_TRACES 16
typedef struct T_ERR_SAVE_STRUCT {
  T_INT4 err_num; /* Talarian error code */
  T_INT4 c_err_num;
  T_INT4 os_err_num;
  T_INT4 socket_err_num;
  T_INT4 trace_count;
  T_PTR trace[T_ERR_MAX_TRACES];
} T_ERR_SAVE_STRUCT, *T_ERR_SAVE;

/* ---------------------------------------------------------------- */
/* The type of error that occurred.                                 */
/* Warning: Print error message and continue.                       */
/* Severe:  Print an error message and long jump to safe place.     */
/* Fatal:   Print error message and exit or abort with stack trace. */
/* ---------------------------------------------------------------- */
enum T_ERR_SEVERITY {
  T_ERR_WARNING,
  T_ERR_SEVERE,
  T_ERR_FATAL
};

typedef enum T_ERR_SEVERITY T_ERR_SEVERITY;

/* ------------------------------------------------------------------ */
/* Maximum number of parameters that are passed to the error          */
/* handling functions.  I wouldn't want to see the message that needs */
/* more than 10 error parameters. :-)                                 */
/* ------------------------------------------------------------------ */
#define T_ERR_MAX_PARAMS     10

/* -------------------------------------- */
/* Set the detail of the output displayed */
/* -------------------------------------- */
enum T_ERR_FEEDBACK {
  T_ERR_NOVICE,
  T_ERR_EXPERT,
  T_ERR_NO_OUTPUT
};

typedef enum T_ERR_FEEDBACK T_ERR_FEEDBACK;

/* ------------------------------------------------------- */
/* Types allowed as input parameters to the error handlers */
/* ------------------------------------------------------- */
enum T_ERR_ARG_TYPE {
  T_ERR_ARG_NONE,        /* Must be first and equal to NULL */
  T_ERR_ARG_STR,
  T_ERR_ARG_REAL4,       /* Not currently supported */
  T_ERR_ARG_REAL8,
  T_ERR_ARG_CHAR,
  T_ERR_ARG_INT2,        /* Not currently supported */
  T_ERR_ARG_UINT2,       /* Not currently supported */
  T_ERR_ARG_INT4,
  T_ERR_ARG_UINT4,       /* Not currently supported */
  T_ERR_ARG_LONG_INT,    /* printf needs machine types for */
  T_ERR_ARG_ULONG_INT,   /* short and long types (h,l,L)   */
  T_ERR_ARG_SHORT_INT,
  T_ERR_ARG_USHORT_INT,
  T_ERR_ARG_LONG_DOUBLE
};

typedef enum T_ERR_ARG_TYPE T_ERR_ARG_TYPE;

/* --------------- */
/* Error structure */
/* --------------- */
typedef struct T_ERR_STRUCT T_ERR_STRUCT;
struct T_ERR_STRUCT {
  FILE *err_file;      /* error file */
  T_OUT_FUNC err_func;
  T_INT4 err_num;      /* error number from error.h */
  T_STR caller;        /* name of the routine where the error occurred */
  T_REAL8 time;        /* time when the error occurred */
  T_ERR_SEVERITY severity;    /* Severity level from error.h */
  T_ERR_FEEDBACK feedback;  /* Feedback level for the user */
  T_INT4 param_count;       /* Number of parameters in the list */
};

/* -------------------------------------------------------------- */
/* Error parameter structure.  Contains the format string used to */
/* display the value and set the arg type.  Symbolic name is set  */
/* when the error file is read.                                   */
/* -------------------------------------------------------------- */
typedef struct T_ERR_PARAM T_ERR_PARAM;
struct T_ERR_PARAM {
  T_STR           format_str;
  T_PTR           value;
  T_STR           symbolic_name;
  T_ERR_ARG_TYPE  arg_type;
};

#endif /* _RTWORKS_ERROR_H_ */

