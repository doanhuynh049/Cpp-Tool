/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmsg.h#1 $
 */

#ifndef _T_MSG_HH_
#define _T_MSG_HH_

#include <rtworks/ipc.h>
#include <rtworks/tmt.h>
#include <rtworks/tsystem.h>
namespace SmartSockets
{

  class System;
  class XML;
  /**
   * Inherits from TipcException so users can trap TipcMsg related
   * exceptions. Offers no additional functionality.
   *
   * @see TipcException, TipcMsg
   */
  class T_IPCX_API TipcMsgException : public TipcException  {

  public:
    /**
     * Constructor.
     */
    TipcMsgException () {}

    /**
     * Destructor.
     */
    ~TipcMsgException ()  throw() {}

  };

  /** @class TipcMsgTraverser tmsg.h rtworks/tmsg.h
   * Traverser class to be used by TipcMsg::traverse().
   */
  class T_IPCX_API TipcMsgTraverser {
  public:

    /**
     * Signature for the traverse method.
     * @param msg the message to which the fields belong.
     * @param type the field type.
     * @param flag to determine whether or not a message field is unknown
     * @param valuePtr void pointer to the current field value; this may be
     * cast to the appropriate value.
     * @param size number of elements of array fields or number
     * of bytes of binary fields.
     * @param field that can be updated by using one of the
     * TipcMsg::updatePtr() methods.
     * @param index zero-based position of the current field.
     * @param name of the field; NULL if not a named field.
     * @return true to continue traversing; false otherwise.
     */
    typedef bool (TipcMsgTraverser::* Handler)
    (
    const TipcMsg&  msg,
    T_IPC_FT        type,
    bool            isUnknown,
    void*           valuePtr,
    int4            size,
    T_IPC_MSG_FIELD field,
    int4            index,
    const char*     name
    ) ;



    TipcMsgTraverser();

    /**
     * Sets the current handler method; if this is not called, the default
      * method onTraverse() is called.
     */
    void setHandler(Handler cbMethod);

    /**
     * Gets the current handler method.
     */
    Handler getHandler();


    /**
     * Destructor.
     */
    virtual ~TipcMsgTraverser() throw();

    /**
     * Default traverse method.
     * @param msg the message to which the fields belong.
     * @param type the field type.
     * @param flag to determine whether or not a message field is unknown
     * @param valuePtr void pointer to the current field value; this may be
     * cast to the appropriate value.
     * @param size number of elements of array fields or number
     * of bytes of binary fields.
     * @param field that can be updated by using one of the
     * TipcMsg::updatePtr() methods.
     * @param index zero-based position of the current field.
     * @param name of the field; NULL if not a named field.
     * @return true to continue traversing; false otherwise.
     */
    virtual bool onTraverse
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
    throw () ;

    static  T_PTR T_ENTRY traverseFuncDelegator
    (
    T_IPC_MSG msg,
    T_IPC_MSG_TRAV trav,
    T_PTR arg
    );

  private:

    Handler _cbMethod;


  };


  class T_IPCX_API TipcSrv;
  class T_IPCX_API TipcMsgFile;

  /** @class TipcMsg tmsg.h rtworks/tmsg.h
   * The methods of the TipcMsg class allow you to construct messages and to
   * manipulate the data in a message. The TipcMsg class acts as a C++
   * interface to the SmartSockets
   * TipcMsg* C language API functions. See the SmartSockets User's
   * Guide for more information on
   * message composition in SmartSockets applications.
   *
   * The TipcMsg class has two ways of appending and accessing
   * data fields from a message.
   * The first is designed to provide an interface similar to the C
   * API and consists of several
   * append*() and next* member functions. The append*() and next*() member
   * functions replace the prefixed TipcMsgAppend* and TipcMsgNext*
   * functions of the C API.
   *
   * The second method of appending and accessing data fields
   * is designed to provide an
   * interface similar to C++ IOstream classes. Overloaded insertion
   * (operator<<) and extraction
   * (operator>>) operators replace the prefixed TipcMsgAppend
   * and TipcMsgNext* functions
   * of the C API.
   * The TipcMsg class contains a private SmartSockets C API T_IPC_MSG message.
   */
  class T_IPCX_API TipcMsg {
  private:
    /// friend class
    //@{
    friend class T_IPCX_API TipcConn;
    friend class T_IPCX_API TipcSrv;
    friend class T_IPCX_API TipcMsgFile;

    //@}

    int4 _size;           /* For 'binary' and 'array' objects */

    T_IPC_MSG _msg;


    void destroyMsg() throw (TipcException);

    void absorb(T_IPC_MSG msg);

  protected:

    /**
     * Increments the reference count of a message. The reference
     * count property can be used
     * to prevent a message from being destroyed. This is useful
     * for keeping copies of messages
     * whilst using methods that always destroy the TipcMsg object (e.g.
     * TipcConn::mainLoop(), or
     * TipcSrv::mainLoop()). For example, connection message-
     * related callbacks are only given
     * temporary access to a message, but sometimes these callbacks
     * need to save the messages
     * for later processing.  When a message is created or read from
     * a connection, its reference
     * count is initialized to 1.  Each call to TipcMsg::
     * incrRefCount() increments the reference
     * count. Each call to TipcMsg::~TipcMsg() decrements the reference
     * count. Once the reference
     * count reaches 0, the internal T_IPC_MSG
     * object is permanently destroyed.
     *
     * While TipcMsg::clone() can also be used to copy a message, if
     * the message is not going
     * to be modified then TipcMsgIncrRefCount can be used
     * instead. TipcMsgIncrRefCount is much
     * faster than TipcMsg::clone() because no memory allocation
     * and copying is performed.
     *
     */
    void incrRefCount() throw (TipcException);

  public:

    /**
     * Default constructor
     */
    TipcMsg()
    {
      _size = 0;
      _msg = (T_IPC_MSG)T_NULL;
    }

    /**
     * Constructor.
     * @param mt - TipcMt object used to determine the message type.
     * @throw TipcMsgException if an invalid message type is passed.
     */
    TipcMsg(const TipcMt& mt)
    throw (TipcException);

    // The default values of the constructors cannot be changed because
    // of a Microsoft Visual C++ 6.0 bug where an object that has an int4 in
    // the constructor is confused with long numbers on polymorphic operators.
    // See PDR 01696 for more details LPPM 01dec2001.
    /**
     * Constructor
     * @param mtNum - message type number either corresponding
     * to a pre-defined SmartSockets message, or to a user-defined message
     * created using the TipcMt::create() method.
     * @param destination - optional parameter that sets the destination
     * field of a message to a subject string (can be NULL).
     * @param sender - optional parameter that sets the sender field of
     * the message.  Note that
     * if the sender field is not set, it will default to the
     * connection's unique subject.
     *
     * @throw TipcMsgException if an invalid message type number is passed,
     * or the sender or destination cannot be set.
     */
    explicit TipcMsg(int4 mtNum,
                     const char *  destination = (const char *)T_NULL,
                     const char *  sender = (const char *)T_NULL)
    throw (TipcException);

    /**
     * Constructor
     * @param mtName - message type name either corresponding
     * to a pre-defined
     * SmartSockets message, or to a user-defined message created using the
     * TipcMt::create() method.
     *
     * @throw TipcMsgException if an invalid message type name is passed.
     */
    explicit TipcMsg(const char * mtName)
    throw (TipcException);

    /**
     * Constructor
     * @param msg - C structure; used for compatibility with the C API.
     *
     * @throw TipcMsgException if a NULL C-structure is passed.
     */
    TipcMsg(const T_IPC_MSG msg)
    throw (TipcException);

    /**
     * Copy Constructor that doesn't clone the whole message
     * , but simply increments the
     * reference count.
     * @throw TipcMsgException if the reference count couldn't be increased.
     */
    TipcMsg(const TipcMsg& msg)
    throw (TipcException);

    /**
     * Assignment operator that copies the state of the TipcMsg class,
     * without doing
     * a full clone (deep copy), and simply increments a reference count.
     * @throw TipcMsgException if the reference count couldn't be increased.
     */
    //@{
    TipcMsg& operator=(const T_IPC_MSG msg)
                      throw (TipcException);
    TipcMsg& operator=(const TipcMsg& msg)
                      throw (TipcException);

    //@}

    /**
     * Destructor. Will only decrease the reference count and
     * only destroy the message if
     * the reference count is back to zero.
     *
     * @throw TipcMsgException if the reference count can't be decreased or
     * if the message can't
     * be destroyed.
     */
    ~TipcMsg() throw ();


    /**
     * Safely destroys the managed T_IPC_MSG message
     */
    void destroy() throw (TipcException)
    {
      destroyMsg();
    }

    /**
     * Fully clones the contents of the message and creates a new object.
     * @throw TipcMsgException if the message can't be cloned.
     */
    TipcMsg* clone() const
    throw (TipcException);


    /**
     * typecast operator used for compatibility with the C API;
     * this gives access to the
     * private _msg variable.
     */
    operator T_IPC_MSG() const throw () { return _msg;}

    /**
     * Static converters that get string representations of various
     * enumerations
     */
    //@{
    static const char* deliveryModeToStr(T_IPC_DELIVERY_MODE delivery_mode)
    throw (TipcException);

    static T_IPC_DELIVERY_MODE strToDeliveryMode(const char * delivery_str)
    throw (TipcException);


    static const char* ftToStr(T_IPC_FT type)
    throw (TipcException);

    static T_IPC_FT strToFt(const char * str)
    throw (TipcException);

    static const char* lbModeToStr(T_IPC_LB_MODE lb_mode)
    throw (TipcException);

    static T_IPC_LB_MODE strToLbMode(const char * str)
    throw (TipcException);

    //@}



    /**
     * Methods used by the TipcMsgManipName and TipcMsgManipSize classes.
     */
    //@{
    static TipcMsg& setName(TipcMsg& msg, const char* name)
    {
      T_INT4 curr = msg.getNumFields();
      msg.setCurrent(curr - 1);
      msg.setNameCurrent(const_cast <char*>(name));
      return msg;
    }

    static const TipcMsg& getName(const TipcMsg& msg, const char*& nameReturn)
    {
      nameReturn = msg.getNameCurrent();

      return msg;
    }


    /* =================================================================== */
    /*..setsize --  set the size attribute of a TipcMsg object */
    static TipcMsg& setSize(TipcMsg& msg, int4 size)
    {
      msg._size = size;
      return msg;
    }

    /* =================================================================== */
    /*.._getsize --  get the size attribute of a TipcMsg object */
    static const TipcMsg& getSize(const TipcMsg& msg, int4& size_return)
    {
      size_return = msg._size;
      return msg;
    }
    //@}


    /**
     * Sets the size of a message pointer field .
     * @param field value returned by one of the TipcMsg::append*Ptr methods.
     * @param size - for scalar and binary field types, the size argument is
     * specified in bytes
     * for all array field types (including MSG_ARRAY), the size argument is
     * specified in elements;
     * for individual MSG fields, the size argument must always be zero.
     * @throw TipcMsgException if the field cannot be set to the given size.
     */
    static void setFieldSize(T_IPC_MSG_FIELD field, int4 size)
    throw (TipcException);



    /**
     * Traverse all message fields.  Allows users to sequentially go through
     * every field of the TipcMsg.
     * @param travObj an instance of a child class of TipcMsgTraverser
     * @param travMethod a traversal method matching the signature of
     * TipcMsgTraverser::onTraverse().  If
     * the default NULL value is used, TipcMsgTraverser::onTraverse() is used.
     */
    void  traverse(TipcMsgTraverser* travObj)
    throw (TipcException);

    /**
     * Gets the read-only property of a message. A message is marked
     * read-only if the message
     * is embedded as a field in another message.
     * @throw TipcMsgException if the read-only flag cannot be read due to
     * an invalid message.
     */
    bool isReadOnly() const
    throw (TipcException);

    /**
     * Checks to see if the current field has a known value. This is used to
     * map TipcMsg objects
     * to database rows where some fields have an unknown value (where zero
     * or a negative number may be valid choices).
     * @throw TipcMsgException if this property cannot be obtained due to an
     * invalid message.
     */
    bool isCurrentFieldKnown() const
    throw (TipcException);

    /**
     * Gets the current reference count.
     * @see TipcMsg::incrRefCount()
     * @throw TipcMsgException if this property cannot be obtained due
     * to an invalid message.
     */
    int2 getRefCount() const
    throw (TipcException);

    /**
     * Gets the sequence number of a message. The sequence number is
     * used by GMD to detect
     * duplicate messages. When a message with a
     * delivery mode of T_IPC_DELIVERY_SOME or
     * T_IPC_DELIVERY_ALL is sent through a connection, TipcConn::
     * send() increments an internal
     * per-connection sequence number and sets the sequence number
     * of the message to the
     * incremented value. If the message is later resent because
     * of a GMD failure,
     * TipcConn::gmdMsgResend preserves the sequence number,
     * which allows the receiving processes
     * to detect the duplicate sequence number and thus avoid
     * processing the same message twice.
     * There is no setter method for this property.
     * @throw TipcMsgException if this property cannot be obtained due
     * to an invalid message.
     */
    int4 getSeqNum() const
    throw (TipcException);


    /**
     * Gets the size of the message object's serialized form.
     * @throw TipcMsgException if this property cannot be obtained due
     * to an invalid message.
     */
    uint4 getPacketSize() const
    throw (TipcException);


    /**
     * Accessor method for the sender and detination properties of the
     * message.  The sender and
     * destination properties are strings whose meanings depend on how the
     * message is being used.
     * Messages sent with publish-subscribe between RTclients by RTserver,
     * for example, use a
     * subject name for the message sender and destination
     * properties. Other applications of
     * messages are free to use the sender and destination for other
     * purposes. Message sender and
     * destination properties can be null if they are not needed.
     */
    //@{
    const char *  getDest() const
    throw (TipcException);

    void setDest(const char * dest_return)
    throw (TipcException);

    const char * getSender() const
    throw (TipcException);

    void setSender(const char *  sender)
    throw (TipcException);
    //@}

    /**
     * Accessor method for the message type property.  Note that the
     * contents of the message will
     * be erased when the type is changed.  If getting a message type that
     * hasn't been created
     * in this process use getTypeNum() (e.g. if the application
     * attempts to filter out messages
     * without knowing the source of the messages, getType() will
     * throw an exception because the
     * message type may not have been created in this process).
     * Note that getType() returns a TipcMt object by value; since this
     * is a failry light weight
     * object, it's preferrable to pass it by value than
     * to keep an internal reference.
     */
    //@{
    const TipcMt getType() const
    throw (TipcException);

    int4 getTypeNum() const
    throw (TipcException);

    void setType(const TipcMt& mt)
    throw (TipcException);

    //@}


    /**
     * Accessor method for the current message field index.  The next*()
     * methods
     * will automatically increment the current field; however if
     * random access to unnamed fields
     * is required, then the field index may be moved
     * using the setter method below.
     * @throw TipcMsgException if this property cannot be retrieved or if it
     * is set to a number
     * out of the range of zero and getNumFields() - 1.
     */
    //@{
    int4 getCurrent() const
    throw (TipcException);
    void setCurrent(int4  field_num) throw (TipcException);
    //@}


    /**
     * Accessor method for the number of fields of the message.  The
     * setter method sets the number
     * of fields in a message by truncating the message to the desired number
     * of fields; note that
     * one can only decrease the number of fields (increasing it can
     * be accomplished by calling one
     * of the append*() methods). If num_fields is 0, the message is
     * completely emptied (the message
     * properties are not changed, but any fields in the data buffer are lost
     * ).  The reset() method
     * is a convenience method that calls the setter method with a zero
     * argument
     * ; it is useful for
     * reusing messages, rather than continually destroying and creating
     * objects.  The best example
     * for message reuse is a publishing process.
     */
    //@{
    int4 getNumFields() const throw (TipcException);
    void setNumFields(int4  num_fields) throw (TipcException);
    //@}


    /**
     * Accessor method for the priority property of the message.
     * Message priorities are used by
     * TipcConn::read to queue incoming messages in priority order. When a
     * message is created, its
     * priority is initialized to the message type priority (if set) or to
     * the value of the option
     * Default_Msg_Priority (if the message type priority is unknown).
     * This allows a default value
     * to be set easily for all messages of a certain
     * type or for all messages.
     * @see TipcMt::getPriority()
     * @throw TipcMsgException if this property cannot be accessed.
     */
    //@{
    int2 getPriority() const throw (TipcException);
    void setPriority(int2 priority) throw (TipcException);
    //@}


    /**
     * Accessor method for the user property of the message. You can use the
     * user-defined property for any purpose, such as attaching
     * a version number to messages.
     * This property is not used internally by SmartSockets. When
     * a message is sent through a
     * connection with TipcConn::send(), the user-defined property is
     * byte-swapped along with
     * all the other integer message properties.  On architectures where
     * a pointer is four bytes,
     * a pointer can be stored in the user-defined property of a message.
     * This is useful for
     * attaching extra temporary data to the message. Use pointers with
     * caution; when a message
     * is sent through a connection, the data the pointer refers to
     * does not exist in the
     * receiving process. In this situation, add extra fields to the
     * message instead of using a
     * pointer in the user-defined property.  When a message is created,
     * its user-defined property
     * is initialized to the message type user-defined property. This
     * allows a default value to
     * easily be set for all messages of a certain type.
     * @throw TipcMsgException if this property cannot be accessed.
     */

    //@{
    int4 getUserProp() const throw (TipcException);
    void setUserProp(int4 user_prop) throw (TipcException);
    //@}


    /**
     * Accessor method for the compression property of the message. The
     * compression property indicates whether the message data is to be
     * compressed at the time the message is sent. The message will not
     * be uncompressed until a receiver of the message attempts to access
     * any of the message data fields.
     *
     * The compression used is governed by the Compression_Name and
     * Compression_Args options.
     *
     * When a message is created, its compression property is initialized to
     * the message type compression property. This allows a default value to
     * easily be set for all messages of a certain type.
     * @throw TipcMsgException if this property cannot be accessed.
     */

    //@{
    bool getCompression() const throw (TipcException);
    void setCompression(bool compression) throw (TipcException);
    //@}


    /**
     * Accessor method for the delivery timeout property of the
     * message.  The delivery timeout
     * of a message is measured in seconds.  It controls how long
     * to wait for guaranteed message
     * delivery to complete when a message is sent through a connection with
     * TipcConn::send. When a
     * message is created, its delivery timeout is initialized to
     * the message type delivery timeout,
     * which defaults to UNKNOWN. This allows a default value to be easily
     * set for all messages of
     * a certain type.
     *
     * When a message is sent using GMD on a connection,
     * the connection delivery timeout is
     * used if the message delivery timeout is not set. The delivery timeout
     * of a message is never
     * sent through the connection, as it is only
     * needed by the sending process.
     * @throw TipcMsgException if this property cannot be set or retrieved
     */

    //@{
    real8 getDeliveryTimeout() const throw (TipcException);
    void setDeliveryTimeout(real8 timeout) throw (TipcException);

    //@}


    /**
     * Accessor method for the delivery mode property of the message.
     * The delivery mode of a
     * message controls what level of guarantee is used when
     * the message is sent through a
     * connection with TipcConn::send(). When a message is created, its
     * delivery mode is
     * initialized to the message type delivery mode. This allows a default
     * value to easily be set
     * for all messages of a certain type.
     *
     * The default message delivery mode is T_IPC_DELIVERY_BEST_EFFORT.
     * In this mode, no
     * special actions are taken to ensure delivery of sent messages.
     * The message is delivered
     * unless network failures or process failures occur. If the message is
     * not delivered, there
     * is no way for the sender to know that delivery failed. When there is
     * a failure, it is
     * possible for some messages to be lost or be delivered in a
     * different order than the order
     * in which they were published.
     *
     * In the case of a network or process failure, a
     * more useful delivery mode is
     * T_IPC_DELIVERY_ORDERED. Messages can still be lost in the event of
     * a failure, but all
     * delivered messages are received in the order in which they
     * were published. Because Java
     * Message Service (JMS) cannot handle messages that come out of order,
     * you must use the
     * T_IPC_DELIVERY_ORDERED delivery mode or one of the GMD
     * delivery modes if you are
     * sending messages to a JMS client.
     *
     * The difference between the other delivery modes and the GMD
     * delivery modes is that in
     * GMD, a copy of the message is kept when the message
     * is sent and acknowledgements are
     * sent when the message is received. No acknowledgements (ACKs)
     * are used in the ordered
     * or best effort delivery modes, and no copy of the message is kept.
     * One of the GMD delivery modes is T_IPC_DELIVERY_SOME. In
     * this mode, the sending
     * process saves a copy of the message in the connection GMD
     * area until the message is
     * successfully delivered, and the sender can also resend the message
     * if necessary. Delivery is
     * considered successful if the sent message is acknowledged by
     * at least one receiving process.
     *
     * The other and most robust GMD delivery mode is
     * T_IPC_DELIVERY_ALL. In this mode,
     * the sending process saves a copy of the message in the
     * connection GMD area until the
     * message is successfully delivered, and the sender can also
     * resend the message if necessary.
     * Delivery is not considered successful until all
     * receiving processes acknowledge the sent
     * message. For two processes communicating through a non-
     * RTclient and non-RTserver
     * T_IPC_CONN connection, T_IPC_DELIVERY_SOME and T_IPC_DELIVERY_ALL are
     * identical because there is only one process receiving the message.
     * For RTclients, the two
     * modes do differ if more than one RTclient is subscribing to the
     * subject in the destination of
     * the message.
     * @throw TipcMsgException if this property cannot be set or retrieved
     *
     */
    //@{
    T_IPC_DELIVERY_MODE getDeliveryMode() const
    throw (TipcException);
    void setDeliveryMode(T_IPC_DELIVERY_MODE  mode)
    throw (TipcException);


    //@}


    /**
     * Accessor methods for the load balance mode property of the message.
     * The load balancing mode
     * of a message controls how (or if) the message is
     * load balanced among RTclients subscribed
     * to a load balanced subject. When a message is created, its
     * load balancing mode is initialized
     * to the message type load balancing mode. This allows a default value to
     * easily be set for all
     * messages of a certain type.
     *
     * The default load balancing mode is T_IPC_LB_NONE. In this mode, no
     * load balancing is
     * done. The message is delivered to all RTclients subscribing to
     * the subject to which the
     * message is published.
     *
     * The other load balancing
     * modes are T_IPC_LB_ROUND_ROBIN, T_IPC_LB_WEIGHTED,
     * and T_IPC_LB_SORTED. In these modes, one and only one RTclient
     * in the list of potential
     * receivers receives the message. In ROUND_ROBIN mode,
     * each successive message is sent
     * to the next RTclient in a circular list of RTclients, therefore,
     * distributing the load evenly.
     * The WEIGHTED mode sends the message to the most responsive RTclient
     * (as measured by the
     * fewest pending GMD acknowledgments), reasoning that this is also
     * the RTclient with the least
     * load. In SORTED mode, each message is sent to the
     * RTclient whose unique subject name
     * sorts lowest lexicographically.
     *
     * @throw TipcMsgException if this property cannot be set or retrieved
     * due to a corrupt message,
     * or if attempting to set the load balancing mode to WEIGHTED in a non-
     * GMD message (with the
     * error set to T_ERRNM_TYPE_INVALID).
     */
    //@{
    T_IPC_LB_MODE getLbMode() const throw (TipcException);
    void setLbMode(T_IPC_LB_MODE lb_mode) throw (TipcException);
    //@}

    /**
     * Accessor methods for the header string encode property, which controls
     * whether the message header string properties are converted
     * into four-byte integers
     * when the message is sent through a connection
     * with TipcConnMsgSend. Enabling this
     * property compresses the message header so that less network bandwidth
     * is used, but this
     * compression requires more CPU utilization.
     *
     * When a message is created, its header string encode property
     * is initialized to the message
     * type header string encode property. This allows a default value to
     * easily be set for all
     * messages of a certain type.
     *
     */
    //@{

    bool isUsingHeaderStrEncode() const throw (TipcException);
    void setHeaderStrEncode(bool header_encode)
    throw (TipcException);

    //@}


    /**
     * Accessor methods for the message ID property.
     * The message identifier property is a
     * string that represents a unique identifier for the message.
     * Applications of messages are
     * free to use the message identifier for any purpose.
     * This message identifier is unique
     * for all messages that are created anywhere. It is sent along
     * with the message and adds
     * 38 extra bytes to the message.
     */
    //@{

    const char * getMessageId() const throw (TipcException);
    void generateMessageId() throw (TipcException);
    //@}

    /**
     * Accessor methods for the message correlation ID property.  The
     * correlation ID is used to
     * logically group together different messages.  Applications of
     * messages are free to use
     * the correlation ID for any purpose; there are no restrictions on
     * the format for this field;
     * i.e. any string of any size can be used.
     */
    //@{
    const char * getCorrelationId() const throw (TipcException);
    void setCorrelationId(const char * correlation_id) throw (TipcException);
    //@}

    /**
     * Accessor methods for the message reply-to property.  The
     * reply-to destination property
     * is a string representing a subject where a reply to
     * the message should be sent.
     */

    //@{
    const char * getReplyTo() const throw (TipcException);
    void setReplyTo(const char * subj) throw (TipcException);
    //@}


    /**
     * Accessor methods for the message reply-to property.  The
     * reply-to destination property
     * is a string representing a subject where a reply to
     * the message should be sent.
     */
    //@{
    real8 getExpiration() const throw (TipcException);
    void setExpiration(real8 expiration) throw (TipcException);
    //@}



    /**
     * Accessor methods for the message sender timestamp property.  A value
     * of 0.0 indicates
     * that the sender timestamp property was not set.
     */
    //@{
    real8 getSenderTimestamp() const throw (TipcException);
    void setSenderTimestamp(real8 timestamp) throw (TipcException);

    //@}


    /**
     * Accessor methods for the message arrival timestamp property.
     * As messages are read, their arrival timestamp property
     * will automatically be set.
     * A value of 0.0 indicates that the arrival
     * timestamp property was not set.
     */
    //@{
    real8 getArrivalTimestamp() const throw (TipcException);
    void setArrivalTimestamp(real8 timestamp) throw (TipcException);
    //@}


    /**
     * Encrypts the current message.  This method first checks to see if the
     * message's message type
     * has a key registered for it. If it does, it calls the
     * encrypt function registered for
     * that message. That encryption function encrypts the function.
     * Note that this modifies the current object; therefore, after calling
     * encrypt(), a lot of
     * the data accessor methods will not work properly.
     * @see TipcMt::setEncrypt()
     * @see TipcMt::setKey()
     */
    void encrypt() throw (TipcException);

    /**
     * Decrypts the current message if it has been previously encrypted.
     * This method first checks
     * to see if the message's message type has a key registered for it. If
     * it does,
     * decrypt() calls the decrypt function registered for that message
     * type, and that decryption
     * function decrypts the message.
     *
     * @warning If the message type has an encrypt function associated
     * with it, it needs a
     * corresponding decrypt function to decrypt it.
     * @see TipcMt::setDecrypt()
     * @see TipcMt::setKey()
     */
    void decrypt() throw (TipcException);

    /**
     * Acknowledges successful delivery of a message by
     * a receiving process. When a
     * message with a delivery mode
     * of T_IPC_DELIVERY_SOME or T_IPC_DELIVERY_ALL is
     * sent through a connection with TipcConnMsgSend, the sender saves
     * a copy of the message
     * until some (in the case of T_IPC_DELIVERY_SOME) or all (in the case of
     * T_IPC_DELIVERY_ALL) receiving processes acknowledge
     * that they have received the
     * message.
     * ~TipcMsg() automatically calls ack() to ensure a received message is
     * acknowledged before it is destroyed. ~TipcMsg()
     * assumes that when a receiving
     * process destroys a message, it is finished processing the message
     * and that the message can
     * be considered to have been successfully
     * delivered to this receiving process.
     * Ack() first updates the highest sequence number (based
     * on the sequence number and
     * sender of msg) in the GMD area of the connection the message was
     * received on.  The highest
     * sequence number is used to detect duplicate messages
     * . Ack() then creates a
     * GMD_ACK message, appends an INT4 integer field containing
     * the sequence number of the
     * message being acknowledged, and sends the GMD_ACK message
     * with TipcConn::send()
     * through the same connection the message was received on.
     * If the message's delivery mode is either T_IPC_DELIVERY_SOME or
     * T_IPC_DELIVERY_ALL and the message has
     * not already been acknowledged manually
     * with ack(), then ~TipcMsg() first calls ack() to acknowledge successful
     * delivery of the message to this receiving process.
     */

    void ack() throw (TipcException);



    /**
     * Insertion operator.  Provides a one to one mapping to the
     * append() and addNamed() methods; (note that no
     * compatibility was provided for the
     * appendPtr(), addNamedPtr(), appendUtf8*(),
     * and addNamedUtf8*() methods since
     * there's no way
     * to distinguish the signature).
     * In order to provide full compatibility with the append and addNamed
     * () methods, two modifiers
     * (setSize() and setName()) were created:
     *
     * - setSize() allows the size of a binary or array field to be set;
     * setSize() should be called
     * before inserting a binary or array field:
     * @code
     *
     *    int4 int4Array [100];
     *    ...
     *    msg << setSize(100) << int4Array;
     *
     * @endcode
     *
     * - setName() allows a name to be given to the last appended field;
     * setName() should be called
     * after inserting a field:
     * @code
     *
     *  real8 real8Array [1000];
     *  ...
     *  msg << setSize(100) << real8Array << setName("named real8 array");
     *
     * @endcode
     */
    //@{
    TipcMsg& operator<<(bool arg)
    throw (TipcException);    /* bool       */

    TipcMsg& operator<<(const bool* arg)
    throw (TipcException);    /* bool array */

    TipcMsg& operator<<(const void *  arg)
    throw (TipcException);    /* binary */
    TipcMsg& operator<<(char arg)
    throw (TipcException);    /* char */
    TipcMsg& operator<<(int2 arg)
    throw (TipcException);    /* int2 */
    TipcMsg& operator<<(const int2* arg)
    throw (TipcException);    /* int2 array */
    TipcMsg& operator<<(int4 arg)
    throw (TipcException);    /* int4 */
    TipcMsg& operator<<(const int4* arg)
    throw (TipcException);    /* int4 array */
    TipcMsg& operator<<(int8 arg)
    throw (TipcException);    /* int8 */
    TipcMsg& operator<<(const int8* arg)
    throw (TipcException);    /* int8 array */
    TipcMsg& operator<<(const TipcMsg& arg)
    throw (TipcException);    /* msg */
    TipcMsg& operator<<(const TipcMsg* arg)
    throw (TipcException);    /* msg array */
    TipcMsg& operator<<(real4 arg)
    throw (TipcException);    /* real4 */
    TipcMsg& operator<<(const real4* arg)
    throw (TipcException);    /* real4 array */
    TipcMsg& operator<<(real8 arg)
    throw (TipcException);    /* real8 */
    TipcMsg& operator<<(const real8* arg)
    throw (TipcException);    /* real8 array */
    TipcMsg& operator<<(real16 arg)
    throw (TipcException);    /* real16 */
    TipcMsg& operator<<(const real16* arg)
    throw (TipcException);    /* real16 array */
    TipcMsg& operator<<(const char * arg)
    throw (TipcException);    /* str */
    TipcMsg& operator<<(const char ** arg)
    throw (TipcException);    /* str array */
    TipcMsg& operator<<(const XML& arg)
    throw (TipcException);    /* XML */
    TipcMsg& operator<<(unsigned char arg)
    throw (TipcException);    /* byte */

    //@}


    /**
     * Extraction operator.  Provides a one to one mapping to all the
     * next*() and getNameCurrent() methods (note that no compatibility was provided for the
     * getNamed*() methods as that implies random access, and >> operators are sequencial in nature).
     *
     * @warning The memory of the return value extracted should NOT BE
     * FREED as it belongs to the TipcMsg class.
     *
     * In order to provide full compatibility with the next*() and getNamed*() methods ,
     * two modifiers (getSize() and getName()) were created:
     *
     * - getSize() allows the size of the last binary or array field extracted to be obtained;
     * getSize() should be called after extracting a binary or array field:
     * @code
     * const int4* int4Array =(const int4*) NULL;
     * int4 size;
     * ...
     * msg >> int4Array >> getSize(size);
     * @endcode
     *
     * - getName() allows the name of the next field to be extracted to be obtained. Note that
     * this modifier should only be used to obtain the name of the current field, and not to
     * determine which field to obtain:
     * @code
     * const real8* real8Array;
     * const char*  name;
     * int4 size;
     * ...
     * msg >> getName(name) >> real8Array >> getSize(size);
     * @endcode
     */
    //@{

    const TipcMsg& operator>>(bool& arg) const
    throw (TipcException);   /* "bool" */
    const TipcMsg& operator>>(const void * & arg) const
    throw (TipcException);   /* binary */
    const TipcMsg& operator>>(char& arg) const
    throw (TipcException);    /* const char */
    const TipcMsg& operator>>(int2& arg) const
    throw (TipcException);    /* int2 */
    const TipcMsg& operator>>(const int2*& arg) const
    throw (TipcException); /* int2 array */
    const TipcMsg& operator>>(int4& arg) const
    throw (TipcException);    /* int4 */
    const TipcMsg& operator>>(const int4*& arg) const
    throw (TipcException);    /* int4 array */
    const TipcMsg& operator>>(int8& arg) const
    throw (TipcException);    /* int8 */
    const TipcMsg& operator>>(const int8*& arg) const
    throw (TipcException);    /* int8 array */
    const TipcMsg& operator>>(TipcMsg& arg) const
    throw (TipcException);    /* msg */
    const TipcMsg& operator>>(real4& arg) const
    throw (TipcException);    /* real4 */
    const TipcMsg& operator>>(const real4*& arg) const
    throw (TipcException);    /* real4 array */
    const TipcMsg& operator>>(real8& arg) const
    throw (TipcException);    /* real8 */
    const TipcMsg& operator>>(const real8*& arg) const
    throw (TipcException);    /* real8 array */
    const TipcMsg& operator>>(real16& arg) const
    throw (TipcException);    /* real16 */
    const TipcMsg& operator>>(const real16*& arg) const
    throw (TipcException);    /* real16 array */
    const TipcMsg& operator>>(const char *& arg) const
    throw (TipcException);    /* str */
    const TipcMsg& operator>>(const char **& arg) const
    throw (TipcException);    /* str array */
    const TipcMsg& operator>>(XML& arg) const
    throw (TipcException);    /* XML */
    const TipcMsg& operator>>(unsigned char& arg) const
    throw (TipcException);    /* byte */

    //@}

    /**
     * Extraction operator.  Provides a one to one mapping
     * to the
     * next*() and getNameCurrent() methods (note that no
     * compatibility was provided for the
     * getNamed*() method as that implies random access, and >>
     * operators are sequencial in nature).
     *
     * @warning The memory of the return value extracted MUST BE
     * FREED using the delete[] operator as it does not belong
     * to the TipcMsg class.
     *
     * In order to provide full compatibility with the next*()
     * and getNamed*() methods ,
     * two modifiers (getSize() and getName()) were created:
     *
     * - getSize() allows the size of the last binary or array
     * field extracted to be obtained;
     * getSize() should be called after extracting a binary
     * or array field:
     * @code
     * bool* boolArray =(bool*) NULL;
     * TipcMsg* msgArray = (TipcMsg*) NULL;
     * int4 size1,size2;
     * ...
     * msg >> boolArray >> getSize(size1)
     *     >> msgArray  >> getSize(size2);
     * ...
     * delete [] boolArray;
     * delete [] msgArray;
     * @endcode
     *
     * - getName() allows the name of the next field to be
     * extracted to be obtained. Note that
     * this modifier should only be used to obtain the name
     * of the current field, and not to
     * determine which field to obtain:
     * @code
     * const real8* real8Array;
     * const char*  name;
     * int4 size;
     * ...
     * msg >> getName(name) >> real8Array >> getSize(size);
     * @endcode
     */
    //@{
    const TipcMsg& operator>>(TipcMsg*& arg) const
    throw (TipcException);    /* msg array */

    const TipcMsg& operator>>(bool*& arg) const
    throw (TipcException);   /* "bool array" */


    //@}


    /**
     * Method used to sequentially insert fields without a name into a message.
     * Fields added to a message using the append() method should only be extracted using the
     * appropriate next*() method; it is not advisable to mix append() calls with calls to
     * addNamed(); in general, either messages have all the fields named, or none at all.
     */
    //@{
    void append(const void *  binary_data, int4 binary_size)
    throw (TipcException);
    void append(char char_data)
    throw (TipcException);
    void append(int2 int2_data)
    throw (TipcException);
    void append(const int2* int2_array_data, int4 int2_array_size)
    throw (TipcException);
    void append(int4 int4_data)
    throw (TipcException);
    void append(const int4* int4_array_data, int4 int4_array_size)
    throw (TipcException);
    void append(int8 int8_data)
    throw (TipcException);
    void append(const int8 *int8_array_data, int4 int8_array_size)
    throw (TipcException);
    void append(const TipcMsg& msg_data)
    throw (TipcException);
    void append(const TipcMsg* msg_array_data, int4 msg_array_size)
    throw (TipcException);
    void append(real4 real4_data)
    throw (TipcException);
    void append(const real4* real4_array_data, int4 real4_array_size)
    throw (TipcException);
    void append(real8 real8_data)
    throw (TipcException);
    void append(const real8* real8_array_data, int4 real8_array_size)
    throw (TipcException);
    void append(real16 real16_data)
    throw (TipcException);
    void append(const real16 *real16_array_data, int4 real16_array_size)
    throw (TipcException);
    void append(const char * str_data)
    throw (TipcException);
    void append(const char ** str_array_data, int4 str_array_size)
    throw (TipcException);
    void append(const char * str_data, real8 real8_data)
    throw (TipcException);
    void append(const XML& xml_data)
    throw (TipcException);
    void append(unsigned char byte_data)
    throw (TipcException);
    void append(bool bool_data)
    throw (TipcException);
    void append(const bool* bool_array_data, int4 bool_array_size)
    throw (TipcException);
    //@}

    /**
     * Method used to append Timestamp and Timestamp Array fields to the
     * message
     */
    //@{
    void appendTimestamp(real8 timestamp_data)
    throw (TipcException);
    void appendTimestampArray(const real8 *timestamp_data,
                              int4 real8_array_size)
    throw (TipcException);
    //@}

    /**
     * Method used to sequentially insert UTF8 string fields without a name into a message.
     * Fields added to a message using the append() method should only be extracted using the
     * appropriate next*() method; it is not advisable to mix append() calls with calls to
     * addNamed(); in general, either messages have all the fields named, or none at all.
     */
    //@{

    void appendUtf8(const char * str_data)
    throw (TipcException);
    void appendUtf8(const char ** str_array_data, int4 str_array_size)
    throw (TipcException);
    //@}


    /**
     * Method used to sequentially insert unnamed fields without making a deep copy into a message.
     * Fields added to a message using the appendPtr() method should only be extracted using the
     * appropriate next*() method; it is not advisable to mix append() calls with calls to
     * addNamed(); in general, either messages have all the fields named, or none at all.  This
     * method does not make a copy of data. Instead, the supplied pointer to the data is entered
     * directly into the new message field's internal data structure. The caller is responsible for
     * making sure that the pointer remains valid for the lifetime of the message field, and for
     * freeing it, if necessary, when the message field has been destroyed.
     *
     * One advantage of TipcMsg::appendPtr() over TipcMsg::append() is that the data can be
     * modified after the field is appended, but before the message is sent. Also, the overhead of
     * one copy operation is avoided, because the data is still copied into the connection message
     * buffer when the message is sent.  To update the pointer to point to a new memory location, use
     * the appropriate TipcMsg::updatePtr() method.
     * @return a T_IPC_MSG_FIELD object that can be used to update individual fields.
     * @see updatePtr().
     */
    //@{
    T_IPC_MSG_FIELD appendPtr(const void *  binary_data, int4 binary_data_size)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(const int2 *int2_array_data, int4 int2_array_size)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(const int4 *int4_array_data, int4 int4_array_size)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(const int8 *int8_array_data, int4 int8_array_size)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(const TipcMsg& msg_data)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(const TipcMsg* arg, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(const real4 *real4_array_data, int4 real4_array_size)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(const real8 *real8_array_data, int4 real8_array_size)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(const real16 *real16_array_data, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(const char * str_data)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(const char * *str_array_data, int4 str_array_size)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(const XML& xml_data)
    throw (TipcException);

    T_IPC_MSG_FIELD appendPtr(bool *bool_array_data, int4 bool_array_size)
    throw (TipcException);
    //@}

    /**
     * Method uses to append Timestamp filed without making a deep copy
     * into a message.
     */
    //@{
    T_IPC_MSG_FIELD appendTimestampArrayPtr(const real8 *timestamp_array_data,
                                            int4 timestamp_array_size)
    throw (TipcException);
    //@}

    /**
     * Method used to sequentially insert unnamed UTF8 string fields without making a deep copy
     * into a message.
     * Fields added to a message using the appendUtf8Ptr() method should only be extracted using
     * the appropriate next*() method; it is not advisable to mix append() calls with calls to
     * addNamed(); in general, either messages have all the fields named, or none at all.  This
     * method does not make a copy of data. Instead, the supplied pointer to the data is entered
     * directly into the new message field's internal data structure. The caller is responsible for
     * making sure that the pointer remains valid for the lifetime of the message field, and for
     * freeing it, if necessary, when the message field has been destroyed.
     *
     * One advantage of TipcMsg::appendUtf8Ptr() over TipcMsg::appendUtf8() is that the data can be
     * modified after the field is appended, but before the message is sent. Also, the overhead of
     * one copy operation is avoided, because the data is still copied into the connection message
     * buffer when the message is sent.  To update the pointer to point to a new memory location, use
     * the appropriate TipcMsg::updateUtf8Ptr() method.
     * @return a T_IPC_MSG_FIELD object that can be used to update individual fields.
     * @see updatePtr().
     */
    //@{
    T_IPC_MSG_FIELD appendUtf8Ptr(const char * str_data)
    throw (TipcException);

    T_IPC_MSG_FIELD appendUtf8Ptr(const char * *str_array_data, int4 str_array_size)
    throw (TipcException);


    //@}

    /**
     * Method used to append an unknown field type into a message.
     */
    void appendUnknown(T_IPC_FT field_type)
    throw (TipcException);

    /**
     * Method used to update a field previously appended to the message using one of the
     * TipcMsg::appendPtr() methods. The contents of the pointer are not copied. Instead, the supplied
     * pointer is entered directly into the message field's internal data structure. The caller is
     * responsible for making sure that the pointer remains valid for the lifetime of the message
     * field, and for freeing it, if necessary, when the message field has been destroyed.
     * @throw TipcMsgException if the field cannot be updated.
     */
    //@{

    void updatePtr(T_IPC_MSG_FIELD field, const void *  binary_data,
                   int4 binary_data_size)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const int2 *int2_array_data,
                   int4 int2_array_size)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const int4 *int4_array_data,
                   int4 int4_array_size)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const int8 *int8_array_data,
                   int4 int8_array_size)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const TipcMsg& msg_data)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const TipcMsg* msg_array_data,
                   int4 msg_array_size)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const real4 *real4_array_data,
                   int4 real4_array_size)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const real8 *real8_array_data,
                   int4 real8_array_size)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const real16 *real16_array_data,
                   int4 size)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const char * str_data)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const char * *str_array_data,
                   int4 str_array_size)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const XML& xml_data)
    throw (TipcException);

    void updatePtr(T_IPC_MSG_FIELD field, const bool *bool_array_data,
                   int4 bool_array_size)
    throw (TipcException);

    void updateTimestampArrayPtr(T_IPC_MSG_FIELD field,
                                 const real8 * real8_array_data,
                                 int4 real8_array_size)
    throw (TipcException);

    void updateUtf8Ptr(T_IPC_MSG_FIELD field, const char * str_data)
    throw (TipcException);

    void updateUtf8Ptr(T_IPC_MSG_FIELD field, const char * *str_array_data,
                       int4 str_array_size)
    throw (TipcException);
    //@}


    /**
     * Method to sequentially extract data from the message.  If this method succeeds, it advances the
     * current field to the next field in the message.  Before extracting any fields from a message, it
     * is advisable to set the current field to zero (or whatever field to be extracted).
     *
     * @warning the memory returned by each method belongs to the message, and therefore, SHOULD NOT
     * BE DELETED OR CHANGED.
     *
     * @throw TipcMsgException if the field cannot be extracted (this usually happens if attempting to
     * go past the last field in the message).
     */
    //@{
    const void* nextBinary(int4& size) const
    throw (TipcException);

    char nextChar() const
    throw (TipcException);

    int2 nextInt2() const
    throw (TipcException);

    const int2* nextInt2Array(int4& size) const
    throw (TipcException);

    int4 nextInt4() const
    throw (TipcException);

    const int4* nextInt4Array(int4& size) const
    throw (TipcException);

    int8 nextInt8() const
    throw (TipcException);

    const int8* nextInt8Array(int4& size) const
    throw (TipcException);

    const TipcMsg nextMsg() const
    throw (TipcException);


    real4 nextReal4() const
    throw (TipcException);

    const real4* nextReal4Array(int4& size) const
    throw (TipcException);

    real8 nextReal8() const
    throw (TipcException);

    const real8* nextReal8Array(int4& size) const
    throw (TipcException);

    real16 nextReal16()  const
    throw (TipcException);

    const real16* nextReal16Array(int4& size) const
    throw (TipcException);

    const char* nextString()  const
    throw (TipcException);

    const char** nextStringArray(int4& size) const
    throw (TipcException);

    const XML nextXML()  const
    throw (TipcException);

    unsigned char nextByte() const
    throw (TipcException);

    bool nextBool() const
    throw (TipcException);

    real8 nextTimestamp() const
    throw (TipcException);

    const real8* nextTimestampArray(int4& size) const
    throw (TipcException);

    const char* nextUtf8() const
    throw (TipcException);

    const char** nextUtf8Array(int4 &size) const
    throw (TipcException);

    //@}

    /**
     * Method to sequentially extract an array of messages from the message.  If this method succeeds,
     * it advances the current field to the next field in the message.  Before extracting any fields from
     * a message, it is advisable to set the current field to zero (or whatever field to be extracted).
     *
     * @warning Unlike with the other next*() methods, the value returned by this method does not belong
     * to the TipcMsg class; therefore, it SHOULD BE FREED with the delete[] operator.
     *
     * @throw TipcMsgException if the field cannot be extracted (this usually happens if attempting to
     * go past the last field in the message).
     */
    TipcMsg* nextMsgArray(int4& size) const
    throw (TipcException);



    /**
     * Method to sequentially extract an array of bools from the message.  If this method succeeds,
     * it advances the current field to the next field in the message.  Before extracting any fields from
     * a message, it is advisable to set the current field to zero (or whatever field to be extracted).
     *
     * @warning Unlike with the other next*() methods, the value returned by this method does not belong
     * to the TipcMsg class; therefore, it SHOULD BE FREED with the delete[] operator.
     *
     * @throw TipcMsgException if the field cannot be extracted (this usually happens if attempting to
     * go past the last field in the message).
     */
    bool* nextBoolArray(int4 &size) const
    throw (TipcException);

#if 0

    /**
     * Alternate method to sequentially extract an array of
     * booleans from the message using iterators.
     * If this method succeeds, it advances the current
     * field to the next field in the message.
     * Before extracting any fields from a message, it is
     * advisable to set the current field to zero (or
     * whatever field to be  extracted).
     * @par Example:
     * Following is a brief example of how this method should
     * be used:
     * @code
     *
     *
     * #include <vector>
     * #include <iterator>
     * #include <iostream>
     *
     * ...
     * TipcMsg testMsg;
     * vector <bool> payload;
     *
     * testMsg.nextBoolArray (
     *    back_insert_iterator <vector <bool> > (payload));
     *
     * cout << "Payload size = "
     *      << payload.end () - payload.begin () << endl;
     *
     * @endcode
     *
     * @throw TipcMsgException if the field cannot be extracted
     * (this usually happens if attempting to
     * go past the last field in the message).
     */
    template <class InsertIterator> void nextBoolArray (InsertIterator it)
    throw (TipcException)
    {
      T_BOOL *c_api_bool_array;
      int4 sizeReturn
      if (!TipcMsgNextMsgArray(_msg, &c_api_bool_array, &sizeReturn)) {
        throw (TipcException);
      }

      for (int4 i = 0; i < sizeReturn; i ++) {
        *it = c_api_bool_array[i];
      }
    }

    /**
     * Alternate method to sequentially extract an array of messages from the message using iterators.
     * If this method succeeds, it advances the current field to the next field in the message.
     * Before extracting any fields from a message, it is advisable to set the current field to zero (or
     * whatever field to be  extracted).
     * @par Example:
     * Following is a brief example of how this method should be used:
     * @code
     *
     *
     * #include <vector>
     * #include <iterator>
     * #include <iostream>
     *
     * ...
     * TipcMsg testMsg;
     * vector <TipcMsg> payload;
     *
     * testMsg.NextMsgArray (
     *    back_insert_iterator <vector <TipcMsg> > (payload));
     *
     * cout << "Payload size = "
     *      << payload.end () - payload.begin () << endl;
     *
     * @endcode
     *
     * @throw TipcMsgException if the field cannot be extracted (this usually happens if attempting to
     * go past the last field in the message).
     */
    template <class InsertIterator> void nextMsgArray (InsertIterator it)
    throw (TipcException)
    {
      T_IPC_MSG *c_api_msg_array;
      int4 sizeReturn
      if (!TipcMsgNextMsgArray(_msg, &c_api_msg_array, &sizeReturn)) {
        throw (TipcException);
      }

      for (int4 i = 0; i < sizeReturn; i ++) {
        *it = c_api_msg_array[i];
      }
    }
#endif

    /**
     * Extract the next field as an unknown field.
     * @throw TipcMsgException if the field cannot be extracted (this
     * usually happens if attempting to
     * go past the last field in the message, or if the field isn't unknown).
     * @see isCurrentFieldKnown()
     */
    void nextUnknown() const
    throw (TipcException);



    /**
     * Inserts a named field into a message.
     * @warning Named fields should not be mixed with unnamed fields; use
     * extreme caution if doing so.  Messages using named fields should
     * always have a grammar of verbose.
     * @throw TipcMsgException if the name already exists in the message.
     */
    //@{
    void addNamed(const char * name, const void *  value, int4 size)
    throw (TipcException);


    void addNamed(const char * name, char value)
    throw (TipcException);

    void addNamed(const char * name, int2 value)
    throw (TipcException);

    void addNamed(const char * name, const int2 *value, int4 size)
    throw (TipcException);

    void addNamed(const char * name, int4 value)
    throw (TipcException);

    void addNamed(const char * name, const int4 *value, int4 size)
    throw (TipcException);


    void addNamed(const char * name, int8 value)
    throw (TipcException);

    void addNamed(const char * name, const int8 *value, int4 size)
    throw (TipcException);
    void addNamed(const char * name, const TipcMsg& value)
    throw (TipcException);

    void addNamed(const char * name, const TipcMsg *value, int4 size)
    throw (TipcException);
    void addNamed(const char * name, real4 value)
    throw (TipcException);

    void addNamed(const char * name, const real4 *value, int4 size)
    throw (TipcException);

    void addNamed(const char * name, real8 value)
    throw (TipcException);

    void addNamed(const char * name, const real8 *value, int4 size)
    throw (TipcException);

    void addNamed(const char * name, real16 value)
    throw (TipcException);

    void addNamed(const char * name, const real16 *value, int4 size)
    throw (TipcException);
    void addNamed(const char * name, const char * value)
    throw (TipcException);

    void addNamed(const char * name, const char * *value, int4 size)
    throw (TipcException);

    void addNamed(const char * name, const XML& value)
    throw (TipcException);

    void addNamed(const char * name, unsigned char value)
    throw (TipcException);

    void addNamed(const char * name, bool value)
    throw (TipcException);

    void addNamed(const char * name, const bool *value, int4 size)
    throw (TipcException);

    void addNamedTimestamp(const char * name, real8 value)
    throw (TipcException);

    void addNamedTimestampArray(const char *name, const real8 *value, int4 size)
    throw (TipcException);

    void addNamedUtf8(const char * name, const char * value)
    throw (TipcException);

    void addNamedUtf8Array(const char * name, const char * *value, int4 size)
    throw (TipcException);

    void addNamedUnknown(const char * name, T_IPC_FT field_type)
    throw (TipcException);

    //@}

    /**
     * Inserts a named field into a message.  Named fields should not be mixed with unnamed fields; use
     * extreme caution if doing so.  Messages using named fields should always have a grammar of verbose.
     *
     * This method does not make a copy of data. Instead, the supplied pointer to the data is entered
     * directly into the new message field's internal data structure. The caller is responsible for
     * making sure that the pointer remains valid for the lifetime of the message field, and for
     * freeing it, if necessary, when the message field has been destroyed.
     *
     * One advantage of TipcMsg::addNamedPtr() over TipcMsg::addNamed() is that the  overhead of
     * one copy operation is avoided, because the data is still copied into the connection message
     * buffer when the message is sent.  To update the pointer to point to a new memory location, use
     * the appropriate TipcMsg::updateNamedPtr() method.
     * @return a T_IPC_MSG_FIELD object that can be used to update individual fields without doing deep
     * copies.
     * @throw TipcMsgException if the name already exists in the message.
     */
    //@{
    T_IPC_MSG_FIELD addNamedPtr(const char * name, const bool *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedPtr(const char * name, const XML& value)
    throw (TipcException);


    T_IPC_MSG_FIELD addNamedPtr(const char * name, const real16 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedPtr(const char * name, const real8 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedPtr(const char * name, const real4 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedPtr(const char * name, const TipcMsg *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedPtr(const char * name, const TipcMsg& value) /* YX 06/21/2000 */
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedPtr(const char * name, const int4 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedPtr(const char * name, const void *  value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedPtr(const char * name, const int2 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedPtr(const char * name, const int8 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedPtr(const char * name, const char * *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedPtr(const char * name, const char * value)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedTimestampArrayPtr(const char * name,
                                              const real8 *value,
                                              int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedUtf8ArrayPtr(const char * name, const char * *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD addNamedUtf8Ptr(const char * name, const char * value)
    throw (TipcException);

    //@}

    /**
     * Extracts a named field from a message.
     * @warning Named fields should not be mixed with unnamed fields; use
     * extreme caution if doing so.
     * @warning Messages using named fields should always have a grammar of verbose.
     * @warning The memory returned should NOT BE FREED; it belongs to this
     * class.
     * @throw TipcMsgException if the name doesn't exists in the message, or if the given name was a
     * NULL pointer, or if the wrong type extraction is attempted.
     * @see getTypeNamed()
     */
    //@{
    const void* getNamedBinary(const char * name, int4& size)  const
    throw (TipcException);

    char getNamedChar(const char * name) const
    throw (TipcException);

    int2 getNamedInt2(const char * name)  const
    throw (TipcException);

    const int2* getNamedInt2Array(const char * name, int4& size) const
    throw (TipcException);

    int4 getNamedInt4(const char * name) const
    throw (TipcException);

    const int4* getNamedInt4Array(const char * name, int4& size) const
    throw (TipcException);

    int8 getNamedInt8(const char * name) const
    throw (TipcException);

    const int8* getNamedInt8Array(const char * name, int4& size) const
    throw (TipcException);

    const TipcMsg getNamedMsg(const char * name) const
    throw (TipcException);


    real4 getNamedReal4(const char * name) const
    throw (TipcException);

    const real4* getNamedReal4Array(const char * name, int4& size) const
    throw (TipcException);

    real8 getNamedReal8(const char * name) const
    throw (TipcException);

    const real8* getNamedReal8Array(const char * name, int4& size) const
    throw (TipcException);

    real16 getNamedReal16(const char * name) const
    throw (TipcException);

    const real16* getNamedReal16Array(const char * name, int4& size) const
    throw (TipcException);

    const char* getNamedString(const char * name) const
    throw (TipcException);

    const char** getNamedStringArray(const char * name, int4& size) const
    throw (TipcException);

    const XML getNamedXML(const char * name) const
    throw (TipcException);

    unsigned char getNamedByte(const char * name) const
    throw (TipcException);

    bool getNamedBool(const char * name) const
    throw (TipcException);

    real8 getNamedTimestamp(const char * name) const
    throw (TipcException);

    const real8 *getNamedTimestampArray(const char * name, int4& size) const
    throw (TipcException);

    const char* getNamedUtf8(const char * name) const
    throw (TipcException);

    const char** getNamedUtf8Array(const char * name, int4& size) const
    throw (TipcException);

    void getNamedUnknown(const char * name) const
    throw (TipcException);

    //@}


    /**
     * Extracts a named field from a message.
     * @warning Named fields should not be mixed with unnamed fields; use
     * extreme caution if doing so.
     * @warning Messages using named fields should always have a grammar of verbose.
     * @warning The memory returned MUST BE FREED using the detele[];
     * operator it belongs to this class.  The following example shows
     * how to do it:
     * @code
     * TipcMsg msg;
     * TipcMsg msgArr[2] = { TipcMsg(), TipcMsg ("info") };
     *
     * msg.addNamed("msg array", msgArr, 2);
     * ...
     * msg.setCurrent(0);
     * int4 size;
     * TipcMsg* msgArrExtracted = getNamedMsgArray("msg array",size);
     * ...
     * delete[] msgArrExtracted;
     * @endcode
     * @throw TipcMsgException if the name doesn't exists in the message, or if the given name was a
     * NULL pointer, or if the wrong type extraction is attempted.
     * @see getTypeNamed()
     */
    TipcMsg* getNamedMsgArray(const char * name, int4& size) const
    throw (TipcException);


    /**
     * Extracts a named field from a message.
     * @warning Named fields should not be mixed with unnamed fields; use
     * extreme caution if doing so.
     * @warning Messages using named fields should always have a grammar of verbose.
     * @warning The memory returned MUST BE FREED using the detele[];
     * operator it belongs to this class.  The following example shows
     * how to do it:
     * @code
     * TipcMsg msg;
     * bool booArray[2] = { true, false };
     *
     * msg.addNamed("bool array", boolArray, 2);
     * ...
     * msg.setCurrent(0);
     * int4 size;
     * bool* boolExtracted = getNamedMsgArray("bool array",size);
     * ...
     * delete[] boolExtracted;
     * @endcode
     * @throw TipcMsgException if the name doesn't exists in the message, or if the given name was a
     * NULL pointer, or if the wrong type extraction is attempted.
     * @see getTypeNamed()
     */

    bool* getNamedBoolArray(const char * name, int4& size) const
    throw (TipcException);



    /**
     * Gets the type of a given named field.  This is useful to prevent a TipcMsgException from being
     * thrown by the getNamed*() methods.
     * @throw TipcMsgException if the name doens't exist, or if the name is a NULL pointer.
     */
    T_IPC_FT getTypeNamed(const char * name) const
    throw (TipcException);


    /**
     * Determines whether or not a field with the given name exists in the message.
     */
    bool existsNamed(const char * name) const
    throw (TipcException);

    /**
     * Sets the name of the current field in the message.
     */
    void setNameCurrent(const char * name)
    throw (TipcException);

    /**
     * Gets the name of the current field of the message.
     */
    const char* getNameCurrent() const
    throw (TipcException);


    /**
     *  Deletes the current field of the message.
     */
    void deleteCurrent()
    throw (TipcException);

    /**
     * Deletes a specific field of the message.
     */
    void deleteField(int4 index)
    throw (TipcException);

    /**
     * Deletes a named field of the message.
     */
    void deleteNamed(const char * name) /* delete named field */
    throw (TipcException);
    /**
     * Updates a named field appended by using the addNamedPtr() method in a message.
     * @throw TipcMsgException if the value cannot be updated because of a type mismatch or an inexistant
     * name.
     */
    //@{
    void updateNamed(const char * name, const void *  value, int4 size)
    throw (TipcException);
    void updateNamed(const char * name, char value)
    throw (TipcException);

    void updateNamed(const char * name, int2 value)
    throw (TipcException);

    void updateNamed(const char * name, const int2 *value, int4 size)
    throw (TipcException);

    void updateNamed(const char * name, int4 value)
    throw (TipcException);
    void updateNamed(const char * name, const int4 *value, int4 size)
    throw (TipcException);

    void updateNamed(const char * name, int8 value)
    throw (TipcException);
    void updateNamed(const char * name, const int8 *value, int4 size)
    throw (TipcException);

    void updateNamed(const char * name, const TipcMsg& value)
    throw (TipcException);
    void updateNamed(const char * name, const TipcMsg *value, int4 size)
    throw (TipcException);
    void updateNamed(const char * name, real4 value)
    throw (TipcException);
    void updateNamed(const char * name, const real4 *value, int4 size)
    throw (TipcException);
    void updateNamed(const char * name, real8 value)
    throw (TipcException);
    void updateNamed(const char * name, const real8 *value, int4 size)
    throw (TipcException);
    void updateNamed(const char * name, real16 value)
    throw (TipcException);

    void updateNamed(const char * name, const real16 *value, int4 size)
    throw (TipcException);

    void updateNamed(const char * name, const char * value)
    throw (TipcException);

    void updateNamed(const char * name, const char * *value, int4 size)
    throw (TipcException);

    void updateNamed(const char * name, const XML& value)
    throw (TipcException);

    void updateNamed(const char * name, unsigned char value)
    throw (TipcException);

    void updateNamed(const char * name, bool value)
    throw (TipcException);
    void updateNamed(const char * name, const bool *value, int4 size)
    throw (TipcException);

    void updateNamedTimestamp(const char * name, real8 value)
    throw (TipcException);

    void updateNamedTimestampArray(const char *name,
                                   const real8 *value,
                                   int4 size)
    throw (TipcException);

    void updateNamedUtf8(const char * name, const char * value)
    throw (TipcException);

    void updateNamedUtf8(const char * name, const char **value, int4 size)
    throw (TipcException);

    //@}

    /**
     * Updates a named field appended by using the addNamedPtr() method in a message.  The contents of
     * the pointer are not copied. Instead, the supplied pointer is entered directly into the message
     * field's internal data structure. The caller is responsible for making sure that the pointer
     * remains valid for the lifetime of the message field, and for freeing it, if necessary, when the
     * message field has been destroyed.
     * @throw TipcMsgException if the value cannot be updated because of a type mismatch or an inexistant
     * name.
     */
    //@{
    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const int8 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const int4 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const int2 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const void *  value, int4 size)
    throw (TipcException);


    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const TipcMsg *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const TipcMsg& value)
    throw (TipcException);


    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const real4 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const real8 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const real16 *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const char * *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const char * value)
    throw (TipcException);


    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const XML& value)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedPtr(const char * name, const bool *value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedTimestampArrayPtr(const char * name,
                                                 const real8 * value,
                                                 int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedUtf8Ptr(const char * name, const char ** value, int4 size)
    throw (TipcException);

    T_IPC_MSG_FIELD updateNamedUtf8Ptr(const char * name, const char * value)
    throw (TipcException);

    //@}

    /**
     * Prints the contents of the message using the given method.
     * @param out_func pointer to a static method with the following signature:
     *  void func( char* formatStr, ...); the default value is TutOut, which outputs
     * data to standard out.  Note: on Windows platforms the function should be
     * declared as follows: void T_ENTRY_VA T_EXPORT func (T_STR format_str,...);
     *
     */
    void print(T_OUT_FUNC out_func = (T_OUT_FUNC) ::TutOut) const
    throw (TipcException);

    /**
     * Resets the data contents of the message.  This is a convenience
     * method that sets the number of
     * fields in a message to zero, therefore, allowing the same message to
     * be reused.  This is useful
     * for publishing processes that publish the same message over and over
     * with the same header properties,
     * but different data payloads each time.  If only some fields of a
     * message will change, try to make
     * those the last few fields in the message, and call the setNumFields()
     * method.
     */
    void reset()        throw (TipcException);

  };


#if 1


  /**
   * Insertion operator that streams a TipcMsg object into an ostream-like class.
   * @warning The data is binary and not text; for outputing this to stdout,
   * please use the TipcMsg::print() method.
   *
   * @par Example:
   * The following example streams a TipcMsg into a binary file:
   * @code
   *  SmartSockets::TipcMsg msg;
   *  msg.setDest("blah");
   *  msg.append((int4) 456);
   *  msg.append((real8) 456.433);
   *  msg.append("aaaaa");
   *
   *  std::ofstream outFile ("file.out", std::ios_base::binary);
   *  outFile << msg ;
   *  outFile.close();
   * @endcode
   */
  template <class OutputStream>
  inline OutputStream& operator << (OutputStream& os,
                                    const TipcMsg& msgArg )
  {
    T_IPC_BUF buf = TutBufCreate(0);

    int1 integerFormat = (int1)System::getIntFormat();
    os.write ((const char*)&integerFormat, sizeof (integerFormat));

    try {

      T_INT4 size;
      char *data;
      T_IPC_MSG msg = (T_IPC_MSG) msgArg;
      T_BOOL result = T_FALSE;

      if (NULL != buf) {
        result = TmsgBufMsgSerializeEx(buf, msgArg, NULL, 0,
                                       (T_PTR*)&data, &size);
      }

      if (result == T_TRUE) {
        os.write((const char*)&size, sizeof (size));
        os.write(data, size);
      }
      else {
        os.clear(os.badbit);
      }
    }
    catch (...) {
      os.clear(os.badbit);
    }

    TutBufDestroy(buf);
    return os;
  }





  /**
   * Extraction operator that gets a TipcMsg object from an istream-like class.
   * Note that the data must have been previously written to the stream by
   * using the templated OutputStream operator << ().
   *
   * @warning The streamed message is binary; ascii formats are currently not
   * supported.
   *
   * @par Example:
   * The following example streams a TipcMsg from a binary file:
   * @code
   *  SmartSockets::TipcMsg msg;
   *
   *  std::ifstream inFile ("file.out", std::ios_base::binary);
   *  inFile >> msg;
   *  inFile.close();
   * @endcode
   */
  template <class InputStream>
  inline InputStream& operator >> (InputStream& is,
                                   TipcMsg& msgArg )
  {
    int1 integerFormat = 0;

    is.read ((char*)&integerFormat, sizeof(integerFormat));


    bool samePlatform = ((int1)System::getIntFormat() == integerFormat);

    uint4 size = 0;
    is.read ((char*)&size, sizeof(size));


    if (!samePlatform) {
      char tmp_val;
      /* Swap the first and fourth bytes. */
      tmp_val = ((char*)(&size))[0];
      ((char*)(&size))[0] = ((char*)(&size))[3];
      ((char*)(&size))[3] = tmp_val;
      /* Swap the second and third bytes. */
      tmp_val = ((char*)(&size))[1];
      ((char*)(&size))[1] = ((char*)(&size))[2];
      ((char*)(&size))[2] = tmp_val;

    }

    char * buff = new char [size];

    if (buff) {
      is.read(buff,size);
    }
    if (!is.bad()) {
      //  msg.packetToMsg(buff);

      T_IPC_BUF tmp = TutBufCreateStatic(buff, size);
      if (NULL != tmp) {
        TutBufSetSize (tmp, size);
        T_IPC_MSG msg = TipcBufMsgNext(tmp);

        if (!msg) {
          is.clear(is.badbit);
        }
        else {
          msgArg = msg;
          TipcMsgDestroy(msg);
        }

        TutBufDestroy(tmp);
      }
    }

    delete [] buff;

    return is;
  }


  inline const TipcMsg& operator >> (const TipcMsg & msg, TipcMsg & msgArg)
  {
    return (msg.operator >> (msgArg));
  }

  inline TipcMsg & operator >> (TipcMsg & msg,
                                TipcMsg & msgArg)
  {
    return const_cast <TipcMsg &> (msg.operator >> (msgArg));
  }

#endif

} // namespace SmartSockets




#endif //_T_MSG_HH_


