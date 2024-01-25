/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tobj.hxx#1 $
 */

#ifndef _T_OBJ_HH_
#define _T_OBJ_HH_

#include <rtworks/ipc.h>

class T_IPCX_API Tobj {

protected:
  T_BOOL    _status;
  
  Tobj();
  virtual ~Tobj();

public:
  // --------------------------------------------------------
  //  Methods for accessing status
  // --------------------------------------------------------
  T_BOOL operator!() const { return !_status; }
  T_BOOL Status()    const { return _status; }

};

#endif



