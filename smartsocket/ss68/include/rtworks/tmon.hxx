/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tmon.hxx#1 $
 */

#ifndef _T_MONITOR_HH_
#define _T_MONITOR_HH_

#include <rtworks/ipc.h>
#include <rtworks/tobj.hxx>

class T_IPCX_API TipcMon : public Tobj {

public:
  TipcMon()  { }

  //  TipcMon(const TipcMon& );
  //  TipcMon& operator=(const TipcMon& );

  virtual ~TipcMon();

public:
  // Ident functions
  T_STR  IdentStr();                                 // Get it
  T_BOOL IdentStr(T_STR type_str);                   // Set it

  // Watch functions
  T_BOOL PrintWatch(T_OUT_FUNC out_func);    

  T_BOOL ProjectNamesWatch();                        // Get it
  T_BOOL ProjectNamesWatch(T_BOOL watch_status);     // Set it
#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
  T_BOOL AppNamesWatch();                            // Get it
  T_BOOL AppNamesWatch(T_BOOL watch_status);         // Set it
#endif

  T_BOOL ClientNamesWatch();                         // Get it
  T_BOOL ClientNamesWatch(T_BOOL watch_status);      // Set it

  T_BOOL ServerNamesWatch();                         // Get it
  T_BOOL ServerNamesWatch(T_BOOL watch_status);      // Set it

  T_BOOL SubjectNamesWatch();                        // Get it
  T_BOOL SubjectNamesWatch(T_BOOL watch_status);     // Set it
#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
  T_BOOL DgNamesWatch();                             // Get it
  T_BOOL DgNamesWatch(T_BOOL watch_status);          // Set it
#endif

  T_BOOL SubjectSubscribeWatch(T_STR subject_name);  // Get it
  T_BOOL SubjectSubscribeWatch(T_STR subject_name,
		               T_BOOL watch_status); // Set it
#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
  T_BOOL DgRecvWatch(T_STR dg_name);                 // Get it
  T_BOOL DgRecvWatch(T_STR dg_name,
		     T_BOOL watch_status);           // Set it
#endif

  // Poll functions

  T_BOOL ProjectNamesPoll();
  T_BOOL SubjectNamesPoll();
  T_BOOL SubjectSubscribePoll(T_STR dg_name);
#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
  T_BOOL AppNamesPoll();
  T_BOOL DgNamesPoll();
  T_BOOL DgRecvPoll(T_STR dg_name);
#endif
  T_BOOL ClientNamesPoll();
  T_BOOL ServerNamesPoll();

#ifndef T_SS35_HIERARCHY
  T_BOOL ServerConnWatch();                    // Get it
  T_BOOL ServerConnWatch(T_BOOL watch_status); // Set it
  T_BOOL ServerConnPoll();
#endif
};

#endif











