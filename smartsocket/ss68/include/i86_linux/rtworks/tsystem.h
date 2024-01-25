/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tsystem.h#1 $
 */

#ifndef _T_SYSTEM_H_
#define _T_SYSTEM_H_

//get STL exception
#include <rtworks/ipc.h>
#include <rtworks/tex.h>
#include <rtworks/types.h>

namespace SmartSockets {

    /**
   * Inherits from Exception so users can trap System related
   * exceptions. Offers no additional functionality.
   *
   * @see Exception, System 
   */
    class T_IPCX_API SystemException : public Exception {

      private:
        int4 _runCommandErr;

      public:
        /**
         * Constructor.
         */
        SystemException ()
        : _runCommandErr(0)
        {
        }

        /**
         * Constructor that lets the error number be set.
         * @param errNum the SmartSockets error number
         * @param spawnError used by the System::runCommand() 
         * method to set the return value of non-successful
         * system calls.
         */
        SystemException (int4 errNum, int4 runCommandErr = 0)
        : Exception (errNum)
        , _runCommandErr(runCommandErr)
        {
        }

        /**
         * Allows one to set the RunCommand error code.  This
         * is used by the System::runCommand() method.
         */
        virtual void setRunCommandError (int4 errNum) throw()
        {
          _runCommandErr = errNum;
        }

        /**
         * Allows one to get the RunCommand error code.  This
         * is value is typically set by the System::runCommand()
         * method when a system call was not successful.
         */
        virtual int4 getRunCommandError () throw()
        {
          return _runCommandErr;
        }
 
        /**
         * Destructor.
         */
        virtual ~SystemException () throw() {}
    };




    /**
     * Class used to wrap miscellaneous System calls.
     */
    class T_IPCX_API System
    {
      public:
       /**
        * Passes a command to be executed by an OS command 
        * processor. 
        * 
        * This is useful for writing code that is portable between
        * OpenVMS, UNIX, and Windows. 
        *
        * The Bourne shell (/bin/sh) is used on UNIX, 
        * DCL is used on OpenVMS, and the Windows command prompt
        * is used on Windows. On MVS, TSO or SAS execution 
        * services ultimately processes the request. runCommand() 
        * returns the exit status of command (on OpenVMS the exit
        * status 1 is mapped to 0 to emulate UNIX semantics).
        * runCommand() on UNIX and MVS simply calls the function 
        * system. On OpenVMS, runCommand() calls system unless the 
        * command string ends with an ampersand (&) character. In 
        * this case, TutSystem emulates a UNIX shell by calling 
        * LIB$SPAWN with the NOWAIT bit set (on UNIX this is 
        * called running the command in the background). On 
        * Windows, TutSystem emulates UNIX to accomodate the 
        * limitations of COMMAND.COM on Windows. On MVS,
        * ending the command string with & does not submit the 
        * command for batch execution. 
        * This is equivalent to calling TutSystem().
        * 
        * @param command command to be executed by the OS.
        * @exception SystemException thrown if the exit code of a 
        * command is not zero.
        */
        static void runCommand(const char * command) throw (SystemException);
  
       /**
        * Gets the current system time.
        * @return floating-point number containing current    
        * computer wall clock time in the number of seconds since 
        * midnight January 1, 1970.
        */
        static real8 getTime() throw ();
  
       /**
        * Sleeps the thread for timeout number of seconds.  This
        * is equivalent to calling TutSleep().  Note that it is 
        * recommended that Thread::sleep() be used whenever using 
        * TIBCO's SmartSockets Thread class. 
        * @param timeout number of seconds to sleep.
        * @exception SystemException thrown if an invalid timeout 
        * is passed, or if an operating system error occurs.
        */
        static void sleep (real8 timeout) throw (SystemException);
  
       /**
        * Calls all exit handlers registered internally by
        * SmartSockets in the reverse order of
        * their registration and then calls the system call
        * exit(). If the process has registered any functions by
        * calling atexit(), these functions are called only after
        * the internal SmartSockets exit handlers functions have 
        * been called.  If the process calls the system call
        * exit() instead of TutExit(), the internal SmartSockets 
        * exit handlers are not called and the behavior of the 
        * process or application is unknown. The correct way to 
        * terminate a SmartSockets application or process is to 
        * call exit().
        * @param exitCode exit status returned to the operating 
        * system when the process exits.
        */
        static void exit(int4 exitCode) throw ();
  
       /** 
        * Gets the integer format for this process.  This is 
        * equivalent to calling TutGetIntFormat()
        * @return System::INT_BIG_ENDIAN or 
        * System::INT_LITTLE_ENDIAN.
        * @see INT_BIG_ENDIAN
        * @see INT_LITTLE_ENDIAN
        */
        static int4 getIntFormat() throw();
  
  
       /** 
        * Gets the floating point format for this process.  This 
        * is equivalent to calling TutGetRealFormat()
        * @return  System::REAL_DEC_D, System::REAL_DEC_G,
        * System::REAL_IEEE, or System::REAL_IBM_370.
        * @see REAL_DEC_D
        * @see REAL_DEC_G
        * @see REAL_IEEE
        * @see REAL_IBM_370 
        */
        static int4 getFloatingPointFormat() throw();
  
       /**
        * Integer format where the most significant byte is stored 
        * in the highest byte.
        */
        static const int4 INT_BIG_ENDIAN;
  
       /**
        * Integer format where the most significant byte is stored 
        * in the lowest byte.
        */
        static const int4 INT_LITTLE_ENDIAN;
  
       /**
        * Floating point format that is the default for 
        * OpenVMS VAX.
        */
        static const int4 REAL_DEC_D;
  
  
       /**
        * Floating point format that is the default 
        * for OpenVMS VAX.
        */
        static const int4 REAL_DEC_G;
  
       /**
        * Floating point format that is the default for most Unix 
        * and Windows OSs.
        */
        static const int4 REAL_IEEE;
  
       /**
        * Floating point format that is the default for MVS.
        */
        static const int4 REAL_IBM_370;    
  
      private:
       /**
        * Disallow this class from being instantiated by making this private
        * access and unimplemented.
        */
        System();
  
       /**
        * Disallow this class from being instantiated by making this private
        * access and unimplemented.
        */
        ~System() throw();
    };

}//namespace TSS

#endif //_T_SYSTEM_H_
