/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tmt.cxx#2 $
 */

#include "includes.h"
#include <rtworks/tmt.h>

struct T_MSG_MT_ENCRYPT_INFO_STRUCT {
  T_ENCRYPT_KEY       encrypt_key;
  T_MSG_ENCRYPT_FUNC  encrypt_func;
  T_PTR               encrypt_arg;
  T_MSG_DECRYPT_FUNC  decrypt_func;
  T_PTR               decrypt_arg;
};

namespace SmartSockets {



#define CALL_HANDLER(object,ptrToMember)  ((object).*(ptrToMember))

  TipcMtTraverser::TipcMtTraverser()
  {
#if !defined(T_OS_SOLARIS) || defined(__GNUC__)
    setHandler(&TipcMtTraverser::onTraverse);
#else
    setHandler(this->onTraverse);
#endif
  }

  void TipcMtTraverser::setHandler(TipcMtTraverser::Handler cbMethod)
  {
    _cbMethod = cbMethod;
  }

  TipcMtTraverser::Handler TipcMtTraverser::getHandler()
  {
    return _cbMethod;
  }

  TipcMtTraverser::~TipcMtTraverser() throw()
  {
  }

  bool TipcMtTraverser::onTraverse (const char* name, TipcMt& mt)throw () 
  {
    return false;
  }


  T_PTR T_ENTRY TipcMtTraverser:: traverseFuncDelegator 
  (
  T_STR name,
  T_IPC_MT mt,
  T_PTR arg
  )
  {
    if (arg == NULL)
      throw TipcMtException();

    TipcMtTraverser* cb = (TipcMtTraverser*) arg;

    TipcMtTraverser::Handler method = (cb->getHandler());

    if (method == T_NULL_FUNC) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMtException();
    }


    bool retVal = false;
    TipcMt mtArg (mt); 
    retVal = CALL_HANDLER(*cb,method) (const_cast <const char*>(name),
                                       mtArg);


    return retVal?NULL:arg;
  }



  void TipcMt::traverse(TipcMtTraverser* travObj)
  throw (TipcException)
  {

    TipcMtTraverser::Handler travMethod = NULL;
    if (travObj == NULL) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMtException();
    }

    if (travMethod != T_NULL_FUNC)
      travObj->setHandler(travMethod);

    TipcMtTraverse(TipcMtTraverser::traverseFuncDelegator, (T_PTR)travObj);

  }

  TipcMtEncrypt::TipcMtEncrypt()
  {
#if !defined(T_OS_SOLARIS) || defined(__GNUC__)
    setEncryptHandler(&TipcMtEncrypt::encrypt);
    setDecryptHandler(&TipcMtEncrypt::decrypt);
#else
    setEncryptHandler(this->encrypt);
    setDecryptHandler(this->decrypt);
#endif

  }

  void TipcMtEncrypt::setEncryptHandler(TipcMtEncrypt::Handler encryptMethod)
  {
    _encryptMethod = encryptMethod;
  }

  TipcMtEncrypt::Handler TipcMtEncrypt::getEncryptHandler()
  {
    return _encryptMethod;
  }

  void TipcMtEncrypt::setDecryptHandler(TipcMtEncrypt::Handler decryptMethod)
  {
    _decryptMethod = decryptMethod;
  }

  TipcMtEncrypt::Handler TipcMtEncrypt::getDecryptHandler()
  {
    return _decryptMethod;
  }

  void TipcMtEncrypt::encrypt(void*& data, int4& dataSize, const void* key,
                              int4 keySize)
  throw (TipcException)
  {
  }


  void TipcMtEncrypt::decrypt(void*& data, int4& dataSize, const void* key,
                              int4 keySize)
  throw (TipcException)
  {
  }

  TipcMtEncrypt::~TipcMtEncrypt() throw()
  {
  }

  /* ======================================================================= */
  /*..TipcMt::TipcMt -- Create a TipcMt object with an existing C API */
  /*..  T_IPC_MT struct. */
  TipcMt::TipcMt(T_IPC_MT mt) throw ()
  { 
    _mt = mt; 
  }


  /* ======================================================================= */
  /*..TipcMt::TipcMt -- Create a TipcMt object by message type name */
  TipcMt::TipcMt(const char * name) throw (TipcException)
  {
    _mt = TipcMtLookup(const_cast<char *> (name));

    if (!_mt) {
      throw TipcMtException();
    }
  }


  /* ======================================================================= */
  /*..TipcMt::TipcMt -- Create a TipcMt object by message type number */
  TipcMt::TipcMt(int4 num) throw (TipcException) 
  {
    _mt = TipcMtLookupByNum(num);

    if (!_mt) {
      throw TipcMtException();
    }
  }


  /* ======================================================================= */
  /*..TipcMt::TipcMt -- Create a TipcMt object with a new message type */
  TipcMt TipcMt::create(const char * name, int4 num, const char * grammar) 
  throw (TipcException)
  {
    T_IPC_MT mt = TipcMtCreate(const_cast<char *> (name), num, 
                               const_cast<char *> (grammar));


    if (!mt) {
      throw TipcMtException();
    }

    return TipcMt(mt);
  }


  /* ======================================================================= */
  /*..TipcMt::TipcMt -- Copy Constructor */
  TipcMt::TipcMt(const TipcMt& mt) throw (TipcException) 
  { 
    _mt = mt._mt; 

    if (!_mt) {
      throw TipcMtException();
    }
  }


  /* ======================================================================= */
  /*..TipcMt::~TipcMt -- TipcMt destructor */
  TipcMt::~TipcMt() throw()
  { 
  }




  /* ======================================================================= */
  /*..TipcMt::Destroy -- destroy a message type */
  void TipcMt::destroy(TipcMt& mt) throw (TipcException)
  {
    if (!TipcMtDestroy(mt._mt))
      throw TipcMtException();
  }

  /* ======================================================================= */
  /*..TipcMt::Print -- print all information about a message type */
  void TipcMt::print(T_OUT_FUNC func) const throw (TipcException)
  {
    if (!TipcMtPrint(_mt, func))
      throw TipcMtException();
  }

  /* ======================================================================= */
  /*..TipcMt::Lookup -- look up a message type by name */
  TipcMt TipcMt::lookup(const char * name) throw (TipcException)
  {

    T_IPC_MT mt = TipcMtLookup(const_cast<char *> (name));

    if (mt == T_NULL)
      throw TipcMtException();

    return TipcMt(mt);
  }


  /* ======================================================================= */
  /*..TipcMt::Lookup -- look up a message type by number */
  TipcMt TipcMt::lookup(int4 num) throw (TipcException)
  {

    T_IPC_MT mt = TipcMtLookupByNum(num);

    if (mt == T_NULL)
      throw TipcMtException();


    return TipcMt(mt);
  }        


  /* ======================================================================= */
  /*..TipcMt::Grammar -- get the grammar of a message type */
  const char * TipcMt::getGrammar() const throw (TipcException)
  {
    char * grammar_return = (char *)"";  // initialize variable

    if (!TipcMtGetGrammar(_mt, &grammar_return))
      throw TipcMtException();

    return const_cast<const char*>(grammar_return);
  }


  /* ======================================================================= */
  /*..TipcMt::Name -- get the name of a message type */
  const char * TipcMt::getName() const throw (TipcException) 
  {
    char * name_return = (char *)"";  // initialize variable

    if (!TipcMtGetName(_mt, &name_return))
      throw TipcMtException();

    return const_cast<const char*>(name_return);
  }

  /* ======================================================================= */
  /*..TipcMt::Num -- get the number of a message type */
  int4 TipcMt::getNum() const throw (TipcException) 
  {
    int4 num_return = 0;  // initialize variable

    if (!TipcMtGetNum(_mt, &num_return))
      throw TipcMtException();

    return num_return;
  } 


  /* ======================================================================= */
  /*..TipcMt::Priority -- get the priority of a message type */
  int2 TipcMt::getPriority() const throw (TipcException)
  {
    int2 priority_return = 0;  // initialize variable

    if (!TipcMtGetPriority(_mt, &priority_return))
      throw TipcMtException();

    return priority_return;
  }


  /* ======================================================================= */
  /*..TipcMt::Priority -- set the priority of a message type */
  void TipcMt::setPriority(int2 priority) throw (TipcException)
  {
    if (!TipcMtSetPriority(_mt, priority))
      throw TipcMtException();
  }


  /* ======================================================================= */
  /*..TipcMt::SetPriorityUnknown -- set the priority of a message type to */
  /*..  unknown */
  void TipcMt::setPriorityUnknown() throw (TipcException)
  {
    if (!TipcMtSetPriorityUnknown(_mt))
      throw TipcMtException();
  }


  /* ======================================================================= */
  /*..TipcMt::UserProp -- get user-defined property from a message type */
  int4 TipcMt::getUserProp() const throw (TipcException)
  {
    int4 user_prop_return = 0;  // initialize variable

    if (!TipcMtGetUserProp(_mt, &user_prop_return))
      throw TipcMtException();

    return user_prop_return;
  }


  /* ======================================================================= */
  /*..TipcMt::UserProp -- set user-defined property from a message type */
  void TipcMt::setUserProp(int4 user_prop) throw (TipcException)
  {
    if (!TipcMtSetUserProp(_mt, user_prop))
      throw TipcMtException();
  }

  /* ======================================================================= */
  /*..TipcMt::getCompression -- get compression property from a message type */
  bool TipcMt::getCompression() const throw (TipcException)
  {
    T_BOOL compression = T_FALSE;

    if (!TipcMtGetCompression(_mt, &compression))
      throw TipcMtException();

    return compression ? true : false;
  }

  /* ======================================================================= */
  /*..TipcMt::setCompression -- set compression property of a message type */
  void TipcMt::setCompression(bool compression) throw (TipcException)
  {
    if (!TipcMtSetCompression(_mt, compression))
      throw TipcMtException();
  }

  /* ======================================================================= */
  /*..TipcMt::DeliveryMode -- get the delivery mode of a message type */
  T_IPC_DELIVERY_MODE TipcMt::getDeliveryMode() const throw (TipcException)
  {
    T_IPC_DELIVERY_MODE delivery_mode_return = T_IPC_DELIVERY_BEST_EFFORT;

    if (!TipcMtGetDeliveryMode(_mt, &delivery_mode_return))
      throw TipcMtException();

    return delivery_mode_return;
  }


  /* ======================================================================= */
  /*..TipcMt::DeliveryMode -- set the delivery mode of a message type */
  void TipcMt::setDeliveryMode(T_IPC_DELIVERY_MODE delivery_mode) 
  throw (TipcException)
  {
    if (!TipcMtSetDeliveryMode(_mt, delivery_mode))
      throw TipcMtException();
  }


  /* ======================================================================= */
  /*..TipcMt::LbMode -- get load balancing mode */
  T_IPC_LB_MODE TipcMt::getLbMode() const throw (TipcException)
  {
    T_IPC_LB_MODE lb_mode = T_IPC_LB_NONE;

    if (!TipcMtGetLbMode(_mt, &lb_mode))
      throw TipcMtException();

    return lb_mode;
  }

  /* ======================================================================= */
  /*..TipcMt::LbMode -- set load balancing mode */
  void TipcMt::setLbMode(T_IPC_LB_MODE lb_mode) throw (TipcException)
  {
    if (!TipcMtSetLbMode(_mt, lb_mode))
      throw TipcMtException();
  }

  /* ======================================================================= */
  /*..TipcMt::DeliveryTimeout -- get delivery timeout */
  real8 TipcMt::getDeliveryTimeout() const throw (TipcException)
  {
    real8 delivery_timeout = -1;

    if (!TipcMtGetDeliveryTimeout(_mt, &delivery_timeout))
      throw TipcMtException();

    return delivery_timeout;
  }

  /* ======================================================================= */
  /*..TipcMt::DeliveryTimeout -- set delivery timeout */
  void TipcMt::setDeliveryTimeout(real8 delivery_timeout) throw (TipcException)
  {
    if (!TipcMtSetDeliveryTimeout(_mt, delivery_timeout))
      throw TipcMtException();
  }

  /* ======================================================================= */
  /*..TipcMt::HeaderStrEncode() -- get header encode mode */
  bool TipcMt::getHeaderStrEncode() const throw (TipcException)
  {
    T_BOOL header_encode = -1;

    if (!TipcMtGetHeaderStrEncode(_mt, &header_encode))
      throw TipcMtException();

    return header_encode?true:false;
  }

  /* ======================================================================= */
  /*..TipcMt::HeaderStrEncode() -- set header encode mode */
  void TipcMt::setHeaderStrEncode(bool header_encode) throw (TipcException)
  {
    if (!TipcMtSetHeaderStrEncode(_mt, header_encode))
      throw TipcMtException();
  }

  /* ======================================================================= */
  /*..TipcMt::setKey() -- set the encrypt key */
  void TipcMt::setKey(const void* keyValue, int4 size) throw (TipcException)
  {
    T_ENCRYPT_KEY key = TutKeyCreate(const_cast<void*> (keyValue), size);
    if (!TipcMtSetKey(_mt, key )) {
      TutKeyDestroy(key);
      throw TipcMtException();
    }
    TutKeyDestroy(key);
  }

  void TipcMtEncrypt::encryptFuncDelegator(T_BUF buf,T_PTR key,T_INT4 key_size,T_PTR arg)
  {
    if (arg == NULL)
      throw TipcMtException();

    TipcMtEncrypt* cb = (TipcMtEncrypt*) arg;

    TipcMtEncrypt::Handler method = (cb->getEncryptHandler());

    if (method == T_NULL_FUNC) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMtException();
    }


    T_PTR data  = buf->data;
    T_INT4 dataSize = buf->size;


    CALL_HANDLER(*cb,method) (data, dataSize,
                              const_cast <const void*>(key),key_size);

    // If the pointer changed, that means a new memory allocation happened inside the handler.
    // Because one should never mix operator new with function TutFree() -- used in the C API --, 
    // we have to make a new copy of the encrypted data, then delete[] the memory.
    if (data != buf->data) {
      TutFree (buf->data);
      buf->data = (char*) TutMalloc(dataSize);
      memcpy(buf->data,const_cast<const void*>(data),(size_t)dataSize);
      delete[] (char*)data;
    }
    buf->size = dataSize;

  }


  typedef void (T_ENTRY1 T_ENTRY2 T_EXPORT T_MSG_ENCRYPT_FUNC_DEREF)
  T_TYPEDEF((T_BUF, T_PTR, T_INT4, T_PTR));

  /* ======================================================================= */
  /*..TipcMt::setEncryptMethod() -- set the encrypt key */
  void TipcMt::setEncryptObj(TipcMtEncrypt*  encryptObj)
  throw (TipcException)
  {

    TipcMtEncrypt::Handler  encryptMethod = NULL;
    if (encryptObj != NULL) {
      if (encryptMethod != T_NULL_FUNC)
        encryptObj->setEncryptHandler(encryptMethod);


      if (!TipcMtSetEncryptFunc(_mt, TipcMtEncrypt::encryptFuncDelegator, (T_PTR) encryptObj))
        throw TipcMtException();



#if 0

      // Currently there's a problem with the TipcMtGetEncryptFunc() method; it doesn't return the
      // argument that was used for the callback, which happens to be the only thing that we're interested in.
      // Because of that, we'll do something fairly ugly:
      // we'll basically dynamically allocate some extra memory before the encrypt function pointer, and
      // also store the pointer to the TipcMtEncrypt object before the function pointer.  To make sure
      // that it's the valid memory, we'll also add a hex signature of 1973babe.
      char signature [4] = { (char)0x19, (char)0x73, (char)0xba, (char)0xbe};
      int4 offset =  sizeof (signature) + sizeof (TipcMtEncrypt*) ;
      char* hack = (char*) TutMalloc(offset + sizeof (T_MSG_ENCRYPT_FUNC_DEREF));
      int i = 0;

      if (!hack) {
        TutErrNumSet(T_ERR_NOMEM);
        throw TipcMtException();
      }

      for (i = 0; i < sizeof (signature); i++) {
        hack[i] = signature[i];
      }
      char* encryptObjData = (char*)((void*)&encryptObj);

      for (i = 0; i < sizeof (TipcMtEncrypt*); i++) {
        hack[i+sizeof (signature)] = encryptObjData[i];
      }

      T_IPC_ENCRYPT_FUNC encryptFunc = TipcMtEncrypt::encryptFuncDelegator;

      char* encryptFuncData = (char*) ((void*) (encryptFunc));

      for (i = 0; i < sizeof (T_MSG_ENCRYPT_FUNC_DEREF); i++) {
        hack[i+offset] = encryptFuncData[i];
      }



      T_MSG_ENCRYPT_FUNC_DEREF encryptFuncPadded = NULL;


      memcpy ((void*)&encryptFuncPadded,const_cast<const void*>((void*)(hack+offset)),sizeof (T_MSG_ENCRYPT_FUNC_DEREF));

      try {
        // If there was a previous encrypt object stored, free up the hack memory.
        TipcMtEncrypt* previousEncryptObj = getEncryptObj();
        if (previousEncryptObj == NULL) {
          throw TipcMtException();
        }
        char* previousHack = (char*) ((void*) &previousEncryptObj);
        previousHack -= offset;
        // Note: it's safe to delete it here because the memory has already been
        // signature checked in the getEncryptedObj() method.
        TutFree(previousHack);
      }
      catch (TipcMtException& e) {
        TutErrNumSet(0);
      }

      if (!TipcMtSetEncryptFunc(_mt, encryptFuncPadded, (T_PTR) encryptObj)) {
        TutFree (hack);
        throw TipcMtException();
      }

#endif
    }

    else {
      if (!TipcMtSetEncryptFunc(_mt, T_NULL_FUNC, (T_PTR) NULL))
        throw TipcMtException();
    }
  }




  /* ======================================================================= */
  /*..TipcMt::getEncryptObj() -- get the encrypt key */
  TipcMtEncrypt* TipcMt::getEncryptObj() const
  throw (TipcException)
  {

    TipcMtEncrypt* encryptObj = NULL;


    T_IPC_MT_ENCRYPT_INFO info = NULL;
    // Currently there's a problem with the TipcMtGetEncryptFunc() method; it doesn't return the
    // argument that was used for the callback, which happens to be the only thing that we're interested in.
    // Because of that, we have to use an undocumented function.

    if (!TipcMtGetEncryptInfoByNumInternal(getNum(),&info))
      throw TipcMtException();

    if (!info) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMtException();
    }

    encryptObj = (TipcMtEncrypt*) info -> encrypt_arg;

    return encryptObj;



#if 0
    if (!TipcMtGetEncryptFunc(_mt, &encryptFunc))
      throw TipcMtException();

    if (encryptFunc != NULL) {
      // Currently there's a problem with the TipcMtGetEncryptFunc() method; it doesn't return the
      // argument that was used for the callback, which happens to be the only thing that we're interested in.
      // Because of that, we'll do something fairly ugly:
      // we'll basically dynamically allocate some extra memory before the encrypt function pointer, and
      // also store the pointer to the TipcMtEncrypt object before the function pointer.  To make sure
      // that it's the valid memory, we'll also add a hex signature of 1973babe.
      char signature [4] = { (char)0x19, (char)0x73, (char)0xba, (char)0xbe};
      int4 offset =  sizeof (signature) + sizeof (TipcMtEncrypt*) ;
      char* hack = (char*) ((void*) &encryptFunc);
      hack -= offset;

      int i = 0;

      // Check the signature; if it doesn't match, return NULL;
      for (i = 0; i < sizeof (signature); i++) {
        if (hack[i] != signature[i]) {
          return NULL;
        }
      }
      char* encryptObjData = (char*)((void*) &(encryptObj));

      for (; i < sizeof (TipcMtEncrypt*); i++) {
        encryptObjData[i] = hack[i];
      }

      char* encryptFuncData = (char*) ((void*) &(TipcMtEncrypt::encryptFuncDelegator));

      // one further check (just in case, make sure that the delegator method matches).
      for (; i < sizeof (TipcMtEncrypt*); i++) {
        if (hack[i] != encryptFuncData[i]) {
          return NULL;
        }
      }
    }
    return encryptObj;
#endif


  }



  void TipcMtEncrypt::decryptFuncDelegator(T_BUF buf,T_PTR key,T_INT4 key_size,T_PTR arg)
  {
    if (arg == NULL)
      throw TipcMtException();

    TipcMtEncrypt* cb = (TipcMtEncrypt*) arg;

    TipcMtEncrypt::Handler method = (cb->getDecryptHandler());

    if (method == T_NULL_FUNC) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMtException();
    }


    T_PTR data  = buf->data;
    T_INT4 dataSize = buf->size;


    CALL_HANDLER(*cb,method) (data,dataSize,
                              const_cast <const void*>(key),key_size);

    // If the pointer changed, that means a new memory allocation happened inside the handler.
    // Because one should never mix operator new with function TutFree() -- used in the C API --, 
    // we have to make a new copy of the encrypted data, then delete[] the memory.
    if (data != buf->data) {
      TutFree (buf->data);
      buf->data = (char*)TutMalloc(dataSize);
      memcpy(buf->data,const_cast<const void*>(data),(size_t)dataSize);
      delete[] (char*)data;
    }
    buf->size = dataSize;

  }



  /* ======================================================================= */
  /*..TipcMt::setDecryptFunc() -- set the decrypt func */
  void TipcMt::setDecryptObj(TipcMtEncrypt* decryptObj)
  throw (TipcException)
  {
    TipcMtEncrypt::Handler  decryptMethod = NULL;
    if (decryptObj != NULL) {
      if (decryptMethod != T_NULL_FUNC)
        decryptObj->setDecryptHandler(decryptMethod);

      if (!TipcMtSetDecryptFunc(_mt, TipcMtEncrypt::decryptFuncDelegator, (T_PTR) decryptObj))
        throw TipcMtException();
    }
    else {
      if (!TipcMtSetDecryptFunc(_mt, T_NULL_FUNC, (T_PTR) NULL))
        throw TipcMtException();
    }
  }

  /* ======================================================================= */
  /*..TipcMt::GetDecryptFunc() -- get the encrypt key */
  TipcMtEncrypt* TipcMt::getDecryptObj() const
  throw (TipcException)
  {
    TipcMtEncrypt* encryptObj = NULL;


    T_IPC_MT_ENCRYPT_INFO info = NULL;
    // Currently there's a problem with the TipcMtGetEncryptFunc() method; it doesn't return the
    // argument that was used for the callback, which happens to be the only thing that we're interested in.
    // Because of that, we have to use an undocumented function.

    if (!TipcMtGetEncryptInfoByNumInternal(getNum(),&info))
      throw TipcMtException();


    if (!info) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcMtException();
    }

    encryptObj = (TipcMtEncrypt*) info -> decrypt_arg;

    return encryptObj;


    return NULL;
  }

} //namespace SmartSockets
