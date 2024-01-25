/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tmsgname.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmsg.h>
#include <rtworks/tmsgname.h>

namespace SmartSockets 
{
/* ========================================================================= */
/*..TipcMsgManip::TipcMsgManip -- TipcMsgManip constructor */
TipcMsgManipName::TipcMsgManipName(SetNameManip setNameFunc, const char* name):
 _setNameFunc(setNameFunc),
 _getNameFunc(NULL),
 _name(name),
 _nameReturn(NULL)
{ 
}

/* ========================================================================= */
/*..TipcMsgManip::TipcMsgManip -- TipcMsgManip constructor */
/* ========================================================================= */
/*..TipcMsgManip::TipcMsgManip -- TipcMsgManip constructor */
TipcMsgManipName::TipcMsgManipName(GetNameManip getNameFunc, const char*& nameReturn):
 _setNameFunc(NULL),
 _getNameFunc(getNameFunc),
 _name(NULL),
 _nameReturn(const_cast<char**>(&nameReturn))
{ 
}


/* ========================================================================= */
/*..operator<< --  call a TipcMsgManip method while inserting into a  */
/*..  TipcMsg object */
TipcMsg& operator<<(TipcMsg& msg, const TipcMsgManipName& manip)
{
  (*(manip._setNameFunc))(msg,manip._name);
  return msg;
}


/* ========================================================================= */
/*..operator>> --  call a TipcMsgManipName method while extracting from a  */
/*..  TipcMsg object */
const TipcMsg& operator>>(const TipcMsg& msg, const TipcMsgManipName& manip)
{
  const char* arg = NULL;


  (*(manip._getNameFunc))(msg,arg);

  (*(manip._nameReturn)) = const_cast<char*>(arg);

  return msg;
}


 

/* ========================================================================= */
/*..SetSize --  invoke the SetSize TipcMsg object manipulator */
TipcMsgManipName setName(const char* name)
{
  return( TipcMsgManipName((SetNameManip)TipcMsg::setName, name) );
}




/* ========================================================================= */
/*..GetSize --  invoke the getName TipcMsg object manipulator */
TipcMsgManipName getName(const char*& nameReturn)
{
  return TipcMsgManipName((GetNameManip)TipcMsg::getName, nameReturn);
}

}// end of namespace.

