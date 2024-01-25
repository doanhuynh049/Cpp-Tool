/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/cloud/CloudApplet.java#1 $
 */

import java.util.Vector;
import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;
import com.smartsockets.*;

/*
 *  SmartSockets Java demo: CloudApplet
 *  BBM 06/23/1998
 *  BBM 06/25/1998 improved init/start handling
 *
 *  This applet demonstrates some basics of monitoring with the TipcMon
 *  static class; servers and clients are monitored as the come and go
 *  from the RTserver communications cloud, and displayed graphically.
 *  Note that the oval displayed connecting the servers is NOT necessarily
 *  an indication of the interconnectedness of the servers. RTclients,
 *  however, ARE accurately portrayed as 'moons' around server 'planets' in
 *  the cloud 'universe'.
 *
 *  More could easily be added to this, however a new LayoutManager will
 *  likely be required. Some of the work to extend the Client and Server
 *  classes to support mouse events is already done (they extend Component
 *  and are added/removed to the parent container as necessary), but more
 *  is needed to get consistent, clean output to the screen. It appears a
 *  proper LayoutManager that understands the graph being displayed needs
 *  to be implemented, or the events don't get percolated to the lightweight
 *  Client and Server objects. (Using FlowLayout, etc., does allow the events
 *  to reach them, but the components are always being moved by the LM to
 *  less desirable :-) locations).
 *
 *  If placing this on a webserver, don't forget server.gif and client.gif,
 *  the bitmaps for the connection entities. They are not absolutely required,
 *  but are nice visual enhancements.
 *
 *  There are two ways for a browser or appletviewer to access the
 *  SmartSockets jar file 'ss.jar'. Either include the SmartSockets jar
 *  file in the browser or appletviewer classpath (or setting the
 *  CLASSPATH environment variable) or by relying on the ARCHIVE attribute
 *  of the APPLET tag in the html file that drives this applet (the jar
 *  file must be present in the same directory, even under appletviewer).
 *
 *  An example of running this applet with Sun's appletviewer bundled with
 *  JDK 1.2 (note: JDK 1.3 does not support the -nosecurity argument so the
 *  java.policy file will usually need to be modified):
 *
 *   appletviewer -J-classpath -J../../lib/ss.jar -nosecurity CloudApplet.html
 *
 */

public class CloudApplet extends Applet
implements TipcDefaultCb, Runnable {
  static final String info = "Displays information about RTservers and clients in our cloud.";
  TipcSrv srv;
  Thread reader = null;
  Cloud cloud = new Cloud();
  static final String defaultStatus = "Place mouse over an object for more info";
  Container applet;
  Image srvImage = null;
  Image clImage = null;
  Color serverColor = Color.blue;
  Color clientColor = Color.red;
  Font smFont = new Font("Serif", Font.PLAIN, 9);
  boolean beenInitialized = false;

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
    if (beenInitialized) {
      //return;
    }
    beenInitialized = true;

    String host = getDocumentBase().getHost();
    try {
      if (!host.equals("")) {
        Tut.setOption("ss.server_names", host);
      }
      srv = TipcSvc.getSrv();
      srv.create();

      srv.addDefaultCb(this, null);
      srv.addProcessCb(cloud, TipcMt.MON_SERVER_NAMES_STATUS, this);
      srv.addProcessCb(cloud, TipcMt.MON_CLIENT_NAMES_STATUS, this);
    }
    catch (TipcException te) {
    }

    //applet = getContentPane();
    applet = this;
    //applet.setLayout(null);

    srvImage = getImage(getDocumentBase(), "server.gif");
    clImage = getImage(getDocumentBase(), "client.gif");
  } // init

  public String getAppletInfo() {
    return info;
  } // getAppletInfo

  public void stop() {
    reader = null;
  } // stop

  public void start() {
    try {
      TipcMon.setServerNamesWatch(false);
      TipcMon.setClientNamesWatch(false);
      TipcMon.setServerNamesWatch(true);
      TipcMon.setClientNamesWatch(true);
    }
    catch (TipcException e) {
      Tut.fatal(e);
    }

    reader = new Thread(this);
    reader.start();
    repaint();
    //showStatus(defaultStatus);
  } // start

  public void destroy() {
    // currently, do NOT do a TipcSrv.destroy() on applet destroy()
    /*try {
      srv.destroy();
    } catch (TipcException te) { }*/
  } // destroy

  public void handle(TipcMsg msg, Object arg) {
    TipcMt typ = msg.getType();
    System.out.println("msg type = "+typ.getName());
  } // handle

  public static void main(String args[]) {
    CloudApplet rt = new CloudApplet();
    rt.init();
  } // main

  public void paint(Graphics g) {
    int ns = cloud.getNumServers();
    Dimension sz = getSize();
    int cy = sz.height/2;
    int cx = sz.width/2;
    int xw = cx-64;
    int yw = cy-64;
    double sr = (2*java.lang.Math.PI)/ns;
    double curr = java.lang.Math.PI/2;  // start at top

    g.setFont(smFont);
    g.setColor(Color.gray);
    g.drawOval(64, 64, sz.width-128, sz.height-128);

    for (int i=0;i<ns;i++) {
      int curx = cx + (int)(java.lang.Math.cos(curr)*xw);
      int cury = cy - (int)(java.lang.Math.sin(curr)*yw);

      Server s = cloud.getServer(i);
      s.setLocation(curx-16, cury-16);
      s.paint(g);

      int nc = s.getNumClients();
      double cr = (2*java.lang.Math.PI)/nc;
      double ccr = curr;
      for (int j=0;j<nc;j++) {
        Client c = s.getClient(j);
        int ccx = curx + (int)(java.lang.Math.cos(ccr)*48);
        int ccy = cury - (int)(java.lang.Math.sin(ccr)*48);
        c.setLocation(ccx-8, ccy-8);
        g.setColor(clientColor);
        g.drawLine(curx, cury, ccx, ccy);
        g.drawLine(curx+1, cury, ccx+1, ccy);
        c.paint(g);
        ccr += cr;
      } // for clients

      curr += sr;
    } // for servers
  } // paint

  void refresh() {
    repaint();
    /*applet.invalidate();
    applet.validate();*/
  } // refresh

//-----------------Client class---------------------------------------------

  class Client extends Component {
    String name;
    String info;

    protected void processEvent(AWTEvent e) {
      switch (e.getID()) {
      case MouseEvent.MOUSE_ENTERED:
        showStatus("Client "+name);
        break;
      case MouseEvent.MOUSE_EXITED:
        showStatus(defaultStatus);
        break;
      case MouseEvent.MOUSE_CLICKED:
        // more detailed handling here in the future
        break;
      }
      super.processEvent(e);
    }

    // getSize, getBounds needed for mouse enter/exit event triggering
    public Dimension getSize() {
      return new Dimension(16,16);
    }
    public Rectangle getBounds() {
      return new Rectangle(getLocation(),getSize());
    }

    public void paint(Graphics g) {
      Point p = getLocation();
      g.setColor(clientColor);
      if (null != clImage)
        g.drawImage(clImage, p.x, p.y, applet);
      else
        g.drawOval(p.x, p.y, 16, 16);
      g.drawString(name, p.x, p.y);
    }

    public Client(String nam, String inf) {
      name = nam;
      info = inf;
      enableEvents(AWTEvent.MOUSE_EVENT_MASK);
    }

    public String getName() {
      return name;
    }

    public String getInfo() {
      return info;
    }

    public void setInfo(String inf) {
      info = inf;
    }
  } // Client

//-----------------Server class---------------------------------------------

  class Server extends Component {
    String name;
    String info;
    Vector clients = new Vector();

    protected void processEvent(AWTEvent e) {
      System.out.println("processEvent on server "+name);
      switch (e.getID()) {
      case MouseEvent.MOUSE_ENTERED:
        showStatus("Server "+name);
        break;
      case MouseEvent.MOUSE_EXITED:
        showStatus(defaultStatus);
        break;
      case MouseEvent.MOUSE_CLICKED:
        // more detailed handling here in the future
        break;
      }
      super.processEvent(e);
    } // processEvent

    // getSize, getBounds needed for mouse enter/exit event triggering
    public Dimension getSize() {
      return new Dimension(32, 32);
    } // getSize
    public Rectangle getBounds() {
      return new Rectangle(getLocation(),getSize());
    } // getBounds

    public void paint(Graphics g) {
      Point p = getLocation();
      g.setColor(serverColor);
      if (null != srvImage)
        g.drawImage(srvImage, p.x, p.y, applet);
      else
        g.drawRect(p.x, p.y, 32, 32);
      g.drawString(name, p.x, p.y);
    } // paint

    public int getNumClients() {
      return clients.size();
    } // getNumClients

    public Client getClient(int which) {
      return(Client)clients.elementAt(which);
    } // getClient

    public Server(String nam, String inf) {
      name = nam;
      info = inf;
      enableEvents(AWTEvent.MOUSE_EVENT_MASK);
    } // constructor

    public String getName() {
      return name;
    } // getName

    int findClient(String client_name) {
      for (int i=0;i<clients.size();i++) {
        Client vc = (Client)clients.elementAt(i);
        if (vc.getName().equals(client_name)) {
          return i;
        } // if found
      } // for

      return -1;
    } // findClient

    public void addClient(Client c) {
      int whichc = findClient(c.getName());
      if (-1 == whichc) {
        clients.addElement(c);
        applet.add(c);
        refresh();
      }
      else {
        if (!"".equals(c.getInfo())) {
          Client cl = (Client)clients.elementAt(whichc);
          cl.setInfo(c.getInfo());
          refresh();
        } // new info
      } // else
    } // addClient

    public void removeClient(String name) {
      int idx = findClient(name);
      if (-1 != idx) {
        Client c = (Client)clients.elementAt(idx);
        clients.removeElementAt(idx);
        applet.remove(c);
        refresh();
      } // if found
    } // removeClient

    public String getInfo() {
      return info;
    } // getInfo

    public void setInfo(String inf) {
      info = inf;
    } // setInfo

  } // Server

//-----------------Cloud class----------------------------------------------

  class Cloud implements TipcProcessCb {
    Vector servers = new Vector();

    public int getNumServers() {
      return servers.size();
    } // getNumServers

    public Server getServer(int which) {
      return(Server)servers.elementAt(which);
    } // getServer

    int findServer(String server_name) {
      for (int i=0;i<servers.size();i++) {
        Server vs = (Server)servers.elementAt(i);
        if (vs.getName().equals(server_name)) {
          return i;
        } // if
      } // for

      return -1;
    } // findServer

    void addServer(String sname, String info) {
      int whichs = findServer(sname);
      if (-1 == whichs) {
        Server new_s = new Server(sname, info);
        servers.addElement(new_s);
        applet.add(new_s);
        refresh();
      }
      else {
        if (!"".equals(info)) {
          Server s = (Server)servers.elementAt(whichs);
          if ("".equals(s.getInfo())) {
            s.setInfo(info);
            refresh();
          } // if
        } // if
      } // else
    } // addServer

    void removeServer(String sname) {
      int whichs = findServer(sname);
      if (-1 != whichs) {
        Server s = (Server)servers.elementAt(whichs);
        servers.removeElementAt(whichs);
        applet.remove(s);
        refresh();
      }
    } // removeServer

    void addClientToServer(String sname, String cname, String cinfo) {
      int whichs = findServer(sname);
      if (-1 == whichs) {
        addServer(sname, "");
      }
      Server s = (Server)servers.elementAt(whichs);
      s.addClient(new Client(cname, cinfo));
    } // addClientToServer

    void removeClient(String cname) {
      for (int i=0;i<servers.size();i++) {
        Server s = (Server)servers.elementAt(i);
        s.removeClient(cname);
      }
    } // removeClient

    public void process(TipcMsg msg, Object o) {
      try {
        Panel p = (Panel)o;
        msg.setCurrent(0);
        int typ = msg.getType().getNum();
        switch (typ) {

        case TipcMt.MON_SERVER_NAMES_STATUS:
          String sn[] = msg.nextStrArray();
          String si[] = msg.nextStrArray();
          String crs = msg.nextStr();
          String des = msg.nextStr();
          if ("".equals(crs) && "".equals(des)) {
            servers.removeAllElements();
            for (int i=0;i<sn.length;i++) {
              addServer(sn[i], si[i]);
            } // for
          } // if
          if (crs.length()>0) {
            for (int i=0;i<sn.length;i++) {
              if (sn[i].equals(crs)) {
                addServer(sn[i], si[i]);
              } // if
            } // for
          } // if
          if (des.length()>0) {
            removeServer(des);
          } // if
          break;

        case TipcMt.MON_CLIENT_NAMES_STATUS:
          String cn[] = msg.nextStrArray();
          String ci[] = msg.nextStrArray();
          String csn[] = msg.nextStrArray();
          String crc = msg.nextStr();
          String dec = msg.nextStr();
          if ("".equals(crc) && "".equals(dec)) {
            for (int i=0;i<cn.length;i++) {
              addServer(csn[i], "");
              addClientToServer(csn[i], cn[i], ci[i]);
            } // for
          } // add all
          if (crc.length()>0) {
            for (int i=0;i<cn.length;i++) {
              if (cn[i].equals(crc)) {
                addClientToServer(csn[i], crc, ci[i]);
              } // if
            } // for
          } // if
          if (dec.length()>0) {
            removeClient(dec);
          } // if
          break;
        }
      }
      catch (TipcException e) {
        System.err.println("SS error: " + e.getMessage());
      }
    } // process

  } // Cloud

} // main class

