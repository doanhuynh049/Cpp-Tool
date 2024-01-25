/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson4d/svrcbs.java#1 $
 */

//--------------------------------------------------------------------
// svrcbs.java -- server create/destroy callbacks

import java.io.*;
import com.smartsockets.*;

public class svrcbs {
  String password_correct = "ssjava";

  public class serverConnect implements TipcCreateCb {

    public void create(Object srv_obj) {
      TipcSrv srv = (TipcSrv)srv_obj;
      System.out.println("Connecting to RTserver...");
      System.out.print("Please enter password: ");
      BufferedReader in = new BufferedReader(
        new InputStreamReader(System.in) );
      String password_entered = null;
      try {
        password_entered = in.readLine();
      } catch (IOException e) {
        System.out.println("Error! "+e.getMessage());
      } // catch
      if (password_entered.equals(password_correct)) {
        System.out.println("Password accepted!");
      }
      else {
        System.out.println("Password is not correct!\n" +
               "You are being disconnected from RTserver");
        try {
          srv.destroy();
          Tut.exitSuccess();
        } catch (TipcException e) {
          Tut.warning("Can't destroy server connection: " +
            e.getMessage() + "\n");
        } // catch
      } // else
    } // create
  } // serverConnect

  public class serverDisconnect implements TipcDestroyCb {

    public void destroy(Object obj) {
      System.out.println("...Disconnecting from RTserver");
    } // destroy
  } // serverDisconnect

  public svrcbs() {
    TipcMsg msg = null;

    try {
      TipcSrv srv = TipcSvc.getSrv();

      // create a new connect callback and register it
      serverConnect sc = new serverConnect();
      TipcCb sch = srv.addCreateCb(sc, srv);
      // check the 'handle' returned for validity
      if (null == sch) {
        Tut.exitFailure("Couldn't register create callback!\n");
      } // if

      // and a destroy callback
      serverDisconnect sd = new serverDisconnect();
      TipcCb sdh = srv.addDestroyCb(sd, srv);
      // check the 'handle' returned for validity
      if (null == sdh) {
        Tut.exitFailure("Couldn't register destroy callback!\n");
      } // if

      // connect to RTserver
      srv.create();

      // read and process all incoming messages
      while (true) {
        try {
      srv.mainLoop(2.0);
        } catch (TipcException e) { }
      } // while

    } catch (TipcException e) {
      Tut.fatal(e);
    } // catch
  } // svrcbs (constructor)

  public static void main(String[] argv) {
    new svrcbs();
  } // main

} // svrcbs
