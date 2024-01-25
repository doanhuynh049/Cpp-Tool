/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/bench/server/send.c#1 $
 */

#include <rtworks/ipc.h>
#include "srvbench.h"

static T_INT4 bmNumExpectedReceivers = 0;
static T_INT4 bmNumReceivers = 0;
static T_INT4 bmReceiverIndex = 0;

/* ========================================================================= */
/*..bmSafeExit -- shutdown receivers, and exit */
static void bmSafeExit
(
T_INT4 ret_code
)
{   
  T_BOOL    status;
  T_IPC_MSG exit_msg;
  T_INT4    counter;
  T_IPC_MT  mt;
  T_INT4    mt_num; 

  /* 
   * Send an exit message to the receivers. 
   */
  status = TipcSrvMsgWrite(T_BM_SUBJECT_RECEIVE,
                           TipcMtLookupByNum(T_BM_MT_RECEIVER_EXIT),
                           FALSE,
                           NULL); 
  T_ASSERT(status == TRUE);

  /*
   * wait for receivers to exit 
   */
  for (counter = 0; counter < bmNumReceivers; ++counter) {
    /*
     * Read from RTserver with a 30 second timeout. 
     */
    exit_msg = TipcSrvMsgNext(30.0);
    if (exit_msg == NULL) {
      TutOut("Timed out on receiving exit message from receiver #%d\n", counter);
    }
    else {
      status = TipcMsgGetType(exit_msg, &mt);
      T_ASSERT(status == TRUE);
      T_ASSERT(mt != NULL);
      status = TipcMtGetNum(mt, &mt_num);
      if (mt_num == T_BM_MT_RECEIVER_EXIT) {
        TutOut("Receiver #%d is exiting (normal).\n", counter);
      }
      else {
        TutOut("Abnormal exit for receiver %d.\n",counter);
        TutOut("Expected exit message !\n");
      }
      status = TipcSrvMsgProcess(exit_msg);
      T_ASSERT(status == TRUE);
      status = TipcMsgDestroy(exit_msg);
      T_ASSERT(status == TRUE);
    }
  } 
  TutExit(ret_code);
} 

/* ========================================================================= */
/*..bmCbProcessReceiverExit -- callback for processing exit message*/
static void T_ENTRY bmCbProcessReceiverExit
(
T_IPC_CONN conn,
T_IPC_CONN_PROCESS_CB_DATA data,
T_CB_ARG arg
)
{
  T_ASSERT(conn != NULL);
  T_ASSERT(data != NULL);
  T_ASSERT(arg == NULL);

  TutOut("Received exit message from receiver.\n");
} 

/* ========================================================================= */
/*..bmCbProcessMonSubjectSubscribeStatus --  MON_SUBJECT_SUBSCRIBE_STATUS msg */
static void T_ENTRY bmCbProcessMonSubjectSubscribeStatus
(
T_IPC_CONN                  conn,
T_IPC_CONN_PROCESS_CB_DATA  data,
T_CB_ARG                    arg
)
{
  static T_BOOL bmCalledBefore = FALSE;
  T_BOOL        status;
  T_STR         subject_name;
  T_STR        *dummy;

  T_ASSERT(conn != NULL);
  T_ASSERT(data != NULL);
  T_ASSERT(arg == NULL);

  if (bmCalledBefore) {
    TutOut("\nBenchmark failure.\n");
    TutOut("A receiver has started or exited.\n");
    TutOut("This benchmark only works when all receivers have been\n");
    TutOut("started before this sender.\n");
    fflush(stdout);
    bmSafeExit(T_EXIT_FAILURE);
  }

  bmCalledBefore = TRUE;
  status = TipcMsgSetCurrent(data->msg, 0);
  T_ASSERT(status == TRUE);
  status = TipcMsgNextStr(data->msg, &subject_name);
  T_ASSERT(status == TRUE);
  status = TipcMsgNextStrArray(data->msg, &dummy, &bmNumReceivers);
  T_ASSERT(status == TRUE);
  if (bmNumReceivers != bmNumExpectedReceivers) {
    TutOut("WARNING! expected %d receivers, %d are running.\n",
           bmNumExpectedReceivers, 
           bmNumReceivers);
  }
} 

/* ========================================================================= */
/*..bmCbProcessAck -- get acknowledgement from receiver*/
static void T_ENTRY bmCbProcessAck(T_IPC_CONN conn,
                                   T_IPC_CONN_PROCESS_CB_DATA data,
                                   T_CB_ARG arg)
{
  T_ASSERT(conn != NULL);
  T_ASSERT(data != NULL);
  T_ASSERT(arg == NULL);
  ++bmReceiverIndex;
} 

/* ========================================================================= */
/*..bmExecuteSendTest -- run one test */
static void bmExecuteSendTest
(
T_INT4 num_msgs,
T_INT4 ack_rate,
T_INT4 iterations,
T_BOOL build_each_msg
)
{
  T_IPC_MSG full_msg;
  T_BOOL    status;
  T_IPC_MSG ack_msg;
  T_INT4    msg_count;
  T_IPC_MT  mt;
  T_INT4    mt_num;

  full_msg = TipcMsgCreate(TipcMtLookupByNum(T_BM_MT_GENERATED_USER_TYPE));
  T_ASSERT(full_msg != NULL);

  status = TipcMsgSetDest(full_msg, T_BM_SUBJECT_RECEIVE); 
  T_ASSERT(status == TRUE);

  if (getenv("BENCH_DELIVERY_MODE")) {
    T_STR               delivery_mode_str = (T_STR)getenv("BENCH_DELIVERY_MODE");
    T_IPC_DELIVERY_MODE delivery_mode     = T_IPC_DELIVERY_BEST_EFFORT;

    if (!TipcStrToDeliveryMode(delivery_mode_str, &delivery_mode)) {
      TutOut("Bad delivery mode %s.\n", delivery_mode_str);
    }
    status = TipcMsgSetDeliveryMode(full_msg, delivery_mode);
    T_ASSERT(status == TRUE);
  }

  /* 
   * Build the message once if we aren't going to build it each send. 
   */
  if (!build_each_msg) {
    status = TbmBuildGeneratedMsg(full_msg, iterations);
    T_ASSERT(status == TRUE);
  }

  /*
   * Send the data messages. 
   */
  for (msg_count = 0; msg_count < num_msgs; ++msg_count) {
#ifdef T_BENCH_DEBUG
    TutOut("Sending message #%d\n", msg_count);
#endif    
    /*
     * Build each message if so desired. 
     */
    if (build_each_msg) {
      status = TipcMsgSetNumFields(full_msg, 0);
      T_ASSERT(status == TRUE);
      status = TbmBuildGeneratedMsg(full_msg, iterations);
      T_ASSERT(status == TRUE);
    }
    status = TipcSrvMsgSend(full_msg, FALSE);
    T_ASSERT(status == TRUE);

    /*
     * ack rate specifies how often the receivers send an ack, we   
     * wait until we receive the ack, to provides a rough mechanism  
     * for flow control.                                            
     */
    if (msg_count % ack_rate == 0) {
      /*
       * Be certain write buffer is flushed.
       */
       status = TipcSrvFlush();
       T_ASSERT(status == TRUE);
   
      /*
       * receive acknowledgement from each receiver. 
       */
#ifdef T_BENCH_DEBUG
      TutOut("Waiting for ack(s).\n");
#endif
      bmReceiverIndex = 0;
      while (bmReceiverIndex < bmNumReceivers) {
        /*
         * Read from RTserver with a 30 second timeout. 
         */
        ack_msg = TipcSrvMsgNext(30.0);
        if (ack_msg == NULL) {
          TutOut("Timed out on receiving ACK from receiver #%d\n", 
                 bmReceiverIndex);
          bmSafeExit(T_EXIT_FAILURE);
        }

        status = TipcMsgGetType(ack_msg, &mt);
        T_ASSERT(status == TRUE);
        T_ASSERT(mt != NULL);
        status = TipcMtGetNum(mt, &mt_num);
        T_ASSERT(status == TRUE);
        if (mt_num != T_BM_MT_ACK) {
          TutOut("Received unexpected msg (type = %d) from receiver #\n", 
                 mt_num,
                 bmReceiverIndex);
          TipcMsgPrint(ack_msg, TutOut);
          bmSafeExit(T_EXIT_FAILURE);
        }
        status = TipcSrvMsgProcess(ack_msg);
        T_ASSERT(status == TRUE);
        status = TipcMsgDestroy(ack_msg);
        T_ASSERT(status == TRUE);
      }
    }
  } 
  TipcMsgDestroy(full_msg);
} 

/* ========================================================================= */
/*..bmStartReceivers -- start receiver programs */
static void bmStartReceivers
(
T_BOOL do_traverse,
T_INT4 ack_rate,
T_INT4 iter
)
{
  T_INT4    counter = 0;
  T_STRING  system_cmd;
  T_IPC_MSG ready_msg;
  T_IPC_MT  mt;
  T_INT4    mt_num;
  T_BOOL    status;
  T_INT4    sys_status;

  /*
   * We don't support automatically starting the receivers on a remote node.
   */

  /* 
   * Start all receivers.  Save their output for debugging purposes.
   * The output from the first receiver will be used for timing data.
   */
  for (counter = 0; counter < bmNumExpectedReceivers; ++counter) {
    TutOut("Starting receiver #%d.\n", counter);
#if defined(T_OS_VMS)
    sprintf(system_cmd, 
            "receive %d %d %d %s%d > recv%02d.out &",
            ack_rate, 
            iter, 
            do_traverse,  
            T_BM_UD_RECEIVE, 
            counter, 
            counter);
#elif defined(T_OS_UNIX)
    sprintf(system_cmd, 
            "./receive.x %d %d %d %s%d > recv%02d.out &",
            ack_rate, 
            iter, 
            do_traverse, 
            T_BM_UD_RECEIVE, 
            counter, 
            counter);
#elif defined(T_OS_WINDOWS)
    sprintf(system_cmd, 
            "start /B receive.exe %d %d %d %s%d > recv%02d.out",
            ack_rate, 
            iter, 
            do_traverse, 
            T_BM_UD_RECEIVE, 
            counter, 
            counter);
#else
    unknown OS;
#endif

    if (getenv("BENCH_RUN_MANUALLY")) {
      TutOut("Please start <%s> now.\n", system_cmd);
    }
    else {
#if defined(T_OS_WINDOWS)
      sys_status = system(system_cmd);
#else
      sys_status = TutSystem(system_cmd);
#endif
      if (sys_status != 0) {
        TutOut("TutSystem returned %d\n", sys_status);
      }
    } 

    /*
     * Wait for the receiver to start and send us a message.
     */
    TutOut("Waiting to receive ready message from receiver #%d.\n", counter);
    ready_msg = TipcSrvMsgNext(30.0);
    if (ready_msg == NULL) {
      TutOut("Timed out on receiving ready message from receiver #%d!\n", counter);
      bmSafeExit(T_EXIT_FAILURE);
    }

    status = TipcMsgGetType(ready_msg, &mt);
    T_ASSERT(status == TRUE);
    T_ASSERT(mt != NULL);
    status = TipcMtGetNum(mt, &mt_num);
    T_ASSERT(status == TRUE);

    if (mt_num == T_BM_MT_RECEIVER_READY) {
      TutOut("Receiver #%d is ready.\n", counter);
    }
    else {
      TutOut("Received unexpected message from receiver #%d!\n", counter);
      bmSafeExit(T_EXIT_FAILURE);
    }
    status = TipcMsgDestroy(ready_msg);
    T_ASSERT(status == TRUE);
  }
} 

/* ========================================================================= */
/*..main() */
int main
(
int    argc,
char **argv
)
{
  T_OPTION  option;
  T_BOOL    status;
  T_INT4    num_msgs;
  T_INT4    ack_rate;
  T_INT4    iterations;
  T_BOOL    build_each_message;
  T_BOOL    do_traverse_on_receive;
  T_IPC_MSG msg;
  T_IPC_MT  mt;
  T_INT4    mt_num;

#ifndef T_RTWORKS35
  if (getenv("BENCH_INIT_THREADS")) {
    TipcInitThreads();
  }
#endif

#if defined(T_OS_VMS)
  TutVmsRedirect((T_INT4 *)&argc, &argv);
#endif

  /*
   * Check arguments. 
   */
  if (argc != 7) {
    TutOut("%s expects 6 arguments.\n", argv[0]);
    TutOut("\nUsage: %s <# messages> <# messages/ack>\n",argv[0]);
    TutOut("  <# grammar iterations/message> <# receivers>\n");
    TutOut("  <boolean int build/nobuild each message>\n");
    TutOut("  <boolean int traverse/notraverse each message>\n");
    TutExit(T_EXIT_FAILURE);
  }

  num_msgs                  = atoi(argv[1]);
  ack_rate                  = atoi(argv[2]);
  iterations                = atoi(argv[3]);
  bmNumExpectedReceivers    = atoi(argv[4]);
  build_each_message        = atoi(argv[5]);
  do_traverse_on_receive    = atoi(argv[6]);

  TutOut("Expecting %d receivers.\n", bmNumExpectedReceivers);
  TutCommandParseStr("setopt server_auto_flush_size 1048576");
  TutCommandParseStr("setopt _conn_read_size_init 1024");
  TutCommandParseStr("unsetopt trace_flags");
  TutCommandParseStr("unsetopt catalog_flags");

  if (
     1 > num_msgs 
     ||
     1 > ack_rate 
     ||
     1 > iterations
     ) {
    TutOut("%s: invalid arguments \"%s %s %s %s\"\n",
           argv[0], 
           argv[1], 
           argv[2], 
           argv[3], 
           argv[4]);
    bmSafeExit(T_EXIT_FAILURE);
  }

  option = TutOptionLookup("project");
  T_ASSERT(option != NULL);
  status = TutOptionSetEnum(option, T_BM_PROJECT);
  T_ASSERT(status == TRUE);

  /*
   * Allow memory-only GMD.
   */
  if (getenv("BENCH_MEMORY_GMD") == NULL) {
    /*
     * Set unique_subject in case guaranteed message delivery is used
     */
    option = TutOptionLookup("unique_subject");
    T_ASSERT(option != NULL);
    status = TutOptionSetEnum(option, T_BM_UD_SEND);
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
  if (TutFileExists("send.cm")) {
    TutCommandParseStr("source send.cm");
  }
  T_BM_MT_INIT();
  status = TbmCreateGeneratedMt();
  T_ASSERT(status == TRUE);

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
   * Note: *must* start receiving this subject before waiting for receivers! 
   */
  status = TipcSrvSubjectSetSubscribe(T_BM_SUBJECT_SEND, TRUE);
  T_ASSERT(status == TRUE);
  TutOut("Started receiving %s subject.\n", T_BM_SUBJECT_SEND);
  status = TipcSrvFlush();
  T_ASSERT(status == TRUE);

  /*
   * Get receivers up and going 
   */
  bmStartReceivers(do_traverse_on_receive, ack_rate, iterations);

  /*
   * Create control message callbacks
   */
  mt = TipcMtLookupByNum(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS);
  T_ASSERT(T_NULL != mt);
  if (TipcSrvProcessCbCreate(mt, 
                             bmCbProcessMonSubjectSubscribeStatus, 
                             NULL) == NULL) {
    TutOut("Could not create T_MT_MON_SUBJECT_SUBSCRIBE_STATUS callback.\n");
    bmSafeExit(T_EXIT_FAILURE);
  }
  mt = TipcMtLookupByNum(T_BM_MT_RECEIVER_EXIT);
  T_ASSERT(T_NULL != mt);
  if (TipcSrvProcessCbCreate(mt,
                             bmCbProcessReceiverExit,
                             NULL) == NULL) {
    TutOut("Could not create T_BM_MT_RECEIVER_EXIT callback.\n");
    bmSafeExit(T_EXIT_FAILURE);
  }
  mt = TipcMtLookupByNum(T_BM_MT_ACK);
  T_ASSERT(T_NULL != mt);
  if (TipcSrvProcessCbCreate(mt, bmCbProcessAck, NULL) == NULL) {
    TutOut("Could not create T_BM_MT_ACK callback.\n");
    bmSafeExit(T_EXIT_FAILURE);
  }
  status = TipcMonSubjectSubscribeSetWatch(T_BM_SUBJECT_RECEIVE, TRUE);
  T_ASSERT(status == TRUE);

  /* 
   * Process watch status message 
   */
  msg = TipcSrvMsgNext(60.0);
  if (msg == NULL) {
    TutOut("Timed out on receiving SUBJECT_RECEIVE_STATUS message.\n");
    bmSafeExit(T_EXIT_FAILURE);
  }
  T_ASSERT(msg != NULL);
  status = TipcMsgGetType(msg, &mt);
  T_ASSERT(status == TRUE);
  T_ASSERT(mt != NULL);
  status = TipcMtGetNum(mt, &mt_num);
  T_ASSERT(status == TRUE);

  if (mt_num != T_MT_MON_SUBJECT_SUBSCRIBE_STATUS) {
    TutOut("Wrong message (type = %d) received. Expecting SUBJECT_RECEIVE_STATUS\n", mt_num);
    bmSafeExit(T_EXIT_FAILURE);
  }

  status = TipcSrvMsgProcess(msg);
  T_ASSERT(status == TRUE);
  status = TipcMsgDestroy(msg);
  T_ASSERT(status == TRUE);

  if (bmNumReceivers < 1) {
    TutOut("No processes are running!\n");
    TutExit(T_EXIT_FAILURE);
  }

  TutCommandParseStr("stats");  /* begin timing */

  /* 
   * time server 
   */
  status = TipcSrvMsgWrite(T_IPC_SUBJECT_SERVER,
                           TipcMtLookupByNum(T_MT_CONTROL),
                           FALSE,
                           T_IPC_FT_STR, "stats",
                           NULL);
  T_ASSERT(status == TRUE);

  bmExecuteSendTest(num_msgs, 
                    ack_rate, 
                    iterations,
                    build_each_message);

  TutCommandParseStr("stats");  /* end timing */

  status = TipcSrvMsgWrite(T_IPC_SUBJECT_SERVER,
                           TipcMtLookupByNum(T_MT_CONTROL),
                           FALSE,
                           T_IPC_FT_STR, "stats",
                           NULL);

  T_ASSERT(status == TRUE);
  status = TipcMonSubjectSubscribeSetWatch(T_BM_SUBJECT_RECEIVE, FALSE);
  T_ASSERT(status == TRUE);
  bmSafeExit(T_EXIT_SUCCESS);
  return 0;          /* needed for picky compilers */
} 
