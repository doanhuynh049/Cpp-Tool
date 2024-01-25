/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/c/burst/burst.c#1 $
 */

#include "includes.h"


#define BURST_MSG_INFO_SIZE 3

#define T_MT_BURST_NAME "burst"
#define T_MT_BURST_GRAMMAR "verbose"

typedef struct T_BURST_FILTER_CLOSURE_STRUCT T_BURST_FILTER_CLOSURE_STRUCT,
*T_BURST_FILTER_CLOSURE;
struct T_BURST_FILTER_CLOSURE_STRUCT {
  T_BURST_FILTER_FUNC burst_filter_func;
  T_PTR burst_filter_arg;
  T_PTR srv_ptr;  /* srv ptr */
};

typedef struct T_BURST_MSG_FAILURE_INF_STRUCT T_BURST_MSG_FAILURE_INF_STRUCT,
*T_BURST_MSG_FAILURE_INF;
struct T_BURST_MSG_FAILURE_INF_STRUCT {
  T_INT4  err_num;
  T_REAL8 send_time;
  T_STR   proc_name;
  T_PTR   arg;
  T_PTR   srv_ptr;
};

typedef struct T_BURST_FLUSH_INFO_CLOSURE_STRUCT T_BURST_FLUSH_INFO_CLOSURE_STRUCT,
*T_BURST_FLUSH_INFO_CLOSURE;
struct T_BURST_FLUSH_INFO_CLOSURE_STRUCT {
  T_INT4 auto_flush_count;
  T_PTR  srv_ptr;
};

typedef struct T_BURST_PTR_INFO_STRUCT T_BURST_PTR_INFO_STRUCT,
*T_BURST_PTR_INFO;
struct T_BURST_PTR_INFO_STRUCT {
  T_IPC_BURST burst;
  T_IPC_SRV   srv;
};

/* ========================================================================= */
/*..ipcBurstInitMsgTypes -- create the burst message types BURST  */
static void T_ENTRY ipcBurstInitMsgTypes
(
 void
)
{
  T_BOOL result = T_FALSE;

  T_STATIC_VAR(T_BOOL initialized = T_FALSE);

  /* one time initilization of the mt types */
  if (T_FALSE == initialized) {

    T_IPC_MT burst_mt = T_NULL;
    T_IPC_MT message_fields_mt = T_NULL;

    burst_mt = TipcMtCreate (T_MT_BURST_NAME,   
                             T_MT_BURST,
                             T_MT_BURST_GRAMMAR);
    if (T_NULL == burst_mt) {
      initialized = T_FALSE;
      return;
    }
    result = TipcMtLogAddMt((T_IPC_MT_LOG_TYPE)T_IPC_SRV_LOG_INTERNAL, 
                            burst_mt);
    if (T_FALSE == result) {
      initialized = T_FALSE;
      return;
    }
    initialized = T_TRUE;
  }
} 

/* ========================================================================= */
/*..ipcBurstAppendMsgHeader -- add minimal info to the BURST message */
static T_BOOL T_ENTRY ipcBurstAppendMsgHeader
(
T_IPC_BURST burst,
T_IPC_MSG msg
)
{
  T_INT4 msg_type;
  T_INT4 number_of_fields;
  T_INT4 user_prop;
  T_BOOL status;
  T_INT4 burst_msg_info_start_index;
  T_IPC_MT mt;

  T_CHECK_NULL(burst, T_FALSE)
  T_CHECK_NULL(msg, T_FALSE)

  status = TipcMsgGetType(msg, &mt);
  T_EXPECT(T_TRUE == status);

  status = TipcMtGetNum(mt, &msg_type);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgGetNumFields(msg, &number_of_fields);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgGetUserProp(msg, &user_prop);
  T_EXPECT(T_TRUE == status);

  burst_msg_info_start_index = burst->info_field_size - BURST_MSG_INFO_SIZE;

  /* Fill up the info_field_array with the minimal information */
  burst->info_field_array[burst_msg_info_start_index + 0] = msg_type;
  burst->info_field_array[burst_msg_info_start_index + 1] = number_of_fields;
  burst->info_field_array[burst_msg_info_start_index + 2] = user_prop;

  return status;

} 


/* ========================================================================= */
/*..ipcBurstAppendMsgFields -- pulls msgs from srcMsg and puts into destMsg */
static T_BOOL T_ENTRY ipcBurstAppendMsgFields
(
T_IPC_MSG destMsg, 
T_IPC_MSG srcMsg,
T_INT4 startingFieldIndex,
T_INT4 numberOfFields
)
{
  T_INT4 num_fields_of_src_msg;
  T_INT4 counter; 
  T_BOOL status;

  T_CHECK_NULL(destMsg, T_FALSE)
  T_CHECK_NULL(srcMsg, T_FALSE)

  /* set current field of burst message to the last one. */
  status = TipcMsgGetNumFields(srcMsg, &num_fields_of_src_msg);
  T_EXPECT(T_TRUE == status);

  T_CHECK_EXPR(numberOfFields <= num_fields_of_src_msg, T_FALSE)

  status = TipcMsgSetCurrent(srcMsg, startingFieldIndex);
  T_EXPECT(T_TRUE == status);

  for (counter = 0; counter < numberOfFields; counter++) {

    T_IPC_FT fieldType;
    T_CHECK_EXPR(TipcMsgNextType(srcMsg, (T_IPC_FT * )&fieldType), T_FALSE)

    switch (fieldType) {
      
      case T_IPC_FT_INVALID:
        {
          return T_FALSE;
        }
      case T_IPC_FT_CHAR:
        {
          T_CHAR charField;

          T_CHECK_EXPR(TipcMsgNextChar(srcMsg, &charField), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendChar(destMsg, charField),T_FALSE)
        }
        break;
      case T_IPC_FT_BINARY:
        {
          T_PTR binaryField;
          T_INT4 size;

          T_CHECK_EXPR(TipcMsgNextBinary(srcMsg, &binaryField, &size), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendBinary(destMsg, binaryField, size), 
                       T_FALSE)
        }
        break;
      case T_IPC_FT_STR:
        {
          T_STR stringField;

          T_CHECK_EXPR(TipcMsgNextStr(srcMsg, &stringField), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendStr(destMsg, stringField), T_FALSE)
        }
        break;
      case T_IPC_FT_STR_ARRAY:
        {
          T_STR* stringArrayField;
          T_INT4 size;

          T_CHECK_EXPR(TipcMsgNextStrArray(srcMsg, &stringArrayField, &size), 
                       T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendStrArray(destMsg, stringArrayField, size),
                       T_FALSE)
        }
        break;
      case T_IPC_FT_INT2:
        {
          T_INT2 int2Field;

          T_CHECK_EXPR(TipcMsgNextInt2(srcMsg, &int2Field), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendInt2(destMsg, int2Field),T_FALSE)
        }
        break;
      case T_IPC_FT_INT2_ARRAY:
        {
          T_INT2* int2ArrayField;
          T_INT4 size;

          T_CHECK_EXPR(TipcMsgNextInt2Array(srcMsg, &int2ArrayField, &size),
                       T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendInt2Array(destMsg, int2ArrayField, size),
                       T_FALSE)
        }
        break;
      case T_IPC_FT_INT4:
        {
          T_INT4 int4Field;

          T_CHECK_EXPR(TipcMsgNextInt4(srcMsg, &int4Field), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendInt4(destMsg, int4Field), T_FALSE)
        }
        break;
      case T_IPC_FT_INT4_ARRAY:
        {
          T_INT4* int4ArrayField;
          T_INT4 size;

          T_CHECK_EXPR(TipcMsgNextInt4Array(srcMsg, &int4ArrayField, &size),
                       T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendInt4Array(destMsg, int4ArrayField, size),
                       T_FALSE)
        }
        break;
      case T_IPC_FT_INT8:
        {
          T_INT8 int8Field;

          T_CHECK_EXPR(TipcMsgNextInt8(srcMsg, &int8Field), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendInt8(destMsg, int8Field), T_FALSE)
        }
        break;
      case T_IPC_FT_INT8_ARRAY:
        {
          T_INT8* int8ArrayField;
          T_INT4 size;

          T_CHECK_EXPR(TipcMsgNextInt8Array(srcMsg, &int8ArrayField, &size), 
                       T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendInt8Array(destMsg, int8ArrayField, size),
                       T_FALSE)
        }
        break;
      case T_IPC_FT_REAL4:
        {
          T_REAL4 real4Field;

          T_CHECK_EXPR(TipcMsgNextReal4(srcMsg, &real4Field), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendReal4(destMsg, real4Field),T_FALSE)
        }
        break;
      case T_IPC_FT_REAL4_ARRAY:
        {
          T_REAL4* real4ArrayField;
          T_INT4 size;

          T_CHECK_EXPR(TipcMsgNextReal4Array(srcMsg, &real4ArrayField, &size),
                       T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendReal4Array(destMsg, real4ArrayField, size),
                       T_FALSE)
        }
        break;
      case T_IPC_FT_REAL8:
        {
          T_REAL8 real8Field;

          T_CHECK_EXPR(TipcMsgNextReal8(srcMsg, &real8Field), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendReal8(destMsg, real8Field),T_FALSE)
        }
        break;
      case T_IPC_FT_REAL8_ARRAY:
        {
          T_REAL8* real8ArrayField;
          T_INT4 size;

          T_CHECK_EXPR(TipcMsgNextReal8Array(srcMsg, &real8ArrayField, &size),
                       T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendReal8Array(destMsg, real8ArrayField, size),
                       T_FALSE)
        }
        break;
      case T_IPC_FT_REAL16:
        {
          T_REAL16 real16Field;

          T_CHECK_EXPR(TipcMsgNextReal16(srcMsg, &real16Field), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendReal16(destMsg, real16Field), T_FALSE)
        }
        break;
      case T_IPC_FT_REAL16_ARRAY:
        {
          T_REAL16* real16ArrayField;
          T_INT4 size;

          T_CHECK_EXPR(TipcMsgNextReal16Array(srcMsg, &real16ArrayField, &size)
                       , T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendReal16Array(destMsg, real16ArrayField, 
                                             size), T_FALSE)
        }
        break;
      case T_IPC_FT_MSG:
        {
          /* adding support for msg in messages. BM 04/07/98 */
          T_IPC_MSG ipcMsg;

          T_CHECK_EXPR(TipcMsgNextMsg(srcMsg, &ipcMsg), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendMsg(destMsg, ipcMsg), T_FALSE)
        }
        break;
      case T_IPC_FT_MSG_ARRAY:
        {
          /* adding support for msg in messages BM 04/07/98 */
          T_IPC_MSG *ipcMsg_array;
          T_INT4 array_size;

          T_CHECK_EXPR(TipcMsgNextMsgArray(srcMsg, &ipcMsg_array, &array_size),
                       T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendMsgArray(destMsg,ipcMsg_array, array_size),
                       T_FALSE)
        }
        break;
      case T_IPC_FT_GUID:
        {
          T_GUID guid_return;
          T_CHECK_EXPR(TipcMsgNextGuid(srcMsg, &guid_return), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendGuid(destMsg, guid_return), T_FALSE)

        }
        break;
      case T_IPC_FT_GUID_ARRAY:
        {
          T_GUID *guid_array_return;
          T_INT4 array_size;
          T_CHECK_EXPR(TipcMsgNextGuidArray(srcMsg, &guid_array_return, 
                                         &array_size), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendGuidArray(destMsg, guid_array_return,
                                           array_size), T_FALSE)
        }
        break;
      case T_IPC_FT_XML:
        {
          T_XML xml_return;
          T_CHECK_EXPR(TipcMsgNextXml(srcMsg, &xml_return), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendXml(destMsg, xml_return), T_FALSE)
        }
        break;
      case T_IPC_FT_UTF8:
        {
          T_STR utf8_str;
          T_CHECK_EXPR(TipcMsgNextUtf8(srcMsg, &utf8_str), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendUtf8(destMsg, utf8_str), T_FALSE)
        }
        break;
      case T_IPC_FT_UTF8_ARRAY:
        {
          T_STR *utf8_array_return;
          T_INT4 array_size;
          T_CHECK_EXPR(TipcMsgNextUtf8Array(srcMsg, &utf8_array_return, 
                                         &array_size), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendUtf8Array(destMsg, utf8_array_return,
                                           array_size), T_FALSE)
        }
        break;
      case T_IPC_FT_BOOL:
        {
          T_BOOL bool_return;
          T_CHECK_EXPR(TipcMsgNextBool(srcMsg, &bool_return), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendBool(destMsg, bool_return), T_FALSE)
        }
        break;
      case T_IPC_FT_BOOL_ARRAY:
        {
          T_BOOL *bool_array_return;
          T_INT4 array_size;
          T_CHECK_EXPR(TipcMsgNextBoolArray(srcMsg, &bool_array_return, &array_size), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendBoolArray(srcMsg, bool_array_return, array_size), T_FALSE)
        }
        break;
      case T_IPC_FT_BYTE:
        {
          T_UCHAR byte_return;
          T_CHECK_EXPR(TipcMsgNextByte(srcMsg, &byte_return), T_FALSE)
          T_CHECK_EXPR(TipcMsgAppendByte(destMsg, byte_return), T_FALSE)
        }
        break;
      default:
        {
          TutErrNumSet(T_ERR_VAL_INVALID);
          return T_FALSE;
        }
    } 
  } 

  return T_TRUE;
}


/* ========================================================================= */
/*..ipcBurstAppendAllMsgFields -- to copy from src to dest */
static T_BOOL T_ENTRY ipcBurstAppendAllMsgFields
(
T_IPC_MSG destMsg,
T_IPC_MSG srcMsg
)
{
  T_INT4 num_fields_of_src_msg; 
  T_BOOL status;

  T_CHECK_NULL(destMsg, T_FALSE)
  T_CHECK_NULL(srcMsg, T_FALSE)

  status = TipcMsgGetNumFields(srcMsg, &num_fields_of_src_msg);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgSetCurrent(srcMsg, 0);
  T_EXPECT(T_TRUE == status);

  status = ipcBurstAppendMsgFields(destMsg, srcMsg, 0, num_fields_of_src_msg);
  T_EXPECT(T_TRUE == status);

  return status;
} 


/* ========================================================================= */
/*..ipcBurstAppendMsg -- puts the message into on big BURST MESSAGE */
static T_BOOL T_ENTRY ipcBurstAppendMsg
(
T_IPC_BURST burst, 
T_IPC_MSG msg
)
{
  T_BOOL status;

  T_CHECK_NULL(burst, T_FALSE)
  T_CHECK_NULL(msg, T_FALSE)

  (burst->info_field_size) += (BURST_MSG_INFO_SIZE);

  /*
   * Add the minimal information to burst and also append all
   * fields to the burst->msg (BM 04/24/98)
   */
  status = ipcBurstAppendMsgHeader(burst, msg);
  status &= ipcBurstAppendAllMsgFields(burst->msg, msg);
  T_EXPECT(T_TRUE == status);

  return status;

} 


/* ========================================================================= */
/*...TipcBurstMsgProcessCb pulls out the coallesced messages */
static void T_ENTRY TipcBurstMsgProcessCb 
(
T_IPC_CONN conn, 
T_IPC_CONN_PROCESS_CB_DATA data, 
T_CB_ARG arg
)
{
  T_IPC_MSG burst_msg; 
  T_INT4* info_field_array = T_NULL;
  T_INT4 info_field_size;
  T_INT4 numb_of_bursted_msgs = 0;
  T_INT4 counter;
  T_BOOL status;
  T_STR dest;
  T_STR sender;
  T_BOOL valid_destination = T_TRUE;
  T_INT4 msg_fields_offset = 0;
  T_INT4 num_fields;
  T_REAL8 sender_timestamp = 0.0;
  T_REAL8 arrival_timestamp = 0.0;
  T_BOOL sender_timestamp_set = T_FALSE;


  /* assign some values */
  T_BURST_FILTER_CLOSURE closure = (T_BURST_FILTER_CLOSURE)arg;
  T_BURST_FILTER_FUNC burst_filter_func = closure->burst_filter_func;
  T_PTR burst_filter_arg = closure->burst_filter_arg;
  T_IPC_SRV srv = (T_IPC_SRV)closure->srv_ptr;

  burst_msg = data->msg;
  T_ASSERT(burst_msg != NULL);

  status = TipcMsgGetNumFields(burst_msg, &num_fields);
  T_EXPECT(T_TRUE == status);

  /* set the current field of the message 0 indicates first field */
  status = TipcMsgSetCurrent(burst_msg, num_fields - 1);
  T_EXPECT(T_TRUE == status);

  /* Get the Int4 array from the burst message */
  status =  TipcMsgNextInt4Array(burst_msg, &info_field_array, 
                              &info_field_size);
  T_EXPECT(T_TRUE == status);

  /* The number of burst messages would then be the infofield size 
   * which gets incremented each time a msg is appended to the burst 
   * message. So it's a multiple of 3. so this divided by
   * the constant BURST_MSG_INFO_SIZE gives the numberof BurstedMsgs
   */
  numb_of_bursted_msgs = info_field_size / BURST_MSG_INFO_SIZE;

  status = TipcMsgGetDest(burst_msg, &dest);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgGetSender(burst_msg, &sender);
  T_EXPECT(T_TRUE == status);

  if (T_NULL == srv) {
    TipcSrvSubjectGetSubscribe(dest, &valid_destination);
  }
  else {
    TipcSrvConnSubjectGetSubscribe(srv, dest, &valid_destination);
  }
  if (T_FALSE == valid_destination) {
    TipcMsgSetDest(data->msg, '\0');
  } 

  status = TipcMsgGetSenderTimestamp(burst_msg, &sender_timestamp);
  if (T_TRUE == status
      && sender_timestamp > 0.0) {
    sender_timestamp_set = T_TRUE;

    status = TipcMsgGetArrivalTimestamp(burst_msg, &arrival_timestamp);
    T_EXPECT(T_TRUE == status);
  }

  /* set the current field of the message 0 indicates first field */
  status = TipcMsgSetCurrent(burst_msg, 0);
  T_EXPECT(T_TRUE == status);

  for (counter = 0; counter < numb_of_bursted_msgs; counter++) {

    T_IPC_MSG orig_msg = T_NULL;
    T_INT4 message_type;
    T_INT4 num_fields;
    T_INT4 user_prop;

    message_type = info_field_array[(counter * BURST_MSG_INFO_SIZE) + 0];
    num_fields   = info_field_array[(counter * BURST_MSG_INFO_SIZE) + 1];
    user_prop    = info_field_array[(counter * BURST_MSG_INFO_SIZE) + 2];

    orig_msg = TipcMsgCreate(TipcMtLookupByNum(message_type));
    if (T_NULL == orig_msg) {
      continue; 
    } 

    status = TipcMsgSetDest(orig_msg,dest);
    T_EXPECT(T_TRUE == status);

    status = TipcMsgSetSender(orig_msg,sender);
    T_EXPECT(T_TRUE == status);

    if (T_TRUE == sender_timestamp_set) {
      status = TipcMsgSetSenderTimestamp(orig_msg, sender_timestamp);
      T_EXPECT(T_TRUE == status);

      status = TipcMsgSetArrivalTimestamp(orig_msg, arrival_timestamp);
      T_EXPECT(T_TRUE == status);
    }

    /* This time ipcBurstAppendMsgFields is called to unpack the message */
    status = ipcBurstAppendMsgFields (orig_msg, burst_msg, msg_fields_offset,
                                      num_fields);
    T_EXPECT(T_TRUE == status);

    status = TipcMsgSetUserProp(orig_msg, user_prop);
    T_EXPECT(T_TRUE == status);

    msg_fields_offset += num_fields;

    /* If the user has defined a valid function pointer then he may be
     * checking some stuff with it. The user looks at it and if he returns 
     * true then we process the message and the callbacks associated with 
     * the message type would be called
     * If he returns false he does not need the message so we just destory
     * it.
     * If he has not specified any function then we just process everything
     * i.e at time of intialization if he calls TipcBurstInit(NULL, NULL)
     * then we process everything
     */
    if (burst_filter_func != NULL) {
      status = burst_filter_func(orig_msg, burst_filter_arg);
      if (T_TRUE == status) {
        if (T_NULL == srv) {
          status = TipcSrvMsgProcess(orig_msg);
          T_EXPECT(T_TRUE == status);
        }
        else {
          status = TipcSrvConnMsgProcess(srv, orig_msg);
          T_EXPECT(T_TRUE == status);
        }
        status = TipcMsgDestroy(orig_msg);
        T_EXPECT(T_TRUE == status);
      } 
      else {
        status = TipcMsgDestroy(orig_msg);
        T_EXPECT(T_TRUE == status);
      } 
    } 
    else {
      if (T_NULL == srv) {
        status = TipcSrvMsgProcess(orig_msg);
        T_EXPECT(T_TRUE == status);
      }
      else {
        status = TipcSrvConnMsgProcess(srv, orig_msg);
        T_EXPECT(T_TRUE == status);
      }
      status = TipcMsgDestroy(orig_msg);
      T_EXPECT(T_TRUE == status);
    } 
  } 
} 

/* ========================================================================= */
/*..ipcBurstInitInternal -- Should be called first for one time Initilization      */
static void T_ENTRY ipcBurstInitInternal
(
T_BURST_FILTER_FUNC burst_filter_func,
T_PTR burst_filter_arg,
T_IPC_SRV srv,
T_BURST_FILTER_CLOSURE *closure
)
{
  T_NEWC(*closure);

  (*closure)->burst_filter_func = burst_filter_func;
  (*closure)->burst_filter_arg = burst_filter_arg;
  (*closure)->srv_ptr = srv;

  ipcBurstInitMsgTypes();  

}

/* ========================================================================= */
/*..TipcBurstConnInit -- Should be called first for one time Initilization      */
T_BOOL T_ENTRY T_EXPORT TipcBurstConnInit
(
T_IPC_SRV srv,
T_BURST_FILTER_FUNC burst_filter_func,
T_PTR burst_filter_arg
)
{
  T_BURST_FILTER_CLOSURE closure = T_NULL;
  
  T_CHECK_NULL(srv, T_FALSE);

  (void)ipcBurstInitInternal(burst_filter_func, burst_filter_arg, srv, &closure);

  if (!TipcSrvConnProcessCbCreate(srv, TipcMtLookupByNum(T_MT_BURST),
                                  TipcBurstMsgProcessCb, (T_PTR)closure)) {
    return T_FALSE;
  }
  return T_TRUE;
} 

/* ========================================================================= */
/*..TipcBurstInit -- Should be called first for one time Initilization      */
T_BOOL T_ENTRY T_EXPORT TipcBurstInit
(
T_BURST_FILTER_FUNC burst_filter_func,
T_PTR burst_filter_arg
)
{

  T_BURST_FILTER_CLOSURE closure;

  (void)ipcBurstInitInternal(burst_filter_func, burst_filter_arg, T_NULL, &closure);
 
  if (!TipcSrvProcessCbCreate(TipcMtLookupByNum(T_MT_BURST),
                              TipcBurstMsgProcessCb, (T_PTR)closure)) {
    return T_FALSE;
  }
  return T_TRUE;
} 


/* =======================================================================  */
/*..ipcBurstConnCreateInternal -- Call this for our internal purposes          */
static T_IPC_BURST T_ENTRY ipcBurstConnCreateInternal
(
T_IPC_SRV srv,
T_INT4 auto_flush_count
)
{
  T_BOOL status;
  T_IPC_BURST burst;

  /* now create the new burst */
  T_NEWC(burst);

  /* call ipcBurstInitMsgTypes for the internal burst's we create */
  ipcBurstInitMsgTypes();

  burst->msg = TipcMsgCreate(TipcMtLookupByNum(T_MT_BURST)); 
  if (T_NULL == burst->msg) {
    T_FREE (burst);
    return T_NULL;
  } 

  status = TipcBurstConnSetAutoFlushCount(srv, burst,auto_flush_count);
  T_ASSERT(T_TRUE == status);

  burst->info_field_size = 0;

  return burst;

}

/* =======================================================================  */
/*..ipcBurstCreateInternal -- Call this for our internal purposes          */
static T_IPC_BURST T_ENTRY ipcBurstCreateInternal
(
T_INT4 auto_flush_count
)
{
 
  T_BOOL status;
  T_IPC_BURST burst;

  /* now create the new burst */
  T_NEWC(burst);

  /* call ipcBurstInitMsgTypes for the internal burst's we create */
  ipcBurstInitMsgTypes();

  burst->msg = TipcMsgCreate(TipcMtLookupByNum(T_MT_BURST)); 
  if (T_NULL == burst->msg) {
    T_FREE (burst);
    return T_NULL;
  } 

  status = TipcBurstSetAutoFlushCount(burst,auto_flush_count);
  T_ASSERT(T_TRUE == status);

  burst->info_field_size = 0;

  return burst;

}

/* ========================================================================= */
/*..ipcBurstSetAutoFlushCountInteral -- sets the number of messages into a BURST */
static T_BOOL T_ENTRY ipcBurstSetAutoFlushCountInternal
(
T_IPC_BURST burst,
T_INT4 auto_flush_count
)
{
  T_CHECK_NULL(burst, T_FALSE)
  T_CHECK_EXPR((auto_flush_count > 0), T_FALSE)

  burst->auto_flush_count = auto_flush_count;

  T_CALLOC(burst->info_field_array,
           burst->auto_flush_count*BURST_MSG_INFO_SIZE*sizeof(T_INT4),
           T_INT4*);

  T_CHECK_NULL(burst->info_field_array, T_FALSE);

  return T_TRUE;

}

/* ========================================================================= */
/*..TipcBurstCreate -- Create the Burst Messages and ipcBurstInitMsgtypes    */
T_IPC_BURST T_ENTRY T_EXPORT TipcBurstCreate
(
T_INT4 auto_flush_count
)
{
  T_BOOL status;
  T_IPC_BURST burst; 

  /* if the auto_flush_count is zero or less than one punt
   * right away (BM 04/29/98)
   */
  T_CHECK_EXPR((auto_flush_count > 0), T_FALSE)  

  T_NEWC(burst);

  /* call ipcBurstInitMsgTypes for the internal burst's we create */
  ipcBurstInitMsgTypes();

  /* ------------------ */
  /* Create the tables. */
  /* ------------------ */

  burst->ipc_burst_table = TutHashCreateStri(4);
  T_ASSERT(T_NULL != burst->ipc_burst_table);

  /* we need to set the auto_flush_count since we use the same value
   * for our internally created burst's. ipcBurstSetAutoFlushCountInternal
   * also does some allocation for info_field_array so we have to
   * free it for the main burst which user creates (BM 04/24/98)
   */
  status = ipcBurstSetAutoFlushCountInternal(burst,auto_flush_count);
  T_ASSERT(T_TRUE == status);

  return burst;

} 


/* ========================================================================= */
/*..ipcBurstDestroyFunc destroy's each of the burst and respective members  */
static void T_ENTRY ipcBurstDestroyFunc 
(
T_PTR key,   /* actually a subject name but we don't use it */
T_PTR value, /* the actual burst of messages */
T_PTR arg
)
{

  T_BOOL status;
  T_BURST_PTR_INFO info_ptr = (T_BURST_PTR_INFO)value;
  T_IPC_BURST internal_burst = info_ptr->burst;
  T_STR key_value = key;

  T_EXPECT(key != NULL);
  T_ASSERT(value != NULL);
  T_EXPECT(arg == NULL);

  T_FREE_IF_NON_NULL(key);
  T_FREE (internal_burst->info_field_array);

  status = TipcMsgDestroy (internal_burst->msg);
  T_EXPECT(T_TRUE == status);

  T_FREE(internal_burst);
  T_FREE(info_ptr);
} 


/* ========================================================================= */
/*..TipcBurstDestroy -- destroy's the BURST MESSAGE                          */
T_BOOL T_ENTRY T_EXPORT TipcBurstDestroy
(
T_IPC_BURST burst
)
{
  /*
   * Flush the remainding burst message.
   */
  T_CHECK_NULL(burst,FALSE);
  T_CHECK_NULL(burst->ipc_burst_table, FALSE);


  /* Free the hashtable first and its associated pointers
   */
  TutHashDestroy(burst->ipc_burst_table, ipcBurstDestroyFunc, NULL);
  /* Free the info field array since it gets created for the burst 
   * when user calls TipcBurstCreate.
   */

  burst->ipc_burst_table = NULL;
  T_FREE(burst->info_field_array);

  T_FREE_IF_NON_NULL(burst);

  return TRUE;
} 


/* ========================================================================= */
/*..TipcBurstGetAutoFlushCount -- gets the number of the messages batched in */
T_BOOL T_ENTRY T_EXPORT TipcBurstGetAutoFlushCount
(
T_IPC_BURST burst,
T_INT4 *auto_flush_count_return
)
{
  T_CHECK_NULL(burst, FALSE)
  T_CHECK_NULL(auto_flush_count_return, T_FALSE) 


  *auto_flush_count_return = burst->auto_flush_count;

  return T_TRUE;
} 


/* ========================================================================= */
/*..ipcBurstConnFlushInternal -- flushes burst messages when called internally  */
static T_BOOL T_ENTRY ipcBurstConnFlushInternal
(
T_IPC_SRV   srv,
T_IPC_BURST burst
)
{

  /* When we call flush internally we do not want to traverse the list since
   * the hashtable do not point to anything, they are not even created 
   */
  T_BOOL status;

  T_CHECK_NULL(burst, T_FALSE)

  status = TipcMsgAppendInt4ArrayPtr(burst->msg, burst->info_field_array,
                                  burst->info_field_size, T_NULL);
  T_EXPECT(T_TRUE == status);

  status = TipcSrvConnMsgSend(srv,burst->msg,TRUE);
  T_EXPECT(T_TRUE == status);

  /* reuse messages; it is more efficient than create/destroy */
  status = TipcMsgSetNumFields(burst->msg,0);
  T_EXPECT(T_TRUE == status);

  burst->info_field_size = 0;

  return T_TRUE;

} 

/* ========================================================================= */
/*..ipcBurstFlushInternal -- flushes burst messages when called internally  */
static T_BOOL T_ENTRY ipcBurstFlushInternal
(
T_IPC_BURST burst
)
{

  /* When we call flush internally we do not want to traverse the list since
   * the hashtable do not point to anything, they are not even created 
   */
  T_BOOL status;

  T_CHECK_NULL(burst, T_FALSE)

  status = TipcMsgAppendInt4ArrayPtr(burst->msg, burst->info_field_array,
                                  burst->info_field_size, T_NULL);
  T_EXPECT(T_TRUE == status);

  status = TipcSrvMsgSend(burst->msg,TRUE);
  T_EXPECT(T_TRUE == status);

  /* reuse messages; it is more efficient than create/destroy */
  status = TipcMsgSetNumFields(burst->msg,0);
  T_EXPECT(T_TRUE == status);

  burst->info_field_size = 0;

  return T_TRUE;

} 

/* ========================================================================= */
/*..ipcBurstGetCount -- Returns the current number of batchMessages */
static T_INT4 T_ENTRY ipcBurstGetCount
(
T_IPC_BURST burst
)
{
  T_INT4 burstCount = -1;
  T_CHECK_NULL(burst, burstCount)

  /*
   * Calculate the current Count by dividing the number of items  
   * in the info field by the number of info field entries per    
   * bursted message.                                                  
   */

  burstCount = (burst->info_field_size / BURST_MSG_INFO_SIZE);
  return burstCount;

} 

/* ========================================================================= */
/*..ipcBurstConnSetAutoFlushFunc -- sets the number of messages into a BURST */
static T_PTR T_ENTRY ipcBurstConnSetAutoFlushFunc
(
T_PTR key,
T_PTR burst_info_ptr,
T_PTR arg
)
{
  T_BURST_FLUSH_INFO_CLOSURE closure = (T_BURST_FLUSH_INFO_CLOSURE)arg;
  T_INT4 auto_flush_count = closure->auto_flush_count;
  T_IPC_SRV srv = (T_IPC_SRV)closure->srv_ptr;
  T_BURST_PTR_INFO info_ptr = (T_BURST_PTR_INFO)burst_info_ptr;
  T_IPC_BURST burst;
  T_BOOL status;

  burst = info_ptr->burst;

 /* if the burst size has been reduced then we flush so that the user 
   * would not loose the messages and then set the size
   */
  if (auto_flush_count < burst->auto_flush_count) {
    if (ipcBurstGetCount(burst) > 0) {
      if (info_ptr->srv == srv) {
        status = ipcBurstConnFlushInternal(srv, burst);
        T_EXPECT(T_TRUE == status);
      }
    }
  } 
  burst->auto_flush_count = auto_flush_count;

  /* reset the info_field_array to what ever the user specified 
   */
  if (burst->info_field_array != T_NULL) {
     T_REALLOC(burst->info_field_array,
               burst->auto_flush_count*BURST_MSG_INFO_SIZE*sizeof(T_INT4),
               T_INT4*);
  }

  return T_NULL;

} 

/* ========================================================================= */
/*..ipcBurstSetAutoFlushFunc -- sets the number of messages into a BURST */
static T_PTR T_ENTRY ipcBurstSetAutoFlushFunc
(
T_PTR key,
T_PTR burst_info_ptr,
T_PTR arg
)
{
  T_BURST_FLUSH_INFO_CLOSURE closure = (T_BURST_FLUSH_INFO_CLOSURE)arg;
  T_INT4 auto_flush_count = closure->auto_flush_count;
  T_IPC_BURST burst;
  T_BOOL status;
  T_BURST_PTR_INFO info_ptr = (T_BURST_PTR_INFO)burst_info_ptr;

  burst = (T_IPC_BURST)info_ptr->burst;

  /* if the burst size has been reduced then we flush so that the user 
   * would not loose the messages and then set the size
   */
  if (auto_flush_count < burst->auto_flush_count) {
    if (ipcBurstGetCount(burst) > 0) {
      status = ipcBurstFlushInternal(burst);
      T_EXPECT(T_TRUE == status);
    }
  } 
  burst->auto_flush_count = auto_flush_count;

  /* reset the info_field_array to what ever the user specified 
   */
  if (burst->info_field_array != T_NULL) {
    T_REALLOC(burst->info_field_array,
              burst->auto_flush_count*BURST_MSG_INFO_SIZE*sizeof(T_INT4),
              T_INT4*);
  }
  return T_NULL;

} 


/* ========================================================================= */
/*..TipcBurstConnSetAutoFlushCount -- sets the number of messages into a BURST */
T_BOOL T_ENTRY T_EXPORT TipcBurstConnSetAutoFlushCount
(
T_IPC_SRV   srv,
T_IPC_BURST burst,
T_INT4 auto_flush_count
)
{
  T_INT4 size_of_hash_table;
  T_BURST_FLUSH_INFO_CLOSURE closure;

  T_CHECK_NULL(burst, T_FALSE)
  T_CHECK_EXPR((auto_flush_count > 0), T_FALSE)


  T_NEWC(closure);
  burst->auto_flush_count = auto_flush_count;

  if (burst->info_field_array != T_NULL) {
    T_REALLOC(burst->info_field_array,
              burst->auto_flush_count*BURST_MSG_INFO_SIZE*sizeof(T_INT4),
              T_INT4*);

    /* we want to traverse the list and set all of the burst's sizes 
     * we could check for hashtable size at this point and traverse
     * the table and set each burst's value and set each of the sizes 
     */
    closure->auto_flush_count = auto_flush_count;
    closure->srv_ptr = srv;

    size_of_hash_table = TutHashGetSize(burst->ipc_burst_table);
    /* If the size of hashtable is greater than zero and the setAutoFlush
     * has been called would mean that user has called it after initially
     * calling TipcBurstCreate and after he has appended some messages
     */

    if (size_of_hash_table > 0) {
      TutHashTraverse(burst->ipc_burst_table, ipcBurstConnSetAutoFlushFunc, 
                      closure);
    }

  }

  /*
   * This is mostly done because TipcBurstSetAutoFlushCount() is called from 
   * TipcBurstCreate() before any memory allocation is done, and not as an
   * attempt of recovering from a failed realloc.                          
   */
  else if (T_NULL == burst->info_field_array) {
    T_CALLOC(burst->info_field_array,
             burst->auto_flush_count*BURST_MSG_INFO_SIZE*sizeof(T_INT4),
             T_INT4*);
  }

  T_CHECK_NULL(burst->info_field_array, T_FALSE);

  T_FREE_IF_NON_NULL(closure);

  return T_TRUE;

} 

/* ========================================================================= */
/*..TipcBurstSetAutoFlushCount -- sets the number of messages into a BURST */
T_BOOL T_ENTRY T_EXPORT TipcBurstSetAutoFlushCount
(
T_IPC_BURST burst,
T_INT4 auto_flush_count
)
{
  T_INT4 size_of_hash_table;
  T_BURST_FLUSH_INFO_CLOSURE closure;

  T_CHECK_NULL(burst, T_FALSE)
  T_CHECK_EXPR((auto_flush_count > 0), T_FALSE)


  T_NEWC(closure);
  burst->auto_flush_count = auto_flush_count;

  if (burst->info_field_array != T_NULL) {
    T_REALLOC(burst->info_field_array,
              burst->auto_flush_count*BURST_MSG_INFO_SIZE*sizeof(T_INT4),
              T_INT4*);

    /* we want to traverse the list and set all of the burst's sizes 
     * we could check for hashtable size at this point and traverse
     * the table and set each burst's value and set each of the sizes 
     */
    closure->auto_flush_count = auto_flush_count;

    size_of_hash_table = TutHashGetSize(burst->ipc_burst_table);
    /* If the size of hashtable is greater than zero and the setAutoFlush
     * has been called would mean that user has called it after initially
     * calling TipcBurstCreate and after he has appended some messages
     */

    if (size_of_hash_table > 0) {
      TutHashTraverse(burst->ipc_burst_table, ipcBurstSetAutoFlushFunc, 
                      closure);
    }

  }

  /*
   * This is mostly done because TipcBurstSetAutoFlushCount() is called from 
   * TipcBurstCreate() before any memory allocation is done, and not as an
   * attempt of recovering from a failed realloc.                          
   */
  else if (T_NULL == burst->info_field_array) {
    T_CALLOC(burst->info_field_array,
             burst->auto_flush_count*BURST_MSG_INFO_SIZE*sizeof(T_INT4),
             T_INT4*);
  }

  T_CHECK_NULL(burst->info_field_array, T_FALSE);

  T_FREE_IF_NON_NULL(closure);

  return T_TRUE;

} 

/* ========================================================================= */
/*..ipcBurstFailedBurstMsgTravFunc -- the trav func that gets called */
static T_BOOL T_ENTRY ipcBurstFailedBurstMsgTravFunc
(
T_IPC_MSG failed_msg,
T_PTR arg
)
{
  T_BOOL status;
  T_IPC_MSG gmd_failure_msg;

  T_BURST_MSG_FAILURE_INF failure_info = (T_BURST_MSG_FAILURE_INF)arg;
  T_IPC_SRV srv = (T_IPC_SRV)failure_info->srv_ptr;
  /*
   * Build a GMD_FAILURE message and process it directly.
   */
  gmd_failure_msg = TipcMsgCreate(TipcMtLookupByNum(T_MT_GMD_FAILURE));
  T_ASSERT(T_NULL != gmd_failure_msg);


  status = TipcMsgAppendMsg(gmd_failure_msg, failed_msg);
  T_EXPECT(T_TRUE == status);
  status = TipcMsgAppendStr(gmd_failure_msg, failure_info->proc_name);
  T_EXPECT(T_TRUE == status);
  status = TipcMsgAppendInt4(gmd_failure_msg, failure_info->err_num);
  T_EXPECT(T_TRUE == status);
  status = TipcMsgAppendReal8(gmd_failure_msg, failure_info->send_time);
  T_EXPECT(T_TRUE == status);

  /* Insert these GMD_FAILURE msgs at the end of the queue */
  if (T_NULL == srv) {
    status = TipcSrvMsgInsert(gmd_failure_msg, T_IPC_POS_PRIORITY);
    T_EXPECT(T_TRUE == status);
  }
  else {
    status = TipcSrvConnMsgInsert(srv, gmd_failure_msg, T_IPC_POS_PRIORITY);
    T_EXPECT(T_TRUE == status);
  }

  return T_TRUE;
}


/* ========================================================================= */
/*..TipcBurstGetMsg -- return the burst msg associated with a subject */
T_IPC_MSG T_ENTRY T_EXPORT TipcBurstGetMsg
(
T_IPC_BURST burst,
T_STR subject
)
{
  T_BURST_PTR_INFO info_ptr;
  T_IPC_BURST burst_ptr;
  T_IPC_MSG msg;

  if ((info_ptr = (T_BURST_PTR_INFO)TutHashLookup(burst->ipc_burst_table, 
                                                   subject)) != NULL) {
   burst_ptr = info_ptr->burst;
   msg = burst_ptr->msg;
   return msg;
  }
  else {
    return T_NULL;
  }
}


/* ========================================================================= */
/*..TipcBurstMsgTraverse -- traverse the burst msg & call the trav func */
T_BOOL T_ENTRY T_EXPORT TipcBurstMsgTraverse
(
T_IPC_MSG burst_msg,
T_BURST_MSG_TRAV_FUNC func,
T_PTR arg
)
{
  T_IPC_MT mt;
  T_INT4 mt_num;
  T_BOOL status;
  T_INT4 num_fields;
  T_INT4 *info_field_array;
  T_INT4 info_field_size;
  T_STR dest;
  T_STR sender;
  T_INT4 numb_of_bursted_msgs;
  T_INT4 counter;
  T_INT4 msg_fields_offset = 0;
  T_INT4 burst_msg_seq_num = 0;
  T_IPC_DELIVERY_MODE  burst_msg_delivery_mode = 0;
  T_REAL8 sender_timestamp = 0.0;
  T_REAL8 arrival_timestamp = 0.0;
  T_BOOL sender_timestamp_set = T_FALSE;

  T_CHECK_NULL(burst_msg, T_FALSE);
  T_CHECK_NULL(func, T_FALSE);

  status = TipcMsgGetType(burst_msg, &mt);
  T_EXPECT(T_TRUE == status);

  status = TipcMtGetNum(mt, &mt_num);
  T_EXPECT(T_TRUE == status);

  if (T_MT_BURST != mt_num) {
    TutErrNumSet(T_ERR_VAL_INVALID);
    return T_FALSE;
  }

  status = TipcMsgGetNumFields(burst_msg, &num_fields);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgSetCurrent(burst_msg, num_fields - 1);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgGetSeqNum(burst_msg, &burst_msg_seq_num);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgGetDeliveryMode(burst_msg, &burst_msg_delivery_mode);
  T_EXPECT(T_TRUE == status);

  status =  TipcMsgNextInt4Array(burst_msg, &info_field_array, 
                              &info_field_size);
  T_EXPECT(T_TRUE == status);

  numb_of_bursted_msgs = info_field_size / BURST_MSG_INFO_SIZE;

  status = TipcMsgGetDest(burst_msg, &dest);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgGetSender(burst_msg, &sender);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgSetCurrent(burst_msg, 0);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgGetSenderTimestamp(burst_msg, &sender_timestamp);
  if (T_TRUE == status
      && sender_timestamp > 0.0) {
    sender_timestamp_set = T_TRUE;

    status = TipcMsgGetArrivalTimestamp(burst_msg, &arrival_timestamp);
    T_EXPECT(T_TRUE == status);
  }

  for (counter = 0; counter < numb_of_bursted_msgs; counter++) {

    T_INT4 message_type;
    T_INT4 num_fields;
    T_INT4 user_prop;
    T_IPC_MSG orig_msg = T_NULL;

    message_type = info_field_array[(counter * BURST_MSG_INFO_SIZE) + 0];
    num_fields   = info_field_array[(counter * BURST_MSG_INFO_SIZE) + 1];
    user_prop    = info_field_array[(counter * BURST_MSG_INFO_SIZE) + 2];

    orig_msg = TipcMsgCreate(TipcMtLookupByNum(message_type));
    if (T_NULL == orig_msg) {
      continue; 
    }

    status = TipcMsgSetDest(orig_msg,dest);
    T_EXPECT(T_TRUE == status);

    status = TipcMsgSetSender(orig_msg,sender);
    T_EXPECT(T_TRUE == status);

    if (T_TRUE == sender_timestamp_set) {
      status = TipcMsgSetSenderTimestamp(orig_msg, sender_timestamp);
      T_EXPECT(T_TRUE == status);

      status = TipcMsgSetArrivalTimestamp(orig_msg, arrival_timestamp);
      T_EXPECT(T_TRUE == status);
    }

    /* This time ipcBurstAppendMsgFields is called to unpack the message */
    status = ipcBurstAppendMsgFields (orig_msg, burst_msg, msg_fields_offset,
                                      num_fields);
    T_EXPECT(T_TRUE == status);

    status = TipcMsgSetUserProp(orig_msg, user_prop);
    T_EXPECT(T_TRUE == status);

    status = TipcMsgSetSeqNum(orig_msg, burst_msg_seq_num);
    T_EXPECT(T_TRUE == status);

    status = TipcMsgSetDeliveryMode(orig_msg, burst_msg_delivery_mode);
    T_EXPECT(T_TRUE == status);

    msg_fields_offset += num_fields;

    status = func(orig_msg, arg);
    T_EXPECT(T_TRUE == status);
    
    status = TipcMsgDestroy(orig_msg);
    T_EXPECT(T_TRUE == status);
  }

  return status;
}

/* ========================================================================= */
/*..TipcCbBurstConnProcessGmdFailure -- process failure of Burst msgs */
void T_ENTRY T_EXPORT TipcCbBurstConnProcessGmdFailure
(
T_IPC_CONN conn,
T_IPC_CONN_PROCESS_CB_DATA data,
T_CB_ARG arg
)
{
  T_IPC_MT   mt;
  T_INT4     mt_num;
  T_BOOL     status;
  T_IPC_MSG  msg = T_NULL;
  T_IPC_MSG  failed_msg;
  T_BURST_MSG_FAILURE_INF failure_info;
  T_INT4     err_num;
  T_REAL8    err_time;
  T_STR      failed_process;
  T_IPC_SRV  srv = T_NULL;
  
  srv = (T_IPC_SRV)arg;
  msg = (T_IPC_MSG)data->msg;

  status = TipcMsgSetCurrent(msg, 0);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgNextMsg(msg, &failed_msg);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgNextStr(msg, &failed_process);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgNextInt4(msg, &err_num);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgNextReal8(msg, &err_time);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgGetType(failed_msg, &mt);
  T_EXPECT(T_TRUE == status);

  status = TipcMtGetNum(mt, &mt_num);
  T_EXPECT(T_TRUE == status);

  if (T_MT_BURST == mt_num) {
    if (T_NULL == srv) {
      TutWarning("Cannot process burst gmd failure, callback arg needs to be srv_ptr and is NULL\n");
      return;
    }

    /*
     * create individual msgs for the each msg in the burst
     */
    T_NEWC(failure_info);
    failure_info->err_num = err_num;
    failure_info->send_time = err_time;
    T_STRDUP(failure_info->proc_name, failed_process);
    failure_info->srv_ptr = srv;

    TipcBurstMsgTraverse(failed_msg, ipcBurstFailedBurstMsgTravFunc, failure_info);

    T_FREE(failure_info->proc_name);
    T_FREE(failure_info);
    
    status = TipcMsgSetCurrent(msg, 0);
    T_EXPECT(T_TRUE == status);
    
    return;
  }
  else {
    status = TipcMsgSetCurrent(msg, 0);
    T_EXPECT(T_TRUE == status);
    return;
  }
}

/* ========================================================================= */
/*..TipcCbBurstProcessGmdFailure -- process failure of Burst msgs */
void T_ENTRY T_EXPORT TipcCbBurstProcessGmdFailure
(
T_IPC_CONN conn,
T_IPC_CONN_PROCESS_CB_DATA data,
T_CB_ARG arg
)
{
  T_IPC_MT   mt;
  T_INT4     mt_num;
  T_BOOL     status;
  T_IPC_MSG  msg = T_NULL;
  T_IPC_MSG  failed_msg;
  T_BURST_MSG_FAILURE_INF failure_info;
  T_INT4     err_num;
  T_REAL8    err_time;
  T_STR      failed_process;
  
  msg = (T_IPC_MSG)data->msg;

  status = TipcMsgSetCurrent(msg, 0);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgNextMsg(msg, &failed_msg);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgNextStr(msg, &failed_process);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgNextInt4(msg, &err_num);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgNextReal8(msg, &err_time);
  T_EXPECT(T_TRUE == status);

  status = TipcMsgGetType(failed_msg, &mt);
  T_EXPECT(T_TRUE == status);

  status = TipcMtGetNum(mt, &mt_num);
  T_EXPECT(T_TRUE == status);

  if (T_MT_BURST == mt_num) {
    /*
     * create individual msgs for the each msg in the burst
     */
    T_NEWC(failure_info);
    failure_info->err_num = err_num;
    failure_info->send_time = err_time;
    T_STRDUP(failure_info->proc_name, failed_process);
    failure_info->srv_ptr = T_NULL;

    TipcBurstMsgTraverse(failed_msg, ipcBurstFailedBurstMsgTravFunc, failure_info);

    T_FREE(failure_info->proc_name);
    T_FREE(failure_info);
    return;
  }
  else {
    return;
  }
}


/* ========================================================================= */
/*..TipcBurstGetCount -- gets the number of messages into a BURST */
T_BOOL T_ENTRY T_EXPORT TipcBurstGetCount
(
T_IPC_BURST burst,
T_STR subject_name,
T_INT4 *get_count_return
)
{
  T_IPC_BURST check_burst;
  T_INT4 size_of_table;
  T_BURST_PTR_INFO info_ptr;

  T_CHECK_NULL(burst, T_FALSE);
  T_CHECK_NULL(subject_name, T_FALSE);
  T_CHECK_NULL(get_count_return, T_FALSE);

  size_of_table = TutHashGetSize(burst->ipc_burst_table);
  if (size_of_table > 0) {
    info_ptr = (T_BURST_PTR_INFO)TutHashLookup(burst->ipc_burst_table, subject_name);
    if (info_ptr == NULL) {
      TutErrNumSet(T_ERR_VAL_UNKNOWN);
      return T_FALSE;
    }
    else {
      /* Note if a flush has taken place then the info_field_size is
       * set to back to zero. When the auto_flush_count is reached a 
       * flush takes place. So a call to TipcBurstGetCount would return 
       * zero (BM 05/12/98)
       */
      check_burst = info_ptr->burst;
      *get_count_return = check_burst->info_field_size / BURST_MSG_INFO_SIZE;
      return T_TRUE;
    } 
  }

  return T_FALSE;
} 

/* ========================================================================= */
/*..ipcBurstFlushFunc send the burst across to RTserver as one big chunck    */
static T_PTR T_ENTRY ipcBurstFlushFunc 

(
T_PTR key,        /* actually a subject name but we don't use it */
T_PTR burst_info_ptr, /* the actual burst of messages */
T_PTR arg
)
{
  T_IPC_BURST burst;
  T_BOOL status;
  T_IPC_SRV srv = (T_IPC_SRV)arg;
  T_BURST_PTR_INFO info_ptr;

  T_CHECK_NULL((T_BURST_PTR_INFO)burst_info_ptr, T_FALSE)
  T_CHECK_NULL(key, T_FALSE)
  
  info_ptr = (T_BURST_PTR_INFO)burst_info_ptr;
  burst = info_ptr->burst;

  if (ipcBurstGetCount(burst) > 0) {
    status = TipcMsgAppendInt4ArrayPtr(burst->msg,burst->info_field_array,
                                    burst->info_field_size, T_NULL);
    T_EXPECT(T_TRUE == status);

    if (T_NULL == srv) {
      status = TipcSrvMsgSend(burst->msg, T_TRUE);
      T_EXPECT(T_TRUE == status);
    }
    else {
      if (info_ptr->srv == srv) {
        status = TipcSrvConnMsgSend(srv, burst->msg, T_TRUE);
        T_EXPECT(T_TRUE == status);
        /* reuse messages; it is more efficient than create/destroy */
        status = TipcMsgSetNumFields(burst->msg,0);
        T_EXPECT(T_TRUE == status);

        burst->info_field_size = 0;
      }
    }
  }

  return T_NULL;

} 

/* ========================================================================= */
/*..TipcBurstConnFlush -- flushes out all the messages currently buffered */
T_BOOL T_ENTRY T_EXPORT TipcBurstConnFlush
(
T_IPC_SRV srv,
T_IPC_BURST burst
)
{
  T_INT4 size_of_table;

  T_CHECK_NULL(burst, T_FALSE);
  T_CHECK_NULL(srv, T_FALSE);

  size_of_table = TutHashGetSize(burst->ipc_burst_table);

  /* 
   * we want to call the traversal function only if the table 
   * size is greater than 0 i.e only for the main burst which 
   * holds pointers to other hash tables created for each    
   * subject (BM 04/23/98)                                    
   */

  if (size_of_table > 0) {
    /* Traverse the hash table and flush each out to the RTserver */
    TutHashTraverse(burst->ipc_burst_table, ipcBurstFlushFunc, srv);
  }

  return T_TRUE;
}


/* ========================================================================= */
/*..TipcBurstFlush -- flushes out all the messages currently buffered */
T_BOOL T_ENTRY T_EXPORT TipcBurstFlush
(
T_IPC_BURST burst
)
{
  T_INT4 size_of_table;

  T_CHECK_NULL(burst, T_FALSE)

  size_of_table = TutHashGetSize(burst->ipc_burst_table);

  /* 
   * we want to call the traversal function only if the table 
   * size is greater than 0 i.e only for the main burst which 
   * holds pointers to other hash tables created for each    
   * subject (BM 04/23/98)                                    
   */

  if (size_of_table > 0) {
    /* Traverse the hash table and flush each out to the RTserver */
    TutHashTraverse(burst->ipc_burst_table, ipcBurstFlushFunc, T_NULL);
  }

  return T_TRUE;
}


/* ========================================================================= */
/*..ipcBurstMsgSendInternal -- batches all msgs into a single BURST MESSAGE */
static T_BOOL T_ENTRY ipcBurstMsgSendInternal
(
T_IPC_SRV srv,
T_IPC_BURST burst, 
T_IPC_MSG msg
)
{
  T_IPC_BURST         check_burst_ptr; 
  T_IPC_BURST         copy_burst_ptr; 
  T_STR               copy_subject_name;
  T_REAL8             timestamp;
  T_STR               subject_name;             
  T_IPC_DELIVERY_MODE delivery_mode;
  T_IPC_BURST         new_burst; 
  T_BOOL              status;
  T_BURST_PTR_INFO    check_info_ptr;
  T_BURST_PTR_INFO    copy_info_ptr;


  /* Get the destination for the bursted message */
  T_CHECK_EXPR(TipcMsgGetDest(msg, &subject_name), T_FALSE);
  T_CHECK_NULL(subject_name, T_FALSE);
  
  status = TipcMsgGetDeliveryMode(msg, &delivery_mode);
  T_EXPECT(T_TRUE == status);

  if ((check_info_ptr = (T_BURST_PTR_INFO)TutHashLookup(burst->ipc_burst_table, 
                                                        subject_name)) == NULL) {
    T_STRDUP(copy_subject_name, subject_name);

    if (T_NULL == srv) {
      new_burst = ipcBurstCreateInternal(burst->auto_flush_count);
    }
    else {
      new_burst = ipcBurstConnCreateInternal(srv, burst->auto_flush_count);
    }
    if (!new_burst) {
      status = TipcBurstDestroy(burst);
      T_EXPECT(T_TRUE == status);
      return T_FALSE;
    }

    /* set the destination of the message here itself */
    status = TipcMsgSetDest(new_burst->msg, copy_subject_name);
    T_EXPECT(T_TRUE == status);

    status = TipcMsgSetDeliveryMode(new_burst->msg, delivery_mode);
    T_EXPECT(T_TRUE == status);

    /* Also copy over the Sender timestamp. This will give the set the
     * time to the oldest message in the burst.
     */
    status = TipcMsgGetSenderTimestamp(msg, &timestamp);
    if (T_TRUE == status
        && timestamp > 0.0 ) {
      status = TipcMsgSetSenderTimestamp(new_burst->msg, timestamp);
      T_EXPECT(T_TRUE == status);
    }

    T_NEWC(copy_info_ptr);
    copy_info_ptr->burst = new_burst;
    copy_info_ptr->srv = srv;
    TutHashInsert(burst->ipc_burst_table, copy_subject_name, copy_info_ptr);
    copy_burst_ptr = new_burst;

    T_CHECK_EXPR((ipcBurstAppendMsg(copy_burst_ptr, msg)), T_FALSE);
    if (ipcBurstGetCount(copy_burst_ptr) >= copy_burst_ptr->auto_flush_count) {
      if (T_NULL == srv) {
        status = ipcBurstFlushInternal(copy_burst_ptr);
        T_EXPECT(T_TRUE == status);
      }
      else {
        status = ipcBurstConnFlushInternal(srv, copy_burst_ptr);
        T_EXPECT(T_TRUE == status);
      }
    } 
  }

  /* 
   * Subject already exists.
   */
  else if ((check_info_ptr = (T_BURST_PTR_INFO)
            TutHashLookup(burst->ipc_burst_table, subject_name)) != T_NULL) {

    T_IPC_DELIVERY_MODE mode;

    check_burst_ptr = check_info_ptr->burst;
    status = TipcMsgGetDeliveryMode(check_burst_ptr->msg, &mode);
    T_EXPECT(T_TRUE == status);

    /*
     * if the delivery mode is either DELIVERY_SOME OR DELIVERY_ALL then
     * set the burst msg delivery to either one of these.
     * if it had started with delivery ordered then another one comes with
     * delivery some/all then set it delivery some/all.
     */
    if (mode != T_IPC_DELIVERY_ORDERED
        && delivery_mode != T_IPC_DELIVERY_ORDERED
        && delivery_mode > mode) {
      status = TipcMsgSetDeliveryMode(check_burst_ptr->msg, delivery_mode);
      T_EXPECT(T_TRUE == status);
    }
    else if (mode == T_IPC_DELIVERY_ORDERED
             && delivery_mode > T_IPC_DELIVERY_BEST_EFFORT
             && delivery_mode <= T_IPC_DELIVERY_ALL) {
      status = TipcMsgSetDeliveryMode(check_burst_ptr->msg, delivery_mode);
      T_EXPECT(T_TRUE == status);
    }
    /* Also copy over the Sender timestamp. This will give the set the
     * time to the oldest message in the burst.
     */
    status = TipcMsgGetSenderTimestamp(msg, &timestamp);
    if (T_TRUE == status
        && timestamp > 0.0) {
      status = TipcMsgSetSenderTimestamp(check_burst_ptr->msg, timestamp);
      T_EXPECT(T_TRUE == status);
    }

    T_CHECK_EXPR((ipcBurstAppendMsg(check_burst_ptr, msg)), T_FALSE);
    if (ipcBurstGetCount(check_burst_ptr) >= check_burst_ptr->auto_flush_count) {
      if (T_NULL == srv) {
        status = ipcBurstFlushInternal(check_burst_ptr);
        T_EXPECT(T_TRUE == status);
      }
      else {
        status = ipcBurstConnFlushInternal(srv, check_burst_ptr);
        T_EXPECT(T_TRUE == status);
      }
    }
  }

  return status;

}

/* ========================================================================= */
/*..TipcBurstConnMsgSend -- batches all the messages into an single BURST MESSAGE */
T_BOOL T_ENTRY T_EXPORT TipcBurstConnMsgSend
(
T_IPC_SRV srv,
T_IPC_BURST burst, 
T_IPC_MSG msg
)
{
  T_BOOL status;      

  T_CHECK_NULL(burst, T_FALSE)
  T_CHECK_NULL(msg, T_FALSE)
  T_CHECK_NULL(srv, T_FALSE);

  status = ipcBurstMsgSendInternal(srv, burst, msg);

  return status;
}

/* ========================================================================= */
/*..TipcBurstMsgSend -- batches all the messages into an single BURST MESSAGE */
T_BOOL T_ENTRY T_EXPORT TipcBurstMsgSend
(
T_IPC_BURST burst, 
T_IPC_MSG msg
)
{
  T_BOOL status;      

  T_CHECK_NULL(burst, T_FALSE)
  T_CHECK_NULL(msg, T_FALSE)

  status = ipcBurstMsgSendInternal(T_NULL, burst, msg);

  return status;
}

