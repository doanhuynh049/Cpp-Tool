/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/monitor.c#1 $
 */

/* ------------------------------------------------------------------- */
/* monitor.c                                                           */
/*                                                                     */
/* USAGE                                                               */
/*    monitor.x <project>                                              */
/*                                                                     */
/* This program is a RTserver client which provides a primary and      */
/* backup IE for the project passed in as a command line arg.          */
/* To use this, one must have two duplicate IE with different startup  */
/* command files. In the first command file, which will be for the     */
/* primary IE, the following lines should be added:                    */
/*	setopt  server_msg_send TRUE                                   */
/*	subscribe primary_ie                                           */
/* The second command file, which will be for the "hot backup", the    */
/* following lines should be added:                                    */
/*	setopt  server_msg_send FALSE                                  */
/*	subscribe backup_ie                                            */
/* One should also have shell scripts to start the primary and backup  */
/* IEs named "start_primary_ie" and "start_backup_ie" respectively.    */
/*                                                                     */
/* The monitor program then watches the primary_ie and backup_ie and   */
/* will switch the backup over needed when the primary fails or will   */
/* restart the backup if it fails.                                     */
/* ------------------------------------------------------------------- */
# define PROCESS_NOT_STARTED -1
#include <rtworks/ipc.h>

static T_STR ProjectName;
static T_TIME_CVT TimeCvtr;
static T_INT4 PrimaryIECount = PROCESS_NOT_STARTED; /* # Primary IEs running */
static T_INT4 BackupIECount  = PROCESS_NOT_STARTED; /* # Backup IEs running */

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

/* --------------------------------------------------------------------- */
/* ReceiveMonSubjectSubscribeStatus is the function which is called when */
/* a subject subscribe status message is received. Each time a process   */
/* starts or stops receiving the primary_ie or backup_ie subjects this   */
/* function will be called.                                              */
/* --------------------------------------------------------------------- */
static void T_ENTRY ReceiveMonSubjectSubscribeStatus
(
 T_IPC_CONN conn,
 T_IPC_CONN_PROCESS_CB_DATA data,
 T_CB_ARG arg
)
{
  T_IPC_MSG msg = data->msg;
  T_IPC_MT mt;
  T_STR subject_id;
  T_STR *client_names;
  T_INT4 subject_count;
  T_REAL8 WallTime;
  T_STR TimeStr;

  /* Get the current wall clock time and convert it to a string */
  WallTime = TutGetWallTime();
  TutTimeCvtNumToStr(TimeCvtr, WallTime, &TimeStr);
  
  /* --------------------------------------------------------- */
  /* Parse the subject status message for the subject id and   */
  /* the number of processes subscribed to it.                 */
  /* --------------------------------------------------------- */
  TipcMsgSetCurrent(msg, 0);
  TipcMsgNextStr(msg, &subject_id);
  TipcMsgNextStrArray(msg, &client_names, &subject_count);

  TutOut("%s => %d processes subscribed to <%s>\n", 
	 TimeStr, subject_count, subject_id );

  if (T_STREQ(subject_id,"primary_ie"))
    PrimaryIECount = subject_count;
  else if (T_STREQ(subject_id,"backup_ie"))
    BackupIECount = subject_count;

  /* ----------------------------------------------------------------------- */
  /* There are four main cases that need to be covered. They are listed      */
  /* below followed by the actions to be taken when the case is encountered: */
  /*    CASE 1: Neither primary or backup IE is running                      */
  /*		start primary IE                                             */
  /*		start backup IE                                              */
  /*    CASE 2: Both primary and backup IEs are running                      */
  /*		output message that all is OK                                */
  /*    CASE 3: Primary IE fails; backup IE is running                       */
  /* 		set server_msg_send option in backup IE to TRUE              */
  /*		Have backup IE join the primary_ie subject                   */
  /*            Have backup IE leave the backup_ie subject                   */
  /*            (This will then cause CASE 4 to occur);                      */
  /*    CASE 4: Backup IE fails, primary IE is running                       */
  /*		Restart backup IE                                            */
  /* ----------------------------------------------------------------------- */
   
  /* Check if neither the Primary IE nor Backup IE have been started yet */
  if (PrimaryIECount == 0 && BackupIECount == 0) {
    TutOut("Both primary and backup IEs not yet started.\n");
    TutOut("Starting Primary RTie...\n");
    TutSystem("start_primary_ie &");
    TutOut("Starting Backup RTie...\n");
    TutSystem("start_backup_ie &");
  } /* if */
  else if (PrimaryIECount == 1 && BackupIECount == 1) {
    TutOut("Both primary and backup IEs up and running!\n");
  } /* else if */
  else if (T_STREQ(subject_id,"primary_ie")) {
    if (PrimaryIECount == 1 && BackupIECount <= 0) {
      TutOut("Primary IE up and running! Waiting on backup...\n");
    } /* if */
    else if (PrimaryIECount == 0 && BackupIECount <= 0) {
      TutOut("No primary IE yet; No report received yet from backup IE\n");
    } /* else if */
    /* Check if we have lost the primary RTie */
    else if (PrimaryIECount == 0 && BackupIECount == 1 ){
      TutOut("Primary RTie has failed!\n");
      TutOut("Switching the backup RTie to be primary...\n");
      mt = TipcMtLookupByNum(T_MT_CONTROL);
      TipcSrvMsgWrite("backup_ie", mt, TRUE, 
                      T_IPC_FT_STR, "setopt server_msg_send TRUE", NULL);
      TipcSrvMsgWrite("backup_ie", mt, TRUE,
                      T_IPC_FT_STR, "subscribe primary_ie", NULL);
      TipcSrvMsgWrite("backup_ie", mt, TRUE,
                      T_IPC_FT_STR, "unsubscribe backup_ie", NULL);
    } /* else if */
    else {
      TutOut("We have an irregular number of RTie processes running!\n");
      TutOut("     Number of primary IEs : %d\n", PrimaryIECount);
      TutOut("     Number of backup IEs  : %d\n", BackupIECount);
    } /* else */
  } /* else if (primary_ie reported) */
  else if (T_STREQ(subject_id,"backup_ie")) {
    if (PrimaryIECount <= 0 && BackupIECount == 1) {
      TutOut("Backup IE up and running! Waiting on primary...\n");
    } /* if */
    else if (PrimaryIECount <= 0 && BackupIECount == 0) {
      TutOut("No backup IE yet; No report received yet from primary IE\n");
    } /* else if */
    /* Check if we have lost the backup RTie */
    else if (PrimaryIECount == 1 && BackupIECount == 0){
      TutOut("Backup RTie is down!\n");
      TutOut("Starting a new backup RTie!\n");
      TutSystem("start_backup_ie &");
    } /* else if */
    else {
      TutOut("We have an irregular number of RTie processes running!\n");
      TutOut("     Number of primary IEs : %d\n", PrimaryIECount);
      TutOut("     Number of backup IEs  : %d\n", BackupIECount);
    } /* else */
  } /* else if (backup_ie reported) */

  TutOut("================================\n");
     
} /* ReceiveMonSubjectSubscribeStatus */

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
  if (argc != 2) {
    printf("Usage: monitor <project>\n");
    T_C_EXIT(T_EXIT_FAILURE); 
  } /* if */

  /* Copy the command line arguments */
  ProjectName = argv[1];
  TutOut("Monitoring project <%s>...\n", ProjectName);

  /* -------------------- */
  /* Set the project name */
  /* -------------------- */
  option = TutOptionLookup("project");
  TutOptionSetEnum(option, ProjectName);

  /* ----------------------- */
  /* Connect to the RTserver */
  /* ----------------------- */
  if (!TipcSrvCreate(T_IPC_SRV_CONN_FULL)) {
    TutOut("monitor: could not connect to RTserver.\n");
    T_C_EXIT(T_EXIT_FAILURE);
  } /* if */

  /* ----------------------------------------------- */
  /* Setup callback to catch subject status messages */
  /* ----------------------------------------------- */
  mt = TipcMtLookupByNum(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS);
  TipcSrvProcessCbCreate(mt, ReceiveMonSubjectSubscribeStatus, NULL);

  /* ------------------------------------------------ */
  /* Setup callback to receive unwanted message types */
  /* ------------------------------------------------ */
  TipcSrvDefaultCbCreate(DefaultReceiveFunc, NULL);

  /* ----------------------------- */
  /* Start "watching" the subjects */
  /* ----------------------------- */
  TipcMonSubjectSubscribeSetWatch("primary_ie", TRUE);
  TipcMonSubjectSubscribeSetWatch("backup_ie", TRUE);

  /* Get a time converter for the FULL format */
  TimeCvtr = TutTimeCvtLookup("full");

  /* ----------------------------------------------------------------------- */
  /* Read messages from RTserver and call the registered callbacks as needed */
  /* ----------------------------------------------------------------------- */
  TipcSrvMainLoop(T_TIMEOUT_FOREVER);

  TutOut(T_EXIT_SUCCESS);

} /* main */





