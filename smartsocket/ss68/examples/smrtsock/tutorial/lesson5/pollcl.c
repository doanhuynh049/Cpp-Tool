/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson5/pollcl.c#1 $
 */

/* pollcl.c - poll clients for information */
/* $RTHOME/examples/smrtsock/tutorial/lesson5/pollcl.c */

#include <rtworks/ipc.h>

/* ========================================================================= */
/* OutputSubjectSubscribePoll - Output message of type                       */
/*                              MON_SUBJECT_SUBSCRIBE_POLL_RESULT            */
static void OutputSubjectSubscribePoll(T_IPC_MSG msg)
{
  T_STR subject_name;
  T_STR *subscribe_client_names;
  T_INT4 num_subscribe_clients;
  T_INT4 n;  

  /* Set current field */
  TipcMsgSetCurrent(msg, 0);

  /* Get the fields from the message */
  TipcMsgNextStr(msg, &subject_name);
  TipcMsgNextStrArray(msg, &subscribe_client_names, 
		      &num_subscribe_clients);

  /* Print out the new information received */
  TutOut("Result of SUBJECT_SUBSCRIBE poll\n");
  TutOut("Number of clients subscribed to <%s> = %d\n", 
         subject_name, num_subscribe_clients);

  for (n = 0; n < num_subscribe_clients; n++) {
    TutOut("   [%d] %s\n", n, subscribe_client_names[n]);
  } /* for */

  TutOut("======================================================\n");
  
} /* OutputSubjectSubscribePoll */

/* ========================================================================= */
int main(int argc, char **argv)
{
  T_IPC_MT mt;
  T_IPC_MSG msg;

  /* Set the project name */
  TutCommandParseStr("setopt project smartsockets");

  /* Connect to the RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver.\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* Poll for names of client processes subscribed to 
     "/tutorial/lesson5" subject */
  TipcMonSubjectSubscribePoll("/tutorial/lesson5");

  /* 
   * The poll will return a message of type 
   * MON_SUBJECT_SUBSCRIBE_POLL_RESULT.
   */
  mt = TipcMtLookupByNum(T_MT_MON_SUBJECT_SUBSCRIBE_POLL_RESULT);

  /*
   * Wait for up to 10 seconds for the poll result. We use
   * TipcSrvMsgSearchType to search the incoming message queue for
   * the poll result
   */
  msg = TipcSrvMsgSearchType(10.0, mt);

  if (msg == NULL) {
    /* If no message received, there are no clients subscribed */
    TutOut("Result of SUBJECT_SUBSCRIBE poll\n");
    TutOut("Number of clients subscribed to </tutorial/lesson5> = 0\n");
  }
  else {
    /* Poll was successful, output the results */
    OutputSubjectSubscribePoll(msg);
  }

  /* Disconnect from RTserver */
  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);

} /* main */
