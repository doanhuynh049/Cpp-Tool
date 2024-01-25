/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tutil.h#1 $
 */

#ifndef _T_UTILITIES_H_
#define _T_UTILITIES_H_

#include <rtworks/ipc.h>
#include <rtworks/tex.h>
#include <rtworks/types.h>


namespace SmartSockets {
  
  /**
   * Inherits from Exception so users can trap Utilties related
   * exceptions. Offers no additional functionality.
   *
   * @see Exception, Utilties 
   */
  class T_IPCX_API UtilitiesException : public Exception {

  public:
      /**
       * Constructor.
       */
      UtilitiesException () {}
  
      /**
       * Destructor.
       */
      virtual ~UtilitiesException () throw() {}
  };

  /**
   * A non-instantiable class with static methods for
   * various SmartSockets utility options
   */
  class T_IPCX_API Utilities {

  private:
      /**
       * Not implemented to prevent users instantiating this class.
       */
      Utilities ();

      /**
       * Not implemented to prevent users instantiating this class.
       */
      ~Utilities () throw();
      
 
  public:
      /**
       * Submits a file to the process' command interface for execution. Each
       * line of the file is executed with parseCommandString().
       * @param file is the command file to be parsed.
       * This method does not search for file_name in any directories
       * such as the
       * SmartSockets standard directory, but simply uses file_name verbatim.
       * @warning This method reads the file one line at a time using the 
       * fgets C function.
       * Multi-line comments, multi-line commands, and commands longer than 
       * 4,094 characters are not allowed.
       * @throw UtilitiesException if an error occurs reading the file.
       */
      static void parseCommandFile (const char * file) 
                        throw (UtilitiesException);
  
      /**
       * Submits a string to the command interface for execution. 
       * If there is a syntax error in command_string, a message is displayed 
       * on standard out. If the command is successfully executed, and
       * the Command_Feedback option is set to always , command
       * feedback is displayed on the output window. If Command_Feedback is 
       * set to never, or interactive, no feedback is generated.
       * @param command_string is the command string to be parsed.
       */
      static void parseCommandString (const char * command_string)
                        throw ();
      
      /**
       * Submits a string to the command interface for execution.  This method
       * behaves exactly like parseCommandString(), except for the feedback of
       * successful commands.  Unlike parseCommandString() -- which will not 
       * write any feedback to standard out if the 'Command_feedback' option is 
       * set to 'interactive' -- this method will provide positive feedback if
       * the option 'Command_Feedback' is set to 'interactive'.
       * 
       * If there is a syntax error in command_string, a message is displayed 
       * on standard out. If the command is successfully executed, and
       * the Command_Feedback option is set to always or interactive, command
       * feedback is displayed on the output window. If Command_Feedback is 
       * set to never, no feedback is generated.
       * To submit a non-interactive string to the command interface use the
       * method parseCommandString().  
       * 
       * @param command_string is the typed command string to be parsed.
       */
      static void parseTypedCommandString (const char * command_string)
                        throw ();
  
      /**
       * Returns the directory name where all SmartSockets local sockets are
       * located. For Windows systems, this does not apply. On UNIX, this is
       * usually the directory
       * /tmp/rtworks. On OpenVMS, this is usually the directory RTSOCKETS:
       * [SOCKETS.node]
       * where node is the node name of the computer.
       * TutGetSocketDir also uses mkdir to create the directory if it does not
       * already exist.
       * When running SmartSockets, it is often necessary to create files (such
       * as UNIX domain
       * sockets) that are world-readable and world-writable. To avoid forcing
       * users to create
       * world-writable directories under their own home directory,
       * getSocketDir() is used instead.
       * @warning The string returned by this method points to read-only
       * storage and should not be modified.
       * @returns the socket directory SmartSockets uses.
       */
      static const char * getSocketDir ()
                        throw ();
  
      /**
       * Retrieves the current version number of SmartSockets as a string of the
       * form Version a.bc mm/dd/yy status, where a is the major release number,
       * b is the minor
       * release number, c is an optional maintenance release number, mm/dd/yy
       * is an optional date,
       * and status is an optional status string.
       * @warning The string returned by this method points to read-only
       * storage and should not be modified.
       * @returns SmartSockets version name.
       */
      static const char * getVersionName ()
                        throw ();
  
      /**
       * Returns an integer that represents the software version number.
       * SmartSockets Version X.Y would have a version number of XY0 . For
       * example, version 3.0 corresponds to version number 300.
       * @returns SmartSockets version number.
       */
      static int4 getVersionNumber ()
                        throw ();

      /**
       * Initializes the thread API and turns on internal thread 
       * synchronization calls within the utility and IPC libraries. Programs 
       * that call SmartSockets API functions from more than one thread must 
       * first call this function, even if they do not use any of the other
       * thread API functions. This protects the integrity of the library's 
       * internal data structures.
       *
       * Single-threaded programs do not need to call initThreads, and indeed 
       * should avoid doing so for optimum performance. The added overhead of 
       * internal thread synchronization calls is not needed for 
       * single-threaded programs.
       * @throw UtilitiesException if the operating system 
       * does not support threads
       */
      static void initThreads() throw (UtilitiesException);

      /**
       * Determines whether or not initThreads has been called.
       */
      static bool isThreadEnabled() throw ()
      {
         return TipcThreadEnabled();
      }

			/**
			 * This method can be used to print information to the user
			 * Simillar to the printf and TutOut in SmartSockets.
			 */
			static void out(const char * format_str, ...) throw ();

  };

} //namespace SmartSockets

#endif //_T_UTILITIES_H_
