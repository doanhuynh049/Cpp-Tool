/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/sockinc.h#3 $
 */

/* =================================================================== */
/*                                                                     */
/*            sockinc.h  -- socket header files                        */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_SOCKET_INC_H_
#define _RTWORKS_SOCKET_INC_H_

/*
 * Portable socket error codes.  OS/2 has many BSD compat
 * defines, so leverage off those if possible.  SMW 11/11/96
 */
#ifdef T_OS_WINDOWS

# define T_C_EADDRINUSE  WSAEADDRINUSE
# define T_C_ECONNREFUSED  WSAECONNREFUSED
# define T_C_ECONNRESET  WSAECONNRESET
# define T_C_EHOSTUNREACH  WSAEHOSTUNREACH
# define T_C_EINPROGRESS  WSAEINPROGRESS
# define T_C_EINTR  WSAEINTR
# define T_C_EISCONN  WSAEISCONN
# define T_C_ENETUNREACH  WSAENETUNREACH
# define T_C_ENOPROTOOPT  WSAENOPROTOOPT
# define T_C_EPROTONOSUPPORT  WSAEPROTONOSUPPORT
# define T_C_ETIMEDOUT  WSAETIMEDOUT

#else

/* UNIX, VMS */
# define T_C_EADDRINUSE  EADDRINUSE
# define T_C_ECONNREFUSED  ECONNREFUSED
# define T_C_ECONNRESET  ECONNRESET
# define T_C_EHOSTUNREACH  EHOSTUNREACH
# define T_C_EINPROGRESS  EINPROGRESS
# define T_C_EINTR EINTR
# define T_C_EISCONN  EISCONN
# define T_C_ENETUNREACH  ENETUNREACH
# define T_C_ENOPROTOOPT  ENOPROTOOPT
# define T_C_EPROTONOSUPPORT  EPROTONOSUPPORT
# define T_C_ETIMEDOUT  ETIMEDOUT

#endif

#define T_GROW_FD_ZERO(sock, set)                     \
   if (sock >= (sizeof(fd_set) * 8)) {                \
     T_INT4 bytes = (T_INT4) sock/8;                  \
     T_CALLOC(set, T_ROUND_UP(bytes+4, 4), fd_set *); \
   }                                                  \
   else {                                             \
     FD_ZERO(set);                                    \
   }

#define T_GROW_FD_CHECK(sock, set1, set2, set3, size) \
   if (sock >= (size * 8)) {                          \
     T_INT4 bytes = (T_INT4) sock/8;                  \
     bytes = T_ROUND_UP(bytes+4, 4);                  \
     T_REALLOC(set1, bytes, fd_set *);                \
     T_REALLOC(set2, bytes, fd_set *);                \
     T_REALLOC(set3, bytes, fd_set *);                \
     memset(((T_STR) set1) + size, '\0', bytes-size); \
     memset(((T_STR) set2) + size, '\0', bytes-size); \
     memset(((T_STR) set3) + size, '\0', bytes-size); \
     size = bytes;                                    \
   }

#define T_GROW_FD_FREE(test, set) if ((test)) T_FREE(set);

#define T_GROW_FD_FREE2(cond, set, except) \
   if ((cond)) {                           \
     T_FREE(set);                          \
     T_FREE(except);                       \
   }
#define T_GROW_FD_FREE3(cond, read, write, except) \
   if ((cond)) {                                   \
     T_FREE(read);                                 \
     T_FREE(write);                                \
     T_FREE(except);                               \
   }

/* based on <netdnet/dn.h> */

/* ----------------------------------------------------------------- */
/* socket address, DECnet style.  This is a very simplified version  */
/* that I invented.  SMW 5/13/90                                     */
/* ----------------------------------------------------------------- */
struct sockaddr_dn {
  short sdn_family; /* must be AF_DECnet */
  /* expand max node name for OpenVMS VAX 6.1.  SMW 07/04/94 */
#define T_MAX_DECNET_NODE_NAME 255
  char sdn_node_name[T_MAX_DECNET_NODE_NAME + 1]; /* DECnet node name */
#define T_MAX_DECNET_TASK_NAME 16
  /* add an extra byte for testing purposes.  SMW 03/12/96 */
  char sdn_task_name[T_MAX_DECNET_TASK_NAME + 2]; /* DECnet task name */
};

#if !defined(T_GENERATE_PROTOTYPES)

# if defined(T_OS_VMS)
#  include <netdb.h>
#  include <socket.h>
#  include <in.h>

/*
 * Well, VMS strikes again.  We now use the socket header files in
 * sys$library instead of rolling our own.  The function prototype for
 * select in socket.h is based on BSD 4.2, not BSD 4.3.  It does NOT use
 * fd_set.  We have to kludge up the FD_* macros to get around this.
 * This is for VMS 5.2 and VAX C 3.0 (maybe this is fixed in VMS 5.3).
 * SMW 7/29/90
 * Nope, still not fixed in VAX C 3.1 (VMS 5.4-1), or DEC C 1.2 (OpenVMS
 * AXP 1.0).  SMW 6/22/93
 * For bug 3455, we do need fd_set with > 32 fds!  SMW 08/28/94
 *
 * DEC C++ 5.3 (and maybe DEC C 5.3?) finally fix the prototype, but
 * they set FD_SETSIZE to 32 in <time.h>, so we have to fix it before
 * including time.h in vms_inc.h.  SMW 08/19/96
 */

#  ifndef FD_SETSIZE
#   define FD_SETSIZE 1024   /* allow more than 256.  SMW 04/13/95 */
#  endif

#  ifndef NBBY
#   define NBBY 8
#  endif

#  ifndef FD_SET /* try to protect if DEC does implement these.  SMW 6/22/93 */
typedef long    fd_mask;
#   define NFDBITS (sizeof(fd_mask) * NBBY) /* bits per mask */
#   ifndef howmany
#    define howmany(x, y) (((x)+((y)-1))/(y))
#   endif

typedef struct fd_set {
  fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;

#   define FD_SET(n, p)   ((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#   define FD_CLR(n, p)   ((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#   define FD_ISSET(n, p) ((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#   define FD_ZERO(p)     memset((char *)(p), '\0', sizeof(*(p)))
#  endif /* FD_SET */

/* ---------------------------------------------------------------------- */
/* Stuff need to (minimally) support our emulation of fcntl.  SMW 4/18/93 */
/* ---------------------------------------------------------------------- */
#  define F_GETFL 3 /* Get file flags */
#  define F_SETFL 4 /* Set file flags */

# elif defined(T_OS_UNIX)
#  include <netdb.h> /* gethostbyname() */

#  include <sys/socket.h> /* basic socket definitions */

#  ifdef T_USE_POLL
#   include <poll.h>
#  endif

#  include <netinet/in.h> /* network socket definitions (Internet domain) */
#  include <fcntl.h> /* F_SETFL, F_GETFL, and FNDELAY */
#  ifdef T_OS_HPUX
#   include <sys/file.h> /* FNDELAY lurks here */
#  endif /* T_OS_HPUX */
#  if defined(T_OS_AIX)
#   include <sys/select.h> /* fd_set lurks here */
#  endif /* T_OS_AIX */
#  ifdef T_OS_OSS
/*
 * FNDELAY lurks in <select.h>, but fd_set is in both <select.h> and
 * <time.h>, and there is no macro protection around the redefinition,
 * so define FNDELAY manually.  SMW 10/27/97
 */
#   ifndef FNDELAY
#    define FNDELAY 0x4000
#   endif
#  endif /* T_OS_OSS */

# elif defined(T_OS_WINDOWS)
#  include <winsock2.h>

# else
socket_inc.h unknown OS
# endif
#endif /* !defined(T_GENERATE_PROTOTYPES) */

/* ----------------------------------------------------------------- */
/* Stupid HP-UX has the fd_set data type, but doesn't use it in the  */
/* prototypes for select(2)!  No wonder they call it Hockey-Pucks!   */
/* SMW 6/22/92                                                       */
/* ----------------------------------------------------------------- */
#if defined(T_HP700_HPUX9) || defined(T_OS_HPUX_10_20)
# define T_SELECT_CAST (int *)
#else
# define T_SELECT_CAST
#endif

/* ----------------------------------------------------------------- */
/* If we are compiling on IRIX for the MIPS ABI, we CANNOT use bzero */
/* as it doesn't exist on Tandem.  SMW 09/14/94                      */
/* ----------------------------------------------------------------- */
#if defined(T_OS_IRIX) && defined(_ABI_SOURCE)
# undef FD_ZERO
# define FD_ZERO(p) memset((void *)(p), 0, sizeof(*(p)))
#endif

/*
 * Linux does not have AF_DECnet, which we use for some socket test
 * cases.  SMW WTJ 08/19/97
 */
#if defined(T_OS_LINUX) && !defined(AF_DECnet)
# define AF_DECnet 12  /* from SunOS <sys/socket.h> */
#endif

/* ------------------------------------------------------------------- */
/* If we have Unix domain sockets, then we need to include <sys/un.h>, */
/* which contains sockaddr_un.  On VMS, we emulate Unix domain sockets */
/* on top of mailboxes.  SMW 5/17/92                                   */
/* ------------------------------------------------------------------- */
#if defined(T_HAS_UNIX_DOMAIN) && !defined(T_OS_VMS)
# include <sys/un.h> /* local socket definitions (Unix domain) */
#else
struct  sockaddr_un {
  short sun_family;     /* AF_UNIX */
  char  sun_path[108];  /* path name (gag) */
};
#endif

/* --------------------------------------------------------------------- */
/* T_SOCKET_BLOCK_FLAG is the flag to use to turn on or off non-blocking */
/* I/O.  If you change these, you might have to change the other         */
/* related defines below too.  SMW 09/24/94                              */
/* --------------------------------------------------------------------- */
#if defined(T_OS_SOLARIS2) || defined(T_OS_AIX) || defined(T_OS_HPUX)
# define T_SOCKET_BLOCK_FLAG (O_NONBLOCK)
#elif defined(T_OS_VMS)
/* we use this since it already defined in <file.h> */
# define T_SOCKET_BLOCK_FLAG (O_NDELAY)
#else
# define T_SOCKET_BLOCK_FLAG (FNDELAY)
#endif

/* --------------------------------------------------------------- */
/* a ptr to T_PORT_ANY can be supplied to TutSocketCreateServerTcp */
/* (and perhaps other driver server create funcs) to create a      */
/* listening server on any available port                          */
/* --------------------------------------------------------------- */
#define T_PORT_ANY (0)

/* --------------------------------------------------------------- */
/* T_SOCKET_IS_INVALID is useful for checking for invalid sockets. */
/* SMW 10/06/95                                                    */
/* --------------------------------------------------------------- */
#ifdef T_OS_WINDOWS
# define T_SOCKET_IS_INVALID(fd)  (INVALID_SOCKET == (fd))
# define T_SOCKET_IS_VALID(fd) (INVALID_SOCKET != (fd))
# define T_INVALID_SOCKET (INVALID_SOCKET)
#else
# define T_SOCKET_IS_INVALID(fd)  (0 > (T_INT4)(fd))
# define T_SOCKET_IS_VALID(fd) (!T_SOCKET_IS_INVALID(fd))
# define T_INVALID_SOCKET (-1)
#endif

/* ------------------------------------------------------------------------ */
/* T_CONNECT_WOULD_BLOCK is useful for non-blocking connects.  SMW 09/24/94 */
/* ------------------------------------------------------------------------ */
#ifdef T_OS_WINDOWS
# define T_CONNECT_WOULD_BLOCK() (WSAEWOULDBLOCK == WSAGetLastError())
#else
# define T_CONNECT_WOULD_BLOCK() (EINPROGRESS == errno)
#endif

/* ------------------------------------------------------------------------ */
/* T_ACCEPT_WOULD_BLOCK is useful for non-blocking accepts.  Try using recv */
/* logic for now.  SMW 10/06/95                                             */
/* ------------------------------------------------------------------------ */
#define T_ACCEPT_WOULD_BLOCK() T_RECV_WOULD_BLOCK()

/* ------------------------------------------------------------------ */
/* T_RECV_WOULD_BLOCK is useful for non-blocking recvs.  SMW 09/24/94 */
/* ------------------------------------------------------------------ */
#if defined(T_OS_AIX) || defined(T_OS_HPUX)
# define T_RECV_WOULD_BLOCK() (EAGAIN == errno)
#elif defined(T_OS_WINDOWS)
# define T_RECV_WOULD_BLOCK() (WSAEWOULDBLOCK == WSAGetLastError())
#else
# define T_RECV_WOULD_BLOCK() (EWOULDBLOCK == errno)
#endif

/* ---------------------------------------------------------------------- */
/* T_SEND_WOULD_BLOCK is useful for non-blocking sends.  SMW 09/24/94     */
/* SunOS 5.1 send() returned ENOMEN, not EAGAIN is it should have.  On    */
/* SunOS 5.3, EAGAIN and EWOULDBLOCK are the same!  SMW 03/10/94          */
/*                                                                        */
/* FWIW, EAGAIN and EWOULDBLOCK are the same on AIX 3.2.5, too.  HP-UX    */
/* 9.01 has *three* types of non-blocking I/O (for send it means          */
/* EWOULDBLOCK, EAGAIN, or return 0). I abritrarily chose the             */
/* "POSIX-style" non-blocking method (send uses EAGAIN).  Unitechnic says */
/* that checking for EWOULDBLOCK helped them, so check for it too (it     */
/* should be benign).  Aren't sockets great?  That's why we are           */
/* SmartSockets! :-) SMW 09/22/95                                         */
/* ---------------------------------------------------------------------- */
#if defined(T_OS_AIX)
# define T_SEND_WOULD_BLOCK() (EAGAIN == errno)
#elif defined(T_OS_HPUX)
# define T_SEND_WOULD_BLOCK() (EAGAIN == errno || EWOULDBLOCK == errno)
#elif defined(T_SUN4_SUNOS_5_1) || defined(T_OS_TANDEM)
# define T_SEND_WOULD_BLOCK() (ENOMEM == errno)
#elif defined(T_OS_IRIX) && defined(_ABI_SOURCE)
/* ------------------------------------------------------------ */
/* For the MIPS ABI, we need EWOULDBLOCK on IRIX but ENOMEM on  */
/* Tandem.  SMW 10/02/94                                        */
/* ------------------------------------------------------------ */
# define T_SEND_WOULD_BLOCK() (EWOULDBLOCK == errno || ENOMEM == errno)
#elif defined(T_OS_WINDOWS)
# define T_SEND_WOULD_BLOCK() (WSAENOBUFS == WSAGetLastError() \
                              || WSAEWOULDBLOCK == WSAGetLastError())
#else
# define T_SEND_WOULD_BLOCK() (EWOULDBLOCK == errno)
#endif

/* -------------------------------------------------------- */
/* T_SEND_AT_EOF is useful for EOF detection.  SMW 09/24/94 */
/* -------------------------------------------------------- */
#if defined(T_OS_HPUX)
/* HP-UX 9.01 uses EINVAL too.  SMW 05/29/94 */
# define T_SEND_AT_EOF() \
  (EPIPE == errno || EINVAL == errno || ECONNRESET == errno)
#elif defined(T_OS_WINDOWS)
# define T_SEND_AT_EOF() \
  (WSAESHUTDOWN == WSAGetLastError() || WSAECONNRESET == WSAGetLastError())
#else
# define T_SEND_AT_EOF() (EPIPE == errno || ECONNRESET == errno)
#endif

/*
 * T_SEND_MSG_TOO_BIG is for detecting that we tried to send a
 * chunk of data that was too large.
 * SMW 07/01/96
 */
#if defined(T_OS_WINDOWS)
# define T_SEND_MSG_TOO_BIG()  (WSAEMSGSIZE == WSAGetLastError())
#else
/* connection-oriented sockets are not supposed to do this!  SMW 07/03/96 */
# define T_SEND_MSG_TOO_BIG()  (T_FALSE)
#endif

/* ---------------------------------------------------------------- */
/* T_RECV_AT_EOF is useful for EOF detection.  SMW 09/24/94         */
/* ECONNRESET added based on information supplied by DC Systems for */
/* SunOS 4.1.X.  SMW 04/14/95                                       */
/* ---------------------------------------------------------------- */
#if defined(T_OS_TANDEM) || defined(T_OS_IRIX) && defined(_ABI_SOURCE)
# define T_RECV_AT_EOF(val) \
  (0 == (val) || -1 == (val) && (ECONNRESET == errno || EPROTO == errno))
#elif defined(T_OS_WINDOWS)
/* bug4031 test case on NT 4.0 SP #1 fleshed out WSAESHUTDOWN.  SMW 01/17/97 */
# define T_RECV_AT_EOF(val) \
  (0 == (val) || (-1 == (val) && (WSAECONNRESET == WSAGetLastError() \
                                 || WSAESHUTDOWN == WSAGetLastError())))
#else
# define T_RECV_AT_EOF(val) (0 == (val) || (-1 == (val) && ECONNRESET == errno))
#endif

/* --------------------------------------------------------------------- */
/* T_BUILD_SOCKET_FILE_NAME builds a file name for a UNIX-domain socket. */
/* --------------------------------------------------------------------- */
#define T_BUILD_SOCKET_FILE_NAME(dest, name, destsize) \
  TutBuildNFileName((dest), (destsize), TutGetSocketDir(), (name), T_NULL)

#endif /* _RTWORKS_SOCKET_INC_H_ */
