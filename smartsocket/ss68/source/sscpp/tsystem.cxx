/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tsystem.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tsystem.h>

namespace SmartSockets {

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
void System::runCommand(const char * command) throw (SystemException)
{
  int4 retVal = TutSystem(const_cast<char*>(command));


  if (retVal != 0)
  {
    throw (SystemException (T_ERR_C,retVal));
  }
}

/**
* Gets the current system time.
* @return floating-point number containing current    
* computer wall clock time in the number of seconds since 
* midnight January 1, 1970.
*/
real8 System::getTime() throw ()
{
  return TutGetWallTime();
}

/**
* Sleeps the thread for timeout number of seconds.  This
* is equivalent to calling TutSleep().  Note that it is 
* recommended that Thread::sleep() be used whenever using 
* TIBCO's Thread class. 
* @param timeout number of seconds to sleep.
* @exception SystemException thrown if an invalid timeout 
* is passed, or if an operating system error occurs.
*/
void System::sleep (real8 timeout) throw (SystemException)
{
	if (!TutSleep(timeout))
    throw SystemException();
}

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
void System::exit(int4 exitCode) throw ()
{
  TutExit(exitCode);
}

/** 
* Gets the integer format for this process.  This is 
* equivalent to calling TutGetIntFormat()
* @return System::INT_BIG_ENDIAN or 
* System::INT_LITTLE_ENDIAN.
* @see INT_BIG_ENDIAN
* @see INT_LITTLE_ENDIAN
*/
int4 System::getIntFormat() throw ()
{
  return TutGetIntFormat();
}


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
int4 System::getFloatingPointFormat() throw ()
{
  return TutGetRealFormat();
}

} //namespace SmartSockets
