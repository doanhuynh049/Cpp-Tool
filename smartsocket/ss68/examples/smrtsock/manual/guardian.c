/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/guardian.c#1 $
 */

/* 
 * From "Project Monitoring" chapter in the "SmartSockets User's Guide" 
 */ 

/* guardian.c -- RTclient example for managing a hot backup process */

/* 
   --------------------------------------------------------------------
  USAGE
		guardian.x <project name>

  Also can use udrecv.c, udsend.c, primary.cm, and backup.cm to test.

  This program is an RTclient which provides a primary and 
  backup RTclient for the project passed in as a command-line 
  argument. To use this, one must have two duplicate RTclients
  with different startup command files. 

  In the first command file, which is for the primary RTclient, the 
  following lines should be added (or modified if they already exist):
    setopt server_msg_send TRUE
    setopt subjects <existing_subjects>, primary_client

  The second command file, which is for the "hot backup", the
  following lines should be added (or modified if they already exist):
    setopt server_msg_send FALSE
    setopt subjects <existing_subjects>, backup_client

  You should also have shell scripts to start the primary and backup
  RTclients named "startpcl" and "startbcl" respectively.

  The guardian program then watches the primary_client and 
  backup_client and will switch the backup over needed when the primary 
  fails or will restart the backup if it fails.
  --------------------------------------------------------------------
  */

#define NOT_STARTED -1
#include <rtworks/ipc.h>

static T_STR project_name;
static T_INT4 num_primary = NOT_STARTED; /* Primary RTclient count */
static T_INT4 num_backup = NOT_STARTED; /* Backup RTclient count */

/* ================================================================ */
/*..cb_default -- default callback to handle unexpected messages */
static void T_ENTRY cb_default
(
 T_IPC_CONN conn,
 T_IPC_CONN_DEFAULT_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MT mt;
  T_STR name;
  
  /* print out the name of the type of the message */
  if (!TipcMsgGetType(data->msg, &mt)) {
    TutOut("Could not get message type from message: error <%s>.\n",
	   TutErrStrGet());
    return;
  }
  if (!TipcMtGetName(mt, &name)) {
    TutOut("Could not get name from message type: error <%s>.\n",
	   TutErrStrGet());
    return;
  }
  TutOut("Unexpected message type name is <%s>\n", name);
} /* cb_default */

/* ================================================================ */
/*..cb_subject_status -- callback to process 
                         MON_SUBJECT_SUBSCRIBE_STATUS messages */
static void T_ENTRY cb_subject_status
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_IPC_MT mt;
  T_STR subject_name;
  T_STR lc_subject_name; /* lower-case version of subject_name */
  T_INT4 subject_count;
  T_REAL8 wall_time;
  T_STR time_str;
  T_STR *subscribe_client_names;
  
  /* 
   * cb_subject_status is called when a MON_SUBJECT_SUBSCRIBE_STATUS 
   * subject status message is received. Each time an RTclient starts 
   * or stops subscribing to the primary_client or backup_client
   * subjects this function is called. 
   */
  
  /* Get the current wall clock time and convert it to a string */
  wall_time = TutGetWallTime();
  time_str = TutTimeNumToStr(wall_time);
  
  /* Parse the subject status message for the subject id and */
  /* the number of RTclients subscribed to it. */
  
  /* Set current field */
  if (!TipcMsgSetCurrent(msg, 0)) {
    TutOut("Could not set current field of message: error <%s>.\n",
	   TutErrStrGet());
    return;
  }

  /* Get the fields of interest from the message */
  if (!TipcMsgNextStr(msg, &subject_name)
      || !TipcMsgNextStrArray(msg, &subscribe_client_names, 
			      &subject_count)) {
    TutOut("Unable to access MON_SUBJECT_SUBSCRIBE_STATUS");
    TutOut("message: error <%s>.\n", TutErrStrGet());
    return;
  }
  
  TutOut("Time = %s\n", time_str );
  TutOut("%d RTclients are subscribing to the %s subject.\n",
	 subject_count, subject_name );
  
  /* make a copy of the subject name and convert to lower case */
  T_STRDUP(lc_subject_name, subject_name);
  TutStrLwr(lc_subject_name, lc_subject_name);
  
  if (strcmp(lc_subject_name, "/primary_client") == 0) {
    num_primary = subject_count;
  }
  else if (strcmp(lc_subject_name, "/backup_client") == 0) {
    num_backup = subject_count;
  }
  else {
    TutOut("Received SUBJECT_SUBSCRIBE_STATUS for unwanted ");
    TutOut("subject %s.\n", lc_subject_name);
    T_FREE(lc_subject_name);
    return;
  }
  
  /* Prepare to send CONTROL messages later on. */
  mt = TipcMtLookupByNum(T_MT_CONTROL);
  if (mt == NULL) {
    TutOut("Could not look up control message: error <%s>.\n",
	   TutErrStrGet());
    return;
  }
  
  /* 
   * There are four main cases that need to be covered. They are
   * listed below followed by the actions to be taken when the case
   * is encountered:
   * CASE 1: Neither primary or backup RTclient is running.
   *	     start primary RTclient
   *	     start backup RTclient
   * CASE 2: Both primary and backup RTclients are running.
   *	     output message that all is OK
   * CASE 3: Primary RTclient fails; backup RTclient is running.
   * 	     set server_msg_send option in backup RTclient to TRUE
   *	     Have backup start subscribing to the primary_client subject
   * 	     Have backup stop subscribing to the backup_client subject
   * 	     (This will then cause CASE 4 to occur);
   * CASE 4: Backup RTclient fails, primary RTclient is running.
   *	     Restart backup RTclient
   */
  
  /* Check if neither the primary RTclient nor backup RTclient have */
  /* been started yet */
  if (num_primary == 0 && num_backup == 0) {
    TutOut("Neither primary nor backup RTclient yet started.\n");
    TutOut("Starting primary RTclient...\n");
    TutSystem("startpcl &");
    TutOut("Starting backup RTclient...\n");
    TutSystem("startbcl &");
  }
  
  else if (num_primary == 1 && num_backup == 1) {
    TutOut("Both primary and backup RTclients are running!\n");
  }
  
  else if (strcmp(lc_subject_name, "/primary_client") == 0) {
    if (num_primary == 1 && num_backup <= 0) {
      TutOut("Primary RTclient up and running! Waiting on backup...\n");
    }
    else if (num_primary == 0 && num_backup <= 0) {
      TutOut("No primary RTclient yet; No report yet from backup \n");
    }
    /* Check if we have lost the primary RTclient */
    else if (num_primary == 0 && num_backup == 1 ){
      TutOut("Primary RTclient has failed!\n");
      
      TutOut("Switching the backup RTclient to be primary...\n");
      if (!TipcSrvMsgWrite("backup_client", mt, TRUE, T_IPC_FT_STR, 
			   "setopt server_msg_send TRUE", NULL)) {
	TutOut("Could not send setopt control message to ");
	TutOut("backup_client: error <%s>.\n", TutErrStrGet());
      }
      
      if (!TipcSrvMsgWrite("backup_client", mt, TRUE, T_IPC_FT_STR,
			   "subscribe primary_client", NULL)) {
	TutOut("Could not send subscribe control message to ");
	TutOut("backup_client: error <%s>.\n", TutErrStrGet());
      }
      
      if (!TipcSrvMsgWrite("backup_client", mt, TRUE, T_IPC_FT_STR,
			   "unsubscribe backup_client", NULL)) {
	TutOut("Could not send unsubscribe control message to ");
	TutOut("backup_client: error <%s>.\n", TutErrStrGet());
      }
    }
    else {
      TutOut("We have an irregular number of RTclients!\n");
      TutOut("Number of primary RTclients: %d\n", num_primary);
      TutOut("Number of backup RTclients: %d\n", num_backup);
    }
  }
  else if (strcmp(lc_subject_name, "/backup_client") == 0) {
    if (num_primary <= 0 && num_backup == 1) {
      TutOut("Backup RTclient up and running! Waiting on primary...\n");
    }
    else if (num_primary <= 0 && num_backup == 0) {
      TutOut("No backup RTclient yet; ");
      TutOut("No report received yet from primary RTclient.\n");
    }
    /* Check if we have lost the backup RTclient */
    else if (num_primary == 1 && num_backup == 0){
      TutOut("Backup RTclient is down!\n");
      TutOut("Starting a new backup RTclient!\n");
      TutSystem("startbcl &");
    }
    else {
      TutOut("We have an irregular number of RTclients!\n"); 
      TutOut("Number of primary RTclients : %d\n", 
	     num_primary);
      TutOut("Number of backup RTclients : %d\n", 
	     num_backup);
    }
  }
  
  TutOut("================================\n");
  T_FREE(lc_subject_name);
} /* cb_subject_status */

/* ================================================================ */
/*..main -- main program */
int main
(
 int argc,
 char **argv
)
{
  T_OPTION option;
  T_IPC_MT mt;
  
  /* Check the command-line arguments */
  if (argc != 2) {
    TutOut("Usage: guardian <project>\n");
    TutExit(T_EXIT_FAILURE); 
  }
  
  /* Save the pointer to the command line argument */
  project_name = argv[1];
  TutOut("Monitoring project <%s>...\n", project_name);
  
  /* Set the project name */
  option = TutOptionLookup("project");
  if (option == NULL) {
    TutOut("Could not look up option named project: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TutOptionSetEnum(option, project_name)) {
    TutOut("Could not set the option named <project>: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* Set the time format for the FULL format */
  TutCommandParseStr("setopt time_format full");
  
  /* Create a connection to RTserver */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("Could not connect to RTserver: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* Create callback to process MON_SUBJECT_SUBSCRIBE_STATUS msgs */
  mt = TipcMtLookupByNum(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS);
  if (mt == NULL) {
    TutOut("Could not look up MON_SUBJECT_SUBSCRIBE_STATUS");
    TutOut("message type: error <%s>.\n", TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (TipcSrvProcessCbCreate(mt, cb_subject_status, NULL) == NULL) {
    TutOut("Could not create MON_SUBJECT_SUBSCRIBE_STATUS");
    TutOut("process callback: error <%s>.\n", TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* Create default callback to handle unwanted message types */
  if (TipcSrvDefaultCbCreate(cb_default, NULL) == NULL) {
    TutOut("Could not create default callback: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
		/* Start watching primary_client and backup_client subjects */
  TutOut("Starting to watch <primary_client> subject.\n");
  if (!TipcMonSubjectSubscribeSetWatch("primary_client", TRUE)) {
    TutOut("Could not start watching primary_client subject.\n");
    TutOut("  error <%s>.\n", TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Starting to watch <backup_client> subject.\n");
  if (!TipcMonSubjectSubscribeSetWatch("backup_client", TRUE)) {
    TutOut("Could not start watching backup_client subject.\n");
    TutOut("  error <%s>.\n", TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  /* If RTserver stops, then TipcSrvMainLoop will restart RTserver */
  /* and return FALSE. We can safely continue. */
  for (;;) {
    if (!TipcSrvMainLoop(T_TIMEOUT_FOREVER)) {
      TutOut("TipcSrvMainLoop failed: error <%s>.\n",
	     TutErrStrGet());
    }
  }
  
  /* This line should never be reached */
  TutOut("This line should never be reached!\n");
  return T_EXIT_FAILURE;
} /* main */
