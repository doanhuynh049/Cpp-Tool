/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/sscpp/msg1.cxx#1 $
 */

#include <rtworks/sscpp.h>

// msg1.cxx -- messages example using Append and Next

using namespace SmartSockets;

int main()
{

  char  *str_val = NULL;
  real8 real8_val;


  Utilities::out("Create the message.\n");
  TipcMsg msg(T_MT_NUMERIC_DATA, (const char*)"_saratoga", (const char*)"thermal");
  if (!msg) {
    Utilities::out("Could not create message.\n");
    return T_EXIT_FAILURE;
  }
  
  Utilities::out("Append fields.\n");
  try {
    msg.append((const char*)"voltage");
  }
  catch (TipcMsgException me) {
    Utilities::out("Could not append first field. %s\n", me.what());
    return T_EXIT_FAILURE;
  }

  // A cast to T_REAL8 here is to insure that the C++ compiler
  // selects the correct overloaded Append member function.
  try {
    msg.append((real8)33.4534);
  } 
  catch (TipcMsgException me) {
    Utilities::out("Could not append second field. %s\n", me.what());
    return T_EXIT_FAILURE;
  }

  try {
    msg.append((const char*)"switch_pos");
  }
  catch (TipcMsgException me) {
    Utilities::out("Could not append third field. %s\n", me.what());
    return T_EXIT_FAILURE;
  }

  try {
    msg.append((real8)0.0);
  }
  catch (TipcMsgException me) {
    Utilities::out("Could not append fourth field. %s\n", me.what());
    return T_EXIT_FAILURE;
  }
  
  Utilities::out("Access fields.\n");
  try {
    msg.setCurrent(0);
  }
  catch (TipcMsgException me) {
    Utilities::out("Could not set current field to first field.\n");
    return T_EXIT_FAILURE;
  }
  
  try {
    str_val = (char *)msg.nextString();
  } 
  catch (TipcMsgException me) {
    Utilities::out("Could not read first field. %s\n", me.what());
    return T_EXIT_FAILURE ;
  }
	
  try {
    real8_val = msg.nextReal8();
  } 
  catch (TipcMsgException me) {
    Utilities::out("Could not read second field. %s\n", me.what());
    return T_EXIT_FAILURE;
  }
  Utilities::out("%s = %s\n", str_val, TutRealToStr(real8_val));
  
  try {
    str_val = (char *)msg.nextString();  
  }
  catch (TipcMsgException me) {
    Utilities::out("Could not read third field. %s\n", me.what());
    return T_EXIT_FAILURE;
  }

  try {
    real8_val = msg.nextReal8();
  }
  catch (TipcMsgException me) {
    Utilities::out("Could not read fourth field. %s\n", me.what());
    return T_EXIT_FAILURE;
  }
  Utilities::out("%s = %s\n", str_val, TutRealToStr(real8_val));

  // msg is destroyed when TipcMsg goes out of scope and
  // the TipcMsg destructor is called.
  return T_EXIT_SUCCESS;  // all done  

}  // main  
