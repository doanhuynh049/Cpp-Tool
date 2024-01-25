/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/bench/server/srvbench.c#3 $
 */

#include <rtworks/ipc.h>
#include "srvbench.h"

/* ========================================================================= */
/*..bmSafeExit -- shutdown RTserver, and exit*/
static void bmSafeExit
(
T_INT4 ret_code
)
{
#if defined(T_OS_VMS)
  TutSystem("stopsrv");
#elif defined(T_OS_UNIX)
  TutSystem("./stopsrv");
#elif defined(T_OS_WINDOWS)
  TutSystem("rtserver -stop_clients > stopsrv.out");
#endif
  TutExit(ret_code);
}

/* ========================================================================= */
/* bmPerformBench -- run one benchmark test */
static void bmPerformBench
(
FILE *input_file,
FILE *output_file
)
{
  T_STRING  input_line;
  T_INT4    counter;
  T_INT4    num_msgs;
  T_INT4    msgs_ack;
  T_INT4    num_receivers;
  T_INT4    num_threads;
  T_INT4    do_traverse_on_receive;
  T_INT4    do_build_on_send;
  T_INT4    iterations;
  T_REAL8   receive_secs;
  T_REAL8   send_secs;
  T_REAL8   server_secs;
  FILE     *time_file;
  T_CHAR    system_cmd[1024];
  T_REAL8   start_wall_time;

  /* As of 6.7, T_INT8 should always be a real numeric type */
  T_INT8    total_bytes = 0;
  T_INT8    num_send_msgs_per_sec    = 0;
  T_INT8    num_send_bytes_per_sec   = 0;
  T_INT8    num_recv_msgs_per_sec    = 0;
  T_INT8    num_recv_bytes_per_sec   = 0;
  T_INT8    num_server_msgs_per_sec  = 0;
  T_INT8    num_server_bytes_per_sec = 0;
  T_INT8    total_msg_size           = 0;

#if defined(T_OS_UNIX)
  T_STRING  num_threads_env;
#endif

  counter = 0;
  while (fgets(input_line, sizeof(input_line), input_file) != NULL) {
    /*
     * Check for comments (first char of line is #) and blank lines
     */
    if (input_line[0] == '#' || input_line[0] == '\n') {
      continue;
    }
    /*
     * Easy way to quit mid-file.
     */
    if (T_STREQI(input_line, "quit\n")) {
      break;
    }

    /*
     * Increment run counter
     */
    counter++;
    TutOut("-------------------------************-------------------------\n");

    /*
     * Read run parameters
     */
    if (sscanf(input_line,
               "%d %d %d %d %d %d %d",
               &num_msgs,
               &msgs_ack,
               &iterations,
               &num_receivers,
               &do_build_on_send,
               &do_traverse_on_receive,
               &num_threads) != 7) {
      TutOut("Ignoring bad input line <%s>.\n", input_line);
      continue;
    }

    /*
     * Start one RTserver, possibly on a remote node.
     *
     * Note : This program does not support automatically using multiple RTservers
     */
    TutOut("Starting RTserver.\n");

    /*
     * Set the environment variable for the number of threads
     */
#if defined(T_OS_VMS)
    sprintf(system_cmd, "startsrv &");
    TutSystem(system_cmd);
#elif defined(T_OS_UNIX)
    sprintf(num_threads_env, "BENCH_NUM_SERVER_THREADS=%d", num_threads);
    putenv(num_threads_env);
    sprintf(system_cmd,"./startsrv &");
    TutSystem(system_cmd);
#elif defined(T_OS_WINDOWS)
    if (getenv("BENCH_SERVER_NODE") != NULL) {
      sprintf(system_cmd,
              "echo Note: expecting RTserver to already be running on %s.",
              getenv("BENCH_SERVER_NODE"));
    }
    else if (getenv("BENCH_SERVER_64BIT") != NULL) {
      sprintf(system_cmd, "rtserver64 -threads %d > server.out", num_threads);
    }
    else {
      sprintf(system_cmd, "rtserver -threads %d > server.out", num_threads);
    }
    TutSystem(system_cmd);
#else
    unknown OS;
#endif

    /*
     * Let RTserver initialize before bringing up the sender
     */
    TutSleep(5.0);

    /*
     * Allow a scale factor for the number of message
     */
    if (getenv("BENCH_MSG_SCALE_FACTOR") != NULL) {
      num_msgs = (T_INT4)((T_REAL8)num_msgs * atof(getenv("BENCH_MSG_SCALE_FACTOR")));
    }
    else if (getenv("BENCH_SERVER_NODE") != NULL) {
      /*
       * If we are using a remote RTserver, go ahead and scale down the
       * number of msgs automatically.  SMW 11/19/94
       */
      num_msgs = (T_INT4)((T_REAL8)num_msgs * 0.4);
    }

    /*
     * Print bench header info
     */
    TutOut("Starting test:  %4d  %4d  %4d  %5d  %8s  %8s  %4d.\n",
           num_msgs,
           msgs_ack,
           iterations,
           num_receivers,
           do_build_on_send ? "true" : "false",
           do_traverse_on_receive ? "true" : "false",
           num_threads);
    fflush(stdout);
    fprintf(output_file,
            "%10d\t%5d %4d %4d %4d %4d    %4d",
            num_msgs,
            msgs_ack,
            iterations,
            num_receivers,
            do_build_on_send ? 1 : 0,
            do_traverse_on_receive ? 1 : 0,
            num_threads);
    fflush(output_file);

    /*
     * Start sender process
     */
#if defined(T_OS_VMS)
    sprintf(system_cmd,
            "send %d %d %d %d %d %d > send.out",
            num_msgs,
            msgs_ack,
            iterations,
            num_receivers,
            do_build_on_send,
            do_traverse_on_receive);
#elif defined(T_OS_UNIX)
    sprintf(system_cmd,
            "./send.x %d %d %d %d %d %d > send.out",
            num_msgs,
            msgs_ack,
            iterations,
            num_receivers,
            do_build_on_send,
            do_traverse_on_receive);
#elif defined(T_OS_WINDOWS)
    sprintf(system_cmd,
            "send.exe %d %d %d %d %d %d > send.out",
            num_msgs,
            msgs_ack,
            iterations,
            num_receivers,
            do_build_on_send,
            do_traverse_on_receive);
#else
    unknown OS;
#endif

    TutOut("Starting publisher.\n");
    start_wall_time = TutGetWallTime();
    TutSystem(system_cmd);
    TutOut("Publisher done.\n");
    TutOut("That took %d seconds of wall clock time.\n",
           (int)(TutGetWallTime() - start_wall_time));
    fflush(stdout);

    TutOut("Stopping RTserver.\n");
#if defined(T_OS_VMS)
    TutSystem("stopsrv");
#elif defined(T_OS_UNIX)
    TutSystem("./stopsrv");
#elif defined(T_OS_WINDOWS)
    if (getenv("BENCH_SERVER_NODE") != NULL) {
      sprintf(system_cmd,
              "echo Note: Remote RTserver on %s will not be stopped",
              getenv("BENCH_SERVER_NODE"));
    }
    else {
      sprintf(system_cmd, "rtserver -stop_clients > stopsrv.out");
    }
    TutSystem(system_cmd);
    TutSleep(5.0);  /* Windows needs extra time */
#endif
    TutSleep(1.0);

#ifndef T_OS_WINDOWS
    /*
     * Give remote RTserver more time for NFS latency.
     */
    if (getenv("BENCH_SERVER_NODE") != NULL) {
      TutSleep(5.0);
    }
#endif

    /*
     * Parse send.out, recv00.out & server.out for results
     */
    TutOut("Preparing '.tim' files (from output files).\n");
#ifdef T_OS_VMS
    TutSystem("search send.out \"CPU time,\" /output=send.tim");
    TutSystem("search recv00.out \"CPU time,\" /output=receive.tim");
    TutSystem("search server.out \"CPU time,\" /output=server.tim");
#elif defined(T_OS_UNIX)
    TutSystem("fgrep 'CPU time,' send.out > send.tim");
    TutSystem("fgrep 'CPU time,' recv00.out > receive.tim");
    TutSystem("fgrep 'CPU time,' server.out > server.tim");
#elif defined(T_OS_WINDOWS)
    /*
    * On Windows, use the NT find.exe located in the SYSTEM32 directory
    * You can also substitute a third party grep utility for the find
    * command as shown in the T_OS_UNIX block above.
    */
    TutSystem("%SYSTEMROOT%\\SYSTEM32\\FIND.EXE \"CPU time,\" < send.out > send.tim");
    TutSystem("%SYSTEMROOT%\\SYSTEM32\\FIND.EXE \"CPU time,\" < recv00.out > receive.tim");
    if (getenv("BENCH_SERVER_NODE") == NULL) {
      TutSystem("%SYSTEMROOT%\\SYSTEM32\\FIND.EXE \"CPU time,\" < server.out > server.tim");
    }
#else
    unknown OS;
#endif

    /*
     * Read send time
     */
    TutOut("Parsing send.tim.\n");
    time_file = fopen("send.tim", "r");
    if (!time_file) {
      TutOut("Could not open send.tim for reading.");
      bmSafeExit(T_EXIT_FAILURE);
    }

    if (getenv("BENCH_MEASURE_CPU_TIME") != NULL) {
      if (fscanf(time_file, "%*s %*s %lf", &send_secs) != 1) {
        TutOut("Could not read CPU time from send.tim.\n");
        bmSafeExit(T_EXIT_FAILURE);
      }
    }
    else {
      if (fscanf(time_file, "%*s %*s %*s %*s %*s %*s %lf", &send_secs) != 1) {
        TutOut("Could not read wall time from send.tim.\n");
        bmSafeExit(T_EXIT_FAILURE);
      }
    }
    fclose(time_file);
    TutOut("Successfully parsed send.tim.\n");

    /*
     * Read receive time
     */
    TutOut("Parsing receive.tim.\n");
    time_file = fopen("receive.tim", "r");
    if (!time_file) {
      TutOut("Could not open receive.tim for reading.");
      bmSafeExit(T_EXIT_FAILURE);
    }

    if (getenv("BENCH_MEASURE_CPU_TIME") != NULL) {
      if (fscanf(time_file, "%*s %*s %lf", &receive_secs) != 1) {
        printf("Could not read CPU time from receive.tim.\n");
        bmSafeExit(T_EXIT_FAILURE);
      }
    }
    else {
      if (fscanf(time_file, "%*s %*s %*s %*s %*s %*s %lf", &receive_secs) != 1) {
        TutOut("Could not read wall time from receive.tim.\n");
        bmSafeExit(T_EXIT_FAILURE);
      }
    }
    fclose(time_file);
    TutOut("Successfully parsed receive.tim.\n");

    /*
     * Read server time
     */
    TutOut("Parsing server.tim.\n");
#ifdef T_OS_WINDOWS
    if (getenv("BENCH_SERVER_NODE") == NULL) {
      time_file = fopen("server.tim", "r");
      if (!time_file) {
        TutOut("Could not open server.tim for reading.");
        bmSafeExit(T_EXIT_FAILURE);
      }
      if (getenv("BENCH_MEASURE_CPU_TIME") != NULL) {
        if (fscanf(time_file, "%*s %*s %lf", &server_secs) != 1) {
          TutOut("Could not read CPU time from server.tim.\n");
          bmSafeExit(T_EXIT_FAILURE);
        }
      }
      else {
        if (fscanf(time_file, "%*s %*s %*s %*s %*s %*s %lf", &server_secs) != 1) {
          TutOut("Could not read wall time from server.tim.\n");
          bmSafeExit(T_EXIT_FAILURE);
        }
      }
      fclose(time_file);
    }
#else
    /*
     * No conditionals needed for non-windows platforms
     */
    time_file = fopen("server.tim", "r");
    if (!time_file) {
      TutOut("Could not open server.tim for reading.");
      bmSafeExit(T_EXIT_FAILURE);
    }
    if (getenv("BENCH_MEASURE_CPU_TIME") != NULL) {
      if (fscanf(time_file, "%*s %*s %lf", &server_secs) != 1) {
        printf("Could not read CPU time from server.tim.\n");
        bmSafeExit(T_EXIT_FAILURE);
      }
    }
    else { /* use wall clock time (more realistic).  SMW 11/19/94 */
      if (fscanf(time_file, "%*s %*s %*s %*s %*s %*s %lf", &server_secs) != 1) {
        printf("Could not read wall time from server.tim.\n");
        bmSafeExit(T_EXIT_FAILURE);
      }
    }
    fclose(time_file);
#endif
    TutOut("Successfully parsed server.tim.\n");

    /*
     * Make sure we don't divide by zero
     */
    if (send_secs == 0) {
      send_secs = .01;
    }
    if (receive_secs == 0) {
      receive_secs = .01;
    }
    if (server_secs == 0) {
      server_secs = .01;
    }

    total_msg_size = (T_INT8)(iterations * TbmGeneratedMsgDataSize());
    total_bytes = (T_INT8)(num_msgs * total_msg_size);
    num_send_msgs_per_sec = (T_INT8)(num_msgs/send_secs);
    num_send_bytes_per_sec = (T_INT8)(total_bytes/send_secs);
    num_recv_msgs_per_sec  = (T_INT8)(num_msgs/receive_secs);
    num_recv_bytes_per_sec = (T_INT8)(total_bytes/receive_secs);
    num_server_msgs_per_sec = (T_INT8)(num_msgs/server_secs);
    num_server_bytes_per_sec = (T_INT8)(total_bytes/server_secs);

#ifdef T_OS_WINDOWS
    if (getenv("BENCH_SERVER_NODE") != NULL) {
      fprintf(output_file, " "
              " "T_INT8_FSPEC("6")" "T_INT8_FSPEC("12")
              " "T_INT8_FSPEC("6")" "T_INT8_FSPEC("12")
              " %6s %12s\n",
              num_msgs/send_secs,
              total_bytes/send_secs,
              num_msgs/receive_secs,
              total_bytes/receive_secs,
              "N/A",
              "N/A");
    }
    else {
      fprintf(output_file, " "
              " "T_INT8_FSPEC("6")" "T_INT8_FSPEC("12")
              " "T_INT8_FSPEC("6")" "T_INT8_FSPEC("12")
              " "T_INT8_FSPEC("6")" "T_INT8_FSPEC("12")"\n",
              num_send_msgs_per_sec,
              num_send_bytes_per_sec,
              num_recv_msgs_per_sec,
              num_recv_bytes_per_sec,
              num_server_msgs_per_sec,
              num_server_bytes_per_sec);
    }
#else
      fprintf(output_file, " "
              " "T_INT8_FSPEC("6")" "T_INT8_FSPEC("12")
              " "T_INT8_FSPEC("6")" "T_INT8_FSPEC("12")
              " "T_INT8_FSPEC("6")" "T_INT8_FSPEC("12")"\n",
              num_send_msgs_per_sec,
              num_send_bytes_per_sec,
              num_recv_msgs_per_sec,
              num_recv_bytes_per_sec,
              num_server_msgs_per_sec,
              num_server_bytes_per_sec);
#endif
    fflush(output_file);
  }
}

/* ========================================================================= */
/*..main -- main program */
int main
(
int    argc,
char **argv
)
{
  char  *bench_in;
  char  *bench_out;
  FILE  *input_file;
  FILE  *output_file;
  T_CHAR system_cmd[1024];

#if defined(T_OS_VMS)
  TutVmsRedirect((T_INT4 *)&argc, &argv);
#endif
  if (argc != 3) {
    TutOut("Usage: %s <input file> <output file>\n", argv[0]);
    TutOut("Example: %s bench.in bench.out\n", argv[0]);
    TutExit(T_EXIT_FAILURE);
  }

  /*
   * set parameters
   */
  bench_in    = argv[1];
  bench_out   = argv[2];
  input_file  = fopen(bench_in, "r");
  if (input_file == NULL) {
    TutOut("%s: unable to open input file %s for reading.\n", argv[0], bench_in);
    TutExit(T_EXIT_FAILURE);
  }
  output_file = fopen(bench_out, "w");
  if (output_file == NULL) {
    TutOut("%s: unable to open output file %s for writing.\n", argv[0], bench_out);
  }

  /*
   * Stop the server if running
   */
#if defined(T_OS_VMS)
  TutSystem("stopsrv");
#elif defined(T_OS_UNIX)
  TutSystem("./stopsrv");
#elif defined(T_OS_WINDOWS)
  if (getenv("BENCH_SERVER_NODE") != NULL) {
    sprintf(system_cmd,
            "echo Note: Will not be stopping RTserver during remote NT tests",
            getenv("BENCH_SERVER_NODE"));
  }
  else {
    sprintf(system_cmd, "rtserver -stop_clients > stopsrv.out");
  }
  TutSystem(system_cmd);
  TutSleep(5.0);  /* Windows needs extra time */
#endif

  /*
   * Sleep for some time waiting for the server to stop
   */
  TutSleep(1.0);

  /*
   * Output header.
   */
  fprintf(output_file, "Total             Msgs Num  Num   Bld  Trav   Num          Send              Receive              Server      \n");
  fprintf(output_file, "msgs              /ack gram recv   on   on   threads  msgs/    bytes/    msgs/     bytes/     msgs/     bytes/\n");
  fprintf(output_file, "sent                   iter procs send recv           sec       sec      sec        sec       sec        sec  \n");
  fprintf(output_file, "--------------------------------------------------------------------------------------------------------------\n");
  fflush(output_file);

  /*
   * Call main test program
   */
  bmPerformBench(input_file, output_file);
  TutExit(T_EXIT_SUCCESS);
  return 0;            /* for picky compilers */
}


