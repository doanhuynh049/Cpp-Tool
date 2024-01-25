/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/waitsbj.c#1 $
 */

/* =================================================================== */
/*                                                                     */
/*             waitsbj.c -- wait for subject recv count               */
/*                                                                     */
/* =================================================================== */

#include <rtworks/ipc.h>

T_CHAR project_name[32];
T_CHAR subject_name[32];

/* ------------------------------------------------------------------ */
static void T_ENTRY DefaultReceiveFunc
(
 T_IPC_CONN conn,
 T_IPC_CONN_MSG_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg; 
  T_IPC_MT mt;
  T_STR dest;
  T_STR type_name;

  if (!TipcMsgGetDest(msg, &dest)) {
    TutWarning("Could not get message destination\n");
    return;
  }

  if (!TipcMsgGetType(msg, &mt)) {
    TutWarning("Could not get msg type\n");
    return;
  }

  if (!TipcMtGetName(mt, &type_name)) {
    TutWarning("Could not get type name\n");
    return;
  }

  TutWarning("Received unwanted message type %s, destination is %s.\n",
	     type_name, dest);

} /* DefaultReceiveFunc */

/* ------------------------------------------------------------------ */
static void T_ENTRY ReceiveMonSubjectSubscribeStatus
(
 T_IPC_CONN conn,
 T_IPC_CONN_MSG_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg; 
  T_STR subject_id;
  T_STR *client_names;
  T_INT4 subject_count;
  static T_BOOL wdFuncCalled;

  if (!TipcMsgNextStr(msg, &subject_id)) {
    TutWarning("Could not read first field of msg: error = %d.\n",
	       (int)TutErrNumGet());
    return; 
  }/* if */

  if (!TipcMsgNextStrArray(msg, &client_names, &subject_count)) {
    TutWarning("Could not read second field of msg: error = %d.\n",
	       (int)TutErrNumGet());
    return; 
  }/* if */

  if (wdFuncCalled) {
    TutOut("New count for subject %s is %d.\n",
	   subject_id, (int)subject_count);
  }
  else {
    TutOut("Initial count of subject %s is %d\n",
	   subject_id, (int)subject_count);
    wdFuncCalled = TRUE;
  }

  if (subject_count == (T_INT4)arg) {
    TutOut("Waiting for %s is done at count %d.\n",
	   subject_id, (int)subject_count);
    T_C_EXIT(T_EXIT_SUCCESS);
  }

} /* ReceiveMonSubjectSubscribeStatus */

/* ========================================================================= */
/*..main -- main program */
int main
(
 int argc,
 char **argv
)
{
  T_CHAR option[80];
  T_IPC_MT mt; 
  T_CB cb; 
  T_BOOL status; 
  T_INT4 count;
  T_REAL8 timeout;
  T_IPC_MSG msg;
  T_STR *subject_names;
  T_INT4 num_subject_names;
  T_INT4 counter;

  /* ----------------------------- */
  /* Check the number of arguments */
  /* ----------------------------- */
  if (argc < 4 || argc > 5) {
    printf("Usage: waitsbj <project> <subject> <count> [timeout]\n");
    T_EXIT_MAIN(T_EXIT_FAILURE); 
  } /* if */


  /* --------------------------------------------------- */
  /* Get the project name from the command line argument */
  /* --------------------------------------------------- */
  strcpy(project_name, argv[1]);
  strcpy(subject_name, argv[2]);
  count = atoi(argv[3]);

  if (argc == 4) {
    timeout = -1.0;
  } /* if */
  else {
    timeout = atof(argv[4]); 
  } /* else */
	
  /* ------------------------ */
  /* set the project name */
  /* ------------------------ */
  sprintf(option, "setopt project %s\n", project_name);
  TutCommandParseStr(option);

  /* ---------------------------------------------------------------------- */
  /* Load a command file if there is one (to allow things like server_names */
  /* to be set).  SMW 04/25/95                                              */
  /* ---------------------------------------------------------------------- */
  if (TutFileExists("waitsbj.cm")) {
    TutCommandParseFile("waitsbj.cm");
  } /* if */

  TutOut("Watching subject %s in project <%s>...\n",
         subject_name, project_name);

  /* ------------------- */
  /* Connect to RTserver */
  /* ------------------- */
  if (TipcSrvGetConn() == NULL) {
    if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
      TutOut("ERROR: could not create a connection\n"); 
      T_C_EXIT(T_EXIT_FAILURE); 
    }
  }

  /* ------------------------------------------------- */
  /* Setup callback to catch subject status messages */
  /* ------------------------------------------------- */
  mt = TipcMtLookupByNum(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS); 
  T_ASSERT(mt != T_NULL); 
  cb = TipcSrvProcessCbCreate(mt, 
			      ReceiveMonSubjectSubscribeStatus, 
			      (T_CB_ARG)count);
  T_ASSERT(cb != T_NULL); 
  cb = TipcSrvDefaultCbCreate(DefaultReceiveFunc, NULL); 
  T_ASSERT(cb != T_NULL); 

  /* --------------------------------------------------------- */
  /* If the desired count is 0, we have to first check if the  */
  /* subject exists (this changed in 3.5 (2), but I'm not sure */
  /* I'm going to leave it that way).  SMW 09/03/95            */
  /* --------------------------------------------------------- */
  if (count == 0) {
    status = TipcMonSubjectNamesPoll();
    T_ASSERT(status == T_TRUE); 
    msg = TipcSrvMsgSearchType(5.0, 
			TipcMtLookupByNum(T_MT_MON_SUBJECT_NAMES_POLL_RESULT));
    T_ASSERT(msg != NULL);
    status = TipcMsgSetCurrent(msg, 0);
    T_ASSERT(status == T_TRUE); 
    status = TipcMsgNextStrArray(msg, &subject_names, &num_subject_names);
    T_ASSERT(status == T_TRUE); 

    for (counter = 0; counter < num_subject_names; counter++) {
      if (T_STREQI(subject_names[counter], subject_name)) {
	break; /* for */
      } /* if */
    } /* for */

    status = TipcMsgDestroy(msg);
    T_ASSERT(status == T_TRUE); 

    if (counter >= num_subject_names) {
      TutOut("Subject %s doesn't exist: done at count 0.\n", subject_name);
      T_C_EXIT(T_EXIT_SUCCESS);
    } /* if */
  } /* if */

  /* ---------------------------- */
  /* Start "watching" the subject */
  /* ---------------------------- */
  status = TipcMonSubjectSubscribeSetWatch(subject_name, TRUE); 
  T_ASSERT(status == T_TRUE); 
  status = TipcSrvFlush();  
  T_ASSERT(status == T_TRUE); 

  /* ----------------------------------------------------------------------- */
  /* Read messages from RTserver and call the registered callbacks as needed */
  /* ----------------------------------------------------------------------- */
  if (timeout < 0.0) {
#if 1
    /* ----------------------------------------------------------- */
    /* Limit wait to 5 minutes to prevent test cases from hanging. */
    /* SMW 09/03/95                                                */
    /* ----------------------------------------------------------- */
    TipcSrvMainLoop(300.0);
#else
    TipcSrvMainLoop(T_TIMEOUT_FOREVER);
#endif
  } /* if */
  else {
    TipcSrvMainLoop(timeout);
  } /* else */

  if (TutErrNumGet() == T_ERR_TIMEOUT_REACHED) {
    TutWarning("Wait_Subject timed out waiting for response from server.\n");
    T_EXIT_MAIN(T_EXIT_FAILURE);
  } /* if */

  T_EXIT_MAIN(T_EXIT_SUCCESS);

} /* main */
