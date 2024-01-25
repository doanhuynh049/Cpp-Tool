/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmsgname.h#1 $
 */

#ifndef T_MSGMANIP_NAME_H_
#define T_MSGMANIP_NAME_H_

 
namespace SmartSockets
{
typedef void (* SetNameManip) (
       TipcMsg&, const char* );

typedef void (* GetNameManip) (
       const TipcMsg& , const char*& );


class T_IPCX_API TipcMsgManipName 
{

private:

  GetNameManip _getNameFunc;
  SetNameManip _setNameFunc;

  const char* _name;
  char** _nameReturn;

public:

  TipcMsgManipName(SetNameManip setNameFunc, const char* value);
  TipcMsgManipName(GetNameManip getNameFunc, const char*& value);


  friend  T_IPCX_API TipcMsg& operator<<(TipcMsg& msg, const TipcMsgManipName& mm);
  friend  T_IPCX_API const TipcMsg& operator>>(const TipcMsg& msg, const TipcMsgManipName& mm);
    
};

TipcMsgManipName T_IPCX_API setName(const char* name);
TipcMsgManipName T_IPCX_API getName(const char*& name);

T_IPCX_API TipcMsg& operator<<(TipcMsg& msg, const TipcMsgManipName& mm);
T_IPCX_API const TipcMsg& operator>>(const TipcMsg& msg, const TipcMsgManipName& mm);

}// end of namespace

#endif //T_MSGMANIP_NAME_H_
