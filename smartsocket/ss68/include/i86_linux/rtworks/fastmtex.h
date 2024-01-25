/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/fastmtex.h#1 $
 */

#ifndef  _RTWORKS_FASTMTEX_H_
#define  _RTWORKS_FASTMTEX_H_

#include "utatomic.h"

#if !defined(T_HAS_THREADS) || defined(T_GENERATE_PROTOTYPES)
/* ------------- */
/* Dummy threads */
/* ------------- */

typedef void T_FAST_MUTEX_TYPE, * T_FAST_MUTEX;

#define T_FASTMTEX_INIT(a)    (T_TRUE)
#define T_FASTMTEX_LOCK(a)
#define T_FASTMTEX_UNLOCK(a)
#define T_FASTMTEX_DESTROY(a)

#else

#ifdef T_PURE_ATOMICS
/* --------------------- */
/* Assembly fast mutexes */
/* --------------------- */

#define T_PURE_FASTMTEX

typedef T_ATOM_TYPE T_FAST_MUTEX_TYPE, * T_FAST_MUTEX;

#define T_FASTMTEX_INIT(a)    (0 == (*a = 0))
#define T_FASTMTEX_LOCK(a)    while (0 != T_ATOMIC_SWAP_FM(a, 1, T_NULL));
#define T_FASTMTEX_UNLOCK(a)  T_ATOMIC_SET_FM(a, 0, T_NULL);
#define T_FASTMTEX_DESTROY(a)

#elif defined(T_POSIX_THREADS)
/* ------------- */
/* POSIX Threads */
/* ------------- */

typedef pthread_mutex_t T_FAST_MUTEX_TYPE, * T_FAST_MUTEX;

#define T_FASTMTEX_INIT(a)    TutFastMutexInit(a)
#define T_FASTMTEX_LOCK(a)    pthread_mutex_lock(a)
#define T_FASTMTEX_UNLOCK(a)  pthread_mutex_unlock(a)
#define T_FASTMTEX_DESTROY(a) pthread_mutex_destroy(a)

#elif defined(T_SOLARIS2_THREADS)
/* ---------------- */
/* Solaris2 Threads */
/* ---------------- */

typedef mutex_t T_FAST_MUTEX_TYPE, * T_FAST_MUTEX;

#define T_FASTMTEX_INIT(a)    TutFastMutexInit(a)
#define T_FASTMTEX_LOCK(a)    mutex_lock(a)
#define T_FASTMTEX_UNLOCK(a)  mutex_unlock(a)
#define T_FASTMTEX_DESTROY(a) mutex_destroy(a)

#else
error unsupported fast mutex platform
#endif

#endif /* defined(T_HAS_THREADS) && !defined(T_GENERATE_PROTOTYPES) */

#endif /* _RTWORKS_FASTMTEX_H_ */
