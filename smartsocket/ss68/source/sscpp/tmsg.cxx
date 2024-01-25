/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tmsg.cxx#3 $
 */

#include "includes.h"
#include <rtworks/tex.h>
#include <rtworks/tmt.h>
#include <rtworks/tmsg.h>
#include <rtworks/txml.h>

namespace SmartSockets {


#define CALL_HANDLER(object,ptrToMember)  ((object).*(ptrToMember))

  TipcMsgTraverser::TipcMsgTraverser()
  {
#if !defined(T_OS_SOLARIS) || defined(__GNUC__)
    _cbMethod = &TipcMsgTraverser::onTraverse;
#else
    _cbMethod = (Handler)this->onTraverse;
#endif
  }

  void TipcMsgTraverser::setHandler(TipcMsgTraverser::Handler cbMethod)
  {
    _cbMethod = cbMethod;
  }

  TipcMsgTraverser::Handler TipcMsgTraverser::getHandler()
  {
    return _cbMethod;
  }


  TipcMsgTraverser::~TipcMsgTraverser() throw()
  {
  }

  bool TipcMsgTraverser::onTraverse
  (
  const TipcMsg&  msg,
  T_IPC_FT        type,
  bool            isUnknown,
  void*           valuePtr,
  int4            size,
  T_IPC_MSG_FIELD field,
  int4            index,
  const char*     name
  )
  throw ()
  {
    return false;
  }

  T_PTR T_ENTRY TipcMsgTraverser:: traverseFuncDelegator 
  (
  T_IPC_MSG msg,
  T_IPC_MSG_TRAV trav,
  T_PTR arg
  )
  {
    if (arg == NULL)
      throw TipcMsgException();

    TipcMsgTraverser* cb = (TipcMsgTraverser*) arg;

    TipcMsgTraverser::Handler method = (cb->getHandler());

    if (method == T_NULL_FUNC) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMsgException();
    }


    bool retVal = false;

    TipcMsg msgArg (msg);
    retVal = CALL_HANDLER(*cb,method) (msgArg,
                                       (T_IPC_FT)*(trav->type_ptr),
                                       (trav->unknown?true:false),
                                       (trav->value_ptr),
                                       (trav->array_size_ptr?*trav->array_size_ptr:0),
                                       (trav->field),
                                       (trav->index),
                                       const_cast<const char*>(trav->name));


    return retVal?NULL:arg;
  }



  void TipcMsg::traverse(TipcMsgTraverser* travObj)
  throw (TipcException)
  {

    TipcMsgTraverser::Handler travMethod = NULL;

    if (_msg == NULL||travObj == NULL) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMsgException();
    }


    if (travMethod != T_NULL_FUNC) {
      travObj->setHandler(travMethod);
    }


    TipcMsgTraverse(_msg, TipcMsgTraverser::traverseFuncDelegator, (T_PTR)travObj);

  }


  /* ======================================================================== */
  /*..TipcMsg::transform -- transform T_IPC_MSG to TipcMsg */
  void TipcMsg::absorb(T_IPC_MSG msg)
  {
    /* ---------------------------------------------------------------------- */
    /* This function should be used cautiously.  It will overwrite the _msg   */
    /* pointers. Friend classes should only use this function on their        */
    /* own private TipcMsg objects which are used as a referent in member     */
    /* functions which return TipcMsg objects by reference. Friend classes    */
    /* must call the destroyMsg() member function on their private TipcMsg    */
    /* object before calling aborb() to guarantee proper memory management.   */
    /*                                                                        */
    /* We don't increment the T_IPC_MSG reference count because this function */
    /* takes over management of a T_IPC_MSG message.                          */
    /* ---------------------------------------------------------------------- */

    _size = 0;
    _msg = msg;

  }

  /* ======================================================================== */
  /*..TipcMsg::tipcMsg -- TipcMsg constructor */
  TipcMsg::TipcMsg(const TipcMt& mt) throw (TipcException)
  {
    _size = 0;
    _msg = TipcMsgCreate((T_IPC_MT)mt);

    if (T_NULL == _msg) {
      throw TipcMsgException();
    }

  }

  /* ======================================================================== */
  /*..TipcMsg::tipcMsg -- TipcMsg constructor */
  TipcMsg::TipcMsg(int4 mt_num, 
                   const char * destination,
                   const char * sender) throw (TipcException)
  {
    T_IPC_MT mt;

    _size = 0;

    mt  = TipcMtLookupByNum(mt_num);

    if (!mt) {
      throw TipcMsgException();
    }

    _msg = TipcMsgCreate(mt);

    if (T_NULL == _msg) {
      throw TipcMsgException();
    }

    if (NULL != destination) {
      if (!TipcMsgSetDest(_msg, const_cast<char *> (destination))) {
        throw TipcMsgException();
      }
    }

    if (NULL != sender) {
      if (!TipcMsgSetSender(_msg, const_cast<char *> (sender))) {
        throw TipcMsgException();
      }
    }

  }

  /* ======================================================================== */
  /*..TipcMsg:: TipcMsg -- TipcMsg constructor */
  TipcMsg:: TipcMsg(const char * mtName) throw (TipcException)
  {
    T_IPC_MT mt;

    _size = 0;

    mt  = TipcMtLookup(const_cast<char *> (mtName));
    if (T_NULL == mt) {
      throw TipcMsgException();
    }

    _msg = TipcMsgCreate(mt);
    if (T_NULL == _msg) {
      throw TipcMsgException();
    }
  }

  // --------------------------------------------------------------
  // Copy constructor.  We avoid cloning and use ref count instead.
  // --------------------------------------------------------------

  /* ======================================================================== */
  /*..TipcMsg::tipcMsg -- TipcMsg copy constructor */
  TipcMsg::TipcMsg(const TipcMsg& msg) throw (TipcException)
  {

    /* -------------------------------- */
    /* Copy the data to the new object. */
    /* -------------------------------- */
    _size = msg._size;
    _msg = msg._msg;

    /* ---------------------------------------------------- */
    /* If non-vacant, then increment ref count of T_IPC_MSG */
    /* ---------------------------------------------------- */
    if (T_NULL != _msg) {
      T_BOOL read_only_return;

      if (!TipcMsgGetReadOnly(_msg, &read_only_return)) {
        throw TipcMsgException();
      }

      if (!read_only_return) {
        if (!TipcMsgIncrRefCount(_msg))
          throw TipcMsgException();
      }
    }
  }

  /* ======================================================================== */
  /*..TipcMsg::tipcMsg -- TipcMsg constructor */
  TipcMsg::TipcMsg(const T_IPC_MSG msg) throw (TipcException)
  {
    if (msg == NULL)
      throw TipcMsgException();

    absorb(msg);
    if (T_NULL != _msg) {
      T_BOOL read_only_return;

      if (!TipcMsgGetReadOnly(_msg, &read_only_return)) {
        throw TipcMsgException();
      }

      if (!read_only_return) {
        if (!TipcMsgIncrRefCount(_msg))
          throw TipcMsgException();
      }
    }
  }

  /* ======================================================================== */
  /*..TipcMsg::destroyMsg -- safely destroy managed T_IPC_MSG message */
  void TipcMsg::destroyMsg(void) throw (TipcException)
  {
    /* ----------------------------------------------------------------- */
    /* Destroy the T_IPC_MSG managed by the object.  TipcMsgDestroy      */
    /* manages the T_IPC_MSG reference count, therefore the actual       */
    /* T_IPC_MSG will not be destroyed until the reference count goes to */
    /* zero.                                                             */
    /* ----------------------------------------------------------------- */
    if (T_NULL != _msg) {
      T_BOOL read_only_return;

      if (!TipcMsgGetReadOnly(_msg, &read_only_return)) {
        throw TipcMsgException();
      }

      if (!read_only_return) {
        if (!TipcMsgDestroy(_msg)) {
          throw TipcMsgException();
        }
        _msg = (T_IPC_MSG)T_NULL;
      }
    }
  }

  /* ======================================================================== */
  /*..TipcMsg::~TipcMsg -- TipcMsg destructor */
  //???
  TipcMsg::~TipcMsg() throw()
  {
    destroyMsg();
  }

  // -------------------------------------------------------------
  // Assignment operators
  // -------------------------------------------------------------

  /* ======================================================================== */
  /*..TipcMsg::operator= -- TipcMsg assignment operator */
  TipcMsg& TipcMsg::operator=(const TipcMsg& msg) throw (TipcException)
  {
    if (T_NULL == msg._msg) {
      throw TipcMsgException(); 
    }

    /* ------------------------------------------------- */
    /* Destroy message currently managed by this object. */
    /* ------------------------------------------------- */
    destroyMsg();

    /* ------------------------------ */
    /* Assign the data to the object. */
    /* ------------------------------ */
    _size = msg._size;
    _msg = msg._msg;

    //Increment the reference count if the message is NOT "read only"
    if (T_NULL != _msg) {
      T_BOOL read_only_return;

      if (!TipcMsgGetReadOnly(_msg, &read_only_return)) {
        throw TipcMsgException();
      }

      if (!read_only_return) {
        if (!TipcMsgIncrRefCount(_msg))
          throw TipcMsgException();
      }
    }

    return *this;
  }

  /* ======================================================================== */
  /*..TipcMsg::operator= -- TipcMsg assignment operator */
  TipcMsg& TipcMsg::operator=(const T_IPC_MSG msg) throw (TipcException)
  {
    if (T_NULL == msg) {
      throw TipcMsgException(); 
    }

    /* ------------------------------------------------- */
    /* Destroy message currently managed by this object. */
    /* ------------------------------------------------- */
    destroyMsg();

    /* ---------------------------------- */
    /* Take over management of T_IPC_MSG. */
    /* ---------------------------------- */
    absorb(msg);

    //Increment the reference count if the message is NOT "read only"
    if (T_NULL != _msg) {
      T_BOOL read_only_return;

      if (!TipcMsgGetReadOnly(_msg, &read_only_return)) {
        throw TipcMsgException();
      }

      if (!read_only_return) {
        if (!TipcMsgIncrRefCount(_msg))
          throw TipcMsgException();
      }
    }

    return *this;

  }

  /* ======================================================================== */
  /*..TipcMsg::clone -- Clone a new message for TipcMsg object */
  TipcMsg* TipcMsg::clone()  const throw (TipcException)
  {
    T_IPC_MSG msg = TipcMsgClone(_msg);

    if (msg == NULL) {
      throw TipcMsgException();
    }

    TipcMsg* retVal = new TipcMsg(msg);

    // TipcMsg ctor increments the ref-count; because we will no longer own
    // the message, we need to decrement it here to avoid a memory leak
    if (!TipcMsgDestroy(msg))
      throw TipcMsgException();

    return retVal;

  }



  /* ======================================================================== */
  /*..TipcMsg::incrRefCount -- increment the reference count of a message */
  void TipcMsg::incrRefCount() throw (TipcException)
  {
    if (!TipcMsgIncrRefCount(_msg))
      throw TipcMsgException();
  }


  /* ======================================================================== */
  /*..TipcMsg::readOnly -- get read-only status of message */
  bool TipcMsg::isReadOnly() const throw (TipcException)
  {
    T_BOOL read_only_return = T_TRUE;  // initialize variable

    bool status = TipcMsgGetReadOnly(_msg, &read_only_return);
    if (false == status)
      throw TipcMsgException();

    return(read_only_return?true:false);
  }

  /* ======================================================================== */
  /*..TipcMsg::refCount -- get the reference count of a message */
  int2 TipcMsg::getRefCount() const throw (TipcException)
  {
    int2 ref_count_return = 0;  // initialize variable

    if (!TipcMsgGetRefCount(_msg, &ref_count_return))
      throw TipcMsgException();

    return ref_count_return;
  }


  /* ======================================================================== */
  /*..TipcMsg::dest -- get the destination property of a message */
  const char * TipcMsg::getDest() const throw (TipcException)
  {
    char * dest_return = (char *)"";  // initialize variable

    if (!TipcMsgGetDest(_msg, &dest_return))
      throw TipcMsgException();

    return const_cast<const char*> (dest_return);
  }

  /* ======================================================================== */
  /*..TipcMsg::dest -- set the destination property of a message */
  void TipcMsg::setDest(const char * dest) throw (TipcException)
  {
    if (!TipcMsgSetDest(_msg, const_cast<char *> (dest)))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::sender -- get the sender property of a message */
  const char * TipcMsg::getSender() const throw (TipcException)
  {
    char * sender_return = (char *)"";  // initialize variable

    if (!TipcMsgGetSender(_msg, &sender_return))
      throw TipcMsgException();

    return sender_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::sender -- set the sender property of a message */
  void TipcMsg::setSender(const char * sender)  throw (TipcException)
  {
    if (!TipcMsgSetSender(_msg, const_cast<char *> (sender)))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::type -- get the type of a message */


  const TipcMt TipcMsg::getType() const throw (TipcException)
  {

    T_IPC_MT retVal = (T_IPC_MT)NULL;

    if (!TipcMsgGetType(_msg, &retVal))
      throw TipcMsgException();

    return(const TipcMt) TipcMt(retVal);
  }


  int4 TipcMsg::getTypeNum() const throw (TipcException)
  {
    int4 retVal;

    if (!TipcMsgGetTypeNum(_msg, &retVal))
      throw TipcMsgException();

    return retVal;
  }


  /* ======================================================================== */
  /*..TipcMsg::type -- set the type of a message */

  void TipcMsg::setType(const TipcMt& mt) throw (TipcException)
  {
    if (!TipcMsgSetType(_msg, (T_IPC_MT)mt))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::current -- get the current field of a message */
  int4 TipcMsg::getCurrent() const throw (TipcException)
  {
    int4 field_num_return = -1;  // initialize variable

    if (!TipcMsgGetCurrent(_msg, &field_num_return))
      throw TipcMsgException();

    return field_num_return;
  }                 

  /* ======================================================================== */
  /*..TipcMsg::current -- set the current field of a message */
  void TipcMsg::setCurrent(int4 field_num) throw (TipcException)
  {
    if (!TipcMsgSetCurrent(_msg, field_num))
      throw TipcMsgException();
  } 


  /* ======================================================================== */
  /*..TipcMsg::numFields -- get the number of fields in a message */
  int4 TipcMsg::getNumFields() const throw (TipcException)
  {
    int4 num_fields_return = -1;  // initialize variable

    if (!TipcMsgGetNumFields(_msg, &num_fields_return))
      throw TipcMsgException();

    return num_fields_return;
  }                 

  /* ======================================================================== */
  /*..TipcMsg::numFields -- set the number of fields in a message */
  void TipcMsg::setNumFields(int4 num_fields) throw (TipcException)
  {
    if (!TipcMsgSetNumFields(_msg, num_fields))
      throw TipcMsgException();
  } 

  /* ======================================================================== */
  /*..TipcMsg::priority -- get the priority of a message */
  int2 TipcMsg::getPriority() const throw (TipcException)
  {
    int2 priority_return = 0;  // initialize variable

    if (!TipcMsgGetPriority(_msg, &priority_return))
      throw TipcMsgException();

    return priority_return;
  }   

  /* ======================================================================== */
  /*..TipcMsg::priority -- set the priority of a message */
  void TipcMsg::setPriority(int2 priority) throw (TipcException)
  {
    if (!TipcMsgSetPriority(_msg, priority))
      throw TipcMsgException();
  } 

  /* ======================================================================== */
  /*..TipcMsg::userProp -- get user-defined property from a message */
  int4 TipcMsg::getUserProp() const throw (TipcException)
  {
    int4 user_prop_return = 0;  // intialize variable

    if (!TipcMsgGetUserProp(_msg, &user_prop_return))
      throw TipcMsgException();

    return user_prop_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::userProp -- set user-defined property from message */
  void TipcMsg::setUserProp(int4 user_prop) throw (TipcException)
  {
    if (!TipcMsgSetUserProp(_msg, user_prop))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::getCompression -- get compression property from a message */
  bool TipcMsg::getCompression() const throw (TipcException)
  {
    T_BOOL compression = T_FALSE;

    if (!TipcMsgGetCompression(_msg, &compression))
      throw TipcMsgException();

    return compression ? true : false;
  }

  /* ======================================================================== */
  /*..TipcMsg::setCompression -- set compression property of a message */
  void TipcMsg::setCompression(bool compression) throw (TipcException)
  {
    if (!TipcMsgSetCompression(_msg, compression))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::seqNum -- get the sequence number of a message */
  int4 TipcMsg::getSeqNum() const throw (TipcException)
  {
    int4 seq_num_return = 0;  // intialize variable

    if (!TipcMsgGetSeqNum(_msg, &seq_num_return))
      throw TipcMsgException();

    return seq_num_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::ack -- acknowledge delivery of a message */
  void TipcMsg::ack() throw (TipcException)
  {
    if (!TipcMsgAck(_msg))
      throw TipcMsgException();
  }



  /* ======================================================================== */
  /*..TipcMsg::deliveryMode -- get the delivery mode of a message */
  T_IPC_DELIVERY_MODE TipcMsg::getDeliveryMode() const throw (TipcException)
  {
    T_IPC_DELIVERY_MODE delivery_mode_return = T_IPC_DELIVERY_BEST_EFFORT;

    if (!TipcMsgGetDeliveryMode(_msg, &delivery_mode_return))
      throw TipcMsgException();

    return delivery_mode_return;
  } 

  /* ======================================================================== */
  /*..TipcMsg::deliveryMode -- set the delivery mode of a message */
  void TipcMsg::setDeliveryMode(T_IPC_DELIVERY_MODE delivery_mode) throw (TipcException)
  {
    if (!TipcMsgSetDeliveryMode(_msg, delivery_mode))
      throw TipcMsgException();
  } 

  /* ======================================================================== */
  /*..TipcMsg::deliveryTimeout -- get the delivery timeout of a message */
  real8 TipcMsg::getDeliveryTimeout() const throw (TipcException)
  {
    real8 delivery_timeout_return = 0.0;

    if (!TipcMsgGetDeliveryTimeout(_msg,&delivery_timeout_return))
      throw TipcMsgException();

    return delivery_timeout_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::deliveryTimeout -- set the delivery timeout of a message */
  void TipcMsg::setDeliveryTimeout(real8 timeout) throw (TipcException)
  {
    if (!TipcMsgSetDeliveryTimeout(_msg,timeout))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::messageId -- get message id of a message */
  const char * TipcMsg::getMessageId() const throw (TipcException)
  {
    char * message_id_return = (char *)NULL;  // initialize variable

    if (!TipcMsgGetMessageId(_msg, &message_id_return))
      throw TipcMsgException();

    return const_cast<const char*>(message_id_return);
  }

  /* ======================================================================== */
  /*..TipcMsg::generateMessageId -- set message id of a message */
  void TipcMsg::generateMessageId() throw (TipcException)
  {
    if (!TipcMsgGenerateMessageId(_msg))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::correlationId -- get correlation id of a message */
  const char * TipcMsg::getCorrelationId()  const throw (TipcException)
  {
    char * correlation_id_return = (char *)NULL;  // intialize variable

    if (!TipcMsgGetCorrelationId(_msg, &correlation_id_return))
      throw TipcMsgException();

    return const_cast<const char*>(correlation_id_return);
  }

  /* ======================================================================== */
  /*..TipcMsg::correlationId -- set correlation id of a message */
  void TipcMsg::setCorrelationId(const char * correlation_id) throw (TipcException)
  {
    if (!TipcMsgSetCorrelationId(_msg, const_cast<char *> (correlation_id)))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::replyTo -- get reply to subject of a message */
  const char * TipcMsg::getReplyTo() const throw (TipcException)
  {
    char * subj_return = (char *)NULL;  // intialize variable

    if (!TipcMsgGetReplyTo(_msg, &subj_return))
      throw TipcMsgException();

    return const_cast<const char*>(subj_return);
  }

  /* ======================================================================== */
  /*..TipcMsg::replyTo -- set reply to subject of a message */
  void TipcMsg::setReplyTo(const char * subj) throw (TipcException)
  {
    if (!TipcMsgSetReplyTo(_msg, const_cast<char *> (subj)))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::expiration -- get expiration of a message */
  real8 TipcMsg::getExpiration() const throw (TipcException)
  {
    real8 expiration_return;

    if (!TipcMsgGetExpiration(_msg, &expiration_return))
      throw TipcMsgException();

    return expiration_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::expiration -- set expiration of a message */
  void TipcMsg::setExpiration(real8 expiration) throw (TipcException)
  {
    if (!TipcMsgSetExpiration(_msg, expiration))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::senderTimestamp -- get sender timestamp of a message */
  real8 TipcMsg::getSenderTimestamp() const throw (TipcException)
  {
    real8 sender_timestamp_return;

    if (!TipcMsgGetSenderTimestamp(_msg, &sender_timestamp_return))
      throw TipcMsgException();

    return sender_timestamp_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::senderTimestamp -- set sender timestamp of a message */
  void TipcMsg::setSenderTimestamp(real8 sender_timestamp) throw (TipcException)
  {
    if (!TipcMsgSetSenderTimestamp(_msg, sender_timestamp))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::arrivalTimestamp -- get arrival timestamp of a message */
  real8 TipcMsg::getArrivalTimestamp() const throw (TipcException)
  {
    real8 arrival_timestamp_return;

    if (!TipcMsgGetArrivalTimestamp(_msg, &arrival_timestamp_return))
      throw TipcMsgException();

    return arrival_timestamp_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::arrivalTimestamp -- set arrival timestamp of a message */
  void TipcMsg::setArrivalTimestamp(real8 arrival_timestamp) throw (TipcException)
  {
    if (!TipcMsgSetArrivalTimestamp(_msg, arrival_timestamp))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::packetSize -- get the packet size of a message */
  uint4 TipcMsg::getPacketSize() const throw (TipcException)
  {
    uint4 packet_size_return = 0;  // initialize variable

    if (!TipcMsgGetPacketSize(_msg, &packet_size_return))
      throw TipcMsgException();

    return packet_size_return;
  }


  /* ======================================================================== */
  /*..TipcMsg::deliveryModeToStr -- convert a delivery mode to a string */
  const char* TipcMsg::deliveryModeToStr(T_IPC_DELIVERY_MODE delivery_mode) 
  throw (TipcException)
  {
    char* deliveryModeReturn = (char*) NULL;

    bool ret = TipcDeliveryModeToStr(delivery_mode, (&deliveryModeReturn));
    if (false == ret)
      throw TipcMsgException();

    return const_cast<const char*>(deliveryModeReturn);
  }

  /* ======================================================================== */
  /*..TipcMsg::strToDeliveryMode -- convert a string to a delivery mode */
  T_IPC_DELIVERY_MODE TipcMsg::strToDeliveryMode(const char * delivery_str) 
  throw (TipcException)
  {
    T_IPC_DELIVERY_MODE delivery_mode_return = T_IPC_DELIVERY_BEST_EFFORT;
    if (!TipcStrToDeliveryMode(const_cast<char *> (delivery_str), &delivery_mode_return))
      throw TipcMsgException();

    return delivery_mode_return;

  }

  /* ======================================================================== */
  /*..TipcMsg::ftToStr -- convert a field type to a string */
  const char* TipcMsg::ftToStr(T_IPC_FT type)  throw (TipcException)
  {
    char* strReturn = (char*)NULL;
    if (!TipcFtToStr(type, (&strReturn)))
      throw TipcMsgException();

    return const_cast<const char*>(strReturn);
  }

  /* ======================================================================== */
  /*..TipcMsg::strToFt -- convert a string to a field type */
  T_IPC_FT TipcMsg::strToFt(const char * str)   throw (TipcException)
  {
    T_IPC_FT typeReturn = T_IPC_FT_INT2;
    if (!TipcStrToFt(const_cast<char *> (str), &typeReturn))
      throw TipcMsgException();

    return typeReturn;
  }

  /* ======================================================================== */
  /*..TipcMsg::fieldSize -- set a field size */
  void TipcMsg::setFieldSize(T_IPC_MSG_FIELD field, int4 size) throw (TipcException)
  {
    if (!TipcMsgFieldSetSize(field, size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*.. TipcMsg::lbModeToStr -- convert load balancing mode to string */
  const char* TipcMsg::lbModeToStr(T_IPC_LB_MODE lb_mode)  throw (TipcException)
  {
    char *strReturn = (char*)NULL;
    if (!TipcLbModeToStr(lb_mode, (&strReturn)))
      throw TipcMsgException();

    return const_cast<const char*>(strReturn);
  } 

  /* ======================================================================== */
  /*.. TipcMsg::strToLbMode -- convert a string to a load balancing mode */


  T_IPC_LB_MODE TipcMsg::strToLbMode(const char * str)   throw (TipcException)
  {
    T_IPC_LB_MODE retVal = T_IPC_LB_NONE;

    if (!TipcStrToLbMode(const_cast<char *> (str), &retVal))
      throw TipcMsgException();

    return retVal;
  }


  /* ------------------------------------------------- */
  /* Insertion                                         */
  /* ------------------------------------------------- */

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- boolean array data insertion operator */
  TipcMsg& TipcMsg::operator<<(const bool *  arg) throw (TipcException)
  {
    if (_size < 0)
      throw TipcMsgException();

    T_BOOL* boolArray = new T_BOOL[_size];

    if (boolArray) {
      for (int i = 0; i < _size; i++) {
        boolArray[i] = arg[i];
      }
    }

    if (!TipcMsgAppendBoolArray(_msg, boolArray, _size)) {
      delete [] boolArray;
      throw TipcMsgException();
    }

    delete [] boolArray;

    return *this;
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- boolean data insertion operator */
  TipcMsg& TipcMsg::operator<<(bool arg) throw (TipcException)
  {

    if (!TipcMsgAppendBool(_msg, (T_BOOL)arg))
      throw TipcMsgException();

    return *this;
  }


  /* ======================================================================== */
  /*..TipcMsg::operator<< -- binary data insertion operator */
  TipcMsg& TipcMsg::operator<<(const void *  arg) throw (TipcException)
  {
    if (_size < 0)
      throw TipcMsgException();


    if (!TipcMsgAppendBinary(_msg, const_cast <void*> (arg), _size))
      throw TipcMsgException();

    return *this;
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- T_CHAR insertion operator */
  TipcMsg& TipcMsg::operator<<(char arg) throw (TipcException)
  {
    if (!TipcMsgAppendChar(_msg, arg))
      throw TipcMsgException();

    return *this;
  }
  /* ======================================================================== */
  /*..TipcMsg::operator<< -- int2 insertion operator */
  TipcMsg& TipcMsg::operator<<(int2 arg) throw (TipcException)
  {
    if (!TipcMsgAppendInt2(_msg, arg))
      throw TipcMsgException();

    return *this;
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- int2 array insertion operator */
  TipcMsg& TipcMsg::operator<<(const int2* arg) throw (TipcException)
  {
    if (_size < 0)
      throw TipcMsgException();

    if (!TipcMsgAppendInt2Array(_msg, const_cast <int2*>(arg), _size))
      throw TipcMsgException();

    return *this;
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- int4 insertion operator */
  TipcMsg& TipcMsg::operator<<(int4 arg) throw (TipcException)
  {
    if (!TipcMsgAppendInt4(_msg, arg))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- int4 array insertion operator */
  TipcMsg& TipcMsg::operator<<(const int4* arg) throw (TipcException)
  {
    if (_size < 0)
      throw TipcMsgException();

    if (!TipcMsgAppendInt4Array(_msg, const_cast<int4*>(arg), _size))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- int8 insertion operator */
  TipcMsg& TipcMsg::operator<<(int8 arg) throw (TipcException)
  {
    if (!TipcMsgAppendInt8(_msg, arg))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- int8 array insertion operator */
  TipcMsg& TipcMsg::operator<<(const int8* arg) throw (TipcException)
  {
    if (_size < 0)
      throw TipcMsgException();

    if (!TipcMsgAppendInt8Array(_msg,const_cast<int8*>(arg), _size))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- TipcMsg object insertion operator */
  TipcMsg& TipcMsg::operator<<(const TipcMsg& arg) throw (TipcException)
  {
    if (!TipcMsgAppendMsg(_msg, arg._msg))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- TipcMsg object array insertion operator */
  TipcMsg& TipcMsg::operator<<(const TipcMsg* arg) throw (TipcException)
  {
    if (_size < 0)
      throw TipcMsgException();

    if (!arg) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMsgException();
    }

    T_IPC_MSG *array = (T_IPC_MSG*) NULL;

    // Windows has a bug where if a zero byte array is created, the OS will crash if it's deleted.
#ifndef T_OS_WINDOWS
    array = new T_IPC_MSG[_size];
#else
    if (_size) {
      array = new T_IPC_MSG[_size];
    }
    else {
      array = new T_IPC_MSG[1];
    }
#endif

    if (!array) {
      TutErrNumSet(T_ERR_NOMEM);
      throw TipcMsgException();
    }

    for (int4 i = 0; i < _size; i++) {
      array[i] = (T_IPC_MSG)arg[i];
    }

    if (!TipcMsgAppendMsgArray(_msg, array, _size)) {
      delete [] array;
      throw TipcMsgException();
    }

    delete [] array;

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- real4 insertion operator */
  TipcMsg& TipcMsg::operator<<(real4 arg) throw (TipcException)
  {
    if (!TipcMsgAppendReal4(_msg, arg))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- real4 array insertion operator */
  TipcMsg& TipcMsg::operator<<(const real4* arg) throw (TipcException)
  {
    if (_size < 0)
      throw TipcMsgException();

    if (!TipcMsgAppendReal4Array(_msg, const_cast<real4*>(arg), _size))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- real8 insertion operator */
  TipcMsg& TipcMsg::operator<<(real8 arg) throw (TipcException)
  {
    if (!TipcMsgAppendReal8(_msg, arg))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- real8 array insertion operator */
  TipcMsg& TipcMsg::operator<<(const real8* arg) throw (TipcException)
  {
    if (_size < 0)
      throw TipcMsgException();

    if (!TipcMsgAppendReal8Array(_msg, const_cast<real8*>(arg), _size))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- real16 insertion operator */
  TipcMsg& TipcMsg::operator<<(real16 arg) throw (TipcException)
  {
    if (!TipcMsgAppendReal16(_msg, arg))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- real16 array insertion operator */
  TipcMsg& TipcMsg::operator<<(const real16* arg) throw (TipcException)
  {
    if (_size < 0)
      throw TipcMsgException();

    if (!TipcMsgAppendReal16Array(_msg, const_cast<real16*>(arg), _size))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- const char * insertion operator */
  TipcMsg& TipcMsg::operator<<(const char * arg) throw (TipcException)
  {
    if (!TipcMsgAppendStr(_msg, const_cast<char *> (arg)))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- const char * array insertion operator */
  TipcMsg& TipcMsg::operator<<(const char ** arg) throw (TipcException)
  {
    if (_size < 0)
      throw TipcMsgException();


    if (!TipcMsgAppendStrArray(_msg, const_cast<char **> (arg), _size))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator<< -- T_XML insertion operator */
  TipcMsg& TipcMsg::operator<<(const XML& arg) throw (TipcException)
  {
    if (!TipcMsgAppendXml(_msg, (T_XML)arg))
      throw TipcMsgException();

    return *this;           
  }



  /* ======================================================================== */
  /*..TipcMsg::operator<< -- byte insertion operator */
  TipcMsg& TipcMsg::operator<<(unsigned char arg) throw (TipcException)
  {
    if (!TipcMsgAppendByte(_msg, arg))
      throw TipcMsgException();

    return *this;           
  }


  /* ------------------------------------------------- */
  /* Extraction                                        */
  /* ------------------------------------------------- */


  /*..TipcMsg::operator>> -- binary data extraction operator */
  const TipcMsg& TipcMsg::operator>>(bool * & arg)  const  throw (TipcException)
  {
    T_BOOL* tempData = NULL;
    if (!TipcMsgNextBoolArray(_msg, (&tempData), const_cast<int4*>(&_size)))
      throw TipcMsgException();

    arg = new bool[_size];

    for (int i = 0; i < _size; i++) {
      arg[i] = (bool)tempData[i];
    }

    return *this;
  }

  /*..TipcMsg::operator>> -- binary data extraction operator */
  const TipcMsg& TipcMsg::operator>>(bool & arg)  const  throw (TipcException)
  {
    T_BOOL val = T_FALSE;

    if (!TipcMsgNextBool(_msg, &val))
      throw TipcMsgException();

    arg = val?true:false;

    return *this;
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- binary data extraction operator */
  const TipcMsg& TipcMsg::operator>>(const void * & arg)  const  throw (TipcException)
  {
    if (!TipcMsgNextBinary(_msg, const_cast <void**>(&arg), const_cast<int4*>(&_size)))
      throw TipcMsgException();

    return *this;
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- T_CHAR extraction operator */
  const TipcMsg& TipcMsg::operator>>(char& arg)  const throw (TipcException)
  {
    if (!TipcMsgNextChar(_msg, &arg))
      throw TipcMsgException();

    return *this;
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- int2 extraction operator */
  const TipcMsg& TipcMsg::operator>>(int2& arg)  const throw (TipcException)
  {
    if (!TipcMsgNextInt2(_msg, &arg))
      throw TipcMsgException();

    return *this;
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- int2 array extraction operator */
  const TipcMsg& TipcMsg::operator>>(const int2*& arg)  const throw (TipcException)
  {
    if (!TipcMsgNextInt2Array(_msg, const_cast<int2**>(&arg), const_cast<int4*>(&_size)))
      throw TipcMsgException();

    return *this;
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- int4 extraction operator */
  const TipcMsg& TipcMsg::operator>>(int4& arg)  const throw (TipcException)
  {
    if (!TipcMsgNextInt4(_msg, &arg))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- int4 array extraction operator */
  const TipcMsg& TipcMsg::operator>>(const int4*& arg) const throw (TipcException)
  {
    if (!TipcMsgNextInt4Array(_msg, const_cast<int4**>(&arg), const_cast<int4*>(&_size)))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- int8 extraction operator */
  const TipcMsg& TipcMsg::operator>>(int8& arg)  const throw (TipcException)
  {
    if (!TipcMsgNextInt8(_msg, &arg))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- int8 array extraction operator */
  const TipcMsg& TipcMsg::operator>>(const int8*& arg)  const throw (TipcException)
  {
    if (!TipcMsgNextInt8Array(_msg, const_cast<int8**>(&arg), const_cast<int4*>(&_size)))
      throw TipcMsgException();


    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- TipcMsg object extraction operator */
  const TipcMsg& TipcMsg::operator>>(TipcMsg& arg)  const throw (TipcException)
  {
    T_IPC_MSG temp;

    if (!TipcMsgNextMsg(_msg, &temp))
      throw TipcMsgException();

    arg = temp;
    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- TipcMsg object array extraction operator */
  const TipcMsg& TipcMsg::operator>>(TipcMsg*& arg) const  throw (TipcException)
  {
    T_IPC_MSG *c_api_msg_array;

    if (!TipcMsgNextMsgArray(_msg, &c_api_msg_array, const_cast<int4*>(&_size)))
      throw TipcMsgException();



#ifndef T_OS_WINDOWS
    arg = new TipcMsg[_size];
#else
    if (_size) {
      arg = new TipcMsg[_size];
    }
    else {
      arg = new TipcMsg[1];
    }
#endif

    if (!arg) {
      TutErrNumSet(T_ERR_NOMEM);
      throw TipcMsgException();
    }

    for (int4 i = 0; i < _size; i++) {
      /* ------------------------------------------------------ */
      /* Absorb the T_IPC_MSG array. Since element of a message */
      /* array are read-only, we do not increment the reference */
      /* count of the T_IPC_MSG array elements.                 */
      /* ------------------------------------------------------ */
      arg[i].destroyMsg();
      arg[i].absorb(c_api_msg_array[i]);
    }

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- real4 extraction operator */
  const TipcMsg& TipcMsg::operator>>(real4& arg)  const throw (TipcException)
  {
    if (!TipcMsgNextReal4(_msg, &arg))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- real4 array extraction operator */
  const TipcMsg& TipcMsg::operator>>(const real4*& arg)  const throw (TipcException)
  {
    if (!TipcMsgNextReal4Array(_msg, const_cast<real4**>(&arg), const_cast<int4*>(&_size)))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- real8 extraction operator */
  const TipcMsg& TipcMsg::operator>>(real8& arg)  const throw (TipcException)
  {
    if (!TipcMsgNextReal8(_msg, &arg))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- real8 array extraction operator */
  const TipcMsg& TipcMsg::operator>>(const real8*& arg)  const throw (TipcException)
  {
    if (!TipcMsgNextReal8Array(_msg, const_cast<real8**>(&arg), const_cast<int4*>(&_size)))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- real16 extraction operator */
  const TipcMsg& TipcMsg::operator>>(real16& arg) const  throw (TipcException)
  {
    if (!TipcMsgNextReal16(_msg, &arg))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- real16 array extraction operator */
  const TipcMsg& TipcMsg::operator>>(const real16*& arg) const  throw (TipcException)
  {
    if (!TipcMsgNextReal16Array(_msg, const_cast<real16**>(&arg), const_cast<int4*>(&_size)))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- const char * extraction operator */
  const TipcMsg& TipcMsg::operator>>(const char *& arg)  const throw (TipcException)
  {
    if (!TipcMsgNextStr(_msg, const_cast<char **>(&arg)))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- const char * array extraction operator */
  const TipcMsg& TipcMsg::operator>>(const char **& arg) const  throw (TipcException)
  {
    if (!TipcMsgNextStrArray(_msg, const_cast<char ***>(&arg), const_cast<int4*>(&_size)))
      throw TipcMsgException();

    return *this;           
  }

  /* ======================================================================== */
  /*..TipcMsg::operator>> -- T_XML extraction operator */
  const TipcMsg& TipcMsg::operator>>(XML& arg) const  throw (TipcException)
  {
    T_XML retVal = (T_XML) NULL;

    if (!TipcMsgNextXml(_msg, &retVal))
      throw TipcMsgException();

    arg = retVal;

    return *this;           
  }



  /* ======================================================================== */
  /*..TipcMsg::operator>> -- byte extraction operator */
  const TipcMsg& TipcMsg::operator>>(unsigned char& arg) const  throw (TipcException)
  {
    if (!TipcMsgNextByte(_msg, &arg))
      throw TipcMsgException();

    return *this;           
  }

  /* ------------------------------------------------------ */
  /* Append functions which provide the C API look-and-feel */
  /* ------------------------------------------------------ */

  /* ======================================================================== */
  /*..TipcMsg::append -- append a binary field to a message */
  void TipcMsg::append(const void *  data, 
                       int4 binary_size) throw (TipcException)
  {
    if (!TipcMsgAppendBinary(_msg, const_cast <void*> (data) , binary_size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a T_CHAR field to a message */
  void TipcMsg::append(char char_data) throw (TipcException)
  {
    if (!TipcMsgAppendChar(_msg, char_data))
      throw TipcMsgException();
  }
  /* ======================================================================== */
  /*..TipcMsg::append -- append a int2 field to a message */
  void TipcMsg::append(int2 int2_data) throw (TipcException)
  {
    if (!TipcMsgAppendInt2(_msg, int2_data))
      throw TipcMsgException();

  }


  /* ======================================================================== */
  /*..TipcMsg::append -- append a int2 array field to a message */
  void TipcMsg::append(const int2* data, 
                       int4 int2_array_size) throw (TipcException)
  {
    if (!TipcMsgAppendInt2Array(_msg, const_cast<int2*>(data), int2_array_size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a int4 field to a message */
  void TipcMsg::append(int4 int4_data) throw (TipcException)
  {
    if (!TipcMsgAppendInt4(_msg, int4_data))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a int4 array field to a message */
  void TipcMsg::append(const int4* data, 
                       int4 int4_array_size) throw (TipcException)
  {
    if (!TipcMsgAppendInt4Array(_msg, const_cast<int4*>(data), int4_array_size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::append -- append a int8 field to a message */
  void TipcMsg::append(int8 int8_data) throw (TipcException)
  {
    if (!TipcMsgAppendInt8(_msg, int8_data))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::append -- append a int8 array field to a message */
  void TipcMsg::append(const int8 *data, 
                       int4 int8_array_size) throw (TipcException)
  {
    if (!TipcMsgAppendInt8Array(_msg, const_cast<int8*>(data), int8_array_size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a message contained in TipcMsg object */
  void TipcMsg::append(const TipcMsg& msg_data) throw (TipcException)
  {
    if (!TipcMsgAppendMsg(_msg, msg_data._msg))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a message array field to a message */
  void TipcMsg::append(const TipcMsg* arg, 
                       int4 size) throw (TipcException)
  {
    if (size < 0)
      throw TipcMsgException();

    if (arg == NULL) {
      TutErrNumSet (T_ERR_NULL_PTR);
      throw TipcMsgException();
    }

    T_IPC_MSG *array = (T_IPC_MSG*) NULL;

    // Windows has a bug where if a zero byte array is created, the OS will crash if it's deleted.
#ifndef T_OS_WINDOWS
    array = new T_IPC_MSG[size];
#else
    if (size) {
      array = new T_IPC_MSG[size];
    }
    else {
      array = new T_IPC_MSG[1];
    }
#endif

    if (!array) {
      TutErrNumSet(T_ERR_NOMEM);
      throw TipcMsgException();
    }

    for (int4 i = 0; i < size; i++) {
      array[i] = (T_IPC_MSG)arg[i];
    }

    if (!TipcMsgAppendMsgArray(_msg, array, size)) {
      delete [] array;
      throw TipcMsgException();
    }

    delete [] array;


  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a real4 field to a message */
  void TipcMsg::append(real4 real4_data) throw (TipcException)
  {
    if (!TipcMsgAppendReal4(_msg, real4_data))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a real4 array field to a message */
  void TipcMsg::append(const real4* data, 
                       int4 real4_array_size) throw (TipcException)
  {
    if (!TipcMsgAppendReal4Array(_msg, 
                                 const_cast<real4*>(data),
                                 real4_array_size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a real8 field to message */
  void TipcMsg::append(real8 real8_data) throw (TipcException)
  {
    if (!TipcMsgAppendReal8(_msg, real8_data))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a real8 array field to a message */
  void TipcMsg::append(const real8* data, 
                       int4 real8_array_size) throw (TipcException)
  {
    if (!TipcMsgAppendReal8Array(_msg, 
                                 const_cast<real8*>(data),
                                 real8_array_size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::append -- append a real16 field to a message */
  void TipcMsg::append(real16 real16_data) throw (TipcException)
  {
    if (!TipcMsgAppendReal16(_msg, real16_data))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*.. TipcMsg::append -- append a real16 array field to a message */
  void TipcMsg::append(const real16 *data, 
                       int4 real16_array_size) throw (TipcException)
  {
    if (!TipcMsgAppendReal16Array(_msg, const_cast<real16*>(data), 
                                  real16_array_size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a const char * field to a message */
  void TipcMsg::append(const char * str_data) throw (TipcException)
  {
    if (!TipcMsgAppendStr(_msg, const_cast<char *> (str_data)))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a const char * array field to a message */
  void TipcMsg::append(const char ** str_array_data, 
                       int4 str_array_size) throw (TipcException)
  {
    if (!TipcMsgAppendStrArray(_msg, const_cast<char **> (str_array_data), 
                               str_array_size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a const char * field and a real8 field to a msg */
  void TipcMsg::append(const char * str_data, 
                       real8 real8_data) throw (TipcException)
  {
    if (!TipcMsgAppendStrReal8(_msg, const_cast<char *> (str_data), real8_data))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a T_XML field to a message */
  void TipcMsg::append(const XML& xml_data) throw (TipcException)
  {

    if (!TipcMsgAppendXml(_msg, (T_XML)xml_data))
      throw TipcMsgException();
  }



  /* ======================================================================== */
  /*..TipcMsg::append -- append a byte field to a message */
  void TipcMsg::append(unsigned char byte_data) throw (TipcException)
  {
    if (!TipcMsgAppendByte(_msg, byte_data))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a bool field to a message */
  void TipcMsg::append(bool bool_data) throw (TipcException)
  {
    if (!TipcMsgAppendBool(_msg, bool_data))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::append -- append a bool array field to a message */
  void TipcMsg::append(const bool* bool_array_data, 
                       int4 bool_array_size) throw (TipcException)
  {
    T_BOOL* tempData = new T_BOOL[bool_array_size];

    if (tempData) {
      for (int i = 0; i < bool_array_size; i++) {
        tempData[i] = bool_array_data[i];
      }
    }
    if (!TipcMsgAppendBoolArray(_msg, tempData, bool_array_size)) {
      delete [] tempData;
      throw TipcMsgException();
    }

    delete [] tempData;

  }

  /* ======================================================================== */
  /*..TipcMsg::appendTimestamp -- append a timestamp field to a message */
  void TipcMsg::appendTimestamp(real8 timestamp_data) throw (TipcException)
  {
    if (!TipcMsgAppendTimestamp(_msg, timestamp_data))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::appendTimestampArray -- append a timestamp array field to a message */
  void TipcMsg::appendTimestampArray(const real8 *timestamp_array_data,
                                     int4 timestamp_array_size)  throw (TipcException)
  {
    if (!TipcMsgAppendTimestampArray(_msg, 
                                     const_cast<real8*>(timestamp_array_data),
                                     timestamp_array_size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::appendUtf8 -- append a UTF-8 string field to a message */
  void TipcMsg::appendUtf8(const char * str_data) throw (TipcException)
  {
    if (!TipcMsgAppendUtf8(_msg, const_cast<char *> (str_data)))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::appendUtf8 -- append a UTF-8 string array field to a message */
  void TipcMsg::appendUtf8(const char ** str_array_data, 
                           int4 str_array_size) throw (TipcException)
  {
    if (!TipcMsgAppendUtf8Array(_msg, const_cast<char **> (str_array_data), 
                                str_array_size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append a binary pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const void *  data, 
                                     int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendBinaryPtr(_msg, const_cast<void*>(data), size, &field_return))
      throw TipcMsgException();
    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append a int2 array pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const int2 *data, 
                                     int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendInt2ArrayPtr(_msg, const_cast<int2*>(data), size,&field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append a int4 array pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const int4 *data, 
                                     int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendInt4ArrayPtr(_msg, const_cast<int4*>(data), size, &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append a int8 array pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const int8 *data, 
                                     int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendInt8ArrayPtr(_msg, const_cast<int8*>(data), size,&field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append a msg pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const TipcMsg& msg_data) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendMsgPtr(_msg, (T_IPC_MSG)msg_data,&field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append a msg array pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const TipcMsg *arg, 
                                     int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (size < 0)
      throw TipcMsgException();

    if (!arg) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMsgException();
    }

    T_IPC_MSG *array = (T_IPC_MSG*) NULL;

    // Windows has a bug where if a zero byte array is created, the OS will crash if it's deleted.
#ifndef T_OS_WINDOWS
    array = new T_IPC_MSG[size];
#else
    if (size) {
      array = new T_IPC_MSG[size];
    }
    else {
      array = new T_IPC_MSG[1];
    }
#endif

    if (!array) {
      TutErrNumSet(T_ERR_NOMEM);
      throw TipcMsgException();
    }

    for (int4 i = 0; i < size; i++) {
      array[i] = (T_IPC_MSG)arg[i];
    }


    if (!TipcMsgAppendMsgArrayPtr(_msg, array, size,&field_return)) {
      delete [] array;
      throw TipcMsgException();
    }

    delete [] array;

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append a real4 array pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const real4 *data, 
                                     int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendReal4ArrayPtr(_msg, const_cast<real4*>(data), size, 
                                    &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append a real8 array pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const real8 *data, 
                                     int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendReal8ArrayPtr(_msg, const_cast<real8*>(data), size, 
                                    &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append a real16 array pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const real16 *data, 
                                     int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendReal16ArrayPtr(_msg, const_cast<real16*>(data), size, 
                                     &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append a string pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const char * str_data) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendStrPtr(_msg, const_cast<char *> (str_data), &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append a str array pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const char * *str_array_data, 
                                     int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendStrArrayPtr(_msg, const_cast<char **> (str_array_data), 
                                  size, &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendPtr -- append an xml pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(const XML& xml_data) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendXmlPtr(_msg, xml_data, &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendBoolPtr -- append a bool array pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendPtr(bool *bool_array_data, 
                                     int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    T_BOOL* tempData = new T_BOOL[size];

    if (tempData) {
      for (int i = 0; i < size; i++) {
        tempData[i] = bool_array_data[i];
      }
    }

    if (!TipcMsgAppendBoolArrayPtr(_msg, tempData, size,  &field_return)) {
      delete [] tempData;
      throw TipcMsgException();
    }

    delete [] tempData;
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::appendTimestampArrayPtr -- append a timestamp array ptr */
  T_IPC_MSG_FIELD TipcMsg::appendTimestampArrayPtr(const real8 *data,
                                                   int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendTimestampArrayPtr(_msg, const_cast<real8*>(data), size, 
                                        &field_return))
      throw TipcMsgException();

    return field_return;

  }

  /* ======================================================================== */
  /*.. TipcMsg::appendUtf8Ptr -- append a UTF-8 pointer field to a message */
  T_IPC_MSG_FIELD TipcMsg::appendUtf8Ptr(const char * str_data) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendUtf8Ptr(_msg, const_cast<char *> (str_data), &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::appendUtf8Ptr -- append a UTF-8 array pointer field to a message*/
  T_IPC_MSG_FIELD TipcMsg::appendUtf8Ptr(const char * *str_array_data, 
                                         int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAppendUtf8ArrayPtr(_msg, const_cast<char **> (str_array_data), 
                                   size, &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update a binary pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const void *  bin_data, 
                          int4 size) throw (TipcException)
  {
    if (!TipcMsgFieldUpdateBinaryPtr(field,const_cast<void*>(bin_data), size))
      throw TipcMsgException();

  }


  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update a int2 array pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const int2 *int2_array_data, 
                          int4 size) throw (TipcException)
  {
    if (!TipcMsgFieldUpdateInt2ArrayPtr(field, const_cast<int2*>(int2_array_data), size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update a int4 array pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const int4 *int4_array_data, 
                          int4 size) throw (TipcException)
  {
    if (!TipcMsgFieldUpdateInt4ArrayPtr(field, const_cast<int4*>(int4_array_data), size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update a int8 array pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const int8 *int8_array_data, 
                          int4 size) throw (TipcException)
  {
    if (!TipcMsgFieldUpdateInt8ArrayPtr(field, const_cast<int8*>(int8_array_data), size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update a msg pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const TipcMsg& msg_data)  throw (TipcException)
  {
    if (!TipcMsgFieldUpdateMsgPtr(field, (T_IPC_MSG)msg_data))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update a msg array pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const TipcMsg* arg, 
                          int4 size) throw (TipcException)
  {
    if (size < 0)
      throw TipcMsgException();

    if (!arg) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMsgException();
    }

    T_IPC_MSG *array = (T_IPC_MSG*) NULL;

    // Windows has a bug where if a zero byte array is created, the OS will crash if it's deleted.
#ifndef T_OS_WINDOWS
    array = new T_IPC_MSG[size];
#else
    if (size) {
      array = new T_IPC_MSG[size];
    }
    else {
      array = new T_IPC_MSG[1];
    }
#endif

    if (!array) {
      TutErrNumSet(T_ERR_NOMEM);
      throw TipcMsgException();
    }

    for (int4 i = 0; i < size; i++) {
      array[i] = (T_IPC_MSG)arg[i];
    }
    if (!TipcMsgFieldUpdateMsgArrayPtr(field, array, size)) {
      delete [] array;
      throw TipcMsgException();
    }

    delete [] array;

  }

  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update a real4 array pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const real4 *real4_array_data, 
                          int4 size) throw (TipcException)
  {
    if (!TipcMsgFieldUpdateReal4ArrayPtr(field, const_cast<real4*>(real4_array_data), size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update a real8 array pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const real8 *real8_array_data, 
                          int4 size) throw (TipcException)
  {
    if (!TipcMsgFieldUpdateReal8ArrayPtr(field, const_cast<real8*>(real8_array_data), size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update a real16 array pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const real16 *real16_array_data, 
                          int4 size) throw (TipcException)
  {
    if (!TipcMsgFieldUpdateReal16ArrayPtr(field, const_cast<real16*>(real16_array_data), size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update a string pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const char * str_data)  throw (TipcException)
  {
    if (!TipcMsgFieldUpdateStrPtr(field, const_cast<char *> (str_data)))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update a str array pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const char * *str_array_data, 
                          int4 size) throw (TipcException)
  {
    if (!TipcMsgFieldUpdateStrArrayPtr(field, 
                                       const_cast<char **> (str_array_data), size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::updatePtr -- update an xml pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const XML& xml_data)  throw (TipcException)
  {
    if (!TipcMsgFieldUpdateXmlPtr(field, xml_data))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::updateBoolPtr -- update a bool array pointer field in a message */
  void TipcMsg::updatePtr(T_IPC_MSG_FIELD field, 
                          const bool *bool_array_data, 
                          int4 size) throw (TipcException)
  {
    T_BOOL* tempData = new T_BOOL[size];

    if (tempData) {
      for (int i = 0; i < size; i++) {
        tempData[i] = bool_array_data[i];
      }
    }

    if (!TipcMsgFieldUpdateBoolArrayPtr(field, tempData, size)) {
      delete [] tempData;
      throw TipcMsgException();
    }

    delete [] tempData;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateTimestampArrayPtr -- update a timestamp array pointer field in a message */
  void TipcMsg::updateTimestampArrayPtr(T_IPC_MSG_FIELD field, 
                                        const real8 *timestamp_array_data, 
                                        int4 size) throw (TipcException)
  {
    if (!TipcMsgFieldUpdateTimestampArrayPtr(field, 
                                             const_cast<real8*>(timestamp_array_data), 
                                             size))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::updateUtf8Ptr -- update a UTF-8 pointer field in a message */
  void TipcMsg::updateUtf8Ptr(T_IPC_MSG_FIELD field, 
                              const char * str_data)  throw (TipcException)
  {
    if (!TipcMsgFieldUpdateUtf8Ptr(field, const_cast<char *> (str_data)))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*.. TipcMsg::updateUtf8Ptr -- update a UTF-8 array pointer field in a message*/
  void TipcMsg::updateUtf8Ptr(T_IPC_MSG_FIELD field, 
                              const char * *str_array_data, 
                              int4 size) throw (TipcException)
  {
    if (!TipcMsgFieldUpdateUtf8ArrayPtr(field, 
                                        const_cast<char **> (str_array_data), size))
      throw TipcMsgException();

  }

  /* Append Unknown SMV 09/25/98 */
  /* ======================================================================== */
  /*..TipcMsg::appendUnknown -- append a T_IPC_FT field to a message */
  void TipcMsg::appendUnknown(T_IPC_FT field_type) throw (TipcException)
  {
    if (!TipcMsgAppendUnknown(_msg, field_type))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::currentFieldKnown -- check if the current field of msg is known */
  bool TipcMsg::isCurrentFieldKnown() const throw (TipcException)
  {
    T_BOOL isKnown = FALSE;
    if (!TipcMsgGetCurrentFieldKnown(_msg, &isKnown))
      throw TipcMsgException();

    return isKnown?true:false;
  }

  /* ---------------------------------------------------- */
  /* Next functions which provide the C API look-and-feel */
  /* ---------------------------------------------------- */

  /* ======================================================================== */
  /*..TipcMsg::next -- get a binary field from a message */
  const void* TipcMsg::nextBinary(int4& size) const  throw (TipcException)
  {
    void* data = (void*) T_NULL;
    if (!TipcMsgNextBinary(_msg, &data, &size))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a T_CHAR field from a message */
  char TipcMsg::nextChar() const  throw (TipcException)
  {
    char data;
    if (!TipcMsgNextChar(_msg, &data))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a int2 field from a message */
  int2 TipcMsg::nextInt2() const  throw (TipcException)
  {
    int2 data;

    if (!TipcMsgNextInt2(_msg, &data))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a int2 array field from a message */
  const int2* TipcMsg::nextInt2Array(int4& size) const  throw (TipcException)
  {
    int2* data = (int2*) NULL;
    if (!TipcMsgNextInt2Array(_msg, &data, &size))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a int4 field from a message */
  int4 TipcMsg::nextInt4() const  throw (TipcException)
  {
    int4 data;
    if (!TipcMsgNextInt4(_msg, &data))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a int4 array field from a message */
  const int4* TipcMsg::nextInt4Array(int4& size) const  throw (TipcException)
  {
    int4* data = (int4*)NULL;
    if (!TipcMsgNextInt4Array(_msg, &data, &size))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a int8 field from a message */
  int8 TipcMsg::nextInt8() const  throw (TipcException)
  {
    int8 data;
    if (!TipcMsgNextInt8(_msg, &data))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a int8 array field from a message */
  const int8* TipcMsg::nextInt8Array(int4& size) const  throw (TipcException)
  {
    int8* data = (int8*) NULL;

    if (!TipcMsgNextInt8Array(_msg,&data,&size))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a message and assign it to TipcMsg object */
  const TipcMsg TipcMsg::nextMsg() const  throw (TipcException)
  {
    T_IPC_MSG temp = NULL;

    if (!TipcMsgNextMsg(_msg, &temp))
      throw TipcMsgException();

    return TipcMsg(temp);             
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a message array field from a message */
  TipcMsg* TipcMsg::nextMsgArray(int4& size) const  throw (TipcException)
  {
    T_IPC_MSG *array = (T_IPC_MSG*) NULL;

    TipcMsg* retVal = (TipcMsg*)NULL;

    if (!TipcMsgNextMsgArray(_msg, &array, &size))
      throw TipcMsgException();

    // Windows has a bug where if a zero byte array is created, the OS will crash if it's deleted.
#ifndef T_OS_WINDOWS
    retVal = new TipcMsg[size];
#else
    if (size) {
      retVal = new TipcMsg[size];
    }
    else {
      retVal = new TipcMsg[1];
    }
#endif

    if (!retVal) {
      TutErrNumSet(T_ERR_NOMEM);
      throw TipcMsgException();
    }


    for (int4 i = 0; i < size; i++) {
      /* ------------------------------------------------------- */
      /* Absorb the T_IPC_MSG array. Since elements of a message */
      /* array are read-only, we do not increment the reference  */
      /* count of the T_IPC_MSG array elements.                  */
      /* ------------------------------------------------------- */
      retVal[i].destroyMsg();
      retVal[i].absorb(array[i]);
    }


    return retVal;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a real4 field from a message */
  real4 TipcMsg::nextReal4() const  throw (TipcException)
  {
    real4 data = 0.0;
    if (!TipcMsgNextReal4(_msg, &data))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a real4 array field from a message */
  const real4* TipcMsg::nextReal4Array(int4& size) const  throw (TipcException)
  {
    real4* data = (real4*)NULL;
    ;
    if (!TipcMsgNextReal4Array(_msg, 
                               &data,
                               &size))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a real8 field from a message */
  real8 TipcMsg::nextReal8() const  throw (TipcException)
  {
    real8 data = 0.0;
    if (!TipcMsgNextReal8(_msg, &data))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a real8 array field from a message */
  const real8* TipcMsg::nextReal8Array(int4& size) const  throw (TipcException)
  {
    real8* data = (real8*) NULL;

    if (!TipcMsgNextReal8Array(_msg,&data,&size))
      throw TipcMsgException();

    return const_cast<const real8*>(data);
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a real16 field from a message */
  real16 TipcMsg::nextReal16()  const throw (TipcException)
  {
    real16 data;
    if (!TipcMsgNextReal16(_msg, &data))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a real16 array field from a message */
  const real16* TipcMsg::nextReal16Array(int4& size) const  throw (TipcException)
  {
    real16* data = (real16*) NULL;
    if (!TipcMsgNextReal16Array(_msg, &data, &size))
      throw TipcMsgException();

    return const_cast<const real16*>(data);
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a const char * field from a message */
  const char* TipcMsg::nextString()  const throw (TipcException)
  {
    char* data = (char*) NULL;

    if (!TipcMsgNextStr(_msg, &data))
      throw TipcMsgException();

    return const_cast<const char*> (data);
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a const char * array field from a message */
  const char** TipcMsg::nextStringArray(int4& size) const   throw (TipcException)
  {
    char** data = (char**) NULL;
    if (!TipcMsgNextStrArray(_msg, &data, &size))
      throw TipcMsgException();

    return const_cast<const char **> (data);
  }


  /* ======================================================================== */
  /*..TipcMsg::next -- get a T_XML field from a message */
  const XML TipcMsg::nextXML()  const throw (TipcException)
  {
    T_XML data = (T_XML)NULL;

    if (!TipcMsgNextXml(_msg, &data))
      throw TipcMsgException();

    return(const XML) XML(data);
  }




  /* ======================================================================== */
  /*..TipcMsg::next -- get a byte field from a message */
  unsigned char TipcMsg::nextByte() const  throw (TipcException)
  {
    unsigned char data = 0;
    if (!TipcMsgNextByte(_msg, &data))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::nextBool -- get a bool field from a message */
  bool TipcMsg::nextBool() const  throw (TipcException)
  {
    T_BOOL data;
    if (!TipcMsgNextBool(_msg, &data))
      throw TipcMsgException();

    return data?true:false;
  }

  /* ======================================================================== */
  /*..TipcMsg::nextBool -- get a bool array field from a message */
  bool* TipcMsg::nextBoolArray(int4& size) const  throw (TipcException)
  {
    T_BOOL* tempData= (T_BOOL*) NULL;

    if (!TipcMsgNextBoolArray(_msg, &tempData, &size))
      throw TipcMsgException();

    bool* data = new bool [size];

    for (int i = 0; i < size; i++) {
      data [i] = tempData[i];
    }

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::nextTimestamp -- get a timestamp field from a message */
  real8 TipcMsg::nextTimestamp() const  throw (TipcException)
  {
    real8 data = 0.0;
    if (!TipcMsgNextTimestamp(_msg, &data))
      throw TipcMsgException();

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::nextTimestampArray -- get a timestamp array field from a message */
  const real8* TipcMsg::nextTimestampArray(int4& size) const  throw (TipcException)
  {
    real8* data = (real8*) NULL;

    if (!TipcMsgNextTimestampArray(_msg,&data,&size))
      throw TipcMsgException();

    return const_cast<const real8*>(data);
  }

  /* ======================================================================== */
  /*..TipcMsg::nextUtf8 -- get a UTF-8 field from a message */
  const char* TipcMsg::nextUtf8() const  throw (TipcException)
  {
    char* data = (char*) NULL;

    if (!TipcMsgNextUtf8(_msg, &data))
      throw TipcMsgException();

    return const_cast<char *>(data);
  }

  /* ======================================================================== */
  /*..TipcMsg::next -- get a UTF-8 array field from a message */
  const char** TipcMsg::nextUtf8Array(int4& size) const  throw (TipcException)
  {
    char** data = (char**)NULL;

    if (!TipcMsgNextUtf8Array(_msg,&data, &size))
      throw TipcMsgException();

    return const_cast<const char **> (data);
  }

  /* Next Unknown SMV 09/25/98 */

  /* ======================================================================== */
  /*..TipcMsg::nextUnknown -- get a field from a message */
  void TipcMsg::nextUnknown() const  throw (TipcException)
  {
    TutErrNumSet(0);
    bool retVal = TipcMsgNextUnknown(_msg);
    if (!retVal)
      if (TutErrNumGet() != 0)
        throw TipcMsgException();

  }

  /* ------------------------------------------------------------- */
  /* Message printing functionality                                */
  /* ------------------------------------------------------------- */

  /* ======================================================================== */
  /*..TipcMsg::print -- print all information of a message */
  void TipcMsg::print(T_OUT_FUNC func) const  throw (TipcException)
  {
    if (!TipcMsgPrint(_msg, func))
      throw TipcMsgException();
  }


  /* ======================================================================== */
  /*..TipcMsg::reset() -- reset the _problem flag */
  void TipcMsg::reset() throw (TipcException)
  {
    setNumFields(0);
  }

  /* ======================================================================== */
  /*..TipcMsg::lbMode -- get load balancing mode */
  T_IPC_LB_MODE TipcMsg::getLbMode() const throw (TipcException)
  {
    T_IPC_LB_MODE retVal = T_IPC_LB_NONE;

    if (!TipcMsgGetLbMode(_msg, &retVal))
      throw TipcMsgException();

    return retVal;
  }

  /* ======================================================================== */
  /*..TipcMsg::lbMode -- set load balancing mode */
  void TipcMsg::setLbMode(T_IPC_LB_MODE lbMode) throw (TipcException)
  {
    T_IPC_DELIVERY_MODE delMode =  getDeliveryMode();

    if (lbMode == T_IPC_LB_WEIGHTED && !(delMode == T_IPC_DELIVERY_SOME || delMode == T_IPC_DELIVERY_ALL)) {
      TutErrNumSet(T_ERR_TYPE_INVALID);
      throw TipcMsgException();
    }

    if (!TipcMsgSetLbMode(_msg, lbMode))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::headerStrEncode -- get header encode mode */
  bool TipcMsg::isUsingHeaderStrEncode() const throw (TipcException)
  {
    T_BOOL header_encode = T_FALSE;

    bool status = TipcMsgGetHeaderStrEncode(_msg, &header_encode);
    if (false == status)
      throw TipcMsgException();

    return header_encode?true:false;
  }

  /* ======================================================================== */
  /*..TipcMsg::headerStrEncode -- set header encode mode */
  void TipcMsg::setHeaderStrEncode(bool header_encode) throw (TipcException)
  {
    if (!TipcMsgSetHeaderStrEncode(_msg, header_encode))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::encrypt -- encrypt the given message */
  void TipcMsg::encrypt() throw (TipcException)
  {
    T_IPC_MSG encrypt_msg;
    encrypt_msg = TipcMsgEncrypt(_msg);
    if (NULL == encrypt_msg)
      throw TipcMsgException();


    destroyMsg();
    absorb(encrypt_msg);
  }

  /* ======================================================================== */
  /*..TipcMsg::decrypt -- decrypt the given message */
  void TipcMsg::decrypt() throw (TipcException)
  {
    T_IPC_MSG decrypt_msg;
    decrypt_msg = TipcMsgDecrypt(_msg);
    if (NULL == decrypt_msg)
      throw TipcMsgException();

    destroyMsg();
    absorb(decrypt_msg);

  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named binary field to the message */
  void TipcMsg::addNamed(const char * name, 
                         const void *  value, 
                         int4 size)  throw (TipcException)
  {
    if (!TipcMsgAddNamedBinary(_msg, const_cast<char *> (name), 
                               const_cast<void*>(value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named binary ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const void *  value, 
                                       int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedBinaryPtr(_msg,const_cast<char *> (name), 
                                  const_cast<void*>(value), size, &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named char field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         char value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedChar(_msg, const_cast<char *> (name), value))
      throw TipcMsgException();
  }


  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named int2 field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         int2 value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedInt2(_msg, const_cast<char *> (name), value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named int2 array field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         const int2 *value, 
                         int4 size)  throw (TipcException)
  {
    if (!TipcMsgAddNamedInt2Array(_msg, const_cast<char *> (name), 
                                  const_cast<int2*>(value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add named int2 array ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const int2 *value, 
                                       int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedInt2ArrayPtr(_msg, const_cast<char *> (name),
                                     const_cast<int2*>(value), 
                                     size, &field_return))
      throw TipcMsgException();
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named int4 field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         int4 value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedInt4(_msg, const_cast<char *> (name), value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named int4 array field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         const int4 *value, 
                         int4 size)  throw (TipcException)
  {
    if (!TipcMsgAddNamedInt4Array(_msg, const_cast<char *> (name), 
                                  const_cast<int4*>(value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add named int4 array ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const int4 *value, 
                                       int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedInt4ArrayPtr(_msg, const_cast<char *> (name),
                                     const_cast<int4*>(value), size, &field_return))
      throw TipcMsgException();
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named int8 field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         int8 value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedInt8(_msg, const_cast<char *> (name), value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named int8 array field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         const int8 *value, 
                         int4 size)  throw (TipcException)
  {
    if (!TipcMsgAddNamedInt8Array(_msg, const_cast<char *> (name), 
                                  const_cast<int8*> (value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add named int8 array ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const int8 *value, 
                                       int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedInt8ArrayPtr(_msg, const_cast<char *> (name), 
                                     const_cast<int8*>(value), size, &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named msg field to the msg */
  void TipcMsg::addNamed(const char * name, const TipcMsg& value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedMsg(_msg, const_cast<char *> (name), (T_IPC_MSG)value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named msg array field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         const TipcMsg* arg, 
                         int4 size)  throw (TipcException)
  {
    if (size < 0)
      throw TipcMsgException();

    if (!arg) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMsgException();
    }

    T_IPC_MSG *array = (T_IPC_MSG*) NULL;

    // Windows has a bug where if a zero byte array is created, the OS will crash if it's deleted.
#ifndef T_OS_WINDOWS
    array = new T_IPC_MSG[size];
#else
    if (size) {
      array = new T_IPC_MSG[size];
    }
    else {
      array = new T_IPC_MSG[1];
    }
#endif

    if (!array) {
      TutErrNumSet(T_ERR_NOMEM);
      throw TipcMsgException();
    }

    for (int4 i = 0; i < size; i++) {
      array[i] = (T_IPC_MSG)arg[i];
    }
    if (!TipcMsgAddNamedMsgArray(_msg, const_cast<char *> (name), array, size)) {
      delete [] array;
      throw TipcMsgException();
    }

    delete [] array;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add named msg array ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name,
                                       const TipcMsg *arg, 
                                       int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (size < 0)
      throw TipcMsgException();

    if (!arg) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMsgException();
    }

    T_IPC_MSG *array = (T_IPC_MSG*) NULL;

    // Windows has a bug where if a zero byte array is created, the OS will crash if it's deleted.
#ifndef T_OS_WINDOWS
    array = new T_IPC_MSG[size];
#else
    if (size) {
      array = new T_IPC_MSG[size];
    }
    else {
      array = new T_IPC_MSG[1];
    }
#endif

    if (!array) {
      TutErrNumSet(T_ERR_NOMEM);
      throw TipcMsgException();
    }

    for (int4 i = 0; i < size; i++) {
      array[i] = (T_IPC_MSG)arg[i];
    }

    if (!TipcMsgAddNamedMsgArrayPtr(_msg, const_cast<char *> (name),  array, size, &field_return)) {
      delete [] array;
      throw TipcMsgException();
    }

    delete [] array;
    return field_return;
  }

  /* YX 06/21/2000 */
  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add named msg ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const TipcMsg& value) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedMsgPtr(_msg, const_cast<char *> (name), 
                               (T_IPC_MSG)value, &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named real4 field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         real4 value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedReal4(_msg, const_cast<char *> (name), value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named real4 array field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         const real4 *value, 
                         int4 size)  throw (TipcException)
  {
    if (!TipcMsgAddNamedReal4Array(_msg, const_cast<char *> (name), 
                                   const_cast<real4 *> (value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add named real4 array ptr field to msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const real4 *value,
                                       int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedReal4ArrayPtr(_msg, const_cast<char *> (name),
                                      const_cast<real4 *> (value), size, &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named real8 field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         real8 value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedReal8(_msg, const_cast<char *> (name), value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named real8 array field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         const real8 *value, 
                         int4 size)  throw (TipcException)
  {
    if (!TipcMsgAddNamedReal8Array(_msg, const_cast<char *> (name), 
                                   const_cast<real8 *> (value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add named real8 array ptr field to msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const real8 *value, 
                                       int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedReal8ArrayPtr(_msg, const_cast<char *> (name),
                                      const_cast<real8 *> (value), size, 
                                      &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named real16 field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         real16 value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedReal16(_msg, const_cast<char *> (name), value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named real16 array field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         const real16 *value,
                         int4 size)  throw (TipcException)
  {
    if (!TipcMsgAddNamedReal16Array(_msg, const_cast<char *> (name), 
                                    const_cast<real16 *>(value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add named real16 array ptr field to msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const real16 *value, 
                                       int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedReal16ArrayPtr(_msg, const_cast<char *> (name), 
                                       const_cast<real16 *>(value), size, 
                                       &field_return))
      throw TipcMsgException();
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named str field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         const char * value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedStr(_msg, const_cast<char *> (name), const_cast<char *> (value)))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named str array field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         const char ** value, 
                         int4 size)  throw (TipcException)
  {
    if (!TipcMsgAddNamedStrArray(_msg, const_cast<char *> (name), 
                                 const_cast<char **> (value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named str ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const char * value) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedStrPtr(_msg, const_cast<char *> (name), 
                               const_cast<char *> (value), &field_return))
      throw TipcMsgException();
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add named str array ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const char **value, 
                                       int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedStrArrayPtr(_msg, const_cast<char *> (name), 
                                    const_cast<char **> (value), size, 
                                    &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named xml field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         const XML& value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedXml(_msg, const_cast<char *> (name), (T_XML)value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named xml ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const XML& value) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedXmlPtr(_msg, const_cast<char *> (name), (T_XML)value, 
                               &field_return))
      throw TipcMsgException();

    return field_return;

  }



  /* ======================================================================== */
  /*..TipcMsg::addNamed -- add a named byte field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         unsigned char value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedByte(_msg, const_cast<char *> (name), value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamedBool -- add a named bool field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         bool value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedBool(_msg, const_cast<char *> (name), value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamedBool -- add a named bool array field to the msg */
  void TipcMsg::addNamed(const char * name, 
                         const bool *value, 
                         int4 size)  throw (TipcException)
  {
    T_BOOL* tempData = new T_BOOL[size];

    if (tempData) {
      for (int i = 0; i < size; i++) {
        tempData[i] = value[i];
      }
    }

    if (!TipcMsgAddNamedBoolArray(_msg, const_cast<char *> (name), tempData, size)) {
      delete [] tempData;
      throw TipcMsgException();
    }

    delete [] tempData;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamedBool -- add named bool array ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedPtr(const char * name, 
                                       const bool *value, 
                                       int4 size) throw (TipcException) 
  {
    T_IPC_MSG_FIELD field_return;
    T_BOOL* tempData = new T_BOOL[size];

    if (tempData) {
      for (int i = 0; i < size; i++) {
        tempData[i] = value[i];
      }
    }

    if (!TipcMsgAddNamedBoolArrayPtr(_msg, const_cast<char *> (name), 
                                     tempData, size, &field_return)) {
      delete [] tempData;
      throw TipcMsgException();
    }

    delete [] tempData;
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamedTimestamp -- add a named timestamp field */
  void TipcMsg::addNamedTimestamp(const char * name, 
                                  real8 value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedTimestamp(_msg, const_cast<char *> (name), value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamedTimestampArray -- add a named timestamp array field */
  void TipcMsg::addNamedTimestampArray(const char * name, 
                                       const real8 *value, 
                                       int4 size)  throw (TipcException)
  {
    if (!TipcMsgAddNamedTimestampArray(_msg, const_cast<char *> (name), 
                                       const_cast<real8 *> (value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamedTimestampArrayPtr -- add named timestamp array ptr */
  T_IPC_MSG_FIELD TipcMsg::addNamedTimestampArrayPtr(const char* name, 
                                                     const real8 *value, 
                                                     int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAddNamedTimestampArrayPtr(_msg, const_cast<char *> (name), 
                                          const_cast<real8 *> (value), 
                                          size, 
                                          &field_return))
    {
      throw TipcMsgException();
    }
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamedUtf8 -- add a named UTF-8 field to the msg */
  void TipcMsg::addNamedUtf8(const char * name, 
                             const char * value)  throw (TipcException)
  {
    if (!TipcMsgAddNamedUtf8(_msg, const_cast<char *> (name), const_cast<char *> (value)))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamedUtf8 -- add a named UTF-8 array field to the msg */
  void TipcMsg::addNamedUtf8Array(const char * name, 
                                  const char * *value, 
                                  int4 size)  throw (TipcException)
  {
    if (!TipcMsgAddNamedUtf8Array(_msg, const_cast<char *> (name), 
                                  const_cast<char **> (value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamedUtf8 -- add a named UTF_8 ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedUtf8Ptr(const char * name, 
                                           const char * value) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    if (!TipcMsgAddNamedUtf8Ptr(_msg, const_cast<char *> (name), 
                                const_cast<char *> (value), &field_return))
      throw TipcMsgException();
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamedUtf8 -- add named UTF-8 array ptr field to the msg */
  T_IPC_MSG_FIELD TipcMsg::addNamedUtf8ArrayPtr(const char* name, 
                                                const char** value, 
                                                int4 size) throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgAddNamedUtf8ArrayPtr(_msg, const_cast<char *> (name), 
                                     const_cast<char **> (value), size, &field_return))
      throw TipcMsgException();
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::addNamedUnknown -- add named unknown field to the msg */
  void TipcMsg::addNamedUnknown(const char * name, 
                                T_IPC_FT type)  throw (TipcException)
  {
    if (!TipcMsgAddNamedUnknown(_msg, const_cast<char *> (name), type))
      throw TipcMsgException();
  }


  //////////////////////////////KKKKK
  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named binary field from the msg */
  const void* TipcMsg::getNamedBinary(const char * name,int4& size) const throw (TipcException)
  {
    void * retVal =(void *)NULL;
    if (!TipcMsgGetNamedBinary(_msg, const_cast<char *> (name), &retVal, &size))
      throw TipcMsgException();

    return const_cast <const void*>(retVal);
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named char field from the msg */
  char TipcMsg::getNamedChar(const char * name)  const  throw (TipcException)
  {
    char retVal = 0;
    if (!TipcMsgGetNamedChar(_msg, const_cast<char *> (name), &retVal))
      throw TipcMsgException();

    return retVal;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named int2 field from the msg */
  int2 TipcMsg::getNamedInt2(const char * name) const   throw (TipcException)
  {
    int2 value = 0;
    if (!TipcMsgGetNamedInt2(_msg, const_cast<char *> (name), &value))
      throw TipcMsgException();

    return value;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named int2 array field from the msg */
  const int2* TipcMsg::getNamedInt2Array(const char * name, int4& size) const   
  throw (TipcException)
  {
    int2* value = (int2*) NULL;

    if (!TipcMsgGetNamedInt2Array(_msg, const_cast<char *> (name), &value, &size))
      throw TipcMsgException();

    return const_cast <const int2*>(value);
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named int4 field from the msg */
  int4 TipcMsg::getNamedInt4(const char * name) const   throw (TipcException)
  {
    int4 value = 0;
    if (!TipcMsgGetNamedInt4(_msg, const_cast<char *> (name), &value))
      throw TipcMsgException();

    return value;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named int4 array field from the msg */
  const int4* TipcMsg::getNamedInt4Array(const char * name, int4& size) const   throw (TipcException)
  {
    int4* value = (int4*) NULL;

    if (!TipcMsgGetNamedInt4Array(_msg, const_cast<char *> (name), &value, &size))
      throw TipcMsgException();

    return const_cast <const int4*>(value);
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named int8 field from the msg */
  int8 TipcMsg::getNamedInt8(const char * name)  const  throw (TipcException)
  {
    int8 value;
    if (!TipcMsgGetNamedInt8(_msg, const_cast<char *> (name), &value))
      throw TipcMsgException();

    return value;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named int8 array field from the msg */
  const int8* TipcMsg::getNamedInt8Array(const char * name, int4& size)  const  throw (TipcException)
  {
    int8* value = (int8*) NULL;

    if (!TipcMsgGetNamedInt8Array(_msg, const_cast<char *> (name), &value, &size))
      throw TipcMsgException();

    return const_cast <const int8*>(value);
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named msg field from the msg */
  const TipcMsg TipcMsg::getNamedMsg(const char * name) const   throw (TipcException)
  {
    T_IPC_MSG value = (T_IPC_MSG) NULL;

    if (!TipcMsgGetNamedMsg(_msg, const_cast<char *> (name),  &value))
      throw TipcMsgException();

    return(const TipcMsg) TipcMsg(value);
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named msg array field from the msg */
  TipcMsg* TipcMsg::getNamedMsgArray(const char * name,int4& size)  const  
  throw (TipcException)
  {
    T_IPC_MSG *array = (T_IPC_MSG*) NULL;

    TipcMsg* retVal = (TipcMsg*)NULL;

    if (!TipcMsgGetNamedMsgArray(_msg, const_cast<char *> (name),  &array, &size))
      throw TipcMsgException();

    // Windows has a bug where if a zero byte array is created, the OS will crash if it's deleted.
#ifndef T_OS_WINDOWS
    retVal = new TipcMsg[size];
#else
    if (size) {
      retVal = new TipcMsg[size];
    }
    else {
      retVal = new TipcMsg[1];
    }
#endif

    if (!retVal) {
      TutErrNumSet(T_ERR_NOMEM);
      throw TipcMsgException();
    }


    for (int4 i = 0; i < size; i++) {
      /* ------------------------------------------------------- */
      /* Absorb the T_IPC_MSG array. Since elements of a message */
      /* array are read-only, we do not increment the reference  */
      /* count of the T_IPC_MSG array elements.                  */
      /* ------------------------------------------------------- */
      (retVal)[i].destroyMsg();
      (retVal)[i].absorb(array[i]);
    }


    return retVal;

  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named real4 field from the msg */
  real4 TipcMsg::getNamedReal4(const char * name) const   
  throw (TipcException)
  {
    real4 value = 0.0;

    if (!TipcMsgGetNamedReal4(_msg, const_cast<char *> (name), &value))
      throw TipcMsgException();

    return value;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named real4 array field from the msg */
  const real4* TipcMsg::getNamedReal4Array(const char * name,int4& size)  const
  throw (TipcException)
  {
    real4* value = (real4*) NULL;

    if (!TipcMsgGetNamedReal4Array(_msg, const_cast<char *> (name),  &value, &size))
      throw TipcMsgException();

    return const_cast <const real4*>(value);
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named real8 field from the msg */
  real8 TipcMsg::getNamedReal8(const char * name) const
  throw (TipcException)
  {
    real8 value = 0.0;

    if (!TipcMsgGetNamedReal8(_msg, const_cast<char *> (name),  &value))
      throw TipcMsgException();

    return value;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named real8 array field from the msg */
  const real8* TipcMsg::getNamedReal8Array(const char * name, int4& size)  const  
  throw (TipcException)
  {
    real8* value = (real8*) NULL;
    if (!TipcMsgGetNamedReal8Array(_msg, const_cast<char *> (name),  &value, &size))
      throw TipcMsgException();

    return const_cast<const real8*>(value);
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named real16 field from the msg */
  real16 TipcMsg::getNamedReal16(const char * name) const   
  throw (TipcException)
  {
    real16 value;

    if (!TipcMsgGetNamedReal16(_msg, const_cast<char *> (name),  &value))
      throw TipcMsgException();

    return value;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named real16 array field from the msg */
  const real16* TipcMsg::getNamedReal16Array(const char * name,int4& size)  const
  throw (TipcException)
  {
    real16* value = (real16*) NULL;

    if (!TipcMsgGetNamedReal16Array(_msg, const_cast<char *> (name), &value, &size))
      throw TipcMsgException();

    return const_cast <real16*>(value);
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named str field from the msg */
  const char* TipcMsg::getNamedString(const char * name) const
  throw (TipcException)
  {
    char* value = (char*) NULL;
    if (!TipcMsgGetNamedStr(_msg, const_cast<char *> (name),  
                            &value))
      throw TipcMsgException();

    return const_cast<const char*>(value) ;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named str array field from the msg */
  const char** TipcMsg::getNamedStringArray(const char * name,int4& size) const 
  throw (TipcException)
  {
    char** value = (char**) NULL;

    if (!TipcMsgGetNamedStrArray(_msg, const_cast<char *> (name),&(value),&size))
      throw TipcMsgException();

    return const_cast<const char**>(value) ;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named xml field from the msg */
  const XML TipcMsg::getNamedXML(const char * name) const   throw (TipcException)
  {
    T_XML value = (T_XML)NULL;
    if (!TipcMsgGetNamedXml(_msg, const_cast<char *> (name),  &value))
      throw TipcMsgException();

    return(const XML) XML(value);
  }


  /* ======================================================================== */
  /*..TipcMsg::getNamed -- get a named byte field from the msg */
  unsigned char TipcMsg::getNamedByte(const char * name) const   throw (TipcException)
  {
    unsigned  char value = 0;
    if (!TipcMsgGetNamedByte(_msg, const_cast<char *> (name),  &value))
      throw TipcMsgException();

    return value;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamedBool -- get a named bool field from the msg */
  bool TipcMsg::getNamedBool(const char * name) const   throw (TipcException)
  {
    T_BOOL value = 0;
    if (!TipcMsgGetNamedBool(_msg, const_cast<char *> (name), &value))
      throw TipcMsgException();

    return value?true:false;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamedBool -- get a named bool array field from the msg */
  bool* TipcMsg::getNamedBoolArray(const char * name,int4& size) const   throw (TipcException)
  {
    T_BOOL* value = 0;
    if (!TipcMsgGetNamedBoolArray(_msg, const_cast<char *> (name), &value, &size))
      throw TipcMsgException();

    bool* data = new bool [size];

    for (int i = 0; i < size; i++) {
      data [i] = value[i];
    }

    return data;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamedTimestamp -- get a named timestamp field from the msg */
  real8 TipcMsg::getNamedTimestamp(const char * name) const
  throw (TipcException)
  {
    real8 value = 0.0;

    if (!TipcMsgGetNamedTimestamp(_msg, const_cast<char *> (name),  &value))
      throw TipcMsgException();

    return value;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamedTimestampArray -- get a named timestamp array field from the msg */
  const real8* TipcMsg::getNamedTimestampArray(const char * name, int4& size)  const  
  throw (TipcException)
  {
    real8* value = (real8*) NULL;
    if (!TipcMsgGetNamedTimestampArray(_msg, const_cast<char *> (name),  &value, &size))
      throw TipcMsgException();

    return const_cast<const real8*>(value);
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamedUtf8 -- get a named UTF-8 field from the msg */
  const char* TipcMsg::getNamedUtf8(const char * name) const   throw (TipcException)
  {
    char* value = (char*) NULL;

    if (!TipcMsgGetNamedUtf8(_msg, const_cast<char *> (name), &value))
      throw TipcMsgException();

    return const_cast<const char*>(value);
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamedUtf8 -- get a named UTF-8 array field from the msg */
  const char** TipcMsg::getNamedUtf8Array(const char * name,int4& size)  const
  throw (TipcException)
  {
    char** value = (char**) NULL;
    if (!TipcMsgGetNamedUtf8Array(_msg, const_cast<char *> (name), const_cast<char***> (&value), &size))
      throw TipcMsgException();

    return const_cast<const char**>(value);
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamedUnknown -- get a named unknown field from the msg */
  void TipcMsg::getNamedUnknown(const char * name)  const  throw (TipcException)
  {
    if (!TipcMsgGetNamedUnknown(_msg,const_cast<char*>(name)))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::getTypeNamed -- get the type of a named field from the msg */
  T_IPC_FT TipcMsg::getTypeNamed(const char * name)  const  throw (TipcException)
  {
    T_IPC_FT type;
    if (!TipcMsgGetTypeNamed(_msg, const_cast<char *> (name),  &type))
      throw TipcMsgException();

    return type;
  }

  /* ======================================================================== */
  /*..TipcMsg::existsNamed -- get whether or not a named field exists in msg */
  bool TipcMsg::existsNamed(const char * name) const   throw (TipcException)
  {
    T_BOOL retVal;

    if (!TipcMsgExistsNamed(_msg, const_cast<char *> (name),  &retVal))
      throw TipcMsgException();

    return retVal?true:false;
  }

  /* ======================================================================== */
  /*..TipcMsg::getNamedCurrent -- get the name of the current field */
  const char* TipcMsg::getNameCurrent()  const  throw (TipcException)
  {
    char* name = NULL;

    if (!TipcMsgGetNameCurrent(_msg,&name))
      throw TipcMsgException();

    return  const_cast<const char*> (name);
  }

  /* ======================================================================== */
  /*..TipcMsg::setNameCurrent -- set the name of the current field */
  void TipcMsg::setNameCurrent(const char * name)  throw (TipcException)
  {
    if (!TipcMsgSetNameCurrent(_msg, const_cast<char *> (name)))
      throw TipcMsgException();

  }

  /* Added for SS5.6 Feature YX 06/10/00 */
  /* ======================================================================== */
  /*..TipcMsg::deleteCurrent -- delete the current field in a message. */
  void TipcMsg::deleteCurrent()  throw (TipcException)
  {
    if (!TipcMsgDeleteCurrent(_msg))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::deleteField -- delete the specified field in a message. */
  void TipcMsg::deleteField(int4 index)  throw (TipcException)
  {
    if (!TipcMsgDeleteField(_msg, index))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::deleteNamed -- delete the named field in a message. */
  void TipcMsg::deleteNamed(const char * name)  throw (TipcException)
  {
    if (!TipcMsgDeleteNamedField(_msg, const_cast<char *> (name)))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update a named binary field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            const void *  value, 
                            int4 size)  throw (TipcException)
  {
    if (!TipcMsgUpdateNamedBinary(_msg, const_cast<char *> (name),  
                                  const_cast<void*>(value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update a named binary ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const void *  value, 
                                          int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgUpdateNamedBinaryPtr(_msg, const_cast<char *> (name),  
                                     const_cast<void*>(value), size, 
                                     &field_return))
      throw TipcMsgException();
    return field_return;

  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update a named char field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            char value)  throw (TipcException)
  {
    if (!TipcMsgUpdateNamedChar(_msg, const_cast<char *> (name),value))
      throw TipcMsgException();
  }


  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- Update a named int2 field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            int2 value)  throw (TipcException)
  {
    if (!TipcMsgUpdateNamedInt2(_msg, const_cast<char *> (name),  value))
      throw TipcMsgException();

  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update a named int2 array field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            const int2 *value, 
                            int4 size)  throw (TipcException)
  {
    if (!TipcMsgUpdateNamedInt2Array(_msg, const_cast<char *> (name),  
                                     const_cast<int2*>(value), size))
      throw TipcMsgException();
  } 

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named int2 array ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const int2 *value, 
                                          int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedInt2ArrayPtr(_msg, const_cast<char *> (name),  
                                        const_cast<int2*>(value), size, &field_return))
      throw TipcMsgException();
    return field_return;
  }


  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- Update named int4 field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            int4 value)  throw (TipcException)
  {
    if (!TipcMsgUpdateNamedInt4(_msg, const_cast<char *> (name),  value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named int4 array field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            const int4 *value, 
                            int4 size)  throw (TipcException)
  {
    if (!TipcMsgUpdateNamedInt4Array(_msg, const_cast<char *> (name), 
                                     const_cast<int4*>(value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named int4 array ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const int4 *value, 
                                          int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedInt4ArrayPtr(_msg, const_cast<char *> (name),
                                        const_cast<int4*>(value), size, &field_return))
      throw TipcMsgException();
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- Update named int8 field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            int8 value)  throw (TipcException)
  {
    if (!TipcMsgUpdateNamedInt8(_msg, const_cast<char *> (name),  value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named int8 array field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            const int8 *value, 
                            int4 size)  throw (TipcException)
  {
    if (!TipcMsgUpdateNamedInt8Array(_msg, const_cast<char *> (name), 
                                     const_cast<int8*>(value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named int8 array ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const int8 *value, 
                                          int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedInt8ArrayPtr(_msg, const_cast<char *> (name),  
                                        const_cast<int8*>(value), size, &field_return))
      throw TipcMsgException();
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- Update named msg field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            const TipcMsg& value)  throw (TipcException)
  {         
    if (!TipcMsgUpdateNamedMsg(_msg, const_cast<char *> (name), (T_IPC_MSG) value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named msg array field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            const TipcMsg *arg, 
                            int4 size)  throw (TipcException)
  {
    if (size < 0)
      throw TipcMsgException();

    if (!arg) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMsgException();
    }

    T_IPC_MSG *array = (T_IPC_MSG*) NULL;

    // Windows has a bug where if a zero byte array is created, the OS will crash if it's deleted.
#ifndef T_OS_WINDOWS
    array = new T_IPC_MSG[size];
#else
    if (size) {
      array = new T_IPC_MSG[size];
    }
    else {
      array = new T_IPC_MSG[1];
    }
#endif

    if (!array) {
      TutErrNumSet(T_ERR_NOMEM);
      throw TipcMsgException();
    }

    for (int4 i = 0; i < size; i++) {
      array[i] = (T_IPC_MSG)arg[i];
    }

    if (!TipcMsgUpdateNamedMsgArray(_msg, const_cast<char *> (name),  array, size)) {
      delete [] array;
      throw TipcMsgException();
    }

    delete [] array;

  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named msg array ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const TipcMsg *arg, 
                                          int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (size < 0)
      throw TipcMsgException();

    if (!arg) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMsgException();
    }

    T_IPC_MSG *array = (T_IPC_MSG*) NULL;

    // Windows has a bug where if a zero byte array is created, the OS will crash if it's deleted.
#ifndef T_OS_WINDOWS
    array = new T_IPC_MSG[size];
#else
    if (size) {
      array = new T_IPC_MSG[size];
    }
    else {
      array = new T_IPC_MSG[1];
    }
#endif

    if (!array) {
      TutErrNumSet(T_ERR_NOMEM);
      throw TipcMsgException();
    }

    for (int4 i = 0; i < size; i++) {
      array[i] = (T_IPC_MSG)arg[i];
    }

    if (!TipcMsgUpdateNamedMsgArrayPtr(_msg, const_cast<char *> (name),  array, size, &field_return)) {
      delete [] array;
      throw TipcMsgException();
    }

    delete [] array;

    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- Update named msg ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const TipcMsg& value)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedMsgPtr(_msg, const_cast<char *> (name),  value, &field_return))
      throw TipcMsgException();
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- Update named real4 field in a msg */
  void TipcMsg::updateNamed(const char * name, real4 value)  throw (TipcException)
  {                
    if (!TipcMsgUpdateNamedReal4(_msg, const_cast<char *> (name),  value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named real4 array field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            const real4 *value, 
                            int4 size)  throw (TipcException)
  {  
    if (!TipcMsgUpdateNamedReal4Array(_msg, const_cast<char *> (name), 
                                      const_cast<real4*>(value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named real4 array ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const real4 *value, 
                                          int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedReal4ArrayPtr(_msg, const_cast<char *> (name),  
                                         const_cast<real4*>(value), size, &field_return))
      throw TipcMsgException();
    return field_return;;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- Update named real8 field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            real8 value)  throw (TipcException)
  {               
    if (!TipcMsgUpdateNamedReal8(_msg, const_cast<char *> (name),  value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update a named real8 array field in the msg */
  void TipcMsg::updateNamed(const char * name, 
                            const real8 *value, 
                            int4 size)  throw (TipcException)
  {  
    if (!TipcMsgUpdateNamedReal8Array(_msg, const_cast<char *> (name), 
                                      const_cast<real8*>(value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named real8 array ptr field in the msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const real8 *value, 
                                          int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedReal8ArrayPtr(_msg, const_cast<char *> (name), 
                                         const_cast<real8*>(value), size, &field_return))
      throw TipcMsgException();
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- Update a named real16 field in the msg */
  void TipcMsg::updateNamed(const char * name, 
                            real16 value) throw (TipcException)
  {               
    if (!TipcMsgUpdateNamedReal16(_msg, const_cast<char *> (name),  value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named real16 array field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            const real16 *value, 
                            int4 size)  throw (TipcException)
  {
    if (!TipcMsgUpdateNamedReal16Array(_msg, const_cast<char *> (name), 
                                       const_cast<real16*>(value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named real16 array ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const real16 *value, 
                                          int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedReal16ArrayPtr(_msg, const_cast<char *> (name),  
                                          const_cast<real16*>(value), size, &field_return))
      throw TipcMsgException();
    return field_return;

  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- Update named str field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            const char * value)  throw (TipcException)
  {                
    if (!TipcMsgUpdateNamedStr(_msg, const_cast<char *> (name),  const_cast<char *> (value)))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named str array field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            const char * *value, 
                            int4 size)  throw (TipcException)
  {   
    if (!TipcMsgUpdateNamedStrArray(_msg, const_cast<char *> (name),  
                                    const_cast<char **> (value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update named str array ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const char * *value, 
                                          int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedStrArrayPtr(_msg, const_cast<char *> (name),  
                                       const_cast<char **> (value), size, &field_return))
      throw TipcMsgException();
    return field_return;

  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update a named str ptr field in the msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const char * value)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedStrPtr(_msg, const_cast<char *> (name),
                                  const_cast<char *> (value), &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update a named xml field in the msg */
  void TipcMsg::updateNamed(const char * name, 
                            const XML& value)  throw (TipcException)
  {                 
    if (!TipcMsgUpdateNamedXml(_msg, const_cast<char *> (name),  (T_XML)value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- update a named xml ptr field in the msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const XML& value)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedXmlPtr(_msg, const_cast<char *> (name),  
                                  (T_XML)value, &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamed -- Update named byte field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            unsigned char value)  throw (TipcException)
  {
    if (!TipcMsgUpdateNamedByte(_msg, const_cast<char *> (name),  value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamedBool -- Update named bool field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            bool value)  throw (TipcException)
  {
    if (!TipcMsgUpdateNamedBool(_msg, const_cast<char *> (name),  value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamedBool -- update named bool array field in a msg */
  void TipcMsg::updateNamed(const char * name, 
                            const bool *value, 
                            int4 size)  throw (TipcException)
  {
    T_BOOL* tempData = new T_BOOL[size];

    if (tempData) {
      for (int i = 0; i < size; i++) {
        tempData[i] = value[i];
      }
    }


    if (!TipcMsgUpdateNamedBoolArray(_msg, const_cast<char *> (name), tempData, size)) {
      delete [] tempData;
      throw TipcMsgException();
    }

    delete [] tempData;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamedBool -- update named bool array ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedPtr(const char * name, 
                                          const bool *value, 
                                          int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;
    T_BOOL* tempData = new T_BOOL[size];

    if (tempData) {
      for (int i = 0; i < size; i++) {
        tempData[i] = value[i];
      }
    }

    if (!TipcMsgUpdateNamedBoolArrayPtr(_msg, const_cast<char *> (name), 
                                        tempData, size,  &field_return)) {
      delete [] tempData;
      throw TipcMsgException();
    }

    delete [] tempData;
    return field_return;

  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamedTimestamp -- Update named timestamp field in a msg */
  void TipcMsg::updateNamedTimestamp(const char * name, 
                                     real8 value)  throw (TipcException)
  {               
    if (!TipcMsgUpdateNamedTimestamp(_msg, const_cast<char *> (name),  value))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamedTimestampArray -- update named timestamp array field in a msg */
  void TipcMsg::updateNamedTimestampArray(const char * name, 
                                          const real8 *value, 
                                          int4 size)  throw (TipcException)
  {   
    if (!TipcMsgUpdateNamedTimestampArray(_msg, const_cast<char *> (name), 
                                          const_cast<real8*>(value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamedTimestampArrayPtr -- update named timestamp array ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedTimestampArrayPtr(const char * name, 
                                                        const real8 *value, 
                                                        int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;

    if (!TipcMsgUpdateNamedTimestampArrayPtr(_msg, const_cast<char *> (name),  
                                             const_cast<real8 *> (value), 
                                             size, &field_return))
    {
      throw TipcMsgException();
    }
    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamedUtf8 -- Update named UTF-8 field in a msg */
  void TipcMsg::updateNamedUtf8(const char * name, 
                                const char * value)  throw (TipcException)
  {                
    if (!TipcMsgUpdateNamedUtf8(_msg, const_cast<char *> (name),  const_cast<char *> (value)))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamedUtf8 -- update named UTF-8 array field in a msg */
  void TipcMsg::updateNamedUtf8(const char * name, 
                                const char * *value, 
                                int4 size)  throw (TipcException)
  {   
    if (!TipcMsgUpdateNamedUtf8Array(_msg, const_cast<char *> (name),  
                                     const_cast<char **> (value), size))
      throw TipcMsgException();
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamedUtf8 -- update named UTF-8 array ptr field in a msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedUtf8Ptr(const char * name, 
                                              const char * *value, 
                                              int4 size)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedUtf8ArrayPtr(_msg, const_cast<char *> (name),  
                                        const_cast<char **> (value), size, &field_return))
      throw TipcMsgException();

    return field_return;
  }

  /* ======================================================================== */
  /*..TipcMsg::updateNamedUtf8 -- update a named UTF-8 ptr field in the msg */
  T_IPC_MSG_FIELD TipcMsg::updateNamedUtf8Ptr(const char * name, 
                                              const char * value)  throw (TipcException)
  {
    T_IPC_MSG_FIELD field_return = (T_IPC_MSG_FIELD)NULL;


    if (!TipcMsgUpdateNamedUtf8Ptr(_msg, const_cast<char *> (name),  
                                   const_cast<char *> (value), &field_return))
      throw TipcMsgException();

    return field_return;
  }

} //namespace SmartSockets
