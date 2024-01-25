/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tconn.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tconn.h>
#include <rtworks/tmsg.h>
#include <rtworks/tutil.h>

namespace SmartSockets {
#define CALL_HANDLER(object,ptrToMember)  ((object).*(ptrToMember))

  TipcConnSearchSelector::TipcConnSearchSelector()
  {
#if defined(T_HP700_HPUX) || defined(T_IA64_HPUX) || defined(T_OS_LINUX) || defined(T_OS_AIX) || defined(T_OS_OSS) || defined(T_OS_MVS_OE) || defined(T_OS_OSF) || defined(T_OS_IRIX) || defined(T_OS_WINDOWS) || defined(T_OS_VMS) || defined(__GNUC__)
    _cbMethod = &TipcConnSearchSelector::onSearch;
#else
    _cbMethod = (Handler)this->onSearch;
#endif
  }

  void TipcConnSearchSelector::setHandler(TipcConnSearchSelector::Handler cbMethod)
  {
    _cbMethod = cbMethod;
  }

  TipcConnSearchSelector::Handler TipcConnSearchSelector::getHandler()
  {
    return _cbMethod;
  }


  TipcConnSearchSelector::~TipcConnSearchSelector() throw()
  {
  }

  bool TipcConnSearchSelector::onSearch
  (
  const TipcConn& conn,
  const TipcMsg& msg
  )
  throw ()
  {
    return false;
  }

  T_BOOL T_ENTRY TipcConnSearchSelector:: searchFuncDelegator
  (
  T_IPC_CONN conn,
  T_IPC_MSG  msg,
  T_PTR arg
  )
  {
    if (arg == NULL)
      throw TipcConnException();


    TipcConnSearchSelector* cb = (TipcConnSearchSelector*) arg;

    TipcConnSearchSelector::Handler method = (cb->getHandler());

    if (method == T_NULL_FUNC) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcConnException();
    }


    bool retVal = false;

    try {
      TipcMsg msgArg(msg);
      TipcConn connArg(conn,false);
      // msgArg.setCurrent(0);

      retVal = CALL_HANDLER(*cb,method) (connArg,msgArg);



      if (msgArg.getNumFields() > 0)
        msgArg.setCurrent(0);

    }
    catch (Exception& e) {
      Utilities::out("Caught exception from Callback method.  Do not throw any exceptions"
                     "from CallBack methods.\n  <error = %s>\n",e.what());
    }


    return retVal?TRUE:FALSE;
  }


  /* ======================================================================= */
  /*..TipcConn::TipcConn -- TipcConn constructor */
  TipcConn::TipcConn()throw (TipcException)
  {
    _destroy_flag = T_TRUE;
    _connection = TipcConnCreate();
  }

  /* ======================================================================= */
  /*..TipcConn::TipcConn -- TipcConn constructor */
  TipcConn::TipcConn(T_IPC_CONN connection,
                     bool destroy_flag)throw (TipcException)
  {
    _destroy_flag = destroy_flag;
    _connection = connection;
  }

  /* ======================================================================= */
  /*..TipcConn::TipcConn -- TipcConn constructor */
  TipcConn::TipcConn(bool do_nothing)
  {

    // this is a kludge to allow TipcConn[Server|Client] to work
    // without any really ridiculous gyrations.   JAM 03/07/97
    if (T_FALSE == do_nothing) {
      _destroy_flag = T_TRUE;
      _connection = TipcConnCreate();
    }
  }

  /* ======================================================================= */
  /*..TipcConn::~TipcConn -- TipcConn destructor */
  TipcConn::~TipcConn() throw()
  {
    if (_destroy_flag)
      TipcConnDestroy(_connection);

  }

  /* ======================================================================= */
  /*..TipcConn::Check -- check if data can be read from or written  */
  /*..  to a connection */
  bool TipcConn::check(T_IO_CHECK_MODE check_mode,
                       real8 timeout) throw (TipcException)
  {
    bool retVal = TipcConnCheck(_connection, check_mode, timeout);
    if (!retVal)
      throw TipcConnException();

    return retVal;
  }


  /* ======================================================================= */
  /*..TipcConn::errorCbDelegator -- delegator method for error callbacks  */
  void T_ENTRY1 TipcConn::errorCbDelegator(T_IPC_CONN connection,
                                           T_IPC_CONN_ERROR_CB_DATA data,
                                           T_CB_ARG arg)
  {
    CallBack<ErrorCallBack> * callBack =
    static_cast <CallBack <ErrorCallBack> *> (arg);

    TipcConnException*  ex = new TipcConnException();

    TipcConn conn (connection,false);

    try {
      ((callBack->m_implementor)->*(callBack->m_handler)) (
                                                          callBack,
                                                          *ex,
                                                          conn);
    }
    catch (Exception& e) {
      Utilities::out("Caught exception from Callback method.  Do not throw any exceptions"
                     "from CallBack methods.\n  <error = %s>\n",e.what());
    }

    delete ex;
  }


  /* ======================================================================= */
  /*..TipcConn::queueCbDelegator -- delegator method for queue callbacks  */
  void T_ENTRY1 TipcConn::queueCbDelegator(T_IPC_CONN connection,
                                           T_IPC_CONN_QUEUE_CB_DATA data,
                                           T_CB_ARG arg)
  {
    CallBack<MessageQueueCallBack> * callBack =
    static_cast <CallBack <MessageQueueCallBack> *> (arg);

    TipcMsg message (data->msg);
    TipcConn conn( connection,false);

    try {
      ((callBack->m_implementor)->*(callBack->m_handler)) (
                                                          callBack,
                                                          message,
                                                          conn,
                                                          data->position,
                                                          (bool) data->insert_flag);
    }
    catch (Exception& e) {
      Utilities::out("Caught exception from Callback method.  Do not throw any exceptions"
                     "from CallBack methods.\n  <error = %s>\n",e.what());
    }
  }

  /* ======================================================================= */
  /*..TipcConn::encodeCbDelegator -- delegator method for encode message  */
  /*..callbacks */
  void T_ENTRY1 TipcConn::encodeCbDelegator(T_IPC_CONN connection,
                                            T_IPC_CONN_ENCODE_CB_DATA data,
                                            T_CB_ARG arg)
  {
    CallBack<EncodeCallBack> * callBack =
    static_cast <CallBack <EncodeCallBack> *> (arg);

    TipcMsg message (data->orig_msg);
    TipcConn conn (connection,false);

    try {
      ((callBack->m_implementor)->*(callBack->m_handler)) (
                                                          callBack,
                                                          data->packet,
                                                          data->packet_size,
                                                          data->orig_packet,
                                                          data->orig_packet_size,
                                                          message,
                                                          conn);
    }
    catch (Exception& e) {
      Utilities::out("Caught exception from Callback method.  Do not throw any exceptions"
                     "from CallBack methods.\n  <error = %s>\n",e.what());
    }
  }


  /* ======================================================================= */
  /*..TipcConn::decodeCbDelegator -- delegator method for decode message  */
  /*..callbacks */
  void T_ENTRY1 TipcConn::decodeCbDelegator(T_IPC_CONN connection,
                                            T_IPC_CONN_DECODE_CB_DATA data,
                                            T_CB_ARG arg)
  {
    CallBack<DecodeCallBack> * callBack =
    static_cast <CallBack <DecodeCallBack> *> (arg);

    TipcConn conn (connection,false);

    try {
      ((callBack->m_implementor)->*(callBack->m_handler)) (
                                                          callBack,
                                                          data->packet,
                                                          data->packet_size,
                                                          data->orig_packet,
                                                          data->orig_packet_size,
                                                          conn);
    }
    catch (Exception& e) {
      Utilities::out("Caught exception from Callback method.  Do not throw any exceptions"
                     "from CallBack methods.\n  <error = %s>\n",e.what());
    }
  }


  /* ======================================================================= */
  /*..TipcConn::messageCbDelegator -- delegator method for message callbacks  */
  void T_ENTRY1 TipcConn::messageCbDelegator(T_IPC_CONN connection,
                                             T_IPC_CONN_MSG_CB_DATA data,
                                             T_CB_ARG arg)
  {
    CallBack<MessageCallBack> * callBack =
    static_cast <CallBack <MessageCallBack> *> (arg);

    TipcMsg message (data->msg);
    TipcConn conn(connection,false);

    try {
      ((callBack->m_implementor)->*(callBack->m_handler)) (callBack, message,
                                                           conn);
    }
    catch (Exception& e) {
      Utilities::out("Caught exception from Callback method.  Do not throw any exceptions"
                     "from CallBack methods.\n  <error = %s>\n",e.what());
    }
  }



  /* ======================================================================= */
  /*..TipcConn::readCbDelegator -- delegator method for read message callbacks */
  void T_ENTRY1 TipcConn::readCbDelegator(T_IPC_CONN connection,
                                          T_IPC_CONN_READ_CB_DATA data,
                                          T_CB_ARG arg)
  {
    CallBack<MessageCallBack> * callBack =
    static_cast <CallBack <MessageCallBack> *> (arg);

    TipcMsg message(data->msg);
    TipcConn conn(connection,false);

    try {
      ((callBack->m_implementor)->*(callBack->m_handler)) (callBack, message,
                                                           conn);
    }
    catch (Exception& e) {
      Utilities::out("Caught exception from Callback method.  Do not throw any exceptions"
                     "from CallBack methods.\n  <error = %s>\n",e.what());
    }
  }


  /* ======================================================================= */
  /*..TipcConn::errorCbCreate -- create an error callback in the connection */
  /*..  to RTserver */
  CallBack<ErrorCallBack> * TipcConn::errorCbCreate(
                                                   ErrorCallBack * implementor,
                                                   void* arg)
  throw (TipcException)
  {
    ErrorCallBack::Handler handler = &ErrorCallBack::onError;

    CallBack<ErrorCallBack> * cb =
    new CallBack<ErrorCallBack> (implementor, handler);

    T_CB tcb = TipcConnErrorCbCreate(_connection, errorCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcConnException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }





  /* ======================================================================= */
  /*..TipcConn::Flush -- flush buffered outgoing messages to a  */
  /*..  connection's socket */
  void TipcConn::flush() throw (TipcException)
  {
    if (!TipcConnFlush(_connection))
      throw TipcConnException();
  }

  /* ======================================================================= */
  /*..TipcConn::AutoFlushSize -- get the automatic flush size of a */
  /*..  connection */
  int4 TipcConn::getAutoFlushSize() const throw (TipcException)
  {
    int4 auto_flush_size_return = -1;  // negative values are illegal

    if (!TipcConnGetAutoFlushSize(_connection, &auto_flush_size_return))
      throw TipcConnException();

    return auto_flush_size_return?true:false;
  }


  /* ======================================================================= */
  /*..TipcConn::AutoFlushSize -- set the automatic flush size of a */
  /*..  connection */
  void TipcConn::setAutoFlushSize(int4 auto_flush_size) throw (TipcException)
  {
    if (!TipcConnSetAutoFlushSize(_connection, auto_flush_size))
      throw TipcConnException();
  }


  /* ======================================================================= */
  /*..TipcConn::BlockMode -- get the block mode of a connection */
  long TipcConn::getBlockMode() const throw (TipcException)
  {
    T_BOOL block_mode_return = T_TRUE;  // initialize variable

    bool status = TipcConnGetBlockMode(_connection, &block_mode_return);
    if (false == status)
      throw TipcConnException();

    return long(block_mode_return);
  }


  /* ======================================================================= */
  /*..TipcConn::BlockMode -- set the block mode of a connection */
  void TipcConn::setBlockMode(bool block_mode) throw (TipcException)
  {
    if (!TipcConnSetBlockMode(_connection, block_mode))
      throw TipcConnException();
  }


  /* ======================================================================= */
  /*..TipcConn::NumQueued -- get number of queued messages from a  */
  /*..  connection */
  int4 TipcConn::getNumQueued() const throw (TipcException)
  {
    int4 num_queued_return = -1;  // initialize variable

    bool status = TipcConnGetNumQueued(_connection, &num_queued_return);
    if (false == status)
      throw TipcConnException();

    return num_queued_return;
  }


  /* ======================================================================= */
  /*..TipcConn::Socket -- get the socket of a connection */
  sock TipcConn::getSocket() const throw (TipcException)
  {
    sock socket_return = - 1;  // initialize variable

    bool status = TipcConnGetSocket(_connection, &socket_return);
    if (false == status)
      throw TipcConnException();

    return socket_return;
  }


  /* ======================================================================= */
  /*..TipcConn::Timeout -- get a timeout property of a connection */
  real8 TipcConn::getTimeout(T_IPC_TIMEOUT timeout) const throw (TipcException)
  {
    real8 value_return = -1.0;  // initialize variable

    bool status = TipcConnGetTimeout(_connection, timeout, &value_return);
    if (false == status)
      throw TipcConnException();

    return value_return;
  }


  /* ======================================================================= */
  /*..TipcConn::Timeout -- set a timeout property of a connection */
  void TipcConn::setTimeout(T_IPC_TIMEOUT timeout,
                            real8 value) throw (TipcException)
  {
    if (!TipcConnSetTimeout(_connection, timeout, value))
      throw TipcConnException();
  }


  /* ======================================================================= */
  /*..TipcConn::XtSource -- get source suitable for XtAppAddInput from */
  /*..   a connection */
  sock TipcConn::getXtSource() const throw (TipcException)
  {
    sock source_return = -1;  // initialize variable

    bool status = TipcConnGetXtSource(_connection, &source_return);
    if (false == status)
      throw TipcConnException();

    return source_return;
  }


  /* ======================================================================= */
  /*..TipcConn::KeepAlive -- check if the process at the other end of */
  /*..  a connection is still alive */
  void TipcConn::keepAlive() throw (TipcException)
  {
    if (!TipcConnKeepAlive(_connection))
      throw TipcConnException();
  }


  /* ======================================================================= */
  /*..TipcConn::MainLoop -- read and process message on a connection */
  bool TipcConn::mainLoop(real8 timeout) throw (TipcException)
  {
    if (!TipcConnMainLoop(_connection, timeout)) {
      if (TutErrNumGet() != T_ERR_TIMEOUT_REACHED) {
        throw TipcConnException();
      }
      return false;
    }
    return true;
  }


  /* ======================================================================= */
  /*..TipcConn::Insert -- insert a message into queue of a connection */
  void TipcConn::insert(TipcMsg& msg, int4 pos) throw (TipcException)
  {
    if (!TipcConnMsgInsert(_connection, msg._msg, pos))
      throw TipcConnException();

    /* ---------------------------------------------------------- */
    /* Since TipcConnMsgInsert takes destroy responsibility from  */
    /* the caller, convert TipcMsg object into a vacant object.   */
    /* ---------------------------------------------------------- */
    msg._msg = (T_IPC_MSG)T_NULL;

  }


  /* ======================================================================= */
  /*..TipcConn::Next -- get the next message from a connection */
  void TipcConn::next(TipcMsg& retMsg, real8 timeout)
  throw (TipcException)
  {
    T_IPC_MSG msg = TipcConnMsgNext(_connection, timeout);

    if (msg == NULL) {
      if (TutErrNumGet() != T_ERR_TIMEOUT_REACHED)
        throw TipcConnException();
    }

    retMsg.absorb(msg);
  }

  /* ======================================================================= */
  /*..TipcConn::nextEx -- get the next message from a connection */
  /*..returns false if the timeout is expired, true otherwise. */
  bool TipcConn::nextEx(TipcMsg& retMsg, real8 timeout) throw (TipcException)
  {
    T_IPC_MSG msg = TipcConnMsgNext(_connection, timeout);
    if (NULL == msg) {
      if (T_ERR_TIMEOUT_REACHED != TutErrNumGet()) {
        throw TipcConnException();
      }
      return false;
    }

    retMsg.absorb(msg);
    return true;
  }


  /* ======================================================================= */
  /*..TipcConn::Process -- process a message in a connection */
  void TipcConn::process(TipcMsg& msg) throw (TipcException)
  {
    if (!TipcConnMsgProcess(_connection, msg._msg))
      throw TipcConnException();
  }


  /* ======================================================================= */
  /*..TipcConn::Search -- search the message queue of a connection for */
  /*..  a specific message */
  void TipcConn::search(TipcMsg& msg,TipcConnSearchSelector* selObj, real8 timeout)
  throw (TipcException)
  {
    if (selObj == NULL) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcConnException();
    }
    TutErrNumSet(0);

    T_IPC_MSG cMsg = TipcConnMsgSearch(_connection, timeout,
                                       TipcConnSearchSelector::searchFuncDelegator,
                                       (T_PTR)selObj);
    int4  errNum = TutErrNumGet();

    if (cMsg == NULL) {
      if ((errNum!= 0) && (errNum != T_ERR_END_OF_TRAVERSAL) &&
          (errNum != T_ERR_TIMEOUT_REACHED)) {
        throw TipcConnException();
      }
    }

    msg.absorb(cMsg);
  }


  /* ======================================================================= */
  /*..TipcConn::SearchType -- search the message queue of a connection */
  /*..  for a message with specific type */
  void TipcConn::searchType(TipcMsg& msg, TipcMt& mt,
                            real8 timeout) throw (TipcException)
  {
    T_IPC_MSG cMsg = TipcConnMsgSearchType(_connection, timeout, mt._mt);

    if (cMsg == NULL) {
      if (TutErrNumGet() != T_ERR_TIMEOUT_REACHED)
        throw TipcConnException();
    }

    msg.absorb(cMsg);
  }


  /* ======================================================================= */
  /*..TipcConn::Send -- send a message through a connection */
  void TipcConn::send(const TipcMsg& msg,
                      bool rtserver_flag) throw (TipcException)
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

    if (!TipcConnMsgSend(_connection, msg._msg))
      throw TipcConnException();
  }


  /* ======================================================================= */
  /*..TipcConn::SendRpc -- make a remote procedure call with messages */
  /*..  on a connection */
  void TipcConn::sendRpc(TipcMsg& reply_msg, const TipcMsg& call_msg,
                         real8 timeout) throw (TipcException)
  {
    T_IPC_MSG msg = TipcConnMsgSendRpc(_connection, call_msg._msg, timeout);

    if (msg == NULL) {
      if (TutErrNumGet() != T_ERR_TIMEOUT_REACHED)
        throw TipcConnException();
    }

    reply_msg.absorb(msg);
  }


  /* ======================================================================= */
  /*..TipcConn::Write -- construct a message and send it through a */
  /*..  connection */
#if defined(T_OS_WINDOWS)
  /* see the header file for an explanation of this ifdef */
  void TipcConn::write(TipcMt mt, ...) throw (TipcException)
#else
  void TipcConn::write(TipcMt& mt, ...) throw (TipcException)
#endif
  {
    // See RTworks & IPC manual, pg 10-83 ff.

    va_list var_arg_list;   // To point to each unnamed arg in turn

    va_start(var_arg_list, mt);
    bool status = TipcConnMsgWriteVa(_connection, mt._mt, var_arg_list);

    va_end(var_arg_list);

    if (false == status)
      throw TipcConnException();
  }


  /* ======================================================================= */
  /*..TipcConn::WriteVa -- construct a message and send it through a */
  /*..  connection */
#if defined(T_OS_WINDOWS)
  /* see the header file for an explanation of this ifdef */
  void TipcConn::writeVa(TipcMt mt,
                         va_list var_arg_list) throw (TipcException)
#else
  void TipcConn::writeVa(TipcMt& mt,
                         va_list var_arg_list) throw (TipcException)
#endif
  {
    if (!TipcConnMsgWriteVa(_connection, mt._mt, var_arg_list))
      throw TipcConnException();
  }


  /* ======================================================================= */
  /*..TipcConn::Print -- print all information about a connection */
  void TipcConn::print(T_OUT_FUNC func) const throw (TipcException)
  {
    if (!TipcConnPrint(_connection, func))
      throw TipcConnException();

  }


  /* ======================================================================= */
  /*..TipcConn::defaultCbCreate -- create a default callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcConn::defaultCbCreate(
                                                       MessageCallBack * implementor,
                                                       void* arg)
  throw (TipcException)
  {
    MessageCallBack::Handler handler = &MessageCallBack::onMessage;

    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcConnDefaultCbCreate(_connection, messageCbDelegator,
                                       cb);

    if (!tcb) {
      delete cb;
      throw TipcConnException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }








  /* ======================================================================= */
  /*..TipcConn::processCbCreate -- create a process callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcConn::processCbCreate(
                                                       const TipcMt & mt,
                                                       MessageCallBack * implementor,
                                                       void* arg)
  throw (TipcException)
  {
    MessageCallBack::Handler handler = &MessageCallBack::onMessage;


    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcConnProcessCbCreate(_connection, mt._mt,
                                       messageCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcConnException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }






  /* ======================================================================= */
  /*..TipcConn::queueCbCreate -- create a queue callback in the connection */
  /*..  to RTserver */
  CallBack<MessageQueueCallBack> * TipcConn::queueCbCreate(
                                                          const TipcMt& mt,
                                                          MessageQueueCallBack * implementor,
                                                          void* arg)
  throw (TipcException)
  {
    MessageQueueCallBack::Handler handler = &MessageQueueCallBack::onMessage;
    CallBack<MessageQueueCallBack> * cb =
    new CallBack<MessageQueueCallBack> (implementor, handler);

    T_CB tcb = TipcConnQueueCbCreate(_connection, (T_IPC_MT)mt, queueCbDelegator,
                                     cb);

    if (!tcb) {
      delete cb;
      throw TipcConnException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }






  /* ======================================================================= */
  /*..TipcConn::Read -- read all available data from a connection */
  /*..  and queue messages in priority order */
  void TipcConn::read(real8 timeout) throw (TipcException)
  {
    if (!TipcConnRead(_connection, timeout))
      throw TipcConnException();

  }


  /* ======================================================================= */
  /*..TipcConn::readCbCreate -- create a read callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcConn::readCbCreate(
                                                    const TipcMt & mt,
                                                    MessageCallBack * implementor,
                                                    void* arg)
  throw (TipcException)
  {
    MessageCallBack::Handler handler = &MessageCallBack::onMessage;
    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcConnReadCbCreate(_connection, mt._mt, readCbDelegator,
                                    cb);

    if (!tcb) {
      delete cb;
      throw TipcConnException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }





  /* ======================================================================= */
  /*..TipcConn::writeCbCreate -- create a write callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcConn::writeCbCreate(
                                                     const TipcMt & mt,
                                                     MessageCallBack * implementor,
                                                     void* arg)
  throw (TipcException)
  {
    MessageCallBack::Handler handler = &MessageCallBack::onMessage;
    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcConnWriteCbCreate(_connection, mt._mt,
                                     messageCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcConnException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }






  /* ======================================================================= */
  /*..TipcConn::encryptCbCreate -- create an encrypt callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcConn::encryptCbCreate(
                                                       const TipcMt & mt,
                                                       MessageCallBack * implementor,
                                                       void* arg)
  throw (TipcException)
  {
    MessageCallBack::Handler handler = &MessageCallBack::onMessage;
    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcConnEncryptCbCreate(_connection, mt._mt,
                                       messageCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcConnException();
    }

    cb->setTCB(tcb);

    cb->setArgument(arg);

    return cb;
  }



  /* ======================================================================= */
  /*..TipcConn::decryptCbCreate -- create a decrypt callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcConn::decryptCbCreate(
                                                       const TipcMt & mt,
                                                       MessageCallBack * implementor,
                                                       void* arg)
  throw (TipcException)
  {
    MessageCallBack::Handler handler = &MessageCallBack::onMessage;
    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcConnDecryptCbCreate(_connection, mt._mt,
                                       messageCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcConnException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }



  /* ======================================================================= */
  /*..TipcConn::UniqueSubject -- get the unique subject of a */
  /*..  connection's peer process */
  const char * TipcConn::getPeerUniqueSubject() const throw (TipcException)
  {
    char * unique_subject_return = (char *)""; // initialize variable

    bool status = TipcConnGetUniqueSubject(_connection, &unique_subject_return);
    if (false == status)
      throw TipcConnException();

    return unique_subject_return;
  }


  /* ======================================================================= */
  /*..TipcConn::Node -- get the node name of a connection's peer */
  /*..  process */
  const char * TipcConn::getNode() const throw (TipcException)
  {
    char * node_return = (char *)"";  // initialize variable

    bool status = TipcConnGetNode(_connection, &node_return);
    if (false == status)
      throw TipcConnException();

    return node_return;
  }


  /* ======================================================================= */
  /*..TipcConn::User -- get the user name of a connection's peer */
  /*..  process */
  const char * TipcConn::getUser() const throw (TipcException)
  {
    char * user_return = (char *)""; // initialize variable

    bool status = TipcConnGetUser(_connection, &user_return);
    if (false == status)
      throw TipcConnException();

    return user_return;
  }


  /* ======================================================================= */
  /*..TipcConn::Pid -- get the process ID of a connection's peer */
  /*..  process */
  int4 TipcConn::getPid() const throw (TipcException)
  {
    int4 pid_return = -1;  // initialize variable

    bool status = TipcConnGetPid(_connection, &pid_return);
    if (false == status)
      throw TipcConnException();

    return pid_return;
  }


  /* ======================================================================= */
  /*..TipcConn::Arch -- get the architecture name of a connection's */
  /*..  peer process */
  const char * TipcConn::getArch() const throw (TipcException)
  {
    char * arch_return = (char *)"";  // initialize variable

    bool status = TipcConnGetArch(_connection, &arch_return);
    if (false == status)
      throw TipcConnException();

    return arch_return;
  }


  /* ======================================================================= */
  /*..TipcConn::GmdFileCreate -- create files needed for GMD on a */
  /*..  connection */
  void TipcConn::gmdFileCreate() throw (TipcException)
  {
    if (!TipcConnGmdFileCreate(_connection))
      throw TipcConnException();

  }


  /* ======================================================================= */
  /*..TipcConn::GmdFileDelete -- delete from disk GMD files for a */
  /*..  connection */
  void TipcConn::gmdFileDelete() throw (TipcException)
  {
    if (!TipcConnGmdFileDelete(_connection))
      throw TipcConnException();

  }


  /* ======================================================================= */
  /*..TipcConn::GmdResend -- resend quaranteed messages after a */
  /*..  delivery failure on a connection */
  void TipcConn::gmdResend() throw (TipcException)
  {
    if (!TipcConnGmdResend(_connection))
      throw TipcConnException();

  }


  /* ======================================================================= */
  /*..TipcConn::GmdMsgDelete -- delete an outgoing message from */
  /*..  connection GMD files */
  void TipcConn::gmdMsgDelete(TipcMsg& msg) throw (TipcException)
  {
    if (!TipcConnGmdMsgDelete(_connection, msg._msg))
      throw TipcConnException();
  }


  /* ======================================================================= */
  /*..TipcConn::GmdMsgResend -- resend one spooled guaranteed message */
  /*..  on a connection */
  void TipcConn::gmdMsgResend(TipcMsg& msg) throw (TipcException)
  {
    if (!TipcConnGmdMsgResend(_connection, msg._msg))
      throw TipcConnException();
  }


  /* ======================================================================= */
  /*..TipcConn::GmdMaxSize -- get gmd max size for a connection */
  uint4 TipcConn::getGmdMaxSize() const throw (TipcException)
  {
    uint4 gmd_max_size_return = 0; // initialize variable

    bool status = TipcConnGetGmdMaxSize(_connection, &gmd_max_size_return);
    if (false == status)
      throw TipcConnException();

    return gmd_max_size_return;
  }


  /* ======================================================================= */
  /*..TipcConn::GmdMaxSize -- set gmd max size for a connection */
  void TipcConn::setGmdMaxSize(uint4 gmd_max_size) throw (TipcException)
  {
    if (!TipcConnSetGmdMaxSize(_connection, gmd_max_size))
      throw TipcConnException();
  }


  /* ======================================================================= */
  /*..TipcConn::GmdDir -- get name of directory where files are */
  /*..  written for GMD */
  const char * TipcConn::getGmdDir() const throw (TipcException)
  {
    return TipcGetGmdDir();
  }

  /* ======================================================================= */
  /*..TipcConn::encodeCbCreate -- create an encode callback in the connection */
  /*..  to RTserver */
  CallBack<EncodeCallBack> * TipcConn::encodeCbCreate(
                                                     EncodeCallBack * implementor,
                                                     void* arg)
  throw (TipcException)
  {
    EncodeCallBack::Handler handler = &EncodeCallBack::onMessage;
    CallBack<EncodeCallBack> * cb =
    new CallBack<EncodeCallBack> (implementor, handler);

    T_CB tcb = TipcConnEncodeCbCreate(_connection, encodeCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcConnException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);
    return cb;
  }



  /* ======================================================================= */
  /*..TipcConn::decodeCbCreate -- create a decode callback in the connection */
  /*..  to RTserver */
  CallBack<DecodeCallBack> * TipcConn::decodeCbCreate(
                                                     DecodeCallBack * implementor,
                                                     void* arg)
  throw (TipcException)
  {
    DecodeCallBack::Handler handler = &DecodeCallBack::onMessage;
    CallBack<DecodeCallBack> * cb =
    new CallBack<DecodeCallBack> (implementor, handler);

    T_CB tcb = TipcConnDecodeCbCreate(_connection, decodeCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcConnException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }





  /* ======================================================================= */
  /*.. TipcConn::Lock() -- acquire exclusive access to connection */
  void TipcConn::lock() throw (TipcException)
  {
    if (!TipcConnLock(_connection))
      throw TipcConnException();
  }

  /* ======================================================================= */
  /*.. TipcConn::Unlock() -- release exclusive access to connection */
  void TipcConn::unlock() throw (TipcException)
  {
    if (!TipcConnUnlock(_connection))
      throw TipcConnException();
  }

  /* ======================================================================= */
  /*.. TipcConn::GmdNumPending -- get the number of messages pending GMD */
  int4 TipcConn::getGmdNumPending() const throw (TipcException)
  {
    int4 num_pending = 0;

    if (!TipcConnGetGmdNumPending(_connection, &num_pending))
      throw TipcConnException();

    return num_pending;
  }

  /* ======================================================================= */
  /*..TipcConn::trafficGetBytesRecv -- gets the total number of bytes        */
  /*..                                 received on the connection            */
  int4 TipcConn::trafficGetBytesRecv() const throw (TipcException)
  {
    int4 bytes_recv = -1;  // negative values are illegal

    if (!TipcConnTrafficGetBytesRecv(_connection, &bytes_recv)) {
      throw TipcConnException();
    }

    return bytes_recv;
  }
  int8 TipcConn::trafficGetBytesRecv8() const throw (TipcException)
  {
    int8 bytes_recv = -1;  // negative values are illegal

    if (!TipcConnTrafficGetBytesRecv8(_connection, &bytes_recv)) {
      throw TipcConnException();
    }

    return bytes_recv;
  }

  /* ======================================================================= */
  /*..TipcConn::trafficGetBytesSent -- Gets the total number of bytes        */
  /*..                                 written into the write buffer on the  */
  /*..                                 connection.  This is usually the same */
  /*..                                 as the total number of bytes actually */
  /*..                                 sent on the connection, but sometimes */
  /*..                                 the retrieved number is larger than   */
  /*..                                 the number of bytes sent when the     */
  /*..                                 buffer is not frequently flushed.     */
  int4 TipcConn::trafficGetBytesSent() const throw (TipcException)
  {
    int4 bytes_sent = -1;  // negative values are illegal

    if (!TipcConnTrafficGetBytesSent(_connection, &bytes_sent)) {
      throw TipcConnException();
    }

    return bytes_sent;
  }
  int8 TipcConn::trafficGetBytesSent8() const throw (TipcException)
  {
    int8 bytes_sent = -1;  // negative values are illegal

    if (!TipcConnTrafficGetBytesSent8(_connection, &bytes_sent)) {
      throw TipcConnException();
    }

    return bytes_sent;
  }

  /* ======================================================================= */
  /*..TipcConn::trafficGetMsgsRecv -- gets the total number of messages      */
  /*..                                received on the connection             */
  int4 TipcConn::trafficGetMsgsRecv() const throw (TipcException)
  {
    int4 msgs_recv = -1;  // negative values are illegal

    if (!TipcConnTrafficGetMsgsRecv(_connection, &msgs_recv)) {
      throw TipcConnException();
    }

    return msgs_recv;
  }
  int8 TipcConn::trafficGetMsgsRecv8() const throw (TipcException)
  {
    int8 msgs_recv = -1;  // negative values are illegal

    if (!TipcConnTrafficGetMsgsRecv8(_connection, &msgs_recv)) {
      throw TipcConnException();
    }

    return msgs_recv;
  }

  /* ======================================================================= */
  /*..TipcConn::trafficGetMsgsSent -- Gets the total number of messages      */
  /*..                                written into the write buffer on the   */
  /*..                                connection.  This is usually the same  */
  /*..                                as the total number of messages        */
  /*..                                actually sent on the connection, but   */
  /*..                                sometimes the retrieved number is      */
  /*..                                larger than the number of bytes sent   */
  /*..                                when the buffer is not frequently      */
  /*..                                flushed.                               */
  int4 TipcConn::trafficGetMsgsSent() const throw (TipcException)
  {
    int4 msgs_sent = -1;  // negative values are illegal

    if (!TipcConnTrafficGetMsgsSent(_connection, &msgs_sent)) {
      throw TipcConnException();
    }

    return msgs_sent;
  }
  int8 TipcConn::trafficGetMsgsSent8() const throw (TipcException)
  {
    int8 msgs_sent = -1;  // negative values are illegal

    if (!TipcConnTrafficGetMsgsSent8(_connection, &msgs_sent)) {
      throw TipcConnException();
    }

    return msgs_sent;
  }

  /* ======================================================================= */
  /*..TipcConn::bufferGetReadSize -- gets the total number of bytes in the   */
  /*..                               connection's read buffer                */
  int4 TipcConn::bufferGetReadSize() const throw (TipcException)
  {
    int4 bytes = -1;  // negative values are illegal

    if (!TipcConnBufferGetReadSize(_connection, &bytes)) {
      throw TipcConnException();
    }

    return bytes;
  }

  /* ======================================================================= */
  /*..TipcConn::bufferGetWriteSize -- gets the total number of bytes in the  */
  /*..                                connection's write buffer              */
  int4 TipcConn::bufferGetWriteSize() const throw (TipcException)
  {
    int4 bytes = -1;  // negative values are illegal

    if (!TipcConnBufferGetWriteSize(_connection, &bytes)) {
      throw TipcConnException();
    }

    return bytes;
  }



  /* ======================================================================= */
  /* wrappers for server and client connections */

  /* ======================================================================= */
  /*..TipcConnServer::TipcConnServer -- TipcConnServer constructor */
  TipcConnServer::TipcConnServer(const char * link_name) throw (TipcException): TipcConn(T_TRUE)
  {
    _destroy_flag = T_TRUE;
    _connection = TipcConnCreateServer(const_cast<char *> (link_name));
    if (T_NULL == _connection) {
      throw TipcConnException();
    }
  }

  /* ======================================================================= */
  /*..TipcConnServer::TipcConnServer -- TipcConnServer constructor */
  TipcConnServer::TipcConnServer(T_IPC_CONN connection,
                                 bool destroy_flag) throw (TipcException)
  : TipcConn(connection,destroy_flag)
  {
  }

  /* ======================================================================= */
  /*..TipcConnServer::~TipcConnServer -- TipcConnServer destructor*/
  /*..  Need this as we have the warning during the shared_optimized build */
  TipcConnServer::~TipcConnServer() throw()
  {
  }

  /* ======================================================================= */
  /*..TipcConnServer::Accept -- accept and create a new TipcConn */
  TipcConn *TipcConnServer::accept(void) throw (TipcException)
  {
    T_IPC_CONN accept_conn = TipcConnAccept(_connection);

    if (T_NULL == accept_conn) {
      throw TipcConnException();
    }

    return new TipcConn(accept_conn);
  }

  /* ======================================================================= */
  /*..TipcConnClient::TipcConnClient -- TipcConnClient constructor */
  TipcConnClient::TipcConnClient(const char * link_name)  throw (TipcException):
  TipcConn(T_TRUE)

  {
    _destroy_flag = T_TRUE;
    _connection = TipcConnCreateClient(const_cast<char *> (link_name));
    if (T_NULL == _connection) {
      throw TipcConnException();
    }
  }

  /* ======================================================================= */
  /*..TipcConnClient::TipcConnClient -- TipcConnClient constructor */
  TipcConnClient::TipcConnClient(T_IPC_CONN connection,
                                 bool destroy_flag)throw (TipcException)
  : TipcConn(connection,destroy_flag)
  {
  }

  /* ======================================================================= */
  /*..TipcConnServer::~TipcConnServer -- TipcConnServer destructor*/
  /*..  Need this as we have the warning during the shared_optimized build */
  TipcConnClient::~TipcConnClient() throw()
  {
  }


} //namespace SmartSockets
