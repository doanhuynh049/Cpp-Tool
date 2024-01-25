/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson3/receive2.c#1 $
 */

/* receive2.c - read a NUMERIC_DATA message and print contents */
/* $RTHOME/examples/smrtsock/tutorial/lesson3/receive2.c */

#include <rtworks/ipc.h>

int main(int argc, char **argv)
{
  T_OPTION option;
  T_IPC_MSG msg;
  T_STR var_name;
  T_REAL8 var_value;
  
  /* Set the name of the project */
  option = TutOptionLookup("project");
  TutOptionSetEnum(option, "smartsockets");

  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver!\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* 
   * Subscribe to receive any messages sent to the 
   * "/tutorial/lesson3" subject
   */
  TipcSrvSubjectSetSubscribe("/tutorial/lesson3", TRUE);

  /* This function calls gets the next message from its message queue */
  msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

  /* 
   * We now have a message to work with. The following call sets the 
   * message pointer (i.e., the current field) to the first field in the
   * data part of the message 
   */
  TipcMsgSetCurrent(msg, 0);

  /* 
   * Move through the data part of the message; print out each 
   * variable name, variable value pair in the message
   */
  TutOut("Contents of NUMERIC_DATA message:\n");
  TutOut("---------------------------------\n");

  while (TipcMsgNextStr(msg, &var_name)) {
    TipcMsgNextReal8(msg, &var_value);
    TutOut("Var name = %s, Var Value = %s\n", var_name, 
           TutRealToStr(var_value));
  } /* while */

  /* Disconnect from RTserver */
  TipcSrvDestroy(T_IPC_SRV_CONN_NONE); 

} /* main */
