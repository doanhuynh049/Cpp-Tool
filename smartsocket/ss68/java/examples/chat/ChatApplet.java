/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/chat/ChatApplet.java#1 $
 */

import java.util.Vector;
import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;
import com.smartsockets.*;

/*
 *  SmartSockets Java demo: ChatApplet
 *
 *  Simple example demonstrating correct applet handling
 *  with SmartSockets, as well as a somewhat useful application.
 *  Note that the message protocol used is not ideal (only one
 *  message type with variable semantics for the fields) but the
 *  primary goal is to demonstrate the minimum an applet is required
 *  to contain, if useful and well-behaved.
 *
 *  Remember, there are two ways for a browser or appletviewer to
 *  access the SmartSockets jar file 'ss.jar'. Either include the
 *  SmartSockets jar file in the browser or appletviewer classpath
 *  (or setting the CLASSPATH environment variable) or by relying on the
 *  ARCHIVE attribute of the APPLET tag in the html file that
 *  drives this applet (the jar file must be present in the same directory,
 *  even under appletviewer).
 *
 *  An example of running this applet with Sun's appletviewer bundled with
 *  JDK 1.2:
 *
 *    appletviewer -J-classpath -J../../lib/ss.jar -nosecurity ChatApplet.html
 *
 *  JDK 1.3 and above do not support the -nosecurity argument so you will
 *  want to use Java's Extension Mechanism instead by copying ss.jar to:
 *
 *    lib/ext      [in the Java 2 Runtime Environment]
 *    jre/lib/ext  [in the Java 2 SDK]
 *
 *  and running the appletviewer as:
 *
 *    appletviewer ChatApplet.html
 *
 *
 *  07/16/1998 BBM added 'leaving chat' functionality
 *  07/15/1998 BBM initial code
 *
 */

public class ChatApplet extends Applet
implements TipcProcessCb, Runnable, ActionListener {
  static final String info = "Multiuser realtime chat applet demo.";
  static final int JAVA_CHAT_MT = 8081;
  static final String chat_subject = "/java_chat";

  TipcSrv srv;
  Thread reader = null;

  CardLayout cl = new CardLayout();
  TextArea out;
  TextField in, name;
  Button go_btn;
  Panel c1, c2;

  TipcMt chat_mt;   // message type
  TipcMsg chat_msg; // message (will always be reused)
  TipcCb the_cb;

  Font my_font = new Font("Serif", Font.PLAIN, 11);
  String my_name;

  public void run() {
    Thread me = Thread.currentThread();
    me.setPriority(Thread.MIN_PRIORITY);
    while (reader == me) {
      try {
        TipcSrv srv = TipcSvc.getSrv();
        srv.mainLoop(1.0);
      }
      catch (TipcException te) {
      }
      try {
        Thread.sleep(100);
      }
      catch (InterruptedException ie) {
        break;
      } // catch
    } // reader
  } // run

  public void init() {
    String host = getDocumentBase().getHost();
    try {
      if (!host.equals("")) {
        Tut.setOption("ss.server_names", host);
      } // if
    }
    catch (TipcException e) {
    }

    try {
      srv = TipcSvc.getSrv();
      srv.create();
    }
    catch (TipcException e) {
    }

    try {
      chat_mt = TipcSvc.createMt("java_chat", JAVA_CHAT_MT, "str str");
    }
    catch (TipcException e) {
      // after a reload, the create will throw an exception. since
      // we want a handle to the mt, must look it up...
      chat_mt = TipcSvc.lookupMt(JAVA_CHAT_MT);
    }

    chat_msg = TipcSvc.createMsg(chat_mt);
    chat_msg.setDest(chat_subject);

    try {
      srv.setSubjectSubscribe(chat_subject, true);
    }
    catch (TipcException e) {
    }

    the_cb = srv.addProcessCb(this, chat_mt, null);

    setupGUI();
  } // init

  public void destroy() {
    try {
      srv.removeProcessCb(the_cb);
      chat_msg.setUserProp(-1);
      announce(my_name, chat_subject);
    }
    catch (TipcException e) {
    }
  } // destroy

  void setupGUI() {
    setLayout(cl);
    c1 = new Panel();
    c1.add(new Label("Enter your name: "));
    name = new TextField("", 16);
    c1.add(name);
    go_btn = new Button("Go");
    go_btn.addActionListener(this);
    c1.add(go_btn);
    add("name", c1);

    c2 = new Panel(new BorderLayout());
    out = new TextArea("", 10, 60,
                       out.SCROLLBARS_VERTICAL_ONLY);
    out.setFont(my_font);
    out.setEditable(false);
    c2.add(new Label("Message Window"), "North");
    c2.add(out, "Center");
    Panel p = new Panel();
    p.add(new Label("Text to send:"));
    in = new TextField("", 60);
    in.setFont(my_font);
    p.add(in);
    Button do_chat = new Button("Send");
    do_chat.addActionListener(this);
    p.add(do_chat);
    c2.add(p, "South");
    add("chat", c2);

    cl.show(this, "name");
    name.requestFocus();
  } // setupGUI

  public void actionPerformed(ActionEvent ae) {
    if (ae.getActionCommand().equals("Go") &&
        name.getText().length()>0) {
      my_name = name.getText();
      cl.show(this, "chat");
      in.requestFocus();
      // request replies to our announcement with user prop=1
      chat_msg.setUserProp(1);
      announce(my_name, chat_subject);
    } // if it's the go button
    if (ae.getActionCommand().equals("Send") &&
        in.getText().length()>0) {
      try {
        chat_msg.setNumFields(0);
        chat_msg.appendStr(my_name);
        chat_msg.appendStr(in.getText());
        srv.send(chat_msg);
        srv.flush();
      }
      catch (TipcException te) {
        Tut.warning(te);
      } // catch
      in.setText("");
      in.requestFocus();
    } // if it's the send button
  } // actionPerformed

  public String getAppletInfo() {
    return info;
  } // getAppletInfo

  public void stop() {
    reader = null;
  } // stop

  public void start() {
    reader = new Thread(this);
    reader.start();
    repaint();
  } // start

  public static void main(String args[]) {
    ChatApplet chat = new ChatApplet();
    chat.init();
  } // main

  void announce(String name, String dest) {
    // let the new person know we're here or announce ourselves
    try {
      chat_msg.setDest(dest);
      chat_msg.setNumFields(0);
      chat_msg.appendStr(my_name);
      chat_msg.appendStr("");
      srv.send(chat_msg);
      srv.flush();
      chat_msg.setDest(chat_subject);
    }
    catch (TipcException e) {
      Tut.warning(e);
    } // catch
  } // announce

  public void process(TipcMsg msg, Object o) {
    try {
      msg.setCurrent(0);
      String who = msg.nextStr();
      String text = msg.nextStr();

      // see if it's someone new to us
      if (text.equals("")) {
        if (msg.getUserProp() == -1) {
          out.append("+ Leaving chat: " + who + "\n");
        }
        else {
          out.append("- Joining chat: " + who + "\n");
        }
        if (!who.equals(my_name) && msg.getUserProp() == 1) {
          chat_msg.setUserProp(0);
          announce(who, msg.getSender());
        } // if "wants replies" flag is set and not our message
      } // if an announcement
      else {
        // a 'regular' message; display it
        out.append("[" + who + "] " + text + "\n");
      } // if
    }
    catch (TipcException e) {
      Tut.warning(e);
    } // catch
  } // process

} // main class

