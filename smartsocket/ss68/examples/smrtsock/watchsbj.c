/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/watchsbj.c#1 $
 */

/* ------------------------------------------------------------------ */
/* watchsbj.c                                                          */
/*                                                                    */
/* USAGE                                                              */
/*    watchsbj <project> <subject>                                     */
/*                                                                    */
/* This program connects to RTserver and outputs a count of how many  */
/* processes have "subscribed" to <subject> in <project>.             */
/* Whenever a process joins or leaves a subject, the current number   */
/* of processes receiving the subject will be output.                 */
/* ------------------------------------------------------------------ */

#include <rtworks/ipc.h>

static T_STR ProjectName;
static T_STR SubjectName;

/* ------------------------------------------------------------------*/
static void T_ENTRY DefaultReceiveFunc
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  TipcMsgPrintError(data->msg);
} /* DefaultReceiveFunc */

/* ------------------------------------------------------------------ */
static void T_ENTRY ReceiveSubjectStatus
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_STR subject_id;
  T_STR *client_names;
  T_INT4 subject_count;

  TipcMsgSetCurrent(msg, 0);
  TipcMsgNextStr(msg, &subject_id);
  TipcMsgNextStrArray(msg, &client_names, &subject_count);

  TutOut("Number of <%s> processes running in project <%s> is %d\n", 
	 SubjectName, ProjectName, subject_count );
} /* ReceiveSubjectStatus */

/* ---------------------------------------------------------------------- */
int main
(
 int argc,
 char **argv
)
{
  T_OPTION option;
  T_IPC_MT mt;

  /* Check the command line arguments */
  if (argc != 3) {
    printf("Usage: watchsbj <project> <subject>\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Copy the command line arguments */
  ProjectName = argv[1];
  SubjectName = argv[2];
  TutOut("Watching subject <%s> in project <%s>...\n",
         SubjectName, ProjectName);

  /* -------------------- */
  /* Set the project name */
  /* -------------------- */
  option = TutOptionLookup("project");
  TutOptionSetEnum(option, ProjectName);

  /* ----------------------- */
  /* Connect to the RTserver */
  /* ----------------------- */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver.\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* -------------------------------------------------- */
  /* Create callback to process subject status messages */
  /* -------------------------------------------------- */
  mt = TipcMtLookupByNum(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS);
  if (mt == NULL) {
    TutOut("Could not lookup message type %d.\n", 
	   T_MT_MON_SUBJECT_SUBSCRIBE_STATUS);
    TutExit(T_EXIT_FAILURE);
  } /* if */

  TipcSrvProcessCbCreate(mt, ReceiveSubjectStatus, NULL);

  /* ------------------------------------------------ */
  /* Setup callback to receive unwanted message types */
  /* ------------------------------------------------ */
  TipcSrvDefaultCbCreate(DefaultReceiveFunc, NULL);

  /* ---------------------------- */
  /* Start "watching" the subject */
  /* ---------------------------- */
  TipcMonSubjectSubscribeSetWatch(SubjectName, TRUE);

  /* ----------------------------------------------------------------------- */
  /* Read messages from RTserver and call the registered callbacks as needed */
  /* ----------------------------------------------------------------------- */
  TipcSrvMainLoop(T_TIMEOUT_FOREVER);

  TutExit(T_EXIT_SUCCESS);
} /* main */

