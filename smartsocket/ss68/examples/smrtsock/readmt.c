/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/readmt.c#1 $
 */

/* --------------------------------------------------------------------- */
/* readmt.c                                                              */
/* USAGE                                                                 */
/*    readmt.x <filename>                                                */
/*                                                                       */
/* This program reads in a file and create a message type for each of    */
/* the one line descriptions, this allows a set of programs to share     */
/* one set of message type. The file format is as follows:               */
/*                                                                       */
/*   name  number  priority  delivery_mode  user-defined_prop  grammar   */
/*                                                                       */
/* for example:                                                          */
/*                                                                       */
/* vector3  1000     0        best_effort         0     "int4 int4 int4" */
/* --------------------------------------------------------------------- */
#include <rtworks/ipc.h>

/* ========================================================================= */
/*..MtCreateFromFile - create message types from a file */ 
T_BOOL MtCreateFromFile
(
 T_STR file_name
)
{
  T_IPC_MT mt;
  FILE *file;
  T_STRING mt_name;
  T_STRING delivery_name;
  T_STRING grammar;
  T_STRING tmp;
  T_INT4 mt_num;
  T_INT4 priority;
  T_INT4 user_prop;
  T_IPC_DELIVERY_MODE delivery_mode;
  /*
   * check parameters
   */
  if (T_NULL == file_name) {
    return T_FALSE;
  } /* if */
  /*
   * Open the file in read only mode.
   */
  TutOut("Opening file <%s> for reading.\n", file_name);
  if (T_NULL == (file = TutFOpen(file_name, "r"))) {
    TutOut("Could not open file\n");
    return T_FALSE;
  } /* if */
  /*
   * Loop until the end of the file, reading in each line, and creating a
   * message type.
   */
  while(!feof(file)){
    fscanf(file, "%s %d %d %s %d \"%[^\"] %[^\n]\n", mt_name, &mt_num, 
	   &priority, delivery_name, &user_prop, grammar, tmp);
  
    TutOut("Creating message type <%s>\n", mt_name);
    if (T_NULL == (mt = TipcMtCreate(mt_name, mt_num, grammar))) {
      TutOut("Could not create message type <%d>\n", mt_num);
      continue;
    } /* if */
  
    if (!TipcSrvLogAddMt(T_IPC_SRV_LOG_DATA, mt)) {
      TutOut("Could not add message type <%d> to log.\n", mt_num);
      continue;
    } /* if */

    if (!TipcMtSetPriority(mt, priority)) {
      TutOut("Could not set priority <%d> for message type <%d>\n", 
	     priority, mt_num);
      continue;
    } /* if */

    if (!TipcMtSetUserProp(mt, user_prop)) {
      TutOut("Could not set user property <%d> for message type <%d>\n", 
	     user_prop, mt_num);
      continue;
    } /* if */

    if (!TipcStrToDeliveryMode(delivery_name, &delivery_mode)) {
      TutOut("Delivery mode <%s> is not valid.\n", delivery_name);
      continue;
    } /* if */

    if (!TipcMtSetDeliveryMode(mt, delivery_mode)) {
      TutOut("Could not set delivery mode <%s> for message type <%d>\n", 
	     delivery_name, mt_num);
      continue;
    } /* if */
  } /* while */
  /*
   * Close the file
   */
  fclose(file);

  return T_TRUE;
} /* MtCreateFromFile */

/* ========================================================================= */
/*..main */
int main
(
 int argc,
 char **argv
)
{
  T_OPTION option;

  if (2 != argc) {
    TutOut("Usage: %s <filename>\n", argv[0]);
    TutExit(T_EXIT_FAILURE);
  } /* if */

  option = TutOptionLookup("project");
  if (T_NULL == option) {
    TutOut("Could not lookup the option named project\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  if( !TutOptionSetEnum(option, "example_app")) {
    TutOut("Could not set the option named project\n");
    TutExit(T_EXIT_FAILURE);
  } /* if */

  if (!MtCreateFromFile(argv[1])) {
    TutOut("Failed to create message types from file <%s>\n", argv[1]);
    TutExit(T_EXIT_FAILURE);
  } /* if */


  return T_EXIT_SUCCESS;

} /*main*/


