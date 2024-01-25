/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/cxx/msg2.cxx#1 $
 */

// msg2.cxx -- messages example using insertion and extraction operators

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

int main()
{

  T_STR str_val;
  T_REAL8 real8_val;

  cout << "Create the message." << endl;
  TipcMsg msg(T_MT_NUMERIC_DATA, (T_STR)"_saratoga_5415", (T_STR)"thermal");
  if (!msg) {
    cout << "Could not create message." << endl;
    return T_EXIT_FAILURE;
  }

  cout << "Append fields." << endl;

  // A cast to T_REAL8 here is to insure that the C++ compiler
  // selects the correct overloaded insertion operator.
  msg << (T_STR)"voltage" << (T_REAL8)33.4534
      << (T_STR)"switch_pos" << (T_REAL8)0.0;
 
  if (!msg) {
    cout << "Could not append data fields to message" << endl;
    return T_EXIT_FAILURE;
  }
  
  cout << "Access fields." << endl;
  if (!msg.Current(0)) {
    cout << "Could not set current field to first field." << endl;
    return T_EXIT_FAILURE;
  }

  msg >> str_val >> real8_val;
  
  if (!msg) {
    cout << "Could not read data fields from message." << endl;
    return T_EXIT_FAILURE;
  }
  cout << str_val << " = " << TutRealToStr(real8_val) << endl;
  
  msg >> str_val >> real8_val;
  
  if (!msg) {
    cout << "Could not read data fields from message." << endl;
    return T_EXIT_FAILURE;
  }
  cout << str_val << " = " << TutRealToStr(real8_val) << endl;

  // msg is destroyed when TipcMsg goes out of scope and
  // the TipcMsg destructor is called.
  return T_EXIT_SUCCESS;  // all done  

}  // main  
