/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tconn.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tconn.hxx>

#ifdef T_SS35_HIERARCHY

/* ========================================================================= */
/*..TipcConn::TipcConn -- TipcConn constructor */
TipcConn::TipcConn(T_IPC_CONN connection, T_BOOL destroy_flag)
{
  _destroy_flag = destroy_flag;
  _connection = connection;
}

/* ========================================================================= */
/*..TipcConn::TipcConn -- TipcConn constructor */
TipcConn::TipcConn(T_BOOL do_nothing)
{
  // this is a kludge to allow TipcConn[Server|Client] to work
  // without any really ridiculous gyrations.   JAM 03/07/97
  if (T_FALSE == do_nothing) {
    _destroy_flag = T_TRUE;
    _connection = TipcConnCreate();
  }
}

/* ========================================================================= */
/*..TipcConn::~TipcConn -- TipcConn destructor */
TipcConn::~TipcConn()
{
  // TipcConnWrapper base class will destroy
}

#else

/* ========================================================================= */
/*..TipcConn::TipcConn -- TipcConn constructor */
TipcConn::TipcConn()
{
  _destroy_flag = T_TRUE;
  _connection = TipcConnCreate();
}

/* ========================================================================= */
/*..TipcConn::TipcConn -- TipcConn constructor */
TipcConn::TipcConn(T_IPC_CONN connection, T_BOOL destroy_flag)
{
  _status = T_TRUE;
  _destroy_flag = destroy_flag;
  _connection = connection;
}

/* ========================================================================= */
/*..TipcConn::TipcConn -- TipcConn constructor */
TipcConn::TipcConn(T_BOOL do_nothing)
{
  _status = T_TRUE;

  // this is a kludge to allow TipcConn[Server|Client] to work
  // without any really ridiculous gyrations.   JAM 03/07/97
  if (T_FALSE == do_nothing) {
    _destroy_flag = T_TRUE;
    _connection = TipcConnCreate();
  }
}

/* ========================================================================= */
/*..TipcConn::~TipcConn -- TipcConn destructor */
TipcConn::~TipcConn()
{
  if (_destroy_flag) {
    TipcConnDestroy(_connection);
  }
}

/* ========================================================================= */
/*..TipcConn::Check -- check if data can be read from or written  */
/*..  to a connection */
T_BOOL TipcConn::Check(T_IO_CHECK_MODE check_mode, T_REAL8 timeout)
{
  _status = TipcConnCheck(_connection, check_mode, timeout);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::ErrorCbCreate -- create an error callback in a  */
/*..  connection */
T_CB  TipcConn::ErrorCbCreate(T_IPC_CONN_ERROR_CB_FUNC func, T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnErrorCbCreate(_connection, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcConn::ErrorCbLookup -- look up an error callback in a  */
/*..  connection */
T_CB  TipcConn::ErrorCbLookup(T_IPC_CONN_ERROR_CB_FUNC func, T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnErrorCbLookup(_connection, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcConn::Flush -- flush buffered outgoing messages to a  */
/*..  connection's socket */
T_BOOL TipcConn::Flush()
{
  _status = TipcConnFlush(_connection);
  return _status;
}

/* ========================================================================= */
/*..TipcConn::AutoFlushSize -- get the automatic flush size of a */
/*..  connection */
T_INT4 TipcConn::AutoFlushSize()
{
  T_INT4 auto_flush_size_return = -1;  // negative values are illegal

  _status = TipcConnGetAutoFlushSize(_connection, &auto_flush_size_return);
  return auto_flush_size_return;
}


/* ========================================================================= */
/*..TipcConn::AutoFlushSize -- set the automatic flush size of a */
/*..  connection */
T_BOOL TipcConn::AutoFlushSize(T_INT4 auto_flush_size)
{
  _status = TipcConnSetAutoFlushSize(_connection, auto_flush_size);

  return _status;
}


/* ========================================================================= */
/*..TipcConn::BlockMode -- get the block mode of a connection */
T_BOOL TipcConn::BlockMode()
{
  T_BOOL block_mode_return = T_TRUE;  // initialize variable

  _status = TipcConnGetBlockMode(_connection, &block_mode_return);
  return block_mode_return;
}


/* ========================================================================= */
/*..TipcConn::BlockMode -- set the block mode of a connection */
T_BOOL TipcConn::BlockMode(T_BOOL block_mode)
{
  _status = TipcConnSetBlockMode(_connection, block_mode);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::NumQueued -- get number of queued messages from a  */
/*..  connection */
T_INT4 TipcConn::NumQueued()
{
  T_INT4 num_queued_return = -1;  // initialize variable

  _status = TipcConnGetNumQueued(_connection, &num_queued_return);
  return num_queued_return;
}


/* ========================================================================= */
/*..TipcConn::Socket -- get the socket of a connection */
T_SOCK TipcConn::Socket()
{
  T_SOCK socket_return = - 1;  // initialize variable

  _status = TipcConnGetSocket(_connection, &socket_return);
  return socket_return;
}


/* ========================================================================= */
/*..TipcConn::Socket -- set the socket of a connection */
T_BOOL TipcConn::Socket(T_SOCK sock)
{
  _status = TipcConnSetSocket(_connection, sock);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::Timeout -- get a timeout property of a connection */
T_REAL8 TipcConn::Timeout(T_IPC_TIMEOUT timeout)
{
  T_REAL8 value_return = -1.0;  // initialize variable

  _status = TipcConnGetTimeout(_connection, timeout, &value_return);
  return value_return;
}


/* ========================================================================= */
/*..TipcConn::Timeout -- set a timeout property of a connection */
T_BOOL TipcConn::Timeout(T_IPC_TIMEOUT timeout, T_REAL8 value)
{
  _status = TipcConnSetTimeout(_connection, timeout, value);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::XtSource -- get source suitable for XtAppAddInput from */
/*..   a connection */
T_SOCK TipcConn::XtSource()
{
  T_SOCK source_return = -1;  // initialize variable

  _status = TipcConnGetXtSource(_connection, &source_return);
  return source_return;
}


/* ========================================================================= */
/*..TipcConn::KeepAlive -- check if the process at the other end of */
/*..  a connection is still alive */
T_BOOL TipcConn::KeepAlive()
{
  _status = TipcConnKeepAlive(_connection);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::MainLoop -- read and process message on a connection */
T_BOOL TipcConn::MainLoop(T_REAL8 timeout)
{
  _status = TipcConnMainLoop(_connection, timeout);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::Insert -- insert a message into queue of a connection */
T_BOOL TipcConn::Insert(TipcMsg& msg,
            T_INT4 pos)
{
  _status = TipcConnMsgInsert(_connection, msg._msg, pos);
  if (_status) {
    /* ---------------------------------------------------------- */
    /* Since TipcConnMsgInsert takes destroy responsibility from  */
    /* the caller, convert TipcMsg object into a vacant object.   */
    /* ---------------------------------------------------------- */
    msg._msg = (T_IPC_MSG)T_NULL;
  }
  return _status;
}


/* ========================================================================= */
/*..TipcConn::Next -- get the next message from a connection */
TipcMsg& TipcConn::Next(T_REAL8 timeout)
{
  _status = T_FALSE;

  _msg_referent.destroyMsg();

  _msg_referent.absorb(TipcConnMsgNext(_connection, timeout));

  if (T_NULL != _msg_referent._msg) {
    _status = T_TRUE;
  }

  return _msg_referent;
}


/* ========================================================================= */
/*..TipcConn::MsgNext -- get the next message from a connection */
TipcMsg& TipcConn::MsgNext(T_REAL8 timeout)
{
  _status = T_FALSE;

  TipcMsg *return_msg = new TipcMsg();

  T_IPC_MSG msg = TipcConnMsgNext(_connection, timeout);
  if (T_NULL != msg) {
    return_msg->absorb(msg);
  }

  if (T_NULL != return_msg->_msg) {
    _status = T_TRUE;
  }

  return *return_msg;
}


/* ========================================================================= */
/*..TipcConn::Process -- process a message in a connection */
T_BOOL TipcConn::Process(TipcMsg& msg)
{
  _status = TipcConnMsgProcess(_connection, msg._msg);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::Search -- search the message queue of a connection for */
/*..  a specific message */
TipcMsg&  TipcConn::Search(T_REAL8 timeout,
               T_IPC_CONN_MSG_SEARCH_FUNC func,
                        T_PTR arg)
{
  _status = T_FALSE;

  _msg_referent.destroyMsg();

  _msg_referent.absorb(TipcConnMsgSearch(_connection, timeout, func, arg));

  if (T_NULL != _msg_referent._msg) {
    _status = T_TRUE;
  }

  return _msg_referent;
}


/* ========================================================================= */
/*..TipcConn::MsgSearch -- search the message queue of a connection for */
/*..  a specific message */
TipcMsg&  TipcConn::MsgSearch(T_REAL8 timeout,
                           T_IPC_CONN_MSG_SEARCH_FUNC func,
                           T_PTR arg)
{
  _status = T_FALSE;

  TipcMsg *return_msg = new TipcMsg();

  T_IPC_MSG msg = TipcConnMsgSearch(_connection, timeout, func, arg);
  if (T_NULL != msg) {
    return_msg->absorb(msg);
  }

  if (T_NULL != return_msg->_msg) {
    _status = T_TRUE;
  }

  return *return_msg;
}


/* ========================================================================= */
/*..TipcConn::SearchType -- search the message queue of a connection */
/*..  for a message with specific type */
TipcMsg& TipcConn::SearchType(T_REAL8 timeout,
                           TipcMt& mt)
{
  _status = T_FALSE;

  _msg_referent.destroyMsg();

  _msg_referent.absorb(TipcConnMsgSearchType(_connection, timeout, mt._mt));

  if (T_NULL != _msg_referent._msg) {
    _status = T_TRUE;
  }

  return _msg_referent;
}


/* ========================================================================= */
/*..TipcConn::MsgSearchType -- search the message queue of a connection */
/*..  for a message with specific type */
TipcMsg& TipcConn::MsgSearchType(T_REAL8 timeout,
                              TipcMt& mt)
{
  _status = T_FALSE;

  TipcMsg *return_msg = new TipcMsg();

  T_IPC_MSG msg = TipcConnMsgSearchType(_connection, timeout, mt._mt);
  if (T_NULL != msg) {
    return_msg->absorb(msg);
  }

  if (T_NULL != return_msg->_msg) {
    _status = T_TRUE;
  }

  return *return_msg;
}


/* ========================================================================= */
/*..TipcConn::Send -- send a message through a connection */
T_BOOL  TipcConn::Send(TipcMsg& msg,
               T_BOOL rtserver_flag)
{
  /* --------------------------------------------------------------- */
  /* The following line is necessary to eliminate a compiler warning */
  /* we are tired of looking at.                                     */
  /* --------------------------------------------------------------- */
  /* this blew Sun CC's mind on Solaris x86.  SMW 09/13/95 */
  /*if (rtserver_flag);*/
  if (rtserver_flag) {
    /*do nothing*/
  } /* if */

  _status = TipcConnMsgSend(_connection, msg._msg);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::SendRpc -- make a remote procedure call with messages */
/*..  on a connection */
TipcMsg& TipcConn::SendRpc(TipcMsg& call_msg,
               T_REAL8 timeout)
{
  _status = T_FALSE;

  _msg_referent.destroyMsg();

  _msg_referent.absorb(TipcConnMsgSendRpc(_connection, call_msg._msg, timeout));

  if (T_NULL != _msg_referent._msg) {
    _status = T_TRUE;
  }

  return _msg_referent;
}


/* ========================================================================= */
/*..TipcConn::MsgSendRpc -- make a remote procedure call with messages */
/*..  on a connection */
TipcMsg& TipcConn::MsgSendRpc(TipcMsg& call_msg,
                  T_REAL8 timeout)
{
  _status = T_FALSE;

  TipcMsg *return_msg = new TipcMsg();

  T_IPC_MSG msg = TipcConnMsgSendRpc(_connection, call_msg._msg, timeout);
  if (T_NULL != msg) {
    return_msg->absorb(msg);
  }

  if (T_NULL != return_msg->_msg) {
    _status = T_TRUE;
  }

  return *return_msg;
}


/* ========================================================================= */
/*..TipcConn::Write -- construct a message and send it through a */
/*..  connection */
#ifdef T_OS_WINDOWS
/* see the header file for an explanation of this ifdef */
T_BOOL TipcConn::Write(TipcMt mt, ...)
#else
T_BOOL TipcConn::Write(TipcMt& mt, ...)
#endif
{
  // See RTworks & IPC manual, pg 10-83 ff.

  va_list var_arg_list;   // To point to each unnamed arg in turn

  va_start(var_arg_list, mt);
  _status = TipcConnMsgWriteVa(_connection, mt._mt, var_arg_list);
  va_end(var_arg_list);

  return _status;
}


/* ========================================================================= */
/*..TipcConn::WriteVa -- construct a message and send it through a */
/*..  connection */
#ifdef T_OS_WINDOWS
/* see the header file for an explanation of this ifdef */
T_BOOL TipcConn::WriteVa(TipcMt mt,
             va_list var_arg_list)
#else
T_BOOL TipcConn::WriteVa(TipcMt& mt,
             va_list var_arg_list)
#endif
{
  _status = TipcConnMsgWriteVa(_connection, mt._mt, var_arg_list);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::Print -- print all information about a connection */
T_BOOL TipcConn::Print(T_OUT_FUNC func)
{
  _status = TipcConnPrint(_connection, func);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::DefaultCbCreate -- create a default callback in a */
/*..  connection */
T_CB TipcConn::DefaultCbCreate(T_IPC_CONN_DEFAULT_CB_FUNC func, T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnDefaultCbCreate(_connection, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcConn::DefaultCbLookup -- look up a default callback in a */
/*..  connection */
T_CB TipcConn::DefaultCbLookup(T_IPC_CONN_DEFAULT_CB_FUNC func, T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnDefaultCbLookup(_connection, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcConn::ProcessCbCreate -- create a process callback in a */
/*..  connection */
T_CB TipcConn::ProcessCbCreate(TipcMt& mt,
                               T_IPC_CONN_PROCESS_CB_FUNC func,
                               T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnProcessCbCreate(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcConn::ProcessCbLookup -- look up a process callback in a */
/*..  connection */
T_CB TipcConn::ProcessCbLookup(TipcMt& mt,
                               T_IPC_CONN_PROCESS_CB_FUNC func,
                               T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnProcessCbLookup(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcConn::QueueCbCreate -- create a queue callback in a */
/*..  connection */
T_CB TipcConn::QueueCbCreate(TipcMt& mt,
                             T_IPC_CONN_QUEUE_CB_FUNC func,
                             T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnQueueCbCreate(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcConn::QueueCbLookup -- look up a queue callback in a */
/*..  connection */
T_CB TipcConn::QueueCbLookup(TipcMt& mt,
                             T_IPC_CONN_QUEUE_CB_FUNC func,
                             T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnQueueCbLookup(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcConn::Read -- read all available data from a connection */
/*..  and queue messages in priority order */
T_BOOL TipcConn::Read(T_REAL8 timeout)
{
  _status = TipcConnRead(_connection, timeout);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::ReadCbCreate -- create a read callback in a connection */
T_CB TipcConn::ReadCbCreate(TipcMt& mt,
                            T_IPC_CONN_READ_CB_FUNC func,
                            T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnReadCbCreate(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}

/* ========================================================================= */
/*..TipcConn::ReadCbLookup -- look up a read callback in a connection */
T_CB TipcConn::ReadCbLookup(TipcMt& mt,
                            T_IPC_CONN_READ_CB_FUNC func,
                            T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnReadCbLookup(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}

/* ========================================================================= */
/*..TipcConn::WriteCbCreate -- create a write callback in a */
/*..  connection */
T_CB TipcConn::WriteCbCreate(TipcMt& mt,
                             T_IPC_CONN_WRITE_CB_FUNC func,
                             T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnWriteCbCreate(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcConn::WriteCbLookup -- look up a write callback in a */
/*..  connection */
T_CB TipcConn::WriteCbLookup(TipcMt& mt,
                             T_IPC_CONN_WRITE_CB_FUNC func,
                             T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnWriteCbLookup(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}

/* ========================================================================= */
/*..TipcConn::EncryptCbCreate -- create a encrypt callback in a connection */
T_CB TipcConn::EncryptCbCreate(TipcMt& mt,
                               T_IPC_CONN_ENCRYPT_CB_FUNC func,
                               T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnEncryptCbCreate(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}

/* ========================================================================= */
/*..TipcConn::EncryptCbLookup -- look up a encrypt callback in a connection */
T_CB TipcConn::EncryptCbLookup(TipcMt& mt,
                               T_IPC_CONN_ENCRYPT_CB_FUNC func,
                               T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnEncryptCbLookup(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}

/* ========================================================================= */
/*..TipcConn::DecryptCbCreate -- create a decrypt callback in a */
/*..  connection */
T_CB TipcConn::DecryptCbCreate(TipcMt& mt,
                               T_IPC_CONN_DECRYPT_CB_FUNC func,
                               T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnDecryptCbCreate(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}

/* ========================================================================= */
/*..TipcConn::DecryptCbLookup -- look up a decrypt callback in a */
/*..  connection */
T_CB TipcConn::DecryptCbLookup(TipcMt& mt,
                               T_IPC_CONN_DECRYPT_CB_FUNC func,
                               T_CB_ARG arg)
{
  T_CB result;

  _status = T_FALSE;

  result = TipcConnDecryptCbLookup(_connection, mt._mt, func, arg);

  if (T_NULL != result) {
    _status = T_TRUE;
  }

  return result;
}


/* ========================================================================= */
/*..TipcConn::UniqueSubject -- get the unique subject of a */
/*..  connection's peer process */
T_STR TipcConn::UniqueSubject()
{
  T_STR unique_subject_return = (T_STR)""; // initialize variable

  _status = TipcConnGetUniqueSubject(_connection, &unique_subject_return);
  return unique_subject_return;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcConn::UniqueDatagroup -- get the unique datagroup of a */
/*..  connection's peer process */
T_STR TipcConn::UniqueDatagroup()
{
  return UniqueSubject();
}
#endif

/* ========================================================================= */
/*..TipcConn::Node -- get the node name of a connection's peer */
/*..  process */
T_STR TipcConn::Node()
{
  T_STR node_return = (T_STR)"";  // initialize variable

  _status = TipcConnGetNode(_connection, &node_return);
  return node_return;
}


/* ========================================================================= */
/*..TipcConn::User -- get the user name of a connection's peer */
/*..  process */
T_STR TipcConn::User()
{
  T_STR user_return = (T_STR)""; // initialize variable

  _status = TipcConnGetUser(_connection, &user_return);
  return user_return;
}


/* ========================================================================= */
/*..TipcConn::Pid -- get the process ID of a connection's peer */
/*..  process */
T_INT4 TipcConn::Pid()
{
  T_INT4 pid_return = -1;  // initialize variable

  _status = TipcConnGetPid(_connection, &pid_return);
  return pid_return;
}


/* ========================================================================= */
/*..TipcConn::Arch -- get the architecture name of a connection's */
/*..  peer process */
T_STR TipcConn::Arch()
{
  T_STR arch_return = (T_STR)"";  // initialize variable

  _status = TipcConnGetArch(_connection, &arch_return);
  return arch_return;
}


/* ========================================================================= */
/*..TipcConn::GmdFileCreate -- create files needed for GMD on a */
/*..  connection */
T_BOOL TipcConn::GmdFileCreate()
{
  _status = TipcConnGmdFileCreate(_connection);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::GmdFileDelete -- delete from disk GMD files for a */
/*..  connection */
T_BOOL TipcConn::GmdFileDelete()
{
  _status = TipcConnGmdFileDelete(_connection);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::GmdResend -- resend quaranteed messages after a */
/*..  delivery failure on a connection */
T_BOOL TipcConn::GmdResend()
{
  _status = TipcConnGmdResend(_connection);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::GmdMsgDelete -- delete an outgoing message from */
/*..  connection GMD files */
T_BOOL TipcConn::GmdMsgDelete(TipcMsg& msg)
{
  _status = TipcConnGmdMsgDelete(_connection, msg._msg);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::GmdMsgResend -- resend one spooled guaranteed message */
/*..  on a connection */
T_BOOL TipcConn::GmdMsgResend(TipcMsg& msg)
{
  _status = TipcConnGmdMsgResend(_connection, msg._msg);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::GmdMaxSize -- get gmd max size for a connection */
T_UINT4 TipcConn::GmdMaxSize()
{
  T_UINT4 gmd_max_size_return = 0; // initialize variable

  _status = TipcConnGetGmdMaxSize(_connection, &gmd_max_size_return);
  return gmd_max_size_return;
}


/* ========================================================================= */
/*..TipcConn::GmdMaxSize -- set gmd max size for a connection */
T_BOOL TipcConn::GmdMaxSize(T_UINT4 gmd_max_size)
{
  _status = TipcConnSetGmdMaxSize(_connection, gmd_max_size);
  return _status;
}


/* ========================================================================= */
/*..TipcConn::GmdDir -- get name of directory where files are */
/*..  written for GMD */
T_STR TipcConn::GmdDir()
{
  return TipcGetGmdDir();
}

/* ========================================================================= */
/*.. TipcConn::EncodeCbCreate -- create an encode callback */
T_CB TipcConn::EncodeCbCreate(T_IPC_CONN_ENCODE_CB_FUNC func, T_CB_ARG arg)
{
  T_CB cb_return;

  cb_return = TipcConnEncodeCbCreate(_connection, func, arg);
  _status = (cb_return) ? T_TRUE : T_FALSE;

  return cb_return;
}

/* ========================================================================= */
/*.. TipcConn::EncodeCbLookup -- lookup an encode callback */
T_CB TipcConn::EncodeCbLookup(T_IPC_CONN_ENCODE_CB_FUNC func, T_CB_ARG arg)
{
  T_CB cb_return;

  cb_return = TipcConnEncodeCbLookup(_connection, func, arg);
  _status = (cb_return) ? T_TRUE : T_FALSE;

  return cb_return;
}

/* ========================================================================= */
/*.. TipcConn::DecodeCbCreate -- create a decode callback */
T_CB TipcConn::DecodeCbCreate(T_IPC_CONN_DECODE_CB_FUNC func, T_CB_ARG arg)
{
  T_CB cb_return;

  cb_return = TipcConnDecodeCbCreate(_connection, func, arg);
  _status = (cb_return) ? T_TRUE : T_FALSE;

  return cb_return;
}

/* ========================================================================= */
/*.. TipcConn::DecodeCbLookup -- lookup a decode callback */
T_CB TipcConn::DecodeCbLookup(T_IPC_CONN_DECODE_CB_FUNC func, T_CB_ARG arg)
{
  T_CB cb_return;

  cb_return = TipcConnDecodeCbLookup(_connection, func, arg);
  _status = (cb_return) ? T_TRUE : T_FALSE;

  return cb_return;
}

#ifndef T_SS35_HIERARCHY

/* ========================================================================= */
/*.. TipcConn::Lock() -- acquire exclusive access to connection */
T_BOOL TipcConn::Lock()
{
  _status = TipcConnLock(_connection);
  return _status;
}

/* ========================================================================= */
/*.. TipcConn::Unlock() -- release exclusive access to connection */
T_BOOL TipcConn::Unlock()
{
  _status = TipcConnUnlock(_connection);
  return _status;
}

/* ========================================================================= */
/*.. TipcConn::GmdNumPending -- get the number of messages pending GMD */
T_INT4 TipcConn::GmdNumPending()
{
  T_INT4 num_pending = 0;

  _status = TipcConnGetGmdNumPending(_connection, &num_pending);
  return num_pending;
}

#endif
#endif

/* ========================================================================= */
/* wrappers for server and client connections */

/* ========================================================================= */
/*..TipcConnServer::TipcConnServer -- TipcConnServer constructor */
TipcConnServer::TipcConnServer(T_STR link_name) : TipcConn(T_TRUE)
{
  _destroy_flag = T_TRUE;
  _connection = TipcConnCreateServer(link_name);
  if (T_NULL == _connection) {
    _status = T_FALSE;
  }
}

/* ========================================================================= */
/*..TipcConnServer::TipcConnServer -- TipcConnServer constructor */
TipcConnServer::TipcConnServer(T_IPC_CONN connection, T_BOOL destroy_flag)
  : TipcConn(connection,destroy_flag)
{
}

/* ========================================================================= */
/*..TipcConnServer::Accept -- accept and create a new TipcConn */
TipcConn *TipcConnServer::Accept(void)
{
  T_IPC_CONN accept_conn = TipcConnAccept(_connection);

  if (T_NULL == accept_conn) {
    _status = T_FALSE;
    return (TipcConn *)T_NULL;
  }
  else {
    _status = T_TRUE;
  }

  return new TipcConn(accept_conn);
}

/* ========================================================================= */
/*..TipcConnClient::TipcConnClient -- TipcConnClient constructor */
TipcConnClient::TipcConnClient(T_STR link_name) : TipcConn(T_TRUE)
{
  _destroy_flag = T_TRUE;
  _connection = TipcConnCreateClient(link_name);
  if (T_NULL == _connection) {
    _status = T_FALSE;
  }
}

/* ========================================================================= */
/*..TipcConnClient::TipcConnClient -- TipcConnClient constructor */
TipcConnClient::TipcConnClient(T_IPC_CONN connection, T_BOOL destroy_flag)
  : TipcConn(connection,destroy_flag)
{
}
