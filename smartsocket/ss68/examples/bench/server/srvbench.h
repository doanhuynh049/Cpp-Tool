/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/bench/server/srvbench.h#1 $
 */

#define T_BM_PROJECT                "ipcb_project"

#define T_BM_SUBJECT_SEND           "/ipc_bench_send_subject"
#define T_BM_SUBJECT_RECEIVE        "/ipc_bench_receive_subject"

#define T_BM_UD_SEND                "/send"
#define T_BM_UD_RECEIVE             "/receive"

#define T_BM_MT_RECEIVER_READY      1000
#define T_BM_MT_ACK                 1001
#define T_BM_MT_RECEIVER_EXIT       1002
#define T_BM_MT_GENERATED_USER_TYPE 1003

#define T_BM_PARAM_ACK_RATE         0
#define T_BM_PARAM_ITERATIONS       1
#define T_BM_PARAM_TRAVERSE         2

T_BOOL T_ENTRY T_EXPORT TbmCreateGeneratedMt T_PROTO(());
T_BOOL T_ENTRY T_EXPORT TbmBuildGeneratedMsg T_PROTO((
       T_IPC_MSG msg,
       T_INT4 iterations				      
       ));
void   T_ENTRY T_EXPORT TbmCbProcessGenerated T_PROTO((
       T_IPC_CONN conn,
       T_IPC_CONN_PROCESS_CB_DATA data,
       T_CB_ARG arg
       ));
T_INT4 T_ENTRY T_EXPORT TbmGeneratedMsgDataSize T_PROTO(());

#define T_BM_MT_INIT() \
{ \
  T_IPC_MT mt; \
  T_BOOL   status; \
\
  mt = TipcMtCreate("bm_receiver_ready", T_BM_MT_RECEIVER_READY, ""); \
  T_ASSERT(mt != NULL); \
  status = TipcSrvLogAddMt(T_IPC_SRV_LOG_INTERNAL, mt); \
  T_ASSERT(T_TRUE == status); \
\
  mt = TipcMtCreate("bm_ack", T_BM_MT_ACK, "int4"); \
  T_ASSERT(mt != NULL); \
  status = TipcSrvLogAddMt(T_IPC_SRV_LOG_INTERNAL, mt); \
  T_ASSERT(T_TRUE == status); \
\
  mt = TipcMtCreate("bm_receiver_exit", T_BM_MT_RECEIVER_EXIT, ""); \
  T_ASSERT(mt != NULL); \
  status = TipcSrvLogAddMt(T_IPC_SRV_LOG_INTERNAL, mt); \
  T_ASSERT(T_TRUE == status); \
\
} /* T_BM_MT_INIT */
