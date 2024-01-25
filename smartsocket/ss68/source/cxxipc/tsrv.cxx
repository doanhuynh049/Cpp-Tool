/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tsrv.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tsrv.hxx>

TipcSrv* TipcSrv::_instance = (TipcSrv*) T_NULL;
T_MUTEX TipcSrv::_mutex = T_INVALID_MUTEX;

/* ------------------------------------------------------------------ */
/* This constructor is protected, and isn't callable by users because */
/* it doesn't make sense to have more than one TipcSrv object.        */
/* Instead, users should call one of the static member functions      */
/* instance() or instanceCreate() depending on whether they would     */
/* like to have TipcSrvCreate() called on their behalf as side effect */
/* of asking for the handle.                                          */
/* ------------------------------------------------------------------ */

/* ========================================================================= */
/*..TipcSrv::TipcSrv -- TipcSrv constructor */
TipcSrv::TipcSrv(T_IPC_SRV_CONN_STATUS create_status,
                 T_BOOL create_conn) : TipcConnClient((T_IPC_CONN)T_NULL, T_FALSE)
{
  T_IPC_SRV_CONN_STATUS conn_status_return;
  T_BOOL conn_status_result = TipcSrvGetConnStatus(&conn_status_return);

  if ((T_TRUE == conn_status_result)                &&
      (conn_status_return == T_IPC_SRV_CONN_NONE) &&
      (T_TRUE == create_conn)) {
    _status = TipcSrvCreate(create_status);

    /*
     * In SS 5.5, this constructor used to call the
     * default TipcConnClient constructor, which called ::TipcConnCreate
     * to create the connection.  However, I don't think this connection
     * was ever used.  In fact, since TipcSrv::Write was never
     * implemented in this class, the msg was serialized into the
     * buffer provided by TipcConnClient (i.e., the _connection)
     * and never used.
     * Other methods are impelmented here to call the corresponding
     * c-based TipcSrv functions to use the conn associated with
     * the TipcSrv layer.
     *
     * In SS 6.0, we added a check for threading, so we don't want
     * to call the default constructor in TipcConnClient.  Instead,
     * we call the constructor to set the connection to null and
     * don't destroy and then now (after creating the TipcSrv connection)
     * we create the TipcConn connection so that we simulate
     * what the SS 5.5 server used to do.
     */
    _connection = ::TipcConnCreate();
    _destroy_flag = T_TRUE;
  }

  // if TipcInitThreads has been called then create the mutex

  if (TipcThreadEnabled()) {
    _mutex = TutMutexCreateFast(T_FALSE);
  }
}


/* ========================================================================= */
/*..TipcSrv::~TipcSrv -- TipcSrv destructor */
TipcSrv::~TipcSrv()
{
  if (_instance) {
    TipcSrvDestroy(T_IPC_SRV_CONN_NONE);
    if (T_INVALID_MUTEX != _mutex) {
      TutMutexDestroy(_mutex);
    }
  }
}


/* ========================================================================= */
/*..TipcSrv::Instance -- return a handle to TipcSrv object, do not create */
/*..  a connection to RTserver */
TipcSrv& TipcSrv::Instance()
{

  if (T_NULL == _instance) {
    _instance = new TipcSrv();
  }
  return *_instance;
}


/* ========================================================================= */
/*..TipcSrv::InstanceCreate -- return a handle to TipcSrv object and create */
/*..  a connection to RTserver */
TipcSrv& TipcSrv::InstanceCreate(T_IPC_SRV_CONN_STATUS create_status)
{
  if (T_NULL == _instance) {
    _instance = new TipcSrv(create_status, T_TRUE);
  }
  return *_instance;
}


/* ----------------------------------------------------------------------- */
/* Note: the next function is a non-static function. The user will only be */
/* able to call it after having called the static member instance()        */
/* in order to obtain a handle to the TipcSrv instance.  The effect of     */
/* the static instanceCreate() member function is to combine               */
/* calls to instance() and create().                                       */
/* ----------------------------------------------------------------------- */

/* ========================================================================= */
/*..TipcSrv::Create -- create the connection to RTserver */
T_BOOL TipcSrv::Create(T_IPC_SRV_CONN_STATUS create_status)
{
  _status = TipcSrvCreate(create_status);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::Destroy -- destroy the connection to RTserver */
T_BOOL TipcSrv::Destroy(T_IPC_SRV_CONN_STATUS destroy_status)
{
  T_BOOL result = TipcSrvDestroy(destroy_status);
  //  if (_instance) delete _instance;
  return result;
}


/* ------------------------------------------------------------ */
/* Virtual function methods inherited from RTConnectionWrapper  */
/* and reimplemented here for TipcSrv rather than TipcConn.     */
/* (well, now they're overridden from TipcConn, not Wrapper)    */
/* ------------------------------------------------------------ */

/* ========================================================================= */
/*..TipcSrv::Check -- check if data can be read from or written to  */
/*..  connectino to RTserver */
T_BOOL TipcSrv::Check(T_IO_CHECK_MODE check_mode,
                      T_REAL8 timeout)
{
  _status = TipcSrvCheck(check_mode, timeout);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::ErrorCbCreate -- create an error callback in the connection */
/*..  to RTserver */
T_CB TipcSrv::ErrorCbCreate(T_IPC_CONN_ERROR_CB_FUNC func,
                             T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvErrorCbCreate(func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcSrv::ErrorCbLookup -- look up a error callback in the connection */
/*..  to RTserver */
T_CB TipcSrv::ErrorCbLookup(T_IPC_CONN_ERROR_CB_FUNC func,
                             T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvErrorCbLookup(func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcSrv::Flush -- flush buffered outgoing messages on the connection */
/*..  to RTserver */
T_BOOL TipcSrv::Flush()
{
  _status = TipcSrvFlush();
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::AutoFlushSize -- get the automatic flush size of the connection */
/*..  to RTserver */
T_INT4 TipcSrv::AutoFlushSize()
{
  T_INT4 auto_flush_size_return = -1;  // initialize variable

  _status = TipcSrvGetAutoFlushSize(&auto_flush_size_return);
  return auto_flush_size_return;
}


/* ========================================================================= */
/*..TipcSrv::AutoFlushSize -- set the automatic flush size of the connection */
/*..  to RTserver */
T_BOOL TipcSrv::AutoFlushSize(T_INT4 auto_flush_size)
{
  _status = TipcSrvSetAutoFlushSize(auto_flush_size);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::BlockMode -- get the block mode of the connection to RTserver */
T_BOOL TipcSrv::BlockMode()
{
  T_BOOL block_mode_return = T_TRUE;  // initialize variable

  _status = TipcSrvGetBlockMode(&block_mode_return);
  return block_mode_return;
}


/* ========================================================================= */
/*..TipcSrv::BlockMode -- set the block mode of the connection to RTserver */
T_BOOL TipcSrv::BlockMode(T_BOOL block_mode)
{
  _status = TipcSrvSetBlockMode(block_mode);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::NumQueued -- get the number of queued message from the  */
/*..  connection to RTserver */
T_INT4 TipcSrv::NumQueued()
{
  T_INT4 num_queued_return = -1;  // initialize variable

  _status = TipcSrvGetNumQueued(&num_queued_return);
  return num_queued_return;
}


/* ========================================================================= */
/*..TipcSrv::Socket -- get the socket of the connection to RTserver */
T_SOCK TipcSrv::Socket()
{
  T_SOCK socket_return = -1;  // initialize variable

  _status = TipcSrvGetSocket(&socket_return);
  return socket_return;
}


/* ========================================================================= */
/*..TipcSrv::Socket -- set the socket of the connectino to RTserver */
T_BOOL TipcSrv::Socket(T_SOCK sock)
{
  _status = TipcSrvSetSocket(sock);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::Timeout -- get a timeout property from the connection to */
/*..  RTserver */
T_REAL8 TipcSrv::Timeout(T_IPC_TIMEOUT timeout)
{
  T_REAL8 value_return = -1.0;  // initialize variable

  _status = TipcSrvGetTimeout(timeout, &value_return);
  return value_return;
}


/* ========================================================================= */
/*..TipcSrv::Timeout -- set a timeout property of the connecction to */
/*..  RTserver */
T_BOOL TipcSrv::Timeout(T_IPC_TIMEOUT timeout, T_REAL8 value)
{
  _status = TipcSrvSetTimeout(timeout, value);
  return _status;
}

/* ========================================================================= */
/*..TipcSrv::XtSource -- get source suitable from XtAppAddInput from the */
/*..  connection to RTserver */
T_SOCK TipcSrv::XtSource()
{
  T_SOCK source_return = -1;  // initialize variable

  _status = TipcSrvGetXtSource(&source_return);
  return source_return;
}


/* ========================================================================= */
/*..TipcSrv::KeepAlive -- check if the connection to RTserver is still alive */
T_BOOL TipcSrv::KeepAlive()
{
  _status = TipcSrvKeepAlive();
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::MainLoop -- read and process messages on the connection to */
/*..  RTserver */
T_BOOL TipcSrv::MainLoop(T_REAL8 timeout)
{
  _status = TipcSrvMainLoop(timeout);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::Insert -- insert message into queue of the connection to */
/*..  RTserver */
T_BOOL TipcSrv::Insert(TipcMsg& msg, T_INT4 pos)
{
  _status = TipcSrvMsgInsert(msg._msg, pos);
  if (_status) {
    /* --------------------------------------------------------- */
    /* Since TipcSrvMsgInsert takes destroy responsibility from  */
    /* the caller, convert TipcMsg object into a vacant object.  */
    /* --------------------------------------------------------- */
    msg._msg = (T_IPC_MSG)T_NULL;
  }
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::Next -- get the next message from the connection to RTserver */
TipcMsg& TipcSrv::Next(T_REAL8 timeout)
{
  _status = T_FALSE;

  T_IPC_MSG msg = TipcSrvMsgNext(timeout);

  T_CXX_MUTEX_LOCK(_mutex);

  _msg_referent.destroyMsg();
  _msg_referent.absorb(msg);

  T_CXX_MUTEX_UNLOCK(_mutex);

  if (T_NULL != _msg_referent._msg) {
    _status = T_TRUE;
  }

  return _msg_referent;
}


/* ========================================================================= */
/*..TipcSrv::MsgNext -- get the next message from the connection to RTserver */
TipcMsg& TipcSrv::MsgNext(T_REAL8 timeout)
{
  _status = T_FALSE;

  TipcMsg *return_msg = new TipcMsg();

  T_IPC_MSG msg = TipcSrvMsgNext(timeout);
  if (T_NULL != msg) {
    return_msg->absorb(msg);
  }

  if (T_NULL != return_msg->_msg) {
    _status = T_TRUE;
  }

  return *return_msg;
}


/* ========================================================================= */
/*..TipcSrv::Process -- process a message in the connection to RTserver */
T_BOOL TipcSrv::Process(TipcMsg& msg)
{
  _status = TipcSrvMsgProcess(msg._msg);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::Search -- search the message queue of the connection to  */
/*..  RTserver for a specific message. */
TipcMsg&  TipcSrv::Search(T_REAL8 timeout,
                          T_IPC_CONN_MSG_SEARCH_FUNC func,
                          T_PTR arg)
{
  _status = T_FALSE;

  T_IPC_MSG msg = TipcSrvMsgSearch(timeout, func, arg);

  T_CXX_MUTEX_LOCK(_mutex);

  _msg_referent.destroyMsg();
  _msg_referent.absorb(msg);

  T_CXX_MUTEX_UNLOCK(_mutex);

  if (T_NULL != _msg_referent._msg) {
    _status = T_TRUE;
  }

  return _msg_referent;
}


/* ========================================================================= */
/*..TipcSrv::MsgSearch -- search the message queue of the connection to  */
/*..  RTserver for a specific message. */
TipcMsg&  TipcSrv::MsgSearch(T_REAL8 timeout,
                             T_IPC_CONN_MSG_SEARCH_FUNC func,
                             T_PTR arg)
{
  _status = T_FALSE;

  TipcMsg *return_msg = new TipcMsg();

  T_IPC_MSG msg = TipcSrvMsgSearch(timeout, func, arg);
  if (T_NULL != msg) {
    return_msg->absorb(msg);
  }

  if (T_NULL != return_msg->_msg) {
    _status = T_TRUE;
  }

  return *return_msg;
}


/* ========================================================================= */
/*..TipcSrv::SearchType -- search the message queue of a connection to  */
/*..  RTserver for a message with a specific type */
TipcMsg& TipcSrv::SearchType(T_REAL8 timeout, TipcMt& mt)
{
  _status = T_FALSE;

  T_IPC_MSG msg = TipcSrvMsgSearchType(timeout, mt._mt);

  T_CXX_MUTEX_LOCK(_mutex);

  _msg_referent.destroyMsg();
  _msg_referent.absorb(msg);

  T_CXX_MUTEX_UNLOCK(_mutex);

  if (T_NULL != _msg_referent._msg) {
    _status = T_TRUE;
  }

  return _msg_referent;
}


/* ========================================================================= */
/*..TipcSrv::MsgSearchType -- search the message queue of a connection to  */
/*..  RTserver for a message with a specific type */
TipcMsg& TipcSrv::MsgSearchType(T_REAL8 timeout, TipcMt& mt)
{
  _status = T_FALSE;

  TipcMsg *return_msg = new TipcMsg();

  T_IPC_MSG msg = TipcSrvMsgSearchType(timeout, mt._mt);
  if (T_NULL != msg) {
    return_msg->absorb(msg);
  }

  if (T_NULL != return_msg->_msg) {
    _status = T_TRUE;
  }

  return *return_msg;
}


/* ========================================================================= */
/*..TipcSrv::Send -- send a message through the connection to RTserver */
T_BOOL  TipcSrv::Send(TipcMsg& msg, T_BOOL check_server_msg_send)
{
  _status = TipcSrvMsgSend(msg._msg, check_server_msg_send);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::SendRpc -- make a remote procedure call with messages on the */
/*..  connection to RTserver */
TipcMsg& TipcSrv::SendRpc(TipcMsg& call_msg, T_REAL8 timeout)
{
  _status = T_FALSE;

  T_IPC_MSG msg = TipcSrvMsgSendRpc(call_msg._msg, timeout);

  T_CXX_MUTEX_LOCK(_mutex);

  _msg_referent.destroyMsg();
  _msg_referent.absorb(msg);

  T_CXX_MUTEX_UNLOCK(_mutex);

  if (T_NULL != _msg_referent._msg) {
    _status = T_TRUE;
  }

  return _msg_referent;
}


/* ========================================================================= */
/*..TipcSrv::MsgSendRpc -- make a remote procedure call with messages on the */
/*..  connection to RTserver */
TipcMsg& TipcSrv::MsgSendRpc(TipcMsg& call_msg, T_REAL8 timeout)
{
  _status = T_FALSE;

  TipcMsg *return_msg = new TipcMsg();

  T_IPC_MSG msg = TipcSrvMsgSendRpc(call_msg._msg, timeout);
  if (T_NULL != msg) {
    return_msg->absorb(msg);
  }

  if (T_NULL != return_msg->_msg) {
    _status = T_TRUE;
  }

  return *return_msg;
}


/* ========================================================================= */
/*..TipcSrv::SrvWrite -- construct a message and send it through the  */
/*..  connection to RTserver */
T_BOOL TipcSrv::SrvWrite(T_STR dest,
                         TipcMt& mt,
                         T_BOOL check_server_msg_send, ...)
{
  va_list var_arg_list;   // To point to each unnamed arg in turn

  va_start(var_arg_list, check_server_msg_send);
  _status = TipcSrvMsgWriteVa(dest, mt._mt, check_server_msg_send, var_arg_list);
  va_end(var_arg_list);

  return _status;
}

/* ========================================================================= */
/*..TipcSrv::SrvWriteVa -- construct a message and send it through the */
/*..  connection to RTserver (va_list version) */
T_BOOL TipcSrv::SrvWriteVa(T_STR dest,
                           TipcMt& mt,
                           T_BOOL check_server_msg_send,
                           va_list var_arg_list)
{
  _status = TipcSrvMsgWriteVa(dest, mt._mt, check_server_msg_send, var_arg_list);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::Print -- print all information about the connection to RTserver */
T_BOOL TipcSrv::Print(T_OUT_FUNC func)
{
  _status = TipcSrvPrint(func);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::DefaultCbCreate -- create a default callback in the connection */
/*..  to RTserver */
T_CB TipcSrv::DefaultCbCreate(T_IPC_CONN_DEFAULT_CB_FUNC func,
                              T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvDefaultCbCreate(func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcSrv::DefaultCbLookup -- look up a default callback in the connection */
/*..  to RTserver */
T_CB TipcSrv::DefaultCbLookup(T_IPC_CONN_DEFAULT_CB_FUNC func,
                              T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvDefaultCbLookup(func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcSrv::ProcessCbCreate -- create a process callback in the connection */
/*..  to RTserver */
T_CB TipcSrv::ProcessCbCreate(TipcMt& mt,
                              T_IPC_CONN_PROCESS_CB_FUNC func,
                              T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvProcessCbCreate(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcSrv::ProcessCbLookup -- look up a process callback in the connection */
/*..  to RTserver */
T_CB TipcSrv::ProcessCbLookup(TipcMt& mt,
                              T_IPC_CONN_PROCESS_CB_FUNC func,
                              T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvProcessCbLookup(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcSrv::QueueCbCreate -- create a queue callback in the connection to  */
/*..  RTserver */
T_CB TipcSrv::QueueCbCreate(TipcMt& mt,
                            T_IPC_CONN_QUEUE_CB_FUNC func,
                            T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvQueueCbCreate(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcSrv::QueueCbLookup -- look up a queu callback in the connection to */
/*..  RTserver */
T_CB TipcSrv::QueueCbLookup(TipcMt& mt,
                            T_IPC_CONN_QUEUE_CB_FUNC func,
                            T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvQueueCbLookup(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcSrv::Read -- read all available data from the connection to RTserver */
/*..  and queue messages in priority order */
T_BOOL TipcSrv::Read(T_REAL8 timeout)
{
  _status = TipcSrvRead(timeout);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::ReadCbCreate -- create a read callback in the connection to  */
/*..  RTserver */
T_CB TipcSrv::ReadCbCreate(TipcMt& mt,
                           T_IPC_CONN_READ_CB_FUNC func,
                           T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvReadCbCreate(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcSrv::ReadCbLookup -- look up a read callback in the connection to */
/*..  RTserver */
T_CB TipcSrv::ReadCbLookup(TipcMt& mt,
                           T_IPC_CONN_READ_CB_FUNC func,
                           T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvReadCbLookup(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcSrv::WriteCbCreate -- create a write callback in the connection to */
/*..  RTserver */
T_CB TipcSrv::WriteCbCreate(TipcMt& mt,
                            T_IPC_CONN_WRITE_CB_FUNC func,
                            T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvWriteCbCreate(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}

/* ========================================================================= */
/*..TipcSrv::EncryptCbCreate -- create a encrypt callback in the connection to */
/*..  RTserver */
T_CB TipcSrv::EncryptCbCreate(TipcMt& mt,
                            T_IPC_CONN_ENCRYPT_CB_FUNC func,
                            T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvEncryptCbCreate(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}

/* ========================================================================= */
/*..TipcSrv::DecryptCbCreate -- create a decrypt callback in the connection to */
/*..  RTserver */
T_CB TipcSrv::DecryptCbCreate(TipcMt& mt,
                            T_IPC_CONN_DECRYPT_CB_FUNC func,
                            T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvDecryptCbCreate(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcSrv::EncryptCbLookup -- look up a encrypt callback in the connection to */
/*..  RTserver */
T_CB TipcSrv::EncryptCbLookup(TipcMt& mt,
                            T_IPC_CONN_ENCRYPT_CB_FUNC func,
                            T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvEncryptCbLookup(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}

/* ========================================================================= */
/*..TipcSrv::DecryptCbLookup -- look up a decrypt callback in the connection to */
/*..  RTserver */
T_CB TipcSrv::DecryptCbLookup(TipcMt& mt,
                            T_IPC_CONN_DECRYPT_CB_FUNC func,
                            T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvDecryptCbLookup(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}

/* ========================================================================= */
/*..TipcSrv::WriteCbLookup -- look up a write callback in the connection to */
/*..  RTserver */
T_CB TipcSrv::WriteCbLookup(TipcMt& mt,
                            T_IPC_CONN_WRITE_CB_FUNC func,
                            T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcSrvWriteCbLookup(mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


// ----------------------------------------------------------------------
// TipcSrv functions that have no TipcConn parallel
// ----------------------------------------------------------------------

/* ========================================================================= */
/*..TipcSrv::CreateCbCreate -- create a server create callback  */
T_CB TipcSrv::CreateCbCreate(T_IPC_SRV_CREATE_CB_FUNC func,
                             T_CB_ARG arg)
{
  T_CB result = TipcSrvCreateCbCreate(func,arg);
  return result;
}


/* ========================================================================= */
/*..TipcSrv::CreateCbLookup -- look up a server create callback */
T_CB TipcSrv::CreateCbLookup(T_IPC_SRV_CREATE_CB_FUNC func,
                             T_CB_ARG arg)
{
  T_CB result = TipcSrvCreateCbLookup(func,arg);

  return result;
}


/* ========================================================================= */
/*..TipcSrv::DestroyCbCreate -- create a server destroy callback */
T_CB TipcSrv::DestroyCbCreate(T_IPC_SRV_DESTROY_CB_FUNC func,
                              T_CB_ARG arg)
{
  T_CB result = TipcSrvDestroyCbCreate(func,arg);

  return result;
}


/* ========================================================================= */
/*..TipcSrv::DestroyCbLookup -- look up a server destroy callback */
T_CB TipcSrv::DestroyCbLookup(T_IPC_SRV_DESTROY_CB_FUNC func,
                              T_CB_ARG arg)
{
  T_CB result = TipcSrvDestroyCbLookup(func,arg);

  return result;
}


// ---------------------------------------------------------------
// The following functions may establish a connection to TipcSrv
// under certain circumstances
// ---------------------------------------------------------------

/* ========================================================================= */
/*..TipcSrv::SubjectSubscribe -- get whether or not a client process is */
/*.. receiving a datagroup */
T_BOOL TipcSrv::SubjectSubscribe(T_STR subject)
{
  T_BOOL subscribe_status_return = T_FALSE;  // initialize variable

  _status = TipcSrvSubjectGetSubscribe(subject, &subscribe_status_return);
  return subscribe_status_return;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcSrv::DgRecv -- get whether or not an RTclient process is receiving */
/*..  a datagroup */
T_BOOL TipcSrv::DgRecv(T_STR dg)
{
  return SubjectSubscribe(dg);
}
#endif

/* ========================================================================= */
/*..TipcSrv::SubjectSubscribe -- start or stop receiving a subject */
T_BOOL TipcSrv::SubjectSubscribe(T_STR subject, T_BOOL subscribe_status)
{
  _status = TipcSrvSubjectSetSubscribe(subject, subscribe_status);
  return _status;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcSrv::DgRecv -- start or stop receiving a datagroup */
T_BOOL TipcSrv::DgRecv(T_STR dg, T_BOOL recv_status)
{
  return SubjectSubscribe(dg, recv_status);
}
#endif

/* ========================================================================= */
/*..TipcSrv::SubjectRetrieve -- retrieve current values from RTdaq for all */
/*..  variables in a datagroup */
T_BOOL TipcSrv::SubjectRetrieve(T_STR subject)
{
  _status = TipcSrvSubjectRetrieve(subject);
  return _status;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcSrv::DgRetrieve -- retrieve current values from RTdaq for all */
/*..  variables in a datagroup */
T_BOOL TipcSrv::DgRetrieve(T_STR dg)
{
  return SubjectRetrieve(dg);
}
#endif

/* ========================================================================= */
/*..TipcSrv::SubjectTraverseSubscribe -- traverse the subjects that the */
/*..  client process is receiving */
T_PTR TipcSrv::SubjectTraverseSubscribe(T_IPC_SRV_DG_TRAV_FUNC func,
                                        T_PTR arg)
{
  return TipcSrvSubjectTraverseSubscribe(func, arg);
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcSrv::DgTraverseRecv -- traverse the datagroups that the RTclient */
/*..  process is receiving */
T_PTR TipcSrv::DgTraverseRecv(T_IPC_SRV_DG_TRAV_FUNC func,
                              T_PTR arg)
{
  return SubjectTraverseSubscribe(func, arg);
}
#endif


// ---------------------------------------------------------------
// TipcSrvIsRunning also possibly creates connection to TipcSrv;
// the next function is a static method.
// ---------------------------------------------------------------

/* ========================================================================= */
/*..TipcSrv::IsRunning -- check if the RTclient process can create a */
/*..  connection to RTserver */
T_BOOL TipcSrv::IsRunning()
{
  return TipcSrvIsRunning();
}


/* ========================================================================= */
/*..TipcSrv::LogAddMt -- add a message type to a message file logging type */
T_BOOL TipcSrv::LogAddMt(T_IPC_SRV_LOG_TYPE log_type,
                         TipcMt& mt)
{
  return TipcSrvLogAddMt(log_type, mt._mt);
}


/* ========================================================================= */
/*..TipcSrv::LogRemoveMt -- remove a message type from a message file */
/*..  logging type */
T_BOOL TipcSrv::LogRemoveMt(T_IPC_SRV_LOG_TYPE log_type,
                            TipcMt& mt)
{
  return TipcSrvLogRemoveMt(log_type, mt._mt);
}

/* ========================================================================= */
/*..TipcSrv::StdSubjectRetrieve - retrieve current values from RTdaq for all */
/*..  variables in standard datagroups */
T_BOOL TipcSrv::StdSubjectRetrieve(T_BOOL retrieve_time)
{
  return TipcSrvStdSubjectRetrieve(retrieve_time);
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcSrv::StdDgRetrieve -- retrieve current values from RTdaq for all */
/*..  variables in standard datagroups */
T_BOOL TipcSrv::StdDgRetrieve(T_BOOL retrieve_time)
{
  return StdSubjectRetrieve(retrieve_time);
}
#endif

/* ========================================================================= */
/*..TipcSrv::StdSubjectSetSubscribe -- start or stop receiving all standard */
/*.. subjects */
T_BOOL TipcSrv::StdSubjectSetSubscribe(T_BOOL recv_flag, T_BOOL time_flag)
{
  return TipcSrvStdSubjectSetSubscribe(recv_flag, time_flag);
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcSrv::StdDgSetRecv -- start or stop receiving all standard datagroups */
T_BOOL TipcSrv::StdDgSetRecv(T_BOOL recv_flag, T_BOOL time_flag)
{
  return StdSubjectSetSubscribe(recv_flag, time_flag);
}
#endif

/* ========================================================================= */
/*..TipcSrv::Stop -- stop RTserver and possibly more processess */
T_BOOL TipcSrv::Stop(T_IPC_SRV_STOP_TYPE stop_type)
{
  return TipcSrvStop(stop_type);
}

/* ========================================================================= */
/*..TipcSrv::UniqueSubject -- get the unique subject of the connected */
/*..  RTserver process */
T_STR TipcSrv::UniqueSubject()
{
  T_STR unique_subject_return = (T_STR)"";  // initialize variable

  _status = TipcSrvGetUniqueSubject(&unique_subject_return);
  return unique_subject_return;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcSrv::UniqueDatagroup -- get the unique datagroup of the connected */
/*..  RTserver process */
T_STR TipcSrv::UniqueDatagroup()
{
  return UniqueSubject();
}
#endif

/* ========================================================================= */
/*..TipcSrv::Node -- get the node name of the connected RTserver process */
T_STR TipcSrv::Node()
{
  T_STR node_return = (T_STR)"";  // initialize variable

  _status = TipcSrvGetNode(&node_return);
  return node_return;
}


/* ========================================================================= */
/*..TipcSrv::User -- get the user name of the connected RTserver process */
T_STR TipcSrv::User()
{
  T_STR user_return = (T_STR)"";  // initialize variable

  _status = TipcSrvGetUser(&user_return);
  return user_return;
}


/* ========================================================================= */
/*..TipcSrv::Pid -- get the process ID of the connected RTserver process */
T_INT4 TipcSrv::Pid()
{
  T_INT4 pid_return = -1;  // initialize variable

  _status = TipcSrvGetPid(&pid_return);
  return pid_return;
}


/* ========================================================================= */
/*..TipcSrv::Arch -- get the architecture name of the connected RTserver */
/*..  process */
T_STR TipcSrv::Arch()
{
  T_STR arch_return = (T_STR)"";  // initialize variable

  _status = TipcSrvGetArch(&arch_return);
  return arch_return;
}


/* ========================================================================= */
/*..TipcSrv::ConnStatus -- get the status of the connection to RTserver */
T_IPC_SRV_CONN_STATUS TipcSrv::ConnStatus()
{
  T_IPC_SRV_CONN_STATUS conn_status;

  _status = TipcSrvGetConnStatus(&conn_status);
  return conn_status;
}


/* ========================================================================= */
/*..TipcSrv::GmdFileCreate -- create files needed for guaranteed message */
/*..  delivery on the connection to RTserver */
T_BOOL TipcSrv::GmdFileCreate()
{
  _status = TipcSrvGmdFileCreate();
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::GmdFileDelete -- delete from disk guaranteed message delivery */
/*..  files for the connection to RTserver */
T_BOOL TipcSrv::GmdFileDelete()
{
  _status = TipcSrvGmdFileDelete();
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::GmdResend -- resend guaranteed messages after a delivery  */
/*..  failure on the connection to RTserver */
T_BOOL TipcSrv::GmdResend()
{
  _status = TipcSrvGmdResend();
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::GmdMsgDelete -- delete an outgoing message from the connection */
/*..  to RTserver GMD files */
T_BOOL TipcSrv::GmdMsgDelete(TipcMsg& msg)
{
  _status = TipcSrvGmdMsgDelete(msg._msg);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::GmdMsgResend -- resend on spooled guaranteed message on the */
/*..  connection to RTserver */
T_BOOL TipcSrv::GmdMsgResend(TipcMsg& msg)
{
  _status = TipcSrvGmdMsgResend(msg._msg);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::GmdMaxSize -- get gmd max size for a connection */
T_UINT4 TipcSrv::GmdMaxSize()
{
  T_UINT4 gmd_max_size_return = 0;  // initialize varible

  _status = TipcSrvGetGmdMaxSize(&gmd_max_size_return);
  return gmd_max_size_return;
}


/* ========================================================================= */
/*..TipcSrv::GmdMaxSize -- set gmd max size for a connection */
T_BOOL TipcSrv::GmdMaxSize(T_UINT4 gmd_max_size)
{
  _status = TipcSrvSetGmdMaxSize( gmd_max_size);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::GmdMsgServerDelete -- tell RTserver to forget about a GMD */
/*..  message */
T_BOOL TipcSrv::GmdMsgServerDelete(TipcMsg& msg)
{
  _status = TipcSrvGmdMsgServerDelete(msg._msg);
  return _status;
}


/* ========================================================================= */
/*..TipcSrv::GmdMsgStatus -- query RTserver for GMD status */
T_BOOL TipcSrv::GmdMsgStatus(TipcMsg& msg)
{
  _status = TipcSrvGmdMsgStatus(msg._msg);
  return _status;
}

#ifndef T_SS35_HIERARCHY

/* ========================================================================= */
/*..TipcSrv::SubjectSubscribeLb -- get subscribe w/ load balancing */
T_BOOL TipcSrv::SubjectSubscribeLb(T_STR subject,
                                   T_BOOL *lb_status_return)
{
  T_BOOL subscribe_status_return;

  _status = TipcSrvSubjectGetSubscribeLb(subject,
                                         &subscribe_status_return,
                                         lb_status_return);
  return subscribe_status_return;
}

/* ========================================================================= */
/*..TipcSrv::SubjectSubscribeLb -- set subscribe w/ load balancing */
T_BOOL TipcSrv::SubjectSubscribeLb(T_STR subject,
                                   T_BOOL subject_status,
                                   T_BOOL lb_status)
{
  _status = TipcSrvSubjectSetSubscribeLb(subject, subject_status, lb_status);
  return _status;
}

/* ========================================================================= */
/*..TipcSrv::SubjectGmdInit -- turn on GMD for a subject */
T_BOOL TipcSrv::SubjectGmdInit(T_STR subject)
{
  _status = TipcSrvSubjectGmdInit(subject);
  return _status;
}

/* ========================================================================= */
/*..TipcSrv::SubjectLbInit -- turn on Lb for a subject */
T_BOOL TipcSrv::SubjectLbInit(T_STR subject)
{
  _status = TipcSrvSubjectLbInit(subject);
  return _status;
}

/* ========================================================================= */
/*..TipcSrv::Lock -- acquire exclusive access to the server connection */
T_BOOL TipcSrv::Lock()
{
  _status = TipcSrvLock();
  return _status;
}

/* ========================================================================= */
/*..TipcSrv::Unlock -- release exclusive access to the server connection */
T_BOOL TipcSrv::Unlock()
{
  _status = TipcSrvUnlock();
  return _status;
}

/* ========================================================================= */
/*..TipcSrv::GmdNumPending -- get the number of messages pending GMD */
T_INT4 TipcSrv::GmdNumPending()
{
  T_INT4 num_return;

  _status = TipcSrvGetGmdNumPending(&num_return);
  return num_return;
}


/* ========================================================================= */
/*..TipcSrv::SubjectCbCreate -- creates a srv subject callback */
T_CB TipcSrv::SubjectCbCreate(T_STR subject,
                               T_IPC_MT mt,
                               T_IPC_SRV_SUBJECT_CB_FUNC func,
                               T_CB_ARG arg)
{
   T_CB result;

   _status = T_FALSE;

   result = TipcSrvSubjectCbCreate(subject, mt, func, arg);

   if (T_NULL != result) {
      _status = T_TRUE;
   }

   return result;
} /*TipcSrv::SubjectCbCreate*/


/* ========================================================================= */
/*..TipcSrv::SubjectCbLookup -- lookup srv subject callback */
T_CB TipcSrv::SubjectCbLookup(T_STR subject,
                              T_IPC_MT mt,
                              T_IPC_SRV_SUBJECT_CB_FUNC func,
                              T_CB_ARG arg)
{
   T_CB result;

   _status = T_FALSE;

   result = TipcSrvSubjectCbLookup(subject, mt, func, arg);

   if (T_NULL != result) {
      _status = T_TRUE;
   }

   return result;
} /*TipcSrv::SubjectCbLookup*/


/* ========================================================================= */
/*..TipcSrv::SubjectDefaultCbCreate -- creates the default srv subject cb */
T_CB TipcSrv::SubjectDefaultCbCreate(T_IPC_SRV_SUBJECT_CB_FUNC func,
                                     T_CB_ARG arg)
{
   T_CB result;

   _status = T_FALSE;

   result = TipcSrvSubjectDefaultCbCreate(func, arg);

   if (T_NULL != result) {
      _status = T_TRUE;
   }

   return result;
} /*TipcSrv::SubjectDefaultCbCreate*/


/* ========================================================================= */
/*..TipcSrv::SubjectDefaultCbLookup -- lookup the default srv subject cb */
T_CB TipcSrv::SubjectDefaultCbLookup(T_IPC_SRV_SUBJECT_CB_FUNC func,
                                     T_CB_ARG arg)
{
   T_CB result;

   _status = T_FALSE;

   result = TipcSrvSubjectDefaultCbLookup(func, arg);

   if (T_NULL != result) {
      _status = T_TRUE;
   }

   return result;
} /*TipcSrv::SubjectDefaultCbLookup*/


/* ========================================================================= */
/*..TipcSrv::SubjectCbDestroyAll -- Destroy all srv subject callbacks */
T_BOOL TipcSrv::SubjectCbDestroyAll()
{
   _status = TipcSrvSubjectCbDestroyAll();
   return _status;
} /*TipcSrv::SubjectCbDestroyAll*/


#endif
