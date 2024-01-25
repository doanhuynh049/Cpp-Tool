/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmsgfile.h#1 $
 */

#ifndef _T_MSGFILE_HH_
#define _T_MSGFILE_HH_

#include <stdio.h>
#include <rtworks/tmsg.h>

namespace SmartSockets {

  /**
   * Inherits from TipcException so users can trap TipcMsgFile related
   * exceptions. Offers no additional functionality.
   *
   * @see TipcException, TipcMsgFile
   */
  class T_IPCX_API TipcMsgFileException : public TipcException {

  public:
    /**
     * Constructor.
     */
    TipcMsgFileException () {};

    /**
     * Destructor.
     */
    virtual ~TipcMsgFileException () throw() {}
  };

  /**
   * TipcMsgFile - user class for message file I/O operations
   * The TipcMsgFile class provides a C++ binding to the SmartSockets 
   * TipcMsgFile API functions using a virtual I/O stream model. It 
   * allows you to create, read, write, append to and close SmartSockets 
   * message files using C++ stream I/O syntax.
   */
  class T_IPCX_API TipcMsgFile {

  private:
    T_IPC_MSG_FILE _msgFile;

  public:
    /**
     * Create a TipcMsgFile object with file_name and create_mode. 
     * @param file_name - the name of the file to use for the message file. 
     * @param create_mode - specifies how to create the file.
     */
    TipcMsgFile(char * filename, T_IPC_MSG_FILE_CREATE_MODE create_mode);

    /**
     * Create a TipcMsgFile object with file and create_mode. 
     * @param file - an existing file to use for the message file. 
     * @param create_mode - specifies how to create (that is,open) the file.
     */
    TipcMsgFile(FILE* file, T_IPC_MSG_FILE_CREATE_MODE create_mode);

    ///Destructor
    virtual ~TipcMsgFile() throw();

    /**
     * Write the internal T_IPC_MSG field of a TipcMsg object to a 
     * message file.
     */
    virtual TipcMsgFile& operator<<(const TipcMsg& msg)
    throw (TipcException);

    /**
     * Read a message from a message file and assign it to the internal 
     * T_IPC_MSG field of a TipcMsg object. Any message that the object 
     * was managing at the time of the call is passed to TipcMsgDestroy().
     */
    virtual TipcMsgFile& operator>>(TipcMsg& msg);

  };

} // namespace SmartSockets

#endif //_T_MSGFILE_HH_
