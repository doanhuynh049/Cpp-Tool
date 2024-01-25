/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson4e/rcv_umsg.java#1 $
 */

//----------------------------------------------------------------------
// rcv_umsg.java -- display contents of received user-defined
// messages (type XYZ_COORD_DATA)

import java.io.*;
import com.smartsockets.*;

public class rcv_umsg {
  static final int XYZ_COORD_DATA = 1001;

  public class processXYZ implements TipcProcessCb {

    public void process(TipcMsg msg, Object arg) {
      System.out.println("Received XYZ_COORD_DATA message");

      // position the field ptr to the beginning of the message
      try {
        msg.setCurrent(0);
      } catch (TipcException e) {
        Tut.fatal(e);
      } // catch

      // traverse message; print value from each field
      try {
        int field_value = msg.nextInt4();
        System.out.println("Field 1 Value = " + field_value);
        field_value = msg.nextInt4();
        System.out.println("Field 2 Value = " + field_value);
        field_value = msg.nextInt4();
        System.out.println("Field 3 Value = " + field_value);
      } catch (TipcException e) {
        Tut.warning("Expected 3 fields--bad XYZ_COORD_DATA!\n");
      } // catch
    } // process
  } // processXYZ

  public class processDefault implements TipcDefaultCb {

    public void handle(TipcMsg msg, Object arg) {
        System.out.println("Receive: unexpected message type name" +
               " is <" + msg.getType().getName() + ">");
    } // handle
  } // processDefault

  public rcv_umsg() {
    TipcMsg msg = null;

    try {
      // set the ss.project
      Tut.setOption("ss.project", "smartsockets");

      // get handle to the RTserver
      TipcSrv srv=TipcSvc.getSrv();

      // define new message type
      TipcMt mt = null;
      try {
        mt = TipcSvc.createMt("xyz_coord_data", XYZ_COORD_DATA,
                              "int4 int4 int4");
      } catch (TipcAlreadyExistsException e) {
        Tut.exitFailure("Message type already exists!");
      } // catch

      // create a new receive listener and register it
      processXYZ pcb = new processXYZ();
      TipcCb ph = srv.addProcessCb(pcb, mt, srv);
      // check the 'handle' returned for validity
      if (null == ph) {
        Tut.exitFailure("Couldn't register processXYZ listener!\n");
      } // if

      // create and register default listener
      processDefault dcb = new processDefault();
      TipcCb dh = srv.addDefaultCb(dcb, srv);

      // check the 'handle' returned for validity
      if (null == dh) {
        Tut.exitFailure("Couldn't register default listener!\n");
      } // if

      // connect to RTserver
      if (!srv.create()) {
        Tut.exitFailure("Couldn't connect to RTserver!\n");
      } // if

      // subscribe to the appropriate subject
      srv.setSubjectSubscribe("/ss/tutorial/lesson4", true);

      // read and process all incoming messages
      while (srv.mainLoop(TipcDefs.TIMEOUT_FOREVER)) {
      } // while

      // unregister the listeners for completeness
      srv.removeProcessCb(ph);
      srv.removeDefaultCb(dh);

      // disconnect from RTserver
      srv.destroy();
    } catch (TipcException e) {
      Tut.fatal(e);
    } // catch
  } // rcv_umsg (constructor)

  public static void main(String[] argv) {
    new rcv_umsg();
  } // main

} // rcv_umsg class
