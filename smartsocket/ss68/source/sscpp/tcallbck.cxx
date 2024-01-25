/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/tcallbck.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tcallbck.h>
#include <rtworks/tconn.h>
#include <rtworks/tmsg.h>
#include <rtworks/tutil.h>


namespace SmartSockets {
  /* ======================================================================= */
  /*..ConnectionCallBack::onConnection -- Default connection callback */
  void ConnectionCallBack::onConnection( 
                                  CallBack <ConnectionCallBack> * callBack,
                                  TipcSrv & connection,
                                  ConnStatus oldConnStatus,
                                  ConnStatus newConnStatus ) 
  {
		Utilities::out("Default Connection callback triggered: \n");
    //callBack->print((T_OUT_FUNC)TutOut);
    //Utilities::out("Connection was: \n");
    //connection.print((T_OUT_FUNC)TutOut);
    Utilities::out("Connection was %d and now is %d\n", oldConnStatus,
            newConnStatus);
  }


  /* ======================================================================= */
  /*..MessageCallBack::onMessage -- Default message callback */
  void MessageCallBack::onMessage( CallBack <MessageCallBack> * callBack,
                                   TipcMsg & msg,
                                   TipcConn & conn )
  { 
    Utilities::out("Message callback triggered: \n");
    callBack->print((T_OUT_FUNC)TutOut);
    Utilities::out("Connection was: \n");
    conn.print((T_OUT_FUNC)TutOut);
    Utilities::out("Message was: \n");
    msg.print((T_OUT_FUNC)TutOut);
  }


  /* ======================================================================= */
  /*..ErrorCallBack::onError -- Default error callback */
  void ErrorCallBack::onError( CallBack <ErrorCallBack> * callBack,
                               Exception & error,
                               TipcConn & conn )
  { 
    Utilities::out("Error callback triggered: \n");
    callBack->print((T_OUT_FUNC)TutOut);
    Utilities::out("Connection was: \n");
    conn.print((T_OUT_FUNC)TutOut);
    Utilities::out("Error was: \n");
    Utilities::out("%s\n", error.what());
  }


  /* ======================================================================= */
  /*..MessageQueueCallBack::onMessage -- Default queue callback */
  void MessageQueueCallBack::onMessage( 
                                   CallBack<MessageQueueCallBack> * callBack,
                                   TipcMsg & message,
                                   TipcConn & connection,
                                   uint4 queuePos,
                                   bool is_insert )
  {
    Utilities::out("Message queue callback triggered: \n");
    callBack->print((T_OUT_FUNC)TutOut);
    Utilities::out("Connection was: \n");
    connection.print((T_OUT_FUNC)TutOut);
    Utilities::out("Message was: \n");
    message.print((T_OUT_FUNC)TutOut);
    Utilities::out("Position: %d\n", queuePos);
    Utilities::out("Insert: %d\n", is_insert);
  }


  /* ======================================================================= */
  /*..EncodeCallBack::onMessage -- Default encode callback */
  void EncodeCallBack::onMessage( CallBack<EncodeCallBack> * callBack,
                                  void * packet,
                                  int4 packetSize,
                                  void * origPacket,
                                  int4 origPacketSize,
                                  TipcMsg & origMessage,
                                  TipcConn & connection ) 
  {
    Utilities::out("Encode callback triggered: \n");
    callBack->print((T_OUT_FUNC)TutOut);
    Utilities::out("Connection was: \n");
    connection.print((T_OUT_FUNC)TutOut);
    Utilities::out("Original message was: \n");
    origMessage.print((T_OUT_FUNC)TutOut);
    Utilities::out("Original packet size: %d\t%d\n", origPacketSize, 
                                             sizeof(origPacket));
    Utilities::out("New packet size: %d\t%d\n", packetSize, sizeof(packet));
  }


  /* ======================================================================= */
  /*..DecodeCallBack::onMessage -- Default decode callback */
  void DecodeCallBack::onMessage( CallBack<DecodeCallBack> * callBack,
                                  void * packet,
                                  int4 packetSize,
                                  void * origPacket,
                                  int4 origPacketSize,
                                  TipcConn & connection ) 
  {
    Utilities::out("Decode callback triggered: \n");
    callBack->print((T_OUT_FUNC)TutOut);
    Utilities::out("Connection was: \n");
    connection.print((T_OUT_FUNC)TutOut);
    Utilities::out("Original packet size: %d\t%d\n", origPacketSize, 
                                             sizeof(origPacket));
    Utilities::out("New packet size: %d\t%d\n", packetSize, sizeof(packet));
  }
}
