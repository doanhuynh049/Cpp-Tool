/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson2/send.java#1 $
 */

//--------------------------------------------------------------
// Program 2: send.java

import java.io.*;
import com.smartsockets.*;

public class send {
  public static void main(String[] argv) {
    try {
      Tut.setOption("ss.project", "smartsockets");
      TipcSrv srv=TipcSvc.getSrv();
      if (!srv.create()) {
        Tut.exitFailure("Couldn't connect to RTserver!\n");
      }
      TipcMsg msg = TipcSvc.createMsg(TipcMt.INFO);
      msg.setDest("/ss/tutorial/lesson2");
      msg.appendStr("Hello World!");
      srv.send(msg);
      srv.flush();

      /*
       * Should always destroy before exiting code.
       */
      srv.destroy();
    } catch (TipcException e) {
      Tut.warning(e);
    } // catch
  } // main
}  // send
