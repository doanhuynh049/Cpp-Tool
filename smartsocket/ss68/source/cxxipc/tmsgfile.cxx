/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tmsgfile.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmsgfile.hxx>

/* ========================================================================= */
/*..TipcMsgFile::TipcMsgFile -- TipcMsgFile constructor */
TipcMsgFile::TipcMsgFile(T_STR file_name, 
			 T_IPC_MSG_FILE_CREATE_MODE create_mode)
{
  _msgFile = TipcMsgFileCreate(file_name, create_mode);

  if (T_NULL == _msgFile) {
    _status = T_FALSE;
  }

}


/* ========================================================================= */
/*..TipcMsgFile::TipcMsgFile -- TipcMsg file constructor */
TipcMsgFile::TipcMsgFile(FILE* stream, 
			 T_IPC_MSG_FILE_CREATE_MODE create_mode)
{
  _msgFile = TipcMsgFileCreateFromFile(stream, create_mode);
}


/* ========================================================================= */
/*..TipcMsgFile::~TipcMsgFile -- TipcMsgFile destructor */
TipcMsgFile::~TipcMsgFile()
{
  TipcMsgFileDestroy(_msgFile);
}


/* ========================================================================= */
/*..TipcMsgFile::operator -- insert a TipcMsg object into a message file */
TipcMsgFile& TipcMsgFile::operator<<(const TipcMsg& msg)
{
  _status = TipcMsgFileWrite(_msgFile, msg);
  return *this;
}


/* ========================================================================= */
/*..TipcMsgFile& TipcMsgFile -- extract a TipcMsg object from a message file */
TipcMsgFile& TipcMsgFile::operator>>(TipcMsg& msg)
{
  T_IPC_MSG c_api_msg;

  _status = TipcMsgFileRead(_msgFile, &c_api_msg);

  msg.destroyMsg();

  /* -------------------------------------------- */
  /* If file read failed, return a vacant object. */
  /* -------------------------------------------- */
  if (_status) {
    msg.absorb(c_api_msg);
  }
  else {
    msg._msg = (T_IPC_MSG)T_NULL;
  }

  return *this;
}


