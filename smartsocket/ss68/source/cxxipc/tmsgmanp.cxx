/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tmsgmanp.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmsg.hxx>
#include <rtworks/tmsgmanp.hxx>

/* ========================================================================= */
/*..TipcMsgManip::TipcMsgManip -- TipcMsgManip constructor */
TipcMsgManip::TipcMsgManip(TipcMsg& (*set_f)(TipcMsg &m, T_INT4 value), 
			   T_INT4 value)
{ 
  _set_fp = set_f; 
  _value = value;
}

/* ========================================================================= */
/*..TipcMsgManip::TipcMsgManip -- TipcMsgManip constructor */
TipcMsgManip::TipcMsgManip(TipcMsg& (*get_f)(TipcMsg &m, T_INT4 *value_return),
			   T_INT4 *value_return)
{ 
  _get_fp = get_f; 
  _value_return = value_return;
}


/* ========================================================================= */
/*..operator<< --  call a TipcMsgManip method while inserting into a  */
/*..  TipcMsg object */
TipcMsg& operator<<(TipcMsg& m, const TipcMsgManip& mm)
{
  (*(mm._set_fp))(m,mm._value);
  return m;
}


/* ========================================================================= */
/*..operator>> --  call a TipcMsgMainip method while extracting from a  */
/*..  TipcMsg object */
TipcMsg& operator>>(TipcMsg& m, const TipcMsgManip& mm)
{
  (*(mm._get_fp))(m,mm._value_return);
  return m;
}


/* ========================================================================= */
/*.._setsize --  set the size attribute of a TipcMsg object */
TipcMsg& _setsize(TipcMsg& m, T_INT4 size)
{
  m.size(size);
  return m;
}


/* ========================================================================= */
/*..SetSize --  invoke the SetSize TipcMsg object manipulator */
TipcMsgManip SetSize(T_INT4 size)
{
  return( TipcMsgManip(_setsize, size) );
}


/* ========================================================================= */
/*.._getsize --  get the size attribute of a TipcMsg object */
TipcMsg& _getsize(TipcMsg& m, T_INT4 *size_return)
{
  *size_return = m.size();
  return m;
}


/* ========================================================================= */
/*..GetSize --  invoke the GetSize TipcMsg object manipulator */
TipcMsgManip GetSize(T_INT4 *size_return)
{
  return( TipcMsgManip(_getsize, size_return) );
}
