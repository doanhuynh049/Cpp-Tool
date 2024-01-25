/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tutil.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tutil.h>

namespace SmartSockets {

/**
 * Maps to TutCommandParseFile.
 * @param file is the command file to be parsed.
 */
void Utilities::parseCommandFile (const char * file)
      throw (UtilitiesException)
{
  if (!TutCommandParseFile(const_cast<char *> (file)))
    throw UtilitiesException();
}

/**
 * Maps to TutCommandParseString.
 * @param command_string is the command string to be parsed.
 */
void Utilities::parseCommandString (const char * command_string)
      throw ()
{
  TutCommandParseStr(const_cast<char *> (command_string));
}

/**
 * Maps to TutCommandParseTypedString.
 * @param command_string is the typed command string to be parsed.
 */
void Utilities::parseTypedCommandString (const char * command_string)
      throw ()
{
  TutCommandParseTypedStr(const_cast<char *> (command_string));
}

/**
 * Maps to TutGetSocketDir.
 * @returns the socket directory Smart Sockets uses.
 */
const char * Utilities::getSocketDir () 
      throw ()
{
  return TutGetSocketDir();
}

/**
 * Maps to TutGetVersionName.
 * @returns Smart Sockets version name.
 */
const char * Utilities::getVersionName ()
      throw ()
{
  return TutGetVersionName();
}

/**
 * Maps to TutGetVersionNumber.
 * @returns Smart Sockets version number.
 */
int4 Utilities::getVersionNumber ()
      throw ()
{
  return TutGetVersionNumber();
}

/**
 * Initialize the threads
 * @returns FALSE if the operating system 
 * does not support threads
 */
void Utilities::initThreads() throw (UtilitiesException)
{
  if(!TipcInitThreads())
    throw UtilitiesException();
}

void Utilities::out(const char * format_str, ...) throw ()
{
	va_list var_arg_list;

	va_start(var_arg_list, format_str);
	TutVOut(const_cast<char *> (format_str), var_arg_list);
	va_end(var_arg_list);
}

} //namespace SmartSockets
