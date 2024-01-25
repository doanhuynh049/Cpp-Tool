/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/options/toptions.java#1 $
 */

import java.io.*;
import java.util.*;
import com.smartsockets.*;

/**
 * Loads a set of options from a local property file and displays
 * the results.
 */
public class toptions {

  static public void main(String[] argv) {

    /*
     * First, we create an option called "spam". The first parameter
         * to create is the name of the option, and the second parameter is
         * the initial value of the option.
         */
        TipcOption opt = null;
        try {
            opt = Tut.createOption("spam", "one");
        }
        catch (TipcException e) {
            Tut.exitFailure("option spam already exists");
        }

        /*
         * We want this option to the an enumerated map option, so
         * we set up the map here.  In this case, the name "one" is
         * bound to the value '1', "two" to '2', etc.
     */
    opt.addEnumMapLegalValue("one", 1);
    opt.addEnumMapLegalValue("two", 2);
    opt.addEnumMapLegalValue("three", 3);
    opt.addEnumMapLegalValue("four", 4);

    /*
     * Now we load the property file that holds our options.
     * We can specify a complete pathname here (using '/' as the separator,
     * regardless of the underlying platform), or just a filename, in which
     * case the current directory is assumed.
     *
     * If we wanted to load options from a URL instead, we would only have
     * to change this one line to something like:
     *
     *   Tut.loadOptionsURL("http://www.foo.com/remote.properties");
     *
     * which would fetch the file "remote.properties" from the web server
     * running on "www.foo.com".
     */
    Tut.loadOptionsFile("local.properties");

    try {
      /*
       * The first option is a string option named "foo".
       */
      String s = Tut.getOptionStr("foo");
      System.out.println("Option foo = " + s);

      /*
       * Since the properties file is just a text file, all options are
       * actually stored as strings.  We provide a set of convenience
       * methods to convert these strings to underlying types, such as
       * boolean, int, double, etc.  Nothing magic here, just using the
       * built-in parsing methods provided by the standard Java library:
       *
       *  String s = Tut.getOptionStr("bar");
       *  Boolean b = Boolean.valueOf(s);
       *  return b.booleanValue();
       */
      boolean b = Tut.getOptionBool("bar");
      System.out.println("Option bar = " + b);

      /*
       * Here, we grab an option who's value is a name tied to an int
       * by the setOptionEnumLegalValue method used above.
       */
            opt = Tut.getOption("spam");
            int i = opt.getValueEnumMap();
      System.out.println("Option spam = " + i);

      /*
       * Finally, you can access a comma-separated list of string values,
       * returned as a Vector of objects, which can in turn be accessed
       * with the standard Java library's Enumeration class.  One you have
       * a list of string options, you can, of course, parse them as any
       * primitive type using the Java library's parsing methods.
       */
            opt = Tut.getOption("list");
      Vector v = opt.getValueList();
      Enumeration en = v.elements();
      for (i=0; en.hasMoreElements(); ++i) {
                s = (String)en.nextElement();
                System.out.println("list[" + i + "] = " + s);
      }

    }
    catch (TipcException e) {
      System.out.println("Did not find value for option. " + e.getMessage());
    }
  }
}
