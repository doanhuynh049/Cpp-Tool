/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmsgmanp.hxx#1 $
 */

#include <rtworks/tmsg.hxx>

class T_IPCX_API TipcMsgManip {

private:
  TipcMsg& (*_set_fp)(TipcMsg& m, T_INT4  value);
  TipcMsg& (*_get_fp)(TipcMsg& m, T_INT4 *value_return);
  T_INT4  _value;
  T_INT4 *_value_return;

public:
  TipcMsgManip(TipcMsg& (*set_f)(TipcMsg& m, T_INT4 value), 
	       T_INT4 value);
  TipcMsgManip(TipcMsg& (*get_f)(TipcMsg& m, T_INT4 *value_return), 
	       T_INT4 *value_return);

  friend T_IPCX_API TipcMsg& operator<<(TipcMsg& m, const TipcMsgManip& mm);
  friend T_IPCX_API TipcMsg& operator>>(TipcMsg& m, const TipcMsgManip& mm);
    
};

TipcMsgManip T_IPCX_API SetSize(T_INT4  size);
TipcMsgManip T_IPCX_API GetSize(T_INT4 *size_return);



