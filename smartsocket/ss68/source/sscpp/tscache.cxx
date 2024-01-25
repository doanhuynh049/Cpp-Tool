/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tscache.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tscache.h>

namespace SmartSockets {

  // Constructor
  TipcSrvCache::TipcSrvCache(const char * smartCache, const TipcSrv &tsrv) 
  throw (TipcException)
  {
    _srv = (T_IPC_SRV) tsrv;

    if (NULL == smartCache || NULL == _srv) {
      TutErrNumSet(T_ERR_NULL_PTR);
      throw TipcSrvCacheException();
    }
    _smartCache = const_cast<char *> (smartCache);
  }


  void TipcSrvCache::init() throw (TipcException)
  {
    bool result = TipcSrvConnCacheInit(_srv);

    if (false == result)
      throw TipcSrvCacheException();
  }

  void TipcSrvCache::initLV(const char * cmd_file) throw (TipcException)
  {
    bool result = TipcSrvConnCacheInitLV(_srv, const_cast<char *> (cmd_file));

    if (false == result)
      throw TipcSrvCacheException();
  }

  void TipcSrvCache::gmdInit() throw (TipcException)
  {
    bool result = TipcSrvConnCacheGMDInit(_srv, _smartCache);

    if (false == result)
      throw TipcSrvCacheException();
  }

  void TipcSrvCache::gmdFinish() throw (TipcException)
  {
    bool result = TipcSrvConnCacheGMDFinish(_srv, _smartCache);

    if (false == result)
      throw TipcSrvCacheException();
  }

  void TipcSrvCache::gmdFlush(const char * coreqSubj, const char * reqSender) 
  throw (TipcException)
  {
    bool result = TipcSrvConnCacheGMDFlush(_srv, const_cast<char *> (coreqSubj), 
                                           const_cast<char *> (reqSender), _smartCache);

    if (false == result)
      throw TipcSrvCacheException();
  }


  void TipcSrvCache::requestMsgs(const char * reqSubj, int4 numMsgs, 
                                 real8 deltaTime) throw (TipcException)
  {
    bool result = TipcSrvConnCacheRequestMsgs(_srv, const_cast<char *> (reqSubj), 
                                              numMsgs, deltaTime, _smartCache); 

    if (false == result)
      throw TipcSrvCacheException();
  }

  void TipcSrvCache::requestStart(const char * reqSubj, int4 numMsgs, 
                                  real8 deltaTime) throw (TipcException)
  {
    bool result = TipcSrvConnCacheRequestStart(_srv, const_cast<char *> (reqSubj), 
                                               numMsgs, deltaTime, _smartCache); 

    if (false == result)
      throw TipcSrvCacheException();
  }

  void TipcSrvCache::requestStop(const char * reqSubj) throw (TipcException)
  {
    bool result = TipcSrvConnCacheRequestStop(_srv, const_cast<char *> (reqSubj), 
                                              _smartCache); 

    if (false == result)
      throw TipcSrvCacheException();
  }

  void TipcSrvCache::requestLastValue(const char * subject, const TipcMt& returnMt) 
  throw (TipcException)
  {
    bool result = TipcSrvConnCacheRequestLastValue(_srv, const_cast<char *> (subject), 
                                                   _smartCache, (T_IPC_MT) returnMt); 

    if (false == result)
      throw TipcSrvCacheException();
  }

  void TipcSrvCache::sendRequest(const char * reqSubj, int4 numMsgs, 
                                 real8 deltaTime) throw (TipcException)
  {
    bool result = TipcSrvConnCacheSendRequest(_srv, const_cast<char *> (reqSubj),
                                              _smartCache, T_FALSE,
                                              numMsgs, deltaTime); 

    if (false == result)
      throw TipcSrvCacheException();
  }


  void TipcSrvCache::sendRequestLastValue(const char * reqSubj,  
                                          real8 deltaTime) throw (TipcException)
  {
    bool result = TipcSrvConnCacheSendRequest(_srv, const_cast<char *> (reqSubj),
                                              _smartCache, T_TRUE,
                                              1, deltaTime); 

    if (false == result)
      throw TipcSrvCacheException();
  }


} //namespace SmartSockets
