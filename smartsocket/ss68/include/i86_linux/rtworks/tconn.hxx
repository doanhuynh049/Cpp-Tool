/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tconn.hxx#1 $
 */

#ifndef _T_CONN_HH_
#define _T_CONN_HH_

#include <rtworks/ipc.h>
#include <rtworks/tmsg.hxx>

#ifdef T_SS35_HIERARCHY
#include <rtworks/tipccw.hxx>

class T_IPCX_API TipcConn : public TipcConnWrapper {

protected:

  TipcConn(T_BOOL do_nothing);

public:
  TipcConn();
  TipcConn(T_IPC_CONN connection, T_BOOL destroy_flag=TRUE);

  virtual ~TipcConn();
};

#endif

#ifndef T_SS35_HIERARCHY

// new heirarchy.  TipcConn is now the base class for all connection
// type objects.  TipcConnWrapper has been moved beneath this, along
// with all of the protocol-specific ConnCreate functions.

class T_IPCX_API TipcConn : public Tobj {

protected:
  T_IPC_CONN _connection;
  T_BOOL _destroy_flag;
  TipcMsg _msg_referent;

  TipcConn(T_BOOL do_nothing);

public:
  TipcConn();
  TipcConn(T_IPC_CONN connection, T_BOOL destroy_flag=TRUE);
  virtual ~TipcConn();

  T_IPC_CONN         Connection() { return _connection;  }

  operator           T_IPC_CONN() { return _connection; }

  //  The following methods will have TipcSrv equivalents
  virtual T_BOOL     Check(T_IO_CHECK_MODE check_mode, T_REAL8 timeout);

  virtual T_BOOL     Flush();

  virtual T_INT4     AutoFlushSize();                        // Get it
  virtual T_BOOL     AutoFlushSize(T_INT4 auto_flush_size);  // Set it

  virtual T_BOOL     BlockMode();                            // Get it
  virtual T_BOOL     BlockMode(T_BOOL  block_mode);          // Set it

  virtual T_REAL8    Timeout(T_IPC_TIMEOUT timeout);         // Get it
  virtual T_BOOL     Timeout(T_IPC_TIMEOUT timeout,
                             T_REAL8  value);                // Set it

  virtual T_SOCK     Socket();                               // Get it
  virtual T_BOOL     Socket(T_SOCK sock);                    // Set it


  virtual T_INT4     NumQueued();                            // Get it
  virtual T_SOCK     XtSource();                             // Get it
  virtual T_BOOL     KeepAlive();

  virtual T_BOOL     MainLoop(T_REAL8 timeout);
  virtual T_BOOL     Insert(TipcMsg& msg, T_INT4 pos);
  virtual T_BOOL     Process(TipcMsg& msg);

  virtual TipcMsg&  Next(T_REAL8 timeout);

  virtual TipcMsg&  MsgNext(T_REAL8 timeout);

  virtual TipcMsg&  Search(T_REAL8 timeout,
                           T_IPC_CONN_MSG_SEARCH_FUNC func,
                           T_PTR arg);

  virtual TipcMsg&  MsgSearch(T_REAL8 timeout,
                              T_IPC_CONN_MSG_SEARCH_FUNC func,
                              T_PTR arg);

  virtual TipcMsg&  SearchType(T_REAL8 timeout, TipcMt& mt);

  virtual TipcMsg&  MsgSearchType(T_REAL8 timeout, TipcMt& mt);

  //  For connections, the rtserver_check flag is defaulted and ignored.
  //  The TipcSrv derived class version of this virtual fn will use it.
  virtual T_BOOL     Send(TipcMsg& msg, T_BOOL rtserver_check=FALSE);

  virtual TipcMsg&  SendRpc(TipcMsg& call_msg, T_REAL8 timeout);

  virtual TipcMsg&  MsgSendRpc(TipcMsg& call_msg, T_REAL8 timeout);

#ifdef T_OS_WINDOWS
  /*
   * The Microsoft Visual C++ compilers (4.2 and 5.0, and maybe earlier ones),
   * have a bug that makes it impossible to use references as the last
   * non-variable argument in a varargs list.  So, for Windows, we're just
   * passing the TipcMt object by value (since it's not modified [really
   * ought to be const, cie la vie...]).  Once this bug is fixed, or if
   * you're using some other compiler, this ifdef can be removed.
   */
  virtual T_BOOL     Write(TipcMt mt, ...);
  virtual T_BOOL     WriteVa(TipcMt mt, va_list var_arg_list);
#else
  virtual T_BOOL     Write(TipcMt& mt, ...);
  virtual T_BOOL     WriteVa(TipcMt& mt, va_list var_arg_list);
#endif

  virtual T_BOOL     Print(T_OUT_FUNC func);

  virtual T_CB       ErrorCbCreate(T_IPC_CONN_ERROR_CB_FUNC func,
                                   T_CB_ARG arg);

  virtual T_CB       ErrorCbLookup(T_IPC_CONN_ERROR_CB_FUNC func,
                                   T_CB_ARG arg);

  virtual T_CB       DefaultCbCreate(T_IPC_CONN_DEFAULT_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_CB       DefaultCbLookup(T_IPC_CONN_DEFAULT_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_CB       ProcessCbCreate(TipcMt& mt,
                                     T_IPC_CONN_PROCESS_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_CB       ProcessCbLookup(TipcMt& mt,
                                     T_IPC_CONN_PROCESS_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_CB       QueueCbCreate(TipcMt& mt,
                                   T_IPC_CONN_QUEUE_CB_FUNC func,
                                   T_CB_ARG arg);

  virtual T_CB       QueueCbLookup(TipcMt& mt,
                                   T_IPC_CONN_QUEUE_CB_FUNC func,
                                   T_CB_ARG arg);

  virtual T_BOOL     Read(T_REAL8 timeout);

  virtual T_CB       ReadCbCreate(TipcMt& mt,
                                  T_IPC_CONN_READ_CB_FUNC func,
                                  T_CB_ARG arg);


  virtual T_CB       ReadCbLookup(TipcMt& mt,
                                  T_IPC_CONN_READ_CB_FUNC func,
                                  T_CB_ARG arg);


  virtual T_CB       WriteCbCreate(TipcMt& mt,
                                   T_IPC_CONN_WRITE_CB_FUNC func,
                                   T_CB_ARG arg);


  virtual T_CB       WriteCbLookup(TipcMt& mt,
                                   T_IPC_CONN_WRITE_CB_FUNC func,
                                   T_CB_ARG arg);

  virtual T_CB       EncryptCbCreate(TipcMt& mt,
                                     T_IPC_CONN_ENCRYPT_CB_FUNC func,
                                     T_CB_ARG arg);


  virtual T_CB       EncryptCbLookup(TipcMt& mt,
                                     T_IPC_CONN_ENCRYPT_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_CB       DecryptCbCreate(TipcMt& mt,
                                     T_IPC_CONN_DECRYPT_CB_FUNC func,
                                     T_CB_ARG arg);


  virtual T_CB       DecryptCbLookup(TipcMt& mt,
                                     T_IPC_CONN_DECRYPT_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_CB EncodeCbCreate(T_IPC_CONN_ENCODE_CB_FUNC func,
                              T_CB_ARG arg);

  virtual T_CB EncodeCbLookup(T_IPC_CONN_ENCODE_CB_FUNC func,
                              T_CB_ARG arg);

  virtual T_CB DecodeCbCreate(T_IPC_CONN_DECODE_CB_FUNC func,
                              T_CB_ARG arg);

  virtual T_CB DecodeCbLookup(T_IPC_CONN_DECODE_CB_FUNC func,
                              T_CB_ARG arg);

  // ---------------------------------------------------------------------
  //  Starred methods have RTserver (TipcSrv*) overrides
  // ---------------------------------------------------------------------
  virtual T_STR      UniqueSubject();                   // Get it *
#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
  virtual T_STR      UniqueDatagroup();                   // Get it *
#endif

  virtual T_STR      Node();                              // Get it *
  virtual T_STR      User();                              // Get it *
  virtual T_INT4     Pid();                               // Get it *
  virtual T_STR      Arch();                              // Get it *

  virtual T_BOOL Lock();
  virtual T_BOOL Unlock();

  virtual T_BOOL     GmdFileCreate();                     // *
  virtual T_BOOL     GmdFileDelete();                     // *
  virtual T_BOOL     GmdResend();                         // *

  virtual T_BOOL     GmdMsgDelete(TipcMsg& msg);          // *
  virtual T_BOOL     GmdMsgResend(TipcMsg& msg);          // *

  virtual T_UINT4    GmdMaxSize();                        // Get it *
  virtual T_BOOL     GmdMaxSize(T_UINT4 gmd_max_size);    // Set it *

  static  T_STR      GmdDir();                            // Get it

  virtual T_INT4 GmdNumPending();    // Get it

};

#endif

class T_IPCX_API TipcConnServer : public TipcConn {

public:
  TipcConnServer(T_STR link_name);
  TipcConnServer(T_IPC_CONN connection, T_BOOL destroy_flag=TRUE);

  virtual ~TipcConnServer() {}

  virtual TipcConn *Accept();
};

class T_IPCX_API TipcConnClient : public TipcConn {

public:
  TipcConnClient() {}
  TipcConnClient(T_STR link_name);
  TipcConnClient(T_IPC_CONN connection, T_BOOL destroy_flag=TRUE);
  virtual ~TipcConnClient(){}
};

#endif
