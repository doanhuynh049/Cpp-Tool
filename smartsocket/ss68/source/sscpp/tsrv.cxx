/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tsrv.cxx#2 $
 */

#include "includes.h"
#include <rtworks/types.h>
#include <rtworks/tsrv.h>
#include <rtworks/tmsg.h>
#include <rtworks/tutil.h>

namespace SmartSockets {
#define CALL_HANDLER(object,ptrToMember)  ((object).*(ptrToMember))

  TipcSrvSubjTraverser::TipcSrvSubjTraverser()
  {
#if !defined(T_OS_SOLARIS) || defined(__GNUC__)
    _cbMethod = &TipcSrvSubjTraverser::onTraverse;
#else
    _cbMethod = (Handler)this->onTraverse;
#endif
  }

  void TipcSrvSubjTraverser::setHandler(TipcSrvSubjTraverser::Handler cbMethod)
  {
    _cbMethod = cbMethod;
  }

  TipcSrvSubjTraverser::Handler TipcSrvSubjTraverser::getHandler()
  {
    return _cbMethod;
  }


  TipcSrvSubjTraverser::~TipcSrvSubjTraverser() throw()
  {
  }

  bool TipcSrvSubjTraverser::onTraverse
  (
  const char* subjName
  )
  throw ()
  {
    return false;
  }

  T_PTR T_ENTRY TipcSrvSubjTraverser:: traverseFuncDelegator
  (
  T_STR subject,
  T_STR dummy,
  T_PTR arg
  )
  {
    if (arg == NULL)
      throw TipcSrvException();

    TipcSrvSubjTraverser* cb = (TipcSrvSubjTraverser*) arg;

    TipcSrvSubjTraverser::Handler method = (cb->getHandler());

    if (method == T_NULL_FUNC) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcSrvException();
    }


    bool retVal = false;

    try {
      retVal = CALL_HANDLER(*cb,method) (const_cast<const char*>(subject));
    }
    catch (Exception& e) {
      Utilities::out("Caught exception from Callback method.  Do not throw any exceptions"
                     "from CallBack methods.\n  <error = %s>\n",e.what());
    }


    return retVal?NULL:arg;
  }



  /* Creates a connection object */
  TipcSrv::TipcSrv(const char * unique_subject,
                   const char * server_names,  /* comma delimited list */
                   const char * project,
                   const char * subject_prefix)
  throw (TipcException)
  {
    _srv = TipcSrvConnCreate(const_cast<char *> (unique_subject),
                             const_cast<char *> (project),
                             const_cast<char *> (server_names),
                             const_cast<char *> (subject_prefix));

    if (NULL == _srv)
      throw TipcSrvException();

  }

  /*
   * Creates a "named" connection object. This connection
   * will use "named" option values.
   */
  TipcSrv::TipcSrv(const char * name) throw (TipcException)
  {
    _srv = TipcSrvConnCreateNamed(const_cast<char *> (name));
    if (NULL == _srv)
      throw TipcSrvException();

  }

  TipcSrv::TipcSrv(T_IPC_SRV srv,bool destroyFlag) throw (TipcException)
  :TipcConnClient(TipcSrvConnGetConn(srv),false)
  {
    if (NULL == srv)
      throw TipcSrvException();

    _srv = srv;
  }

  /**
   * Get the C style T_IPC_SRV handle to the connection.
   * @throws TipcSrvException
   */
  TipcSrv::operator T_IPC_SRV () const throw (TipcException)
  {
    if (NULL == _srv)
      throw TipcSrvException();

    return _srv;
  }


  /* ======================================================================= */
  /*..TipcSrv::~TipcSrv -- TipcSrv destructor */
  TipcSrv::~TipcSrv() throw()
  {
    if (_destroy_flag)
      TipcSrvConnDestroy(_srv);
  }


  void TipcSrv::open(T_IPC_SRV_CONN_STATUS conn_status) throw (TipcException)
  {
    bool result = TipcSrvConnOpen(_srv, conn_status);
    if (false == result)
      throw TipcSrvException();
  }



  /* ======================================================================= */
  /*..TipcSrv::close -- closes the connection to RTserver */
  void TipcSrv::close(T_IPC_SRV_CONN_STATUS close_status) throw (TipcException)
  {
    if (!TipcSrvConnClose(_srv,close_status))
      throw TipcSrvException();

  }

  /* ======================================================================= */
  /*..TipcSrv::Check -- check if data can be read from or written to  */
  /*..  connectino to RTserver */
  bool TipcSrv::check(T_IO_CHECK_MODE check_mode, real8 timeout) throw (TipcException)
  {
    TutErrNumSet(0);
    bool retVal = TipcSrvConnCheck(_srv, check_mode, timeout);

    if (!retVal) {
      if (TutErrNumGet()  != 0) {
        throw TipcSrvException();
      }
    }

    return retVal;
  }


  /* ======================================================================= */
  /*..TipcSrv::errorCbCreate -- create an error callback in the connection */
  /*..  to RTserver */
  CallBack<ErrorCallBack> * TipcSrv::errorCbCreate(
                                                  ErrorCallBack * implementor,
                                                  void * arg)
  throw (TipcException)
  {
    ErrorCallBack::Handler handler = &ErrorCallBack::onError;
    CallBack<ErrorCallBack> * cb =
    new CallBack<ErrorCallBack> (implementor, handler);

    T_CB tcb = TipcSrvConnErrorCbCreate(_srv,errorCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcSrvException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }







  /* ======================================================================= */
  /*..TipcSrv::Flush -- flush buffered outgoing messages on the connection */
  /*..  to RTserver */
  void TipcSrv::flush() throw (TipcException)
  {
    if (!TipcSrvConnFlush(_srv))
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrv::getAutoFlushSize -- get the automatic flush size of the connection */
  /*..  to RTserver */
  int4 TipcSrv::getAutoFlushSize() const throw (TipcException)
  {
    int4 auto_flush_size_return = -1;  // initialize variable

    bool result = TipcSrvConnGetAutoFlushSize(_srv, &auto_flush_size_return);
    if (false == result)
      throw TipcSrvException();

    return auto_flush_size_return;
  }


  /* ======================================================================= */
  /*..TipcSrv::setAutoFlushSize -- set the automatic flush size of the connection */
  /*..  to RTserver */
  void TipcSrv::setAutoFlushSize(int4 auto_flush_size) throw (TipcException)
  {
    if (!TipcSrvConnSetAutoFlushSize(_srv, auto_flush_size))
      throw TipcSrvException();
  }

  /* ======================================================================= */
  /*..TipcSrv::trafficGetBytesRecv -- get the total number of bytes received */
  /*..                                on the connection                      */
  int4 TipcSrv::trafficGetBytesRecv() const throw (TipcException)
  {
    int4 bytes_recv = -1;  // initialize variable

    if (!TipcSrvConnTrafficGetBytesRecv(_srv, &bytes_recv)) {
      throw TipcSrvException();
    }

    return bytes_recv;
  }
  int8 TipcSrv::trafficGetBytesRecv8() const throw (TipcException)
  {
    int8 bytes_recv = -1;  // initialize variable

    if (!TipcSrvConnTrafficGetBytesRecv8(_srv, &bytes_recv)) {
      throw TipcSrvException();
    }

    return bytes_recv;
  }

  /* ======================================================================= */
  /*..TipcSrv::trafficGetBytesSent -- Get the total number of bytes written  */
  /*..                                into the write buffer on the           */
  /*..                                connection.  This is usually the same  */
  /*..                                as the total number of bytes actually  */
  /*..                                sent on the connection, but sometimes  */
  /*..                                the retrieved number is larger than    */
  /*..                                the number of bytes sent when the      */
  /*..                                buffer is not frequently flushed.      */
  int4 TipcSrv::trafficGetBytesSent() const throw (TipcException)
  {
    int4 bytes_sent = -1;  // initialize variable

    if (!TipcSrvConnTrafficGetBytesSent(_srv, &bytes_sent)) {
      throw TipcSrvException();
    }

    return bytes_sent;
  }
  int8 TipcSrv::trafficGetBytesSent8() const throw (TipcException)
  {
    int8 bytes_sent = -1;  // initialize variable

    if (!TipcSrvConnTrafficGetBytesSent8(_srv, &bytes_sent)) {
      throw TipcSrvException();
    }

    return bytes_sent;
  }

  /* ======================================================================= */
  /*..TipcSrv::trafficGetMsgsRecv -- get the total number of messages        */
  /*..                               received on the connection              */
  int4 TipcSrv::trafficGetMsgsRecv() const throw (TipcException)
  {
    int4 msgs_recv = -1;  // initialize variable

    if (!TipcSrvConnTrafficGetMsgsRecv(_srv, &msgs_recv)) {
      throw TipcSrvException();
    }

    return msgs_recv;
  }
  int8 TipcSrv::trafficGetMsgsRecv8() const throw (TipcException)
  {
    int8 msgs_recv = -1;  // initialize variable

    if (!TipcSrvConnTrafficGetMsgsRecv8(_srv, &msgs_recv)) {
      throw TipcSrvException();
    }

    return msgs_recv;
  }

  /* ======================================================================= */
  /*..TipcSrv::trafficGetMsgsSent -- Get the total number of messages        */
  /*..                               written into the write buffer on the    */
  /*..                               connection.  This is usually the same   */
  /*..                               as the total number of messages         */
  /*..                               actually sent on the connection, but    */
  /*..                               sometimes the retrieved number is       */
  /*..                               larger than the number of messages sent */
  /*..                               when the buffer is not frequently       */
  /*..                               flushed.                                */
  int4 TipcSrv::trafficGetMsgsSent() const throw (TipcException)
  {
    int4 msgs_sent = -1;  // initialize variable

    if (!TipcSrvConnTrafficGetMsgsSent(_srv, &msgs_sent)) {
      throw TipcSrvException();
    }

    return msgs_sent;
  }
  int8 TipcSrv::trafficGetMsgsSent8() const throw (TipcException)
  {
    int8 msgs_sent = -1;  // initialize variable

    if (!TipcSrvConnTrafficGetMsgsSent8(_srv, &msgs_sent)) {
      throw TipcSrvException();
    }

    return msgs_sent;
  }

  /* ======================================================================= */
  /*..TipcSrv::bufferGetReadSize -- get the total number of bytes in the     */
  /*..                              connection's read buffer                 */
  int4 TipcSrv::bufferGetReadSize() const throw (TipcException)
  {
    int4 bytes = -1;  // initialize variable

    if (!TipcSrvConnBufferGetReadSize(_srv, &bytes)) {
      throw TipcSrvException();
    }

    return bytes;
  }

  /* ======================================================================= */
  /*..TipcSrv::bufferGetWriteSize -- get the total number of bytes in the    */
  /*..                               connection's read buffer                */
  int4 TipcSrv::bufferGetWriteSize() const throw (TipcException)
  {
    int4 bytes = -1;  // initialize variable

    if (!TipcSrvConnBufferGetWriteSize(_srv, &bytes)) {
      throw TipcSrvException();
    }

    return bytes;
  }


  /* ======================================================================= */
  /*..TipcSrv::getNumQueued -- get the number of queued message from the  */
  /*..  connection to RTserver */
  int4 TipcSrv::getNumQueued() const throw (TipcException)
  {
    int4 num_queued_return = -1;  // initialize variable

    if (!TipcSrvConnGetNumQueued(_srv, &num_queued_return))
      throw TipcSrvException();

    return num_queued_return;
  }


  /* ======================================================================= */
  /*..TipcSrv::getSocket -- get the socket of the connection to RTserver */
  sock TipcSrv::getSocket() const throw (TipcException)
  {
    sock socket_return = -1;  // initialize variable


    bool result = TipcSrvConnGetSocket(_srv, &socket_return);
    if (false == result)
      throw TipcSrvException();

    return socket_return;
  }

  /* ======================================================================= */
  /*..TipcSrv::getTimeout -- get a timeout property from the connection to */
  /*..  RTserver */
  real8 TipcSrv::getTimeout(T_IPC_TIMEOUT timeout) const throw (TipcException)
  {
    T_REAL8 value_return = -1.0;  // initialize variable

    bool result = TipcSrvConnGetTimeout(_srv, timeout, &value_return);
    if (false == result)
      throw TipcSrvException();

    return value_return;
  }


  /* ======================================================================= */
  /*..TipcSrv::setTimeout -- set a timeout property of the connecction to */
  /*..  RTserver */
  void TipcSrv::setTimeout(T_IPC_TIMEOUT timeout, real8 value) throw (TipcException)
  {
    if (!TipcSrvConnSetTimeout(_srv, timeout, value))
      throw TipcSrvException();
  }

  /* ======================================================================= */
  /*..TipcSrv::getXtSource -- get source suitable from XtAppAddInput from the */
  /*..  connection to RTserver */
  sock TipcSrv::getXtSource() const throw (TipcException)
  {
    sock source_return = -1;  // initialize variable

    bool result  = TipcSrvConnGetXtSource(_srv, &source_return);
    if (false == result)
      throw TipcSrvException();

    return source_return;
  }

  /* ======================================================================= */
  /*..TipcSrv::setUsernamePassword -- set the username/password for the */
  /*..  connection to RTserver */
  void TipcSrv::setUsernamePassword(const char * username, const char * password) throw (TipcException)
  {
    if (!TipcSrvConnSetUsernamePassword(_srv,
                                        const_cast<char *>(username),
                                        const_cast<char *>(password)))
      throw TipcSrvException();
  }

  /* ======================================================================= */
  /*..TipcSrv::keepAlive -- check if the connection to RTserver is still alive */
  void TipcSrv::keepAlive() throw (TipcException)
  {
    if (!TipcSrvConnKeepAlive(_srv))
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrv::mainLoop -- read and process messages on the connection to */
  /*..  RTserver */
  bool TipcSrv::mainLoop(real8 timeout) throw (TipcException)
  {
    try {
      if (!TipcSrvConnMainLoop(_srv, timeout)) {
        if (TutErrNumGet() != T_ERR_TIMEOUT_REACHED) {
          throw TipcSrvException();
        }
        return false;
      }
    }
    catch (...) {
      throw TipcSrvException();
    }

    return true;

  }


  /* ======================================================================= */
  /*..TipcSrv::insert -- insert message into queue of the connection to */
  /*..  RTserveTipcSrvExceptionr */
  void TipcSrv::insert(TipcMsg& msg, int4 pos) throw (TipcException)
  {
    if (!TipcSrvConnMsgInsert(_srv, msg._msg, pos))
      throw TipcSrvException();


    msg._msg = (T_IPC_MSG)T_NULL;
  }


  /* ======================================================================= */
  /*..TipcSrv::next -- get the next message from the connection to RTserver */
  void TipcSrv::next(TipcMsg& retMsg, real8 timeout) throw (TipcException)
  {
    T_IPC_MSG msg = TipcSrvConnMsgNext(_srv, timeout);

    if (msg == NULL)
      throw TipcSrvException();

    retMsg.absorb(msg);

  }

  /* ======================================================================= */
  /*..TipcSrv::nextEx -- get the next message from the connection to RTserver */
  /*..returns false if the timeout is expired, true otherwise. */
  bool TipcSrv::nextEx(TipcMsg& retMsg, real8 timeout) throw (TipcException)
  {
    T_IPC_MSG msg = TipcSrvConnMsgNext(_srv, timeout);
    if (NULL == msg) {
      if (T_ERR_TIMEOUT_REACHED != TutErrNumGet()) {
        throw TipcSrvException();
      }
      return false;
    }

    retMsg.absorb(msg);
    return true;
  }


  /* ======================================================================= */
  /*..TipcSrv::Process -- process a message in the connection to RTserver */
  void TipcSrv::process(TipcMsg& msg) throw (TipcException)
  {
    if (!TipcSrvConnMsgProcess(_srv, msg._msg))
      throw TipcSrvException();
  }


  void TipcSrv::search(TipcMsg& msg, TipcConnSearchSelector* selObj,
                       real8 timeout)
  throw (TipcException)
  {

    TipcConnSearchSelector::Handler selMethod = NULL;
    if (selObj == NULL) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcSrvException();
    }
    if (selMethod != T_NULL_FUNC) {
      selObj->setHandler(selMethod);
    }

    TutErrNumSet(0);


    T_IPC_MSG cMsg = TipcSrvConnMsgSearch(_srv, timeout,
                                          TipcConnSearchSelector::searchFuncDelegator,
                                          (T_PTR)selObj);

    if (cMsg == NULL)
      throw TipcSrvException();

    msg.absorb(cMsg);

  }

  /* ======================================================================= */
  /*..TipcSrv::SearchType -- search the message queue of a connection to  */
  /*..  RTserver for a message with a specific type */
  void TipcSrv::searchType(TipcMsg& msg,TipcMt& mt, real8 timeout)
  throw (TipcException)
  {
    T_IPC_MSG cMsg = TipcSrvConnMsgSearchType(_srv, timeout, mt._mt);

    if (cMsg == NULL)
      throw TipcSrvException();

    msg.absorb(cMsg);
  }

  /* ======================================================================= */
  /*..TipcSrv::send -- send a message through the connection to RTserver */
  void TipcSrv::send(const TipcMsg& msg, bool check_server_msg_send) throw (TipcException)
  {
    if (!TipcSrvConnMsgSend(_srv, msg._msg, check_server_msg_send))
      throw TipcSrvException();

  }


  /* ======================================================================= */
  /*..TipcSrv::sendRpc -- make a remote procedure call with messages on the */
  /*..  connection to RTserver */
  void TipcSrv::sendRpc(TipcMsg& reply_msg, const TipcMsg& call_msg, real8 timeout) throw (TipcException)
  {
    T_IPC_MSG msg = TipcSrvConnMsgSendRpc(_srv, call_msg._msg, timeout);

    if (msg == NULL)
      throw TipcSrvException();

    reply_msg.absorb(msg);

  }


  /* ======================================================================= */
  /*..TipcSrv::write -- construct a message and send it through the  */
  /*..  connection to RTserver */
  void TipcSrv::write(const char * dest, TipcMt& mt,
                      bool check_server_msg_send, ...) throw (TipcException)
  {
    va_list var_arg_list;   // To point to each unnamed arg in turn
    bool status;

  /* The Alpha VMS DEC C v6.0 compiler for OpenVMS Alpha v7.2-1 abends  */
  /* with an access violation.  I believe this happens because va_start */
  /* has a problem with the 2nd parm being a bool type. Here's what the */
  /* man pages say about the 2nd parm for va_start: "If this parameter  */
  /* is declared with the register storage class or with a function or  */
  /* array type,  or  with  a type that is not compatible with the type */
  /* that results after application of the default argument  promotions,*/
  /* the behavior is undefined."                                        */
  /* So we will va_start using the prior argument which means the       */
  /* variable argument pointer (var_arg_list in this case) will point at*/
  /* the bool first.  Then we will use va_arg to skip to the next arg   */
  /* in the list which is the true start of the variable args list.     */

#if defined(T_OS_VMS)
    va_start(var_arg_list, mt);
    va_arg(var_arg_list, int);
#else
    va_start(var_arg_list, check_server_msg_send);
#endif
    status = TipcSrvConnMsgWriteVa(_srv, const_cast<char *> (dest), mt._mt,
                                        check_server_msg_send, var_arg_list);

    va_end(var_arg_list);

    if (false == status)
      throw TipcSrvException();

  }

  /* ======================================================================= */
  /*..TipcSrv::writeVa -- construct a message and send it through the */
  /*..  connection to RTserver (va_list version) */
  void TipcSrv::writeVa(const char * dest, TipcMt& mt,
                        bool check_server_msg_send, va_list var_arg_list)
  throw (TipcException)
  {
    if (!TipcSrvConnMsgWriteVa(_srv, const_cast<char *> (dest), mt._mt,
                               check_server_msg_send, var_arg_list))
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrv::Print -- print all information about the connection to RTserver */
  void TipcSrv::print(T_OUT_FUNC func) const throw (TipcException)
  {
    if (!TipcSrvConnPrint(_srv, func))
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrv::defaultCbCreate -- create a default callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcSrv::defaultCbCreate(
                                                      MessageCallBack * implementor,
                                                      void* arg)
  throw (TipcException)
  {

    MessageCallBack::Handler handler = &MessageCallBack::onMessage;

    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcSrvConnDefaultCbCreate(_srv,messageCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcSrvException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }



  /* ======================================================================= */
  /*..TipcSrv::processCbCreate -- create a process callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcSrv::processCbCreate(
                                                      const TipcMt & mt,
                                                      MessageCallBack * implementor,
                                                      void * arg)
  throw (TipcException)
  {
    MessageCallBack::Handler handler = &MessageCallBack::onMessage;
    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcSrvConnProcessCbCreate(_srv,mt._mt, messageCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcSrvException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }

  /* ======================================================================= */
  /*..TipcSrv::queueCbCreate -- create a queue callback in the connection */
  /*..  to RTserver */
  CallBack<MessageQueueCallBack> * TipcSrv::queueCbCreate(
                                                         const TipcMt& mt,
                                                         MessageQueueCallBack * implementor,
                                                         void * arg)
  throw (TipcException)
  {
    MessageQueueCallBack::Handler handler = &MessageQueueCallBack::onMessage;
    CallBack<MessageQueueCallBack> * cb =
    new CallBack<MessageQueueCallBack> (implementor, handler);

    T_CB tcb = TipcSrvConnQueueCbCreate(_srv,mt, queueCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcSrvException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }




  /* ======================================================================= */
  /*..TipcSrv::read -- read all available data from the connection to RTserver */
  /*..  and queue messages in priority order */
  void TipcSrv::read(real8 timeout) throw (TipcException)
  {
    if (!TipcSrvConnRead(_srv, timeout))
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrv::readCbCreate -- create a read callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcSrv::readCbCreate(
                                                   const TipcMt & mt,
                                                   MessageCallBack * implementor,
                                                   void * arg)
  throw (TipcException)
  {
    MessageCallBack::Handler handler = &MessageCallBack::onMessage;
    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcSrvConnReadCbCreate(_srv,mt._mt, readCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcSrvException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }





  /* ======================================================================= */
  /*..TipcSrv::writeCbCreate -- create a write callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcSrv::writeCbCreate(
                                                    const TipcMt & mt,
                                                    MessageCallBack * implementor,
                                                    void * arg)
  throw (TipcException)
  {
    MessageCallBack::Handler handler = &MessageCallBack::onMessage;

    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcSrvConnWriteCbCreate(_srv,mt._mt, messageCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcSrvException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }




  /* ======================================================================= */
  /*..TipcSrv::encryptCbCreate -- create an encrypt callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcSrv::encryptCbCreate(
                                                      const TipcMt & mt,
                                                      MessageCallBack * implementor,
                                                      void * arg)
  throw (TipcException)
  {

    MessageCallBack::Handler handler = &MessageCallBack::onMessage;
    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcSrvConnEncryptCbCreate(_srv,mt._mt, messageCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcSrvException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }


  /* ======================================================================= */
  /*..TipcSrv::decryptCbCreate -- create a decrypt callback in the connection */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcSrv::decryptCbCreate(
                                                      const TipcMt & mt,
                                                      MessageCallBack * implementor,
                                                      void * arg)
  throw (TipcException)
  {
    MessageCallBack::Handler handler = &MessageCallBack::onMessage;
    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcSrvConnDecryptCbCreate(_srv,mt._mt, messageCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcSrvException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }




  // ----------------------------------------------------------------------
  // TipcSrv functions that have no TipcConn parallel
  // ----------------------------------------------------------------------

  /* ======================================================================= */
  /*..TipcSrv::openCbCreate -- create a server create callback */
  CallBack<ConnectionCallBack> * TipcSrv::openCbCreate(
                                                      ConnectionCallBack * implementor,
                                                      void * arg)
  throw (TipcException)
  {
    ConnectionCallBack::Handler handler = &ConnectionCallBack::onConnection;

    CallBack<ConnectionCallBack> * cb =
    new CallBack<ConnectionCallBack> (implementor, handler);

    T_CB tcb = TipcSrvConnOpenCbCreate(_srv,openCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcSrvException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }




  /* ======================================================================= */
  /*..TipcSrv::closeCbCreate -- create a server destroy callback */
  CallBack<ConnectionCallBack> * TipcSrv::closeCbCreate(
                                                       ConnectionCallBack * implementor,
                                                       void * arg)
  throw (TipcException)
  {
    ConnectionCallBack::Handler handler = &ConnectionCallBack::onConnection;

    CallBack<ConnectionCallBack> * cb =
    new CallBack<ConnectionCallBack> (implementor, handler);

    T_CB tcb = TipcSrvConnCloseCbCreate(_srv,closeCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcSrvException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }




  // ---------------------------------------------------------------
  // The following functions may establish a connection to TipcSrv
  // under certain circumstances
  // ---------------------------------------------------------------

  /* ======================================================================= */
  /*..TipcSrv::SubjectSubscribe -- get whether or not a client process is */
  /*.. receiving a datagroup */
  bool TipcSrv::getSubscribe(const char * subject) throw (TipcException)
  {
    T_BOOL subscribe_status_return = T_FALSE;  // initialize variable

    if (!TipcSrvConnSubjectGetSubscribe(_srv, const_cast<char *> (subject),
                                        &subscribe_status_return))
      throw TipcSrvException();

    return(subscribe_status_return?true:false);
  }

  /* ======================================================================= */
  /*..TipcSrv::setSubscribe -- start or stop receiving a subject */
  void TipcSrv::setSubscribe(const char * subject, bool subscribe_status)
  throw (TipcException)
  {
    if (!TipcSrvConnSubjectSetSubscribe(_srv, const_cast<char *> (subject),
                                        subscribe_status))
      throw TipcSrvException();
  }

  /* ======================================================================= */
  /*..TipcSrv::traverseSubscribe -- traverse the subjects that the */
  /*..  client process is receiving */
  void TipcSrv::traverseSubscribe(TipcSrvSubjTraverser* travObj)
  throw (TipcException)

  {
    TipcSrvSubjTraverser::Handler travMethod = NULL;
    if (travObj == NULL) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcSrvException();
    }
    if (travMethod != T_NULL_FUNC) {
      travObj->setHandler(travMethod);
    }

    TutErrNumSet(0);

    void * result = TipcSrvConnSubjectTraverseSubscribe(_srv,
                                                        TipcSrvSubjTraverser::traverseFuncDelegator,
                                                        (T_PTR) travObj);

    if (NULL == result) {
      int4  errNum = TutErrNumGet();

      if ((errNum!= 0) && (errNum != T_ERR_END_OF_TRAVERSAL)) {
        throw TipcSrvException();
      }
    }

  }


  // ---------------------------------------------------------------
  // TipcSrvIsRunning also possibly creates connection to TipcSrv;
  // the next function is a static method.
  // ---------------------------------------------------------------

  /* ======================================================================= */
  /*..TipcSrv::isRunning -- check if the RTclient process can create a */
  /*..  connection to RTserver */
  //??? DO WE NEED THIS...
#if 0
  bool TipcSrv::isRunning() throw (TipcException)
  {
    bool ret = TipcSrvConnIsRunning();
    if (false == ret)
      throw TipcSrvException();

    return ret;
  }
#endif

  /* ======================================================================= */
  /*..TipcSrv::LogAddMt -- add a message type to a message file logging type */
  void TipcSrv::logAddMt(T_IPC_MT_LOG_TYPE log_type,
                         TipcMt& mt) throw (TipcException)
  {
    bool ret  = TipcMtLogAddMt(log_type, mt._mt);
    if (false == ret)
      throw TipcMtException();
  }


  /* ======================================================================= */
  /*..TipcSrv::LogRemoveMt -- remove a message type from a message file */
  /*..  logging type */
  void TipcSrv::logRemoveMt(T_IPC_MT_LOG_TYPE log_type,
                            TipcMt& mt) throw (TipcException)
  {
    bool ret = TipcMtLogRemoveMt(log_type, mt._mt);
    if (false == ret)
      throw TipcMtException();
  }


  /* ======================================================================= */
  /*..TipcSrv::StdSubjectSetSubscribe -- start or stop receiving all standard */
  /*.. subjects */
  void TipcSrv::setSubscribeStd(bool subscribe_status) throw (TipcException)
  {
    bool ret = TipcSrvConnStdSubjectSetSubscribe(_srv, subscribe_status);
    if (false == ret)
      throw TipcSrvException();
  }

  /* ======================================================================= */
  /*..TipcSrv::getUniqueSubject -- get the unique subject of the connected */
  /*..  RTserver process */
  const char * TipcSrv::getPeerUniqueSubject() const throw (TipcException)
  {
    char * unique_subject_return = (char *) "";  // initialize variable

    bool result = TipcSrvConnGetUniqueSubject(_srv, &unique_subject_return);
    if (false == result)
      throw TipcSrvException();

    return unique_subject_return;
  }


  /* ======================================================================= */
  /*..TipcSrv::getNode -- get the node name of the connected RTserver process */
  const char * TipcSrv::getNode() const throw (TipcException)
  {
    char * node_return = (char *)"";  // initialize variable

    bool result = TipcSrvConnGetNode(_srv, &node_return);
    if (false == result)
      throw TipcSrvException();

    return node_return;
  }


  /* ======================================================================= */
  /*..TipcSrv::getUser -- get the user name of the connected RTserver process */
  const char * TipcSrv::getUser() const throw (TipcException)
  {
    char * user_return = (char *)"";  // initialize variable

    bool result = TipcSrvConnGetUser(_srv, &user_return);
    if (false == result)
      throw TipcSrvException();

    return user_return;
  }


  /* ======================================================================= */
  /*..TipcSrv::getPid -- get the process ID of the connected RTserver process */
  int4 TipcSrv::getPid() const throw (TipcException)
  {
    int4 pid_return = -1;  // initialize variable

    bool result = TipcSrvConnGetPid(_srv, &pid_return);
    if (false == result)
      throw TipcSrvException();

    return pid_return;
  }


  /* ======================================================================= */
  /*..TipcSrv::getArch -- get the architecture name of the connected RTserver */
  /*..  process */
  const char *TipcSrv::getArch() const throw (TipcException)
  {
    char *arch_return = (char *)"";  // initialize variable

    bool result = TipcSrvConnGetArch(_srv, &arch_return);
    if (false == result)
      throw TipcSrvException();

    return arch_return;
  }


  /* ======================================================================= */
  /*..TipcSrv::getStatus -- get the status of the connection to RTserver */
  T_IPC_SRV_CONN_STATUS TipcSrv::getStatus() const throw (TipcException)
  {
    T_IPC_SRV_CONN_STATUS conn_status;

    bool result = TipcSrvConnGetConnStatus(_srv, &conn_status);
    if (false == result)
      throw TipcSrvException();

    return conn_status;
  }


  /* ======================================================================= */
  /*..TipcSrv::GmdFileCreate -- create files needed for guaranteed message */
  /*..  delivery on the connection to RTserver */
  void TipcSrv::gmdFileCreate() throw (TipcException)
  {
    if (!TipcSrvConnGmdFileCreate(_srv))
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrv::gmdFileDelete -- delete from disk guaranteed message delivery */
  /*..  files for the connection to RTserver */
  void TipcSrv::gmdFileDelete() throw (TipcException)
  {
    if (!TipcSrvConnGmdFileDelete(_srv))
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrv::gmdResend -- resend guaranteed messages after a delivery  */
  /*..  failure on the connection to RTserver */
  void TipcSrv::gmdResend() throw (TipcException)
  {
    if (!TipcSrvConnGmdResend(_srv))
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrv::gmdMsgDelete -- delete an outgoing message from the connection */
  /*..  to RTserver GMD files */
  void TipcSrv::gmdMsgDelete(TipcMsg& msg) throw (TipcException)
  {
    if (!TipcSrvConnGmdMsgDelete(_srv, msg._msg))
      throw TipcSrvException();
  }

#if 0

  /* ======================================================================= */
  /*..TipcSrv::gmdMsgResend -- resend on spooled guaranteed message on the */
  /*..  connection to RTserver */
  void TipcSrv::gmdMsgResend(TipcMsg& msg) throw (TipcException)
  {
    if (!TipcSrvConnGmdMsgResend(_srv, msg._msg))
      throw TipcSrvException();
  }

#endif


  /* ======================================================================= */
  /*..TipcSrv::getGmdMaxSize -- get gmd max size for a connection */
  uint4 TipcSrv::getGmdMaxSize() const throw (TipcException)
  {
    uint4 gmd_max_size_return = 0;  // initialize varible

    if (!TipcSrvConnGetGmdMaxSize(_srv, &gmd_max_size_return))
      throw TipcSrvException();

    return gmd_max_size_return;
  }


  /* ======================================================================= */
  /*..TipcSrv::setGmdMaxSize -- set gmd max size for a connection */
  void TipcSrv::setGmdMaxSize(uint4 gmd_max_size) throw (TipcException)
  {
    if (!TipcSrvConnSetGmdMaxSize(_srv, gmd_max_size))
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrv::gmdMsgServerDelete -- tell RTserver to forget about a GMD */
  /*..  message */
  void TipcSrv::gmdMsgServerDelete(TipcMsg& msg) throw (TipcException)
  {
    if (!TipcSrvConnGmdMsgServerDelete(_srv, msg._msg))
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrv::gmdMsgStatus -- query RTserver for GMD status */
  void TipcSrv::gmdMsgStatus(TipcMsg& msg) throw (TipcException)
  {
    if (!TipcSrvConnGmdMsgStatus(_srv, msg._msg))
      throw TipcSrvException();
  }

  /* ======================================================================= */
  /*..TipcSrv::getSubscribeLb -- get subscribe w/ load balancing */
  bool TipcSrv::getSubscribeLb(const char * subject,
                               bool *lb_status_return) throw (TipcException)
  {
    T_BOOL subscribe_status_return = false;
    T_BOOL lb_status;

    if (!TipcSrvConnSubjectGetSubscribeLb(_srv, const_cast<char *> (subject),
                                          &subscribe_status_return, &lb_status))
      throw TipcSrvException();

    *lb_status_return = (bool)lb_status;
    return(subscribe_status_return?true:false);
  }

  /* ======================================================================= */
  /*..TipcSrv::setSubscribeLb -- set subscribe w/ load balancing */
  void TipcSrv::setSubscribeLb(const char * subject,
                               bool subject_status,
                               bool lb_status) throw (TipcException)
  {
    if (!TipcSrvConnSubjectSetSubscribeLb(_srv, const_cast<char *> (subject),
                                          subject_status, lb_status))
      throw TipcSrvException();
  }

  /* ======================================================================= */
  /*..TipcSrv::subjectGmdInit -- turn on GMD for a subject */
  void TipcSrv::subjectGmdInit(const char * subject) throw (TipcException)
  {
    if (!TipcSrvConnSubjectGmdInit(_srv, const_cast<char *> (subject)))
      throw TipcSrvException();
  }

  /* ======================================================================= */
  /*..TipcSrv::subjectLbInit -- turn on Lb for a subject */
  void TipcSrv::subjectLbInit(const char * subject) throw (TipcException)
  {
    if (!TipcSrvConnSubjectLbInit(_srv, const_cast<char *> (subject)))
      throw TipcSrvException();
  }

  /* ======================================================================= */
  /*..TipcSrv::lock -- acquire exclusive access to the server connection */
  void TipcSrv::lock() throw (TipcException)
  {
    if (!TipcSrvConnLock(_srv))
      throw TipcSrvException();
  }

  /* ======================================================================= */
  /*..TipcSrv::unlock -- release exclusive access to the server connection */
  void TipcSrv::unlock() throw (TipcException)
  {
    if (!TipcSrvConnUnlock(_srv))
      throw TipcSrvException();
  }

  /* ======================================================================= */
  /*..TipcSrv::getGmdNumPending -- get the number of messages pending GMD */
  int4 TipcSrv::getGmdNumPending() const throw (TipcException)
  {
    int4 num_return;

    bool result = TipcSrvConnGetGmdNumPending(_srv, &num_return);
    if (false == result)
      throw TipcSrvException();

    return num_return;
  }


  /* ======================================================================= */
  /*..TipcSrv::subjectCbCreate -- create a srv subjcect callback               */
  /*..  to RTserver */
  CallBack<MessageCallBack> * TipcSrv::subjectCbCreate(
                                                      const char * subject,
                                                      const TipcMt & mt,
                                                      MessageCallBack * implementor,
                                                      void* arg)
  throw (TipcException)
  {
    MessageCallBack::Handler handler = &MessageCallBack::onMessage;
    CallBack<MessageCallBack> * cb =
    new CallBack<MessageCallBack> (implementor, handler);

    T_CB tcb = TipcSrvConnSubjectCbCreate(_srv,(T_STR) subject, mt._mt,
                                          subjectCbDelegator, cb);

    if (!tcb) {
      delete cb;
      throw TipcSrvException();
    }

    cb->setTCB(tcb);
    cb->setArgument(arg);

    return cb;
  }



  /**
   * This call is used to subscibe to a subject using extended
   * options. Currently the only option available is caching.
   * @throws TipcSrvException
   */
  void TipcSrv::setSubscribeCache(const char * subj_name,
                                  bool status,
                                  int4 flags)
  throw (TipcException)
  {
    bool result = TipcSrvConnSubjectSetSubscribeEx(_srv,
                                                   const_cast<char *> (subj_name),
                                                   status,
                                                   T_SUBJ_CACHE,
                                                   flags, NULL);
    if (false == result)
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrvConnOpenWithCredentials -- create connection to RTserver and auth */
  void TipcSrv::setCredentials(int4 auth_policy_id, void * auth_data,
                               int4 auth_data_len) throw (TipcException)
  {
    bool result = TipcSrvConnSetCredentials(_srv, auth_policy_id,
                                            auth_data, auth_data_len);

    if (false == result)
      throw TipcSrvException();
  }


  /* ======================================================================= */
  /*..TipcSrv::subjectCbDelegator -- delegator for subject callbacks */
  void T_ENTRY1 TipcSrv::subjectCbDelegator(T_IPC_CONN connection,
                                            T_IPC_SRV_SUBJECT_CB_DATA data,
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
  /*..TipcSrv::openCbDelegator -- delegator for opening a connection */
  void T_ENTRY1 TipcSrv::openCbDelegator(T_IPC_SRV connection,
                                         T_IPC_SRV_OPEN_CB_DATA data,
                                         T_CB_ARG arg)
  {
    CallBack<ConnectionCallBack> * callBack =
    static_cast <CallBack <ConnectionCallBack> *> (arg);

    TipcSrv srv (connection,false);

    ConnectionCallBack::ConnStatus old_status =
    (ConnectionCallBack::ConnStatus) (data->old_conn_status);
    ConnectionCallBack::ConnStatus new_status =
    (ConnectionCallBack::ConnStatus) (data->new_conn_status);

    try {
      ((callBack->m_implementor)->*(callBack->m_handler)) (
                                                          callBack,
                                                          srv,
                                                          old_status,
                                                          new_status);
    }
    catch (Exception& e) {
      Utilities::out("Caught exception from Callback method.  Do not throw any exceptions"
                     "from CallBack methods.\n  <error = %s>\n",e.what());
    }

  }


  /* ======================================================================= */
  /*..TipcSrv::closeCbDelegator -- delegator for closing a connection */
  void T_ENTRY1 TipcSrv::closeCbDelegator(T_IPC_SRV connection,
                                          T_IPC_SRV_CLOSE_CB_DATA data,
                                          T_CB_ARG arg)
  {
    CallBack<ConnectionCallBack> * callBack =
    static_cast <CallBack <ConnectionCallBack> *> (arg);


    TipcSrv srv (connection,false);

    ConnectionCallBack::ConnStatus old_status =
    (ConnectionCallBack::ConnStatus) (data->old_conn_status);
    ConnectionCallBack::ConnStatus new_status =
    (ConnectionCallBack::ConnStatus) (data->new_conn_status);

    try {
      ((callBack->m_implementor)->*(callBack->m_handler)) (
                                                          callBack,
                                                          srv,
                                                          old_status,
                                                          new_status);
    }
    catch (Exception& e) {
      Utilities::out("Caught exception from Callback method.  Do not throw any exceptions"
                     "from CallBack methods.\n  <error = %s>\n",e.what());
    }
  }

  void TipcSrv::setProject(const char* project)
  throw (TipcException)
  {
    if (!TipcSrvConnSetProject(_srv, const_cast<char *> (project)))
      throw TipcSrvException();
  }


  const char* TipcSrv::getProject() throw (TipcException)
  {
    char * project = (char *)"";  // initialize variable

    if (!TipcSrvConnGetProject(_srv, &project))
      throw TipcSrvException();

    return project;
  }

  void TipcSrv::setServerNames(const char* server_names)
  throw (TipcException)
  {
    if (!TipcSrvConnSetServerNames(_srv, const_cast<char *> (server_names)))
      throw TipcSrvException();
  }

  void TipcSrv::setServerNamesList(T_STR_LIST server_names_list)
  throw (TipcException)
  {
    if (!TipcSrvConnSetServerNamesList(_srv, server_names_list))
      throw TipcSrvException();
  }

  T_STR_LIST TipcSrv::getServerNamesList()
  throw (TipcException)
  {
    T_STR_LIST server_names_list;
    if (!TipcSrvConnGetServerNamesList(_srv, &server_names_list))
      throw TipcSrvException();

    return server_names_list;
  }

  void TipcSrv::setDefaultSubjectPrefix(const char* default_subject_prefix)
  throw (TipcException)
  {
    if (!TipcSrvConnSubjectSetDefaultPrefix(_srv,
                                            const_cast<char *> (default_subject_prefix)))
      throw TipcSrvException();
  }

  const char * TipcSrv::getDefaultSubjectPrefix() throw (TipcException)
  {
    char * default_subject_prefix = (char*) "";
    if (!TipcSrvConnSubjectGetDefaultPrefix(_srv, &default_subject_prefix))
      throw TipcSrvException();

    return default_subject_prefix;
  }

  void TipcSrv::setUniqueSubject(const char * unique_subject)
  throw (TipcException)
  {
    if (!TipcSrvConnSubjectSetUnique(_srv, const_cast<char *> (unique_subject)))
      throw TipcSrvException();
  }

  const char * TipcSrv::getUniqueSubject() throw (TipcException)
  {
    char * unique_subject = (char*) "";
    if (!TipcSrvConnSubjectGetUnique(_srv, &unique_subject))
      throw TipcSrvException();

    return unique_subject;
  }


} //namespace SmartSockets

