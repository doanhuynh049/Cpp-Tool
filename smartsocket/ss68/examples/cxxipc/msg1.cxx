/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/cxx/msg1.cxx#1 $
 */

// msg1.cxx -- messages example using Append and Next

#include <iostream.h>
#include <rtworks/cxxipc.hxx>

int main()
{

  T_STR str_val;
  T_REAL8 real8_val;

  cout << "Create the message." << endl;
  TipcMsg msg(T_MT_NUMERIC_DATA, (T_STR)"_saratoga", (T_STR)"thermal");
  if (!msg) {
    cout << "Could not create message." << endl;
    return T_EXIT_FAILURE;
  }
  
  cout << "Append fields." << endl;
  if (!msg.Append((T_STR)"voltage")) {
    cout << "Could not append first field." << endl;
    return T_EXIT_FAILURE;
  }

  // A cast to T_REAL8 here is to insure that the C++ compiler
  // selects the correct overloaded Append member function.
  if (!msg.Append((T_REAL8)33.4534)) {
    cout << "Could not append second field." << endl;
    return T_EXIT_FAILURE;
  }

  if (!msg.Append((T_STR)"switch_pos")) {
    cout << "Could not append third field." << endl;
    return T_EXIT_FAILURE;
  }

  if (!msg.Append((T_REAL8)0.0)) {
    cout << "Could not append fourth field." << endl;
    return T_EXIT_FAILURE;
  }
  
  cout << "Access fields." << endl;
  if (!msg.Current(0)) {
    cout << "Could not set current field to first field." << endl;
    return T_EXIT_FAILURE;
  }
  
  if (!msg.Next(&str_val)) {
    cout << "Could not read first field." << endl;
    return T_EXIT_FAILURE ;
  }
		
  if (!msg.Next(&real8_val)) {
    cout << "Could not read second field." << endl;
    return T_EXIT_FAILURE;
  }
  cout << str_val << " = " << TutRealToStr(real8_val) << endl;
  
  if (!msg.Next(&str_val)) {
    cout << "Could not read third field." << endl;
    return T_EXIT_FAILURE;
  }

  if (!msg.Next(&real8_val)) {
    cout << "Could not read fourth field." << endl;
    return T_EXIT_FAILURE;
  }
  cout << str_val << " = " << TutRealToStr(real8_val) << endl;

  // msg is destroyed when TipcMsg goes out of scope and
  // the TipcMsg destructor is called.
  return T_EXIT_SUCCESS;  // all done  

}  // main  
