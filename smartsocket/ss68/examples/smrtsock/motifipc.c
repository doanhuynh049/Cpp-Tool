/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/motifipc.c#1 $
 */

/* =================================================================== */
/*                                                                     */
/*          motifipc.c -- example of mixing RTworks IPC and Motif      */
/*                                                                     */
/* =================================================================== */

#include <rtworks/ipc.h>
#include <Xm/PushB.h>

/* --------------------- */
/* Example message type. */
/* --------------------- */
#define EX_MT_MOTIF_MESSAGE 1001

/* ========================================================================= */
/*..cb_process_motif_message -- process EX_MT_MOTIF_MESSAGE message */
static void T_ENTRY cb_process_motif_message(conn, data, arg)
T_IPC_CONN conn;
T_IPC_CONN_PROCESS_CB_DATA data;
T_CB_ARG arg;
{
  T_STR str_val;

  TutOut("Entering cb_process_motif_message.\n");
  
  /* ------------------------------------------- */
  /* Set current field to first field in message */
  /* ------------------------------------------- */
  if (!TipcMsgSetCurrent(data->msg, 0)) {
    TutOut("Could not get current field of message.\n");
    return;
  }

  /* ------------------- */
  /* Access string field */
  /* ------------------- */
  while (TipcMsgNextStr(data->msg, &str_val)) {
    TutOut("Message received: %s\n", str_val);
  }
  
} /* cb_process_motif_message */

/* ========================================================================= */
/*..exSendMessage -- send a message to ourself */
static void T_ENTRY exSendMessage()
{
  T_IPC_MT mt;
  T_IPC_MSG msg;
  T_STRING str_field;
  static T_INT4 counter = 0;

  TutOut("Entering exSendMessage.\n");

  /* ---------------------------------------- */
  /* Send Motif Message to "motifipc" subject */
  /* ---------------------------------------- */
  mt = TipcMtLookupByNum(EX_MT_MOTIF_MESSAGE);
  if (mt == NULL) {
    TutOut("Could not lookup EX_MT_MOTIF_MESSAGE message type.\n");
    return;
  } /* if */

  msg = TipcMsgCreate(mt);
  if (msg == NULL) {
    TutOut("Could not create EX_MT_MOTIF_MESSAGE message.\n");
    return;
  } /* if */

  sprintf(str_field, "This is message %d\n", ++counter);
  if (!TipcMsgAppendStr(msg, str_field)) {
    TutOut("Could not append string field.\n");
  } /* if */

  if (!TipcMsgSetDest(msg, "motifipc")) {
    TutOut("Could not set msg dest.\n");
  } /* if */

  if (!TipcSrvMsgSend(msg, TRUE)) {
    TutOut("Could not send MOTIF_MESSAGE message.\n");
  } /*if*/

  if (!TipcMsgDestroy(msg)) {
    TutOut("Could not destroy msg.\n");
  } /* if */

  if (!TipcSrvFlush()) {          /* Send message */
    TutOut("Could not flush msg.\n");
  } /* if */

} /* exSendMessage */

/* ========================================================================= */
/*..exGetMessages -- process all messages from RTserver */
/* ARGSUSED */
static void T_ENTRY exGetMessages(client_data, source, id)
XtPointer client_data; /* not used */
int *source; /* not used */
XtInputId *id; /* not used */
{
  T_IPC_MSG msg;

  TutOut("Entering exGetMessages.\n");

  /* ---------------------------------------------------------------------- */
  /* Use TipcSrvMainLoop (or TipcSrvMsgNext in a loop) here instead of just */
  /* calling TipcSrvMsgNext once; otherwise, you may read in a message but  */
  /* leave it in the connection message queue (where Xt cannot see it).     */
  /* ---------------------------------------------------------------------- */
  if (!TipcSrvMainLoop(0.0)) {
    TutOut("TipcSrvMainLoop failed with error code %d.\n",
	   TutErrNumGet());
  } /* if */

} /* exGetMessages */

/* ========================================================================= */
/*..main -- main program */
int main(argc, argv)
int argc;
char **argv;
{
  Widget top;
  XtAppContext app_context;
  Widget push_button;
  T_INT4 source;
  T_IPC_MT mt;

  /* ----------------------------- */
  /* Initialize the Motif toolkit. */
  /* ----------------------------- */
  top = XtVaAppInitialize(&app_context,
			  "Motifipc",
			  NULL,
			  0,
			  (T_PTR)&argc,
			  argv,
			  NULL,
			  XtNallowShellResize, True,
			  NULL);
  if (top == NULL) {
    TutFatalError("main: XtAppIntialize failed.\n");
  } /* if */

  /* ----------------- */
  /* Connect to server */
  /* ----------------- */
  TutOut("Creating connection to RTserver.\n");
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not create the connection to RTserver, err_code = %d\n",
	   TutErrNumGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* ------------------------------------------------------- */
  /* Start receiving the standard subjects for the hci, with */
  /* retrieval, including the _time subject.                 */
  /* ------------------------------------------------------- */
  if (!TipcSrvStdSubjectSetSubscribe(TRUE,   /* value of subject receive flag*/
				     TRUE)) { /* _time subject flag */
    TutOut("Could not start receiving standard subjects\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  if (!TipcSrvStdSubjectRetrieve(TRUE)) {  /* retrieve the _time subject */
    TutOut("Could not retrieve standard subjects\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* --------------------------------- */
  /* Start receiving subject motifipc. */
  /* --------------------------------- */
  if (!TipcSrvSubjectSetSubscribe("motifipc", TRUE)) {
    TutOut("Could not start receiving motifipc subject\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */
  if (!TipcSrvFlush()) {
    TutOut("Could not flush messages.\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* --------------------------------------------------- */
  /* Create new message type, EX_MOTIF_MESSAGE, with the */
  /* following grammar "str".                            */
  /* --------------------------------------------------- */
  mt = TipcMtCreate("motif_msg", EX_MT_MOTIF_MESSAGE, "str");
  if (mt == NULL) {
    TutOut("Could not create EX_MT_MOTIF_MESSAGE message type.\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* ---------------------------------- */
  /* process callback for MOTIF_MESSAGE */
  /* ---------------------------------- */
  if (!TipcSrvProcessCbCreate(mt, cb_process_motif_message, NULL)) {
    TutOut("Could not create MOTIF_MESSAGE process callback.\n");
    TutExit(T_EXIT_FAILURE);
  }

  /* --------------------- */
  /* Create a push button. */
  /* --------------------- */
  push_button = XtVaCreateManagedWidget("sendMessage",
					xmPushButtonWidgetClass,
					top, /* parent */
					XmNlabelString, 
					XmStringCreateSimple("Send Message"),
					NULL);
  XtAddCallback(push_button, 
		XmNactivateCallback, 
		(XtCallbackProc) exSendMessage, 
		NULL);
		  
  /* ---------------------------------------------------- */
  /* Register the RTworks IPC file descriptor with Motif. */
  /* ---------------------------------------------------- */
  if (!TipcSrvGetXtSource(&source)) {
    TutOut("Could not get Xt source.\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */
  TutOut("TipcGetXtSource ==> %d\n", source);
  XtAppAddInput(app_context,
		source,
#ifdef T_OS_VMS
		NULL,
#else
		(XtPointer) XtInputReadMask,
#endif
		exGetMessages,
		(XtPointer)NULL);

  /* ------------------------------------------------ */
  /* Realize our top widget and start the event loop. */
  /* ------------------------------------------------ */
  XtRealizeWidget(top);
  XtAppMainLoop(app_context);
} /* main */

/* ========================================================================= */

/*..motifipc.c */










