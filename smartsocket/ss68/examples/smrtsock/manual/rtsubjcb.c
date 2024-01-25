/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/rtsubjcb.c#1 $
 */

/* 
 * Mentioned in "RTserver and RTclient" chapter in the
 * "SmartSockets User's Guide". 
 */

/* rtsubjcb.c - print out contents of messages processed via 
	subject callback */

#include <rtworks/ipc.h>
#include "rtclient.h"

/* ============================================================== */
/*..ProcessInfo - callback for processing INFO messages published 
                  to the EXAMPLE_SUBJECT subject */
static void T_ENTRY ProcessInfo
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_STR msg_text;
  
  TutOut("Entering ProcessInfo callback.\n");

  if (!TipcMsgSetCurrent(msg, 0)) {
    TutOut("Could not set current field of message: error <%s>.\n",
            TutErrStrGet());
    return;
  }

  if (!TipcMsgNextStr(msg, &msg_text)) {
    TutOut("Could not text from INFO messag: error <%s>.\n",
            TutErrStrGet());
    return;
  }

  TutOut("Text from message = %s\n", msg_text);
} /* ProcessInfo */
/* ================================================================ */
/*..ProcessNumData - callback for processing NUMERIC_DATA messages 
                     published  to the /tutorail/lesson4 subject */
static void T_ENTRY ProcessNumData
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_STR name;
		T_REAL8 value;
  
  TutOut("Entering ProcessNumData callback.\n");

  if (!TipcMsgSetCurrent(msg, 0)) {
    TutOut("Could not set current field of message: error <%s>.\n",
            TutErrStrGet());
    return;
  }

  /* Access and print fields */
  while (TipcMsgNextStrReal8(msg, &name, &value)) {
    TutOut("Var Name = %s; Value = %s\n", name, TutRealToStr(value));
  } /* while */

  /* Make sure we reached end of message */
  if (TutErrNumGet() != T_ERR_MSG_EOM) {
    TutOut("Did not reach end of message: error <%s>.\n",
            TutErrStrGet());
  }
} /* ProcessNumData */

/* ================================================================ */
int main
(
 int argc,
 char **argv
)
{
  T_OPTION option;
  T_IPC_MT mt;

  /* Set the project name */
  option = TutOptionLookup("project");
  if (option == NULL) {
    TutOut("Could not look up option named project: error <%s>.\n",
            TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TutOptionSetEnum(option, EXAMPLE_PROJECT)) {
    TutOut("Could not set option named project: error <%s>.\n",
            TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Connect to the RTserver */
  TutOut("Creating a connection to RTserver.\n");
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Create subject callbacks to be executed when messages arrive
     with a given destination and given type */
  TutOut("Creating subject callbacks.\n");

  /* Subject callback for INFO messages with destination 
     EXAMPLE_SUBJECT subject */
  mt = TipcMtLookupByNum(T_MT_INFO);
  if (mt == NULL) {
    TutOut("Could not lookup INFO message type: error <%s>.\n",
            TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
		if (TipcSrvSubjectCbCreate(EXAMPLE_SUBJECT, mt, 
                             ProcessInfo, NULL)
      == NULL) {
    TutOut("Could not create ProcessInfo subject callback: ",
							"error <%s>.\n", TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Subject callback for NUMERIC_DATA messages with destination 
     EXAMPLE_SUBJECT subject */
  mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
  if (mt == NULL) {
    TutOut("Could not lookup NUMERIC_DATA message type: ",
							"error ,<%s>.\n", TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  if (TipcSrvSubjectCbCreate(EXAMPLE_SUBJECT, mt, 
                             ProcessNumData, NULL)
      == NULL) {
    TutOut("Could not create ProcessNumData subject callback: ",
           "error <%s>.\n", TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Start subscribing to the EXAMPLE_SUBJECT subject */
  TutOut("Start subscribing to the %s subject.\n", EXAMPLE_SUBJECT);
  if (!TipcSrvSubjectSetSubscribe(EXAMPLE_SUBJECT, TRUE)) {
    TutOut("Could not subscribe to %s subject: error <%s>.\n",
            TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Read and process all incoming messages */
  if (!TipcSrvMainLoop(T_TIMEOUT_FOREVER)) {
     TutOut("TipcSrvMainLoop failed: error <%s>.\n",
            TutErrStrGet());
  }

} /* main */
