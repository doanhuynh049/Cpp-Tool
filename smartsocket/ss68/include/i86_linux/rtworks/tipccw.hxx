/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tipccw.hxx#2 $
 */

#ifndef _T_IPC_CONN_WRAP_HH_
#define _T_IPC_CONN_WRAP_HH_

#include <rtworks/tobj.hxx>
#include <rtworks/tmsg.hxx>

#ifdef T_SS35_HIERARCHY
class T_IPCX_API TipcConnWrapper : public Tobj {

protected:
  /* ---------------------------------------------------------------------- */
  /* Derived classes (TipcConn, TipcTcpClient, TipcTcpServer,               */
  /* TipcLocalClient, TipcLocalServer) will set _connection at construction */
  /* via appropriate TipcConnCreate calls.  The wrapper does nothing at     */
  /* construction, but does do TipcConnDestroy on behalf of derived classes */
  /* at destruction unless the user overrides destruction of the            */
  /* _connection, as indicated by this internal _destroy_flag.              */
  /* ---------------------------------------------------------------------- */
  T_IPC_CONN _connection;
  T_BOOL     _destroy_flag;

  /* ---------------------------------------------------------- */
  /* Holding place for TipcMsg objects returned by reference by */
  /* member functions of this class.                            */
  /* ---------------------------------------------------------- */
  TipcMsg _msg_referent;

  /* ---------------------------------------------------------------------- */
  /* Permit construction in derived class only.  The effect is to render    */
  /* TipcConnWrapper into an abstract base class.  Functionality common to  */
  /* the C API functions TipcConn* and TipcSrv* will be represented here,   */
  /* and implemented here via virtual functions.  The TipcSrv class derives */
  /* itself from the TipcConnWrapper class and reimplements the common      */
  /* functionality using the parallel C API TipcSrv* function calls.        */
  /* ---------------------------------------------------------------------- */
  TipcConnWrapper();

  virtual ~TipcConnWrapper();  // Will destroy at destruction

public:
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
			     T_REAL8  value);                   // Set it

  virtual T_INT4     Socket();                               // Get it
  virtual T_BOOL     Socket(T_INT4 sock);                    // Set it


  virtual T_INT4     NumQueued();                            // Get it
  virtual T_INT4     XtSource();                             // Get it
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

  virtual T_BOOL     Write(TipcMt& mt, ...);
  virtual T_BOOL     WriteVa(TipcMt& mt, va_list var_arg_list);

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


  virtual T_BOOL     GmdFileCreate();                     // *
  virtual T_BOOL     GmdFileDelete();                     // *
  virtual T_BOOL     GmdResend();                         // *

  virtual T_BOOL     GmdMsgDelete(TipcMsg& msg);          // *
  virtual T_BOOL     GmdMsgResend(TipcMsg& msg);          // *

  virtual T_UINT4    GmdMaxSize();                        // Get it *
  virtual T_BOOL     GmdMaxSize(T_UINT4 gmd_max_size);    // Set it *

  static  T_STR      GmdDir();                            // Get it
};

#else

// new heirarchy arrangement. TipcConnWrapper now derives from TipcConn,
// rather than the other way around.  All code from ConnWrapper has been
// moved up into Conn.  This class lingers as a hook for old code.

#include <rtworks/tconn.hxx>

class T_IPCX_API TipcConnWrapper : public TipcConn {
protected:
  TipcConnWrapper() {}
  TipcConnWrapper(T_BOOL do_nothing) : TipcConn(do_nothing) {}
  virtual ~TipcConnWrapper(){}
};

#endif

#endif





