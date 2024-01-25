/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tiof/biof/dev/68/src/rtworks/metal.h#2 $
 */

#ifndef  _METAL_H_
#define  _METAL_H_

#include <rtworks/common.h>
/*
 * T_LINK_DRIVER_<NAME> are the names of all the standard link drivers.
 */
#define T_LINK_DRIVER_LOCAL "local"
#define T_LINK_DRIVER_TCP "tcp"
#define T_LINK_DRIVER_UDP_BROADCAST "udp_broadcast"


typedef struct T_EVENT_STRUCT T_EVENT_STRUCT, *T_EVENT;
#ifndef T_INVALID_EVENT
#define T_INVALID_EVENT ((T_EVENT)0)
#endif
typedef struct T_LINK_STRUCT T_LINK_STRUCT, *T_LINK;
typedef struct T_LINK_DRIVER_STRUCT T_LINK_DRIVER_STRUCT, *T_LINK_DRIVER;
typedef struct T_MUX_IDLE_CB_DATA_STRUCT T_MUX_IDLE_CB_DATA_STRUCT,
  *T_MUX_IDLE_CB_DATA;
typedef struct T_MUX_STRUCT T_MUX_STRUCT, *T_MUX;
typedef struct T_TIMER_STRUCT T_TIMER_STRUCT, *T_TIMER;


typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_EVENT_FUNC)
T_TYPEDEF((T_EVENT,             /* event */
           T_PTR));             /* dispatch_arg */

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_FUNC)
T_TYPEDEF((T_LINK,              /* link */
           T_EVENT));           /* event */

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_MUX_IDLE_CB_FUNC)
T_TYPEDEF((T_MUX,               /* mux */
           T_MUX_IDLE_CB_DATA,  /* data */
           T_CB_ARG));          /* arg */

typedef T_EVENT (T_ENTRY1 * T_ENTRY2 T_EXPORT T_POST_FUNC)
T_TYPEDEF((T_PTR,               /* post_arg */
           T_MUX,               /* mux */
           T_EVENT));           /* event */


/* Link and link driver operations */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_CREATE_SERVER_FUNC)
T_TYPEDEF((T_LINK));            /* server_link */

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_CREATE_CLIENT_FUNC)
T_TYPEDEF((T_LINK));            /* client_link */

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_ACCEPT_CLIENT_FUNC)
T_TYPEDEF((T_LINK,              /* server_link */
           T_LINK));            /* accept_link */

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_DESTROY_FUNC)
T_TYPEDEF((T_LINK));            /* link */

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_CHECK_FUNC)
T_TYPEDEF((T_LINK,              /* link */
           T_IO_CHECK_MODE,     /* check_mode */
           T_REAL8));           /* timeout */

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_CONNECT_FUNC)
T_TYPEDEF((T_LINK,              /* link */
           T_REAL8));           /* timeout */

typedef T_INT4 (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_RECV_FUNC)
T_TYPEDEF((T_LINK,              /* link */
           T_PTR,               /* data */
           T_INT4));            /* size */

typedef T_INT4 (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_SEND_FUNC)
T_TYPEDEF((T_LINK,              /* link */
           T_PTR,               /* data */
           T_INT4));            /* size */

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_SET_BLOCK_MODE_FUNC)
T_TYPEDEF((T_LINK,              /* link */
           T_BOOL));            /* block_mode */

/* Multiplexer-based asynchronous I/O initiation */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_POST_FUNC)
T_TYPEDEF((T_LINK,              /* server_link */
           T_MUX,               /* mux */
           T_EVENT));           /* event */

typedef T_LINK_POST_FUNC T_LINK_POST_ACCEPT_FUNC;
typedef T_LINK_POST_FUNC T_LINK_POST_CONNECT_FUNC;
typedef T_LINK_POST_FUNC T_LINK_POST_RECV_FUNC;
typedef T_LINK_POST_FUNC T_LINK_POST_SEND_FUNC;

/* Driver management */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_LINK_DRIVER_TRAV_FUNC)
T_TYPEDEF((T_STR,               /* name */
           T_LINK_DRIVER,       /* driver */
           T_PTR));             /* arg */


#if defined(T_OS_SOLARIS2) || defined(T_OS_HPUX_11_00) || defined(T_OS_AIX) || defined(T_OS_LINUX)
#define T_MUX_UNIX_POLL
#if defined(T_OS_HPUX_11_00) || defined(T_OS_AIX) || defined(T_OS_LINUX)
#include <poll.h>
#endif
#else
#if defined(T_OS_WIN32)
#define T_MUX_WIN32_IOCP
#endif
#endif


typedef enum {
  T_EVENT_INVALID,              /* Not (yet?) an event */
  T_EVENT_WAKE,                 /* Wake event loop event */
  T_EVENT_SIMPLE,               /* Simple event */
  T_EVENT_TIMER,                /* Timer event */
  /* Link events *only* from here on -- KLC 4/8/99 */
  T_EVENT_LINK_ACCEPT,          /* Link accept event */
  T_EVENT_LINK_CONNECT,         /* Link connect event */
  T_EVENT_LINK_RECV,            /* Link recv event */
  T_EVENT_LINK_SEND,            /* Link send event */
  T_EVENT_LINK_SHUTDOWN         /* Link shutdown event */
} T_EVENT_KIND;

typedef struct T_EVENT_U_TIMER_STRUCT T_EVENT_U_TIMER_STRUCT,
  *T_EVENT_U_TIMER;
struct T_EVENT_U_TIMER_STRUCT {
  T_TIMER timer;                /* Associated timer object */
  T_REAL8 expiration;           /* Timer expiration time */
};

typedef struct T_EVENT_U_LINK_STRUCT T_EVENT_U_LINK_STRUCT,
  *T_EVENT_U_LINK;
struct T_EVENT_U_LINK_STRUCT {
  T_LINK link;                  /* Associated link object */
  T_LINK_FUNC complete_func;    /* Link driver completion function */
  T_LINK accept_link;           /* Accepted link object (or NULL) */
  T_BUF  buf;                   /* Buffer for send/recv data */
  T_BOOL successful;            /* TRUE if I/O completed successfully */
  T_INT4 bytes_transferred;     /* Count of bytes transferred (or zero) */
  T_INT4 link_error;            /* Link-driver-specific error code */
#if defined(T_MUX_UNIX_POLL)
  T_INT4 fd;                    /* File descriptor to poll */
  T_IO_CHECK_MODE mode;         /* I/O check mode */
  T_BOOL ready;                 /* TRUE if fd *might* be ready for I/O */
#endif
#if defined(T_MUX_WIN32_IOCP)
  WSABUF wsabuf;                /* Winsock buffer structure */
#endif
};

struct T_EVENT_STRUCT {
  T_EVENT left;                 /* For deque routines */
  T_EVENT right;                /* For deque routines */
  T_EVENT_KIND kind;            /* Kind of event */
  union {
    T_EVENT_U_TIMER_STRUCT te;  /* Timer event data */
    T_EVENT_U_LINK_STRUCT le;   /* Link event data */
  } u;
  T_POST_FUNC post_func;        /* Function for posting this kind of event */
  T_PTR post_arg;               /* Postable object for above function */
  T_MUX mux;                    /* Multiplexer for this event */
  T_EVENT_FUNC dispatch_func;   /* Event dispatch function */
  T_PTR dispatch_arg;           /* Argument for above function */
  T_PTR user_prop;              /* User-defined property */
#if defined(T_MUX_WIN32_IOCP)
  OVERLAPPED overlapped;        /* Overlapped I/O completion key */
  T_EVENT event;                /* Pointer to beginning of event struct */
#endif
};


struct T_TIMER_STRUCT {
  T_TIMER left;                 /* For deque routines */
  T_TIMER right;                /* For deque routines */
  T_STR id;                     /* Timer ID (for diagnostics) */
  T_REAL8 interval;             /* Time interval (in seconds) */
  T_MUX mux;                    /* Multiplexer for this timer */
  T_INT4 ref_count;             /* Reference count (pending events) */
  T_PTR user_prop;              /* User-defined property */
};


/*
 * T_LINK is a socket-like endpoint of a communication link. It can be
 * thought of as an instance of a link driver.
 */
struct T_LINK_STRUCT {
  T_LINK left;                  /* For deque routines */
  T_LINK right;                 /* For deque routines */
  T_LINK_DRIVER driver;         /* Link driver */
  T_STR node;                   /* resolved node from LCN */
  T_STR address;                /* resolved address from LCN */
  T_PTR device;                 /* Driver-specific data */
  T_BOOL block_mode;            /* TRUE means emulate/use blocking I/O */
  T_MUX mux;                    /* Multiplexer for this link */
  T_INT4 ref_count;             /* Reference count (pending events) */
  T_PTR user_prop;              /* User-defined property */
};

#define T_LINK_CREATE(link, link_driver, link_node, link_address)   \
{                                                                   \
  T_NEWC(link);                                                     \
  link->driver = link_driver;                                       \
  T_STRDUP(link->node, link_node);                                  \
  T_STRDUP(link->address, link_address);                            \
}

#define T_LINK_CHANGE_ADDRESS(link, new_address) \
{ \
  T_FREE(link->address); \
  T_STRDUP(link->address, new_address); \
}

#define T_LINK_DESTROY(link)    \
{                               \
  T_FREE(link->node);           \
  T_FREE(link->address);        \
  T_FREE(link);                 \
}


/*
 * T_LINK_SOCKET_DEVICE is T_LINK->device for socket-based drivers.
 */
typedef struct T_LINK_SOCKET_DEVICE_STRUCT T_LINK_SOCKET_DEVICE_STRUCT,
  *T_LINK_SOCKET_DEVICE;
struct T_LINK_SOCKET_DEVICE_STRUCT {
  T_SOCK socket_fd;
};

#define T_GLINK_SOCKET_FD(link) \
  (((T_LINK_SOCKET_DEVICE)((link)->device))->socket_fd)


/*
 * T_LINK_DRIVER is for managing transports for communication links.
 */
struct T_LINK_DRIVER_STRUCT {
  T_STR name;                   /* Driver name (e.g. "tcp") */
  T_BOOL network_mode;          /* does protocol use network? */
  T_LINK_CREATE_SERVER_FUNC create_server_func;
  T_LINK_CREATE_CLIENT_FUNC create_client_func;
  T_LINK_ACCEPT_CLIENT_FUNC accept_client_func;
  T_LINK_DESTROY_FUNC destroy_func;
  T_LINK_CHECK_FUNC check_func;
  T_LINK_CONNECT_FUNC connect_func;
  T_LINK_RECV_FUNC recv_func;
  T_LINK_SEND_FUNC send_func;
  T_LINK_SET_BLOCK_MODE_FUNC set_block_mode_func;
  T_LINK_POST_ACCEPT_FUNC post_accept_func;
  T_LINK_POST_CONNECT_FUNC post_connect_func;
  T_LINK_POST_RECV_FUNC post_recv_func;
  T_LINK_POST_SEND_FUNC post_send_func;
  T_PTR user_prop;              /* User-defined property */
};


struct T_MUX_IDLE_CB_DATA_STRUCT {
  T_CB cb;                      /* Callback object */
  T_EVENT quit_event;           /* Event that caused exit, or T_NULL */
};


struct T_MUX_STRUCT {
  T_MUTEX notify_mutex;         /* Mutex protects instance state */
#if defined(T_MUX_UNIX_POLL)
  T_BOOL poll_in_progress;      /* TRUE if there's a thread in poll */
  T_INT4 notify_cond_count;     /* Count of threads waiting on notify_cond */
  T_COND notify_cond;           /* Condition variable for waiting threads */
  T_INT4 notify_read_fd;        /* Internal notification read fd */
  T_INT4 notify_write_fd;       /* Internal notification write fd */
  T_INT4 notify_event_count;    /* Count of events ready for dispatch */
  T_EVENT notify_event_list;    /* Deque of events ready for dispatch */
  T_INT4 le_new_count;          /* Count of new link I/O entries */
  T_EVENT le_new_list;          /* Deque of new link I/O entries */
  T_INT4 le_waiting_count;      /* Count of pending link I/O entries */
  T_EVENT le_waiting_list;      /* Deque of pending link I/O entries */
#endif
#if defined(T_MUX_WIN32_IOCP)
  HANDLE iocp;                  /* Win32 I/O completion port */
#endif
  T_INT4 te_new_count;          /* Count of new timer entries */
  T_EVENT te_new_list;          /* Deque of new timer entries */
  T_INT4 te_waiting_count;      /* Count of pending timer entries */
  T_EVENT te_waiting_list;      /* Deque of pending timer entries */
  T_INT4 te_expired_count;      /* Count of expired timer entries */
  T_EVENT te_expired_list;      /* Deque of expired timer entries */
  T_INT4 timer_quantum;         /* Quantum for forced timeout checks */
  T_INT4 timer_countdown;       /* Countdown to forced timeout check */
  T_REAL8 timestamp;            /* Wall time at last timeout check */
  T_CB_LIST idle_cb_list;       /* Idle callback list */
  T_PTR user_prop;              /* User-defined property */
};


/*
 * This controls the default setting for the inner_loop_quantum member of
 * the mux structure. This is how many events we'll allow to pour through
 * the inner loop of TutMuxEventLoop before we force a check for expired
 * timers (in utMuxFrobTimers).
 */
#define T_MUX_DEFAULT_TIMER_QUANTUM (1000)

#include <rtworks/metalpr.h>
#endif /* _METAL_H_ */
