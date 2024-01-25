/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/ipcdevpr.h#1 $
 */

/* This file is generated by cproto - do not edit! */

/* ../rtworks/ipcdevpr.h -- generated prototypes */

#ifndef T_IPC_API
# ifndef T_OS_WIN32
# define T_IPC_API
# else
# ifndef T_STATIC_LINK
# ifndef T_IPC_SOURCE
# if !defined T_GENERATE_PROTOTYPES
# define T_IPC_API __declspec(dllimport)
# else
# define T_IPC_API
# endif
# else
# if !defined T_GENERATE_PROTOTYPES
# define T_IPC_API __declspec(dllexport)
# else
# define T_IPC_API
# endif
# endif
# else
# define T_IPC_API
# endif /* T_STATIC_LINK */
# endif /* T_OS_WIN32 */
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ipcdev.c */
T_IPC_API T_IPC_CONN T_ENTRY T_EXPORT TipcDevGetConn T_PROTO((void));
T_IPC_API T_BOOL T_ENTRY T_EXPORT TipcDevSetConn T_PROTO((
T_IPC_CONN conn
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevInitCommonMt T_PROTO((void));
T_IPC_API void T_ENTRY T_EXPORT TipcDevInitDaqMt T_PROTO((void));
T_IPC_API void T_ENTRY T_EXPORT TipcDevInitIeMt T_PROTO((void));
T_IPC_API void T_ENTRY T_EXPORT TipcDevInitMonMt T_PROTO((void));
T_IPC_API void T_ENTRY T_EXPORT TipcDevInitVarMt T_PROTO((void));
T_IPC_API void T_ENTRY T_EXPORT TipcDevClientCreate T_PROTO((
T_STR name,
T_INT4 pid
));
T_IPC_API T_IPC_CONN T_ENTRY T_EXPORT TipcDevServerCreate T_PROTO((
T_STR name,
T_INT4 pid
));
T_IPC_API T_BOOL T_ENTRY T_EXPORT TipcDevServerDestroy T_PROTO((
T_IPC_CONN conn,
T_STR name,
T_INT4 pid
));
T_IPC_API T_BOOL T_ENTRY T_EXPORT TipcDevMsgSend T_PROTO((
T_IPC_MSG msg
));
T_IPC_API T_BOOL T_ENTRY T_EXPORT TipcDevMsgForward T_PROTO((
T_IPC_MSG msg
));
T_IPC_API T_BOOL T_ENTRY T_EXPORT TipcDevSendNull T_PROTO((
T_IPC_MT mt
));
T_IPC_API T_BOOL T_ENTRY T_EXPORT TipcDevSendStr T_PROTO((
T_IPC_MT mt,
T_STR str
));
T_IPC_API T_BOOL T_ENTRY T_EXPORT TipcDevSendInt4 T_PROTO((
T_IPC_MT mt,
T_INT4 int4
));
T_IPC_API T_BOOL T_ENTRY_VA T_EXPORT TipcDevMsgWriteVa T_PROTO((
T_IPC_MT mt,
va_list var_arg_ptr
));
T_IPC_API T_BOOL T_ENTRY_VA T_EXPORT TipcDevMsgWrite T_PROTO((
T_IPC_MT mt,
...
));
T_IPC_API T_IPC_MSG T_ENTRY T_EXPORT TipcDevMsgSendRPC T_PROTO((
T_IPC_MSG msg
));
T_IPC_API T_IPC_MSG T_ENTRY_VA T_EXPORT TipcDevMsgWriteRPC T_PROTO((
T_IPC_MT mt,
...
));
T_IPC_API T_CB T_ENTRY T_EXPORT TipcDevProcessCbCreate T_PROTO((
T_IPC_MT mt,
T_IPC_CONN_PROCESS_CB_FUNC func,
T_CB_ARG arg
));
T_IPC_API T_CB T_ENTRY T_EXPORT TipcDevDefaultCbCreate T_PROTO((
T_IPC_CONN_DEFAULT_CB_FUNC func,
T_CB_ARG arg
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevGetConnTimeout T_PROTO((
T_REAL8 * timeout_ptr
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevIssueSupportNotice T_PROTO((
T_STR title
));
T_IPC_API T_BOOL T_ENTRY T_EXPORT TipcDevConnect T_PROTO((
T_IPC_CONN srv_conn,
T_REAL8 timeout,
T_BOOL nofork
));
T_IPC_API T_BOOL T_ENTRY T_EXPORT TipcDevFork T_PROTO((
T_STR program,
T_STR * argv
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevRecvTypedCommand T_PROTO((
T_IPC_CONN conn,
T_IPC_CONN_PROCESS_CB_DATA data,
T_CB_ARG arg
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevRecvInternalCommand T_PROTO((
T_IPC_CONN conn,
T_IPC_CONN_PROCESS_CB_DATA data,
T_CB_ARG arg
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevRecvInterrupt T_PROTO((
T_IPC_CONN conn,
T_IPC_CONN_PROCESS_CB_DATA data,
T_CB_ARG arg
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevProcessNonTyped T_PROTO((void));
T_IPC_API void T_ENTRY T_EXPORT TipcDevInterruptCheck T_PROTO((void));
T_IPC_API void T_ENTRY T_EXPORT TipcDevSetState T_PROTO((
T_STR value
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevMainLoop T_PROTO((void));
T_IPC_API T_PTR T_ENTRY T_EXPORT TipcDevAddOneCvt T_PROTO((
T_STR name,
T_TIME_CVT ptr,
T_PTR dummy
));
T_IPC_API T_PTR T_ENTRY T_EXPORT TipcDevAddOneOption T_PROTO((
T_STR option_name,
T_OPTION option,
T_PTR dummy
));
T_IPC_API T_PTR T_ENTRY T_EXPORT TipcDevPrimeOneOption T_PROTO((
T_STR option_name,
T_OPTION option,
T_PTR dummy
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevSendOneOption T_PROTO((
T_OPTION option,
T_OPTION_CHANGE_CB_DATA cbdata,
T_PTR dummy
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevRecvTimeToStringCall T_PROTO((
T_IPC_CONN conn,
T_IPC_CONN_PROCESS_CB_DATA data,
T_CB_ARG arg
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevRecvDoQuit T_PROTO((
T_IPC_CONN conn,
T_IPC_CONN_PROCESS_CB_DATA data,
T_CB_ARG arg
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevRecvWatchMsgQueueStart T_PROTO((
T_IPC_CONN conn,
T_IPC_CONN_PROCESS_CB_DATA data,
T_CB_ARG arg
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevRecvWatchMsgQueueStop T_PROTO((
T_IPC_CONN conn,
T_IPC_CONN_PROCESS_CB_DATA data,
T_CB_ARG arg
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevPrintMsgError T_PROTO((
T_IPC_CONN conn,
T_IPC_CONN_DEFAULT_CB_DATA data,
T_CB_ARG arg
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevCommandParseQuit T_PROTO((
T_LEX lexer
));
T_IPC_API void T_ENTRY T_EXPORT TipcDevSetStateLoading T_PROTO((void));
T_IPC_API void T_ENTRY T_EXPORT TipcDevSetStateAcquiring T_PROTO((void));
T_IPC_API void T_ENTRY T_EXPORT TipcDevSetStateDelaying T_PROTO((void));
T_IPC_API T_BOOL T_ENTRY_VA T_EXPORT TipcDevSendControl T_PROTO((
T_STR fmt,
...
));
T_IPC_API T_BOOL T_ENTRY T_EXPORT TipcDevGetInternalCommand T_PROTO((void));

#ifdef __cplusplus
}
#endif
