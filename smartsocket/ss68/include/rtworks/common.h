/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/common.h#2 $
 */

/* =================================================================== */
/*                                                                     */
/*                  common.h -- common header file                     */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_COMMON_H_
#define _RTWORKS_COMMON_H_

#if defined(T_OS_ALPHA_OSF_40)
typedef unsigned long socklen_t;
#endif

/*
 * Bug 5968
 */
#if defined(PT_KIT)
#if !defined(T_NO_LICENSE_FOR_TESTING)
# define T_NO_LICENSE_FOR_TESTING
#endif
#endif

/*
 * config.h must be first include file
 */
#include <rtworks/config.h>

/*
 * Version-number defines for SmartSockets.
 */
#define T_RTWORKS_VERSION 680 /* version times 100 */

#define T_RTWORKS_REVISION 0      /* deprecated - use T_RTWORKS_VERSION % 10 */

/* -------------------------- */
/* Include OS-specific items. */
/* -------------------------- */
#if !defined(T_GENERATE_PROTOTYPES)
# if defined(T_OS_WINDOWS)
#  include <rtworks/win32/wininc.h>
# elif defined(T_OS_VMS)
#  include <rtworks/vmsinc.h>            /* VMS header files */
# elif defined(T_OS_UNIX)
#  include <rtworks/unix/unixinc.h>      /* UNIX header files */
#  if defined(T_OS_OSS)
#   include <rtworks/unix/ossinc.h>
#  endif
# else
  common.h: unknown machine type
# endif
#endif /* !defined(T_GENERATE_PROTOTYPES) */


/*
 * Support for Unicode - UTF conversions
 */
#ifdef T_ANSI_C
typedef wchar_t T_WCHAR;
typedef wchar_t *T_WSTR;
#else
typedef T_UINT2 T_WCHAR;
typedef T_UINT2 *T_WSTR;
#endif

#include <rtworks/modules/type.h> /* basic type information             */
#include <rtworks/debug.h>        /* debugging macros                   */
#include <rtworks/util.h>         /* useful utilities                   */
#include <rtworks/error.h>        /* error handling                     */
#include <rtworks/list.h>         /* generic linked lists               */
#include <rtworks/stack.h>        /* linked-list stacks                 */
#include <rtworks/queue.h>        /* linked-list queue                  */
#include <rtworks/deque.h>        /* double-ended queues                */
#include <rtworks/ptrstk.h>       /* linked-list of pointer stacks      */
#include <rtworks/ptrq.h>         /* linked-list of pointer queues      */
#include <rtworks/ptrdq.h>        /* linked-list of pointer deques      */
#include <rtworks/bppq.h>         /* boundaried priority pointer queues */
#include <rtworks/hash.h>         /* hash tables                        */
#include <rtworks/callback.h>     /* callback header                    */
#include <rtworks/lex.h>          /* lexical analyzer                   */
#include <rtworks/modules/ring.h> /* ring buffer typedefs               */
#include <rtworks/threads.h>      /* multi-threading & synchronization  */
#include <rtworks/utatomic.h>     /* macros for atomic functions        */
#include <rtworks/fastmtex.h>     /* fast mutexes                       */
#include <rtworks/shrq.h>         /* shared queues                      */
#include <rtworks/mempool.h>      /* memory pools                       */
#include <rtworks/utmd5.h>        /* md5 interface                      */
#include <rtworks/utlic.h>        /* licensing items                    */
#include <rtworks/xml.h>          /* xml interface                      */
#include <rtworks/uttrace.h>      /* trace interface                    */
#include <rtworks/utcat.h>        /* catalog                            */
#include <rtworks/props.h>        /* properties                         */

/*
 * T_PTR is defined in <arph/nameser.h> (see bug 4131).
 */
#ifdef T_PTR
#undef T_PTR
#endif

/*
 * T_NULL is defined in <arph/nameser.h> (see bug 4131).
 */
#ifdef T_NULL
#undef T_NULL
#endif

#include <rtworks/sockinc.h>

/*
 * T_PTR is defined in <arph/nameser.h> (see bug 4131).
 */
#ifdef T_PTR
#undef T_PTR
#endif

/*
 * T_NULL is defined in <arph/nameser.h> (see bug 4131).
 */
#ifdef T_NULL
#undef T_NULL
#endif

#define T_NULL ((T_PTR)0)

#ifdef T_OS_VMS
/*
 * VMS limits identifier names to 31 characters.
 */
#define TutOptionGetUniqueSubjectDefault  TutOptionGetUniqueSubjectDefaul
#define TutFuncSetArgValueByPosToUnknown  TutFuncSetArgValueByPosToUnknow
#define TutLicCalcExternalSerialNumberChecksum TutLicCalcExternalSerialNumberC
#define TutLicCalcInternalSerialNumberChecksum TutLicCalcInternalSerialNumberC
#define TutTraceLevelDefaultOptionCreate  TutTraceLevelDefaultOptionCreat
#define TutTraceLevelDefaultOptionLookup  TutTraceLevelDefaultOptionLooku
#define TutTraceLevelDefaultOptionDestroy TutTraceLevelDefaultOptionDestr
#define TutTraceFileDefaultOptionDestroy  TutTraceFileDefaultOptionDestro
#define TutTraceFlagsDefaultOptionCreate  TutTraceFlagsDefaultOptionCreat
#define TutTraceFlagsDefaultOptionLookup  TutTraceFlagsDefaultOptionLooku
#define TutTraceFlagsDefaultOptionDestroy TutTraceFlagsDefaultOptionDestr

#define TutFuncSetArgValueByPosToUnknown  TutFuncSetArgValueByPosToUnknow

#define TmsgMtGetEncryptInfoByNumInternal TmsgMtGetEncryptInfoByNumIntern
#define TmsgFieldUpdateTimestampArrayPtr  TmsgFieldUpdateTimestampArrayPt
#define TmsgUpdateNamedTimestampArrayPtr  TmsgUpdateNamedTimestampArrayPt

#define TipcLinkDriverSetConnectBeginFunc TipcLinkDriverSetConnectBeginFu
#define TipcLinkDriverSetConnectCompleteFunc TipcLinkDriverSetConnectComplet
#define TipcLinkSocketGetMultiplexSource TipcLinkSocketGetMultiplexSourc

#define TsmAuthenticateCompletionDataCreate  TsmAuthenticateCompletionDataCr
#define TsmAuthenticateCompletionDataDestroy TsmAuthenticateCompletionDataDe
#define TsmAuthorizeCompletionDataCreate     TsmAuthorizeCompletionDataCreat
#define TsmAuthorizeCompletionDataDestroy    TsmAuthorizeCompletionDataDestr

#define TipcCbBurstConnProcessGmdFailure  TipcCbBurstConnProcessGmdFailur
#define TipcConnWriteBufferChangeCbCreate TipcConnWriteBufferChangeCbCrea
#define TipcConnWriteBufferChangeCbLookup TipcConnWriteBufferChangeCbLook
#define TipcMonMaxClientLicensesGetWatch  TipcMonMaxClientLicensesGetWatc
#define TipcMonServerMaxClientLicensesSetWatch TipcMonServerMaxClientLicensesS
#define TipcMsgAddNamedTimestampArrayPtr  TipcMsgAddNamedTimestampArrayPt
#define TipcMsgGetCurrentFieldRealFormat  TipcMsgGetCurrentFieldRealForma
#define TipcMsgGetCurrentFieldCharFormat  TipcMsgGetCurrentFieldCharForma
#define TipcMsgFieldUpdateReal16ArrayPtr  TipcMsgFieldUpdateReal16ArrayPt
#define TipcMsgFieldUpdateTimestampArrayPtr TipcMsgFieldUpdateTimestampArra
#define TipcMsgUpdateNamedReal16ArrayPtr  TipcMsgUpdateNamedReal16ArrayPt
#define TipcMsgCheckIfEncryptionRequired  TipcMsgCheckIfEncryptionRequire
#define TipcMsgCheckIfDecryptionRequired  TipcMsgCheckIfDecryptionRequire
#define TipcMtGetEncryptInfoByNumInternal TipcMtGetEncryptInfoByNumIntern
#define TipcSrvConnCacheRequestLastValue  TipcSrvConnCacheRequestLastValu
#define TrmsClientSessionSetDurableSubscribe   TrmsClientSessionSetDurableSubs
#define TrmsClientSessionUnsetDurableSubscribe TrmsClientSessionUnsetDurableSu
#define TipcSrvConnSubjectGetSubscribeLb  TipcSrvConnSubjectGetSubscribeL
#define TipcSrvConnSubjectSetSubscribeEx  TipcSrvConnSubjectSetSubscribeE
#define TipcSrvConnSubjectSetSubscribeLb  TipcSrvConnSubjectSetSubscribeL
#define TipcSrvConnSubjectTraverseSubscribe TipcSrvConnSubjectTraverseSubsc
#define TipcSrvConnSubjectDefaultCbCreate TipcSrvConnSubjectDefaultCbCrea
#define TipcSrvConnSubjectDefaultCbLookup TipcSrvConnSubjectDefaultCbLook
#define TipcSrvConnStdSubjectSetSubscribe TipcSrvConnStdSubjectSetSubscri
#define TipcSrvConnWriteBufferChangeCbCreate TipcSrvConnWriteBufferChangeCbC
#define TipcSrvConnWriteBufferChangeCbLookup TipcSrvConnWriteBufferChangeCbL
#define TipcSrvConnGetKeepAliveBlockMode  TipcSrvConnGetKeepAliveBlockMod
#define TipcSrvConnSetKeepAliveBlockMode  TipcSrvConnSetKeepAliveBlockMod
#define TipcSrvConnSubjectSetDefaultPrefix TipcSrvConnSubjectSetDefaultPre
#define TipcSrvConnSubjectGetDefaultPrefix TipcSrvConnSubjectGetDefaultPre
#define TipcSrvConnSubjectAddDefaultPrefix TipcSrvConnSubjectAddDefaultPre
#define TipcSessionObjectGetMultiplexMode  TipcSessionObjectGetMultiplexMo
#define TipcSrvMonSetIdentStrFromLicense   TipcSrvMonSetIdentStrFromLicens
#define TipcSrvMonSubjectSubscribeGetWatch TipcSrvMonSubjectSubscribeGetWa
#define TipcSrvMonSubjectSubscribeSetWatch TipcSrvMonSubjectSubscribeSetWa
#define TipcSrvMonGetSubjectSubscribeTable TipcSrvMonGetSubjectSubscribeTa
#define TipcSrvMonClientSubscribeGetWatch  TipcSrvMonClientSubscribeGetWat
#define TipcSrvMonClientSubscribeSetWatch  TipcSrvMonClientSubscribeSetWat
#define TipcSrvMonGetClientSubscribeTable  TipcSrvMonGetClientSubscribeTab
#define TipcSrvMonClientCongestionGetWatch TipcSrvMonClientCongestionGetWa
#define TipcSrvMonClientCongestionSetWatch TipcSrvMonClientCongestionSetWa
#define TipcSrvMonServerCongestionGetWatch TipcSrvMonServerCongestionGetWa
#define TipcSrvMonServerCongestionSetWatch TipcSrvMonServerCongestionSetWa
#define TipcSrvMonClientSubscribeNumPoll   TipcSrvMonClientSubscribeNumPol
#define TipcSubjectTableTraverseInternal   TipcSubjectTableTraverseInterna

#define TspcPPServerAcceptNetworkConnects  TspcPPServerAcceptNetworkConnec
#define TspcPPPeerGetNumMsgQuantaExpiries  TspcPPPeerGetNumMsgQuantaExpiri
#define TspcPPPeerGetNumTimeQuantaExpiries TspcPPPeerGetNumTimeQuantaExpir

#define TsvcClientHandshakeCheckNComplete  TsvcClientHandshakeCheckNComple
#define TsvcGroupHandshakeCheckNComplete   TsvcGroupHandshakeCheckNComplet
#define TsvcServerHandshakeCheckNComplete  TsvcServerHandshakeCheckNComple
#define TsvcUnknownHandshakeCheckNComplete TsvcUnknownHandshakeCheckNCompl

#define TsmrServerSendAckDestroyUniqueSubjectCheck TsmrServerSendAckDestroyUniqueS
#define TsmrCbClientBatchSubjectSetSubscribe   TsmrCbClientBatchSubjectSetSubs
#define TsmrServerSendAckDestroyCollectivePoll TsmrServerSendAckDestroyCollect
#define TsmrCbClientMonClientNamesSetWatch     TsmrCbClientMonClientNamesSetWa
#define TsmrCbClientMonServerNamesSetWatch     TsmrCbClientMonServerNamesSetWa
#define TsmrCbClientMonMaxClientLicensesSetWatch TsmrCbClientMonMaxClientLicense
#define TsmrCbClientMonSubjectNamesSetWatch     TsmrCbClientMonSubjectNamesSetW
#define TsmrCbClientMonClientSubscribeSetWatch  TsmrCbClientMonClientSubscribeS
#define TsmrCbClientMonClientTimeSetWatch       TsmrCbClientMonClientTimeSetWat
#define TsmrCbClientMonClientCongestionSetWatch TsmrCbClientMonClientCongestion
#define TsmrCbClientMonServerCongestionSetWatch TsmrCbClientMonServerCongestion
#define TsmrCbClientMonClientBufferSetWatch     TsmrCbClientMonClientBufferSetW
#define TsmrCbClientMonClientMsgRecvSetWatch    TsmrCbClientMonClientMsgRecvSet
#define TsmrCbClientMonClientMsgSendSetWatch    TsmrCbClientMonClientMsgSendSet
#define TsmrCbClientMonServerConnSetWatch       TsmrCbClientMonServerConnSetWat
#define TsmrCbClientMonProjectNamesPollCall     TsmrCbClientMonProjectNamesPoll
#define TsmrCbClientMonClientExtPollCall        TsmrCbClientMonClientExtPollCal
#define TsmrCbClientMonClientGeneralPollCall    TsmrCbClientMonClientGeneralPol
#define TsmrCbClientMonClientInfoPollCall       TsmrCbClientMonClientInfoPollCa
#define TsmrCbClientMonClientTimePollCall       TsmrCbClientMonClientTimePollCa
#define TsmrCbClientMonClientCpuPollCall        TsmrCbClientMonClientCpuPollCal
#define TsmrCbClientMonClientBufferPollCall     TsmrCbClientMonClientBufferPoll
#define TsmrCbClientMonClientOptionPollCall     TsmrCbClientMonClientOptionPoll
#define TsmrCbClientMonClientMsgTypePollCall    TsmrCbClientMonClientMsgTypePol
#define TsmrCbClientMonClientMsgTrafficPollCall TsmrCbClientMonClientMsgTraffic
#define TsmrCbClientMonClientSubjectPollCall    TsmrCbClientMonClientSubjectPol
#define TsmrCbClientMonServerNamesPollCall      TsmrCbClientMonServerNamesPollC
#define TsmrCbClientMonServerGeneralPollCall    TsmrCbClientMonServerGeneralPol
#define TsmrCbClientMonServerTimePollCall       TsmrCbClientMonServerTimePollCa
#define TsmrCbClientMonServerCpuPollCall        TsmrCbClientMonServerCpuPollCal
#define TsmrCbClientMonServerBufferPollCall     TsmrCbClientMonServerBufferPoll
#define TsmrCbClientMonServerOptionPollCall     TsmrCbClientMonServerOptionPoll
#define TsmrCbClientMonServerConnPollCall       TsmrCbClientMonServerConnPollCa
#define TsmrCbClientMonServerRoutePollCall      TsmrCbClientMonServerRoutePollC
#define TsmrCbClientMonServerMsgTrafficPollCall TsmrCbClientMonServerMsgTraffic
#define TsmrCbClientMonClientNamesPollCall      TsmrCbClientMonClientNamesPollC
#define TsmrCbClientMonClientNamesNumPollCall   TsmrCbClientMonClientNamesNumPo
#define TsmrCbClientMonSubjectNamesPollCall     TsmrCbClientMonSubjectNamesPoll
#define TsmrCbClientMonClientSubscribePollCall  TsmrCbClientMonClientSubscribeP
#define TsmrCbClientMonClientSubscribeNumPollCall TsmrCbClientMonClientSubscribeN
#define TsmrCbClientMonProjectNamesSetWatch     TsmrCbClientMonProjectNamesSetW
#define TsmrCbClientAdminSetOutboundRateParams  TsmrCbClientAdminSetOutboundRat
#define TsmrPublishClientSendAckLookupBySeqNum  TsmrPublishClientSendAckLookupB
#define TsmrGmdClientSubjectSetSubscribe        TsmrGmdClientSubjectSetSubscrib
#define TsmrCreateServerCongestionWatchKey      TsmrCreateServerCongestionWatch
#define TsmrCbMonServerCongestionWatchServerCreate TsmrCbMonServerCongestionWatchS
#define TsmrCbMonServerCongestionWatchGroupCreate  TsmrCbMonServerCongestionWatchG
#define TsmrCbMonServerCongestionWatchClientCreate TsmrCbMonServerCongestionWatchC
#define TsmrCbMonClientNamesClientCreateRemote     TsmrCbMonClientNamesClientCreat
#define TsmrCbMonClientNamesClientDestroyRemote    TsmrCbMonClientNamesClientDestr
#define TsmrCbMonSubjectNamesSubjectSubscribeRemote TsmrCbMonSubjectNamesSubjectSub
#define TsmrCbMonSubjectNamesSubjectCreateRemote   TsmrCbMonSubjectNamesSubjectCre
#define TsmrCbMonSubjectNamesSubjectDestroyRemote  TsmrCbMonSubjectNamesSubjectDes
#define TsmrCbMonSubjectSubscribeSubjectSubscribeRemote TsmrCbMonSubjectSubscribeSubjec
#define TsmrCbMonSubjectSubscribeCleanup           TsmrCbMonSubjectSubscribeCleanu
#define TsmrCbMonClientSubscribeClientSubscribeRemote TsmrCbMonClientSubscribeClientS
#define TsmrCbMonClientCongestionCleanup  TsmrCbMonClientCongestionCleanu
#define TsmrCbMonServerCongestionCleanup  TsmrCbMonServerCongestionCleanu
#define TsmrCbMonMaxClientLicensesCleanup TsmrCbMonMaxClientLicensesClean
#define TsmrMonSubjectNamesSubjectTableTf TsmrMonSubjectNamesSubjectTable
#define TsmrMonSubjectSubscribeSubjectTableTf TsmrMonSubjectSubscribeSubjectT
#define TsmrMonPrimeSubjectSubscribeStatus  TsmrMonPrimeSubjectSubscribeSta
#define TsmrMonSendClientMsgSendSetWatch    TsmrMonSendClientMsgSendSetWatc
#define TsmrMonSendSubjectSubscribeStatus   TsmrMonSendSubjectSubscribeStat
#define TsmrMonPrimeClientSubscribeStatus   TsmrMonPrimeClientSubscribeStat
#define TsmrMonSendClientMsgRecvSetWatch    TsmrMonSendClientMsgRecvSetWatc
#define TsmrMonMaxClientLicensesGetWatch    TsmrMonMaxClientLicensesGetWatc
#define TsmrMonMaxClientLicensesSetWatch    TsmrMonMaxClientLicensesSetWatc
#define TsmrMonSendClientCongestionSetWatch TsmrMonSendClientCongestionSetW
#define TsmrMonSendServerCongestionIoctl    TsmrMonSendServerCongestionIoct
#define TsmrMonSendServerCongestionStatus   TsmrMonSendServerCongestionStat
#define TsmrServerMaxClientLicensesCbCreate TsmrServerMaxClientLicensesCbCr
#define TsmrServerMaxClientLicensesCbCall   TsmrServerMaxClientLicensesCbCa
#define TsmrServerMaxClientLicensesCbLookup TsmrServerMaxClientLicensesCbLo
#define TsmrServerGetServerDirectAcceptCount TsmrServerGetServerDirectAccept
#define TsmrServerGetServerDirectConnectCount TsmrServerGetServerDirectConnec
#define TsmrServerGetServerIndirectCount    TsmrServerGetServerIndirectCoun
#define TsmrServerSrvGmdInitDeleteResult    TsmrServerSrvGmdInitDeleteResul
#define TsmrServerSrvGmdClientSubjectSetSubscribe TsmrServerSrvGmdClientSubjectSe
#define TsmrServerSendAckDestroyGmdSrvInit  TsmrServerSendAckDestroyGmdSrvI
#define TsmrGmdPublishSubjectUpdateTimeout  TsmrGmdPublishSubjectUpdateTime
#define TsmrCbMonServerGeneralPollResult    TsmrCbMonServerGeneralPollResul
#define TsmrCbMonServerMsgTrafficPollResult TsmrCbMonServerMsgTrafficPollRe
#define TsmrCbMonClientNamesNumPollResult   TsmrCbMonClientNamesNumPollResu
#define TsmrCbMonSubjectSubscribePollResult TsmrCbMonSubjectSubscribePollRe
#define TsmrCbMonClientSubscribePollResult  TsmrCbMonClientSubscribePollRes
#define TsmrCbMonClientSubscribeNumPollResult TsmrCbMonClientSubscribeNumPoll
#define TsmrCbServerSrvGmdClientSubjectSetSubscribe TsmrCbServerSrvGmdClientSubject
#define TsmrCbServer65SrvGmdInitAddResult   TsmrCbServer65SrvGmdInitAddResu
#define TsmrCbServer65SrvGmdInitDeleteResult TsmrCbServer65SrvGmdInitDeleteR
#define TsmrCbServerServerSubjectSetSubscribe TsmrCbServerServerSubjectSetSub
#define TsmrCbServer67UniqueSubjectCheckCall  TsmrCbServer67UniqueSubjectChec
#define TsmrCbServerUniqueSubjectCheckCall    TsmrCbServerUniqueSubjectCheckC
#define TsmrCbServerUniqueSubjectCheckResult  TsmrCbServerUniqueSubjectCheckR
#define TsmrCbServerMonClientNamesSetWatch    TsmrCbServerMonClientNamesSetWa
#define TsmrCbServerMonClientNamesStatus      TsmrCbServerMonClientNamesStatu
#define TsmrCbServerMonSubjectNamesSetWatch   TsmrCbServerMonSubjectNamesSetW
#define TsmrCbServerMonSubjectNamesStatus     TsmrCbServerMonSubjectNamesStat
#define TsmrCbServerMonClientTimeSetWatch       TsmrCbServerMonClientTimeSetWat
#define TsmrCbServerMonClientCongestionSetWatch TsmrCbServerMonClientCongestion
#define TsmrCbServerMonClientBufferSetWatch     TsmrCbServerMonClientBufferSetW
#define TsmrCbServerMonClientMsgRecvSetWatch    TsmrCbServerMonClientMsgRecvSet
#define TsmrCbServerMonClientMsgSendSetWatch    TsmrCbServerMonClientMsgSendSet
#define TsmrCbServerMonCollectivePollCall       TsmrCbServerMonCollectivePollCa
#define TsmrCbServerMonCollectivePollResult     TsmrCbServerMonCollectivePollRe
#define TsmrCbServerMonServerGeneralPollCall    TsmrCbServerMonServerGeneralPol
#define TsmrCbServerMonServerTimePollCall       TsmrCbServerMonServerTimePollCa
#define TsmrCbServerMonServerCpuPollCall        TsmrCbServerMonServerCpuPollCal
#define TsmrCbServerMonServerBufferPollCall     TsmrCbServerMonServerBufferPoll
#define TsmrCbServerMonServerOptionPollCall     TsmrCbServerMonServerOptionPoll
#define TsmrCbServerMonServerRoutePollCall      TsmrCbServerMonServerRoutePollC
#define TsmrCbServerMonServerMsgTrafficPollCall TsmrCbServerMonServerMsgTraffic
#define TipcSrvConnGetCurrentConnectedLcn       TipcSrvConnGetCurrentConnectedL
#define TipcSrvMonServerMsgTrafficExPoll        TipcSrvMonServerMsgTrafficExPol
#define TsmrCbClientMonServerStartTimePollCall  TsmrCbClientMonServerStartTimeP
#define TsmrCbClientMonServerVersionPollCall    TsmrCbClientMonServerVersionPol
#define TsmrCbClientMonClientVersionPollCall    TsmrCbClientMonClientVersionPol
#define TsmrCbClientMonClientMsgTypeExPollCall  TsmrCbClientMonClientMsgTypeExP
#define TsmrCbClientMonClientSubjectExPollCall  TsmrCbClientMonClientSubjectExP
#define TsmrCbServerMonServerStartTimePollCall  TsmrCbServerMonServerStartTimeP
#define TsmrCbServerMonServerVersionPollCall    TsmrCbServerMonServerVersionPol
#define TsmrCbMonServerStartTimePollResult      TsmrCbMonServerStartTimePollRes
#define TsmrCbMonServerVersionPollResult        TsmrCbMonServerVersionPollResul

/*
 * These names are greater than 31 characters and are non-unique to 31
 * characters.
 */
#define TsmrCbServerMonServerCongestionSetWatch TsmrCbSrvrMonSrvrCongestionSetW
#define TsmrCbServerMonServerCongestionStatus   TsmrCbSrvrMonSrvrCongestionStat
#define TsmrCbServerMonMaxClientLicensesSetWatch TsmrCbSrvrMonMaxClntLicSetWatch
#define TsmrCbServerMonMaxClientLicensesStatus   TsmrCbSrvrMonMaxClntLicStatus
#define TsmrCbServerMonClientSubscribeSetWatch  TsmrCbSrvrMonClientSubSetWatch
#define TsmrCbServerMonClientSubscribeStatus    TsmrCbSrvrMonClientSubStatus
#define TsmrCbServerMonSubjectSubscribeSetWatch TsmrCbSrvrMonSubjSubSetWatch
#define TsmrCbServerMonSubjectSubscribeStatus   TsmrCbSrvrMonSubjSubStatus
#define TsmrCbClientMonSubjectSubscribeSetWatch TsmrCbClientMonSubjSubSetWatch
#define TsmrCbClientMonSubjectSubscribePollCall TsmrCbClientMonSubjSubPollCall
#define TipcMsgUpdateNamedTimestampArray        TipcMsgUpdateNamedTimestpArray
#define TipcMsgUpdateNamedTimestampArrayPtr     TipcMsgUpdateNamedTimestpArrPtr
#define TipcCbSrvProcessSubscribeResult2        TipcCbSrvProcessSubscribeRslt2
#define TipcSrvConnSubjectBatchSubscribe        TipcSrvConnSubjBatchSubscribe
#define TipcSrvConnSubjectBatchSubscribeLb      TipcSrvConnSubjBatchSubscribeLb
#define TipcSrvMonServerMaxClientLicensesGetWatch TipcSrvMonSrvrMaxClntLicGetWatc
#define TipcSrvMonServerMaxClientLicensesSetWatch TipcSrvMonSrvrMaxClntLicSetWatc
#define TsmrCbClientMonServerMsgTrafficExPollCall TsmrCbClntMonServerMsgTrafExPol
#define TsmrCbServerMonServerMsgTrafficExPollCall TsmrCbSrvrMonServerMsgTrafExPol
#define TsmrCbMonServerMsgTrafficExPollResult     TsmrCbMonSrvrMsgTrafficExPolRes
#endif

#include <rtworks/utpr.h>    /* util prototypes */
#ifdef T_OS_VMS
#include <rtworks/vmsutpr.h>
#endif
#ifdef T_OS_WINDOWS
#include <rtworks/win32/winutpr.h>
#endif

/* -------------------------------------------------------- */
/* Turn on the printf style error checking provided by gcc. */
/* -------------------------------------------------------- */
#if (__GNUC__ == 2 && ! defined(__cplusplus))
extern void TutCommandPrintFeedback (T_STR fmt, ...)
  __attribute__ ((format (printf, 1, 2)));
extern void TutDumpOut (T_STR fmt, ...)
  __attribute__ ((format (printf, 1, 2)));
extern void TutLexError (T_LEX lexer, T_STR fmt, ...)
  __attribute__ ((format (printf, 2, 3)));
extern void TutOut(T_STR fmt, ...)
  __attribute__ ((format (printf, 1, 2)));
extern void TutFatalError (T_CONST T_STR fmt, ...)
  __attribute__ ((format (printf, 1, 2)));
extern void TutWarning (T_STR fmt, ...)
  __attribute__ ((format (printf, 1, 2)));
extern void TutDebug(T_STR fmt, ...)
  __attribute__ ((format (printf, 1, 2)));
#endif

/* ---------------------------------------------------- */
/* This silences lint about pointer alignment problems. */
/* ---------------------------------------------------- */
#ifdef lint
#define TutHashLookup(table, key) 0
#define TutHashDelete(table, key) 0
#endif

/* ----------------------------------*/
/* Turn off licensing at build time. */
/* ----------------------------------*/
#define T_LICENSE_EVAL_DAYS 30
#define T_LICENSE_KEY_VERSION 1      /* only goes to 255 (one byte) */

#if defined(T_LICENSE_EVAL)
#define T_LM_NONE
#define T_LICENSE_MAX_CLIENTS 10.0
#define T_LICENSE_MAX_SERVERS 1.0
#else
#if defined(T_LICENSE_SMALL)
#define T_LM_NONE
#define T_LICENSE_MAX_CLIENTS 10.0
#define T_LICENSE_MAX_SERVERS 1.0
#else
#if defined(T_LICENSE_MEDIUM)
#define T_LM_NONE
#define T_LICENSE_MAX_CLIENTS 20.0
#define T_LICENSE_MAX_SERVERS 5.0
#else
#if defined(T_LICENSE_BIG)
#define T_LM_NONE
#define T_LICENSE_MAX_CLIENTS 0.0    /* unlimited */
#define T_LICENSE_MAX_SERVERS 0.0
#else
#undef T_LM_NONE
#define T_LICENSE_MAX_CLIENTS 0.0
#define T_LICENSE_MAX_SERVERS 0.0
#endif
#endif
#endif
#endif

#endif /* _RTWORKS_COMMON_H_ */
