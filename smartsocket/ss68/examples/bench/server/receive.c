/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/bench/server/receive.c#1 $
 */

#include <rtworks/ipc.h>
#include "srvbench.h"

static T_BOOL do_exit;

/* ========================================================================= */
/*..bmCbProcessReceiverExit -- callback for processing exit message */
static void T_ENTRY bmCbProcessReceiverExit
(
T_IPC_CONN                  conn,
T_IPC_CONN_PROCESS_CB_DATA  data,
T_CB_ARG                    arg
)
{
  T_BOOL status;

  T_ASSERT(conn != NULL);
  T_ASSERT(data != NULL);
  T_ASSERT(arg == NULL);

  TutOut("Received exit message.\n");
  TutCommandParseStr("stats");  /* end timing */

  /* 
   * tell sender we are going to exit 
   */
  status = TipcSrvMsgWrite(T_BM_SUBJECT_SEND,
                           TipcMtLookupByNum(T_BM_MT_RECEIVER_EXIT),
                           FALSE,
                           NULL);
  T_ASSERT(status == TRUE);
  status = TipcSrvFlush();
  T_ASSERT(status == TRUE);
  do_exit = T_TRUE;
} 

/* ========================================================================= */
/*..main -- main program */
int main
(
int    argc,
char **argv
)
{
  T_OPTION  option;
  T_BOOL    status;
  T_INT4    param[3];
  T_IPC_MT  mt;

#ifndef T_RTWORKS35
  if (getenv("BENCH_INIT_THREADS")) {
    TipcInitThreads();
  }
#endif

#ifdef T_OS_VMS
  TutVmsRedirect((T_INT4 *)&argc, &argv);
#endif

  /*
   * Check arguments. 
   */
  if (argc != 5) {
    TutOut("%s expects 4 arguments.\n", argv[0]);
    TutOut("\nSender process should be running before starting\n");
    TutOut("the receiver process(es).\n");
    TutOut("\nUsage: %s <ack rate>\n", argv[0]);
    TutOut("            <# grammar iterations/message\n");
    TutOut("            <boolean int traverse/notraverse messages>\n");
    TutExit(T_EXIT_FAILURE);
  }

  param[T_BM_PARAM_ACK_RATE]    = atoi(argv[1]);
  param[T_BM_PARAM_ITERATIONS]  = atoi(argv[2]);
  param[T_BM_PARAM_TRAVERSE]    = atoi(argv[3]);
  if (
     1 > param[T_BM_PARAM_ACK_RATE] 
     ||
     1 > param[T_BM_PARAM_ITERATIONS]
     ) {
    TutOut("%s: invalid arguments \"%s %s %s %s\"\n",
           argv[0], 
           argv[1], 
           argv[2], 
           argv[3], 
           argv[4]);
    TutExit(T_EXIT_FAILURE);
  }

  option = TutOptionLookup("project");
  T_ASSERT(option != NULL);
  status = TutOptionSetEnum(option, T_BM_PROJECT);
  T_ASSERT(status == TRUE);

  TutCommandParseStr("setopt _conn_read_size_init 32768");
  TutCommandParseStr("unsetopt trace_flags");
  TutCommandParseStr("unsetopt catalog_flags");

  /*
   * Allow memory-only GMD.
   */
  if (getenv("BENCH_MEMORY_GMD") == NULL) {
    /*
     * Set unique_subject in case guaranteed message delivery is used
     */
    option = TutOptionLookup("unique_subject");
    T_ASSERT(option != NULL);
    status = TutOptionSetEnum(option, argv[4]);
    T_ASSERT(status == TRUE);
  }

  /* 
   * Use a remote RTserver if BENCH_SERVER_NODE is set.
   */
  if (getenv("BENCH_SERVER_NODE") != NULL) {
    T_STRING setopt_cmd;

    sprintf(setopt_cmd, "setopt server_names %s", getenv("BENCH_SERVER_NODE"));
    TutCommandParseStr(setopt_cmd);
  }

  if (TutFileExists("receive.cm")) {
    TutCommandParseStr("source receive.cm");
  }

  T_BM_MT_INIT();
  status = TbmCreateGeneratedMt();
  T_EXPECT(T_TRUE == status);

  /*
   * Clear out any old GMD files. 
   */
  status = TipcSrvGmdFileDelete();
  T_ASSERT(status == TRUE);

  TutOut("Connecting to RTserver.\n");
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver.\n");
    TutExit(T_EXIT_FAILURE);
  }

  /*
   * Create callback for T_BM_MT_RECEIVER_EXIT
   */
  mt = TipcMtLookupByNum(T_BM_MT_RECEIVER_EXIT);
  T_ASSERT(mt != NULL);
  if (T_NULL == TipcSrvProcessCbCreate(mt, bmCbProcessReceiverExit, T_NULL)) {
    TutOut("Could not create T_BM_MT_RECEIVER_EXIT callback.\n");
    TutExit(T_EXIT_FAILURE);
  }

  /*
   * Create callback for T_BM_MT_GENERATED_USER_TYPE 
   */
  mt = TipcMtLookupByNum(T_BM_MT_GENERATED_USER_TYPE);
  T_ASSERT(mt != NULL);
  if (T_NULL == TipcSrvProcessCbCreate(mt, TbmCbProcessGenerated,(T_CB_ARG)param)) {
    TutOut("Could not create T_MT_BM_GENERATED_USER_TYPE callback.\n");
    TutExit(T_EXIT_FAILURE);
  }

  status = TipcSrvSubjectSetSubscribe(T_BM_SUBJECT_RECEIVE, TRUE);
  T_ASSERT(status == TRUE);
  TutOut("Started receiving %s subject.\n", T_BM_SUBJECT_RECEIVE);
  status = TipcSrvMsgWrite(T_BM_SUBJECT_SEND,
                           TipcMtLookupByNum(T_BM_MT_RECEIVER_READY), 
                           FALSE,
                           NULL);
  T_ASSERT(status == TRUE);
  status = TipcSrvFlush();
  T_ASSERT(status == TRUE);
  TutCommandParseStr("stats");  /* begin timing */
  while (!do_exit) {
    TipcSrvMainLoop(1.0);
  }
  status = TipcSrvDestroy(T_IPC_SRV_CONN_NONE);
  T_ASSERT(status == TRUE);
}