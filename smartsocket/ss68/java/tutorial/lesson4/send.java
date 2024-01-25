/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson4a/send.java#1 $
 */

//-------------------------------------------------------------
// send.java -- write a NUMERIC_DATA message

import java.io.*;
import com.smartsockets.*;

public class send {

  public static void main(String[] argv) {
    try {
      Tut.setOption("ss.project", "smartsockets");
      TipcSrv srv=TipcSvc.getSrv();

      if (!srv.create()) {
        Tut.exitFailure("Couldn't connect to RTserver!\n");
      } // if
      // create a message of type NUMERIC_DATA
      TipcMsg msg = TipcSvc.createMsg(TipcMt.NUMERIC_DATA);

      // set the destination subject of the message
      msg.setDest("/ss/tutorial/lesson4");

      // build a NUMERIC_DATA msg with 3 variable-value pairs,
      // set as follows (X, 10.0), (Y, 20.0) and (Z, 30.0)
      msg.appendStr("X");
      msg.appendReal8(10.0);
      msg.appendStr("Y");
      msg.appendReal8(20.0);
      msg.appendStr("Z");
      msg.appendReal8(30.0);

      // send the message
      srv.send(msg);
      srv.flush();

      // disconnect from RTserver
      srv.destroy();
    } catch (TipcException e) {
      Tut.warning(e);
    } // catch
  } // main

} // send
