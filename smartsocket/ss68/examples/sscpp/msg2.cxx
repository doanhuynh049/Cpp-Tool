/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/sscpp/msg2.cxx#1 $
 */

#include <rtworks/sscpp.h>

// msg2.cxx -- messages example using insertion and extraction operators

using namespace SmartSockets;

int main()
{
  const char  *str_val;
  real8 real8_val;

  Utilities::out("Create the message.\n");

  TipcMsg msg(T_MT_NUMERIC_DATA, (const char*)"_saratoga_5415", (const char*)"thermal");
  if(!msg) {
    Utilities::out("Could not create message.\n");
    return T_EXIT_FAILURE;
  }

  Utilities::out("Append fields.\n");
  // A cast to real8 here is to insure that the C++ compiler
  // selects the correct overloaded insertion operator.
  try {
    msg << (const char*)"voltage" << (real8)33.4534
        << (const char*)"switch_pos" << (real8)0.0;
  }
  catch (TipcMsgException me) {
    Utilities::out("Could not append data fields to message. %s\n", me.what());
    return T_EXIT_FAILURE;
  }
  
  Utilities::out("Access fields.\n");
  try {
    msg.setCurrent(0);
  }
  catch (TipcMsgException me) {
    Utilities::out("Could not set current field to first field. %s\n", me.what());
    return T_EXIT_FAILURE;
  }

  try {
    msg >> str_val >> real8_val;
  }
  catch (TipcMsgException me) {
    Utilities::out("Could not read data fields from message. %s\n", me.what());
    return T_EXIT_FAILURE;
  }
  Utilities::out("%s = %s\n", str_val, TutRealToStr(real8_val));
  
  try {
    msg >> str_val >> real8_val;
  }
  catch (TipcMsgException me) {
    Utilities::out("Could not read data fields from message. %s\n", me.what());
    return T_EXIT_FAILURE;
  }
  Utilities::out("%s = %s\n", str_val, TutRealToStr(real8_val));

  // msg is destroyed when TipcMsg goes out of scope and
  // the TipcMsg destructor is called.
  return T_EXIT_SUCCESS;  // all done  

}  // main  
