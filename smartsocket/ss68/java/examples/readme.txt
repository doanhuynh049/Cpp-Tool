/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/readme.txt#1 $
 */

SmartSockets Java Class Library Examples
========================================

The following directories contain several example programs that
have been build with the TIBCO Software Inc. SmartSockets Java Class Library.
Don't forget to examine the tutorial directory also, it contains
various useful source code to accompany the printed documentation,
including programs which illustrate some techniques not represented
by the examples here.

applet
------
  A simple applet example, it echoes text entered into one text
  box back into another text box, using SmartSockets. It uses the
  old-style AWT event model, and will therefore work even on less
  current browsers.

chat
----
  A slightly more complex applet, providing a multiple-user 'chat
  room'. Uses the 1.1 AWT event model.

cloud
-----
  This applet graphically displays the current configuration of the
  RTserver/RTclient connection cloud, with dynamic updates. Can be
  used as a very rudimentary monitoring tool. This example illustrates
  use of the TipcMon monitoring API.

options
-------
  A simple applet that demonstrates some of the methods used for
  manipulating SmartSockets Java Properties ("options databases".)

servlet
-------
  An interesting little servlet which provides text-based feedback
  from the monitoring API to requesting web clients. The Servlet
  Development Kit (SDK) is required to use this example; it can be
  downloaded for free from Sun's website.

threads
-------
  This example illustrates the use of multiple threads to trigger 
  message processing from a single connection. The concept of 
  callback and thread autonomy is clearly demonstrated.

whiteboard
----------
  An application which provides a concurrent, collaborative
  virtual 'whiteboard', including features such as graphics
  and text to be displayed as well as freehand drawing. Shows
  integration of SmartSockets messaging techniques into a slightly
  more significant application. Requires the Swing GUI classes.

