/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tmsgsize.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmsg.h>
#include <rtworks/tmsgsize.h>

namespace SmartSockets {

/* ========================================================================= */
/*..TipcMsgManipSize::TipcMsgManipSize -- TipcMsgManipSize constructor */
TipcMsgManipSize::TipcMsgManipSize(SetSizeManip setSizeFunc, int4 value)
:_setSizeFunc(setSizeFunc),
 _getSizeFunc(NULL),
 _value(value),
 _value_return (0)
{ 
}

/* ========================================================================= */
/*..TipcMsgManipSize::TipcMsgManipSize -- TipcMsgManipSize constructor */
TipcMsgManipSize::TipcMsgManipSize(GetSizeManip getSizeFunc, int4& value)
:_setSizeFunc(NULL),
 _getSizeFunc(getSizeFunc),
 _value(0),
 _value_return (&value)
{ 
}



/* ========================================================================= */
/*..operator<< --  call a TipcMsgManipSize method while inserting into a  */
/*..  TipcMsg object */
TipcMsg& operator<<(TipcMsg& msg, const TipcMsgManipSize& manip)
{
  (*(manip._setSizeFunc))(msg,manip._value);
  return msg;
}


/* ========================================================================= */
/*..operator>> --  call a TipcMsgMainip method while extracting from a  */
/*..  TipcMsg object */
const TipcMsg& operator>>(const TipcMsg& msg, const TipcMsgManipSize& manip)
{
  (*(manip._getSizeFunc))(msg,*(manip._value_return));
  return msg;
}

/* ========================================================================= */
/*..SetSize --  invoke the SetSize TipcMsg object manipulator */
TipcMsgManipSize setSize(int4 size)
{
  return( TipcMsgManipSize((SetSizeManip)(TipcMsg::setSize), size) );
}


/* ========================================================================= */
/*..GetSize --  invoke the GetSize TipcMsg object manipulator */
TipcMsgManipSize getSize(int4& size_return)
{
  return( TipcMsgManipSize((GetSizeManip)(TipcMsg::getSize), size_return) );
}

} //namespace SmartSockets
