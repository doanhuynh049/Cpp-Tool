/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson4c/send3.java#1 $
 */

//----------------------------------------------------------------------
// send3.java -- write an INFO and then NUMERIC_DATA messages

import java.io.*;
import com.smartsockets.*;

public class send3 {

  public static void main(String[] argv) {
    try {
      Tut.setOption("ss.project", "smartsockets");

      TipcSrv srv=TipcSvc.getSrv();
      if (!srv.create()) {
        Tut.exitFailure("Couldn't connect to RTserver!\n");
      } // if

      // send a message of type INFO
      TipcMsg msgi = TipcSvc.createMsg(TipcMt.INFO);
      msgi.setDest("/ss/tutorial/lesson4");
      msgi.appendStr("Hello World!");
      srv.send(msgi);
      srv.flush();

      // create a message of type NUMERIC_DATA
      TipcMsg msg = TipcSvc.createMsg(TipcMt.NUMERIC_DATA);

      // set the destination subject of the message
      msg.setDest("/ss/tutorial/lesson4");

      // each time through the loop send a NUMERIC_DATA
      // message with three values
      for (int i = 0; i < 30; i = i + 3) {
        msg.setNumFields(0);
        msg.appendStr("X");
        msg.appendReal8(i);
        msg.appendStr("Y");
        msg.appendReal8(i+1.0);
        msg.appendStr("Z");
        msg.appendReal8(i+2.0);

        // send the message
        srv.send(msg);
        srv.flush();
      }

      // disconnect from RTserver
      srv.destroy();
    } catch (TipcException e) {
      Tut.warning(e);
    } // catch
  } // main

} // send3
