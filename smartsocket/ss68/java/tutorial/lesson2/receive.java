/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson2/receive.java#1 $
 */

//----------------------------------------------------
// Program 2: receive.java

import java.io.*;
import com.smartsockets.*;

public class receive {

  public static void main(String[] argv) {
    TipcMsg msg = null;
    String text = null;

    try {
      Tut.setOption("ss.project", "smartsockets");
      TipcSrv srv=TipcSvc.getSrv();
      if (!srv.create()) {
        Tut.exitFailure("Couldn't connect to RTserver!\n");
      }
      srv.setSubjectSubscribe("/ss/tutorial/lesson2", true);
      msg = srv.next(TipcDefs.TIMEOUT_FOREVER);

      msg.setCurrent(0);
      text = msg.nextStr();
    } catch (TipcException e) {
      Tut.fatal(e);
    } // catch

    System.out.println("Text from INFO message = " + text);
  } // main
} // receive

