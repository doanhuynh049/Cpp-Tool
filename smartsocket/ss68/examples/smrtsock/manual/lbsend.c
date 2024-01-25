/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/lbsend.c#1 $
 */

/* From "RTserver and RTclient" chapter in the "SmartSockets User's Guide". */

/* lbsend.c - send messages (some load balanced, some not) */

#include <rtworks/ipc.h>
#define MSG_COUNT 1001


/* ==================================================================== */
int main
(
 int argc,
 char **argv
)
{
  T_OPTION option;
  T_IPC_MSG msg;
  T_IPC_MT mt;
  T_INT4 i;

  /* Set the option Project to partition ourself. */
  option = TutOptionLookup("project");
  if (option == NULL) {
    TutOut("Could not look up option named project: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TutOptionSetEnum(option, "smartsockets")) {
    TutOut("Could not set option named project: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* Define a new message type */
  mt = TipcMtCreate("msg_count", MSG_COUNT, "int4");
  if (mt == NULL) {
    TutOut("Could not create message type MSG_COUNT: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* Create a message of type MSG_COUNT */
  msg = TipcMsgCreate(mt);
  if (msg == NULL) {
    TutOut("Could not create msg of type MSG_COUNT: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* Set the destination subject of the message */
  if (!TipcMsgSetDest(msg, "/manual/chapter4")) {
    TutOut("Could not set subject of message: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  for (i = 0; i < 20; i++) {
    /* Reset num of fields to 0 so we can reuse message */
    if (!TipcMsgSetNumFields(msg, 0)) {
      TutOut("<%d> Could not clear message: error <%s>.\n",
	     i, TutErrStrGet());
      TutExit(T_EXIT_FAILURE);
    }
    
    /* 
     * First 10 messages, send to everyone,
     * Second 10 messages, load balance using round robin
     */   
    if (i < 10) {
      /* This is the default behavior and not required */
      if (!TipcMsgSetLbMode(msg, T_IPC_LB_NONE)) {
	TutOut("Could not set load balance to NONE: error <%s>.\n",
	       TutErrStrGet());
      }
    }
    else {
      if (!TipcMsgSetLbMode(msg, T_IPC_LB_ROUND_ROBIN)) {
        TutOut("Could not set load balance to NONE: error <%s>.\n",
	       TutErrStrGet());
      }
    }
    
    /* Build the data part of the message with 1 integer */
    if (!TipcMsgAppendInt4(msg, i)) {
      TutOut("<%d> Could not build message: error <%s>.\n",
	     i, TutErrStrGet());
      TutExit(T_EXIT_FAILURE);
    }
    
    /* Publish the message */
    if (!TipcSrvMsgSend(msg, TRUE)) {
      TutOut("<%d> Could not publish message: error <%s>.\n",
	     i, TutErrStrGet());
    }
    
    /* Make sure message is flushed */  
    if (!TipcSrvFlush()) {
      TutOut("<%d> Could not flush message: error <%s>.\n",
	     i, TutErrStrGet());
    }
    
  }
  
  /* Destroy the message */
  if (!TipcMsgDestroy(msg)) {
    TutOut("<%d> Could not destroy message: error <%s>.\n",
	   i, TutErrStrGet());
  }

  /* destory server connection */
  if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE)) {
    TutOut("Can't disconnect. Failed with error <%s>.\n",
           TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  return T_EXIT_SUCCESS; /* all done */
} /* main */
