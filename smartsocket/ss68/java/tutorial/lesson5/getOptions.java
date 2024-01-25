/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson5/getOptions.java#1 $
 */

// getOptions.java
// Example of retrieving SmartSockets and user-defined options
// settings from a file, URL, or InputStream
// NOTE: read 'readme.txt' for info on the 'remote.opt' file!

import java.io.*;
import java.util.*;
import com.smartsockets.*;

public class getOptions {

  static public void main(String[] argv) {
    final String optionFile = "local.opt";

    // must create enumerated map option BEFORE loading!
    TipcOption bulb = null;
    try {
      bulb = Tut.createOption("ss.bulb", "off");
    }
    catch (TipcException te) {
      Tut.fatal(te);
    }
    bulb.addEnumMapLegalValue("on", 1);
    bulb.addEnumMapLegalValue("off", 0);
    bulb.addEnumMapLegalValue("broken", -1);

    System.out.print("Getting options from ");

    // if argument was supplied, assume it's a URL to properties
    if (0 < argv.length) {
      System.out.println("URL " + argv[0] +"\n");
      Tut.loadOptionsURL(argv[0]);
    }
    else {
      // no argument, so use local options file
      System.out.println("local file " + optionFile + "\n");
      Tut.loadOptionsFile(optionFile);
    } // if

    try {
      String project = Tut.getOptionStr("ss.project");
      System.out.println("ss.project = " + project);

      String server_names = Tut.getOptionStr("ss.server_names");
      System.out.println("ss.server_names = " + server_names);

      String user_name = Tut.getOptionStr("ss.user_name");
      System.out.println("ss.user_name = " + user_name);

      // get some user-defined options
      // these would be meaningful to this particular program
      boolean bv = Tut.getOptionBool("ss.booleanValue1");
      System.out.println("ss.booleanValue1 = " + bv);

      double dv = Tut.getOptionDouble("ss.doubleValue1");
      System.out.println("ss.doubleValue1 = " + dv);

      int iv = Tut.getOptionInt("ss.intValue1");
      System.out.println("ss.intValue1 = " + iv);

      String sv = Tut.getOptionStr("ss.stringValue1");
      System.out.println("ss.stringValue1 = " + sv);

      TipcOption so = Tut.getOption("ss.stringValue2");
      Vector ov = so.getValueList();
      if (null == ov) {
        System.out.println("Can't parse stringValue2!\n");
      }
      else {
        // use an Enumeration object to move through parsed list
        Enumeration en = ov.elements();
        for (int i=0; en.hasMoreElements(); i++) {
          String el = (String)en.nextElement();
          System.out.println("ss.stringValue2[" + i + "] = " + el);
        } // for
      } // else

      TipcOption sw = Tut.getOption("ss.bulb");
      System.out.println("ss.bulb(string) = " + sw.getValueEnum());
      int ev = sw.getValueEnumMap();
      System.out.println("ss.bulb(mapped) = " + ev);
    }
    catch (TipcException te) {
      Tut.warning(te);
    } // catch
  } // main
} // getOptions
