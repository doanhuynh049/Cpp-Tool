/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson4f/subjcbs.java#1 $
 */

//----------------------------------------------------------------------
// subjcbs.java -- output messages via subject callbacks

import java.io.*;
import com.smartsockets.*;

public class subjcbs {

  public class processLesson4 implements TipcProcessCb {

    public void process(TipcMsg msg, Object arg) {
      System.out.println("*** Received message of type <" +
                         msg.getType().getName()+">");

      // position the field ptr to the beginning of the message
      try {
        msg.setCurrent(0);
      }
      catch (TipcException e) {
        Tut.fatal(e);
      }

      // display message contents based on type
      int mt = msg.getType().getNum();
      switch (mt) {
      case TipcMt.INFO:
        try {
          System.out.println("Text from message = " + msg.nextStr());
        } catch (TipcException e) { }
        break;

      case TipcMt.NUMERIC_DATA:
        String var_name;
        try {
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
        break;

      default:
        // handle messages of unknown type
        System.out.println("Unable to process messages of this type!");
        break;
      } // switch
    } // process

  } // processLesson4

  public subjcbs() {
    TipcMsg msg = null;

    // set the ss.project
    try {
      Tut.setOption("ss.project", "smartsockets");
      TipcSrv srv=TipcSvc.getSrv();

      // create a new receive SUBJECT callback and register it
      processLesson4 pl = new processLesson4();
      TipcCb rcbh = srv.addProcessCb(pl, "/ss/tutorial/lesson4", srv);
      // check the 'handle' returned for validity
      if (null == rcbh) {
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
      srv.removeProcessCb(rcbh);

      // disconnect from RTserver
      srv.destroy();
    } catch (TipcException e) {
      Tut.fatal(e);
    } // catch
  } // subjcbs (constructor)

  public static void main(String[] argv) {
    new subjcbs();
  } // main

} // subjcbs class
