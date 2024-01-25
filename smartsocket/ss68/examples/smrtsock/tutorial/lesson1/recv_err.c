/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/tutorial/lesson1/recv_err.c#1 $
 */

/* recv_err.c - read/print a message; receive.c with error checking */
/* $RTHOME/examples/smrtsock/tutorial/lesson1/recv_err.c */

#include <rtworks/ipc.h>

int main(int argc, char **argv)
{
  T_IPC_MSG msg;
  T_STR msg_text;

  /* 
   * The following function call performs two tasks:
   *     1. Since there was no explicit call to connect to RTserver,
   * 	    this process will first establish a connection to RTserver
   * 	    (this might include auto-starting RTserver).
   *     2. Inform RTserver to forward any message which has
   *        been sent to the "/tutorial/lesson1" subject by any client 
   *        in the project.
   */
  if (!TipcSrvSubjectSetSubscribe("/tutorial/lesson1", TRUE)) {
    TutOut("Can't subscribe to </tutorial/lesson1>. Failed with error <%s>\n",
            TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /*
   * This function calls gets the next message from its message queue;
   * The T_TIMEOUT_FORVER constant specifies that the function will block 
   * (wait) forever for a message. You can specify an actual time-out period
   * for this argument; for example, 10.0 would mean the function would
   * return with a failure code after 10 seconds.
   */
  msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);
  if (msg == NULL) {
    TutOut("Could not retrieve message. Failed with error <%s>.\n",
            TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* 
   * We now have a message to work with. The following call sets the message
   * pointer (i.e., the current field) to the first field in the data part
   * of the message 
   */
  if (!TipcMsgSetCurrent(msg, 0))  {
    TutOut("Can't set message pointer. Failed with error <%s>.\n",
            TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  } /* if */

  /* The next call gets the string field from the data part of the message */
  if (!TipcMsgNextStr(msg, &msg_text))  {
    TutOut("Can't retrieve str field. Failed with error <%s>.\n",
            TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  } /* if */
  
  /* Finally, we print out the text we have retrieved from the message */  
  TutOut("Text from INFO message = %s\n", msg_text);

  if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE)) {
    TutOut("Can't disconnect. Failed with error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }

} /* main */
