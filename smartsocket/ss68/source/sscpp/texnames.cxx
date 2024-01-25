/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/texnames.cxx#1 $
 */

#include "includes.h"
#include <rtworks/texnames.h>

namespace SmartSockets {

/**
* Gets an error string based on what
* the SmartSockets error code was set to.  This method 
* will have similar functionality to TutErrNumToStr().
* 
* @param ssErrNum the SmartSockets error number.
* @return the string associated with the SmartSockets
*  Error.
*/
const char* ExceptionNames::getErrorString(int4 ssErrNum)
{
	return TutErrNumToStr(ssErrNum);
}


/**
* Creates an error string associated with the given
* SmartSockets error code.  This method 
* will have similar functionality to TutErrStrCreate().
* 
* @param ssErrNum the SmartSockets error number.
* @param errStr the string associated with the error code.
* 
*/
void ExceptionNames::setErrorString(int4 ssErrNum, const char* errStr)
{
	TutErrStrCreate(ssErrNum, const_cast <char*>(errStr));
}

} //namespace SmartSockets
