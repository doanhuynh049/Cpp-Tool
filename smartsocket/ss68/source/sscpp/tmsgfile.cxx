/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tmsgfile.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmsgfile.h>

namespace SmartSockets {

  /* ======================================================================= */
  /*..TipcMsgFile::TipcMsgFile -- TipcMsgFile constructor */
  TipcMsgFile::TipcMsgFile(char * file_name, 
                           T_IPC_MSG_FILE_CREATE_MODE create_mode) 
  {
    _msgFile = TipcMsgFileCreate(file_name, create_mode);
    if (!_msgFile) {
      throw TipcMsgFileException();
    }
  }


  /* ======================================================================= */
  /*..TipcMsgFile::TipcMsgFile -- TipcMsg file constructor */
  TipcMsgFile::TipcMsgFile(FILE* file, 
                           T_IPC_MSG_FILE_CREATE_MODE create_mode) 
  {
    _msgFile = TipcMsgFileCreateFromFile(file, create_mode);
    if (!_msgFile) {
      throw TipcMsgFileException();
    }
  }


  /* ======================================================================= */
  /*..TipcMsgFile::~TipcMsgFile -- TipcMsgFile destructor */
  TipcMsgFile::~TipcMsgFile() throw()
  {
    TipcMsgFileDestroy(_msgFile);
  }


  /* ======================================================================= */
  /*..TipcMsgFile::operator -- insert a TipcMsg object into a message file */
  TipcMsgFile& TipcMsgFile::operator<<(const TipcMsg& msg) 
  throw (TipcException)
  {
    if (!TipcMsgFileWrite(_msgFile, msg)) {
      throw TipcMsgFileException();
    }

    return *this;
  }


  /* ======================================================================= */
  /*..TipcMsgFile& TipcMsgFile -- extract a TipcMsg object from a message file */
  TipcMsgFile& TipcMsgFile::operator>>(TipcMsg& msg)
  {
    T_IPC_MSG c_api_msg;

    bool status = TipcMsgFileRead(_msgFile, &c_api_msg);

    msg.destroyMsg();

    /* -------------------------------------------- */
    /* If file read failed, return a vacant object. */
    /* -------------------------------------------- */
    if (status) {
      msg.absorb(c_api_msg);
    }
    else {
      msg._msg = (T_IPC_MSG)T_NULL;
    }

    return *this;
  }

} //namespace SmartSockets
