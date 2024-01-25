/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmt.hxx#1 $
 */

#ifndef _T_MESSAGE_TYPE_HH_
#define _T_MESSAGE_TYPE_HH_

#include <rtworks/ipc.h>
#include <rtworks/tobj.hxx>

class T_IPCX_API TipcConnWrapper;
class T_IPCX_API TipcSrv;
class T_IPCX_API TipcMsg;

class T_IPCX_API TipcMt : public Tobj {

friend class TipcConn;
friend class TipcConnWrapper;
friend class TipcSrv;
friend class TipcMsg;

private:
  T_IPC_MT _mt;    

public:
  // Create a vacant TipcMt object
  TipcMt();

  // Create a TipcMt object with an existing C API
  // T_IPC_MT struct.
  TipcMt(T_IPC_MT mt);

  // Create a TipcMt object by message type name
  TipcMt(T_STR name);

  // Create a TipcMt object by message type number
  TipcMt(T_INT4 num);

  // Create a TipcMt object with a new message type
  TipcMt(T_STR name, T_INT4 num, T_STR grammar);

  // Copy constructor
  TipcMt(const TipcMt& mt);

  virtual ~TipcMt();

  T_BOOL Create(T_STR name, T_INT4 num, T_STR grammar);
  T_BOOL Destroy();

  T_IPC_MT MessageType() const { return _mt; }

  operator T_IPC_MT() const { return _mt; }

  T_BOOL Print(T_OUT_FUNC func);

  T_BOOL Lookup(T_STR name);
  T_BOOL Lookup(T_INT4 num);

  static T_PTR    Traverse(T_IPC_MT_TRAV_FUNC func, T_PTR arg);

  T_STR  Grammar();
  T_STR  Name();
  T_INT4 Num();

  T_INT2 Priority();                          // Get it
  T_BOOL Priority(T_INT2  priority);          // Set it

  T_BOOL SetPriorityUnknown();                // Set it

  T_INT4 UserProp();                          // Get it
  T_BOOL UserProp(T_INT4 user_prop);          // Set it

  T_IPC_DELIVERY_MODE DeliveryMode();         // Get it
  T_BOOL DeliveryMode(T_IPC_DELIVERY_MODE delivery_mode); // Set it

  T_BOOL Vacant();                            // Get it

#ifndef T_SS35_HIERARCHY
  T_IPC_LB_MODE LbMode();                     // Get it
  T_BOOL LbMode(T_IPC_LB_MODE lb_mode);       // Set it

  T_REAL8 DeliveryTimeout();                  // Get it
  T_BOOL DeliveryTimeout(T_REAL8 delivery_timeout); // Set it
#endif

  T_BOOL HeaderStrEncode();	                // Get it
  T_BOOL HeaderStrEncode(T_BOOL header_encode); // Set it

  T_BOOL SetKey(T_ENCRYPT_KEY key);

  T_BOOL SetEncryptFunc(T_IPC_ENCRYPT_FUNC encrypt_func, T_PTR arg);
  T_BOOL GetEncryptFunc(T_IPC_ENCRYPT_FUNC *encrypt_func_return);

  T_BOOL SetDecryptFunc(T_IPC_DECRYPT_FUNC decrypt_func, T_PTR arg);
  T_BOOL GetDecryptFunc(T_IPC_DECRYPT_FUNC *decrypt_func_return);
  
};

#endif
