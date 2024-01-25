/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/tutorial/lesson5/getOptionsApplet.java#1 $
 */

// getOptionsApplet.java
// Example of retrieving SmartSockets and user-defined options
// settings from an URL on the same webserver as this applet

//
// NOTE: see 'readme.txt' in this directory for info about the
// 'remote.opt' file referenced in this applet. Or, simply copy
// (or create a symbolic link if your OS supports them) from
// 'remote.opt' to the provided 'local.opt'.
//

import java.io.*;
import java.util.*;
import java.awt.*;
import java.applet.*;
import com.smartsockets.*;

public class getOptionsApplet
  extends Applet
  implements Runnable {

  TextArea out;

  public void init() {
    out = new TextArea("", 14, 65, out.SCROLLBARS_VERTICAL_ONLY);
    this.add(new Label("Applet Options Example", Label.CENTER));
    this.add(out);
  } // init

  public void run() {
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

    String optionsURL = this.getCodeBase() + "remote.opt";
    Tut.loadOptionsURL(optionsURL);

    // display the URL we're using for options
    this.add(new Label(optionsURL, Label.CENTER));
    try {
      // get some standard options
      String project = Tut.getOptionStr("ss.project");
      out.append("ss.project = " + project + "\n");

      String server_names = Tut.getOptionStr("ss.server_names");
      out.append("ss.server_names = " + server_names + "\n");

      String user_name = Tut.getOptionStr("ss.user_name");
      out.append("ss.user_name = " + user_name + "\n");

      // get some user-defined options
      // these would be meaningful to this particular program
      boolean bv = Tut.getOptionBool("ss.booleanValue1");
      out.append("ss.booleanValue1 = " + bv + "\n");

      double dv = Tut.getOptionDouble("ss.doubleValue1");
      out.append("ss.doubleValue1 = " + dv + "\n");

      int iv = Tut.getOptionInt("ss.intValue1");
      out.append("ss.intValue1 = " + iv + "\n");

      String sv = Tut.getOptionStr("ss.stringValue1");
      out.append("ss.stringValue1 = " + sv + "\n");

      TipcOption so = Tut.getOption("ss.stringValue2");
      Vector ov = so.getValueList();
      if (null == ov) {
        out.append("Can't parse stringValue2!\n");
      }
      else {
        // use an Enumeration object to move through parsed list
        Enumeration en = ov.elements();
        for (int i=0; en.hasMoreElements(); i++) {
          String el = (String)en.nextElement();
          out.append("ss.stringValue2[" + i + "] = " + el + "\n");
        } // for
      } // else

      TipcOption sw = Tut.getOption("ss.bulb");
      out.append("ss.bulb(string) = " + sw.getValueEnum() + "\n");
      int ev = sw.getValueEnumMap();
      out.append("ss.bulb(mapped) = " + ev + "\n");
    }
    catch (TipcException te) {
      Tut.warning(te);
    } // catch
  } // run

  public void start() {
    Thread t = new Thread(this);
    t.start();
  } // start

} // getOptionsApplet
