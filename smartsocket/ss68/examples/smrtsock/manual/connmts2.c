/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/connmts2.c#1 $
 */

/* From "Connections" chapter in the "SmartSockets User's Guide". */

/* connmts1.c -- multithreaded connections example server 1 */

/*
 * This program uses multiple server threads in order to allow multiple 
 * clients to connect and submit messages simultaneously.
 */

#include <rtworks/ipc.h>

#define SERVER_COUNT 2
#define HELPER_COUNT 2

T_IPC_CONN server_conn;
T_TSD_KEY id_key = T_INVALID_TSD_KEY;

/* ================================================================ */
/*..cb_process_numeric_data -- process callback for NUMERIC_DATA */
void T_ENTRY cb_process_numeric_data
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_STR id_str;
  T_INT4 i;
  T_STR var_name[3];
  T_REAL8 var_value[3];
  T_REAL8 in_time;
  T_REAL8 out_time;
  T_STR the_time = NULL;
  
  if (!TutTsdGetValue(id_key, (T_PTR)&id_str)) {
    TutOut("Could not get TSD value for thread: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* set current field to first field in message */
  if (!TipcMsgSetCurrent(data->msg, 0)) {
    TutOut("%s: Could not set current field of msg: error <%s>.\n",
	   id_str, TutErrStrGet());
    TutThreadExit(NULL);
  }
  
  for (i = 0; i < 3; i++) {
    if (!TipcMsgNextStrReal8(data->msg,
                             &var_name[i], &var_value[i])) {
      TutOut("%s: Could not parse NUMERIC_DATA msg: error <%s>.\n",
	     id_str, TutErrStrGet());
      TutThreadExit(NULL);
    }
  }
  
  in_time = TutGetWallTime();
  the_time = TutStrDup(TutTimeNumToStr(in_time));
  
  TutOut("%s: (%s %d %s %s %s %s) at %s\n", id_str,
         var_name[0], (T_INT4)var_value[0],
         var_name[1], TutRealToStr(var_value[1]),
         var_name[2], TutTimeNumToStr(var_value[2]),
         the_time);
  
  TutFree(the_time);
  TutSleep(var_value[1]);
  
  out_time = TutGetWallTime();
  
  if (!TipcMsgWrite(data->msg,
                    T_IPC_FT_STR, "in-time",
                    T_IPC_FT_REAL8, in_time,
                    T_IPC_FT_STR, "out-time",
                    T_IPC_FT_REAL8, out_time,
                    NULL)) {
    TutOut("%s: Could not append to NUMERIC_DATA msg: error <%s>.\n",
           id_str, TutErrStrGet());
    TutThreadExit(NULL);
  }
  
  if (!TipcConnMsgSend(conn, data->msg)) {
    TutOut("%s: Could not send NUMERIC_DATA msg: error <%s>.\n",
	   id_str, TutErrStrGet());
    TutThreadExit(NULL);
  }
  
  if (!TipcConnFlush(conn)) {
    TutOut("%s: Could not flush conn to client: error <%s>.\n",
           id_str, TutErrStrGet());
    TutThreadExit(NULL);
  }
  
} /* cb_process_numeric_data */

/* ================================================================ */
/*..helper_thread -- thread function for helper threads */
T_PTR T_ENTRY helper_thread
(
 T_PTR arg
)
{
  T_PTR *argv = (T_PTR *)arg;
  T_STR id_str = argv[0];
  T_IPC_CONN client_conn = argv[1];
  
  TutFree(argv);
  
  if (!TutTsdSetValue(id_key, id_str)) {
    TutOut("Could not set TSD value for thread: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  if (!TipcConnMainLoop(client_conn, T_TIMEOUT_FOREVER)) {
    /* make sure we reached the end of the data */
    if (TutErrNumGet() != T_ERR_EOF) {
      TutOut("%s: Did not reach end of data: error <%s>.\n",
      id_str, TutErrStrGet());
      TutExit(T_EXIT_FAILURE);
    }
  }
  
  return NULL;
} /* helper_thread */

/* ================================================================ */
/*..server_thread -- thread function for server threads */
T_PTR T_ENTRY server_thread
(
 T_PTR arg
)
{
  T_STRING id_str;
  T_IPC_MT mt;
  T_IPC_CONN client_conn;
  T_INT4 i;
  T_INT4 j;
  T_THREAD thread[HELPER_COUNT];
  T_STRING id_strs[HELPER_COUNT];
  
  sprintf(id_str, "%d", *(T_INT4 *)arg);
  if (!TutTsdSetValue(id_key, id_str)) {
    TutOut("Could not set TSD value for thread: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
  if (NULL == mt) {
    TutOut("%s: Could not look up NUMERIC_DATA mt: error <%s>.\n",
           id_str, TutErrStrGet());
    TutThreadExit(NULL);
  }
  
  for (i = 0; i < 2; i++) {
    client_conn = TipcConnAccept(server_conn);
    if (NULL == client_conn) {
      TutOut("%s: Could not accept client: error <%s>.\n",
	     id_str, TutErrStrGet());
      TutThreadExit(NULL);
    }
    
    TutOut("%s: Accepted client connection %d.\n", id_str, i + 1);
    if (TipcConnProcessCbCreate(client_conn, mt,
				cb_process_numeric_data,
				arg) == NULL) {
      TutOut("%s: Could not create NUMERIC_DATA process cb: error <%s>.\n",
             id_str, TutErrStrGet());
      TutThreadExit(NULL);
    }

    for (j = 0; j < HELPER_COUNT; j++) {
      T_PTR *argv = (T_PTR *)TutMalloc(sizeof(T_PTR) * 2);
      
      sprintf(id_strs[j], "%d%c", *(T_INT4 *)arg, 'a' + j);
      argv[0] = id_strs[j];
      argv[1] = client_conn;
      thread[j] = TutThreadCreate((T_THREAD_FUNC)helper_thread, 
                                  argv, NULL);
      if (TutThreadEqual(T_INVALID_THREAD, thread[j])) {
        TutOut("Could not create thread %s: error <%s>.\n",
	       id_strs[j], TutErrStrGet());
        TutExit(T_EXIT_FAILURE);
      }
    }
    
    if (!TipcConnMainLoop(client_conn, T_TIMEOUT_FOREVER)) {
      /* make sure we reached the end of the data */
      if (TutErrNumGet() != T_ERR_EOF) {
	TutOut("%s: Did not reach end of data: error <%s>.\n",
	       id_str, TutErrStrGet());
      }
    }

    for (j = 0; j < HELPER_COUNT; j++) {
      if (!TutThreadWait(thread[j], NULL)) {
	TutOut("Could not wait for thread %s: error <%s>.\n",
	       id_strs[j], TutErrStrGet());
	TutExit(T_EXIT_FAILURE);
      }
    }

    TutOut("%s: Destroying client connection %d.\n", id_str, i + 1);
    if (!TipcConnDestroy(client_conn)) {
      TutOut("%s: Could not destroy client conn: error <%s>.\n",
	     id_str, TutErrStrGet());
    }
  }
  
  return NULL;
} /* server_thread */

/* ================================================================ */
/*..main -- main program */
int main()
{
  T_THREAD thread[SERVER_COUNT];
  T_INT4 id_args[SERVER_COUNT];
  T_INT4 i;
  
  if (!TipcInitThreads()) {
    TutOut("This platform does not support threads: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  if (!TutTsdKeyCreate(&id_key, 0)) {
    TutOut("Could not create TSD key: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Creating server connection.\n");
  server_conn = TipcConnCreateServer("tcp:_node:4000");
  if (NULL == server_conn) {
    TutOut("Could not create server connection: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  for (i = 0; i < SERVER_COUNT; i++) {
    id_args[i] = i + 1;
    thread[i] = TutThreadCreate((T_THREAD_FUNC)server_thread, 
                                &id_args[i], NULL);
    if (TutThreadEqual(T_INVALID_THREAD, thread[i])) {
      TutOut("Could not create thread %d: error <%s>.\n",
	     i, TutErrStrGet());
      TutExit(T_EXIT_FAILURE);
    }
  }
  
  for (i = 0; i < SERVER_COUNT; i++) {
    if (!TutThreadWait(thread[i], NULL)) {
      TutOut("Could not wait for thread %d: error <%s>.\n", 
	     i, TutErrStrGet());
      TutExit(T_EXIT_FAILURE);
    }
  }
  
  TutOut("Destroying server connection.\n");
  if (!TipcConnDestroy(server_conn)) {
    TutOut("Could not destroy server connection: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  return T_EXIT_SUCCESS;
} /* main */
