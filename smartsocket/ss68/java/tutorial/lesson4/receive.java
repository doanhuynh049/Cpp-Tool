/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson4a/receive.java#1 $
 */

//---------------------------------------------------------------
// receive.java -- output a NUMERIC_DATA via callback

import java.io.*;
import com.smartsockets.*;

public class receive {

  public class receiveCb implements TipcProcessCb {

    public void process(TipcMsg msg, Object arg) {
      System.out.println("Received NUMERIC_DATA message.\n");

      // position the field ptr to the beginning of the message
      try {
        msg.setCurrent(0);
      } catch (TipcException e) {
        Tut.fatal(e);
      }

      System.out.println("Contents of NUMERIC_DATA message:\n" +
                         "---------------------------------");

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

  } // receiveCb

  public receive() {
    TipcMsg msg = null;

    // set the project
    try {
      Tut.setOption("ss.project", "smartsockets");
      TipcSrv srv=TipcSvc.getSrv();

      // connect to RTserver
      if (!srv.create()) {
        Tut.exitFailure("Couldn't connect to RTserver!\n");
      } // if
      // subscribe to the appropriate subject
      srv.setSubjectSubscribe("/ss/tutorial/lesson4", true);

      // create a new receive callback and register it
      receiveCb rcb = new receiveCb();
      TipcCb rcbh = srv.addProcessCb(rcb, TipcMt.NUMERIC_DATA, srv);
      // check the 'handle' returned for validity
      if (null == rcbh) {
        Tut.exitFailure("Couldn't register process listener!\n");
      } // if

      // read and process a message
      msg = srv.next(TipcDefs.TIMEOUT_FOREVER);

      // all callbacks are triggered by TipcSrv's process() method
      srv.process(msg);

      // clean up and disconnect from RTserver
      srv.removeProcessCb(rcbh);
      srv.destroy();
    } catch (TipcException e) {
      Tut.fatal(e);
    } // catch
  } // receive (constructor)

  public static void main(String[] argv) {
    receive r = new receive();
  } // main

} // receive
