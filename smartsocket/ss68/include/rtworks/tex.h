/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tex.h#1 $
 */

#ifndef T_EXCEPTION_H_
#define T_EXCEPTION_H_

//get STL exception
#if !defined(T_OS_HPUX) && !defined(T_OS_IRIX)
#include <exception>
#include <iostream>
#endif

#include <rtworks/ipc.h>
#include <rtworks/types.h>

namespace SmartSockets {

  /**
   * Base Class for all exception classes.  All TIBCO 
   * SmartSockets C++ exceptions will be derived from 
   * this base class.
   */
#if !defined(T_OS_HPUX) && !defined(T_OS_LINUX) && !defined(T_OS_IRIX)
  class T_IPCX_API Exception : public std::exception
#else
  class T_IPCX_API Exception
#endif
  {
    int4 _errNum;
    int4 _osErrNum;
    int4 _socketErrNum;
    int4 _cErrNum;
    char * _errorString;

  public:

    /**
     * This constructor will be used to create an exception 
     * using TutErrNumGet(), TutErrNumGetOs(),
     * TutErrNumGetSocket(), TutErrNumGetC to populate the error 
     * numbers and set the error string based on the error 
     * number.            
     */
    Exception ();

    /**
     * This constructor will be used to create an exception 
     * using the arguments to populate the error 
     * numbers.  This is used for any exceptions created 
     * specifically for C++ that don't have a C equivalent 
     * SmartSockets error number.  Note that the ExceptionNames
     * class should be used to associate any new C++ specific 
     * error codes with its string.  The string returned by 
     * the what() method will be obtained that way.
     *  
     * @param errNum     the SmartSockets error code
     * @see ExceptionNames
     * @see what()
     */

    Exception 
    (
    int4 errNum
    );


    /** 
     * Destructor
     */
    virtual ~Exception () throw();

    /**
     * Gets an error string based on what
     * the SmartSockets error code was set to.  To create new
     * error codes, please use the ExceptionNames class.
     * @return The string associated with the exception.
     * @see ExceptionNames
     */
    virtual const char* what () const throw() ;

    /**
     * Gets the SmartSockets error code set when the exception
     * was thrown.
     */
    int4 getErrNum () const;

    /**
     * Gets the Operating System error code set when the 
     * exception was thrown.
     */
    int4 getOSErrNum () const;

    /**
     * Gets the Socket error code set when the 
     * exception was thrown.
     */
    int4 getSocketErrNum () const;

    /**
     * Gets the C code set when the 
     * exception was thrown.
     */
    int4 getCErrNum () const;
  };

  /**
   * Base class for all IPC exception classes, derived from Exception.
   * All TIBCO SmartSockets C++ IPC exceptions will be derived from 
   * this base class.
   *
   * @see Exception
   */
  class T_IPCX_API TipcException : public Exception
  {
    public:
      /**
       * Constructor.
       */
      TipcException() {}

      /**
       * Constructor.
       */
      TipcException(int4 errorNum) : Exception(errorNum) {}

      /**
       * Destructor.
       */
      ~TipcException() throw() {}
  };

} //namespace SmartSockets

#endif // T_EXCEPTION_H_
