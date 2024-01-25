/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tipccw.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmsg.hxx>
#include <rtworks/tipccw.hxx>

#ifdef T_SS35_HIERARCHY

/* ========================================================================= */
/*..TipcConnWrapper::TipcConnWrapper -- TipcConnWrapper constructor */
TipcConnWrapper::TipcConnWrapper()
{
  _status = T_TRUE;
  // Derived classes will set _connection at construction
}

/* ========================================================================= */
/*..TipcConnWrapper::~TipcConnWrapper -- TipcConnWrapper destructor */
TipcConnWrapper::~TipcConnWrapper()
{
  if (_destroy_flag) {
    TipcConnDestroy(_connection);
  }

}


/* ========================================================================= */
/*..TipcConnWrapper::Check -- check if data can be read from or written  */
/*..  to a connection */
T_BOOL TipcConnWrapper::Check(T_IO_CHECK_MODE check_mode, T_REAL8 timeout)
{
  _status = TipcConnCheck(_connection, check_mode, timeout);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::ErrorCbCreate -- create an error callback in a  */
/*..  connection */
T_CB  TipcConnWrapper::ErrorCbCreate(T_IPC_CONN_ERROR_CB_FUNC func,
                                     T_CB_ARG arg)
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
/*..TipcConnWrapper::ErrorCbLookup -- look up an error callback in a  */
/*..  connection */
T_CB  TipcConnWrapper::ErrorCbLookup(T_IPC_CONN_ERROR_CB_FUNC func,
                                     T_CB_ARG arg)
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
/*..TipcConnWrapper::Flush -- flush buffered outgoing messages to a  */
/*..  connection's socket */
T_BOOL TipcConnWrapper::Flush()
{
  _status = TipcConnFlush(_connection);
  return _status;
}

/* ========================================================================= */
/*..TipcConnWrapper::AutoFlushSize -- get the automatic flush size of a */
/*..  connection */
T_INT4 TipcConnWrapper::AutoFlushSize()
{
  T_INT4 auto_flush_size_return = -1;  // negative values are illegal

  _status = TipcConnGetAutoFlushSize(_connection, &auto_flush_size_return);
  return auto_flush_size_return;
}


/* ========================================================================= */
/*..TipcConnWrapper::AutoFlushSize -- set the automatic flush size of a */
/*..  connection */
T_BOOL TipcConnWrapper::AutoFlushSize(T_INT4 auto_flush_size)
{
  _status = TipcConnSetAutoFlushSize(_connection, auto_flush_size);

  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::BlockMode -- get the block mode of a connection */
T_BOOL TipcConnWrapper::BlockMode()
{
  T_BOOL block_mode_return = T_TRUE;  // initialize variable

  _status = TipcConnGetBlockMode(_connection, &block_mode_return);
  return block_mode_return;
}


/* ========================================================================= */
/*..TipcConnWrapper::BlockMode -- set the block mode of a connection */
T_BOOL TipcConnWrapper::BlockMode(T_BOOL block_mode)
{
  _status = TipcConnSetBlockMode(_connection, block_mode);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::NumQueued -- get number of queued messages from a  */
/*..  connection */
T_INT4 TipcConnWrapper::NumQueued()
{
  T_INT4 num_queued_return = -1;  // initialize variable

  _status = TipcConnGetNumQueued(_connection, &num_queued_return);
  return num_queued_return;
}


/* ========================================================================= */
/*..TipcConnWrapper::Socket -- get the socket of a connection */
T_SOCK TipcConnWrapper::Socket()
{
  T_SOCK socket_return = - 1;  // initialize variable

  _status = TipcConnGetSocket(_connection, &socket_return);
  return socket_return;
}


/* ========================================================================= */
/*..TipcConnWrapper::Socket -- set the socket of a connection */
T_BOOL TipcConnWrapper::Socket(T_SOCK sock)
{
  _status = TipcConnSetSocket(_connection, sock);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::Timeout -- get a timeout property of a connection */
T_REAL8 TipcConnWrapper::Timeout(T_IPC_TIMEOUT timeout)
{
  T_REAL8 value_return = -1.0;  // initialize variable

  _status = TipcConnGetTimeout(_connection, timeout, &value_return);
  return value_return;
}


/* ========================================================================= */
/*..TipcConnWrapper::Timeout -- set a timeout property of a connection */
T_BOOL TipcConnWrapper::Timeout(T_IPC_TIMEOUT timeout, T_REAL8 value)
{
  _status = TipcConnSetTimeout(_connection, timeout, value);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::XtSource -- get source suitable for XtAppAddInput from */
/*..   a connection */
T_BOOL TipcConnWrapper::XtSource()
{
  T_INT4 source_return = -1;  // initialize variable

  _status = TipcConnGetXtSource(_connection, &source_return);
  return source_return;
}


/* ========================================================================= */
/*..TipcConnWrapper::KeepAlive -- check if the process at the other end of */
/*..  a connection is still alive */
T_BOOL TipcConnWrapper::KeepAlive()
{
  _status = TipcConnKeepAlive(_connection);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::MainLoop -- read and process message on a connection */
T_BOOL TipcConnWrapper::MainLoop(T_REAL8 timeout)
{
  _status = TipcConnMainLoop(_connection, timeout);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::Insert -- insert a message into queue of a connection */
T_BOOL TipcConnWrapper::Insert(TipcMsg& msg, T_INT4 pos)
{
  _status = TipcConnMsgInsert(_connection, msg._msg, pos);
  if (_status) {
    /* ---------------------------------------------------------- */
    /* Since TipcConnMsgInsert takes destroy responsibility from  */
    /* the caller, convert TipcMsg object into a vacant object.   */
    /* ---------------------------------------------------------- */
    msg._msg = T_NULL;
  }
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::Next -- get the next message from a connection */
TipcMsg& TipcConnWrapper::Next(T_REAL8 timeout)
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
/*..TipcConnWrapper::MsgNext -- get the next message from a connection */
TipcMsg& TipcConnWrapper::MsgNext(T_REAL8 timeout)
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
/*..TipcConnWrapper::Process -- process a message in a connection */
T_BOOL TipcConnWrapper::Process(TipcMsg& msg)
{
  _status = TipcConnMsgProcess(_connection, msg._msg);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::Search -- search the message queue of a connection for */
/*..  a specific message */
TipcMsg&  TipcConnWrapper::Search(T_REAL8 timeout,
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
/*..TipcConnWrapper::MsgSearch -- search the message queue of a connection */
/*..  for a specific message */
TipcMsg&  TipcConnWrapper::MsgSearch(T_REAL8 timeout,
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
/*..TipcConnWrapper::SearchType -- search the message queue of a connection */
/*..  for a message with specific type */
TipcMsg& TipcConnWrapper::SearchType(T_REAL8 timeout, TipcMt& mt)
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
/*..TipcConnWrapper::MsgSearchType -- search the message queue of a connection*/
/*..  for a message with specific type */
TipcMsg& TipcConnWrapper::MsgSearchType(T_REAL8 timeout, TipcMt& mt)
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

  return return_msg;
}


/* ========================================================================= */
/*..TipcConnWrapper::Send -- send a message through a connection */
T_BOOL  TipcConnWrapper::Send(TipcMsg& msg, T_BOOL rtserver_flag)
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
/*..TipcConnWrapper::SendRpc -- make a remote procedure call with messages */
/*..  on a connection */
TipcMsg& TipcConnWrapper::SendRpc(TipcMsg& call_msg, T_REAL8 timeout)
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
/*..TipcConnWrapper::MsgSendRpc -- make a remote procedure call with messages */
/*..  on a connection */
TipcMsg& TipcConnWrapper::MsgSendRpc(TipcMsg& call_msg, T_REAL8 timeout)
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
/*..TipcConnWrapper::Write -- construct a message and send it through a */
/*..  connection */
T_BOOL TipcConnWrapper::Write(TipcMt& mt, ...)
{
  // See RTworks & IPC manual, pg 10-83 ff.

  va_list var_arg_list;   // To point to each unnamed arg in turn

  va_start(var_arg_list, mt);
  _status = TipcConnMsgWriteVa(_connection, mt._mt, var_arg_list);
  va_end(var_arg_list);

  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::WriteVa -- construct a message and send it through a */
/*..  connection */
T_BOOL TipcConnWrapper::WriteVa(TipcMt& mt, va_list var_arg_list)
{
  _status = TipcConnMsgWriteVa(_connection, mt._mt, var_arg_list);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::Print -- print all information about a connection */
T_BOOL TipcConnWrapper::Print(T_OUT_FUNC func)
{
  _status = TipcConnPrint(_connection, func);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::DefaultCbCreate -- create a default callback in a */
/*..  connection */
T_CB TipcConnWrapper::DefaultCbCreate(T_IPC_CONN_DEFAULT_CB_FUNC func,
                                      T_CB_ARG arg)
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
/*..TipcConnWrapper::DefaultCbLookup -- look up a default callback in a */
/*..  connection */
T_CB TipcConnWrapper::DefaultCbLookup(T_IPC_CONN_DEFAULT_CB_FUNC func,
                                      T_CB_ARG arg)
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
/*..TipcConnWrapper::ProcessCbCreate -- create a process callback in a */
/*..  connection */
T_CB TipcConnWrapper::ProcessCbCreate(TipcMt& mt,
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
/*..TipcConnWrapper::ProcessCbLookup -- look up a process callback in a */
/*..  connection */
T_CB TipcConnWrapper::ProcessCbLookup(TipcMt& mt,
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
/*..TipcConnWrapper::QueueCbCreate -- create a queue callback in a */
/*..  connection */
T_CB TipcConnWrapper::QueueCbCreate(TipcMt& mt,
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
/*..TipcConnWrapper::QueueCbLookup -- look up a queue callback in a */
/*..  connection */
T_CB TipcConnWrapper::QueueCbLookup(TipcMt& mt,
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
/*..TipcConnWrapper::Read -- read all available data from a connection */
/*..  and queue messages in priority order */
T_BOOL TipcConnWrapper::Read(T_REAL8 timeout)
{
  _status = TipcConnRead(_connection, timeout);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::ReadCbCreate -- create a read callback in a connection */
T_CB TipcConnWrapper::ReadCbCreate(TipcMt& mt,
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
/*..TipcConnWrapper::ReadCbLookup -- look up a read callback in a connection */
T_CB TipcConnWrapper::ReadCbLookup(TipcMt& mt,
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
/*..TipcConnWrapper::WriteCbCreate -- create a write callback in a */
/*..  connection */
T_CB TipcConnWrapper::WriteCbCreate(TipcMt& mt,
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
/*..TipcConnWrapper::WriteCbLookup -- look up a write callback in a */
/*..  connection */
T_CB TipcConnWrapper::WriteCbLookup(TipcMt& mt,
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
/*..TipcConnWrapper::EncryptCbCreate -- create a encrypt callback in a */
/*..  connection */
T_CB TipcConnWrapper::EncryptCbCreate(TipcMt& mt,
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
/*..TipcConnWrapper::EncryptCbLookup -- look up a encrypt callback in a */
/*..  connection */
T_CB TipcConnWrapper::EncryptCbLookup(TipcMt& mt,
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
/*..TipcConnWrapper::DecryptCbCreate -- create a decrypt callback in a */
/*..  connection */
T_CB TipcConnWrapper::DecryptCbCreate(TipcMt& mt,
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
/*..TipcConnWrapper::DecryptCbLookup -- look up a decrypt callback in a */
/*..  connection */
T_CB TipcConnWrapper::DecryptCbLookup(TipcMt& mt,
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
/*..TipcConnWrapper::UniqueSubject -- get the unique subject of a */
/*..  connection's peer process */
T_STR TipcConnWrapper::UniqueSubject()
{
  T_STR unique_subject_return = ""; // initialize variable

  _status = TipcConnGetUniqueSubject(_connection, &unique_subject_return);
  return unique_subject_return;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcConnWrapper::UniqueDatagroup -- get the unique datagroup of a */
/*..  connection's peer process */
T_STR TipcConnWrapper::UniqueDatagroup()
{
  return UniqueSubject();
}
#endif

/* ========================================================================= */
/*..TipcConnWrapper::Node -- get the node name of a connection's peer */
/*..  process */
T_STR TipcConnWrapper::Node()
{
  T_STR node_return = "";  // initialize variable

  _status = TipcConnGetNode(_connection, &node_return);
  return node_return;
}


/* ========================================================================= */
/*..TipcConnWrapper::User -- get the user name of a connection's peer */
/*..  process */
T_STR TipcConnWrapper::User()
{
  T_STR user_return = ""; // initialize variable

  _status = TipcConnGetUser(_connection, &user_return);
  return user_return;
}


/* ========================================================================= */
/*..TipcConnWrapper::Pid -- get the process ID of a connection's peer */
/*..  process */
T_INT4 TipcConnWrapper::Pid()
{
  T_INT4 pid_return = -1;  // initialize variable

  _status = TipcConnGetPid(_connection, &pid_return);
  return pid_return;
}


/* ========================================================================= */
/*..TipcConnWrapper::Arch -- get the architecture name of a connection's */
/*..  peer process */
T_STR TipcConnWrapper::Arch()
{
  T_STR arch_return = "";  // initialize variable

  _status = TipcConnGetArch(_connection, &arch_return);
  return arch_return;
}


/* ========================================================================= */
/*..TipcConnWrapper::GmdFileCreate -- create files needed for GMD on a */
/*..  connection */
T_BOOL TipcConnWrapper::GmdFileCreate()
{
  _status = TipcConnGmdFileCreate(_connection);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::GmdFileDelete -- delete from disk GMD files for a */
/*..  connection */
T_BOOL TipcConnWrapper::GmdFileDelete()
{
  _status = TipcConnGmdFileDelete(_connection);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::GmdResend -- resend quaranteed messages after a */
/*..  delivery failure on a connection */
T_BOOL TipcConnWrapper::GmdResend()
{
  _status = TipcConnGmdResend(_connection);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::GmdMsgDelete -- delete an outgoing message from */
/*..  connection GMD files */
T_BOOL TipcConnWrapper::GmdMsgDelete(TipcMsg& msg)
{
  _status = TipcConnGmdMsgDelete(_connection, msg._msg);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::GmdMsgResend -- resend one spooled guaranteed message */
/*..  on a connection */
T_BOOL TipcConnWrapper::GmdMsgResend(TipcMsg& msg)
{
  _status = TipcConnGmdMsgResend(_connection, msg._msg);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::GmdMaxSize -- get gmd max size for a connection */
T_UINT4 TipcConnWrapper::GmdMaxSize()
{
  T_UINT4 gmd_max_size_return = 0; // initialize variable

  _status = TipcConnGetGmdMaxSize(_connection, &gmd_max_size_return);
  return gmd_max_size_return;
}


/* ========================================================================= */
/*..TipcConnWrapper::GmdMaxSize -- set gmd max size for a connection */
T_BOOL TipcConnWrapper::GmdMaxSize(T_UINT4 gmd_max_size)
{
  _status = TipcConnSetGmdMaxSize(_connection, gmd_max_size);
  return _status;
}


/* ========================================================================= */
/*..TipcConnWrapper::GmdDir -- get name of directory where files are */
/*..  written for GMD */
T_STR TipcConnWrapper::GmdDir()
{
  return TipcGetGmdDir();
}

#endif

