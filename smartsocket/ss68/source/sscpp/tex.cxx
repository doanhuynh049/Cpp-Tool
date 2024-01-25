/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tex.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tex.h>

namespace SmartSockets {


  /**
  * This constructor will be used to create an exception 
  * using TutErrNumGet(), TutErrNumGetOs(),
  * TutErrNumGetSocket(), TutErrNumGetC to populate the error 
  * numbers and set the error string based on the error 
  * number.            
  */
  Exception::Exception ()
  {
    _osErrNum = 0;
    _socketErrNum = 0;
    _cErrNum = 0;

    // Get the global error number
    _errNum = TutErrNumGet();

    // Get the error string associated with the 
    // global error number
    _errorString = TutErrNumToStr(_errNum);

    // Get the specific error code such as OS, Socket or
    // C error codes

    if (T_ERR_OS == _errNum)
      _osErrNum = TutErrNumGetOs();
    else if (T_ERR_SOCKET == _errNum)
      _socketErrNum = TutErrNumGetSocket();
    else if (T_ERR_C == _errNum)
      _cErrNum = TutErrNumGetC();
    /*
      switch (_errNum)
      {
        case T_ERR_OS:
          _osErrNum = TutErrNumGetOs();
          break;
    
        case T_ERR_SOCKET:
          _socketErrNum = TutErrNumGetSocket();
          break;
    
        case T_ERR_C:
          _cErrNum = TutErrNumGetC();
          break;
      }  // switch ...
    */
  }

  /**
   * This constructor will be used to create an exception 
   * using the arguments to populate the error 
   * numbers.  This is used for any exceptions created 
   * specifically for C++ that don't have a C equivalent 
   * Global error number.  Note that the ExceptionNames
   * class should be used to associate any new C++ specific 
   * error codes with its string.  The string returned by 
   * the what() method will be obtained that way.
   *  
   * @param errNum     the Global error code
   * @param osErrNum     the Operating system error code.
   * @param socketErrNum the Socket error code.
   * @param cErrNum      the C error code.
   * @see ExceptionNames
   * @see what()
   */

  Exception::Exception 
  (
  int4 errNum
  )
  :_errNum(errNum)
  {
    // Get the error string associated with the 
    // global error number
    _errorString = TutErrNumToStr(errNum);
    TutErrNumSet(errNum);

    // Get the specific error code such as OS, Socket or
    // C error codes

    if (T_ERR_OS == _errNum)
      _osErrNum = TutErrNumGetOs();
    else if (T_ERR_SOCKET == _errNum)
      _socketErrNum = TutErrNumGetSocket();
    else if (T_ERR_C == _errNum)
      _cErrNum = TutErrNumGetC();
  }

  /** 
  * Destructor
  */
  Exception::~Exception () throw()
  {
  }

  /**
  * Gets an error string based on what
  * the Global error code was set to.  To create new
  * error codes, please use the ExceptionNames class.
  * @return The string associated with the exception.
  * @see ExceptionNames
  */
  const char* Exception::what () const throw()
  {
    return _errorString;
  }

  /**
  * Gets the Global error code set when the exception
  * was thrown.
  */
  int4 Exception::getErrNum () const
  {

    return _errNum;
  }

  /**
  * Gets the Operating System error code set when the 
  * exception was thrown.
  */
  int4 Exception::getOSErrNum () const
  {
    return _osErrNum;
  }

  /**
  * Gets the Socket error code set when the 
  * exception was thrown.
  */
  int4 Exception::getSocketErrNum () const
  {
    return _socketErrNum;
  }

  /**
  * Gets the C code set when the 
  * exception was thrown.
  */
  int4 Exception::getCErrNum () const
  {
    return _cErrNum;
  }

} //namespace SmartSockets
