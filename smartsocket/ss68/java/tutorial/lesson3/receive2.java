/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson3/receive2.java#1 $
 */

//----------------------------------------------------------------
// receive2.java -- receive a NUMERIC_DATA message

import java.io.*;
import com.smartsockets.*;

public class receive2 {

  public static void main(String[] argv) {
    TipcMsg msg = null;
    TipcSrv srv = null;

    // set the ss.project
    try {
      Tut.setOption("ss.project", "smartsockets");
      srv=TipcSvc.getSrv();

      // connect to RTserver
      if (!srv.create()) {
        Tut.exitFailure("Couldn't connect to RTserver!\n");
      } // if
      // subscribe to the appropriate subject
      srv.setSubjectSubscribe("/ss/tutorial/lesson3", true);
      msg = srv.next(TipcDefs.TIMEOUT_FOREVER);
    }
    catch (TipcException e) {
      Tut.fatal(e);
    } // catch

    // position the field ptr to the beginning of the message
    try {
      msg.setCurrent(0);
    } catch (TipcException e) {
      Tut.fatal(e);
    } // catch

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
    }
    // catch end-of-message-data exception, do nothing.
    catch (TipcException e) {
      // we expect this exception from the while loop
    } // catch

    // drop our connection to RTserver
    try {
      srv.destroy();
    } catch (TipcException e) {
      // not concerned with problems dropping connection
    } // catch
  } // main

} // receive2

