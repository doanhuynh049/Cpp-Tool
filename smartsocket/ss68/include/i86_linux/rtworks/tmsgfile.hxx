/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmsgfile.hxx#1 $
 */

#ifndef _T_MSGFILE_HH_
#define _T_MSGFILE_HH_

#include <stdio.h>
#include <rtworks/tmsg.hxx>
#include <rtworks/tobj.hxx>

class T_IPCX_API TipcMsgFile : public Tobj {

private:
  T_IPC_MSG_FILE _msgFile;

public:
  TipcMsgFile(T_STR filename, T_IPC_MSG_FILE_CREATE_MODE create_mode);
  TipcMsgFile(FILE* file,     T_IPC_MSG_FILE_CREATE_MODE create_mode);

  virtual ~TipcMsgFile();

  virtual TipcMsgFile& operator<<(const TipcMsg& msg);

  virtual TipcMsgFile& operator>>(TipcMsg& msg);

};

#endif




