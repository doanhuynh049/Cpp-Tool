/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/watchsbj.c#1 $
 */

/*
  --------------------------------------------------------------------
  watchsbj.c                                                          
                                                                      
  USAGE                                                              
    watchsbj <project> <subject>                               

  This program connects to RTserver and outputs a count and names    
  of RTclients that subscribed to <subject> in <project>.   
  Whenever an RTclient subscribes or unsubscribes to a subject, 
		information about the subject is output.                                      
  ------------------------------------------------------------------- 
*/

#include <rtworks/ipc.h>

/* =============================================================== */
/*..cb_default - callback to process anything but 
                 MON_SUBJECT_SUBSCRIBE_STATUS */
static void T_ENTRY cb_default
(
 T_IPC_CONN conn,
 T_IPC_CONN_DEFAULT_CB_DATA data,
 T_CB_ARG arg
)
{
  TipcMsgPrintError(data->msg);
} /* cb_default */

/* =============================================================== */
/*..process_subject_status - callback to process 
                             MON_SUBJECT_SUBSCRIBE_STATUS msg */
static void T_ENTRY process_subject_status
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_STR subject_name;
  T_STR *subscribe_client_names;
  T_INT4 num_subscribe_clients;
  T_STR start_client;
  T_STR stop_client;
  T_INT4 n;  

  /* Set current field */
  if (!TipcMsgSetCurrent(msg, 0)) {
    TutOut("Could not set current field of message: error <%s>.\n",
	   TutErrStrGet());
    return;
  }

  /* Get the fields from the message */
  if (!TipcMsgNextStr(msg, &subject_name)
      || !TipcMsgNextStrArray(msg, &subscribe_client_names, 
			      &num_subscribe_clients)
      || !TipcMsgNextStr(msg, &start_client)
      || !TipcMsgNextStr(msg, &stop_client)) {
    TutOut("Unable to access MON_SUBJECT_SUBSCRIBE_STATUS");
    TutOut(" message: error <%s>.\n",
	   TutErrStrGet());
    return;
  }

  /* Print out the new information extracted from message */
  TutOut("Received change notice for subject <%s>\n", subject_name);
  TutOut("Number of clients subscribed to <%s> = %d\n", 
         subject_name, num_subscribe_clients);
  for (n = 0; n < num_subscribe_clients; n++) {
    TutOut("   [%d] %s\n", n, subscribe_client_names[n]);
  }

  TutOut("RTclient who just subscribed:   %s\n", start_client);
  TutOut("RTclient who just unsubscribed: %s\n", stop_client);
  TutOut("======================================================\n");
  
} /* process_subject_status */

/* =============================================================== */
int main
(
 int argc,
 char **argv
)
{
  T_STR project_name;
  T_STR subject_name;
  T_IPC_MT mt;
  T_OPTION option;

  /* Check the command line arguments */
  if (argc != 3) {
    TutOut("Usage: watchsbj <project> <subject>\n");
    TutExit(T_EXIT_FAILURE);
  } 

  /* Save pointers to command line arguments */
  project_name = argv[1];
  subject_name = argv[2];

  TutOut("Watching subject <%s> in project <%s>...\n",
         subject_name, project_name);

  /* Set the project name */
  option = TutOptionLookup("project");
  if (option == NULL) {
    TutOut("Could not look up option <project>: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  } 

  if (!TutOptionSetEnum(option, project_name)) {
    TutOut("Could not set option <project>: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  } 

  /* Connect to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  } 

  /* Create callback to process subject status messages */
  mt = TipcMtLookupByNum(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS);
  if (mt == NULL) {
    TutOut("Could not look up message type"); 
    TutOut(" MON_SUBJECT_SUBSCRIBE_STATUS: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  } 

  if (TipcSrvProcessCbCreate(mt, process_subject_status, NULL)
      == NULL) {
    TutOut("Could not create callback to process ");
    TutOut("MON_SUBJECT_SUBSCRIBE_STATUS: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  } 

  /* Create default callback to receive unwanted message types */
  if (TipcSrvDefaultCbCreate(cb_default, NULL) == NULL) {
    TutOut("Could not create default callback: error <%s>.\n",
	   TutErrStrGet());
  }

  /* Start "watching" the subject */
  if (!TipcMonSubjectSubscribeSetWatch(subject_name, TRUE)) {
    TutOut("Could not start watching ");
    TutOut("subject <%s>: error <%s>.\n",
	   subject_name, TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  } 

  /* If an error occurs, then TipcSrvMainLoop will restart RTserver */
  /* and return FALSE. We can safely continue. */
  for (;;) {
    if (!TipcSrvMainLoop(T_TIMEOUT_FOREVER)) {
      TutOut("TipcSrvMainLoop failed: error <%s>.\n", 
	     TutErrStrGet());
    } 
  }
  
  /* This line should not be reached. */
  return T_EXIT_FAILURE;
} /* main */
