/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson4c/receive3.java#1 $
 */

//----------------------------------------------------------------------
// receive3.java -- output NUMERIC_DATA and other message with callbacks

import java.io.*;
import com.smartsockets.*;

public class receive3 {

  public class receiveCb
    implements TipcProcessCb, TipcDefaultCb {

    public void process(TipcMsg msg, Object arg) {
      System.out.println("Received NUMERIC_DATA message");

      // position the field ptr to the beginning of the message
      try {
        msg.setCurrent(0);
      }
      catch (TipcException e) {
        Tut.fatal(e);
      }

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
      } catch (TipcException e) {
        // catch end-of-message-data exception, do nothing.
      } // catch
    } // process

    // handle() is for responding to default messages
    public void handle(TipcMsg msg, Object arg) {
        System.out.println("Receive: unexpected message type name" +
          " is <" + msg.getType().getName() + ">");
    } // handle

  } // receiveCb

  public receive3() {
    TipcMsg msg = null;

    // set the ss.project
    try {
      Tut.setOption("ss.project", "smartsockets");
      TipcSrv srv=TipcSvc.getSrv();

      // create a new receive callback and register it
      receiveCb rl = new receiveCb();
      TipcCb rcbh = srv.addProcessCb(
                       rl, TipcSvc.lookupMt(TipcMt.NUMERIC_DATA), srv);
      // check the 'handle' returned for validity
      if (null == rcbh) {
        Tut.exitFailure("Couldn't register process callback!\n");
      }

      // register receiveCb again as a default callback
      TipcCb dcbh = srv.addDefaultCb(rl, srv);
      // check the 'handle' returned for validity
      if (null == dcbh) {
        Tut.exitFailure("Couldn't register default callback!\n");
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

      // unregister the callbacks
      srv.removeProcessCb(rcbh);
      srv.removeDefaultCb(dcbh);

      // disconnect from RTserver
      srv.destroy();
    } catch (TipcException e) {
      Tut.fatal(e);
    } // catch
  } // receive3 (constructor)

  public static void main(String[] argv) {
    receive3 r = new receive3();
  } // main

} // receive3 class
