/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson3/receive.c#1 $
 */

/* receive.c - read an INFO message and print out its contents */
/* $RTHOME/examples/smrtsock/tutorial/lesson3/receive.c */

#include <rtworks/ipc.h>

int main(int argc, char **argv)
{
  T_OPTION option;
  T_IPC_MSG msg;
  T_STR msg_text;
  
  /* Set the name of the project */
  option = TutOptionLookup("project");
  TutOptionSetEnum(option, "smartsockets");

  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to Rtserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* 
   * Subscribe to receive any messages sent to the 
   * "/tutorial/lesson3" subject.
   */
  TipcSrvSubjectSetSubscribe("/tutorial/lesson3", TRUE);

  /*
   * This function calls gets the next message from its message queue;
   * The T_TIMEOUT_FORVER constant specifies that the function will block 
   * (wait) forever for a message. You can specify an actual time-out period
   * for this argument; for example, 10.0 would mean the function would
   * return with a failure code after 10 seconds.
   */
  msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

  /* 
   * We now have a message to work with. The following call sets the
   * message pointer (i.e., the current field) to the first field in the
   * data part of the message 
   */
  TipcMsgSetCurrent(msg, 0);

  /*
   * The next call gets the string field from the data part
   * of the message.  
   */
  TipcMsgNextStr(msg, &msg_text);
  
  /* 
   * Finally, we print out the infromation we have retrieved from the
   * message
   */
  TutOut("Text from INFO message = %s\n", msg_text);

  TipcSrvDestroy(T_IPC_SRV_CONN_NONE);
} /* main */
