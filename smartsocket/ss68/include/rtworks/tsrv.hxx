/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/tsrv.hxx#1 $
 */

#ifndef _T_SERVER_HH_
#define _T_SERVER_HH_

#include <rtworks/ipc.h>

/* ----------------------------------------------------------------------- */
/* The RTServer constructor is protected because in RTworks it doesn't     */
/* make sense to have more than one RTServer connection per process.  So   */
/* users won't be allowed to construct an RTServer instance directly; in   */
/* fact, if they try, their code won't even compile.  However, the user    */
/* still needs to be able to get a handle to an RTServer instance in order */
/* to call the C API TipcSrv* versions of C API TipcConn* virtual          */
/* functions inherited from the TipcConnWrapper class, and he or she still */
/* needs to be able cause TipcSrvCreate() to be called by some mechanism.  */
/* For the underlying C function TipcSrvCreate() to be called, the user    */
/* should instead call the static RTServer function instanceCreate() or    */
/* instance(), the difference between these functions being whether        */
/* TipcSrvCreate gets called or not as a side effect.  It's then OK to ask */
/* for the instance() more than once in user code, as for example would be */
/* required to call any nonstatic member function of the RTServer class    */
/* outside the user scope in which the RTServer handle was originally      */
/* obtained.                                                               */
/*                                                                         */
/* Since C++ doesn't allow args to destructors we also offer a destroy()   */
/* method to let the user specify a RTworks destroy status.  When RTServer */
/* is destroyed automatically (for ex: the user doesn't call destroy()     */
/* himself, and the RTServer ref obtained via instance() passes out of     */
/* scope), he will get the T_IPC_SRV_CONN_NONE destroy status by default.  */
/* ----------------------------------------------------------------------- */

#ifdef T_SS35_HIERARCHY
#include <rtworks/tipccw.hxx>
class T_IPCX_API TipcSrv : public TipcConnWrapper {
#else
  // this was moved for conceptual cleanliness.  a server connection is
  // always a speciallized client connection, so it makes sense to use a
  // heirarchy that reflects that notion.

#include <rtworks/tconn.hxx>
class T_IPCX_API TipcSrv : public TipcConnClient {
#endif

private:
  static TipcSrv* _instance;
  static T_MUTEX  _mutex;

public:
  //  Give me handle only, please don't call TipcSrvCreate() for me
  static TipcSrv& Instance();

  //  Give me handle & create connection for me (ie call TipcSrvCreate())
  static TipcSrv& InstanceCreate(T_IPC_SRV_CONN_STATUS create_status=
                                 T_IPC_SRV_CONN_FULL);

  static T_BOOL Destroy(T_IPC_SRV_CONN_STATUS destroy_status);

  // For causing creation after obtaining handle via instance()
  T_BOOL Create(T_IPC_SRV_CONN_STATUS create_status);

protected:
  TipcSrv(T_IPC_SRV_CONN_STATUS create_status=T_IPC_SRV_CONN_FULL,
          T_BOOL create=FALSE);

  virtual ~TipcSrv();

private:
  TipcSrv(const TipcSrv&) {};
  TipcSrv& operator=(const TipcSrv&) { return *this; };

public:
  // ----------------------------------------------------------------------
  //  The TipcSrv equivalents of the TipcConn virtual functions
  //  defined in TipcConnWrapper base class follow
  // ----------------------------------------------------------------------
  virtual T_BOOL  Check(T_IO_CHECK_MODE check_mode, T_REAL8 timeout);

  virtual T_CB    ErrorCbCreate(T_IPC_CONN_ERROR_CB_FUNC func, T_CB_ARG arg);
  virtual T_CB    ErrorCbLookup(T_IPC_CONN_ERROR_CB_FUNC func, T_CB_ARG arg);

  virtual T_BOOL  Flush();

  virtual T_INT4  AutoFlushSize();                               // Get it
  virtual T_BOOL  AutoFlushSize(T_INT4  auto_flush_size);        // Set it

  virtual T_BOOL  BlockMode();                                   // Get it
  virtual T_BOOL  BlockMode(T_BOOL  block_mode);                 // Set it

  virtual T_REAL8 Timeout(T_IPC_TIMEOUT timeout);                // Get it
  virtual T_BOOL  Timeout(T_IPC_TIMEOUT timeout,
                          T_REAL8  value);                       // Set it

  virtual T_SOCK  Socket();                                      // Set it
  virtual T_BOOL  Socket(T_SOCK sock);                           // Set it

  virtual T_INT4  NumQueued();
  virtual T_SOCK  XtSource();
  virtual T_BOOL  KeepAlive();

  virtual T_BOOL  MainLoop(T_REAL8 timeout);

  virtual T_BOOL     Insert(TipcMsg& msg, T_INT4 pos);

  virtual TipcMsg&   Next(T_REAL8 timeout);

  virtual TipcMsg&   MsgNext(T_REAL8 timeout);

  virtual T_BOOL     Process(TipcMsg& msg);

  virtual TipcMsg&   Search(T_REAL8 timeout,
                            T_IPC_CONN_MSG_SEARCH_FUNC func,
                            T_PTR arg);

  virtual TipcMsg&   MsgSearch(T_REAL8 timeout,
                               T_IPC_CONN_MSG_SEARCH_FUNC func,
                               T_PTR arg);

  virtual TipcMsg&   SearchType(T_REAL8 timeout, TipcMt& mt);

  virtual TipcMsg&   MsgSearchType(T_REAL8 timeout, TipcMt& mt);

  virtual T_BOOL     Send(TipcMsg& msg,
                          T_BOOL check_server_msg_send=FALSE);

  virtual TipcMsg&   SendRpc(TipcMsg& call_msg, T_REAL8 timeout);

  virtual TipcMsg&   MsgSendRpc(TipcMsg& call_msg, T_REAL8 timeout);

  virtual T_BOOL     SrvWrite(T_STR dest,
                              TipcMt& mt,
                              T_BOOL check_server_msg_send, ...);

  virtual T_BOOL     SrvWriteVa(T_STR dest,
                                TipcMt& mt,
                                T_BOOL check_server_msg_send,
                                va_list var_arg_list);

  virtual T_BOOL     Print(T_OUT_FUNC func);

  virtual T_CB       DefaultCbCreate(T_IPC_CONN_DEFAULT_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_CB       DefaultCbLookup(T_IPC_CONN_DEFAULT_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_CB       ProcessCbCreate(TipcMt& mt,
                                     T_IPC_CONN_PROCESS_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_CB       ProcessCbLookup(TipcMt& mt,
                                     T_IPC_CONN_PROCESS_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_CB       QueueCbCreate(TipcMt& mt,
                                   T_IPC_CONN_QUEUE_CB_FUNC func,
                                   T_CB_ARG arg);

  virtual T_CB       QueueCbLookup(TipcMt& mt,
                                   T_IPC_CONN_QUEUE_CB_FUNC func,
                                   T_CB_ARG arg);

  virtual T_BOOL     Read(T_REAL8 timeout);

  virtual T_CB       ReadCbCreate(TipcMt& mt,
                                  T_IPC_CONN_READ_CB_FUNC func,
                                  T_CB_ARG arg);


  virtual T_CB       ReadCbLookup(TipcMt& mt,
                                  T_IPC_CONN_READ_CB_FUNC func,
                                  T_CB_ARG arg);


  virtual T_CB       WriteCbCreate(TipcMt& mt,
                                   T_IPC_CONN_WRITE_CB_FUNC func,
                                   T_CB_ARG arg);


  virtual T_CB       WriteCbLookup(TipcMt& mt,
                                   T_IPC_CONN_WRITE_CB_FUNC func,
                                   T_CB_ARG arg);

  virtual T_CB       EncryptCbCreate(TipcMt& mt,
                                     T_IPC_CONN_ENCRYPT_CB_FUNC func,
                                     T_CB_ARG arg);


  virtual T_CB       EncryptCbLookup(TipcMt& mt,
                                     T_IPC_CONN_ENCRYPT_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_CB       DecryptCbCreate(TipcMt& mt,
                                     T_IPC_CONN_DECRYPT_CB_FUNC func,
                                     T_CB_ARG arg);


  virtual T_CB       DecryptCbLookup(TipcMt& mt,
                                     T_IPC_CONN_DECRYPT_CB_FUNC func,
                                     T_CB_ARG arg);

  virtual T_STR                 UniqueSubject();               // Get it
#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
  virtual T_STR                 UniqueDatagroup();               // Get it
#endif
  virtual T_STR                 Node();                          // Get it
  virtual T_STR                 User();                          // Get it
  virtual T_INT4                Pid();                           // Get it
  virtual T_STR                 Arch();                          // Get it
  virtual T_IPC_SRV_CONN_STATUS ConnStatus();                    // Get it

#ifndef T_SS35_HIERARCHY
  virtual T_BOOL Lock();
  virtual T_BOOL Unlock();

  virtual T_INT4 GmdNumPending();
#endif

  // ----------------------------------------------------------------------
  // The following correspond to TipcSrv calls that have no TipcConn parallel
  // ----------------------------------------------------------------------

  // These functions static to allow calls before we call TipcSrvCreate()
  static T_CB CreateCbCreate(T_IPC_SRV_CREATE_CB_FUNC func,
                             T_CB_ARG arg);

  static T_CB DestroyCbCreate(T_IPC_SRV_DESTROY_CB_FUNC func,
                              T_CB_ARG arg);

  static T_CB CreateCbLookup(T_IPC_SRV_CREATE_CB_FUNC func,
                             T_CB_ARG arg);

  static T_CB DestroyCbLookup(T_IPC_SRV_DESTROY_CB_FUNC func,
                              T_CB_ARG arg);

  // The following functions possibly establish an TipcSrv connection.
  T_BOOL SubjectSubscribe(T_STR subject);                          // Get it
  T_BOOL SubjectSubscribe(T_STR subject, T_BOOL  recv_status);     // Set it

  T_BOOL SubjectRetrieve(T_STR subject);

  static T_PTR SubjectTraverseSubscribe(T_IPC_SRV_DG_TRAV_FUNC func,
                                        T_PTR arg);
#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
  T_BOOL DgRecv(T_STR dg);                              // Get it
  T_BOOL DgRecv(T_STR dg, T_BOOL  recv_status);         // Set it

  T_BOOL DgRetrieve(T_STR dg);

  static T_PTR  DgTraverseRecv(T_IPC_SRV_DG_TRAV_FUNC func,
                               T_PTR arg);
#endif

  // Next function also possibly creates connection to TipcSrv,
  // but won't start it;  we've made it static
  static T_BOOL IsRunning();

  // The message logging functions should be called before TipcSrv conn
  // is created, so they are static too.
  static T_BOOL LogAddMt(T_IPC_SRV_LOG_TYPE log_type,
                         TipcMt& mt);

  static T_BOOL LogRemoveMt(T_IPC_SRV_LOG_TYPE log_type,
                            TipcMt& mt);

  // These could possibly be made nonstatic (see dg calls above)
  static T_BOOL StdSubjectRetrieve(T_BOOL retrieve_time);
  static T_BOOL StdSubjectSetSubscribe(T_BOOL recv_flag, T_BOOL time_flag);
#ifdef T_RTWORKS35_COMPAT /* compat with 3.5 */
  static T_BOOL StdDgRetrieve(T_BOOL retrieve_time);
  static T_BOOL StdDgSetRecv(T_BOOL recv_flag, T_BOOL time_flag);
#endif

  static T_BOOL Stop(T_IPC_SRV_STOP_TYPE stop_type);

  // GMD
  virtual T_BOOL GmdFileCreate();
  virtual T_BOOL GmdFileDelete();
  virtual T_BOOL GmdResend();

  virtual T_BOOL GmdMsgDelete(TipcMsg& msg);
  virtual T_BOOL GmdMsgResend(TipcMsg& msg);

  virtual T_UINT4 GmdMaxSize();
  virtual T_BOOL  GmdMaxSize(T_UINT4 gmd_max_size);

  T_BOOL GmdMsgServerDelete(TipcMsg& msg);
  T_BOOL GmdMsgStatus(TipcMsg& msg);

#ifndef T_SS35_HIERARCHY
  T_BOOL SubjectSubscribeLb(T_STR subject, // Get it
                            T_BOOL *lb_status_return);
  T_BOOL SubjectSubscribeLb(T_STR subject, // Set it
                            T_BOOL subject_status,
                            T_BOOL lb_status);
  T_BOOL SubjectGmdInit(T_STR subject);
  T_BOOL SubjectLbInit(T_STR subject);

  /* subject callback stuff */
  T_CB SubjectCbCreate(T_STR subject,
                              T_IPC_MT mt,
                              T_IPC_SRV_SUBJECT_CB_FUNC func,
                              T_CB_ARG arg);
  T_CB SubjectCbLookup(T_STR subject,
                              T_IPC_MT mt,
                              T_IPC_SRV_SUBJECT_CB_FUNC func,
                              T_CB_ARG arg);
  T_CB SubjectDefaultCbCreate(T_IPC_SRV_SUBJECT_CB_FUNC func,
                                     T_CB_ARG arg);
  T_CB SubjectDefaultCbLookup(T_IPC_SRV_SUBJECT_CB_FUNC func,
                                     T_CB_ARG arg);
  T_BOOL SubjectCbDestroyAll();
#endif
};

#endif
