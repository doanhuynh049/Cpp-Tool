/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/servlet/readme.txt#1 $
 */

SmartSockets Servlet Example
============================

To compile and run this example you will need to have the Java
Servlet Development Kit (JSDK) installed, version 2.0 or later.
(The JSDK is included with JDK 1.2 and above.) To get the JSDK
for use with JDK 1.1, visit Sun's website, specifically:

  http://java.sun.com/products/java-server/servlets/

Once the JSDK classes are available (those in the javax.servlet
package), compile the example as usual. To execute the servlet,
use the servletrunner utility from the JSDK. For example:

  servletrunner -d '/ss50/java/demos/servlet'

(Replace the path in quotes with the current working directory,
appropriate to your installation.) Upon running servletrunner,
you should see something like the following:

servletrunner starting with settings:
  port = 8080
  backlog = 50
  max handlers = 100
  timeout = 5000
  servlet dir = /ss50/java/demos/servlet
  document dir = ./examples
  servlet propfile = /ss50/java/demos/servlet/servlet.properties

The important thing to notice is the setting for 'port', in this
case 8080. To run the servlet, point your browser at the URL:

  http://localhost:port/servlet/Mon

(replace 'port' with 8080 in this case; 'localhost' can also be
replaced with the appropriate machine name, or IP address).

You can also configure servletrunner via the servlet.properties
file and command line. See the JSDK documentation for more
information.

