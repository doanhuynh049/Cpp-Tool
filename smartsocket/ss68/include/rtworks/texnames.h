/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/texnames.h#1 $
 */

#ifndef T_EXCEPTIONNAMES_H_
#define T_EXCEPTIONNAMES_H_

#include <rtworks/ipc.h>
#include <rtworks/tex.h>
#include <rtworks/types.h>

namespace SmartSockets {

  /**
   * Class used to create or modify the string associated with
   * SmartSockets error numbers.  This class will be used to
   * create any C++ specific exception error codes, or to 
   * customize error messages for existing error codes.
   */
  class T_IPCX_API ExceptionNames : public Exception
  {
    public:
     /**
      * Gets an error string based on what
      * the SmartSockets error code was set to.  This method 
      * will have similar functionality to TutErrNumToStr().
      * 
      * @param ssErrNum the SmartSockets error number.
      * @return the string associated with the SmartSockets
      *  Error.
      */
      static const char* getErrorString(int4 ssErrNum);
  
     /**
      * Creates an error string associated with the given
      * SmartSockets error code.  This method 
      * will have similar functionality to TutErrStrCreate().
      * 
      * @param ssErrNum the SmartSockets error number.
      * @param errStr the string associated with the error code.
      * 
      */
      static void setErrorString(int4 ssErrNum, const char* errStr);
  
  };
}

#endif // T_EXCEPTIONNAMES_H_
