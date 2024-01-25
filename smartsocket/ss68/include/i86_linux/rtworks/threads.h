/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/threads.h#4 $
 */

/* =================================================================== */
/*                                                                     */
/*              threads.h -- multi-threading & synchronization         */
/*                                                                     */
/* =================================================================== */

#ifndef  _RTWORKS_THREADS_H_
#define  _RTWORKS_THREADS_H_

/*
 * The following function typedefs are now identical for all platforms,
 * regardless of what the "native" signatures are.  This makes it much
 * simpler to write portable code.  --KLC 10/19/96
 */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_THREAD_FUNC)
T_TYPEDEF((
           T_PTR                /* arg */
           ));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_TSD_KEY_DESTRUCTOR_FUNC)
T_TYPEDEF((
           T_PTR                /* value */
           ));

#define TutMutexLockFast(mt) TutMutexLock((mt), T_TIMEOUT_FOREVER)

#if defined(T_HAS_THREADS) && !defined(T_GENERATE_PROTOTYPES)

# if defined(T_OS_AIX) || defined(T_OS_OSF) || defined(T_OS_VMS) || defined(T_OS_IRIX) || defined(T_OS_LINUX) || defined(T_OS_OSS) || defined(T_OS_HPUX)
/* ------------- */
/* POSIX Threads */
/* ------------- */
#  define T_POSIX_THREADS

#  if defined(T_OS_OSS)
#   include <spthread.h>
#  else
#   include <pthread.h>
#  endif

typedef pthread_t T_THREAD;
typedef pthread_cond_t *T_COND;
typedef struct T_MUTEX_STRUCT T_MUTEX_STRUCT, *T_MUTEX;
typedef pthread_key_t T_TSD_KEY;

struct T_MUTEX_STRUCT {
  pthread_mutex_t mp;           /* POSIX threads mutex */
  pthread_cond_t cv;
  pthread_t thread;             /* Thread which holds mutex */
  T_INT4 level;                 /* Recursion level */
  T_BOOL lightweight;
};

#  define T_INVALID_THREAD  TutThreadPosixGetInvalid()

#  if defined(T_OS_HPUX_11_00) || defined(T_OS_HPUX_64BIT) || defined(T_OS_LINUX) || defined(T_OS_ALPHA_OSF_40) || defined(T_OS_IRIX_62) || defined(T_OS_OSS) || (defined(T_OS_VMS) && !defined(T_OS_VMS_6))
#   define T_PTHREAD_CREATE_UNDETACHED PTHREAD_CREATE_JOINABLE
#  else
#   define T_PTHREAD_CREATE_UNDETACHED PTHREAD_CREATE_UNDETACHED
#  endif

# elif defined(T_OS_SOLARIS2)
/* ---------------- */
/* Solaris2 Threads */
/* ---------------- */
#  define T_SOLARIS2_THREADS
#  include <thread.h>
typedef cond_t *T_COND;
typedef struct T_MUTEX_STRUCT T_MUTEX_STRUCT, *T_MUTEX;
typedef thread_t T_THREAD;
typedef thread_key_t T_TSD_KEY;

struct T_MUTEX_STRUCT {
  mutex_t mp;                   /* Solaris threads mutex */
  cond_t cv;
  thread_t thread;              /* Thread which holds mutex */
  T_INT4 level;
  T_BOOL lightweight;
};

# elif defined(T_OS_WIN32)
/* --------------- */
/* Windows Threads */
/* --------------- */
#  define T_WIN32_THREADS
#  include <process.h>
typedef HANDLE T_COND;
typedef HANDLE T_THREAD;
typedef DWORD T_TSD_KEY;
typedef struct T_MUTEX_STRUCT T_MUTEX_STRUCT, *T_MUTEX;

struct T_MUTEX_STRUCT {
  union {
    HANDLE mutex;
    CRITICAL_SECTION critical_section;
  } m;
  T_BOOL lightweight;
};

# else
error unknown threads platform
# endif

#else
# if defined(T_GENERATE_PROTOTYPES)
typedef T_PTR T_COND;
typedef T_PTR T_MUTEX;
typedef T_INT4 T_THREAD;
typedef T_INT4 T_TSD_KEY;
# else
#  error "As of 2008-12-15, thread support is required"
# endif
#endif /* defined(T_HAS_THREADS) && !defined(T_GENERATE_PROTOTYPES) */


typedef struct T_DATA_PAIR_STRUCT T_DATA_PAIR_STRUCT, *T_DATA_PAIR;
struct T_DATA_PAIR_STRUCT {
  T_DATA_PAIR left;
  T_DATA_PAIR right;
  T_TSD_KEY key;
  T_PTR value;
};

typedef struct T_OPTION_TSD_STRUCT *T_OPTION_TSD; /* option.c */
typedef struct T_UTLIB_TSD_STRUCT T_UTLIB_TSD_STRUCT, *T_UTLIB_TSD;
struct T_UTLIB_TSD_STRUCT {
  T_BOOL command_interactive;       /* command.c */
  T_INT4 err_num;                   /* error code */
  /*
   * Descriptive string.  This is a string that is used by the error
   * string functions to return an "unknown error" value with the
   * error number. However, it's available for any function that needs
   * to return a ptr to a static string that's safe between Tut* calls
   * within a thread.  JAM 03/21/97
   */
  T_STRING desc_str;
#ifdef T_POSIX_THREADS
  pthread_cond_t mutex_wait_cv;     /* POSIX threads condition variable */
#endif
#ifdef T_SOLARIS2_THREADS
  cond_t mutex_wait_cv;             /* Solaris threads condition variable */
#endif
  /*
   * Used by the PGM link driver to store the name of this connection
   * so that named options work.  This allows the user to have multiple
   * connections that belong to different groups DSG (4/23/03)
   */
  T_STRING connectName;             /* name of this connection setnopt xxxx */
  T_PTR command_control_msg_conn;   /* command.c */
  T_STRING scratch_str;             /* scratch string buffer (TutRealToStr) */
  T_OPTION_TSD option_tsd;          /* option.c */
  T_INT4 err_trace_count;           /* num entries in error stack trace */
  T_PTR err_trace[T_ERR_MAX_TRACES];/* error stack trace */
};


/*
 * thread attribtes
 */
typedef enum {
  T_THREAD_ATTR_TYPE_INVALID = 0,   /* invalid (end of list) */
  T_THREAD_ATTR_TYPE_STR            /* string */
} T_THREAD_ATTR_TYPE;


/*
 * Read/write mutex-- built with T_MUTEX and T_COND synchronization
 * primitives.  This may be recursively locked by either up to reader_quota
 * readers or the writer.
 */
typedef struct T_RW_MUTEX_WAIT_STRUCT T_RW_MUTEX_WAIT_STRUCT, *T_RW_MUTEX_WAIT;
struct T_RW_MUTEX_WAIT_STRUCT {
  T_RW_MUTEX_WAIT left;         /* For deque routines */
  T_RW_MUTEX_WAIT right;        /* For deque routines */
  T_COND cond;                  /* Wake when write ok */
  T_INT4 lock_count;            /* Number of locks (negative for writer) */
  T_THREAD thread;              /* Thread that owns this struct */
}; /* T_RW_MUTEX_WAIT_STRUCT */

typedef struct T_RW_MUTEX_STRUCT T_RW_MUTEX_STRUCT, *T_RW_MUTEX;
struct T_RW_MUTEX_STRUCT {
  T_MUTEX mutex;
  T_COND read_cond;             /* Wake when read ok */
  T_INT4 read_quota;            /* Max allowable read_count */
  T_THREAD *read_threads;       /* Vector of reader thread ids */
  T_INT4 *read_lock_counts;     /* Vector of reader lock counts */
  T_INT4 wait_count;            /* Number of writers waiting */
  T_RW_MUTEX_WAIT wait_list;    /* Doubly-linked list of write-lock waiters */
  T_RW_MUTEX_WAIT_STRUCT write; /* Tail waiter/active lock info */
}; /* T_RW_MUTEX_STRUCT */

#define T_MAX_READ_QUOTA 32


#include "fastmtex.h"
typedef T_FAST_MUTEX T_SPINLOCK;

/*
 * Define invalid object values if they haven't already been defined.
 */
#ifndef T_INVALID_COND
#define T_INVALID_COND ((T_COND)0)
#endif

#ifndef T_INVALID_MUTEX
#define T_INVALID_MUTEX ((T_MUTEX)0)
#endif

#define T_INVALID_RW_MUTEX ((T_RW_MUTEX)0)

#define T_INVALID_SPINLOCK ((T_SPINLOCK)0)

#ifndef T_INVALID_THREAD
#define T_INVALID_THREAD ((T_THREAD)0)
#endif

#ifndef T_INVALID_TSD_KEY
#define T_INVALID_TSD_KEY ((T_TSD_KEY)-1)
#endif

/*
 * These macros support conditional compilation of thread synchronization
 * code with a minimal amount of effort.  In addition, they can be
 * #define'd so as to support runtime selection of thread support.  See
 * ipcp.h for a working example.
 */
#ifndef T_HAS_THREADS
# error "As of 2008-12-15, thread support is required"
#else
# ifndef T_THREAD_ATOMIC
#  define T_THREAD_ATOMIC(stmt)                 \
{                                               \
  TutProcessMutexLock();                        \
  stmt;                                         \
  TutProcessMutexUnlock();                      \
}
# endif

# ifndef T_THREAD_SYNC
#  define T_THREAD_SYNC(expr)                   \
{                                               \
  T_BOOL status = (expr);                       \
  T_EXPECT(T_TRUE == status);                   \
}
# endif

# ifndef T_THREAD_SYNC2
#  define T_THREAD_SYNC2(expr,result) (expr)
# endif

# ifndef T_THREAD_WITH_LOCK
#  define T_THREAD_WITH_LOCK(mtx,stmt)                  \
{                                                       \
  T_BOOL status;                                        \
  status = TutMutexLock((mtx), T_TIMEOUT_FOREVER);      \
  T_EXPECT(T_TRUE == status);                           \
  stmt;                                                 \
  status = TutMutexUnlock((mtx));                       \
  T_EXPECT(T_TRUE == status);                           \
}
# endif
#endif /* T_HAS_THREADS */

#define T_THREAD_GET_OR_NEW_TSD_VALUE(key,val)  \
if (!TutTsdGetValue((key), &(val))) {           \
  T_EXPECT(T_INVALID_TSD_KEY != (key));         \
  T_NEWC(val);                                  \
  TutTsdSetValue((key), (val));                 \
}

#endif /* _RTWORKS_THREADS_H_ */
