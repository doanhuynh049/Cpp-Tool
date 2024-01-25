/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/servlet/Mon.java#1 $
 */

import java.io.*;

import javax.servlet.*;
import javax.servlet.http.*;

import com.smartsockets.*;

/*
 *  Example SmartSockets servlet
 *  (tested with "servletrunner -d /path/to/class/file")
 *
 *  Demonstrates simple use of the monitoring API (TipcMon) as
 *  well as basic servlet technique.
 *
 *  NOTE: read the readme.txt in this directory for more info!
 *  ----
 */

public class Mon
extends HttpServlet
implements SingleThreadModel {
  TipcSrv srv;

  static final int S_NAMES = 1;
  static final int S_GENERAL = 2;
  static final int S_CLIENT = 3;

  int output_mode;
  ServletOutputStream out;
  int recd;
  String requestUri;
  static final int timeout_secs = 15;


  public void doGet (HttpServletRequest req, HttpServletResponse res)
  throws ServletException, IOException {
    out = res.getOutputStream();
    res.setContentType("text/html");
    requestUri = req.getRequestURI();

    initSS();

    out.println("<h1>SmartSockets Simple Monitoring Servlet</h1><hr>");

    recd = 0;
    String server_name = req.getParameter("server");
    String client_name = req.getParameter("client");

    try {
      if (server_name != null) {
        output_mode = S_GENERAL;
        TipcMon.serverGeneralPoll(server_name);
      } else if (client_name != null) {
        output_mode = S_CLIENT;
        TipcMon.clientGeneralPoll(client_name);
      } else {
        output_mode = S_NAMES;
        TipcMon.serverNamesPoll();
      } // if - determine mode

      srv = TipcSvc.getSrv();
      int num_tries = 0;
      while ( (recd == 0) && (num_tries < timeout_secs)) {
        srv.mainLoop(1.0);
        num_tries++;
      } // while
    } catch (TipcException te) {
      // no catch
    } // catch

    if (recd != output_mode) {
      out.print("<h2>Unable to process request, sorry!</h2>");
    } // if no response in time

    out.close();

    stopSS();
  } // doGet


  public String getServletInfo() {
    return "A simple servlet";
  } // getServletInfo


  public void parseServerGeneral(TipcMsg msg) throws IOException {
    String ro = "<tr><td>";
    String rc = "</td></tr>";
    String cc = "</td><td>";
    String b = "<b>";

    try {
      String sn = msg.nextStr();
      out.println("<h2>General information about RTserver <i>" + sn + "</i></h2>");
      out.println("<table border=2 cellpadding=3>");
      out.println(ro + b + "Server name " + cc + sn + rc);
      out.println(ro + b + "Identity: " + cc + msg.nextStr() + rc);
      out.println(ro + b + "Node: " + cc + msg.nextStr() + rc);
      out.println(ro + b + "Username: " + cc + msg.nextStr() + rc);
      out.println(ro + b + "Process ID: " + cc + msg.nextInt4() + rc);
      out.println(ro + b + "Architecture: " + cc + msg.nextStr() + rc);
      out.println(ro + b + "Current sbrk: " + cc + msg.nextInt4() + rc);
      out.println(ro + b + "Delta sbrk: " + cc + msg.nextInt4() + rc);
      out.println(ro + b + "Int format: " + cc + msg.nextInt2() + rc);
      out.println(ro + b + "Real format: " + cc + msg.nextInt2() + rc);
      out.println(ro + b + "Command file: " + cc + msg.nextStr() + rc);
      out.println(ro + b + "No daemon flag: " + cc + msg.nextInt4() + rc);
      out.println(ro + b + "Command mode flag: " + cc + msg.nextInt4() + rc);
      out.println("</table><table>");

      String dc[] = msg.nextStrArray();
      out.println(ro + "<b>Direct clients:</b>" + rc);
      if (dc.length > 0) {
        for (int i = 0;i < dc.length;i++) {
          out.println(ro + "<a href=" + requestUri + "?client=" +
                      dc[i] + ">" + dc[i] + "</a>" + rc);
        } // for dir clients
      } else out.println(ro + "(None)" + rc);

      String ds[] = msg.nextStrArray();
      out.println(ro + "<b>Direct servers:</b>" + rc);
      if (ds.length > 0) {
        for (int i = 0;i < ds.length;i++) {
          out.println(ro + "<a href=" + requestUri + "?server=" +
                      ds[i] + ">" + ds[i] + "</a>" + rc);
        } // for dir svrs
      } else out.println(ro + "(None)" + rc);

      String ss[] = msg.nextStrArray();
      out.println(ro + "<b>Server subscribes:</b>" + rc);
      if (ss.length > 0) {
        for (int i = 0;i < ss.length;i++) {
          out.println(ro + ss[i] + rc);
        } // for svr subs
      } else out.println(ro + "(None)" + rc);

      String cs[] = msg.nextStrArray();
      out.println(ro + "<b>Client subscribes:</b>" + rc);
      if (cs.length > 0) {
        for (int i = 0;i < cs.length;i++) {
          out.println(ro + cs[i] + rc);
        } // for client subs
      } else out.println(ro + "(None)" + rc);
      out.println("</table>");

    } catch (TipcException e) {
      out.println("<tr><td>" + e.getMessage() + "</td></tr>");
    } // catch
  } // parseServerGeneral


  public void parseClientGeneral(TipcMsg msg) throws IOException {
    String ro = "<tr><td>";
    String rc = "</td></tr>";
    String cc = "</td><td>";
    String b = "<b>";

    try {
      String cn = msg.nextStr();
      out.println("<h2>General information about RTclient <i>" + cn + "</i></h2>");
      out.println("<table border=2 cellpadding=3>");
      out.println(ro + b + "Client name " + cc + cn + rc);
      out.println(ro + b + "Identity: " + cc + msg.nextStr() + rc);
      out.println(ro + b + "Node: " + cc + msg.nextStr() + rc);
      out.println(ro + b + "Username: " + cc + msg.nextStr() + rc);
      out.println(ro + b + "Process ID: " + cc + msg.nextInt4() + rc);
      out.println(ro + b + "Project: " + cc + msg.nextStr() + rc);
      String sn = msg.nextStr();
      out.println(ro + b + "Server: " + cc + "<a href=" + requestUri +
                  "?server=" + sn + ">" + sn + "</a>" + rc);
      out.println(ro + b + "Architecture: " + cc + msg.nextStr() + rc);
      out.println(ro + b + "Current sbrk: " + cc + msg.nextInt4() + rc);
      out.println(ro + b + "Delta sbrk: " + cc + msg.nextInt4() + rc);
      out.println(ro + b + "Int format: " + cc + msg.nextInt2() + rc);
      out.println(ro + b + "Real format: " + cc + msg.nextInt2() + rc);
      out.println(ro + b + "LCN: " + cc + msg.nextStr() + rc);
      out.println("</table><table>");

      int count_lic[] = msg.nextInt4Array();
      String extra_lic[] = msg.nextStrArray();

      String cs[] = msg.nextStrArray();
      out.println(ro + "<b>Subscribed subjects:</b>" + rc);
      if (cs.length > 0) {
        for (int i = 0;i < cs.length;i++) {
          out.println(ro + cs[i] + rc);
        } // for client subs
      } else out.println(ro + "(None)" + rc);
      out.println("</table>");

    } catch (TipcException e) {
      out.println("<tr><td>" + e.getMessage() + "</td></tr>");
    } // catch
  } // parseClientGeneral


  public void parseServerNames(TipcMsg msg) throws IOException {
    String ro = "<tr><td>";
    String rc = "</td></tr>";
    String cc = "</td><td>";
    String i = "<i>";

    out.println("<h2>Servers Available</h2>");
    out.println("<table border=2 cellpadding=3>");
    try {
      String[] snames = msg.nextStrArray();
      String[] sinfo = msg.nextStrArray();
      for (int ct=0; ct<snames.length; ct++) {
        out.println(ro + "<a href=" + requestUri + "?server=" +
                    snames[ct] + ">" + snames[ct] + "</a>" +
                    cc + i + sinfo[ct] + rc);
      } // for
      out.println("</table>");

    } catch (TipcException nfe) {
      out.println("<tr><td>Difficulty parsing message!</td></tr>");
    } // catch
  } // parseServerNames


  void initSS() {
    try {
      srv = TipcSvc.getSrv();
      srv.create();
      srv.addDefaultCb(new Handler(), null);
    } catch (TipcException e) {

    } // catch
  } // initSS

  void stopSS() {
    /*try {
      srv.destroy();
    } catch (TipcException e) {
    }*/
  } // stopSS

  public void destroy() {
    //srv.removeDefaultCb(this);
    //stopSS();
  } // destroy


  class Handler implements TipcDefaultCb {

    public void handle(TipcMsg msg, Object obj) {
      if (recd == output_mode) {
        return;
      }

      try {
        switch (msg.getType().getNum()) {

        case TipcMt.MON_SERVER_GENERAL_POLL_RESULT:
          if (output_mode == S_GENERAL) {
            parseServerGeneral(msg);
            recd = S_GENERAL;
          } // if general
          break;

        case TipcMt.MON_SERVER_NAMES_POLL_RESULT:
          if (output_mode == S_NAMES) {
            parseServerNames(msg);
            recd = S_NAMES;
          } // if names
          break;

        case TipcMt.MON_CLIENT_GENERAL_POLL_RESULT:
          if (output_mode == S_CLIENT) {
            parseClientGeneral(msg);
            recd = S_CLIENT;
          } // if client
          break;

        } // switch
      } catch (IOException ioe) {

      } // catch
    } // handle
  } // Handler

} // servlet

