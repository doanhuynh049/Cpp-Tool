/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tiof/biof/dev/68/src/rtworks/link.h#2 $
 */

/* =================================================================== */
/*                                                                     */
/*               link.h -- peer-to-peer communication link             */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_LINK_H__
#define _RTWORKS_LINK_H__

#include <rtworks/ioctl.h>

/*
 * Changed the name of function TipcWinLinkSocketSetMultiplex to
 * TipcLinkWinSocketSetMultiplex.  This is needed so that mkaixexp.sh can
 * export this function name on AIX.  mkaixexp.sh searches for "ipcL" in the
 * function name.  To make things simpler, I am defining the old function name
 * as the new one.
 */
#if defined (T_OS_AIX)
#define TipcWinLinkSocketSetMultiplex TipcLinkWinSocketSetMultiplex
#endif

/*
 * Link properties.
 */
#define T_LINK_PROP_BROADCAST "BROADCAST"      /* Broadcast protocol (true/false) */
#define T_LINK_PROP_PEER_ADDR "link.peer_addr" /* Peer IP Address / Port (for non-broadcast) */
#define T_LINK_PROP_PROTOCOL  "link.protocol"  /* Link protocol */
#define T_LINK_PROP_ADDR      "link.addr"      /* Local IP Address / Port (for non-broadcast) */

/*
 * SSL Specific link properties
 */

#define T_LINK_PROP_PEER_CERT "link.peer_cert" /* Peer Certificate */
#define T_LINK_PROP_PEER_DN   "link.peer_dn"   /* Peer DN */

#define T_LINK_PROP_CERT      "link.cert"      /* Local Certificate */
#define T_LINK_PROP_DN        "link.dn"        /* Local DN */

/*
 * Link IOCTL commands.
 */
#define T_LINK_IOCTL_EXEC_CLOSE "CLOSE"

/*
 * Properties component
 */
typedef struct T_IPC_LINK_PROPS_STRUCT
               T_IPC_LINK_PROPS_STRUCT,
              *T_IPC_LINK_PROPS;

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_PROPS_TRAV_FUNC)
T_TYPEDEF((
           T_PTR,     /* key   */
           T_PTR,     /* queue */
           T_PTR      /* arg   */
           ));

/*
 * VMS function names must be 31 characters or less.  We defined the
 * short names here and in ipc.h.  KEL - 5/28/2000
 */
#if 0 /* Put these in common.h JWA 2004/02/19 */
#ifdef T_OS_VMS
#define TipcLinkDriverSetConnectBeginFunc TipcLinkDriverSetConnectBeginF
#define TipcLinkDriverSetConnectCompleteFunc TipcLinkDriverSetConnectComple
#define TipcLinkSocketGetMultiplexSource TipcLinkSocketGetMultiplexSourc
#endif
#endif

/*
 * T_IPC_LINK_NAME_SEP is the separator for logical link names (which have
 * the form protocol:node:address).
 *
 * We use T_IPC_LINK_NAME_SEP in both string and char comparision
 * functions. Using T_IPC_LINK_NAME_SEP in a call to strchr as
 * strchr(foo, T_IPC_LINK_NAME_SEP[0]) confused some compilers
 * (like Borland on Win16), so T_IPC_LINK_NAME_SEP_CHR was added
 * and the calls to strchr were changed.  dlc  8-13-96
 */
#define T_IPC_LINK_NAME_SEP ":"
#define T_IPC_LINK_NAME_SEP_CHR ':'

/*
 * T_IPC_LINK_PROPS_SEP is the separator for properties component found in
 * logical link names (which have the form protocol:node:address?properties).
 */
#define T_IPC_LINK_PROPS_SEP "?"
#define T_IPC_LINK_PROPS_SEP_CHR '?'

/*
 * T_IPC_LINK_THIS_NODE is the special value for the node portion of a
 * logical link name.
 */
#define T_IPC_LINK_THIS_NODE "_node"

/*
 * T_IPC_LINK_DRIVER_<NAME> are the names of all the standard link drivers.
 */
#define T_IPC_LINK_DRIVER_DECNET "decnet"
#define T_IPC_LINK_DRIVER_LOCAL "local"
#define T_IPC_LINK_DRIVER_TCP "tcp"
#define T_IPC_LINK_DRIVER_SNA "sna"
#define T_IPC_LINK_DRIVER_UDP_BROADCAST "udp_broadcast"

/*
 * T_IPC_LINK_DEFAULT_<MODULE> are the names of the programs that have
 * default addresses with the standard link drivers.
 */
#define T_IPC_LINK_DEFAULT_RTLM "RTlm"
#define T_IPC_LINK_DEFAULT_RTSERVER "RTserver"
#ifdef T_MODIFY_PT
#define T_IPC_LINK_DEFAULT_RTBROKER "RTbroker"
#endif
#define T_IPC_LINK_DEFAULT_MQSERVER "SmartQueue"
#define T_IPC_LINK_DEFAULT_RTGMS "RTgms"

/*
 * The values of the default addresses for T_IPC_LINK_DEFAULT_<MODULE>.
 *
 * For 4.0 we want to find a neutral set of default addresses that we
 * will support forever (we have committed to maintain protocol
 * compatibility).  "TCP/IP Illustrated, Volume 1" by Stevens says
 * (p. 13), "The port numbers above 5000 are intended for other
 * servers (those that aren't well known across the Internet)", and
 * thus I picked 5100 as our starting point.  SMW 04/29/96
 *
 */
#define T_IPC_LINK_SRV_DEFAULT_NAME "RTSERVER" /* for decnet and local */
#define T_IPC_LINK_SRV_DEFAULT_PORT 5101 /* for tcp and udp_broadcast */
#define T_IPC_LINK_SRV_DEFAULT_PORT_SSL 5102 /* for ssl */
#define T_IPC_LINK_SRV_DEFAULT_PORT_SSL_STR "5102" /* for ssl */
#define T_IPC_LINK_SRV_DEFAULT_PORT_PGM 5104  /* for tcp connection to group manager server (GMS) */
#define T_IPC_LINK_SRV_DEFAULT_PORT_PGM_STR "5104"
#define T_IPC_LINK_SRV_DEFAULT_PORT_HTTP 8080 /* for http */
#define T_IPC_LINK_SRV_DEFAULT_PORT_HTTP_STR "8080" /* for http */
#define T_IPC_LINK_SRV_DEFAULT_PORT_HTTPS 8081 /* for https */
#define T_IPC_LINK_SRV_DEFAULT_PORT_HTTPS_STR "8081" /* for https */

#define T_IPC_LINK_SRV_DEFAULT_PORT_NONE "none" /* no tcp conn to group manager server (GMS) */
#define T_IPC_LINK_LM_DEFAULT_NAME "RTLM" /* for decnet and local */
#define T_IPC_LINK_LM_DEFAULT_PORT 5111 /* for tcp and udp_broadcast */

#ifdef T_MODIFY_PT
#define T_IPC_LINK_BRK_DEFAULT_NAME "RTBROKER" /* for decnet and local */
#define T_IPC_LINK_BRK_DEFAULT_PORT 5103 /* for tcp and udp_broadcast */
#endif
#define T_IPC_LINK_MQS_DEFAULT_NAME "SMARTQUEUE" /* for local */
#define T_IPC_LINK_MQS_DEFAULT_PORT 2493       /* for tcp (and udp) */

/*
 * T_IPC_LINK_MULTIPLEX_MODE is the link multiplex mode (used by
 * T_IPC_SESSION).
 */
typedef enum {
  T_IPC_LINK_MULTIPLEX_SELECT, /* select()-able object */
  T_IPC_LINK_MULTIPLEX_WIN_MSG, /* multiplex via Windows HWND and msg queue */
  T_IPC_LINK_MULTIPLEX_THREAD_LINK, /* one thread per link */
  T_IPC_LINK_MULTIPLEX_THREAD_DRIVER, /* one thread per driver */
  T_IPC_LINK_MULTIPLEX_SEMAPHORE, /* multiplex via a semaphore */
  T_IPC_LINK_MULTIPLEX_SNA,      /* SNA or CPIC avail             */
  T_IPC_LINK_MULTIPLEX_ITSELECT, /* Interlink TCP mvsselect avail */
  T_IPC_LINK_MULTIPLEX_POLL, /* last resort */
  T_IPC_LINK_MULTIPLEX_TIMEOUT, /* a multiplex timeout element */
  T_IPC_LINK_MULTIPLEX_EVENT /* a multiplex event element */
} T_IPC_LINK_MULTIPLEX_MODE;

/*
 * T_IPC_LINK_WIN_MSG_*_READY -- Network event message types
 */
#define T_IPC_LINK_WIN_MSG_WSA_READY   0x7000  /* Windows Socket API (winsock) */
#define T_IPC_LINK_WIN_MSG_SPX_READY   0x7001  /* Windows SPX API */
#define T_IPC_LINK_WIN_MSG_SNA_READY   0x7002  /* Windows SNA API */

/*
 * T_IPC_LINK_DRIVER is for managing protocols for communication links.
 */
typedef struct T_IPC_LINK_DRIVER_STRUCT T_IPC_LINK_DRIVER_STRUCT,
  *T_IPC_LINK_DRIVER;

/*
 * T_IPC_LINK is a socket-like endpoint of a communication link.  It can
 * be thought of as an instance of a link driver.
 */
typedef struct T_IPC_LINK_STRUCT T_IPC_LINK_STRUCT, *T_IPC_LINK;

/*
 * T_IPC_LINK_SOCKET_DEVICE is T_IPC_LINK->device for socket-based drivers.
 */
typedef struct T_IPC_LINK_SOCKET_DEVICE_STRUCT T_IPC_LINK_SOCKET_DEVICE_STRUCT,
  *T_IPC_LINK_SOCKET_DEVICE;
struct T_IPC_LINK_SOCKET_DEVICE_STRUCT {
  T_SOCK socket_fd;
#ifdef T_OS_WINDOWS
  T_INT4 wsa_mask;
#endif
  T_PROPERTIES properties; /*  Link Properties */

};

/*
 * T_IPC_LINK_PROXY_DEVICE is T_IPC_LINK->device for proxy drivers.
 */
typedef struct T_IPC_LINK_PROXY_DEVICE_STRUCT T_IPC_LINK_PROXY_DEVICE_STRUCT,
  *T_IPC_LINK_PROXY_DEVICE;
struct T_IPC_LINK_PROXY_DEVICE_STRUCT {
  T_SOCK socket_fd;
#ifdef T_OS_WINDOWS
  T_INT4 wsa_mask;
#endif
  T_IPC_LINK link;
  T_STR proxy_host;
  T_STR proxy_port;
  T_STR dest_node;
  T_STR dest_addr;
  T_INT4 user_prop;
};

/*
 * T_IPC_LINK_DEFAULT contains a default address (e.g. port 2035) for a
 * specific use (e.g. RTserver).
 */
typedef struct T_IPC_LINK_DEFAULT_STRUCT T_IPC_LINK_DEFAULT_STRUCT,
  *T_IPC_LINK_DEFAULT;

/*
 * Link and link driver operations.
 */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_CREATE_SERVER_FUNC)
T_TYPEDEF((
            T_IPC_LINK /* empty server link whose device needs setting */,
            T_STR /*node*/, /* node portion of logical link name */
            T_STR /*addr*/ /* address portion of logical link name */
            ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_CREATE_CLIENT_FUNC)
T_TYPEDEF((
            T_IPC_LINK /* empty client link whose device needs setting */,
            T_STR /*node*/, /* node portion of logical link name */
            T_STR /*addr*/ /* address portion of logical link name */
            ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_ACCEPT_CLIENT_FUNC)
T_TYPEDEF((
           T_IPC_LINK /*server_link*/,
           T_IPC_LINK /* empty accept link whose device needs setting */
           ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_DESTROY_FUNC)
T_TYPEDEF((
           T_IPC_LINK/*link*/
           ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_CHECK_FUNC)
T_TYPEDEF((
           T_IPC_LINK /*link*/,
           T_IO_CHECK_MODE /*check_mode*/,
           T_REAL8 /*timeout*/ /* max amount of time to block */
           ));

typedef T_INT4 (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_SEND_FUNC)
T_TYPEDEF((
           T_IPC_LINK /*link*/,
           T_PTR /*data*/, /* data to send */
           T_INT4 /*size*/ /* number of bytes to send */
           ));

typedef T_INT4 (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_RECV_FUNC)
T_TYPEDEF((
           T_IPC_LINK /*link*/,
           T_PTR /*data*/, /* buffer to recv into */
           T_INT4 /*size*/ /* number of bytes to recv */
           ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_SET_BLOCK_MODE_FUNC)
T_TYPEDEF((
           T_IPC_LINK /*link*/,
           T_BOOL /*block_mode*/
           ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_SET_MULTIPLEX_FUNC)
T_TYPEDEF((
           T_IPC_LINK          /* link*/,
       T_IPC_LINK_SOCKET_DEVICE /*device*/,
           T_SOCKET_CHECK_MODE /*check_mode*/,
           T_PTR /*multiplex_data*/, /* multiplex object (e.g., HWND)*/
           T_BOOL /*start_flag*/, /* TRUE == start multiplex, FALSE == stop */
           T_PTR  /* multiplex arg (e.g., T_IPC_LINK_WIN_MSG_WSA_READY) */
           ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_GET_MULTIPLEX_SOURCE_FUNC)
T_TYPEDEF((
           T_IPC_LINK /*link*/,
           T_PTR * /*multiplex_source_return*/
           ));


typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_CONNECT_BEGIN_FUNC)
T_TYPEDEF ((
            T_IPC_LINK /* empty client link whose device needs setting */,
            T_STR /*node*/, /* node portion of logical link name */
            T_STR /*addr*/ /* address portion of logical link name */
            ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_CONNECT_COMPLETE_FUNC)
T_TYPEDEF ((
            T_IPC_LINK /* empty client link whose device needs setting */,
            T_REAL8 /* timeout to wait before giving up */
            ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_SEND_QUEUED_FUNC)
T_TYPEDEF ((
            T_IPC_LINK /* link to send out queued data on */
            ));

/* Returns T_TRUE if handshake is complete, otherwise returns T_FALSE.
 * User MUST wait on READ or WRITE and call function when either is
 * available
 */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_HANDSHAKE_FUNC)
T_TYPEDEF ((
            T_IPC_LINK /* link to perform handshake */
            ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_IOCTL_FUNC)
T_TYPEDEF ((
            T_IPC_LINK /* link to perform ioctl */,
            T_IOCTL_COMMANDS /* the ioctl command to be executed */,
            T_PTR /* user-defined argument (eg. T_STR, T_PROPERTIES) */
            ));

 /*
 * T_IPC_LINK_DRIVER_TRAV_FUNC is for traversing all known link drivers.
 */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_DRIVER_TRAV_FUNC)
T_TYPEDEF((
           T_STR /*name*/,
           T_IPC_LINK_DRIVER /*driver*/,
           T_PTR /*arg*/
           ));

/*
 * T_IPC_LINK_EXPAND_TRAV_STRUCT is used to pass various name
 * information to the link expand traverse funcion.
 */
typedef struct T_IPC_LINK_NAME_EXPAND_TRAV_STRUCT T_IPC_LINK_NAME_EXPAND_TRAV_STRUCT,
  *T_IPC_LINK_NAME_EXPAND_TRAV;
struct T_IPC_LINK_NAME_EXPAND_TRAV_STRUCT {
  T_STR short_name; /* original short name */
  T_STR full_name; /* fully expanded protocol:node:address name */
  T_BOOL using_defaults; /* whether or not default protocols are being used */
};

/*
 * T_IPC_LINK_EXPAND_TRAV_FUNC is for traversing all expansions of
 * a logical link name.
 */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_IPC_LINK_NAME_EXPAND_TRAV_FUNC)
T_TYPEDEF((
           T_IPC_LINK_DRIVER /*driver*/,
           T_IPC_LINK_NAME_EXPAND_TRAV /*trav*/,
           T_PTR /*arg*/
           ));

#ifndef _PTM_LINK_H__
#define _PTM_LINK_H__

/*
 * T_LINK_DRIVER_<NAME> are the names of all the standard link drivers.
 */
#define T_LINK_DRIVER_SNA T_IPC_LINK_DRIVER_SNA
#define T_LINK_DRIVER_ITCP "itcp"

/* ------------------------------------------------------------------------- */
/* T_ITCP_SET_SOCKET_BUFFERS sets the socket buffers of a socket-based link. */
/* ------------------------------------------------------------------------- */
#define T_ITCP_SET_SOCKET_BUFFERS(sock) \
{ \
  T_OPTION conn_socket_buffer_size; \
  T_INT4 size; \
 \
  conn_socket_buffer_size = TutOptionLookup("_conn_socket_buffer_size"); \
  size = (T_INT4)utOptionGetNum(conn_socket_buffer_size); \
  if (0 != size)) { \
    /* ---------------------------------------------------------- */ \
    /* Beef up the socket send and receive buffers.  SMW 09/28/93 */ \
    /* ---------------------------------------------------------- */ \
 \
    /* -------------------------------------------------------------- */ \
    /* Simplify the code to make porting simpler.  If we can't set    */ \
    /* the buffer sizes, it's not a big deal.  It's just not worth    */ \
    /* the porting effort trying to add ifdefs for VMS, Solaris, etc. */ \
    /* SMW 06/29/94                                                   */ \
    /* -------------------------------------------------------------- */ \
    (void)ptITcpSetRecvBufSize(sock, size); \
    (void)ptITcpSetSendBufSize(sock, size); \
  } /* if */ \
} /* T_ITCP_SET_SOCKET_BUFFERS */


/*
 * The values of the default addresses for T_LINK_DEFAULT_<MODULE>.
 */
#define T_LINK_SRV_DEFAULT_APPL "RS"          /* base default applid */
#define T_LINK_LM_DEFAULT_APPL  "LM"          /* base default applid */
#define T_LINK_DEFAULT_PORT     "0"           /* base default port   */
#define T_LINK_DEFAULT_LOGMODE  "ROUTH001"    /* default logmode     */
#define T_SNA_DELIM             '.'           /* address delimeter   */
#define T_SNA_DELIM_STR         "."           /* address delimeter   */
#define T_SNA_TP_SEND_PREFIX    "S"           /* send TP name prefix */
#define T_SNA_TP_RECV_PREFIX    "R"           /* Recv TP name prefix */
#define T_SNA_MAX_TPNAME_LEN     64

/*
 * T_LINK_MULTIPLEX_MODE is the link multiplex mode (used by T_IPC_SESSION).
 */
#define T_LINK_MULTIPLEX_ECB  T_LINK_MULTIPLEX_SEMAPHORE

    /* ----------------------------------------------------------- */
    /*                                                             */
    /*  Structures for the protocol drivers we add                 */
    /*                                                             */
    /* ----------------------------------------------------------- */

#define T_SNA_LU_LENGTH  8
#define T_SNA_MODENAME_LENGTH  8
#define T_SNA_PORT_LENGTH  8

    /* ----------------------------------------------------------- */
    /*                                                             */
    /* T_SNA_LU_ADDRESS - structure defining the source lu,        */
    /*     destination lu, and modetable.                          */
    /*                                                             */
    /* ----------------------------------------------------------- */

struct tagT_SNA_Lu_Address {
    T_CHAR szSlu[T_SNA_LU_LENGTH + 1];     /* source lu name           */
    T_CHAR szDlu[T_SNA_LU_LENGTH + 1];     /* destination lu name      */
    T_CHAR szModeName[T_SNA_MODENAME_LENGTH +1]; /* modetable name     */
    T_CHAR szPort[T_SNA_PORT_LENGTH + 1];
    T_CHAR *pszNodeName;
    };
typedef struct tagT_SNA_Lu_Address T_SNA_LU_ADDRESS ;

    /* ----------------------------------------------------------- */
    /*                                                             */
    /* T_SNA_SNA_TOKEN  - platform independent area off of the     */
    /*     link->device field for maintaining the status of our    */
    /*     I/O.  There is a platform dependent field for our       */
    /*     different implementations of SNA (APPC vs. CPIC).       */
    /*                                                             */
    /* ----------------------------------------------------------- */

enum  tagT_SNA_Token_Type { TTserver,
                          TTclient,
                          TTaccept
} ;
typedef enum tagT_SNA_Token_Type T_SNA_TOKEN_TYPE  ;

typedef struct T_SNA_SNA_TOKEN_STRUCT T_SNA_SNA_TOKEN_STRUCT, *T_SNA_SNA_TOKEN;
#endif /* _PTM_LINK_H__ */

#endif /* _RTWORKS_LINK_H__ */
