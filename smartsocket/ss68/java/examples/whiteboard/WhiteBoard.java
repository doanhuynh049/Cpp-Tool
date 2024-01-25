/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/whiteboard/WhiteBoard.java#1 $
 */

import java.io.*;
import java.applet.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import javax.swing.*;
import javax.swing.event.*;
import com.smartsockets.*;

/*
 * This example application implements a shared 'whiteboard' using
 * TIBCO Software Inc.'s SmartSockets technology.
 *
 * NOTE: This application was tested with JDK 1.1.6 and Swing 1.0.2,
 *       Swting 1.0.3.
 *
 * The text field after "Whiteboard:" specifies the subject to which
 * this whiteboard will publish and receive 'strokes'. (The string
 * entered is actually prepended with "/whiteboard/"). This allows for
 * multiple whiteboard "groups" to coexist and not interfere with one
 * another. Note that the group is not actually changed until 'ENTER'
 * is hit in this text entry field.
 *
 * Right-clicking (Windows) or double-clicking (Win/Solaris) on the
 * board area produces a popup menu with the following options:
 * Select drawing color, add annotation (places entered text at the
 * current mouse position), place image (allows selection of a .gif
 * or .jpg and places it at the current mouse position--remote board
 * users do NOT need a local copy of the file), republish all strokes
 * (to update new boards, etc.), clear this board, and clear all boards.
 * The drawing color can also be selected by clicking the 'Drawing color'
 * button on the lower part of the window.
 *
 * The window is fully resizeable, and strokes hidden by decreasing
 * the window size are not lost.
 *
 * The abstract class 'wbStroke' is extended by wbLine, wbText and
 * wbImage to provide storage for the stroke data; a corresponding
 * SmartSockets message type exists for each as well. All relevant
 * message types are received by the process() callback and decoded.
 * NOTE that this is fairly-memory inefficient object storage, and
 * probably would not be practical for prolonged whiteboarding sessions!
 *
 * The SetupSS() method configures the application's SmartSockets
 * interaction, and a background thread (see run()) performs an
 * infinite mainLoop() to receive messages and fire process().
 * About 15% of this application's code is SmartSockets-related; most
 * is GUI and other standard Java stuff.
 */

/*
 *   support classes
 */
abstract class wbStroke {
  // abstract class we derive strokes from
  protected Point origin;
  protected Color color;
  protected String username;

  // for displaying onscreen
  public abstract void render(Graphics g);

  public void setUser(String u) {
    username = u;
  } // setUser

  public void setColor(int col) {   // combined (24bit value) RGB
    color = new Color(col);
  } // setColor

  // for publishing over SmartSockets
  public abstract void publish(String subject);
}

class wbLine extends wbStroke {
  Point end;

  public wbLine(Point p1, Point p2) {
    origin = new Point(p1);
    end = new Point(p2);
  } // constructor

  public void render(Graphics g) {
    g.setColor(color);
    g.drawLine(origin.x, origin.y, end.x, end.y);
  } // render

  public void publish(String subject) {
    TipcMsg msg = TipcSvc.createMsg(TipcSvc.lookupMt("wb_line"));
    msg.setDest(subject);
    msg.appendStr(username);
    msg.appendInt4(color.getRGB());
    msg.appendInt4(origin.x);
    msg.appendInt4(origin.y);
    msg.appendInt4(end.x);
    msg.appendInt4(end.y);
    try {
      TipcSvc.getSrv().send(msg);
    } catch (TipcException te) {
      System.err.println("Can't publish a text event message!");
    }
  } // publish
} // wbLine

class wbText extends wbStroke {
  String text;

  public wbText(Point p1, String txt) {
    origin = p1;
    text = txt;
  } // constructor

  public void render(Graphics g) {
    g.setColor(color);
    g.drawString(text, origin.x, origin.y);
  } // render

  public void publish(String subject) {
    TipcMsg msg = TipcSvc.createMsg(TipcSvc.lookupMt("wb_text"));
    msg.setDest(subject);
    msg.appendStr(username);
    msg.appendInt4(color.getRGB());
    msg.appendInt4(origin.x);
    msg.appendInt4(origin.y);
    msg.appendStr(text);
    try {
      TipcSvc.getSrv().send(msg);
    } catch (TipcException te) {
      System.err.println("Can't publish a text event message!");
    }
  } // publish
} // wbText

class wbImage extends wbStroke {
  Image image;
  byte[] imdata;        // raw data (gfx file's complete contents)
  ImageObserver io;

  public wbImage(Point p1, byte[] imraw, ImageObserver o) {
    origin = p1;
    imdata = imraw;
    image = Toolkit.getDefaultToolkit().createImage(imdata);
    io = o;
  } // constructor

  public void render(Graphics g) {
    g.drawImage(image, origin.x, origin.y, io);
  } // render

  public void publish(String subject) {
    TipcMsg msg = TipcSvc.createMsg(TipcSvc.lookupMt("wb_image"));
    msg.setDest(subject);
    msg.appendStr(username);
    msg.appendInt4(0);
    msg.appendInt4(origin.x);
    msg.appendInt4(origin.y);
    msg.appendBinary(imdata);
    try {
      TipcSvc.getSrv().send(msg);
    } catch (TipcException te) {
      System.err.println("Can't publish image!");
    }
  } // publish
} // wbImage

/*
 *   main class
 */
public class WhiteBoard
  extends JFrame
  implements Runnable,
         TipcProcessCb,
         ActionListener,
         ComponentListener,
         WindowListener,
         MouseMotionListener,
         MouseListener {

  // message type IDs
  static final int WB_LINE = 12021;
  static final int WB_TEXT = WB_LINE + 1;
  static final int WB_IMAGE = WB_TEXT + 1;
  static final int WB_CLEAR = WB_IMAGE + 1;

  static final String programName = "SmartSockets WhiteBoard Demo";
  static final String baseSubject = "/whiteboard/";
  JTextField subj = new JTextField("general", 8);
  String oldSubj = baseSubject + subj.getText();

  String username = System.getProperty("user.name","boarduser");
  // pick a random drawing color
  Color myColor = new Color((float)Math.random(),
                (float)Math.random(), (float)Math.random());

  TipcMt mtLine, mtText, mtImage, mtClear;
  JButton btnColor;
  Point Plast;
  JPopupMenu pop;
  wBoard board;
  JPanel tools;

  // current x, y mouse pos
  int curx,cury;

  Thread whiteBoard = null;
  TipcSrv srv = null;

  // storage for the stroke objects
  Vector wbObjects;

  public WhiteBoard() {
    wbObjects = new Vector();
    init();
    start();
  } // constructor

  public void run() {
    Thread thisThread = Thread.currentThread();
    while (whiteBoard == thisThread) {
      // read and process all incoming messages
      try {
    while (true) {
      srv.mainLoop(1.0);    // catch up on waiting messages
      try {
        whiteBoard.sleep(100);  // rest for a bit
        // (can increase sleep duration if appears
        // to be starving out other threads)
      } catch (InterruptedException ie) {
      }
    } // while
      }
      catch (TipcException e) { }
    } // while
  } // run


  public void destroy() {
    // disconnect from RTserver
    try {
      srv.destroy();
    } catch (TipcException e) { }
  } // destroy


  public void stop() {
    whiteBoard = null;
  } // stop


  public void start() {
    whiteBoard = new Thread(this);
    whiteBoard.start();
  } // start


  public void process(TipcMsg msg, Object arg) {
    // read the data part of the message
    try {
      Point p1,p2;
      String txt;

      msg.setCurrent(0);
      String user = msg.nextStr();
      int color = msg.nextInt4();

      wbStroke news = null;

      switch (msg.getType().getNum()) {
      case WB_LINE:
    p1 = new Point(msg.nextInt4(), msg.nextInt4());
    p2 = new Point(msg.nextInt4(), msg.nextInt4());
    news = new wbLine(p1, p2);
    break;
      case WB_TEXT:
    p1 = new Point(msg.nextInt4(), msg.nextInt4());
    txt = new String(msg.nextStr());
    news = new wbText(p1, txt);
    break;
      case WB_IMAGE:
    p1 = new Point(msg.nextInt4(), msg.nextInt4());
    news = new wbImage(p1, msg.nextBinary(), this);
    break;
      case WB_CLEAR:
    doClear();
    break;
      default:
    System.out.println("Unknown stroke, type = "+msg.getType().getName());
    break;
      }

      if (news != null) {
    news.setUser(user);
    news.setColor(color);
    Graphics g = board.getGraphics();
    if (g!=null) {
      news.render(g);
    }
    wbObjects.addElement(news);
      }

    }
    catch (TipcException e) {
      System.out.println("Malformed message: "+e.getMessage());
    }
  } // process

  private void setupGUI() {
    JPanel bbm = new JPanel();
    bbm.setLayout(new BorderLayout());
    setTitle(programName);

    // top panel
    JPanel top = new JPanel();
    JLabel wmd = new JLabel("right mousebutton or double-click on board for options");
    wmd.setFont(new Font("SansSerif", Font.BOLD, 12));
    top.add(wmd);
    bbm.add("North",top);

    // whiteboard area
    board = new wBoard();
    board.setSize(100,100);
    bbm.add("Center",board);
    board.addMouseMotionListener(this);
    board.addMouseListener(this);

    tools = new JPanel();
    // color picker button
    btnColor = new JButton("Drawing color");
    btnColor.setBackground(myColor);
    btnColor.setToolTipText("select your drawing color");
    btnColor.setVerticalTextPosition(AbstractButton.CENTER);
    btnColor.setActionCommand("color");
    btnColor.addActionListener(this);
    tools.add(btnColor);
    // subject box
    tools.add(new JLabel("Whiteboard:"));
    subj.addActionListener(this);
    tools.add(subj);
    bbm.add("South",tools);

    // add big panel
    this.getContentPane().add(bbm);

    // create popup menu
    String[] items = new String[] { "Select drawing color",
                    "Add annotation", "Place image",
                    "Republish all strokes",
                    "Clear this board", "Clear all boards" };
    String[] cmds = new String[] { "color", "text", "image",
                   "republish", "clear", "clearall" };
    pop = new JPopupMenu("Whiteboard");
    for (int i=0;i<items.length;i++) {
      if (i==3 || i==4)
    pop.addSeparator();
      JMenuItem jmi = new JMenuItem(items[i]);
      jmi.setActionCommand(cmds[i]);
      jmi.addActionListener(this);
      pop.add(jmi);
    }

    pack();
    this.setSize(400,380); // default size
    this.show();

    this.addComponentListener(this);
    this.addWindowListener(this);
  } // setupGUI

  public void componentResized(ComponentEvent ce) {
    Rectangle b = ce.getComponent().getBounds();
    board.setSize(b.width - 40,
          tools.getLocation().y - board.getLocation().y - 5);
    board.setLocation(15,board.getLocation().y);
    board.repaint();
  } // resize

  public void windowClosing(WindowEvent we) {
    // catch window close and exit
    destroy();
    System.exit(0);
  } //

  public void mouseClicked(MouseEvent e) {
    // double-click is acceptable to product popup menu
    if (e.getClickCount()==2) {
      pop.show(board, e.getX(), e.getY());
    }
  } // mouseClicked

  public void mousePressed(MouseEvent e) {
  }

  public void mouseEntered(MouseEvent e) {
  }

  public void mouseExited(MouseEvent e) {
  }

  public void mouseReleased(MouseEvent e) {
    // was this the RMB? do popup
    if (e.isPopupTrigger()) {
      pop.show(board, e.getX(), e.getY());
    }
  } // mouseReleased

  public void mouseDragged(MouseEvent e) {
    // publish the dragged line
    Point Pnew = new Point(e.getX(), e.getY());
    if (Plast != null) {
      wbLine nl = new wbLine(Plast, Pnew);
      nl.setColor(myColor.getRGB());
      nl.setUser(username);
      nl.publish(oldSubj);
      try {
    srv.flush();
      } catch (TipcException te) {
    System.err.println("Can't publish a line!");
      }
    }
    Plast = Pnew;
  } // mouseDragged

  public void mouseMoved(MouseEvent e) {
    // track the mouse
    curx = e.getX();
    cury = e.getY();
    Plast = new Point(curx, cury);
  } // mouseMoved

  void doClear() {
    wbObjects = new Vector();
    repaint();
  } // doClear

  public void actionPerformed(ActionEvent e) {
    String cmd = e.getActionCommand();
    Object src = e.getSource();

    // color picker
    if (cmd == "color") {
      // show color picker
      myColor = JColorChooser.showDialog(this,
                     "Select drawing color",
                     getBackground());
      btnColor.setBackground(myColor);
      btnColor.repaint();
      return;
    }

    // annotation
    if (cmd == "text") {
      Point where = new Point(curx, cury);
      String text = JOptionPane.showInputDialog("Enter annotation");
      if (text == null)
    return;
      if (!text.equals("")) {
    wbText nt = new wbText(where, text);
    nt.setColor(myColor.getRGB());
    nt.setUser(username);
    nt.publish(oldSubj);
    try {
      srv.flush();
    } catch (TipcException te) {
      System.err.println("Can't publish annotation!");
    }
      }
      return;
    }

    // add image
    if (cmd == "image") {
      // use file picker to grab a gif...
      Point where = new Point(curx, cury);

      JFileChooser chooser = new JFileChooser();
      int retval = chooser.showOpenDialog(this);
      if (retval == JFileChooser.APPROVE_OPTION) {
    File theFile = chooser.getSelectedFile();
    if (theFile != null) {
      // load gif/jpg...
      byte[] fileContents = new byte[(int)theFile.length()];
      try {
        FileInputStream fis = new FileInputStream(theFile);
        try {
          fis.read(fileContents);
          wbImage nw = new wbImage(where, fileContents, this);
          nw.setUser(username);
          nw.publish(oldSubj);
          srv.flush();
        }   catch (IOException ie) {
          System.err.println("Error reading image!");
        } catch (TipcException te) {
              System.err.println("Error publishing image!");
            }
      } catch (FileNotFoundException fnf) {
        System.err.println("File not found!");
      }
    }
      }
      return;
    }

    // republish all strokes
    if (cmd == "republish") {
      Vector old = (Vector)wbObjects.clone();
      doClear();

      for (int i=0;i<old.size();i++) {
    wbStroke str = (wbStroke)old.elementAt(i);
    str.publish(oldSubj);
      }
      try {
    srv.flush();
      } catch (TipcException te) {
    System.err.println("Can't republish!");
      }

      return;
    }

    // clear (local)
    if (cmd == "clear") {
      doClear();
      return;
    }

    // clear (all)
    if (cmd == "clearall") {
      TipcMsg cm = TipcSvc.createMsg(mtClear);
      try {
    cm.setDest(oldSubj);
    cm.appendStr(username);
    cm.appendInt4(0);
    srv.send(cm);
    srv.flush();
      } catch (TipcException ioe1) {
    System.err.println("Can't publish clear all request!");
      }
      return;
    }

    // change subscribed subject
    if ((JTextField)src == subj) {
      try {
    srv.setSubjectSubscribe(oldSubj, false);
    srv.setSubjectSubscribe(baseSubject + subj.getText(), true);
    oldSubj = baseSubject + subj.getText();
    System.out.println("Changed board group to " + baseSubject +
               subj.getText());
      } catch (TipcException i1) {
    System.err.println("Can't change board group to " + baseSubject +
               subj.getText());
      }
      return;
    }
  } // actionPerformed

  /* a bunch of methods we're obligated to implement but don't
     really need... we just wanted certain parts of the interfaces. */
  public void windowClosed(WindowEvent we) {
  }
  public void windowActivated(WindowEvent we) {
  }
  public void windowDeactivated(WindowEvent we) {
  }
  public void windowDeiconified(WindowEvent we) {
  }
  public void windowIconified(WindowEvent we) {
  }
  public void windowOpened(WindowEvent we) {
  }
  public void componentMoved(ComponentEvent ce) {
  }
  public void componentShown(ComponentEvent ce) {
  }
  public void componentHidden(ComponentEvent ce) {
  }

  private void setupSS() {
    // set up SmartSockets
    try {
      srv = TipcSvc.getSrv();

      // set project
      Tut.setOption("ss.project", "rtworks");

      try {
        // register message types
        mtLine = TipcSvc.createMt("wb_line", WB_LINE,
                  "str int4 int4 int4 int4 int4 int4");
        mtText = TipcSvc.createMt("wb_text", WB_TEXT,
                  "str int4 int4 int4 int4 str");
        mtImage = TipcSvc.createMt("wb_image", WB_IMAGE,
                   "str int4 int4 int4 binary");
        mtClear = TipcSvc.createMt("wb_clear", WB_CLEAR,
                   "str int4 int4");
      } catch (TipcException e) {
        System.out.println("Can't register whiteboard message types!");
      }

      // make us the listener for all the types
      if (!(null!=srv.addProcessCb(this, mtLine, srv)) ||
      !(null!=srv.addProcessCb(this, mtText, srv)) ||
      !(null!=srv.addProcessCb(this, mtClear, srv)) ||
      !(null!=srv.addProcessCb(this, mtImage, srv)) ) {
        Tut.exitFailure("Couldn't register process listener!\n");
      }

      // connect to RTserver
      if (!srv.create()) {
        Tut.exitFailure("Couldn't connect to RTserver!\n");
      }

      // subscribe to the appropriate subject
      srv.setSubjectSubscribe(baseSubject + subj.getText(), true);
    }
    catch (TipcException e) {
      Tut.warning("An exception occurred: " + e.getMessage());
      System.exit(1);
    }
  } // setupSS

  public void init() {
    // call the setup functions; one for the GUI, one for SmartSockets
    setupGUI();
    setupSS();
  } // init

  /*
   *   interior class--the whiteboard graphic area
   */
  class wBoard extends JLayeredPane {
    // would extend Canvas but that's a heavyweight component, can't
    // do a popup menu over that (well, not prettily)

    public void paint(Graphics g) {
      Rectangle rect = this.getBounds();
      g.setColor(Color.white);
      g.fillRect(0, 0, rect.width, rect.height);
      Dimension d = this.getSize();
      g.setColor(Color.black);
      g.drawRect(0, 0, d.width-1, d.height-1);

      for (int i=0;i<wbObjects.size();i++) {
    wbStroke str = (wbStroke)wbObjects.elementAt(i);
    str.render(g);
      }
    } // paint
  } // wBoard

  public static void main(String args[]) {
    // start it up
    new WhiteBoard();
  } // main

} // SSWhiteBoard

