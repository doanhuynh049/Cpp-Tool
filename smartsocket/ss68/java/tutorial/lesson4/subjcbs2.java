/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson4f/subjcbs2.java#1 $
 */

//----------------------------------------------------------------------
// subjcbs2.java -- output messages via subject/mt callbacks

import java.io.*;
import com.smartsockets.*;

public class subjcbs2 {

  public class processInfo implements TipcProcessCb {

    public void process(TipcMsg msg, Object arg) {
      System.out.println("*** Received INFO message");
      try {
        msg.setCurrent(0);
        System.out.println("Text from message = " + msg.nextStr());
      } // catch
      catch (TipcException e) { }
    } // process
  } // processInfo

  public class processNumData implements TipcProcessCb {

    public void process(TipcMsg msg, Object arg) {
      System.out.println("*** Received NUMERIC_DATA message");
      String var_name;
      try {
        msg.setCurrent(0);
        // display the repeating part of NUMERIC_DATA message
        while (true) {
          var_name = msg.nextStr();
          double var_value;
          var_value = msg.nextReal8();
          System.out.println("Var name = " + var_name +
                             ", value = " + var_value);
          } // while
        // catch end-of-message-data exception, do nothing.
        } catch (TipcException e) { }
      } // process
    } // processNumData

  public subjcbs2() {
    TipcMsg msg = null;

    // set the ss.project
    try {
      Tut.setOption("ss.project", "smartsockets");
      TipcSrv srv=TipcSvc.getSrv();

      // create a new info mt/subject callback and register it
      processInfo pi = new processInfo();
      TipcCb rcbh1 = srv.addProcessCb(pi, TipcSvc.lookupMt(TipcMt.INFO),
                                     "/ss/tutorial/lesson4", null);
      // check the 'handle' returned for validity
      if (null == rcbh1) {
        Tut.exitFailure("Couldn't register subject callback!\n");
      } // if

      // create a new info mt/subject callback and register it
      processNumData pnd = new processNumData();
      TipcCb rcbh2 = srv.addProcessCb(pnd,
                         TipcSvc.lookupMt(TipcMt.NUMERIC_DATA),
                         "/ss/tutorial/lesson4", null);
      // check the 'handle' returned for validity
      if (null == rcbh2) {
        Tut.exitFailure("Couldn't register subject callback!\n");
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
      srv.removeProcessCb(rcbh1);
      srv.removeProcessCb(rcbh2);

      // disconnect from RTserver
      srv.destroy();
    } catch (TipcException e) {
      Tut.fatal(e);
    } // catch
  } // subjcbs2 (constructor)

  public static void main(String[] argv) {
    new subjcbs2();
  } // main

} // subjcbs2 class
