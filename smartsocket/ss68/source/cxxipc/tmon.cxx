/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tmon.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tmon.hxx>

/* ========================================================================= */
/*..TipcMon::~TipcMon -- TipcMon destructor */
TipcMon::~TipcMon()
{
}

#if 0
// -------------------------------------------------------------
// Assignment operator & copy constructor
// -------------------------------------------------------------
TipcMon& TipcMon::operator=(const TipcMon& m)
{

}
TipcMon::TipcMon(const TipcMon& msg)
{
}
#endif

// -------------------------------------------------------------
// Ident functions
// -------------------------------------------------------------

/* ========================================================================= */
/*..TipcMon::IdentStr -- get the monitoring identification string of */
/*..  this process */
T_STR TipcMon::IdentStr()                           
{
  T_STR result = (T_STR)"";  // initialize variable

  _status = TipcMonGetIdentStr(&result);
  return result;
}

/* ========================================================================= */
/*..TipcMon::IdentStr -- set the monitoring identification string of */
/*..  this process */
T_BOOL TipcMon::IdentStr(T_STR type_str)             
{
  _status = TipcMonSetIdentStr(type_str);
  return _status;
}

// -------------------------------------------------------------
// Watch functions
// -------------------------------------------------------------

/* ========================================================================= */
/*..TipcMon::PrintWatch -- print all monitoring categories being watched */
T_BOOL TipcMon::PrintWatch(T_OUT_FUNC out_func)   
{
  _status = TipcMonPrintWatch(out_func);
  return _status;
}

/* ========================================================================= */
/*..TipcMon::ProjectNamesWatch --get whether or not this RTclient process is */
/*..  watching project names */
T_BOOL TipcMon::ProjectNamesWatch()
{
  T_BOOL result = T_FALSE;  // initialize variable

  _status = TipcMonProjectNamesGetWatch(&result);
  return result;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMon::AppNamesWatch -- get whether or not this RTclient process is */
/*..  watching application names */
T_BOOL TipcMon::AppNamesWatch()
{
  return ProjectNamesWatch();
}
#endif

/* ========================================================================= */
/*..TipcMon::ProjectNamesWatch -- start or stop watching project names       */
T_BOOL TipcMon::ProjectNamesWatch(T_BOOL watch_status)
{
  _status = TipcMonProjectNamesSetWatch(watch_status);
  return _status;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMon::AppNamesWatch -- start or stop watching application names */
T_BOOL TipcMon::AppNamesWatch(T_BOOL watch_status)
{
  return ProjectNamesWatch(watch_status);
}
#endif

/* ========================================================================= */
/*..TipcMon::ClientNamesWatch -- get whether or not this RTclient process */
/*..  is watching RTclient names */
T_BOOL TipcMon::ClientNamesWatch()
{
  T_BOOL result = T_FALSE;  // initialize variable

  _status = TipcMonClientNamesGetWatch(&result);
  return result;
}

/* ========================================================================= */
/*..TipcMon::ClientNamesWatch -- start or stop watching RTclient names */
T_BOOL TipcMon::ClientNamesWatch(T_BOOL watch_status)
{
  _status = TipcMonClientNamesSetWatch(watch_status);
  return _status;
}

/* ========================================================================= */
/*..TipcMon::ServerNamesWatch -- get whether or not this RTclient process */
/*..  is watching RTserver names */
T_BOOL TipcMon::ServerNamesWatch()                   
{
  T_BOOL result = T_FALSE;  // initialize variable

  _status = TipcMonServerNamesGetWatch(&result);
  return result;
}

/* ========================================================================= */
/*..TipcMon::ServerNamesWatch -- start or stop watching RTserver names */
T_BOOL TipcMon::ServerNamesWatch(T_BOOL watch_status)
{
  _status = TipcMonServerNamesSetWatch(watch_status);
  return _status;
}

/* ========================================================================= */
/*..TipcMon::SujectNamesWatch -- get whether or not this RTclient process */
/*..  is watching subject names */
T_BOOL TipcMon::SubjectNamesWatch()
{
  T_BOOL result = T_FALSE;  // initialize variable

  _status = TipcMonSubjectNamesGetWatch(&result);
  return result;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMon::DgNamesWatch -- get whether or not this RTclient process */
/*..  is watching datagroup names */
T_BOOL TipcMon::DgNamesWatch()
{
  return SubjectNamesWatch();
}
#endif

/* ========================================================================= */
/*..TipcMon::SubjectNamesWatch -- start or stop watching subject names */
T_BOOL TipcMon::SubjectNamesWatch(T_BOOL watch_status)
{
  _status = TipcMonSubjectNamesSetWatch(watch_status);
  return _status;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMon::DgNamesWatch -- start or stop watching datagroup names */
T_BOOL TipcMon::DgNamesWatch(T_BOOL watch_status)
{
  return SubjectNamesWatch(watch_status);
}
#endif

/* ========================================================================= */
/*..TipcMon::SubjectSubscribeWatch -get whether or not this RTclient process */
/*..  is watching the RTclient processes that are receiving a subject */
T_BOOL TipcMon::SubjectSubscribeWatch(T_STR subject_name)
{
  T_BOOL result = T_FALSE; // initialize variable

  _status = TipcMonSubjectSubscribeGetWatch(subject_name, &result);
  return result;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMon::DgRecvWatch -- get whether or not this RTclient process */
/*..  is watching the RTclient processes that are receiving a datagroup */
T_BOOL TipcMon::DgRecvWatch(T_STR dg_name)
{
  return SubjectSubscribeWatch(dg_name);
}
#endif

/* ========================================================================= */
/*..TipcMon::SubjectSubscribeWatch start or stop watching RTclient processes */
/*..  that are receiving a subject */
T_BOOL TipcMon::SubjectSubscribeWatch(T_STR subject_name, T_BOOL watch_status)
{
  _status = TipcMonSubjectSubscribeSetWatch(subject_name, watch_status);
  return _status;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMon::DgRecvWatch -- start or stop watching RTclient processes */
/*..  that are receiving a datagroup */
T_BOOL TipcMon::DgRecvWatch(T_STR dg_name, T_BOOL watch_status)
{
  return SubjectSubscribeWatch(dg_name, watch_status);
}
#endif

// -------------------------------------------------------------------
// Poll functions
// -------------------------------------------------------------------

/* ========================================================================= */
/*..TipcMon::ProjectNamesPoll -- poll once for project names */
T_BOOL TipcMon::ProjectNamesPoll()
{
  _status = TipcMonProjectNamesPoll();
  return _status;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMon::AppNamesPoll -- poll once for application names */
T_BOOL TipcMon::AppNamesPoll()
{
  return ProjectNamesPoll();
}
#endif

/* ========================================================================= */
/*..TipcMon::ClientNamesPoll -- poll once for RTclient names */
T_BOOL TipcMon::ClientNamesPoll()
{
  _status = TipcMonClientNamesPoll();
  return _status;
}

/* ========================================================================= */
/*..TipcMon::ServerNamesPoll -- poll once for RTserver names */
T_BOOL TipcMon::ServerNamesPoll()
{
  _status = TipcMonServerNamesPoll();
  return _status;
}

/* ========================================================================= */
/*..TipcMon::SubjectNamesPoll -- poll once for subject names */
T_BOOL TipcMon::SubjectNamesPoll()
{
  _status = TipcMonSubjectNamesPoll();
  return _status;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMon::DgNamesPoll -- poll once for datagroup names */
T_BOOL TipcMon::DgNamesPoll()
{
  return SubjectNamesPoll();
}
#endif

/* ========================================================================= */
/*..TipcMon::SubjectSubscribePoll -- poll once for the names of the RTclient */
/*..  processes that are receiving a subject */
T_BOOL TipcMon::SubjectSubscribePoll(T_STR subject_name)
{
  _status = TipcMonSubjectSubscribePoll(subject_name);
  return _status;
}

#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
/* ========================================================================= */
/*..TipcMon::DgRecvPoll -- poll once for the names of the RTclient */
/*..  processes that are receiving a datagroup */
T_BOOL TipcMon::DgRecvPoll(T_STR dg_name)
{
  return SubjectSubscribePoll(dg_name);
}

#endif

#ifndef T_SS35_HIERARCHY

/* ========================================================================= */
/*.. TipcMon::ServerConnWatch -- get watch on server connection */
T_BOOL TipcMon::ServerConnWatch()
{
  T_BOOL status_return;

  _status = TipcMonServerConnGetWatch(&status_return);
  return status_return;
}

/* ========================================================================= */
/*.. TipcMon::ServerConnWatch -- set watch on server connections */
T_BOOL TipcMon::ServerConnWatch(T_BOOL watch_status)
{
  _status = TipcMonServerConnSetWatch(watch_status);
  return _status;
}

/* ========================================================================= */
/*.. TipcMon::ServerConnPoll -- poll once on server connections */
T_BOOL TipcMon::ServerConnPoll()
{
  _status = TipcMonServerConnPoll();
  return _status;
}


#endif
