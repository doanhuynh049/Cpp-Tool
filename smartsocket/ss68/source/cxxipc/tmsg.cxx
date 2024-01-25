/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tmsg.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmsg.hxx>
#include <rtworks/tmt.hxx>

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

  _status = T_TRUE;
  _problem = T_FALSE;
  _size = 0;
  _msg = msg;

}

/* ======================================================================== */
/*..TipcMsg::TipcMsg -- TipcMsg constructor for vacant objects */
TipcMsg::TipcMsg()
{
  _status = T_TRUE;
  _problem = T_FALSE;
  _size = 0;
  _msg = (T_IPC_MSG)T_NULL;

}

/* ======================================================================== */
/*..TipcMsg::TipcMsg -- TipcMsg constructor */
TipcMsg::TipcMsg(const TipcMt& mt)
{
  _status = T_TRUE;
  _problem = T_FALSE;
  _size = 0;
  _msg = TipcMsgCreate(mt._mt);

  if (T_NULL == _msg) {
    _status = T_FALSE;
  }

}

/* ======================================================================== */
/*..TipcMsg::TipcMsg -- TipcMsg constructor */
TipcMsg::TipcMsg(T_INT4 mt_num, 
     T_STR destination,
     T_STR sender)
{
  T_IPC_MT mt;

  _status = T_TRUE;
  _problem = T_FALSE;

  _size = 0;

  mt  = TipcMtLookupByNum(mt_num);

  if (!mt) {
    _status = T_FALSE;
  }

  _msg = TipcMsgCreate(mt);

  if (T_NULL == _msg) {
    _status = T_FALSE;
    return;
  }

  if (T_NULL != destination) {
    _status |= TipcMsgSetDest(_msg, destination);
  }

  if (T_NULL != sender) {
    _status |= TipcMsgSetSender(_msg, sender);
  }

}

/* ======================================================================== */
/*..TipcMsg:: TipcMsg -- TipcMsg constructor */
TipcMsg:: TipcMsg(T_STR mt_name)
{
  T_IPC_MT mt;

  _status = T_TRUE;
  _problem = T_FALSE;

  _size = 0;

  mt  = TipcMtLookup(mt_name);
  if (!mt) {
    _status = T_FALSE;
  }

  _msg = TipcMsgCreate(mt);
  if (T_NULL == _msg) {
    _status = T_FALSE;
  }
}

// --------------------------------------------------------------
// Copy constructor.  We avoid cloning and use ref count instead.
// --------------------------------------------------------------

/* ======================================================================== */
/*..TipcMsg::TipcMsg -- TipcMsg copy constructor */
TipcMsg::TipcMsg(const TipcMsg& msg)
{

  /* -------------------------------- */
  /* Copy the data to the new object. */
  /* -------------------------------- */
  _status = msg._status;
  _problem = msg._problem;
  _size = msg._size;
  _msg = msg._msg;
  
  /* ---------------------------------------------------- */
  /* If non-vacant, then increment ref count of T_IPC_MSG */
  /* ---------------------------------------------------- */
  if (T_NULL != _msg) {
    _status = TipcMsgIncrRefCount(_msg);
  }

}

/* ======================================================================== */
/*..TipcMsg::TipcMsg -- TipcMsg constructor */
TipcMsg::TipcMsg(const T_IPC_MSG msg)
{

  /* ---------------------------------- */
  /* Take over management of T_IPC_MSG. */
  /* ---------------------------------- */
  absorb(msg);

  /* ---------------------------------------------------- */
  /* If non-vacant, then increment ref count of T_IPC_MSG */
  /* ---------------------------------------------------- */
  if (T_NULL != _msg) {
    _status = TipcMsgIncrRefCount(_msg);
  }

}

/* ======================================================================== */
/*..TipcMsg::destroyMsg -- safely destroy managed T_IPC_MSG message */
void TipcMsg::destroyMsg(void)
{
  /* ----------------------------------------------------------------- */
  /* Destroy the T_IPC_MSG managed by the object.  TipcMsgDestroy      */
  /* manages the T_IPC_MSG reference count, therefore the actual       */
  /* T_IPC_MSG will not be destroyed until the reference count goes to */
  /* zero.                                                             */
  /* ----------------------------------------------------------------- */
  if (T_NULL != _msg) {
    T_BOOL status;
    T_BOOL read_only_return;

    status = TipcMsgGetReadOnly(_msg, &read_only_return);
    T_EXPECT(T_TRUE == status)    // expect the msg to be valid
#ifndef T_NO_ASSERTS /* needed for AIX 3.2.5 xlC.  SMW 02/10/96 */
      ;
#endif
#if 0 /* no point in using T_EXPECT and TutWarning together.  SMW 02/10/96 */
    if (!status) {
      TutWarning("  TutErrNum = %d.\n", TutErrNumGet());
    }
#endif
    
    if ( !read_only_return ) {
      status = TipcMsgDestroy(_msg);
      T_EXPECT(T_TRUE == status)   // expect the msg to be valid
#ifndef T_NO_ASSERTS /* needed for AIX 3.2.5 xlC.  SMW 02/10/96 */
  ;
#endif
#if 0 /* no point in using T_EXPECT and TutWarning together.  SMW 02/10/96 */
      if (!status) {
  TutWarning("  TutErrNum = %d.\n", TutErrNumGet());
      }
#endif
    }
  }
  
}

/* ======================================================================== */
/*..TipcMsg::~TipcMsg -- TipcMsg destructor */
TipcMsg::~TipcMsg()
{
  destroyMsg();
}

// -------------------------------------------------------------
// Assignment operators
// -------------------------------------------------------------

/* ======================================================================== */
/*..TipcMsg::operator= -- TipcMsg assignment operator */
TipcMsg& TipcMsg::operator=(const TipcMsg& msg)
{
  _status = msg._status;
  _problem = msg._problem;

  /* ------------------------------------------------- */
  /* Destroy message currently managed by this object. */
  /* ------------------------------------------------- */
  destroyMsg();

  /* ------------------------------ */
  /* Assign the data to the object. */
  /* ------------------------------ */
  _size = msg._size;
  _msg = msg._msg;

  /* ---------------------------------------------------- */
  /* If non-vacant, then increment ref count of T_IPC_MSG */
  /* ---------------------------------------------------- */
  if (T_NULL != _msg) {
    T_BOOL status;
    T_BOOL read_only_return;

    status = TipcMsgGetReadOnly(_msg, &read_only_return);
    T_EXPECT(T_TRUE == status)    // expect the msg to be valid

#ifndef T_NO_ASSERTS /* needed for AIX 3.2.5 xlC.  SMW 02/10/96 */
      ;
#endif
#if 0 /* no point in using T_EXPECT and TutWarning together.  SMW 02/10/96 */
    if (!status) {
      TutWarning("  TutErrNum = %d.\n", TutErrNumGet());
    }
#endif
    
    if ( !read_only_return ) {
      _status = TipcMsgIncrRefCount(_msg);
      T_EXPECT(T_TRUE == status)   // expect the msg to be valid
#ifndef T_NO_ASSERTS /* needed for AIX 3.2.5 xlC.  SMW 02/10/96 */
  ;
#endif
#if 0 /* no point in using T_EXPECT and TutWarning together.  SMW 02/10/96 */
    if (!status) {
      TutWarning("  TutErrNum = %d.\n", TutErrNumGet());
    }
#endif
    
    }
  }

  return *this;
}

/* ======================================================================== */
/*..TipcMsg::operator= -- TipcMsg assignment operator */
TipcMsg& TipcMsg::operator=(const T_IPC_MSG msg)
{
  /* ------------------------------------------------- */
  /* Destroy message currently managed by this object. */
  /* ------------------------------------------------- */
  destroyMsg();

  /* ---------------------------------- */
  /* Take over management of T_IPC_MSG. */
  /* ---------------------------------- */
  absorb(msg);

  /* ---------------------------------------------------- */
  /* If non-vacant, then increment ref count of T_IPC_MSG */
  /* ---------------------------------------------------- */
  if (T_NULL != _msg) {
    T_BOOL status;
    T_BOOL read_only_return;

    status = TipcMsgGetReadOnly(_msg, &read_only_return);
    T_EXPECT(T_TRUE == status)    // expect the msg to be valid

#ifndef T_NO_ASSERTS /* needed for AIX 3.2.5 xlC.  SMW 02/10/96 */
      ;
#endif
#if 0 /* no point in using T_EXPECT and TutWarning together.  SMW 02/10/96 */
    if (!status) {
      TutWarning("  TutErrNum = %d.\n", TutErrNumGet());
    }
#endif
    
    if ( !read_only_return ) {
      _status = TipcMsgIncrRefCount(_msg);
      T_EXPECT(T_TRUE == status)   // expect the msg to be valid
#ifndef T_NO_ASSERTS /* needed for AIX 3.2.5 xlC.  SMW 02/10/96 */
  ;
#endif
#if 0 /* no point in using T_EXPECT and TutWarning together.  SMW 02/10/96 */
    if (!status) {
      TutWarning("  TutErrNum = %d.\n", TutErrNumGet());
    }
#endif
    
    }
  }

  return *this;

}

/* ======================================================================== */
/*..TipcMsg::Create -- Create a new message for TipcMsg object */
T_BOOL TipcMsg::Create(TipcMt& mt)
{
  /* ------------------------------------------------- */
  /* Destroy message currently managed by this object. */
  /* ------------------------------------------------- */
  destroyMsg();

  /* ------------------------------------------------ */
  /* Create new message to be managed by this object. */
  /* ------------------------------------------------ */
  _status = T_TRUE;
  _problem = T_FALSE;

  _size = 0;
 
  _msg = TipcMsgCreate(mt._mt);

  if (T_NULL == _msg) {
    _status = T_FALSE;
  }

  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Create -- Create a new message for TipcMsg object */
T_BOOL TipcMsg::Create(T_INT4 mt_num)
{
  T_IPC_MT mt;

  /* ------------------------------------------------- */
  /* Destroy message currently managed by this object. */
  /* ------------------------------------------------- */
  destroyMsg();

  /* ------------------------------------------------ */
  /* Create new message to be managed by this object. */
  /* ------------------------------------------------ */
  _status = T_TRUE;
  _problem = T_FALSE;

  _size = 0;

  mt  = TipcMtLookupByNum(mt_num);
  if (!mt) {
    _status = T_FALSE;
    _msg = (T_IPC_MSG)T_NULL;
    return _status;
  }
 
  _msg = TipcMsgCreate(mt);
  if (T_NULL == _msg) {
    _status = T_FALSE;
  }

  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Create -- Create a new message for TipcMsg object */
T_BOOL TipcMsg::Create(T_STR mt_name)
{
  T_IPC_MT mt;

  /* ------------------------------------------------- */
  /* Destroy message currently managed by this object. */
  /* ------------------------------------------------- */
  destroyMsg();

  /* ------------------------------------------------ */
  /* Create new message to be managed by this object. */
  /* ------------------------------------------------ */
  _status = T_TRUE;
  _problem = T_FALSE;

  _size = 0;

  mt  = TipcMtLookup(mt_name);
  if (!mt) {
    _status = T_FALSE;
    _msg = (T_IPC_MSG)T_NULL;
    return _status;
  }
 
  _msg = TipcMsgCreate(mt);
  if (T_NULL == _msg) {
    _status = T_FALSE;
  }

  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Clone -- Clone a new message for TipcMsg object */
T_BOOL TipcMsg::Clone(TipcMsg& msg)
{
  /* ------------------------------------------------- */
  /* Destroy message currently managed by this object. */
  /* ------------------------------------------------- */
  destroyMsg();

  /* ----------------------------------------------- */
  /* Clone new message to be managed by this object. */
  /* ----------------------------------------------- */
  _status = T_TRUE;
  _problem = T_FALSE;

  _size = 0;
 
  /* ---------------------------------- */
  /* If non-vacant, then clone message. */
  /* ---------------------------------- */
  if (T_NULL != msg._msg) {
    _msg = TipcMsgClone(msg._msg);
    if (T_NULL == _msg) {
      _status = T_FALSE;
    }
  }
  else {
    _msg = (T_IPC_MSG)T_NULL;
  }

  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Destroy -- destroy a message */
T_BOOL TipcMsg::Destroy()
{
  if (T_NULL != _msg) {
    _status = TipcMsgDestroy(_msg);

    /* ----------------------------------------------------------------- */
    /* If TipcMsgDestroy worked, then convert object to a vacant object. */
    /* ----------------------------------------------------------------- */
    if (_status) {
      _msg = (T_IPC_MSG)T_NULL;
    }
  }

  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Traverse -- traverse all fields in a message */
T_PTR TipcMsg::Traverse(T_IPC_MSG_TRAV_FUNC func,
      T_PTR arg)
{
  return TipcMsgTraverse(_msg,func,arg);
}

/* ======================================================================== */
/*..TipcMsg::IncrRefCount -- increment the reference count of a message */
T_BOOL TipcMsg::IncrRefCount()
{
  _status = TipcMsgIncrRefCount(_msg);
  return _status;
}

#ifdef T_RTWORKS35_COMPAT
/* ======================================================================== */
/*..TipcMsg::DataPtr -- get the ptr to the beginning of a message's data */
T_PTR TipcMsg::DataPtr()
{
  T_PTR data_ptr_return = T_NULL;  // initialize variable

  _status = TipcMsgGetDataPtr(_msg, &data_ptr_return);
  return data_ptr_return;
}

/* ======================================================================== */
/*..TipcMsg::DataSize -- get the size of a message's data buffer */
T_INT4 TipcMsg::DataSize()
{
  T_INT4 data_size_return = -1;  // initialize variable

  _status = TipcMsgGetDataSize(_msg, &data_size_return);
  return data_size_return;
}
#endif

/* ======================================================================== */
/*..TipcMsg::ReadOnly -- get read-only status of message */
T_BOOL TipcMsg::ReadOnly()
{
  T_BOOL read_only_return = T_TRUE;  // initialize variable

  _status = TipcMsgGetReadOnly(_msg, &read_only_return);
  return read_only_return;
}

/* ======================================================================== */
/*..TipcMsg::RefCount -- get the reference count of a message */
T_INT4 TipcMsg::RefCount()
{
  T_INT2 ref_count_return = 0;  // initialize variable

  _status = TipcMsgGetRefCount(_msg, &ref_count_return);
  return (T_INT4)ref_count_return;
}

/* ======================================================================== */
/*..TipcMsg::Vacant -- get the vacant status of a TipcMsg object */
T_BOOL TipcMsg::Vacant()
{
  if (T_NULL == _msg) {
    return T_TRUE;  // vacant object
  }
  else {
    return T_FALSE; // non-vacant object
  }
}

/* ======================================================================== */
/*..TipcMsg::Dest -- get the destination property of a message */
T_STR TipcMsg::Dest()
{
  T_STR dest_return = (T_STR)"";  // initialize variable

  _status = TipcMsgGetDest(_msg, &dest_return);
  return dest_return;
}

/* ======================================================================== */
/*..TipcMsg::Dest -- set the destination property of a message */
T_BOOL TipcMsg::Dest(T_STR dest)
{
  _status = TipcMsgSetDest(_msg, dest);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Sender -- get the sender property of a message */
T_STR TipcMsg::Sender()
{
  T_STR sender_return = (T_STR)"";  // initialize variable

  _status = TipcMsgGetSender(_msg, &sender_return);
  return sender_return;
}

/* ======================================================================== */
/*..TipcMsg::Sender -- set the sender property of a message */
T_BOOL TipcMsg::Sender(T_STR sender)
{
  _status = TipcMsgSetSender(_msg, sender);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Type -- get the type of a message */
TipcMt& TipcMsg::Type()
{
  _mt_referent._mt = (T_IPC_MT)T_NULL;  // initialize variable

  _status = TipcMsgGetType(_msg, &_mt_referent._mt);

  return _mt_referent;
}

/* ======================================================================== */
/*..TipcMsg::Type -- set the type of a message */
T_BOOL TipcMsg::Type(TipcMt& mt)
{
  _status = TipcMsgSetType(_msg, mt._mt);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Current -- get the current field of a message */
T_INT4 TipcMsg::Current()
{
  T_INT4 field_num_return = -1;  // initialize variable

  _status = TipcMsgGetCurrent(_msg, &field_num_return);
  return field_num_return;
}                 

/* ======================================================================== */
/*..TipcMsg::Current -- set the current field of a message */
T_BOOL TipcMsg::Current(T_INT4 field_num)
{
  _status = TipcMsgSetCurrent(_msg, field_num);
  return _status;
} 

#ifdef T_RTWORKS35_COMPAT
/* ======================================================================== */
/*..TipcMsg::DataMax -- get the maximum size of a message's data buffer */
T_INT4 TipcMsg::DataMax()
{
  T_INT4 data_max_return = -1;  // initialize variable

  _status = TipcMsgGetDataMax(_msg, &data_max_return);
  return data_max_return;
}                 

/* ======================================================================== */
/*..TipcMsg::DataMax -- set the maximum size of a message's data buffer */
T_BOOL TipcMsg::DataMax(T_INT4 data_max)
{
  _status = TipcMsgSetDataMax(_msg, data_max);
  return _status;
} 
#endif

/* ======================================================================== */
/*..TipcMsg::NumFields -- get the number of fields in a message */
T_INT4 TipcMsg::NumFields()
{
  T_INT4 num_fields_return = -1;  // initialize variable

  _status = TipcMsgGetNumFields(_msg, &num_fields_return);
  return num_fields_return;
}                 

/* ======================================================================== */
/*..TipcMsg::NumFields -- set the number of fields in a message */
T_BOOL TipcMsg::NumFields(T_INT4 num_fields)
{
  _status = TipcMsgSetNumFields(_msg, num_fields);
  return _status;
} 

/* ======================================================================== */
/*..TipcMsg::Priority -- get the priority of a message */
T_INT2 TipcMsg::Priority()
{
  T_INT2 priority_return = 0;  // initialize variable

  _status = TipcMsgGetPriority(_msg, &priority_return);
  return priority_return;
}   

/* ======================================================================== */
/*..TipcMsg::Priority -- set the priority of a message */
T_BOOL TipcMsg::Priority(T_INT2 priority)
{
  _status = TipcMsgSetPriority(_msg, priority);
  return _status;
} 

/* ======================================================================== */
/*..TipcMsg::UserProp -- get user-defined property from a message */
T_INT4 TipcMsg::UserProp()
{
  T_INT4 user_prop_return = 0;  // intialize variable

  _status = TipcMsgGetUserProp(_msg, &user_prop_return);
  return user_prop_return;
}

/* ======================================================================== */
/*..TipcMsg::UserProp -- set user-defined property from message */
T_BOOL TipcMsg::UserProp(T_INT4 user_prop)
{
  _status = TipcMsgSetUserProp(_msg, user_prop);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::SeqNum -- get the sequence number of a message */
T_INT4 TipcMsg::SeqNum()
{
  T_INT4 seq_num_return = 0;  // intialize variable

  _status = TipcMsgGetSeqNum(_msg, &seq_num_return);
  return seq_num_return;
}

/* ======================================================================== */
/*..TipcMsg::Ack -- acknowledge delivery of a message */
T_BOOL TipcMsg::Ack()
{
  _status = TipcMsgAck(_msg);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::DeliveryMode -- get the delivery mode of a message */
T_IPC_DELIVERY_MODE TipcMsg::DeliveryMode()
{
  T_IPC_DELIVERY_MODE delivery_mode_return = T_IPC_DELIVERY_BEST_EFFORT;

  _status = TipcMsgGetDeliveryMode(_msg, &delivery_mode_return);
  return delivery_mode_return;
} 

/* ======================================================================== */
/*..TipcMsg::DeliveryMode -- set the delivery mode of a message */
T_BOOL TipcMsg::DeliveryMode(T_IPC_DELIVERY_MODE delivery_mode)
{
  _status = TipcMsgSetDeliveryMode(_msg, delivery_mode);
  return _status;
} 

/* ======================================================================== */
/*..TipcMsg::DeliveryTimeout -- get the delivery timeout of a message */
T_REAL8 TipcMsg::DeliveryTimeout()
{
  T_REAL8 delivery_timeout_return;

  _status = TipcMsgGetDeliveryTimeout(_msg,&delivery_timeout_return);
  return delivery_timeout_return;
}

/* ======================================================================== */
/*..TipcMsg::DeliveryTimeout -- set the delivery timeout of a message */
T_BOOL TipcMsg::DeliveryTimeout(T_REAL8 timeout)
{
  _status = TipcMsgSetDeliveryTimeout(_msg,timeout);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::MessageId -- get message id of a message */
T_STR TipcMsg::MessageId()
{
  T_STR message_id_return = (T_STR)"";  // initialize variable

  _status = TipcMsgGetMessageId(_msg, &message_id_return);
  return message_id_return;
}

/* ======================================================================== */
/*..TipcMsg::GenerateMessageId -- set message id of a message */
T_BOOL TipcMsg::GenerateMessageId()
{
  _status = TipcMsgGenerateMessageId(_msg);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::CorrelationId -- get correlation id of a message */
T_STR TipcMsg::CorrelationId()
{
  T_STR correlation_id_return = (T_STR)"";  // intialize variable

  _status = TipcMsgGetCorrelationId(_msg, &correlation_id_return);
  return correlation_id_return;
}

/* ======================================================================== */
/*..TipcMsg::CorrelationId -- set correlation id of a message */
T_BOOL TipcMsg::CorrelationId(T_STR correlation_id)
{
  _status = TipcMsgSetCorrelationId(_msg, correlation_id);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::ReplyTo -- get reply to subject of a message */
T_STR TipcMsg::ReplyTo()
{
  T_STR subj_return = (T_STR)"";  // intialize variable

  _status = TipcMsgGetReplyTo(_msg, &subj_return);
  return subj_return;
}

/* ======================================================================== */
/*..TipcMsg::ReplyTo -- set reply to subject of a message */
T_BOOL TipcMsg::ReplyTo(T_STR subj)
{
  _status = TipcMsgSetReplyTo(_msg, subj);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Expiration -- get expiration of a message */
T_REAL8 TipcMsg::Expiration()
{
  T_REAL8 expiration_return;

  _status = TipcMsgGetExpiration(_msg, &expiration_return);
  return expiration_return;
}

/* ======================================================================== */
/*..TipcMsg::Expiration -- set expiration of a message */
T_BOOL TipcMsg::Expiration(T_REAL8 expiration)
{
  _status = TipcMsgSetExpiration(_msg, expiration);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::SenderTimestamp -- get sender timestamp of a message */
T_REAL8 TipcMsg::SenderTimestamp()
{
  T_REAL8 sender_timestamp_return;

  _status = TipcMsgGetSenderTimestamp(_msg, &sender_timestamp_return);
  return sender_timestamp_return;
}

/* ======================================================================== */
/*..TipcMsg::SenderTimestamp -- set sender timestamp of a message */
T_BOOL TipcMsg::SenderTimestamp(T_REAL8 sender_timestamp)
{
  _status = TipcMsgSetSenderTimestamp(_msg, sender_timestamp);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::ArrivalTimestamp -- get arrival timestamp of a message */
T_REAL8 TipcMsg::ArrivalTimestamp()
{
  T_REAL8 arrival_timestamp_return;

  _status = TipcMsgGetArrivalTimestamp(_msg, &arrival_timestamp_return);
  return arrival_timestamp_return;
}

/* ======================================================================== */
/*..TipcMsg::ArrivalTimestamp -- set arrival timestamp of a message */
T_BOOL TipcMsg::ArrivalTimestamp(T_REAL8 arrival_timestamp)
{
  _status = TipcMsgSetArrivalTimestamp(_msg, arrival_timestamp);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::PacketSize -- get the packet size of a message */
T_UINT4 TipcMsg::PacketSize()
{
  T_UINT4 packet_size_return = 0;  // initialize variable

  _status = TipcMsgGetPacketSize(_msg, &packet_size_return);
  return packet_size_return;
}

/* ======================================================================== */
/*..TipcMsg::MsgToPacket -- copy the message to a single packet of memory */
T_BOOL TipcMsg::MsgToPacket(T_PTR packet_return,
          T_UINT4 packet_size)
{
  _status = TipcMsgToPacket(_msg, packet_return, packet_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::PacketToMsg -- convert a single pkt of memory back into a msg */
T_BOOL TipcMsg::PacketToMsg(T_PTR packet)
{
  T_IPC_MSG msg_return;

  /* ------------------------------------------------- */
  /* Destroy message currently managed by this object. */
  /* ------------------------------------------------- */
  destroyMsg();

  /* ---------------------------- */
  /* Convert packet to T_IPC_MSG. */
  /* ---------------------------- */
  _status = TipcPacketToMsg(packet, &msg_return);
  if (!_status) {
    _msg = (T_IPC_MSG)T_NULL;
  }
  else {
    /* ---------------------------------- */
    /* Take over management of T_IPC_MSG. */
    /* ---------------------------------- */
    absorb(msg_return);
  }

  return _status;
}

#ifdef T_RTWORKS35_COMPAT
/* ======================================================================== */
/*..TipcMsg::ModeToStr -- convert a delivery mode to a string */
T_BOOL TipcMsg::ModeToStr(T_IPC_DELIVERY_MODE delivery_mode,
        T_STR* delivery_mode_return)
{
  return TipcDeliveryModeToStr(delivery_mode, delivery_mode_return);
}

/* ======================================================================== */
/*..TipcMsg::StrToMode -- convert a string to a delivery mode */
T_BOOL TipcMsg::StrToMode(T_STR delivery_str, 
        T_IPC_DELIVERY_MODE* delivery_mode_return)
{
  return TipcStrToDeliveryMode(delivery_str, delivery_mode_return);
}
#endif

/* ======================================================================== */
/*..TipcMsg::DeliveryModeToStr -- convert a delivery mode to a string */
T_BOOL TipcMsg::DeliveryModeToStr(T_IPC_DELIVERY_MODE delivery_mode,
          T_STR* delivery_mode_return)
{
  return TipcDeliveryModeToStr(delivery_mode, delivery_mode_return);
}

/* ======================================================================== */
/*..TipcMsg::StrToDeliveryMode -- convert a string to a delivery mode */
T_BOOL TipcMsg::StrToDeliveryMode(T_STR delivery_str, 
          T_IPC_DELIVERY_MODE* delivery_mode_return)
{
  return TipcStrToDeliveryMode(delivery_str, delivery_mode_return);
}

/* ======================================================================== */
/*..TipcMsg::FtToStr -- convert a field type to a string */
T_BOOL TipcMsg::FtToStr(T_IPC_FT type,
      T_STR* str_return)
{
  return TipcFtToStr(type, str_return);
}

/* ======================================================================== */
/*..TipcMsg::StrToFt -- convert a string to a field type */
T_BOOL TipcMsg::StrToFt(T_STR str,
      T_IPC_FT *type_return)
{
  return TipcStrToFt(str, type_return);
}

/* ======================================================================== */
/*..TipcMsg::FieldSize -- set a field size */
T_BOOL TipcMsg::FieldSize(T_IPC_MSG_FIELD field, 
        T_INT4 field_size)
{
  return TipcMsgFieldSetSize(field, field_size);
}

/* ======================================================================== */
/*.. TipcMsg::LbModeToStr -- convert load balancing mode to string */
T_BOOL TipcMsg::LbModeToStr(T_IPC_LB_MODE lb_mode, 
          T_STR *str_return)
{
  return TipcLbModeToStr(lb_mode, str_return);
} 

/* ======================================================================== */
/*.. TipcMsg::StrToLbMode -- convert a string to a load balancing mode */
T_BOOL TipcMsg::StrToLbMode(T_STR str, 
          T_IPC_LB_MODE *lb_mode_return)
{
  return TipcStrToLbMode(str, lb_mode_return);
}

/* -------------------------------------------------------------- */
/* Manipulator Check function for Message Insertion or Extraction */
/* -------------------------------------------------------------- */

/* ======================================================================== */
/*..Check -- manipulator to set object status to T_FALSE if a problem */
/*..         occurred during insertion or extraction */
TipcMsg& Check(TipcMsg& msg)
{
  msg._status = !msg._problem;
  return msg;
}

/* ------------------------------------------------- */
/* Insertion                                         */
/* ------------------------------------------------- */

/* ======================================================================== */
/*..TipcMsg::operator<< -- binary data insertion operator */
TipcMsg& TipcMsg::operator<<(T_PTR arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendBinary(_msg, arg, _size);
    _problem = !_status;
  }

  return *this;
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_CHAR insertion operator */
TipcMsg& TipcMsg::operator<<(T_CHAR arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendChar(_msg, arg);
    _problem = !_status;
  }
  return *this;
}

/* insert T_GUID YX 06/21/2000 */
/* ======================================================================== */
/*..TipcMsg::operator<< -- T_GUID insertion operator */
TipcMsg& TipcMsg::operator<<(T_GUID arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendGuid(_msg, arg);
    _problem = !_status;
  }
  return *this;         
}

/* insert T_GUID array YX 06/21/2000 */
/* ======================================================================== */
/*..TipcMsg::operator<< -- T_GUID array insertion operator */
TipcMsg& TipcMsg::operator<<(T_GUID* arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendGuidArray(_msg, arg, _size);
    _problem = !_status;
  }
  return *this;         
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_INT2 insertion operator */
TipcMsg& TipcMsg::operator<<(T_INT2 arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendInt2(_msg, arg);
    _problem = !_status;
  }
  return *this;
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_INT2 array insertion operator */
TipcMsg& TipcMsg::operator<<(T_INT2* arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendInt2Array(_msg, arg, _size); 
    _problem = !_status;
  }
  return *this;
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_INT4 insertion operator */
TipcMsg& TipcMsg::operator<<(T_INT4 arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendInt4(_msg, arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_INT4 array insertion operator */
TipcMsg& TipcMsg::operator<<(T_INT4* arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendInt4Array(_msg, arg, _size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_INT8 insertion operator */
TipcMsg& TipcMsg::operator<<(T_INT8 arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendInt8(_msg, arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_INT8 array insertion operator */
TipcMsg& TipcMsg::operator<<(T_INT8* arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendInt8Array(_msg, arg, _size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- TipcMsg object insertion operator */
TipcMsg& TipcMsg::operator<<(TipcMsg& arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendMsg(_msg, arg._msg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- TipcMsg object array insertion operator */
TipcMsg& TipcMsg::operator<<(TipcMsg* arg)
{
  T_IPC_MSG *c_api_msg_array = new T_IPC_MSG[_size];

  for (T_INT4 i = 0; i < _size; i++ ) {
    c_api_msg_array[i] = arg[i]._msg;
  }

  if ( !_problem ) {
    _status = TipcMsgAppendMsgArray(_msg, c_api_msg_array, _size);
    _problem = !_status;
  }

  delete c_api_msg_array;

  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_REAL4 insertion operator */
TipcMsg& TipcMsg::operator<<(T_REAL4 arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendReal4(_msg, arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_REAL4 array insertion operator */
TipcMsg& TipcMsg::operator<<(T_REAL4* arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendReal4Array(_msg, arg, _size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_REAL8 insertion operator */
TipcMsg& TipcMsg::operator<<(T_REAL8 arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendReal8(_msg, arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_REAL8 array insertion operator */
TipcMsg& TipcMsg::operator<<(T_REAL8* arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendReal8Array(_msg, arg, _size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_REAL16 insertion operator */
TipcMsg& TipcMsg::operator<<(T_REAL16 arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendReal16(_msg, arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_REAL16 array insertion operator */
TipcMsg& TipcMsg::operator<<(T_REAL16* arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendReal16Array(_msg, arg, _size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_STR insertion operator */
#if defined(T_OS_HPUX)
TipcMsg& TipcMsg::operator<<(const char* arg)
#else
TipcMsg& TipcMsg::operator<<(T_STR arg)
#endif
{
  if ( !_problem ) {
    _status = TipcMsgAppendStr(_msg, (T_STR)arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_STR array insertion operator */
TipcMsg& TipcMsg::operator<<(T_STR* arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendStrArray(_msg, arg, _size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- T_XML insertion operator */
TipcMsg& TipcMsg::operator<<(T_XML arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendXml(_msg, arg);
    _problem = !_status;
  }
  return *this;           
}


/* ======================================================================== */
/*..TipcMsg::operator<< -- byte insertion operator */
TipcMsg& TipcMsg::operator<<(T_UCHAR arg)
{
  if ( !_problem ) {
    _status = TipcMsgAppendByte(_msg, arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator<< -- execute a parameterized manipulator function */
TipcMsg& TipcMsg::operator<<(_TipcMsgManipF func)
{
  return (*func)(*this);
}

/* ------------------------------------------------- */
/* Extraction                                        */
/* ------------------------------------------------- */

/* ======================================================================== */
/*..TipcMsg::operator>> -- binary data extraction operator */
TipcMsg& TipcMsg::operator>>(T_PTR& arg) 
{
  if ( !_problem ) {
    _status = TipcMsgNextBinary(_msg, &arg, &_size);
    _problem = !_status;
  }
  return *this;
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_CHAR extraction operator */
TipcMsg& TipcMsg::operator>>(T_CHAR& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextChar(_msg, &arg);
    _problem = !_status;
  }
  return *this;
}

/* T_GUID extraction YX 06/21/2000 */
/* ======================================================================== */
/*..TipcMsg::operator>> -- T_GUID extraction operator */ 
TipcMsg& TipcMsg::operator>>(T_GUID& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextGuid(_msg, &arg);
    _problem = !_status;
  }
  return *this;
}

/* T_GUID array extraction YX 06/21/2000 */
/* ======================================================================== */
/*..TipcMsg::operator>> -- T_GUID array extraction operator */
TipcMsg& TipcMsg::operator>>(T_GUID*& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextGuidArray(_msg, &arg, &_size);
    _problem = !_status;
  }
  return *this;
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_INT2 extraction operator */
TipcMsg& TipcMsg::operator>>(T_INT2& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextInt2(_msg, &arg);
    _problem = !_status;
  }
  return *this;
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_INT2 array extraction operator */
TipcMsg& TipcMsg::operator>>(T_INT2*& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextInt2Array(_msg, &arg, &_size);
    _problem = !_status;
  }
  return *this;
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_INT4 extraction operator */
TipcMsg& TipcMsg::operator>>(T_INT4& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextInt4(_msg, &arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_INT4 array extraction operator */
TipcMsg& TipcMsg::operator>>(T_INT4*& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextInt4Array(_msg, &arg, &_size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_INT8 extraction operator */
TipcMsg& TipcMsg::operator>>(T_INT8& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextInt8(_msg, &arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_INT8 array extraction operator */
TipcMsg& TipcMsg::operator>>(T_INT8*& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextInt8Array(_msg, &arg, &_size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- TipcMsg object extraction operator */
TipcMsg& TipcMsg::operator>>(TipcMsg& arg)
{
  T_IPC_MSG temp;

  if ( !_problem ) {
    _status = TipcMsgNextMsg(_msg, &temp);
    _problem = !_status;
    arg = temp;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- TipcMsg object array extraction operator */
TipcMsg& TipcMsg::operator>>(TipcMsg*& arg)
{
  T_IPC_MSG *c_api_msg_array;

  if ( !_problem ) {
    _status = TipcMsgNextMsgArray(_msg, &c_api_msg_array, &_size);
    _problem = !_status;
  }

  if (_status) {
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
    for (T_INT4 i = 0; i < _size; i++ ) {
      /* ------------------------------------------------------ */
      /* Absorb the T_IPC_MSG array. Since element of a message */
      /* array are read-only, we do not increment the reference */
      /* count of the T_IPC_MSG array elements.                 */
      /* ------------------------------------------------------ */
      arg[i].destroyMsg();
      arg[i].absorb(c_api_msg_array[i]);
    }
  }

  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_REAL4 extraction operator */
TipcMsg& TipcMsg::operator>>(T_REAL4& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextReal4(_msg, &arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_REAL4 array extraction operator */
TipcMsg& TipcMsg::operator>>(T_REAL4*& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextReal4Array(_msg, &arg, &_size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_REAL8 extraction operator */
TipcMsg& TipcMsg::operator>>(T_REAL8& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextReal8(_msg, &arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_REAL8 array extraction operator */
TipcMsg& TipcMsg::operator>>(T_REAL8*& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextReal8Array(_msg, &arg, &_size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_REAL16 extraction operator */
TipcMsg& TipcMsg::operator>>(T_REAL16& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextReal16(_msg, &arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_REAL16 array extraction operator */
TipcMsg& TipcMsg::operator>>(T_REAL16*& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextReal16Array(_msg, &arg, &_size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_STR extraction operator */
TipcMsg& TipcMsg::operator>>(T_STR& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextStr(_msg, &arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_STR array extraction operator */
TipcMsg& TipcMsg::operator>>(T_STR*& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextStrArray(_msg, &arg, &_size);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- T_XML extraction operator */
TipcMsg& TipcMsg::operator>>(T_XML& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextXml(_msg, &arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- byte extraction operator */
TipcMsg& TipcMsg::operator>>(T_UCHAR& arg)
{
  if ( !_problem ) {
    _status = TipcMsgNextByte(_msg, &arg);
    _problem = !_status;
  }
  return *this;           
}

/* ======================================================================== */
/*..TipcMsg::operator>> -- execute a parameterized manipulator function */
TipcMsg& TipcMsg::operator>>(_TipcMsgManipF func)
{
  return (*func)(*this);
}

/* ------------------------------------------------------ */
/* Append functions which provide the C API look-and-feel */
/* ------------------------------------------------------ */

/* ======================================================================== */
/*..TipcMsg::Append -- append a binary field to a message */
T_BOOL TipcMsg::Append(T_PTR binary_data, 
           T_INT4 binary_size)
{
  _status = TipcMsgAppendBinary(_msg, binary_data, binary_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_CHAR field to a message */
T_BOOL TipcMsg::Append(T_CHAR char_data)
{
  _status = TipcMsgAppendChar(_msg, char_data);
  return _status;
}

/* Append T_GUID YX 06/21/2000 */
/* ======================================================================== */
/*..TipcMsg::Append -- append a T_GUID field to a message */
T_BOOL TipcMsg::Append(T_GUID guid_data)
{
  _status = TipcMsgAppendGuid(_msg, guid_data);
  return _status;
}

/* Append T_GUID array YX 06/21/2000 */
/* ======================================================================== */
/*..TipcMsg::Append -- append a T_GUID array field to a message */
T_BOOL TipcMsg::Append(T_GUID *guid_array_data, 
           T_INT4 guid_array_size)
{
  _status = TipcMsgAppendGuidArray(_msg, guid_array_data, guid_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_INT2 field to a message */
T_BOOL TipcMsg::Append(T_INT2 int2_data)
{
  _status = TipcMsgAppendInt2(_msg, int2_data);
  return _status;
}


/* ======================================================================== */
/*..TipcMsg::Append -- append a T_INT2 array field to a message */
T_BOOL TipcMsg::Append(T_INT2* int2_array_data, 
           T_INT4 int2_array_size)
{
  _status = TipcMsgAppendInt2Array(_msg, int2_array_data, int2_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_INT4 field to a message */
T_BOOL TipcMsg::Append(T_INT4 int4_data)
{
  _status = TipcMsgAppendInt4(_msg, int4_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_INT4 array field to a message */
T_BOOL TipcMsg::Append(T_INT4* int4_array_data, 
           T_INT4 int4_array_size)
{
  _status = TipcMsgAppendInt4Array(_msg, int4_array_data, int4_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::Append -- append a T_INT8 field to a message */
T_BOOL TipcMsg::Append(T_INT8 int8_data)
{
  _status = TipcMsgAppendInt8(_msg, int8_data);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::Append -- append a T_INT8 array field to a message */
T_BOOL TipcMsg::Append(T_INT8 *int8_array_data, 
           T_INT4 int8_array_size)
{
  _status = TipcMsgAppendInt8Array(_msg, int8_array_data, int8_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a message contained in TipcMsg object */
T_BOOL TipcMsg::Append(TipcMsg& msg_data)
{
  _status = TipcMsgAppendMsg(_msg, msg_data._msg);
  return _status;             
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a message array field to a message */
T_BOOL TipcMsg::Append(TipcMsg* msg_array_data, 
           T_INT4 msg_array_size)
{
  T_IPC_MSG *c_api_msg_array = new T_IPC_MSG[msg_array_size];

  for (T_INT4 i = 0; i < msg_array_size; i++ ) {
    c_api_msg_array[i] = msg_array_data[i]._msg;
  }

  _status = TipcMsgAppendMsgArray(_msg, c_api_msg_array, msg_array_size);

  delete c_api_msg_array;

  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_REAL4 field to a message */
T_BOOL TipcMsg::Append(T_REAL4 real4_data)
{
  _status = TipcMsgAppendReal4(_msg, real4_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_REAL4 array field to a message */
T_BOOL TipcMsg::Append(T_REAL4* real4_array_data, 
           T_INT4 real4_array_size)
{
  _status = TipcMsgAppendReal4Array(_msg, 
                    real4_array_data,
                    real4_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_REAL8 field to message */
T_BOOL TipcMsg::Append(T_REAL8 real8_data)
{
  _status = TipcMsgAppendReal8(_msg, real8_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_REAL8 array field to a message */
T_BOOL TipcMsg::Append(T_REAL8* real8_array_data, 
           T_INT4 real8_array_size)
{
  _status = TipcMsgAppendReal8Array(_msg, 
                    real8_array_data,
                    real8_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::Append -- append a T_REAL16 field to a message */
T_BOOL TipcMsg::Append(T_REAL16 real16_data)
{
  _status = TipcMsgAppendReal16(_msg, real16_data);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::Append -- append a T_REAL16 array field to a message */
T_BOOL TipcMsg::Append(T_REAL16 *real16_array_data, 
           T_INT4 real16_array_size)
{
  _status = TipcMsgAppendReal16Array(_msg, real16_array_data, 
                     real16_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_STR field to a message */
T_BOOL TipcMsg::Append(T_STR str_data)
{
  _status = TipcMsgAppendStr(_msg, str_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_STR array field to a message */
T_BOOL TipcMsg::Append(T_STR* str_array_data, 
           T_INT4 str_array_size)
{
  _status = TipcMsgAppendStrArray(_msg, str_array_data, str_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_STR field and a T_REAL8 field to a msg */
T_BOOL TipcMsg::Append(T_STR str_data, 
           T_REAL8 real8_data)
{
  _status = TipcMsgAppendStrReal8(_msg, str_data, real8_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a T_XML field to a message */
T_BOOL TipcMsg::Append(T_XML xml_data)
{
  _status = TipcMsgAppendXml(_msg, xml_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Append -- append a byte field to a message */
T_BOOL TipcMsg::Append(T_UCHAR byte_data)
{
  _status = TipcMsgAppendByte(_msg, byte_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AppendBool -- append a T_BOOL field to a message */
T_BOOL TipcMsg::AppendBool(T_BOOL bool_data)
{
  _status = TipcMsgAppendBool(_msg, bool_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AppendBool -- append a T_BOOL array field to a message */
T_BOOL TipcMsg::AppendBool(T_BOOL* bool_array_data, 
               T_INT4 bool_array_size)
{
  _status = TipcMsgAppendBoolArray(_msg, bool_array_data, bool_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AppendUtf8 -- append a UTF-8 string field to a message */
T_BOOL TipcMsg::AppendUtf8(T_STR str_data)
{
  _status = TipcMsgAppendUtf8(_msg, str_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AppendUtf8 -- append a UTF-8 string array field to a message */
T_BOOL TipcMsg::AppendUtf8(T_STR* str_array_data, 
               T_INT4 str_array_size)
{
  _status = TipcMsgAppendUtf8Array(_msg, str_array_data, str_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a binary pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_PTR binary_data, 
        T_INT4 binary_size, 
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendBinaryPtr(_msg, binary_data, binary_size, field_return);
  return _status;
}

/* Append T_GUID array ptr YX 06/21/2000 */
/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a T_GUID array pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_GUID *guid_array_data, 
        T_INT4 guid_array_size,
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendGuidArrayPtr(_msg, guid_array_data, guid_array_size, 
              field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a int2 array pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_INT2 *int2_array_data, 
        T_INT4 int2_array_size, 
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendInt2ArrayPtr(_msg, int2_array_data, int2_array_size, 
                      field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a int4 array pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_INT4 *int4_array_data, 
        T_INT4 int4_array_size,
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendInt4ArrayPtr(_msg, int4_array_data, int4_array_size, 
                      field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a int8 array pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_INT8 *int8_array_data, 
        T_INT4 int8_array_size,
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendInt8ArrayPtr(_msg, int8_array_data, int8_array_size, 
                      field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a msg pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_IPC_MSG msg_data, 
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendMsgPtr(_msg, msg_data, field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a msg array pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_IPC_MSG *msg_array_data, 
        T_INT4 msg_array_size,
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendMsgArrayPtr(_msg, msg_array_data, msg_array_size, 
             field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a real4 array pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_REAL4 *real4_array_data, 
        T_INT4 real4_array_size,
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendReal4ArrayPtr(_msg, real4_array_data, real4_array_size,
                       field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a real8 array pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_REAL8 *real8_array_data, 
        T_INT4 real8_array_size,
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendReal8ArrayPtr(_msg, real8_array_data, real8_array_size,
                       field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a real16 array pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_REAL16 *real16_array_data, 
        T_INT4 real16_array_size,
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendReal16ArrayPtr(_msg, real16_array_data, real16_array_size, 
                    field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a string pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_STR str_data, 
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendStrPtr(_msg, str_data, field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append a str array pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_STR *str_array_data, 
        T_INT4 str_array_size,
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendStrArrayPtr(_msg, str_array_data, str_array_size, 
             field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendPtr -- append an xml pointer field to a message */
T_BOOL TipcMsg::AppendPtr(T_XML xml_data, 
        T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendXmlPtr(_msg, xml_data, field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendBoolPtr -- append a bool array pointer field to a message */
T_BOOL TipcMsg::AppendBoolPtr(T_BOOL *bool_array_data, 
            T_INT4 bool_array_size,
            T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendBoolArrayPtr(_msg, bool_array_data, bool_array_size, 
                                      field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendUtf8Ptr -- append a UTF-8 pointer field to a message */
T_BOOL TipcMsg::AppendUtf8Ptr(T_STR str_data, 
            T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendUtf8Ptr(_msg, str_data, field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::AppendUtf8Ptr -- append a UTF-8 array pointer field to a message*/
T_BOOL TipcMsg::AppendUtf8Ptr(T_STR *str_array_data, 
            T_INT4 str_array_size,
            T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAppendUtf8ArrayPtr(_msg, str_array_data, str_array_size, 
              field_return);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a binary pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_PTR binary_data, 
        T_INT4 binary_size)
{
  _status = TipcMsgFieldUpdateBinaryPtr(field, binary_data, binary_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a T_GUID array pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_GUID *guid_array_data, 
        T_INT4 guid_array_size)
{
  _status = TipcMsgFieldUpdateGuidArrayPtr(field, guid_array_data, guid_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a int2 array pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_INT2 *int2_array_data, 
        T_INT4 int2_array_size)
{
  _status = TipcMsgFieldUpdateInt2ArrayPtr(field, int2_array_data, int2_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a int4 array pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_INT4 *int4_array_data, 
        T_INT4 int4_array_size)
{
  _status = TipcMsgFieldUpdateInt4ArrayPtr(field, int4_array_data, int4_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a int8 array pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_INT8 *int8_array_data, 
        T_INT4 int8_array_size)
{
  _status = TipcMsgFieldUpdateInt8ArrayPtr(field, int8_array_data, int8_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a msg pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_IPC_MSG msg_data) 
{
  _status = TipcMsgFieldUpdateMsgPtr(field, msg_data);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a msg array pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_IPC_MSG *msg_array_data, 
        T_INT4 msg_array_size)
{
  _status = TipcMsgFieldUpdateMsgArrayPtr(field, msg_array_data, msg_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a real4 array pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_REAL4 *real4_array_data, 
        T_INT4 real4_array_size)
{
  _status = TipcMsgFieldUpdateReal4ArrayPtr(field, real4_array_data, real4_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a real8 array pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_REAL8 *real8_array_data, 
        T_INT4 real8_array_size)
{
  _status = TipcMsgFieldUpdateReal8ArrayPtr(field, real8_array_data, real8_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a real16 array pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_REAL16 *real16_array_data, 
        T_INT4 real16_array_size)
{
  _status = TipcMsgFieldUpdateReal16ArrayPtr(field, real16_array_data, real16_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a string pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_STR str_data) 
{
  _status = TipcMsgFieldUpdateStrPtr(field, str_data);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update a str array pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_STR *str_array_data, 
        T_INT4 str_array_size)
{
  _status = TipcMsgFieldUpdateStrArrayPtr(field, str_array_data, str_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdatePtr -- update an xml pointer field in a message */
T_BOOL TipcMsg::UpdatePtr(T_IPC_MSG_FIELD field, 
        T_XML xml_data) 
{
  _status = TipcMsgFieldUpdateXmlPtr(field, xml_data);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdateBoolPtr -- update a bool array pointer field in a message */
T_BOOL TipcMsg::UpdateBoolPtr(T_IPC_MSG_FIELD field, 
            T_BOOL *bool_array_data, 
            T_INT4 bool_array_size)
{
  _status = TipcMsgFieldUpdateBoolArrayPtr(field, bool_array_data, bool_array_size);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdateUtf8Ptr -- update a UTF-8 pointer field in a message */
T_BOOL TipcMsg::UpdateUtf8Ptr(T_IPC_MSG_FIELD field, 
            T_STR str_data) 
{
  _status = TipcMsgFieldUpdateUtf8Ptr(field, str_data);
  return _status;
}

/* ======================================================================== */
/*.. TipcMsg::UpdateUtf8Ptr -- update a UTF-8 array pointer field in a message*/
T_BOOL TipcMsg::UpdateUtf8Ptr(T_IPC_MSG_FIELD field, 
            T_STR *str_array_data, 
            T_INT4 str_array_size)
{
  _status = TipcMsgFieldUpdateUtf8ArrayPtr(field, str_array_data, str_array_size);
  return _status;
}

/* Append Unknown SMV 09/25/98 */
/* ======================================================================== */
/*..TipcMsg::AppendUnknown -- append a T_IPC_FT field to a message */
T_BOOL TipcMsg::AppendUnknown(T_IPC_FT field_type)
{
  _status = TipcMsgAppendUnknown(_msg, field_type);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::CurrentFieldKnown -- check if the current field of msg is known */
T_BOOL TipcMsg::CurrentFieldKnown(T_BOOL * field_known_return)
{
  _status = TipcMsgGetCurrentFieldKnown(_msg, field_known_return);
  return _status;
}

/* ---------------------------------------------------- */
/* Next functions which provide the C API look-and-feel */
/* ---------------------------------------------------- */

/* ======================================================================== */
/*..TipcMsg::Next -- get a binary field from a message */
T_BOOL TipcMsg::Next(T_PTR* binary_data, 
         T_INT4* binary_size)
{
  _status = TipcMsgNextBinary(_msg, binary_data, binary_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_CHAR field from a message */
T_BOOL TipcMsg::Next(T_CHAR* char_data)
{
  _status = TipcMsgNextChar(_msg, char_data);
  return _status;
}

/* Next T_GUID field YX 06/21/2000 */
/* ======================================================================== */
/*..TipcMsg::Next -- get a T_GUID field from a message */
T_BOOL TipcMsg::Next(T_GUID* guid_data)
{
  _status = TipcMsgNextGuid(_msg, guid_data);
  return _status;
}

/* Next T_GUID array field YX 06/21/2000 */
/* ======================================================================== */
/*..TipcMsg::Next -- get a T_GUID array field from a message */
T_BOOL TipcMsg::Next(T_GUID** guid_array_data, 
         T_INT4* guid_array_size)
{
  _status = TipcMsgNextGuidArray(_msg, guid_array_data, guid_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_INT2 field from a message */
T_BOOL TipcMsg::Next(T_INT2* int2_data)
{
  _status = TipcMsgNextInt2(_msg, int2_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_INT2 array field from a message */
T_BOOL TipcMsg::Next(T_INT2** int2_array_data, 
         T_INT4* int2_array_size)
{
  _status = TipcMsgNextInt2Array(_msg, int2_array_data, int2_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_INT4 field from a message */
T_BOOL TipcMsg::Next(T_INT4* int4_data)
{
  _status = TipcMsgNextInt4(_msg, int4_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_INT4 array field from a message */
T_BOOL TipcMsg::Next(T_INT4** int4_array_data, 
         T_INT4* int4_array_size)
{
  _status = TipcMsgNextInt4Array(_msg, int4_array_data, int4_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_INT8 field from a message */
T_BOOL TipcMsg::Next(T_INT8* int8_data)
{
  _status = TipcMsgNextInt8(_msg, int8_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_INT8 array field from a message */
T_BOOL TipcMsg::Next(T_INT8** int8_array_data, 
         T_INT4* int8_array_size)
{
  _status = TipcMsgNextInt8Array(_msg, 
                 int8_array_data,
                 int8_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a message and assign it to TipcMsg object */
T_BOOL TipcMsg::Next(TipcMsg* msg_data)
{
  T_IPC_MSG temp;

  if (!_problem) {
    _status = TipcMsgNextMsg(_msg, &temp);
    if (!_status) {
      return _status;
    }
    *msg_data = temp;
  }

  return _status;             
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a message array field from a message */
T_BOOL TipcMsg::Next(TipcMsg** msg_array_data, 
         T_INT4* msg_array_size)
{
  T_IPC_MSG *c_api_msg_array;

  _status = TipcMsgNextMsgArray(_msg, &c_api_msg_array, msg_array_size);

  if (_status) {
#ifndef T_OS_WINDOWS 
    *msg_array_data = new TipcMsg[*msg_array_size];
#else 
    if (*msg_array_size) {
      *msg_array_data = new TipcMsg[*msg_array_size];
    } else {
      *msg_array_data = new TipcMsg[1];
    }
#endif
    
    for (T_INT4 i = 0; i < *msg_array_size; i++ ) {
      /* ------------------------------------------------------- */
      /* Absorb the T_IPC_MSG array. Since elements of a message */
      /* array are read-only, we do not increment the reference  */
      /* count of the T_IPC_MSG array elements.                  */
      /* ------------------------------------------------------- */
      (*msg_array_data)[i].destroyMsg();
      (*msg_array_data)[i].absorb(c_api_msg_array[i]);
    }
  }

  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_REAL4 field from a message */
T_BOOL TipcMsg::Next(T_REAL4* real4_data)
{
  _status = TipcMsgNextReal4(_msg, real4_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_REAL4 array field from a message */
T_BOOL TipcMsg::Next(T_REAL4** real4_array_data, 
         T_INT4* real4_array_size)
{
  _status = TipcMsgNextReal4Array(_msg, 
                  real4_array_data,
                  real4_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_REAL8 field from a message */
T_BOOL TipcMsg::Next(T_REAL8* real8_data)
{
  _status = TipcMsgNextReal8(_msg, real8_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_REAL8 array field from a message */
T_BOOL TipcMsg::Next(T_REAL8** real8_array_data, 
         T_INT4* real8_array_size)
{
  _status = TipcMsgNextReal8Array(_msg, 
                  real8_array_data,
                  real8_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_REAL16 field from a message */
T_BOOL TipcMsg::Next(T_REAL16 *real16_data)
{
  _status = TipcMsgNextReal16(_msg, real16_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_REAL16 array field from a message */
T_BOOL TipcMsg::Next(T_REAL16** real16_array_data, 
         T_INT4* real16_array_size)
{
  _status = TipcMsgNextReal16Array(_msg, real16_array_data, real16_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_STR field from a message */
T_BOOL TipcMsg::Next(T_STR* str_data)
{
  _status = TipcMsgNextStr(_msg, str_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_STR array field from a message */
T_BOOL TipcMsg::Next(T_STR** str_array_data, 
         T_INT4* str_array_size)
{
  _status = TipcMsgNextStrArray(_msg, str_array_data, str_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_STR field and a T_REAL8 field from a message */
T_BOOL TipcMsg::Next(T_STR* str_data, 
         T_REAL8* real8_data)
{
  _status = TipcMsgNextStrReal8(_msg, str_data, real8_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a T_XML field from a message */
T_BOOL TipcMsg::Next(T_XML* xml_data)
{
  _status = TipcMsgNextXml(_msg, xml_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a byte field from a message */
T_BOOL TipcMsg::Next(T_UCHAR* byte_data)
{
  _status = TipcMsgNextByte(_msg, byte_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::NextBool -- get a T_BOOL field from a message */
T_BOOL TipcMsg::NextBool(T_BOOL* bool_data)
{
  _status = TipcMsgNextBool(_msg, bool_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::NextBool -- get a T_BOOL array field from a message */
T_BOOL TipcMsg::NextBool(T_BOOL** bool_array_data, 
             T_INT4* bool_array_size)
{
  _status = TipcMsgNextBoolArray(_msg, bool_array_data, bool_array_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::NextUtf8 -- get a UTF-8 field from a message */
T_BOOL TipcMsg::NextUtf8(T_STR* str_data)
{
  _status = TipcMsgNextUtf8(_msg, str_data);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Next -- get a UTF-8 array field from a message */
T_BOOL TipcMsg::NextUtf8(T_STR** str_array_data, 
             T_INT4* str_array_size)
{
  _status = TipcMsgNextUtf8Array(_msg, str_array_data, str_array_size);
  return _status;
}

/* Next Unknown SMV 09/25/98 */

/* ======================================================================== */
/*..TipcMsg::NextUnknown -- get a field from a message */
T_BOOL TipcMsg::NextUnknown()
{
  _status = TipcMsgNextUnknown(_msg);
  return _status;
}

/* ------------------------------------------------------------- */
/* Message printing functionality                                */
/* ------------------------------------------------------------- */

/* ======================================================================== */
/*..TipcMsg::Print -- print all information of a message */
T_BOOL TipcMsg::Print(T_OUT_FUNC func)
{
  _status = TipcMsgPrint(_msg, func);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::PrintError -- report an error about an unexpected message */
T_BOOL TipcMsg::PrintError()
{
  _status = TipcMsgPrintError(_msg);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::ResetCheck -- reset the _problem flag */
void TipcMsg::ResetCheck()
{
  _problem = T_FALSE;
}

/* ======================================================================== */
/*..TipcMsg::LbMode -- get load balancing mode */
T_IPC_LB_MODE TipcMsg::LbMode()
{
  T_IPC_LB_MODE lb_mode = T_IPC_LB_NONE;

  _status = TipcMsgGetLbMode(_msg, &lb_mode);
  return lb_mode;
}

/* ======================================================================== */
/*..TipcMsg::LbMode -- set load balancing mode */
T_BOOL TipcMsg::LbMode(T_IPC_LB_MODE lb_mode)
{
  _status = TipcMsgSetLbMode(_msg, lb_mode);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::HeaderStrEncode -- get header encode mode */
T_BOOL TipcMsg::HeaderStrEncode()
{
  T_BOOL header_encode = -1;

  _status = TipcMsgGetHeaderStrEncode(_msg, &header_encode);
  return header_encode;
}

/* ======================================================================== */
/*..TipcMsg::HeaderStrEncode -- set header encode mode */
T_BOOL TipcMsg::HeaderStrEncode(T_BOOL header_encode)
{
  _status = TipcMsgSetHeaderStrEncode(_msg, header_encode);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::Encrypt -- encrypt the given message */
T_IPC_MSG TipcMsg::Encrypt()
{
  T_IPC_MSG encrypt_msg;
  encrypt_msg = TipcMsgEncrypt(_msg);
  return encrypt_msg;
}

/* ======================================================================== */
/*..TipcMsg::Decrypt -- decrypt the given message */
T_IPC_MSG TipcMsg::Decrypt()
{
  T_IPC_MSG decrypt_msg;
  decrypt_msg = TipcMsgDecrypt(_msg);
  return decrypt_msg;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named binary field to the message */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_PTR value, 
       T_INT4 value_size) 
{
  _status = TipcMsgAddNamedBinary(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named binary ptr field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_PTR value, 
       T_INT4 value_size, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedBinaryPtr(_msg, name, value, value_size, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named char field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_CHAR value) 
{
  _status = TipcMsgAddNamedChar(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named GUID field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_GUID value) 
{
  _status = TipcMsgAddNamedGuid(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named GUID array field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_GUID *value, 
       T_INT4 value_size) 
{
  _status = TipcMsgAddNamedGuidArray(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add named GUID array ptr field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_GUID *value, 
       T_INT4 value_size, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedGuidArrayPtr(_msg, name, value, value_size, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named int2 field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_INT2 value) 
{
  _status = TipcMsgAddNamedInt2(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named int2 array field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_INT2 *value, 
       T_INT4 value_size) 
{
  _status = TipcMsgAddNamedInt2Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add named int2 array ptr field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_INT2 *value, 
       T_INT4 value_size, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedInt2ArrayPtr(_msg, name, value, value_size, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named int4 field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_INT4 value) 
{
  _status = TipcMsgAddNamedInt4(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named int4 array field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_INT4 *value, 
       T_INT4 value_size) 
{
  _status = TipcMsgAddNamedInt4Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add named int4 array ptr field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_INT4 *value, 
       T_INT4 value_size, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedInt4ArrayPtr(_msg, name, value, value_size, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named int8 field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_INT8 value) 
{
  _status = TipcMsgAddNamedInt8(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named int8 array field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_INT8 *value, 
       T_INT4 value_size) 
{
  _status = TipcMsgAddNamedInt8Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add named int8 array ptr field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_INT8 *value, 
       T_INT4 value_size, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedInt8ArrayPtr(_msg, name, value, value_size, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named msg field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_IPC_MSG value) 
{
  _status = TipcMsgAddNamedMsg(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named msg array field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_IPC_MSG *value, 
       T_INT4 value_size) 
{
  _status = TipcMsgAddNamedMsgArray(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add named msg array ptr field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name,
       T_IPC_MSG *value, 
       T_INT4 value_size, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedMsgArrayPtr(_msg, name, value, value_size, field_return);
  return _status;
}

/* YX 06/21/2000 */
/* ======================================================================== */
/*..TipcMsg::AddNamed -- add named msg ptr field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_IPC_MSG value, 
       T_IPC_MSG_FIELD *field_return)
{
  _status = TipcMsgAddNamedMsgPtr(_msg, name, value, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named real4 field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_REAL4 value) 
{
  _status = TipcMsgAddNamedReal4(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named real4 array field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_REAL4 *value, 
       T_INT4 value_size) 
{
  _status = TipcMsgAddNamedReal4Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add named real4 array ptr field to msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_REAL4 *value,
       T_INT4 value_size, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedReal4ArrayPtr
         (_msg, name, value, value_size, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named real8 field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_REAL8 value) 
{
  _status = TipcMsgAddNamedReal8(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named real8 array field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_REAL8 *value, 
       T_INT4 value_size) 
{
  _status = TipcMsgAddNamedReal8Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add named real8 array ptr field to msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_REAL8 *value, 
       T_INT4 value_size, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedReal8ArrayPtr(_msg, name, value, value_size, 
           field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named real16 field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_REAL16 value) 
{
  _status = TipcMsgAddNamedReal16(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named real16 array field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_REAL16 *value,
       T_INT4 value_size) 
{
  _status = TipcMsgAddNamedReal16Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add named real16 array ptr field to msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_REAL16 *value, 
       T_INT4 value_size, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedReal16ArrayPtr(_msg, name, value, value_size, 
            field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named str field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_STR value) 
{
  _status = TipcMsgAddNamedStr(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named str array field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_STR *value, 
       T_INT4 value_size) 
{
  _status = TipcMsgAddNamedStrArray(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named str ptr field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_STR value, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedStrPtr(_msg, name, value, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add named str array ptr field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_STR *value, 
       T_INT4 value_size, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedStrArrayPtr(_msg, name, value, value_size, 
               field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named xml field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_XML value) 
{
  _status = TipcMsgAddNamedXml(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named xml ptr field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_XML value, 
       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedXmlPtr(_msg, name, value, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamed -- add a named byte field to the msg */
T_BOOL TipcMsg::AddNamed(T_STR name, 
       T_UCHAR value) 
{
  _status = TipcMsgAddNamedByte(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamedBool -- add a named bool field to the msg */
T_BOOL TipcMsg::AddNamedBool(T_STR name, 
           T_BOOL value) 
{
  _status = TipcMsgAddNamedBool(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamedBool -- add a named bool array field to the msg */
T_BOOL TipcMsg::AddNamedBool(T_STR name, 
           T_BOOL *value, 
           T_INT4 value_size) 
{
  _status = TipcMsgAddNamedBoolArray(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamedBool -- add named bool array ptr field to the msg */
T_BOOL TipcMsg::AddNamedBool(T_STR name, 
           T_BOOL *value, 
           T_INT4 value_size, 
           T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedBoolArrayPtr(_msg, name, value, value_size, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamedUtf8 -- add a named UTF-8 field to the msg */
T_BOOL TipcMsg::AddNamedUtf8(T_STR name, 
           T_STR value) 
{
  _status = TipcMsgAddNamedUtf8(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamedUtf8 -- add a named UTF-8 array field to the msg */
T_BOOL TipcMsg::AddNamedUtf8(T_STR name, 
           T_STR *value, 
           T_INT4 value_size) 
{
  _status = TipcMsgAddNamedUtf8Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamedUtf8 -- add a named UTF_8 ptr field to the msg */
T_BOOL TipcMsg::AddNamedUtf8(T_STR name, 
           T_STR value, 
           T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedUtf8Ptr(_msg, name, value, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamedUtf8 -- add named UTF-8 array ptr field to the msg */
T_BOOL TipcMsg::AddNamedUtf8(T_STR name, 
           T_STR *value, 
           T_INT4 value_size, 
           T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgAddNamedUtf8ArrayPtr(_msg, name, value, value_size, 
                field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::AddNamedUnknown -- add named unknown field to the msg */
T_BOOL TipcMsg::AddNamedUnknown(T_STR name, 
        T_INT4 type) 
{
  _status = TipcMsgAddNamedUnknown(_msg, name, type);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named binary field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_PTR *value, 
       T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedBinary(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named char field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_CHAR *value) 
{
  _status = TipcMsgGetNamedChar(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named GUID field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_GUID *value) 
{
  _status = TipcMsgGetNamedGuid(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named GUID array field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_GUID **value, 
       T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedGuidArray(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named int2 field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_INT2 *value) 
{
  _status = TipcMsgGetNamedInt2(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named int2 array field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_INT2 **value, 
       T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedInt2Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named int4 field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_INT4 *value) 
{
  _status = TipcMsgGetNamedInt4(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named int4 array field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_INT4 **value, 
       T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedInt4Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named int8 field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_INT8 *value) 
{
  _status = TipcMsgGetNamedInt8(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named int8 array field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_INT8 **value, 
       T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedInt8Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named msg field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_IPC_MSG *value) 
{
  _status = TipcMsgGetNamedMsg(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named msg array field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_IPC_MSG **value, 
       T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedMsgArray(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named real4 field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_REAL4 *value) 
{
  _status = TipcMsgGetNamedReal4(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named real4 array field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_REAL4 **value, 
       T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedReal4Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named real8 field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_REAL8 *value) 
{
  _status = TipcMsgGetNamedReal8(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named real8 array field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_REAL8 **value, 
       T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedReal8Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named real16 field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_REAL16 *value) 
{
  _status = TipcMsgGetNamedReal16(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named real16 array field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_REAL16 **value, 
       T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedReal16Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named str field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_STR *value) 
{
  _status = TipcMsgGetNamedStr(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named str array field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_STR **value, 
       T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedStrArray(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named xml field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_XML *value) 
{
  _status = TipcMsgGetNamedXml(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamed -- get a named byte field from the msg */
T_BOOL TipcMsg::GetNamed(T_STR name, 
       T_UCHAR *value) 
{
  _status = TipcMsgGetNamedByte(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamedBool -- get a named bool field from the msg */
T_BOOL TipcMsg::GetNamedBool(T_STR name, 
           T_BOOL *value) 
{
  _status = TipcMsgGetNamedBool(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamedBool -- get a named bool array field from the msg */
T_BOOL TipcMsg::GetNamedBool(T_STR name, 
           T_BOOL **value, 
           T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedBoolArray(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamedUtf8 -- get a named UTF-8 field from the msg */
T_BOOL TipcMsg::GetNamedUtf8(T_STR name, 
           T_STR *value) 
{
  _status = TipcMsgGetNamedUtf8(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamedUtf8 -- get a named UTF-8 array field from the msg */
T_BOOL TipcMsg::GetNamedUtf8(T_STR name, 
           T_STR **value, 
           T_INT4 *value_size) 
{
  _status = TipcMsgGetNamedUtf8Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamedUnknown -- get a named unknown field from the msg */
T_BOOL TipcMsg::GetNamedUnknown(T_STR name) 
{
  _status = TipcMsgGetNamedUnknown(_msg, name);
  return _status;
}
  
/* ======================================================================== */
/*..TipcMsg::GetTypeNamed -- get the type of a named field from the msg */
T_BOOL TipcMsg::GetTypeNamed(T_STR name, 
           T_IPC_FT *type) 
{
  _status = TipcMsgGetTypeNamed(_msg, name, type);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::ExistsNamed -- get whether or not a named field exists in msg */
T_BOOL TipcMsg::ExistsNamed(T_STR name, 
          T_BOOL *bool_return) 
{
  _status = TipcMsgExistsNamed(_msg, name, bool_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::GetNamedCurrent -- get the name of the current field */
T_BOOL TipcMsg::GetNameCurrent(T_STR *name) 
{
  _status = TipcMsgGetNameCurrent(_msg, name);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::SetNameCurrent -- set the name of the current field */
T_BOOL TipcMsg::SetNameCurrent(T_STR name) 
{
  _status = TipcMsgSetNameCurrent(_msg, name);
  return _status;
}

/* Added for SS5.6 Feature YX 06/10/00 */
/* ======================================================================== */
/*..TipcMsg::DeleteCurrent -- delete the current field in a message. */
T_BOOL TipcMsg::DeleteCurrent() 
{
 _status = TipcMsgDeleteCurrent(_msg);
 return _status;
}

/* ======================================================================== */
/*..TipcMsg::DeleteField -- delete the specified field in a message. */
T_BOOL TipcMsg::DeleteField(T_INT4 index) 
{
 _status = TipcMsgDeleteField(_msg, index);
 return _status;
}

/* ======================================================================== */
/*..TipcMsg::DeleteNamedField -- delete the named field in a message. */
T_BOOL TipcMsg::DeleteNamedField(T_STR name) 
{
 _status = TipcMsgDeleteNamedField(_msg, name);
 return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update a named binary field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_PTR value, 
          T_INT4 value_size) 
{
  _status = TipcMsgUpdateNamedBinary(_msg, name, value, value_size); 
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update a named binary ptr field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_PTR value, 
          T_INT4 value_size, 
          T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgUpdateNamedBinaryPtr(_msg, name, value, value_size, 
          field_return); 
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update a named char field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_CHAR value) 
{
  _status = TipcMsgUpdateNamedChar(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update a named GUID field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_GUID value) 
{
  _status = TipcMsgUpdateNamedGuid(_msg, name, value);
  return _status;
}
  
/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update a named GUID array field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_GUID *value, 
          T_INT4 value_size) 
{
  _status = TipcMsgUpdateNamedGuidArray(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update named GUID array ptr field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_GUID *value, 
          T_INT4 value_size, 
          T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgUpdateNamedGuidArrayPtr(_msg, name, value, value_size, 
             field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update a named int2 field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_INT2 value) 
{
  _status = TipcMsgUpdateNamedInt2(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update a named int2 array field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_INT2 *value, 
          T_INT4 value_size) 
{
  _status = TipcMsgUpdateNamedInt2Array(_msg, name, value, value_size);
  return _status;
} 
 
/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named int2 array ptr field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_INT2 *value, 
          T_INT4 value_size, 
                       T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgUpdateNamedInt2ArrayPtr(_msg, name, value, value_size, 
             field_return);
  return _status;
}

 
/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update named int4 field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_INT4 value) 
{
  _status = TipcMsgUpdateNamedInt4(_msg, name, value);
  return _status;
}
 
/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named int4 array field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_INT4 *value, 
          T_INT4 value_size) 
{
  _status = TipcMsgUpdateNamedInt4Array(_msg, name, value, value_size);
  return _status;
}
 
/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named int4 array ptr field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_INT4 *value, 
          T_INT4 value_size, 
          T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgUpdateNamedInt4ArrayPtr(_msg, name, value, value_size, 
             field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update named int8 field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_INT8 value) 
{
  _status = TipcMsgUpdateNamedInt8(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named int8 array field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_INT8 *value, 
          T_INT4 value_size) 
{
  _status = TipcMsgUpdateNamedInt8Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named int8 array ptr field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_INT8 *value, 
          T_INT4 value_size, 
          T_IPC_MSG_FIELD *field_return) 
{          
  _status = TipcMsgUpdateNamedInt8ArrayPtr(_msg, name, value, value_size, 
             field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update named msg field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_IPC_MSG value) 
{         
  _status = TipcMsgUpdateNamedMsg(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named msg array field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_IPC_MSG *value, 
          T_INT4 value_size) 
{
  _status = TipcMsgUpdateNamedMsgArray(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named msg array ptr field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_IPC_MSG *value, 
          T_INT4 value_size, 
          T_IPC_MSG_FIELD *field_return) 
{            
  _status = TipcMsgUpdateNamedMsgArrayPtr(_msg, name, value, value_size, 
            field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update named msg ptr field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_IPC_MSG value, 
          T_IPC_MSG_FIELD *field_return) 
{             
  _status = TipcMsgUpdateNamedMsgPtr(_msg, name, value, 
             field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update named real4 field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, T_REAL4 value) 
{                
  _status = TipcMsgUpdateNamedReal4(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named real4 array field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_REAL4 *value, 
          T_INT4 value_size) 
{  
  _status = TipcMsgUpdateNamedReal4Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named real4 array ptr field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_REAL4 *value, 
          T_INT4 value_size, 
          T_IPC_MSG_FIELD *field_return) 
{            
  _status = TipcMsgUpdateNamedReal4ArrayPtr(_msg, name, value, value_size, 
              field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update named real8 field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_REAL8 value) 
{               
  _status = TipcMsgUpdateNamedReal8(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update a named real8 array field in the msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_REAL8 *value, 
          T_INT4 value_size) 
{  
  _status = TipcMsgUpdateNamedReal8Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named real8 array ptr field in the msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_REAL8 *value, 
          T_INT4 value_size,    
          T_IPC_MSG_FIELD *field_return) 
{            
  _status = TipcMsgUpdateNamedReal8ArrayPtr(_msg, name, value, value_size, 
              field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update a named real16 field in the msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_REAL16 value) 
{               
  _status = TipcMsgUpdateNamedReal16(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named real16 array field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_REAL16 *value, 
          T_INT4 value_size) 
{
  _status = TipcMsgUpdateNamedReal16Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named real16 array ptr field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_REAL16 *value, 
          T_INT4 value_size, 
          T_IPC_MSG_FIELD *field_return) 
{           
  _status = TipcMsgUpdateNamedReal16ArrayPtr(_msg, name, value, value_size, 
               field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update named str field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_STR value) 
{                
  _status = TipcMsgUpdateNamedStr(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named str array field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_STR *value, 
          T_INT4 value_size) 
{   
  _status = TipcMsgUpdateNamedStrArray(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update named str array ptr field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_STR *value, 
          T_INT4 value_size, 
          T_IPC_MSG_FIELD *field_return) 
{          
  _status = TipcMsgUpdateNamedStrArrayPtr(_msg, name, value, value_size, 
            field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update a named str ptr field in the msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_STR value, 
          T_IPC_MSG_FIELD *field_return) 
{            
  _status = TipcMsgUpdateNamedStrPtr(_msg, name, value, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update a named xml field in the msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_XML value) 
{                 
  _status = TipcMsgUpdateNamedXml(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- update a named xml ptr field in the msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_XML value,
          T_IPC_MSG_FIELD *field_return) 
{           
  _status = TipcMsgUpdateNamedXmlPtr(_msg, name, value, field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamed -- Update named byte field in a msg */
T_BOOL TipcMsg::UpdateNamed(T_STR name, 
          T_UCHAR value) 
{
  _status = TipcMsgUpdateNamedByte(_msg, name, value);
  return _status;
}
 
/* ======================================================================== */
/*..TipcMsg::UpdateNamedBool -- Update named bool field in a msg */
T_BOOL TipcMsg::UpdateNamedBool(T_STR name, 
              T_BOOL value) 
{
  _status = TipcMsgUpdateNamedBool(_msg, name, value);
  return _status;
}
 
/* ======================================================================== */
/*..TipcMsg::UpdateNamedBool -- update named bool array field in a msg */
T_BOOL TipcMsg::UpdateNamedBool(T_STR name, 
              T_BOOL *value, 
              T_INT4 value_size) 
{
  _status = TipcMsgUpdateNamedBoolArray(_msg, name, value, value_size);
  return _status;
}
 
/* ======================================================================== */
/*..TipcMsg::UpdateNamedBool -- update named bool array ptr field in a msg */
T_BOOL TipcMsg::UpdateNamedBool(T_STR name, 
              T_BOOL *value, 
              T_INT4 value_size, 
              T_IPC_MSG_FIELD *field_return) 
{
  _status = TipcMsgUpdateNamedBoolArrayPtr(_msg, name, value, value_size, 
             field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamedUtf8 -- Update named UTF-8 field in a msg */
T_BOOL TipcMsg::UpdateNamedUtf8(T_STR name, 
              T_STR value) 
{                
  _status = TipcMsgUpdateNamedUtf8(_msg, name, value);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamedUtf8 -- update named UTF-8 array field in a msg */
T_BOOL TipcMsg::UpdateNamedUtf8(T_STR name, 
              T_STR *value, 
              T_INT4 value_size) 
{   
  _status = TipcMsgUpdateNamedUtf8Array(_msg, name, value, value_size);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamedUtf8 -- update named UTF-8 array ptr field in a msg */
T_BOOL TipcMsg::UpdateNamedUtf8(T_STR name, 
              T_STR *value, 
              T_INT4 value_size, 
              T_IPC_MSG_FIELD *field_return) 
{          
  _status = TipcMsgUpdateNamedUtf8ArrayPtr(_msg, name, value, value_size, 
             field_return);
  return _status;
}

/* ======================================================================== */
/*..TipcMsg::UpdateNamedUtf8 -- update a named UTF-8 ptr field in the msg */
T_BOOL TipcMsg::UpdateNamedUtf8(T_STR name, 
              T_STR value, 
              T_IPC_MSG_FIELD *field_return) 
{            
  _status = TipcMsgUpdateNamedUtf8Ptr(_msg, name, value, field_return);
  return _status;
}


