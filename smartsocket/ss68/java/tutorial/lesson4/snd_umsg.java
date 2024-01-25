/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson4e/snd_umsg.java#1 $
 */

//----------------------------------------------------------------------
// snd_umsg.java - create and send a series of messages of user-defined
// type XYZ_COORD_DATA

import java.io.*;
import com.smartsockets.*;

public class snd_umsg {
  private static final int XYZ_COORD_DATA = 1001;

  public static void main(String[] argv) {
    try {
      // set the ss.project
      Tut.setOption("ss.project", "smartsockets");

      // get handle to the RTserver
      TipcSrv srv = TipcSvc.getSrv();

      // connect to RTserver
      if (!srv.create()) {
        Tut.exitFailure("Couldn't connect to RTserver!\n");
      } // if

      // define new message type
      TipcMt mt = null;
      try {
        mt = TipcSvc.createMt("xyz_coord_data", XYZ_COORD_DATA,
                              "int4 int4 int4");
      } catch (TipcException e) {
        Tut.exitFailure("Message type already exists!");
      } // catch

      // create a nessage of type XYZ_COORD_DATA
      TipcMsg msg = TipcSvc.createMsg(mt);

      // set message destination
      msg.setDest("/ss/tutorial/lesson4");

      for (int i = 0; i < 30; i += 3) {
        // in order to re-use message, reset number of fields to zero
        msg.setNumFields(0);
        msg.appendInt4(i);
        msg.appendInt4(i + 1);
        msg.appendInt4(i + 2);

        // send and flush the message
        srv.send(msg);
        srv.flush();
      } // for

      // disconnect from RTserver
      srv.destroy();
    } catch (TipcException e) {
      Tut.fatal(e);
    } // catch
  } // main

} // snd_umsg
