/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tscache.h#1 $
 */

#include <rtworks/types.h>
#include <rtworks/tmt.h>
#include <rtworks/tex.h>
#include <rtworks/tsrv.h>

namespace SmartSockets {

  /**
   * Inherits from TipcException so users can trap TipcSrvCache related
   * exceptions. Offers no additional functionality.
   *
   * @see TipcException, TipcSrvCache
   */
  class T_IPCX_API TipcSrvCacheException : public TipcException {

  public:
    /**
     * Constructor.
     */
    TipcSrvCacheException () {}

    /**
     * Destructor.
     */
    virtual ~TipcSrvCacheException () throw() {}
  };



  /**
   * TipcSrvCache class provides the SmartCache functionality.
   * It provides the ability to create a cache to store messages in. Temporary 
   * offline SmartSockets clients will be able to have SmartCache store 
   * messages and retrieve them when they come back online.  
   */
  class T_IPCX_API TipcSrvCache {

    ///cache name -- Subject of the SmartCache group
    char * _smartCache;

    ///C style T_IPC_SRV handle
    T_IPC_SRV _srv;

  public:

    /**
     * Constructor
     * param smartCache - subject of SmartCache group
     * param tsrv - TipcSrv object required for the SmartCache 
     * throw TipcSrvCacheException if the TipcSrvCache is not created
     */
    TipcSrvCache(const char * smartCache, const TipcSrv &tsrv) 
    throw (TipcException);

    ///Destructor
    ~TipcSrvCache() throw() {}

    /**
     * Performs initializations required to use SmartCache. This function 
     * must be called prior to any other SmartCache API call and prior to 
     * parsing a configuration/command file.
     *
     * @throw TipcSrvCacheException if the initialization fails.
     */
    void init() throw (TipcException);

    /**
     * Performs initializations required to use SmartCache with last value 
     * caching. This function must be used instead of init if 
     * the client will be asking for last values from the cache. It must 
     * be called prior to any other SmartCache API call and prior to parsing 
     * a configuration/command file.
     * @param cmd_file name of the client application's configuration file.
     * throw TipcSrvException if the initialization fails.
     */
    void initLV(const char * cmd_file) 
    throw (TipcException);

    /**
     * Performs initializations required to use SmartCache to cache GMD message
     * from this RTclient. It sends a message to the group smartCache to start 
     * the GMD message caching for this publisher.
     *
     * NOTE:
     * Use gmdInit() only on the publisher, whose GMD message is 
     * cached by SmartCache.
     *        
     * @param smartCache - subject of SmartCache group
     * @throw TipcSrvCacheException if the GMD init fails.
     */
    void gmdInit() throw (TipcException);

    /**
     * Performs required clean up procedure when SmartCache stops to cache 
     * GMD message for this RTclient. It sends a message to smartCache to 
     * stop the GMD message caching.
     *
     * throw TipcSrvCacheException if the clean up procedure fails.
     */
    void gmdFinish() throw (TipcException);

    /**
     * This function is used to flush the SmartCache for cached messages from 
     * senders. This function sends a message to SmartCache to delete cached
     * messages for the specified subject from the specified RTclient 
     * (identified by its unique subject). The value reqSubj or reqSender 
     * (but not both) can be set to T_CACHE_FLUSH_ALL to flush all the 
     * subjects or senders
     * param reqSubj - Subject to be flushed
     * @param reqSender - Sender to be flushed
     *
     * @throws TipcSrvCacheException if the flush operation is not successful.
     */
    void gmdFlush(const char * reqSubj, const char * reqSender)
    throw (TipcException);

    /**
     * This function is used to request messages from SmartCache. This function 
     * sends an RPC message to the SmartCache group identified by smartCache 
     * requesting the last numMsgs messages (or all the messages available if 
     * set to T_CACHE_ALL) no older than deltaTime seconds (or all the messages 
     * regardless of their age if set to T_CACHE_ALL) cached for the subject 
     * reqSubj. If reqSubj is a wildcard, SmartCache will return messages for 
     * all cached subjects matching the wildcard.
     *
     * SmartCache replies with a message containing all of the messages matching
     * the two criteria. The function then calls TipcSrvMsgProcess() for each 
     * one of these messages.
     *
     * If T_NULL is specified for smartCache the client library will make a 
     * request to each of the SmartCache groups listed in the option 
     * CACHE_GROUP_NAMES. Only the cache group containing the required subject 
     * will reply. If multiple cache groups send replies to the request, 
     * only the first one will be accepted.
     * @param reqSubj - requested subject, can be a wildcard
     * @param numMsgs        - number of messages
     * @param deltaTime        - time span of messages
     * 
     * @throw TipcSrvCacheException if the operation is not successful
     */
    void requestMsgs(const char * reqSubj, int4 numMsgs, 
                     real8 deltaTime) throw (TipcException);

    /**
     * Sends a request to SmartCache to begin caching messages for a 
     * particular subject.
     * @param reqSubj - subject to cache
     * @param numMsgs - max number of messages
     * @param deltaTime        - max time span of messages
     * 
     * @throw TipcSrvCacheException if the operation fails.
     */
    void requestStart(const char * reqSubj, int4 numMsgs, 
                      real8 deltaTime) throw (TipcException);

    /**
     * Sends a request to SmartCache to stop caching messages for a
     * particular subject.
     * @param reqSubj        - subject to cache
     * 
     * @throw TipcSrvCacheException if the stop operation fails.
     */
    void requestStop(const char * reqSubj) throw (TipcException);

    /**
     * Sends a request to SmartCache to return the last message of each 
     * message type published to that subject. SmartCache will return a 
     * T_MT_CACHE_LV_RESPONSE message type.  This message will contain 
     * the last message of each of the message type being cached in the
     * cache in time-sequenced order.  The client library will parse the 
     * reply message from cache to build the returnMt by extracting the 
     * last field from each of the messages.
     *
     * If T_NULL is specified for returnMt, or if the client library does 
     * not have a definition of any of the message types involved, each 
     * of the messages in the reply message from the cache will be 
     * processed in time-sequenced order using TipcSrvMsgProcess.
     * 
     * If T_NULL is specified for smartCache the client library will make a 
     * request to each of the SmartCache groups listed in the option 
     * CACHE_GROUP_NAMES. Only the cache group containing the required 
     * subject will reply. If multiple cache groups send replies to the 
     * request, only the first one will be accepted.
     *
     * @param subject        - subject name for which we want the last value(s)
     * @param returnMt        - the return message type required by the 
     * client application
     *
     * throw TipcSrvCacheException if the operation fails.
     */
    void requestLastValue(const char * subject, const TipcMt& returnMt) 
    throw (TipcException);

    /**
     * This function is used to format and send a request message to SmartCache.
     * Unlike the requestMsgs method, it does not wait for the response.  The
     * bundled response is expected to be handled by the caller.  Therefore, a
     * callback must be in place for the return message type T_MT_CACHE_RESPONSE.
     *
     * If T_NULL is specified for smartCache the client library will make a 
     * request to each of the SmartCache groups listed in the option 
     * CACHE_GROUP_NAMES. Only the cache group containing the required subject 
     * will reply. If multiple cache groups send replies to the request, 
     * only the first one will be accepted.
     * @param reqSubj - requested subject, can be a wildcard
     * @param numMsgs        - number of messages
     * @param deltaTime        - time span of messages
     * 
     * @throw TipcSrvCacheException if the operation is not successful
     */
    void sendRequest(const char * reqSubj, int4 numMsgs, 
                     real8 deltaTime) throw (TipcException);

    /**
     * This function is used to format and send a request message to SmartCache.
     * Unlike the requestMsgs method, it does not wait for the response.  The
     * bundled response is expected to be handled by the caller.  Therefore, a
     * callback must be in place for the return message type T_MT_CACHE_LV_RESPONSE.
     * This function is to get the last value from the cache.
     *
     * If T_NULL is specified for smartCache the client library will make a 
     * request to each of the SmartCache groups listed in the option 
     * CACHE_GROUP_NAMES. Only the cache group containing the required subject 
     * will reply. If multiple cache groups send replies to the request, 
     * only the first one will be accepted.
     * @param reqSubj - requested subject, can be a wildcard
     * @param deltaTime        - time span of messages
     * 
     * @throw TipcSrvCacheException if the operation is not successful
     */
    void sendRequestLastValue(const char * reqSubj,  
                              real8 deltaTime) throw (TipcException);


  };

} //namespace SmartSockets
