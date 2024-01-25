/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson1/send.java#1 $
 */

//---------------------------------------------------------------
// Program 1: send.java

import java.io.*;
import com.smartsockets.*;

public class send {

  public static void main(String[] argv) {
    /*
     * First an instance of the RTserver object is called to
     * send the message.
     */
    TipcSrv srv = TipcSvc.getSrv();

    /*
     * Then a  message is created using the INFO message type.
     * The destination "/ss/tutorial/lesson1" subject is set,
     * and the string "Hello World" is appended.
     */
    TipcMsg msg = TipcSvc.createMsg(TipcMt.INFO);
    msg.setDest("/ss/tutorial/lesson1");
    msg.appendStr("Hello World!");

    /*
     * The send method automatically creates a connection to an RTserver
     * (which must already be running), and queues the message to be sent.
     * Then the queue is flushed to ensure the message is sent immediately.
     * We must also destroy the server connection to guarantee the flush
     * is carried out, since it is at the end of the program's execution.
     */

    try {
      srv.send(msg);
      srv.flush();
      srv.destroy();
    } catch (TipcException te) {
      Tut.warning(te);
    } // catch
  } // main
} // send
