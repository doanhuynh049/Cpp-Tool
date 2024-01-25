/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/applet/tapplet.java#1 $
 */

import java.awt.*;
import java.awt.event.*;
import java.applet.*;
import java.io.*;
import java.net.*;
import com.smartsockets.*;

/*
 * This example applet creates a simple GUI with three text fields.
 *
 * The first field contains a subject this applet is subscribed and
 * sending to.  You must fill in this field and hit <return> before
 * using the second field.
 *
 * The second field allows the user to enter a string to send as an
 * INFO message to that subject.  Hitting <return> in this field will
 * send and clear the contents of the field.
 *
 * The third displays any strings received as INFO messages on that
 * subject.
 *
 * The applet uses a process callback to process received INFO messages.
 * To support the infinite mainLoop(), it creates a background thread.
 *
 * Compile this file normally, and copy the class file to the appropriate
 * directory for your web server.  Also copy the file 'tapplet.properties'
 * after editing it to set the "ss.server_names" option within it.  This
 * should be set to the hostname of the machine on which RTserver will be
 * running.
 *
 * For appletviewer or HotJava, the "ss.server_names" option can be any
 * machine.  The default value "_node", can also be used, which represents
 * the local host. For Netscape, IE, or any other high-security browser,
 * this must be the same machine as the web browser.
 *
 * You will need to make sure that the browser or appletviewer can access
 * the SmartSockets jar file 'ss.jar'. This is typically done by setting
 * the classpath for the browser or appletviewer, or by setting the
 * CLASSPATH environment variable. For example, to run Sun's appletviewer
 * bundled with JDK 1.2 (note: JDK 1.3 does not support the -nosecurity
 * argument so the java.policy file will usually need to be modified):
 *
 *   appletviewer -J-classpath -J../../lib/ss.jar -nosecurity tapplet.html
 *
 * Alternatively, the ARCHIVE attribute of the APPLET tag in 'tapplet.html'
 * may be used to specify the location of the SmartSockets jar file. For
 * browsers, the SmartSockets jar file should reside on the web server.
 * For appletviewers, the SmartSockets jar file will reside on the local
 * file system.
 */

public class tapplet extends Applet implements TipcProcessCb, Runnable {
  TextField subject;
  TextField sent;
  TextField received;
  TipcSrv srv;
  volatile Thread processor;
  boolean subscribed;

  public void init() {
    /*
     * create the GUI.
     */
    subject = new TextField(16);
    sent = new TextField(32);
    received = new TextField(32);
    this.setLayout(new GridLayout(3,2));
    this.add(new Label("subject: "));
    this.add(subject);
    this.add(new Label("send: "));
    this.add(sent);
    this.add(new Label("received: "));
    this.add(received);

    /*
     * This is the location where we'll be downloading our properties.
     */
    URL codeBase = this.getCodeBase();

    /*
     * The server_names property in the properties file should be set to
     * the hostname of the web server this applet will be loaded from,
     * which must also be where RTserver is running.  If you are using
     * HotJava or appletviewer this can be the localhost.  Netscape,
     * however, will insist they are on some other host.
     */
    Tut.loadOptionsURL(codeBase + "tapplet.properties");

    processor = null;
    subscribed = false;  // we haven't subscribed to anything yet
  }

  /*
   * Called when an action fires on one of our text fields.  We use
   * this deprecated API because 1) it's simpler than the new one, 2)
   * Netscape doesn't appear to like ActionListeners.
   */
  /** @deprecated */
  public boolean action(Event ev, Object arg) {

    TextField target = (TextField)ev.target;

    /*
     * target is the GUI object that fired the 'action'.  'sent' is
     * our outbound message text area.  From here, we're sending the
     * contents of this text area as an INFO message to whatever
     * subject is in the 'subject' text area.
     */
    if (target == sent) {
      /*
       * If we haven't subscribed to anything yet, we can't send anything.
       */
      if (!subscribed) {
        this.showStatus("aren't subscribed to anything yet.");
        return true;
      }

      TipcMsg msg = TipcSvc.createMsg(TipcMt.INFO);
      msg.setDest(subject.getText());
      msg.appendStr(sent.getText());
      try {
        srv.send(msg);
        /*
         * Note: it is important to flush here, rather than rely on
         * the auto-flush that srv.next() will do, since srv.next()
         * in this case is running in a different thread, which won't
         * auto-flush until after it processes a message.  It
         * can't process a message, however, until it gets one to
         * process.  Catch-22.
         */
        srv.flush();
      }
      catch (TipcException e) {
        sent.setText("error sending " + e.getMessage());
        return true;
      }
      /*
       * clear the field to indicate it's been sent.
       */
      sent.setText("");
      sent.setCaretPosition(0);
      return true;
    }
    else if (target == subject) {
      /*
       * the subject has been changed, so subscribe to the new subject.
       * Note that we're not unsubscribing from the old subject, so the
       * new one is added on.
       */
      try {
        srv.setSubjectSubscribe(subject.getText(), true);
        this.showStatus("subscribed to " + subject.getText());
        subscribed = true;
      }
      catch (TipcException e) {
        return true;
      }
      return true;
    }
    return false;
  }

  /*
   * Called when an applet should begin (or resume) execution.
   */
  public void start() {
    /*
     * We get the TipcSrv instance here since we're going to destroy
     * it in stop.
     */
    srv = TipcSvc.getSrv();

    /*
     * add our process callback, watching for the INFO message type we
     * intend to bounce off of RTserver.
     */
    srv.addProcessCb(this, TipcSvc.lookupMt(TipcMt.INFO), null);
    /*
     * fire up our message processing thread
     */
    processor = new Thread(this);
    processor.start();
    this.showStatus("process thread started.");
  }

  /*
   * Called when an applet should stop (or pause) execution
   */
  public void stop() {
    /*
     * Destroy the server connection, so we don't have one lingering if
     * the browser simply leaves the page we're on.
     */
    /*processor.stop();*/
    /* use interrupt() to replace deprecated stop() method */
    processor.interrupt();
    processor = null;
    try {
      srv.destroy();
      srv = null;
    }
    catch (TipcException e) {
    }
    this.showStatus("process thread stopped.");
  }

  /*
   * The body of the processor thread
   */
  public void run() {
    try {
      srv.mainLoop(TipcDefs.TIMEOUT_FOREVER);
    }
    catch (TipcException e) {
    }
  }

  /*
   * This is called when the applet needs updating, like when it first
   * comes up.  We do nothing here, just let the text areas handle their
   * own repainting.
   */
  public void paint(Graphics g) {
  }

  /*
   * Called when a applet should destroy itself.  We shut down the
   * server connection.  Theoretically, this should be handled by
   * stop(), but we're just making sure.
   */
  public void destroy() {
    if (processor != null) {
      /*processor.stop();*/
      /* use interrupt to replace deprecated stop() method */
      processor.interrupt();
      processor = null;
    }
    if (srv != null) {
      try {
        srv.destroy();
      }
      catch (TipcException e) {
      }
    }
  }

  /*
   * The process callback.  This extracts the text from the INFO message,
   * and places it in to 'received' text area.
   */
  public void process(TipcMsg msg, Object arg) {
    try {
      msg.setCurrent(0);
      String s = msg.nextStr();
      received.setText(s);
    }
    catch (TipcException e) {
    }
  }
}
