/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmt.h#1 $
 */

#ifndef _T_MESSAGE_TYPE_HH_
#define _T_MESSAGE_TYPE_HH_

#include <rtworks/ipc.h>
#include <rtworks/tex.h>


namespace SmartSockets {                     

  class T_IPCX_API TipcSrv;
  class T_IPCX_API TipcMsg;

  /**
   * Inherits from TipcException so users can trap TipcMt related
   * exceptions. Offers no additional functionality.
   *
   * @see TipcException, TipcMt
   */
  class T_IPCX_API TipcMtException : public TipcException {

  public:
    /**
     * Constructor.
     */
    TipcMtException () {}

    /**
     * Destructor.
     */
    virtual ~TipcMtException () throw() {}

  };


  class T_IPCX_API TipcMt;

  /**
   * Traverser class used with TipcMt::traverse()
   */
  class T_IPCX_API TipcMtTraverser {
  public:
    /**
     * Method signature for traversal methods.
     * @param name name of the type
     * @param mt message type object.
     * @return true to continue traversing; false otherwise.
     */
    typedef bool (TipcMtTraverser::* Handler) (const char* name, TipcMt& mt); 

    /** 
     * constructor that sets the default handler method to onTraverse().
     */
    TipcMtTraverser();

    /**
     * Sets the current handler method; if this is not called, the default method onTraverse() is called.
     */
    void setHandler(Handler cbMethod);

    /**
     * Gets the current handler method.
     */
    Handler getHandler();

    /**
     * Destructor.
     */
    virtual ~TipcMtTraverser() throw();

    /**
     * Default implementation of the traverse method.
     * @param name name of the type
     * @param mt message type object.
     * @return true to continue traversing; false otherwise.
     */
    virtual bool onTraverse (const char* name, TipcMt& mt)throw () ;

    static T_PTR T_ENTRY traverseFuncDelegator 
    (
    T_STR name,
    T_IPC_MT mt,
    T_PTR arg
    );

  private:

    Handler _cbMethod;


  };

  /**
   * Encryption and Decryption class for Message types. Used with TipcMt::setEncryptObj(),
   * TipcMt::setDecryptObj(), and triggered whenever TipcMsg::encrypt() or TipcMsg::decrypt() is called.
   * Note that not only encryption (for data scrambling) can be done, but 
   * also data compression.  The message header is left intact with this mechanism.
   */
  class T_IPCX_API TipcMtEncrypt {
  public:
    /**
     * Method signature for encryption and decryption methods.  
     *
     * Warning:\n
     *   If the data needs to grow (e.g. be reallocated) then the new char[<size>]; operator should be\n 
     *   used.\n
     *
     * @param data the user data portion of the message to be encrypted/decrypted serialized in a binary 
     * blob.  This parameter is also used to return the encrypted/decrypted data.
     * @param dataSize the number of bytes in the user data binary blob.  This parameter is also used to 
     * return the size of the encrypted/decrypted data.
     * @param key the key to encrypt/decrypt the data set using the TipcMt::setKey() method.
     * @param keySize the number of bytes in the key.
     */
    typedef void (TipcMtEncrypt::* Handler) (void*& data, int4& dataSize, 
                                             const void* key, int4 keySize); 

    /** 
     * constructor that sets the default handler method to onTraverse().
     */
    TipcMtEncrypt();

    /**
     * Sets the current encrypt handler method; if this is not called, the 
     * default method encrypt() is 
     * called.
     */
    void setEncryptHandler(Handler encryptMethod) ;
    /**
     * Gets the current encrypt handler method.
     */
    Handler getEncryptHandler();

    /**
     * Sets the current decrypt handler method; if this is not called, the 
     * default method decrypt() is 
     * called.
     * @param encryptMethod a method conforming to the TipcMtEncrypt::Handler.
     */
    void setDecryptHandler(Handler decryptMethod);

    /**
     * Gets the current decrypt handler method.
     */
    Handler getDecryptHandler();


    /**
     * Destructor.
     */
    virtual ~TipcMtEncrypt() throw();

    /**
     * Default implementation for an encryption method that doesn't do anything.
     * @param data the user data portion of the message to be encrypted serialized in a binary blob.  This
     * parameter is also used to return the encrypted data.
     * @param dataSize the number of bytes in the user data binary blob.  This parameter is also used to 
     * return the size of the encrypted data. If the data needs to grow (e.g. be reallocated)
     * then the new char[<size>]; operator should be used.
     * @param key the key to encrypt the data set using the TipcMt::setKey() method.
     * @param keySize the number of bytes in the key.
     */
    virtual  void encrypt(void*& data, int4& dataSize, const void* key, int4 keySize) 
    throw (TipcException);

    /**
     * Default implementation for an decryption method that doesn't do anything.
     * @param data the user data portion of the message to be encrypted serialized in a binary blob.  This
     * parameter is also used to return the encrypted data.  If the data needs to grow (e.g. be reallocated)
     * then the new char[<size>]; operator should be used.
     * @param dataSize the number of bytes in the user data binary blob.  This parameter is also used to 
     * return the size of the encrypted data.
     * @param key the key to encrypt the data set using the TipcMt::setKey() method.
     * @param keySize the number of bytes in the key.
     */
    virtual  void decrypt(void*& data, int4& dataSize, const void* key, int4 keySize) 
    throw (TipcException);



    static void T_ENTRY encryptFuncDelegator 
    (
    T_BUF buf ,
    T_PTR key,
    T_INT4 key_size,
    T_PTR arg
    );

    static void T_ENTRY decryptFuncDelegator 
    (
    T_BUF buf ,
    T_PTR key,
    T_INT4 key_size,
    T_PTR arg
    );

  private:

    Handler _encryptMethod;
    Handler _decryptMethod;


  };



  /**
   * Class that is used for constructing, looking up, and destroying message types.  Message types are 
   * stencils that are used to build a TipcMsg object; message types can be used to set values on TipcMsg
   * headers.  Message types are sent across the network as a 4 byte numeric value that is primarily
   * used to trigger callbacks at the receiving end.  Message types are also used to determine the layout of
   * the data going through the network.  For example, NUMERIC_DATA is a message type with a 
   * predefined layout requiring a series of name-value pairs, with each string name followed immediately by
   * a numeric value. To send numeric data to a process, the sending process constructs a message that 
   * uses the NUMERIC_DATA message type. A message type is created once and available for use as the type 
   * for any number of messages.  
   * 
   * SmartSockets provides a large number of standard message types that you can use and that
   * are also used internally by SmartSockets. When a standard message type does not satisfy a
   * specific need, you can create a user-defined message type. Both standard and user-defined
   * message types are handled in the same manner. Once the message type is created, messages
   * can be constructed, sent, received, and processed through a variety of methods. 
   */
  class T_IPCX_API TipcMt {

    /// friend class
    //@{

    friend class TipcConn;
    friend class TipcSrv;
    //@}
  private:
    T_IPC_MT _mt;  


  public:


    /**
     * Constructor for compatibility with the C API; use this constructor a
     * to create NULL message types used to match all entries in some of 
     * the TipcSrv callback registration methods.
     */
    TipcMt(T_IPC_MT mt) throw ();

    /**
     * Create a TipcMt object by message type name.  In order for this constructor to work, the message type 
     * must be created using the TipcMt::create() static method.
     * @param name message type name that matches one of the standard message types or a user-defined type
     * created using the TipcMt::create() method.
     * @throw TipcMtException if the message type name doesn't exist or is NULL.
     */
    TipcMt(const char * name) throw (TipcException);

    /**
     * Create a TipcMt object by message type number.  In order for this constructor to work, the message type 
     * must be created using the TipcMt::create() static method.
     * @param num message type number that matches one of the standard message types or a user-defined type
     * created using the TipcMt::create() method.
     * @throw TipcMtException if the message type name doesn't exist or is NULL.
     */
    TipcMt(int4 num) throw (TipcException);


    /**
     * Copy constructor.
     */
    TipcMt(const TipcMt& mt) throw (TipcException);

    /**
     * Destructor.  Note that the destructor does not destroy the message type created by calling 
     * TipcMt::create(); instead it destroys the object with the wrapper interface.  To destroy the message
     * type for the whole process, call TipcMt::destroy().
     */
    virtual ~TipcMt() throw();

    /**
     * Creates a new message type for the process.  Message types are stored in a process-wide hash table
     * that is accessible from any thread; create() adds a new entry to this global hash table.  
     * 
     * A message type is a template for a specific kind
     * of message. Once the message type is created, any number of TipcMsg objects of that type can be 
     * created.
     *
     * The new message type is created with an unset priority property, a delivery mode property of 
     * T_IPC_DELIVERY_BEST_EFFORT, an unset delivery timeout property, a load balancing mode property of
     * T_IPC_LB_NONE, a header string encode property of FALSE, a user-defined property of 0,
     * and an unset compression property.
     * These properties can be set with the methods TipcMt::setPriority(), TipcMt::setDeliveryMode(),
     * TipcMt::setDeliveryTimeout(), TipcMt::setLbMode(), TipcMt::setHeaderStrEncode(), and 
     * TipcMt::setUserProp(), TipcMt::setCompression, respectively.
     *
     * @param name should be a legal identifier. 
     * @param num  is a signed four-byte integer. Message type numbers less than 1 are reserved for 
     * SmartSockets standard message types. The standard SmartSockets message types use similar names and
     * numbers. (For example, the message type with name "numeric_data" has a C #define defined number 
     * T_MT_NUMERIC_DATA.) See the SmartSockets User—s Guide for a list of all standard message types.
     * @param grammar identifies the layout of fields in messages that use this message type. The grammar
     * consists of a list of field types. See the SmartSockets User—s Guide for more information on message
     * type grammars.
     * @return An object holding a copy the newly created message type.  Note that this is passed by
     * value because TipcMt is a very light weight wrapper class, and there's not much to be done in the
     * copy constructor.
     * @throw TipcMtException if attempting to create a message type with a name or number that already
     * exists.
     * @see getGrammar()
     */
    static TipcMt create(const char * name, int4 num, const char * grammar) throw (TipcException);

    /**
     * Looks up a message type with the desired name from the global hash table of message types. The 
     * standard SmartSockets message types use C #defines for numbers that are similar to the 
     * names. (For example, the message type with name "numeric_data" has a defined number 
     * T_MT_NUMERIC_DATA.) 
     * @param name should be a legal identifier listed as a key in the global message type table.
     * @return An object holding a copy the globally stored message type.  Note that this is passed by
     * value because TipcMt is a very light weight wrapper class, and there's not much to be done in the
     * copy constructor.
     * @throw TipcMtException if a message type with this name cannot be deleted.
     */
    static TipcMt lookup(const char * name) throw (TipcException); 

    /**
     * Traverses all the elements of the global message type hash table calling the specified Handler
     * method in the specified traverser object.
     * @param pointer to an object inheriting from the traverser object.
     * @see TipcMtTraverser::onTraverse()
     * @see TipcMtTraverser
     * @throw TipcMtException if an error occurred in the traversal process (usually due to a NULL pointer
     * passed in as a parameter).
     */
    static void traverse(TipcMtTraverser* travObj)
    throw (TipcException);



    /**
     * Looks up a message type with the desired number. The standard SmartSockets
     * message types use C #defines for numbers that are similar to the names. (For
     * example, the message type with name "numeric_data" has a defined number T_MT_NUMERIC_DATA.) 
     * @param num is a signed four-byte integer. Message type numbers less than 1 are reserved for 
     * SmartSockets standard message types. The standard SmartSockets message types use similar names and
     * numbers. (For example, the message type with name "numeric_data" has a C #define defined number 
     * T_MT_NUMERIC_DATA.) See the SmartSockets User—s Guide for a list of all standard message types.
     * @return An object holding a copy the globally stored message type.  Note that this is passed by
     * value because TipcMt is a very light weight wrapper class, and there's not much to be done in the
     * copy constructor.
     * @throw TipcMtException if a message type with this name cannot be deleted.
     */
    static TipcMt lookup(int4 num) throw (TipcException);


    /**
     * Removes an entry matching the parameter mt from the global message type table.
     * @throw TipcMtException the message type cannot be deleted.
     */
    static void destroy(TipcMt& mt) throw (TipcException);


    /**
     * type cast operator used for compatibility with the C API.
     * @return the internal C T_IPC_MT pointer.
     */
    operator T_IPC_MT() const  throw() { return _mt;}

    /**
     * Prints the contents of the message type using the given method.
     * @param out_func pointer to a static method with the following signature:
     *  void func( char* formatStr, ...); the default value is TutOut, which outputs
     * data to standard out.  Note: on Windows platforms the function should be 
     * declared as follows: void T_ENTRY_VA T_EXPORT func (T_STR format_str,...);
     * @throw TipcMtException if the message type object is corrupt.
     */
    void print(T_OUT_FUNC func =(T_OUT_FUNC)::TutOut) const 
    throw (TipcException);



    /**
     * Gets the grammar property of the message type.
     * The grammar property identifies the layout of fields in messages that use this type. The
     * grammar consists of a list of field types. Each field type in the grammar corresponds to one
     * field in the message. For example, the standard message type TIME has a grammar real8,
     * which defines the first and only field as being an eight-byte real number. Comments (delimited 
     * by / *   * / or (* *)) are also allowed in the grammar.
     *
     * The main purpose of the grammar is to allow messages to be written to text files in a more
     * compact format (see TipcMsgFile for information on message files). Without
     * a message type grammar, it is difficult to know if the number 45 in a text message file was
     * an INT2, INT4, INT8, REAL4, REAL8, or REAL16 field. Message type grammars also
     * provide some self-documentation for message types. The grammar is not enforced,
     * however, when a message is constructed. A message type with a grammar STR REAL8, for
     * example, does not stop a message from being constructed with ten INT4 fields.
     * Message type grammars do not have any relationship to the data conversion capabilities of
     * connections.  SmartSockets messages have strongly-typed fields. The field types, not the grammar,
     * enable connections to perform byte swapping and floating-point conversions.
     * The following fields can be used when defining a message type grammar:\n
     * - CHAR        : one-byte integer
     * - BINARY      : non-restrictive array of characters, such as an entire C data structure or the entire 
     * contents of a file.
     * - STR         : a C string (NULL-terminated array of characters)
     * - STR_ARRAY   : array of STR
     * - INT2        : two-byte integer
     * - INT2_ARRAY  : array of INT2
     * - INT4        : four-byte integer
     * - INT4_ARRAY  : array of INT4
     * - INT8        : eight-byte integer
     * - INT8_ARRAY  : array of INT8
     * - REAL4       : four-byte real number
     * - REAL4_ARRAY : array of REAL4
     * - REAL8       : eight-byte real number
     * - REAL8_ARRAY : array of REAL8
     * - REAL16      : sixteen-byte real number
     * - REAL16_ARRAY: array of REAL16
     * - MSG         : a message
     * - MSG_ARRAY   : array of MSG
     * - XML         : XML object
     * - BOOL        : an INT4 field type when a value of 1 should be printed as TRUE and a
     * value of 0 should be printed as FALSE.
     * - ID          : STR field type when the value should be printed without quotes.
     * - VERBOSE     : Use for any field type when both the field type and value should be printed.
     * For example, instead of values printing as voltage 33.4534, they would be
     * printed as str "voltage" real8 33.4534.
     * 
     * @throw TipcMtException if the grammar field cannot be obtained.
     */
    const char *  getGrammar() const throw (TipcException);

    /**
     * Gets the name property of the message type.  The name is used as a key to the TipcMt object in the
     * global message type table; it identifies the message type within the process; therefore, each
     * message type name must be unique.  Message type names that start with an underscore are reserved for
     * internal SmartSockets message types.
     *
     * @throw TipcMtException if the name field cannot be obtained.
     */
    const char *  getName() const throw (TipcException);



    /**
     * Gets the number property of the message type.  The number is used as a key to the TipcMt object in the
     * global message type table; it identifies the message type within both the process and the RTserver cloud; 
     * therefore, each message type number within a project should be unique. Negative message type numbers 
     * are reserved for internal SmartSockets message types.
     *
     * @throw TipcMtException if the name field cannot be obtained.
     */
    int4 getNum() const throw (TipcException);


    /**
     * Accessor method for the priority message property.  The priority property identifies the default 
     * priority for messages of this type. The message priority property controls where an incoming
     * message is inserted into a connection—s message queue. The message type priority can either be set 
     * to a specific value or it can be unknown.
     *
     * When a message is created, its priority is initialized to the message type priority
     * (if set) or to the value of the option Default_Msg_Priority (if the message type priority is
     * unknown). 
     *
     * The priority of an outgoing message can always be set on a per-message basis, but using
     * message type priorities makes it easier to raise or lower the default priority for all outgoing
     * messages of a specific type. Note that if set, the message type priority always overrides the
     * value in the option Default_Msg_Priority. The standard SmartSockets message types, by
     * default, all have a priority of unknown, but these can be changed if desired. User-defined
     * message types can use whatever priority you choose.
     *
     * @throw TipcMtException if the operation is not successfully completed.
     */
    //@{
    int2 getPriority() const throw (TipcException);
    void setPriority(int2 priority) throw (TipcException);
    void setPriorityUnknown()throw (TipcException);
    //@}



    /**
     * Accessor method for the user property of the message.  The message user-defined property can be 
     * used for any purpose, such as attaching a version number to messages. When a message is created,
     * its user-defined property is initialized to the message type user-defined property. 
     *
     * The user-defined property of an outgoing message can always be set on a per-message
     * basis, but using the message type user-defined property makes it easier to change the
     * default user-defined property for all outgoing messages of a specific type. The standard
     * SmartSockets message types by default all have a user-defined property of zero (0), but
     * these may be changed if desired. User-defined message types can also use whatever
     * user-defined property is appropriate.
     *
     * @throw TipcMtException if the operation is not successfully completed.
     */
    //@{
    int4 getUserProp() const throw (TipcException);
    void setUserProp(int4 user_prop) throw (TipcException);
    //@}




    /**
     * Accessor method for the compression property of the message. When a 
     * message is created its compression property is initialized to the 
     * message type compression property. 
     *
     * The compression property of an outgoing message can always be set on a 
     * per-message basis, but using the message type user-defined property 
     * makes it easier to change the default compression property for all 
     * outgoing messages of a specific type. The standard SmartSockets message 
     * types by default all have a compression property of false, but
     * these may be changed if desired. 
     *
     * @throw TipcMtException if the operation is not successfully completed.
     */
    //@{
    bool getCompression() const throw (TipcException);
    void setCompression(bool compression) throw (TipcException);
    //@}




    /**
     * Accessor method for the delivery mode property of the message.
     * @see TipcMsg::getDeliveryMode()
     */
    //@{
    T_IPC_DELIVERY_MODE getDeliveryMode() const throw (TipcException);

    void setDeliveryMode(T_IPC_DELIVERY_MODE delivery_mode)
    throw (TipcException);

    //@}



    /**
     * Accessor method for the load balancing mode property of the message.
     * @see TipcMsg::getLbMode()
     */
    //@{
    T_IPC_LB_MODE getLbMode() const throw (TipcException);
    void setLbMode(T_IPC_LB_MODE lb_mode) throw (TipcException); 

    //@}


    /**
     * Accessor method for the delivery mode property of the message. The delivery mode of a
     * message controls what level of guarantee is used when the message is sent through a
     * connection with TipcConn::send(). When a message is created, its delivery mode is
     * initialized to the message type delivery mode. This allows a default value to easily be set 
     * for all messages of a certain type. 
     *
     * The default message delivery mode is T_IPC_DELIVERY_BEST_EFFORT. In this mode, no
     * special actions are taken to ensure delivery of sent messages. The message is delivered
     * unless network failures or process failures occur. If the message is not delivered, there 
     * is no way for the sender to know that delivery failed. When there is a failure, it is
     * possible for some messages to be lost or be delivered in a different order than the order 
     * in which they were published.
     * 
     * In the case of a network or process failure, a more useful delivery mode is         
     * T_IPC_DELIVERY_ORDERED. Messages can still be lost in the event of a failure, but all
     * delivered messages are received in the order in which they were published. Because Java
     * Message Service (JMS) cannot handle messages that come out of order, you must use the
     * T_IPC_DELIVERY_ORDERED delivery mode or one of the GMD delivery modes if you are
     * sending messages to a JMS client.
     *
     * The difference between the other delivery modes and the GMD delivery modes is that in
     * GMD, a copy of the message is kept when the message is sent and acknowledgements are
     * sent when the message is received. No acknowledgements (ACKs) are used in the ordered
     * or best effort delivery modes, and no copy of the message is kept.
     * One of the GMD delivery modes is T_IPC_DELIVERY_SOME. In this mode, the sending
     * process saves a copy of the message in the connection GMD area until the message is
     * successfully delivered, and the sender can also resend the message if necessary. Delivery is
     * considered successful if the sent message is acknowledged by at least one receiving process.
     *
     * The other and most robust GMD delivery mode is T_IPC_DELIVERY_ALL. In this mode,
     * the sending process saves a copy of the message in the connection GMD area until the
     * message is successfully delivered, and the sender can also resend the message if necessary.
     * Delivery is not considered successful until all receiving processes acknowledge the sent
     * message. For two processes communicating through a non-RTclient and non-RTserver
     * T_IPC_CONN connection, T_IPC_DELIVERY_SOME and T_IPC_DELIVERY_ALL are
     * identical because there is only one process receiving the message. For RTclients, the two
     * modes do differ if more than one RTclient is subscribing to the subject in the destination of
     * the message.      
     * @throw TipcMtException if this property cannot be set or retrieved 
     */
    //@{
    real8 getDeliveryTimeout() const throw (TipcException);

    void setDeliveryTimeout(real8 delivery_timeout) 
    throw (TipcException);

    //@}



    /**
     * Accessor method for the Header string encode message property.
     *
     * @throw TipcMtException if the property cannot be accessed.
     * @see TipcMsg::getHeaderStrEncode()
     *
     */
    //@{
    bool getHeaderStrEncode() const throw (TipcException);

    // set the header encode mode
    void setHeaderStrEncode(bool header_encode) 
    throw (TipcException);

    //@}


    /**
     * Accessor method for the Header string encode message property.
     *
     * @throw TipcMtException if the property cannot be accessed.
     * @see TipcMsg::getHeaderStrEncode()
     *
     */

    /**
     * Sets the encryption key used by the encrypt and decrypt objects. If the key is null, then the
     * encryption/decryption is turned off for that message type.  
     */
    void setKey(const void* keyValue, int4 size) throw (TipcException);

    /**
     * Sets the Encryption object, and optionally an encryption method within that object to encrypt the
     * data portion of the message.  The Encryption object is invoked when TipcMsg::encrypt() is called on
     * a TipcMsg of this TipcMt type.  If the default NULL values are used, the encryption method is cleared
     * and a call to TipcMsg::encrypt() will not do anything to the message.
     * @param encryptObj instance of a child class of TipcMtEncrypt with either the TipcMtEncrypt::encrypt()
     * method overriden, or with another method with the same signature as TipcMtEncrypt::Handler.
     * @param encryptMethod method with the same signature as TipcMtEncrypt::Handler.  If NULL, the method used
     * will be the overloaded TipcMtEncrypt::encrypt().
     * @throw TipcMtException if there's a failure setting the encryption object.
     */
    void setEncryptObj(TipcMtEncrypt* encryptObj = NULL)
    throw (TipcException);    


    /**
     * Gets the Encryption object associated with this message type. 
     * @throw TipcMtException if there's a failure setting the encryption object.
     */
    TipcMtEncrypt* getDecryptObj() const 
    throw (TipcException);

    /**
     * Sets the Decryption object, and optionally a decryption method within that object to decrypt the
     * data portion of the message.  The Decryption object is invoked when TipcMsg::decrypt() is called on
     * a TipcMsg of this TipcMt type.  If the default NULL values are used, the encryption method is cleared
     * and a call to TipcMsg::decrypt() will not do anything to the message.
     * @param decryptObj instance of a child class of TipcMtEncrypt with either the TipcMtEncrypt::decrypt()
     * method overriden, or with another method with the same signature as TipcMtEncrypt::Handler.
     * @param decryptMethod method with the same signature as TipcMtEncrypt::Handler.  If NULL, the method used
     * will be overloaded TipcMtEncrypt::decrypt().
     * @throw TipcMtException if there's a failure setting the decryption object.
     */
    void setDecryptObj(TipcMtEncrypt* decryptObj = NULL)
    throw (TipcException);      



    /**
     * Gets the Decryption object associated with this message type. 
     * @throw TipcMtException if there's a failure setting the decryption object.
     */
    TipcMtEncrypt* getEncryptObj() const
    throw (TipcException);

  };

} // namespace SmartSockets

#endif //_T_MESSAGE_TYPE_HH_
