/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson4b/receive2.java#1 $
 */

//----------------------------------------------------------------------
// receive2.java -- output a NUMERIC_DATA message with a callback

import java.io.*;
import com.smartsockets.*;

public class receive2 {

  public class receiveCb
    implements TipcProcessCb, TipcDefaultCb {

    public void process(TipcMsg msg, Object arg) {
      System.out.println("Received NUMERIC_DATA message");

      // position the field ptr to the beginning of the message
      try {
        msg.setCurrent(0);
      } catch (TipcException e) {
        Tut.fatal(e);
      } // catch

      // read the data part of the message
      try {
        String var_name;
        while (true) {
          var_name = msg.nextStr();
          double var_value;
          var_value = msg.nextReal8();
          System.out.println("Var name = " + var_name +
                             ", value = " + var_value);
        } // while
      } catch (TipcException e) { }
      // catch end-of-message-data exception, do nothing.
    } // process

    // handle() is for responding to default messages
    public void handle(TipcMsg msg, Object arg) {
        System.out.println("Receive: unexpected message type name" +
          " is <" + msg.getType().getName() + ">");
    } // handle

  } // receiveCb

  public receive2() {
    TipcMsg msg = null;

    // set the ss.project
    try {
      Tut.setOption("ss.project", "smartsockets");
      TipcSrv srv=TipcSvc.getSrv();

      // create a new receive listener and register it
      receiveCb rcb = new receiveCb();
      TipcCb rcbh = srv.addProcessCb(
                       rcb, TipcSvc.lookupMt(TipcMt.NUMERIC_DATA), srv);
      // check the 'handle' returned for validity
      if (null == rcbh) {
        Tut.exitFailure("Couldn't register process listener!\n");
      }

      // register receiveCb again as a default listener
      TipcCb dcbh = srv.addDefaultCb(rcb, srv);
      // check the 'handle' returned for validity
      if (null == dcbh) {
        Tut.exitFailure("Couldn't register default listener!\n");
      }

      // connect to RTserver
      if (!srv.create()) {
        Tut.exitFailure("Couldn't connect to RTserver!\n");
      }
      // subscribe to the appropriate subject
      srv.setSubjectSubscribe("/ss/tutorial/lesson4", true);

      // read and process all incoming messages
      while (null != (msg = srv.next(TipcDefs.TIMEOUT_FOREVER))) {
        srv.process(msg);
      } // while

      // disconnect from RTserver
      srv.destroy();

      // unregister the listeners for completeness
      srv.removeProcessCb(rcbh);
      srv.removeDefaultCb(dcbh);
    } catch (TipcException e) {
      Tut.fatal(e);
    } // catch
  } // receive2 (constructor)

  public static void main(String[] argv) {
    receive2 r = new receive2();
  } // main

} // receive2 class
