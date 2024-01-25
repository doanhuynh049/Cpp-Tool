/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson1/receive.java#1 $
 */

//----------------------------------------------------
// Program 2: receive.java

import java.io.*;
import com.smartsockets.*;

public class receive {

  public static void main(String[] argv) {
    TipcMsg msg = null;
    String text = null;

    /*
     * First, an instance of the RTserver object is used
     * to subscribe  to the "/ss/tutorial/lesson1" subject,
     * then the code blocks forever, waiting for a message
     * to be received.  The parameter is the time (in seconds)
     * to wait ; TipcDefs.TIMEOUT_FOREVER meaning "forever."
     * If  you do not want to block forever, specify some number
     * of seconds, say 10.0, which means the method will return
     * null if the message is not received within 10 seconds.
     */

    TipcSrv srv = TipcSvc.getSrv();
    try {
      srv.setSubjectSubscribe("/ss/tutorial/lesson1", true);
      msg = srv.next(TipcDefs.TIMEOUT_FOREVER);

    /*
     * Now there is a message to work with.  First set the current
     * field index to the first field, then get that field, which is
     * a String.
     */
      msg.setCurrent(0);
      text = msg.nextStr();
    } catch (TipcException e) {
      Tut.fatal(e);
    } // try-catch

    /*
     * Lastly, print the string.
     */
    System.out.println("Text from INFO message = " + text);
  } // main
} // receive

