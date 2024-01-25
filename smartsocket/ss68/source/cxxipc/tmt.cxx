/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tmt.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmt.hxx>

/* ========================================================================= */
/*..TipcMt::TipcMt -- Create an empty TipcMt object for friend classes */
TipcMt::TipcMt()
{
  _status = T_TRUE;
  _mt = (T_IPC_MT)T_NULL;
}

/* ========================================================================= */
/*..TipcMt::TipcMt -- Create a TipcMt object with an existing C API */
/*..  T_IPC_MT struct. */
TipcMt::TipcMt(T_IPC_MT mt) 
{ 
  _status = T_TRUE;
  _mt = mt; 

  if (!_mt) {
    _status = T_FALSE;
  }
}


/* ========================================================================= */
/*..TipcMt::TipcMt -- Create a TipcMt object by message type name */
TipcMt::TipcMt(T_STR name)
{
  _status = T_TRUE;
  _mt = TipcMtLookup(name);

  if (!_mt) {
    _status = T_FALSE;
  }
}


/* ========================================================================= */
/*..TipcMt::TipcMt -- Create a TipcMt object by message type number */
TipcMt::TipcMt(T_INT4 num)
{
  _status = T_TRUE;
  _mt = TipcMtLookupByNum(num);

  if (!_mt) {
    _status = T_FALSE;
  }
}


/* ========================================================================= */
/*..TipcMt::TipcMt -- Create a TipcMt object with a new message type */
TipcMt::TipcMt(T_STR name, T_INT4 num, T_STR grammar)
{
  _status = T_FALSE;
  _mt = TipcMtCreate(name, num, grammar);

  if (_mt != T_NULL) {
    _status = T_TRUE;
  }
}


/* ========================================================================= */
/*..TipcMt::TipcMt -- Copy Constructor */
TipcMt::TipcMt(const TipcMt& mt) 
{ 
  _status = T_TRUE;
  _mt = mt._mt; 

  if (!_mt) {
    _status = T_FALSE;
  }
}


/* ========================================================================= */
/*..TipcMt::~TipcMt -- TipcMt destructor */
TipcMt::~TipcMt() 
{ 
  // Don't do this
  // TipcMtDestroy(_mt); 
}


/* ========================================================================= */
/*..TipcMt::Traverse -- traverse all message types */
T_PTR TipcMt::Traverse(T_IPC_MT_TRAV_FUNC func, T_PTR arg)
{
  return TipcMtTraverse(func, arg);
}


/* ========================================================================= */
/*..TipcMt::Create -- create a message type */
T_BOOL TipcMt::Create(T_STR name, T_INT4 num, T_STR grammar)
{
  _status = T_FALSE;
  _mt = TipcMtCreate(name, num, grammar);

  if (_mt != T_NULL) {
    _status = T_TRUE;
  }

  return _status;
}

/* ========================================================================= */
/*..TipcMt::Destroy -- destroy a message type */
T_BOOL TipcMt::Destroy()
{
  _status = TipcMtDestroy(_mt);

  _mt = (T_IPC_MT)T_NULL;

  return _status;
}

/* ========================================================================= */
/*..TipcMt::Print -- print all information about a message type */
T_BOOL TipcMt::Print(T_OUT_FUNC func)
{
  return TipcMtPrint(_mt, func);
}

/* ========================================================================= */
/*..TipcMt::Lookup -- look up a message type by name */
T_BOOL TipcMt::Lookup(T_STR name)
{
  _status = T_FALSE;

  _mt = TipcMtLookup(name);

  if (_mt != T_NULL) {
    _status = T_TRUE;
  }

  return _status;
}


/* ========================================================================= */
/*..TipcMt::Lookup -- look up a message type by number */
T_BOOL TipcMt::Lookup(T_INT4 num)
{
  _status = T_FALSE;

  _mt = TipcMtLookupByNum(num);

  if (_mt != T_NULL) {
    _status = T_TRUE;
  }

  return _status;
}        


/* ========================================================================= */
/*..TipcMt::Grammar -- get the grammar of a message type */
T_STR TipcMt::Grammar()
{
  T_STR grammar_return = (T_STR)"";  // initialize variable

  _status = TipcMtGetGrammar(_mt, &grammar_return);
  return grammar_return;
}


/* ========================================================================= */
/*..TipcMt::Name -- get the name of a message type */
T_STR TipcMt::Name()
{
  T_STR name_return = (T_STR)"";  // initialize variable

  _status = TipcMtGetName(_mt, &name_return);
  return name_return;
}

/* ========================================================================= */
/*..TipcMt::Num -- get the number of a message type */
T_INT4 TipcMt::Num()
{
  T_INT4 num_return = 0;  // initialize variable

  _status = TipcMtGetNum(_mt, &num_return);
  return num_return;
} 


/* ========================================================================= */
/*..TipcMt::Priority -- get the priority of a message type */
T_INT2 TipcMt::Priority()
{
  T_INT2 priority_return = 0;  // initialize variable

  _status = TipcMtGetPriority(_mt, &priority_return);
  return priority_return;
}


/* ========================================================================= */
/*..TipcMt::Priority -- set the priority of a message type */
T_BOOL TipcMt::Priority(T_INT2 priority)
{
  _status = TipcMtSetPriority(_mt, priority);
  return _status;
}


/* ========================================================================= */
/*..TipcMt::SetPriorityUnknown -- set the priority of a message type to */
/*..  unknown */
T_BOOL TipcMt::SetPriorityUnknown()
{
  _status = TipcMtSetPriorityUnknown(_mt);
  return _status;
}


/* ========================================================================= */
/*..TipcMt::UserProp -- get user-defined property from a message type */
T_INT4 TipcMt::UserProp()
{
  T_INT4 user_prop_return = 0;  // initialize variable

  _status = TipcMtGetUserProp(_mt, &user_prop_return);
  return user_prop_return;
}


/* ========================================================================= */
/*..TipcMt::UserProp -- set user-defined property from a message type */
T_BOOL TipcMt::UserProp(T_INT4 user_prop)
{
  _status = TipcMtSetUserProp(_mt, user_prop);
  return _status;
}

/* ========================================================================= */
/*..TipcMt::DeliveryMode -- get the delivery mode of a message type */
T_IPC_DELIVERY_MODE TipcMt::DeliveryMode()
{
  T_IPC_DELIVERY_MODE delivery_mode_return = T_IPC_DELIVERY_BEST_EFFORT;

  _status = TipcMtGetDeliveryMode(_mt, &delivery_mode_return);
  return delivery_mode_return;
}


/* ========================================================================= */
/*..TipcMt::DeliveryMode -- set the delivery mode of a message type */
T_BOOL TipcMt::DeliveryMode(T_IPC_DELIVERY_MODE delivery_mode)
{
  _status = TipcMtSetDeliveryMode(_mt, delivery_mode);
  return _status;
}


/* ========================================================================= */
/*..TipcMt::Vacant -- return T_TRUE if TipcMt object is vacant */
T_BOOL TipcMt::Vacant()
{
  if (_mt == T_NULL) {
    return T_TRUE;
  }
  else {
    return T_FALSE;
  }
}

#ifndef T_SS35_HIERARCHY

/* ========================================================================= */
/*..TipcMt::LbMode -- get load balancing mode */
T_IPC_LB_MODE TipcMt::LbMode()
{
  T_IPC_LB_MODE lb_mode = T_IPC_LB_NONE;

  _status = TipcMtGetLbMode(_mt, &lb_mode);
  return lb_mode;
}

/* ========================================================================= */
/*..TipcMt::LbMode -- set load balancing mode */
T_BOOL TipcMt::LbMode(T_IPC_LB_MODE lb_mode)
{
  _status = TipcMtSetLbMode(_mt, lb_mode);
  return _status;
}

/* ========================================================================= */
/*..TipcMt::DeliveryTimeout -- get delivery timeout */
T_REAL8 TipcMt::DeliveryTimeout()
{
  T_REAL8 delivery_timeout = -1;

  _status = TipcMtGetDeliveryTimeout(_mt, &delivery_timeout);
  return delivery_timeout;
}

/* ========================================================================= */
/*..TipcMt::DeliveryTimeout -- set delivery timeout */
T_BOOL TipcMt::DeliveryTimeout(T_REAL8 delivery_timeout)
{
  _status = TipcMtSetDeliveryTimeout(_mt, delivery_timeout);
  return _status;
}

#endif

/* ========================================================================= */
/*..TipcMt::HeaderStrEncode() -- get header encode mode */
T_BOOL TipcMt::HeaderStrEncode()
{
  T_BOOL header_encode = -1;

  _status = TipcMtGetHeaderStrEncode(_mt, &header_encode);
  return header_encode;
}

/* ========================================================================= */
/*..TipcMt::HeaderStrEncode() -- set header encode mode */
T_BOOL TipcMt::HeaderStrEncode(T_BOOL header_encode)
{
  _status = TipcMtSetHeaderStrEncode(_mt, header_encode);
  return _status;
}

/* ========================================================================= */
/*..TipcMt::SetKey() -- set the encrypt key */
T_BOOL TipcMt::SetKey(T_ENCRYPT_KEY key)
{
  _status = TipcMtSetKey(_mt, key);
  return _status;
}

/* ========================================================================= */
/*..TipcMt::SetEncryptFunc() -- set the encrypt key */
T_BOOL TipcMt::SetEncryptFunc(T_IPC_ENCRYPT_FUNC encrypt_func, T_PTR arg)
{
  _status = TipcMtSetEncryptFunc(_mt, encrypt_func, arg);
  return _status;
}

/* ========================================================================= */
/*..TipcMt::GetEncryptFunc() -- get the encrypt key */
T_BOOL TipcMt::GetEncryptFunc(T_IPC_ENCRYPT_FUNC *encrypt_func_return)
{
  _status = TipcMtGetEncryptFunc(_mt, encrypt_func_return);
  return _status;
}


/* ========================================================================= */
/*..TipcMt::SetDecryptFunc() -- set the decrypt func */
T_BOOL TipcMt::SetDecryptFunc(T_IPC_DECRYPT_FUNC decrypt_func, T_PTR arg)
{
  _status = TipcMtSetDecryptFunc(_mt, decrypt_func, arg);
  return _status;
}

/* ========================================================================= */
/*..TipcMt::GetDecryptFunc() -- get the encrypt key */
T_BOOL TipcMt::GetDecryptFunc(T_IPC_DECRYPT_FUNC *decrypt_func_return)
{
  _status = TipcMtGetDecryptFunc(_mt, decrypt_func_return);
  return _status;
}
