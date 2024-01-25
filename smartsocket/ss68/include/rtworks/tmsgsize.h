/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmsgsize.h#1 $
 */

#ifndef T_MSGMANIP_H_
#define T_MSGMANIP_H_

#include <rtworks/types.h>
#include <rtworks/tmsg.h>


namespace SmartSockets {
 
typedef TipcMsg& (* SetSizeManip) (TipcMsg&, int4 size );

typedef const TipcMsg& (* GetSizeManip) (const TipcMsg&, int4& size );

	class T_IPCX_API TipcMsgManipSize {


	private:

    SetSizeManip _setSizeFunc;
    GetSizeManip _getSizeFunc;

		int4  _value;
	  int4 *_value_return;

	public:


		TipcMsgManipSize(SetSizeManip setSizeFunc, int4 value);
		TipcMsgManipSize(GetSizeManip getSizeFunc, int4 &value_return);

		friend T_IPCX_API TipcMsg& operator<<(TipcMsg& msg, const TipcMsgManipSize& manip);
		friend T_IPCX_API const TipcMsg&  operator>>(const TipcMsg& msg, const TipcMsgManipSize& manip);
	};

	TipcMsgManipSize T_IPCX_API setSize(int4  size);
	TipcMsgManipSize T_IPCX_API getSize(int4& size_return);

  T_IPCX_API TipcMsg& operator<<(TipcMsg& msg, const TipcMsgManipSize& manip);
  T_IPCX_API const TipcMsg& operator>>(const TipcMsg& msg, const TipcMsgManipSize& manip);

} //namespace SmartSockets

#endif //T_MSGMANIP_H_

