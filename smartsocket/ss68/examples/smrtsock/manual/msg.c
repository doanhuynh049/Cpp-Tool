/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/manual/msg.c#1 $
 */

/* From "Messages" chapter in the "SmartSockets User's Guide". */

/* msg.c -- messages example */

#include <rtworks/ipc.h>

/* ================================================================ */
/*..main -- main program */
int main()
{
  T_IPC_MT mt;
  T_IPC_MSG msg;
  T_STR str_val;
  T_REAL8 real8_val;

  TutOut("Create the message.\n");
  mt = TipcMtLookupByNum(T_MT_NUMERIC_DATA);
  if (mt == NULL) {
    TutOut("Could not look up NUMERIC_DATA msg type: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  msg = TipcMsgCreate(mt);
  if (msg == NULL) {
    TutOut("Could not create message: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Set the message properties.\n");
  if (!TipcMsgSetSender(msg, "/_conan_5415")) {
    TutOut("Could not set message sender: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgSetDest(msg, "/system/thermal")) {
    TutOut("Could not set message dest: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgSetPriority(msg, 2)) {
    TutOut("Could not set message priority: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgSetDeliveryMode(msg, T_IPC_DELIVERY_ALL)) {
    TutOut("Could not set message delivery mode: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgSetDeliveryTimeout(msg, 20.0)) {
    TutOut("Could not set message delivery timeout: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgSetLbMode(msg, T_IPC_LB_WEIGHTED)) {
    TutOut("Could not set message load balancing mode: error <%s>.\n",
						 TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgSetHeaderStrEncode(msg, TRUE)) {
    TutOut("Could not set message header str encode: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgSetUserProp(msg, 42)) {
    TutOut("Could not set message user-defined prop: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Append fields.\n");
  if (!TipcMsgAppendStr(msg, "voltage")) {
    TutOut("Could not append first field: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
		}
  if (!TipcMsgAppendReal8(msg, 33.4534)) {
    TutOut("Could not append second field: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgAppendStr(msg, "switch_pos")) {
    TutOut("Could not append third field: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgAppendReal8(msg, 0.0)) {
    TutOut("Could not append fourth field: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  TutOut("Access fields.\n");
  if (!TipcMsgSetCurrent(msg, 0)) {
    TutOut("Could not set current field: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgNextStr(msg, &str_val)) {
    TutOut("Could not read first field: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgNextReal8(msg, &real8_val)) {
    TutOut("Could not read second field: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  TutOut("%s = %s\n", str_val, TutRealToStr(real8_val));
  
  if (!TipcMsgNextStr(msg, &str_val)) {
    TutOut("Could not read third field: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  if (!TipcMsgNextReal8(msg, &real8_val)) {
    TutOut("Could not read fourth field: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  TutOut("%s = %s\n", str_val, TutRealToStr(real8_val));
  
  TutOut("Destroy the message.\n");
  if (!TipcMsgDestroy(msg)) {
    TutOut("Could not destroy message: error <%s>.\n",
	   TutErrStrGet());
    TutExit(T_EXIT_FAILURE);
  }
  
  return T_EXIT_SUCCESS; /* all done */
} /* main */
