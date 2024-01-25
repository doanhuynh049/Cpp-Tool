/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmonext.h#1 $
 */

#ifndef _T_MONITOR_EXT_HH_
#define _T_MONITOR_EXT_HH_

#include <rtworks/ipc.h>
#include <rtworks/tex.h>
#include <rtworks/tsrv.h>
#include <rtworks/tmon.h>

namespace SmartSockets {                     

  /**
   * Inherits from TipcException so users can trap TipcMonExt related
   * exceptions. Offers no additional functionality.
   * @see TipcException, Monitor
   */
  class T_IPCX_API TipcMonExtException : public TipcException {

  public:
    /**
     * Constructor.
     */
    TipcMonExtException() {}

    /**
     * Destructor.
     */
    virtual ~TipcMonExtException () throw() {}

  };

  /**
   * TipcMonExt class 
   */

  class T_IPCX_API TipcMonExt {

    T_IPC_SRV _srv;

  public:
    /**
     * Constructor
     * @param tsrv server connection used to send the monitoring request.  All
     * results received from the monitoring requests should be trapped using this
     * object (either by calling TipcSrv::searchType() or by creating a callback
     * on the appropriate message type.
     * @exception TipcMonException thrown if the connection to RTserver is 
     * invalid.
     */
    TipcMonExt(const TipcSrv &tsrv) throw (TipcException);

    /**
     * Destructor
     */
    virtual ~TipcMonExt() throw();

  public:

    void set(const char * name, bool value)  
      throw (TipcMonExtException);

    void set(const char * name, const bool *value, int4 size)
      throw (TipcMonExtException);

    void set(const char * name, const char * value)
      throw (TipcMonExtException);
    
    void set(const char * name, const char ** value, int4 size)
      throw (TipcMonExtException);

    void set(const char * name, const int4 value)
      throw (TipcMonExtException);
    
    void set(const char * name, const int4 * value, int4 size)
      throw (TipcMonExtException);

    void set(const char * name, const int2 value)
      throw (TipcMonExtException);
    
    void set(const char * name, const int2 * value, int4 size)
      throw (TipcMonExtException);

    void set(const char * name, const int8 value)
      throw (TipcMonExtException);
    
    void set(const char * name, const int8 * value, int4 size)
      throw (TipcMonExtException);

    void set(const char * name, const real8 value)
      throw (TipcMonExtException);
    
    void set(const char * name, const real8 * value, int4 size)
      throw (TipcMonExtException);

    void set(const char * name, const real16 value)
      throw (TipcMonExtException);
    
    void set(const char * name, const real16 * value, int4 size)
      throw (TipcMonExtException);

    void set(const char * name, const real4 value)
      throw (TipcMonExtException);
    
    void set(const char * name, const real4 * value, int4 size)
      throw (TipcMonExtException);

    void setUtf8(const char * name, const char * value)
      throw (TipcMonExtException);
    
    void setUtf8Array(const char * name, const char ** value, int4 size)
      throw (TipcMonExtException);

    void set(const char * name, const void * value, int4 size)
      throw (TipcMonExtException);

    void clear(const char * name)
      throw (TipcMonExtException);

  };

} //namespace SmartSockets

#endif // _T_MONITOR_EXT_HH_
