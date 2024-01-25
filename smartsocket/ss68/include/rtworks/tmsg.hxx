/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmsg.hxx#1 $
 */

#ifndef _T_MSG_HH_
#define _T_MSG_HH_

#include <rtworks/ipc.h>

#include <rtworks/tobj.hxx>
#include <rtworks/tmt.hxx>

class T_IPCX_API TipcMsg;
typedef TipcMsg& (*_TipcMsgManipF)(TipcMsg&);

class T_IPCX_API TipcConnWrapper;
class T_IPCX_API TipcSrv;
class T_IPCX_API TipcMsgFile;

class T_IPCX_API TipcMsg : public Tobj {
  friend class TipcConn;
  friend class TipcConnWrapper;
  friend class TipcSrv;
  friend class TipcMsgFile;

  friend T_IPCX_API TipcMsg& _setsize(TipcMsg& msg, T_INT4  size);
  friend T_IPCX_API TipcMsg& _getsize(TipcMsg& msg, T_INT4* size);
  friend T_IPCX_API TipcMsg& Check(TipcMsg& msg);
  
private:
  T_INT4 _size;           /* For 'binary' and 'array' objects */
  
  T_IPC_MSG _msg;
  
  /* --------------------------------------------------------- */
  /* Holding place for TipcMt objects returned by reference by */
  /* member functions of this class.                           */
  /* --------------------------------------------------------- */
  TipcMt _mt_referent;

  /* ------------------------------------------------------------ */
  /* Record the first problem encountered while using the TipcMsg */
  /* insertion and extraction operators.                          */
  /* ------------------------------------------------------------ */
  T_BOOL _problem;

  void destroyMsg();

  /* --------------------------------------------------------- */
  /* The following function is used by friend classes on their */
  /* private TipcMsg objects which are used as a referent in   */
  /* calls that return TipcMsg objects by reference.           */
  /* --------------------------------------------------------- */
  void absorb(T_IPC_MSG msg);

protected:
  /* Accessors */
  T_INT4 size() const { return _size; }
  void   size(T_INT4 size_arg) { _size = size_arg; }

public:
  TipcMsg();
  
  TipcMsg(const TipcMt& mt);

  TipcMsg(T_INT4 mt_num, 
    T_STR  destination = (T_STR)T_NULL,
    T_STR  sender = (T_STR)T_NULL);

  TipcMsg(T_STR mt_name);

  TipcMsg(const T_IPC_MSG msg);

  TipcMsg(const TipcMsg& msg);

  TipcMsg& operator=(const T_IPC_MSG msg);
  TipcMsg& operator=(const TipcMsg& msg);

  virtual ~TipcMsg();

  T_BOOL     Create(TipcMt& mt);
  T_BOOL     Create(T_INT4 mt_num);
  T_BOOL     Create(T_STR mt_name);
  T_BOOL     Clone(TipcMsg& msg);
  T_BOOL     Destroy();

  // Allow private access
  T_IPC_MSG  Message() { return _msg; }

  // Conversion Operator
  operator   T_IPC_MSG() const { return _msg; }


  T_BOOL MsgToPacket(T_PTR packet_return,
         T_UINT4 packet_size);

  T_BOOL PacketToMsg(T_PTR packet);

  // Static converters
  static T_BOOL DeliveryModeToStr(T_IPC_DELIVERY_MODE delivery_mode,
          T_STR* delivery_mode_return);

  static T_BOOL StrToDeliveryMode(T_STR delivery_str,
          T_IPC_DELIVERY_MODE* delivery_mode_return);

#ifdef T_RTWORKS35_COMPAT
  static T_BOOL ModeToStr(T_IPC_DELIVERY_MODE delivery_mode,
        T_STR* delivery_mode_return);

  static T_BOOL StrToMode(T_STR delivery_str,
        T_IPC_DELIVERY_MODE* delivery_mode_return);
#endif

  static T_BOOL FtToStr(T_IPC_FT type,
      T_STR* str_return);

  static T_BOOL StrToFt(T_STR str,
      T_IPC_FT *type_return);
  static T_BOOL LbModeToStr(T_IPC_LB_MODE lb_mode, T_STR *str_return); 
  static T_BOOL StrToLbMode(T_STR str, T_IPC_LB_MODE *lb_mode_return);
  static T_BOOL FieldSize(T_IPC_MSG_FIELD field, T_INT4 size);

  // Traverse all fields
  T_PTR         Traverse(T_IPC_MSG_TRAV_FUNC func,
                         T_PTR arg);

  // Increment T_IPC_MSG reference count
  T_BOOL        IncrRefCount();

  // Only the 'get' operation is allowed
#ifdef T_RTWORKS35_COMPAT
  T_PTR         DataPtr();                            // Get it
  T_INT4        DataSize();                           // Get it
#endif
  T_BOOL        ReadOnly();                           // Get it
  T_INT4        RefCount();                           // Get it
  T_BOOL        Vacant();                             // Get it
  T_BOOL        SeqNum();                             // Get it
  T_UINT4       PacketSize();                         // Get it

  // Both 'get' & 'set' operations allowed
  T_STR         Dest();                               // Get it
  T_BOOL        Dest(T_STR dest_return);              // Set it

  T_STR         Sender();                             // Get it
  T_BOOL        Sender(T_STR  sender);                // Set it

  TipcMt&       Type();                               // Get it
  T_BOOL        Type(TipcMt& mt);                     // Set it

  T_INT4        Current();                            // Get it
  T_BOOL        Current(T_INT4  field_num);           // Set it

#ifdef T_RTWORKS35_COMPAT
  T_INT4        DataMax();                            // Get it
  T_BOOL        DataMax(T_INT4  data_max);            // Set it
#endif

  T_INT4        NumFields();                          // Get it
  T_BOOL        NumFields(T_INT4  num_fields);        // Set it

  T_INT2        Priority();                           // Get it
  T_BOOL        Priority(T_INT2  priority);           // Set it

  T_INT4        UserProp();                           // Get it
  T_BOOL        UserProp(T_INT4 user_prop);           // Set it

  T_REAL8       DeliveryTimeout();                    // Get it
  T_BOOL        DeliveryTimeout(T_REAL8 timeout);     // Set it

  T_IPC_DELIVERY_MODE DeliveryMode();                           // Get
  T_BOOL              DeliveryMode(T_IPC_DELIVERY_MODE  mode);  // Set

  T_BOOL CurrentFieldKnown(T_BOOL * field_known_return);       // Get Known SMV

  T_IPC_LB_MODE LbMode();                             // Get it
  T_BOOL        LbMode(T_IPC_LB_MODE lb_mode);        // Set it

  T_BOOL        HeaderStrEncode();                  // Get it
  T_BOOL        HeaderStrEncode(T_BOOL header_encode); // Set it

  T_STR         MessageId();                          // Get it
  T_BOOL        GenerateMessageId();                  // Set it

  T_STR         CorrelationId();                      // Get it
  T_BOOL        CorrelationId(T_STR correlation_id);  // Set it

  T_STR         ReplyTo();                            // Get it
  T_BOOL        ReplyTo(T_STR subj);                  // Set it

  T_REAL8       Expiration();                         // Get it
  T_BOOL        Expiration(T_REAL8 expiration);       // Set it

  T_REAL8       SenderTimestamp();                    // Get it
  T_BOOL        SenderTimestamp(T_REAL8 timestamp);   // Set it

  T_REAL8       ArrivalTimestamp();                   // Get it
  T_BOOL        ArrivalTimestamp(T_REAL8 timestamp);  // Set it

  T_IPC_MSG        Encrypt(); // Encrypt the message

  T_IPC_MSG        Decrypt(); // Decrypt the message

  T_BOOL        Ack();

  /* The '_array' and 'binary' field types require a    */
  /* size() to be preset on insertion.  On extraction,  */
  /* result is accessible by calling argless size()     */

  // Insertion Operators
  virtual TipcMsg& operator<<(T_PTR arg);     /* "binary" */
  virtual TipcMsg& operator<<(T_CHAR arg);    /* char */
  virtual TipcMsg& operator<<(T_GUID arg);    /* GUID       YX 06/20/2000 */
  virtual TipcMsg& operator<<(T_GUID* arg);   /* GUID array YX 06/20/2000 */
  virtual TipcMsg& operator<<(T_INT2 arg);    /* int2 */
  virtual TipcMsg& operator<<(T_INT2* arg);   /* int2 array */
  virtual TipcMsg& operator<<(T_INT4 arg);    /* int4 */
  virtual TipcMsg& operator<<(T_INT4* arg);   /* int4 array */
  virtual TipcMsg& operator<<(T_INT8 arg);    /* int8 */
  virtual TipcMsg& operator<<(T_INT8* arg);   /* int8 array */
  virtual TipcMsg& operator<<(TipcMsg& arg);  /* msg */
  virtual TipcMsg& operator<<(TipcMsg* arg);  /* msg array */
  virtual TipcMsg& operator<<(T_REAL4 arg);   /* real4 */
  virtual TipcMsg& operator<<(T_REAL4* arg);  /* real4 array */
  virtual TipcMsg& operator<<(T_REAL8 arg);   /* real8 */
  virtual TipcMsg& operator<<(T_REAL8* arg);  /* real8 array */
  virtual TipcMsg& operator<<(T_REAL16 arg);  /* real16 */
  virtual TipcMsg& operator<<(T_REAL16* arg); /* real16 array */
#if defined(T_OS_HPUX) 
  virtual TipcMsg& operator<<(const char* arg);/* str */
#else
  virtual TipcMsg& operator<<(T_STR arg);     /* str */
#endif
  virtual TipcMsg& operator<<(T_STR* arg);    /* str array */
  virtual TipcMsg& operator<<(T_XML arg);     /* XML */
  virtual TipcMsg& operator<<(T_UCHAR arg);   /* byte */
  virtual TipcMsg& operator<<(_TipcMsgManipF func);

  // Extraction Operators
  virtual TipcMsg& operator>>(T_PTR& arg);     /* binary */
  virtual TipcMsg& operator>>(T_CHAR& arg);    /* char */
  virtual TipcMsg& operator>>(T_GUID& arg);    /* GUID       YX 06/20/2000 */
  virtual TipcMsg& operator>>(T_GUID*& arg);   /* GUID array YX 06/20/2000 */
  virtual TipcMsg& operator>>(T_INT2& arg);    /* int2 */
  virtual TipcMsg& operator>>(T_INT2*& arg);   /* int2 array */ 
  virtual TipcMsg& operator>>(T_INT4& arg);    /* int4 */
  virtual TipcMsg& operator>>(T_INT4*& arg);   /* int4 array */
  virtual TipcMsg& operator>>(T_INT8& arg);    /* int8 */
  virtual TipcMsg& operator>>(T_INT8*& arg);   /* int8 array */
  virtual TipcMsg& operator>>(TipcMsg& arg);   /* msg */
  virtual TipcMsg& operator>>(TipcMsg*& arg);  /* msg array */
  virtual TipcMsg& operator>>(T_REAL4& arg);   /* real4 */
  virtual TipcMsg& operator>>(T_REAL4*& arg);  /* real4 array */
  virtual TipcMsg& operator>>(T_REAL8& arg);   /* real8 */
  virtual TipcMsg& operator>>(T_REAL8*& arg);  /* real8 array */
  virtual TipcMsg& operator>>(T_REAL16& arg);  /* real16 */
  virtual TipcMsg& operator>>(T_REAL16*& arg); /* real16 array */
  virtual TipcMsg& operator>>(T_STR& arg);     /* str */
  virtual TipcMsg& operator>>(T_STR*& arg);    /* str array */
  virtual TipcMsg& operator>>(T_XML& arg);     /* XML */
  virtual TipcMsg& operator>>(T_UCHAR& arg);   /* byte */
  virtual TipcMsg& operator>>(_TipcMsgManipF func);

  // Append functions which provide the C API look-and-feel
  virtual T_BOOL Append(T_PTR binary_data, T_INT4 binary_size);
  virtual T_BOOL Append(T_CHAR char_data);
  virtual T_BOOL Append(T_GUID guid_data);
  virtual T_BOOL Append(T_GUID* guid_array_data, T_INT4 guid_array_size);
  virtual T_BOOL Append(T_INT2 int2_data);
  virtual T_BOOL Append(T_INT2* int2_array_data, T_INT4 int2_array_size);
  virtual T_BOOL Append(T_INT4 int4_data);
  virtual T_BOOL Append(T_INT4* int4_array_data, T_INT4 int4_array_size);
  virtual T_BOOL Append(T_INT8 int8_data);
  virtual T_BOOL Append(T_INT8 *int8_array_data, T_INT4 int8_array_size);
  virtual T_BOOL Append(TipcMsg& msg_data);
  virtual T_BOOL Append(TipcMsg* msg_array_data, T_INT4 msg_array_size);
  virtual T_BOOL Append(T_REAL4 real4_data);
  virtual T_BOOL Append(T_REAL4* real4_array_data, T_INT4 real4_array_size);
  virtual T_BOOL Append(T_REAL8 real8_data);
  virtual T_BOOL Append(T_REAL8* real8_array_data, T_INT4 real8_array_size);
  virtual T_BOOL Append(T_REAL16 real16_data);
  virtual T_BOOL Append(T_REAL16 *real16_array_data, T_INT4 real16_array_size);
  virtual T_BOOL Append(T_STR str_data);
  virtual T_BOOL Append(T_STR* str_array_data, T_INT4 str_array_size);
  virtual T_BOOL Append(T_STR str_data, T_REAL8 real8_data);
  virtual T_BOOL Append(T_XML xml_data);
  virtual T_BOOL Append(T_UCHAR byte_data);

  virtual T_BOOL AppendBool(T_BOOL bool_data);
  virtual T_BOOL AppendBool(T_BOOL* bool_array_data, T_INT4 bool_array_size);
  virtual T_BOOL AppendUtf8(T_STR str_data);
  virtual T_BOOL AppendUtf8(T_STR* str_array_data, T_INT4 str_array_size);

  virtual T_BOOL AppendPtr(T_PTR binary_data, T_INT4 binary_data_size, 
         T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendPtr(T_GUID *guid_array_data, T_INT4 guid_array_size,
         T_IPC_MSG_FIELD *field_return); /* YX 06/07/2000 */

  virtual T_BOOL AppendPtr(T_INT2 *int2_array_data, T_INT4 int2_array_size,
         T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendPtr(T_INT4 *int4_array_data, T_INT4 int4_array_size,
         T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendPtr(T_INT8 *int8_array_data, T_INT4 int8_array_size,
         T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendPtr(T_IPC_MSG msg_data, T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendPtr(T_IPC_MSG *msg_array_data, T_INT4 msg_array_size,
         T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendPtr(T_REAL4 *real4_array_data, T_INT4 real4_array_size,
         T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendPtr(T_REAL8 *real8_array_data, T_INT4 real8_array_size,
         T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendPtr(T_REAL16 *real16_array_data, T_INT4 size,
         T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendPtr(T_STR str_data, T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendPtr(T_STR *str_array_data, T_INT4 str_array_size,
         T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendPtr(T_XML xml_data, T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendBoolPtr(T_BOOL *bool_array_data, T_INT4 bool_array_size,
             T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendUtf8Ptr(T_STR str_data, T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendUtf8Ptr(T_STR *str_array_data, T_INT4 str_array_size,
             T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AppendUnknown(T_IPC_FT field_type);  /* Append Unknown SMV*/ 

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_PTR binary_data, 
                           T_INT4 binary_data_size);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_GUID *guid_array_data, 
                           T_INT4 guid_array_size);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_INT2 *int2_array_data, 
                           T_INT4 int2_array_size);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_INT4 *int4_array_data, 
                           T_INT4 int4_array_size);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_INT8 *int8_array_data, 
                           T_INT4 int8_array_size);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_IPC_MSG msg_data);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_IPC_MSG *msg_array_data, 
                           T_INT4 msg_array_size);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_REAL4 *real4_array_data, 
                           T_INT4 real4_array_size);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_REAL8 *real8_array_data, 
                           T_INT4 real8_array_size);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_REAL16 *real16_array_data, 
                           T_INT4 size);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_STR str_data);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_STR *str_array_data, 
                           T_INT4 str_array_size);

  virtual T_BOOL UpdatePtr(T_IPC_MSG_FIELD field, T_XML xml_data);

  virtual T_BOOL UpdateBoolPtr(T_IPC_MSG_FIELD field, T_BOOL *bool_array_data, 
                               T_INT4 bool_array_size);

  virtual T_BOOL UpdateUtf8Ptr(T_IPC_MSG_FIELD field, T_STR str_data);

  virtual T_BOOL UpdateUtf8Ptr(T_IPC_MSG_FIELD field, T_STR *str_array_data, 
                               T_INT4 str_array_size);

  // Next functions which provide the C API look-and-feel
  virtual T_BOOL Next(T_PTR* binary_data, T_INT4* binary_size);

  virtual T_BOOL Next(T_CHAR* char_data);

  virtual T_BOOL Next(T_GUID* guid_data); /* YX 06/07/2000 */
  virtual T_BOOL Next(T_GUID** guid_array_data, T_INT4* guid_array_size); 
                                          /* YX 06/20/2000 */
  virtual T_BOOL Next(T_INT2* int2_data );
  virtual T_BOOL Next(T_INT2** int2_array_data, T_INT4* int2_array_size);

  virtual T_BOOL Next(T_INT4* int4_data );
  virtual T_BOOL Next(T_INT4** int4_array_data, T_INT4* int4_array_size);

  virtual T_BOOL Next(T_INT8* int8_data); 
  virtual T_BOOL Next(T_INT8** int8_array_data, T_INT4* int8_array_size); 

  virtual T_BOOL Next(TipcMsg* msg_data );
  virtual T_BOOL Next(TipcMsg** msg_array_data, T_INT4* msg_array_size);

  virtual T_BOOL Next(T_REAL4* real4_data);
  virtual T_BOOL Next(T_REAL4** real4_array_data, T_INT4* real4_array_size);

  virtual T_BOOL Next(T_REAL8* real8_data);
  virtual T_BOOL Next(T_REAL8** real8_array_data, T_INT4* real8_array_size);

  virtual T_BOOL Next(T_REAL16 *real16_data); 
  virtual T_BOOL Next(T_REAL16 **real16_array_data, 
          T_INT4 *real16_array_size); 

  virtual T_BOOL Next(T_STR* str_data);
  virtual T_BOOL Next(T_STR** str_array_data, T_INT4* str_array_size);

  virtual T_BOOL Next(T_STR* str_data, T_REAL8* real8_data);

  virtual T_BOOL Next(T_XML* xml_data );

  virtual T_BOOL Next(T_UCHAR* byte_data );

  virtual T_BOOL NextBool(T_BOOL* bool_data );
  virtual T_BOOL NextBool(T_BOOL** bool_array_data, T_INT4* bool_array_size);

  virtual T_BOOL NextUtf8(T_STR* str_data);
  virtual T_BOOL NextUtf8(T_STR** str_array_data, T_INT4* str_array_size);

  virtual T_BOOL NextUnknown( ); /* Next Unknown */

  virtual T_BOOL AddNamed(T_STR name, T_PTR value, T_INT4 size);
  virtual T_BOOL AddNamed(T_STR name, T_PTR value, T_INT4 size, 
        T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL AddNamed(T_STR name, T_CHAR value);

  virtual T_BOOL AddNamed(T_STR name, T_GUID value);
  virtual T_BOOL AddNamed(T_STR name, T_GUID* value, T_INT4 size);  
  virtual T_BOOL AddNamed(T_STR name, T_GUID *value, T_INT4 size, 
        T_IPC_MSG_FIELD *field_return); 
         
  virtual T_BOOL AddNamed(T_STR name, T_INT2 value); 
  virtual T_BOOL AddNamed(T_STR name, T_INT2 *value, T_INT4 size); 
  virtual T_BOOL AddNamed(T_STR name, T_INT2 *value, T_INT4 size, 
        T_IPC_MSG_FIELD *field_return); 

  virtual T_BOOL AddNamed(T_STR name, T_INT4 value); 
  virtual T_BOOL AddNamed(T_STR name, T_INT4 *value, T_INT4 size); 
  virtual T_BOOL AddNamed(T_STR name, T_INT4 *value, T_INT4 size, 
        T_IPC_MSG_FIELD *field_return); 

  virtual T_BOOL AddNamed(T_STR name, T_INT8 value); 
  virtual T_BOOL AddNamed(T_STR name, T_INT8 *value, T_INT4 size); 
  virtual T_BOOL AddNamed(T_STR name, T_INT8 *value, T_INT4 size, 
        T_IPC_MSG_FIELD *field_return); 

  virtual T_BOOL AddNamed(T_STR name, T_IPC_MSG value); 
  virtual T_BOOL AddNamed(T_STR name, T_IPC_MSG *value, T_INT4 size); 
  virtual T_BOOL AddNamed(T_STR name, T_IPC_MSG *value, T_INT4 size, 
        T_IPC_MSG_FIELD *field_return); 
  virtual T_BOOL AddNamed(T_STR name, T_IPC_MSG value, 
        T_IPC_MSG_FIELD *field_return); /* YX 06/21/2000 */

  virtual T_BOOL AddNamed(T_STR name, T_REAL4 value); 
  virtual T_BOOL AddNamed(T_STR name, T_REAL4 *value, T_INT4 size);
  virtual T_BOOL AddNamed(T_STR name, T_REAL4 *value, T_INT4 size, 
        T_IPC_MSG_FIELD *field_return); 
 
  virtual T_BOOL AddNamed(T_STR name, T_REAL8 value); 
  virtual T_BOOL AddNamed(T_STR name, T_REAL8 *value, T_INT4 size); 
  virtual T_BOOL AddNamed(T_STR name, T_REAL8 *value, T_INT4 size, 
        T_IPC_MSG_FIELD *field_return); 

  virtual T_BOOL AddNamed(T_STR name, T_REAL16 value); 
  virtual T_BOOL AddNamed(T_STR name, T_REAL16 *value, T_INT4 size); 
  virtual T_BOOL AddNamed(T_STR name, T_REAL16 *value, T_INT4 size, 
        T_IPC_MSG_FIELD *field_return); 

  virtual T_BOOL AddNamed(T_STR name, T_STR value); 
  virtual T_BOOL AddNamed(T_STR name, T_STR *value, T_INT4 size); 
  virtual T_BOOL AddNamed(T_STR name, T_STR *value, T_INT4 size, 
        T_IPC_MSG_FIELD *field_return);
  virtual T_BOOL AddNamed(T_STR name, T_STR value, 
        T_IPC_MSG_FIELD *field_return); 

  virtual T_BOOL AddNamed(T_STR name, T_XML value); 
  virtual T_BOOL AddNamed(T_STR name, T_XML value, 
        T_IPC_MSG_FIELD *field_return); 

  virtual T_BOOL AddNamed(T_STR name, T_UCHAR value);

  virtual T_BOOL AddNamedBool(T_STR name, T_BOOL value); 
  virtual T_BOOL AddNamedBool(T_STR name, T_BOOL *value, T_INT4 size); 
  virtual T_BOOL AddNamedBool(T_STR name, T_BOOL *value, T_INT4 size, 
            T_IPC_MSG_FIELD *field_return); 

  virtual T_BOOL AddNamedUtf8(T_STR name, T_STR value); 
  virtual T_BOOL AddNamedUtf8(T_STR name, T_STR *value, T_INT4 size); 
  virtual T_BOOL AddNamedUtf8(T_STR name, T_STR *value, T_INT4 size, 
            T_IPC_MSG_FIELD *field_return);
  virtual T_BOOL AddNamedUtf8(T_STR name, T_STR value, 
            T_IPC_MSG_FIELD *field_return); 

  virtual T_BOOL AddNamedUnknown(T_STR name, T_INT4 type);

  virtual T_BOOL GetNamed(T_STR name, T_PTR *value, T_INT4 *size);

  virtual T_BOOL GetNamed(T_STR name, T_CHAR *value);

  virtual T_BOOL GetNamed(T_STR name, T_GUID *value); 
  virtual T_BOOL GetNamed(T_STR name, T_GUID **value, T_INT4 *size); 

  virtual T_BOOL GetNamed(T_STR name, T_INT2 *value); 
  virtual T_BOOL GetNamed(T_STR name, T_INT2 **value, T_INT4 *size); 

  virtual T_BOOL GetNamed(T_STR name, T_INT4 *value); 
  virtual T_BOOL GetNamed(T_STR name, T_INT4 **value, T_INT4 *size); 

  virtual T_BOOL GetNamed(T_STR name, T_INT8 *value); 
  virtual T_BOOL GetNamed(T_STR name, T_INT8 **value, T_INT4 *size); 

  virtual T_BOOL GetNamed(T_STR name, T_IPC_MSG *value); 
  virtual T_BOOL GetNamed(T_STR name, T_IPC_MSG **value, T_INT4 *size); 

  virtual T_BOOL GetNamed(T_STR name, T_REAL4 *value); 
  virtual T_BOOL GetNamed(T_STR name, T_REAL4 **value, T_INT4 *size); 

  virtual T_BOOL GetNamed(T_STR name, T_REAL8 *value); 
  virtual T_BOOL GetNamed(T_STR name, T_REAL8 **value, T_INT4 *size); 

  virtual T_BOOL GetNamed(T_STR name, T_REAL16 *value); 
  virtual T_BOOL GetNamed(T_STR name, T_REAL16 **value, T_INT4 *size); 

  virtual T_BOOL GetNamed(T_STR name, T_STR *value); 
  virtual T_BOOL GetNamed(T_STR name, T_STR **value, T_INT4 *size); 

  virtual T_BOOL GetNamed(T_STR name, T_XML *value); 

  virtual T_BOOL GetNamed(T_STR name, T_UCHAR *value); 

  virtual T_BOOL GetNamedBool(T_STR name, T_BOOL *value); 
  virtual T_BOOL GetNamedBool(T_STR name, T_BOOL **value, T_INT4 *size); 

  virtual T_BOOL GetNamedUtf8(T_STR name, T_STR *value); 
  virtual T_BOOL GetNamedUtf8(T_STR name, T_STR **value, T_INT4 *size); 

  virtual T_BOOL GetNamedUnknown(T_STR name);
  
  virtual T_BOOL GetTypeNamed(T_STR name, T_IPC_FT *type);

  virtual T_BOOL ExistsNamed(T_STR name, T_BOOL *bool_return);

  virtual T_BOOL SetNameCurrent(T_STR name);

  virtual T_BOOL GetNameCurrent(T_STR *name);

  /* Added for Feild Delete/Update YX 06/06/2000 */
  virtual T_BOOL DeleteCurrent();
  virtual T_BOOL DeleteField(T_INT4 index);
  virtual T_BOOL DeleteNamedField(T_STR name); /* delete named field */

  virtual T_BOOL UpdateNamed(T_STR name, T_PTR value, T_INT4 size);
  virtual T_BOOL UpdateNamed(T_STR name, T_PTR value, T_INT4 size, 
           T_IPC_MSG_FIELD *field_return);

  virtual T_BOOL UpdateNamed(T_STR name, T_CHAR value);

  virtual T_BOOL UpdateNamed(T_STR name, T_GUID value);
  virtual T_BOOL UpdateNamed(T_STR name, T_GUID *value, T_INT4 size);
  virtual T_BOOL UpdateNamed(T_STR name, T_GUID *value, T_INT4 size, 
                       T_IPC_MSG_FIELD *field_return);  

  virtual T_BOOL UpdateNamed(T_STR name, T_INT2 value);
  virtual T_BOOL UpdateNamed(T_STR name, T_INT2 *value, T_INT4 size);
  virtual T_BOOL UpdateNamed(T_STR name, T_INT2 *value, T_INT4 size, 
                       T_IPC_MSG_FIELD *field_return);  

  virtual T_BOOL UpdateNamed(T_STR name, T_INT4 value);
  virtual T_BOOL UpdateNamed(T_STR name, T_INT4 *value, T_INT4 size);
  virtual T_BOOL UpdateNamed(T_STR name, T_INT4 *value, T_INT4 size, 
                       T_IPC_MSG_FIELD *field_return);             

  virtual T_BOOL UpdateNamed(T_STR name, T_INT8 value);               
  virtual T_BOOL UpdateNamed(T_STR name, T_INT8 *value, T_INT4 size);
  virtual T_BOOL UpdateNamed(T_STR name, T_INT8 *value, T_INT4 size, 
           T_IPC_MSG_FIELD *field_return); 

  virtual T_BOOL UpdateNamed(T_STR name, T_IPC_MSG value); 
  virtual T_BOOL UpdateNamed(T_STR name, T_IPC_MSG *value, T_INT4 size);
  virtual T_BOOL UpdateNamed(T_STR name, T_IPC_MSG *value, T_INT4 size, 
           T_IPC_MSG_FIELD *field_return);            
  virtual T_BOOL UpdateNamed(T_STR name, T_IPC_MSG value, 
           T_IPC_MSG_FIELD *field_return);           

  virtual T_BOOL UpdateNamed(T_STR name, T_REAL4 value);                 
  virtual T_BOOL UpdateNamed(T_STR name, T_REAL4 *value, T_INT4 size); 
  virtual T_BOOL UpdateNamed(T_STR name, T_REAL4 *value, T_INT4 size, 
                             T_IPC_MSG_FIELD *field_return);            

  virtual T_BOOL UpdateNamed(T_STR name, T_REAL8 value); 
  virtual T_BOOL UpdateNamed(T_STR name, T_REAL8 *value, T_INT4 size); 
  virtual T_BOOL UpdateNamed(T_STR name, T_REAL8 *value, T_INT4 size,    
           T_IPC_MSG_FIELD *field_return);    

  virtual T_BOOL UpdateNamed(T_STR name, T_REAL16 value); 
  virtual T_BOOL UpdateNamed(T_STR name, T_REAL16 *value, T_INT4 size);
  virtual T_BOOL UpdateNamed(T_STR name, T_REAL16 *value, T_INT4 size, 
           T_IPC_MSG_FIELD *field_return);     

  virtual T_BOOL UpdateNamed(T_STR name, T_STR value);  
  virtual T_BOOL UpdateNamed(T_STR name, T_STR *value, T_INT4 size); 
  virtual T_BOOL UpdateNamed(T_STR name, T_STR *value, T_INT4 size, 
           T_IPC_MSG_FIELD *field_return); 
  virtual T_BOOL UpdateNamed(T_STR name, T_STR value, 
           T_IPC_MSG_FIELD *field_return);  

  virtual T_BOOL UpdateNamed(T_STR name, T_XML value); 
  virtual T_BOOL UpdateNamed(T_STR name, T_XML value,
           T_IPC_MSG_FIELD *field_return);
 
  virtual T_BOOL UpdateNamed(T_STR name, T_UCHAR value);

  virtual T_BOOL UpdateNamedBool(T_STR name, T_BOOL value);
  virtual T_BOOL UpdateNamedBool(T_STR name, T_BOOL *value, T_INT4 size);
  virtual T_BOOL UpdateNamedBool(T_STR name, T_BOOL *value, T_INT4 size, 
                                 T_IPC_MSG_FIELD *field_return);             

  virtual T_BOOL UpdateNamedUtf8(T_STR name, T_STR value);  
  virtual T_BOOL UpdateNamedUtf8(T_STR name, T_STR *value, T_INT4 size); 
  virtual T_BOOL UpdateNamedUtf8(T_STR name, T_STR *value, T_INT4 size, 
               T_IPC_MSG_FIELD *field_return); 
  virtual T_BOOL UpdateNamedUtf8(T_STR name, T_STR value, 
               T_IPC_MSG_FIELD *field_return);  

  // Another print method
  virtual T_BOOL Print(T_OUT_FUNC out_func);
  virtual T_BOOL PrintError();

  // reset the _problem flag
  void ResetCheck();
};

#endif


