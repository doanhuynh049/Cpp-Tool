/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/debug.h#3 $
 */

/* =================================================================== */
/*                                                                     */
/*                   debug.h -- debugging macros                       */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_DEBUG_H_
#define _RTWORKS_DEBUG_H_

/* --------------------------------------------------------------- */
/* We unfortunately need a separate macro for assertions that have */
/* string constants in them.  Perhaps ANSI C has a better way of   */
/* doing this.  SMW                                                */
/*                                                                 */
/* Yes, it does.  With ANSI C, you can use # to convert something  */
/* to a string.  SMW 5/1/91                                        */
/* --------------------------------------------------------------- */
#ifdef T_NO_ASSERTS /* no asserts */

# define T_ASSERT(expr)
# define T_ASSERT_STR(expr)
# define T_EXPECT(expr)
# define T_EXPECT_STR(expr)

# define T_TRY_ASSERT_EXPR( expr, type, op, val ) expr
# define T_TRY_EXPECT_EXPR( expr, type, op, val ) expr

#else

/* ------------------------------------------------------------------------- */
/* Use helper functions TutAssert and TutExpect to make object code coverage */
/* tools (like PureCoverage) more accurate.  SMW 02/19/95                    */
/* ------------------------------------------------------------------------- */
# ifdef T_ANSI_C
#  define T_ASSERT(expr) ( (T_BOOL)(expr) ? ((void)0) : (TutAssert((T_BOOL)(expr), #expr, __FILE__, __LINE__)) )
#  define T_ASSERT_STR(expr) T_ASSERT(expr)

#  define T_EXPECT(expr) ( (T_BOOL)(expr) ? ((void)0) : (TutExpect((T_BOOL)(expr), #expr, __FILE__, __LINE__)) )
#  define T_EXPECT_STR(expr) T_EXPECT(expr)

# else /* non-ANSI C */

#  define T_ASSERT(expr) ( (T_BOOL)(expr) ? ((void)0): (TutAssert(0 != (expr), "expr", __FILE__, __LINE__)))
#  define T_ASSERT_STR(expr) ( (T_BOOL)(expr) ? ((void)0): \
    (TutAssert(0 != (expr), "text not avail", __FILE__, __LINE__)))

#  define T_EXPECT(expr) ( (T_BOOL)(expr) ? ((void)0) : (TutExpect(0 != (expr), "expr", __FILE__, __LINE__)))
#  define T_EXPECT_STR(expr) ( (T_BOOL)(expr) ? ((void)0): \
    (TutExpect(0 != (expr), "text not avail", __FILE__, __LINE__)))

# endif /* T_ANSI_C */

# define T_TRY_ASSERT_EXPR( expr, type, op, val ) \
  { register type __result = expr; T_ASSERT( __result op val ); } (void)0

# define T_TRY_EXPECT_EXPR( expr, type, op, val ) \
  { register type __result = expr; T_EXPECT( __result op val ); } (void)0

#endif /* T_NO_ASSERTS */

#define T_TRY_ASSERT( expr ) T_TRY_ASSERT_EXPR( expr, T_BOOL, ==, T_TRUE )
#define T_TRY_EXPECT( expr ) T_TRY_EXPECT_EXPR( expr, T_BOOL, ==, T_TRUE )

#define T_TRY_ASSERT_EQ( expr, type, val ) \
  T_TRY_ASSERT_EXPR( expr, type, ==, val );
#define T_TRY_EXPECT_EQ( expr, type, val ) \
  T_TRY_EXPECT_EXPR( expr, type, ==, val );

#define T_TRY_ASSERT_NEQ( expr, type, val ) \
  T_TRY_ASSERT_EXPR( expr, type, !=, val );
#define T_TRY_EXPECT_NEQ( expr, type, val ) \
  T_TRY_EXPECT_EXPR( expr, type, !=, val );

#define T_TRY_ASSERT_NULL( expr ) \
  T_TRY_ASSERT_EXPR( expr, T_PTR, ==, T_NULL );
#define T_TRY_EXPECT_NULL( expr ) \
  T_TRY_EXPECT_EXPR( expr, T_PTR, ==, T_NULL );

#define T_TRY_ASSERT_NON_NULL( expr ) \
  T_TRY_ASSERT_EXPR( expr, T_PTR, !=, T_NULL );
#define T_TRY_EXPECT_NON_NULL( expr ) \
  T_TRY_EXPECT_EXPR( expr, T_PTR, !=, T_NULL );

#ifdef T_NO_CHECK_ARGS /* no arg checking */

Syntax error (see bug 3183): 3.5 (2) and beyond
should never turn on T_NO_CHECK_ARGS.

#define T_CHECK_NULL(ptr, ret_value)
#define T_CHECK_RANGE(num, upper, lower, ret_value)
#define T_CHECK_LT(num, bound, ret_value)
#define T_CHECK_GT(num, bound, ret_value)
#define T_CHECK_BOOL(flag, ret_value)
#define T_CHECK_EQ(num, val, ret_value)
#define T_CHECK_NEQ(num, val, ret_value)
#define T_CHECK_EXPR(expr, ret_value)

#else /* yes, we want arg checking */

#define T_CHECK_NULL(ptr, ret_value) \
  if ((ptr) == NULL) { \
    TutErrNumSet(T_ERR_NULL_PTR); \
    return (ret_value); \
  } /* if */

#define T_CHECK_RANGE(num, upper, lower, ret_value) \
  if ((num) > (upper) || (num) < (lower)) { \
    TutErrNumSet(T_ERR_VAL_INVALID); \
    return (ret_value); \
  } /* if */

#define T_CHECK_LT(num, bound, ret_value) \
  if ((num) < (bound)) { \
    TutErrNumSet(T_ERR_VAL_TOO_SMALL); \
    return (ret_value); \
  } /* if */

#define T_CHECK_GT(num, bound, ret_value) \
  if ((num) > (bound)) { \
    TutErrNumSet(T_ERR_VAL_TOO_LARGE); \
    return (ret_value); \
  } /* if */

#define T_CHECK_BOOL(flag, ret_value) \
  T_CHECK_RANGE((flag), T_TRUE, T_FALSE, (ret_value))

#define T_CHECK_EQ(num, val, ret_value) \
  if ((num) != (val)) { \
    TutErrNumSet(T_ERR_VAL_INVALID); \
    return (ret_value); \
  } /* if */

#define T_CHECK_NEQ(num, val, ret_value) \
  if ((num) == (val)) { \
    TutErrNumSet(T_ERR_VAL_INVALID); \
    return (ret_value); \
  } /* if */

#define T_CHECK_EXPR(expr, ret_value) \
  if (!(expr)) { \
    TutErrNumSet(T_ERR_VAL_INVALID); \
    return (ret_value); \
  } /* if */

#endif /* T_NO_CHECK_ARGS */

/* ---------------------------------------------------------------------- */
/* T_DBG macros provide run-time debugging.  The global variable          */
/* TutDebugFlags is a character array where each character represents one */
/* area of debugging that can be turned on and off.  If T_NO_DBG is       */
/* defined, then the debugging macros are completely turned off.          */
/* Originally the mask was a bit mask (a long integer), but that only     */
/* allows 32 different debugging areas.  Making the mask a character      */
/* array allows us to have as many levels of debugging as we want.  SMW   */
/* ---------------------------------------------------------------------- */

/* ------------------------------------------------------------------ */
/*  Note: If you change this file, you should update the command-line */
/* arguments so these can be set from the command line.               */
/* ------------------------------------------------------------------ */

/* -------------------------------------------------------------------- */
/* Util stuff.  Note that the Util and IPC debug flags must be globally */
/* available to everyone, since everyone must be able to set and unset  */
/* them.                                                                */
/* -------------------------------------------------------------------- */
#define T_DBG_UTIL_START   0

#define T_DBG_ALLOC        T_DBG_UTIL_START + 0 /* memory management */
#define T_DBG_X            T_DBG_UTIL_START + 1 /* X Window code */
#define T_DBG_HASH         T_DBG_UTIL_START + 2 /* dynamic hash tables */
#define T_DBG_RING         T_DBG_UTIL_START + 3 /* Ring Buffers */
#define T_DBG_LEX          T_DBG_UTIL_START + 4 /* lexical analysis */
#define T_DBG_VAR          T_DBG_UTIL_START + 5 /* patterns (was variables) */
#define T_DBG_CB           T_DBG_UTIL_START + 6 /* callbacks */

/* --------- */
/* IPC Stuff */
/* --------- */
#define T_DBG_IPC_START  20

#define T_DBG_VMS_FD  T_DBG_IPC_START + 1 /* file descrip library (VMS only) */
#define T_DBG_DECNET  T_DBG_IPC_START + 2 /* DECnet IPC (VMS only) */
#define T_DBG_MBX     T_DBG_IPC_START + 3 /* mailbox IPC (VMS only) */
#define T_DBG_TGV     T_DBG_IPC_START + 4 /* TGV MultiNet TCP/IP (VMS only) */
#define T_DBG_UCX     T_DBG_IPC_START + 5 /* DEC UCX TCP/IP (VMS only) */


/* ---------------------------------------------------------------- */
/* IE Stuff.  I guess the IE/DAQ/HCI start could overlap, but don't */
/* bother for now.                                                  */
/* ---------------------------------------------------------------- */
#define T_DBG_IE_START   70

/* --------- */
/* DAQ Stuff */
/* --------- */
#define T_DBG_DAQ_START         90

/* --------- */
/* HCI Stuff */
/* --------- */
#define T_DBG_HCI_START   100 /* start of HCI debug flags */

#define T_DBG_MISC 200 /* stuff that doesn't fit elsewhere */
#define T_DBG_CM   201 /* color message formatter */

#define T_DBG_MASK_SIZE (T_DBG_MISC + 10)  /* leave room for extra flags */

/* ========================================================================= */
/* check if level of debugging is on */
#define T_DBG_QUERY(level) (TutDebugFlags[(level)])
/* ========================================================================= */
/* get pointer to debug flag */
#define T_DBG_PTR(level)   (&TutDebugFlags[(level)])

#ifdef T_NO_DBG

/* no run-time debugging compiled in */
#define T_DBG_NONE()
#define T_DBG_ALL()
#define T_DBG_ON(level)
#define T_DBG_OFF(level)
#define T_DBG(level, fmt)
#define T_DBG_STMT(level, stmt)

#else /* run-time debugging compiled in */

/* ========================================================================= */
/* turn off all debugging */
#define T_DBG_NONE()  memset(TutDebugFlags, FALSE, sizeof(TutDebugFlags))
/* ========================================================================= */
/* turn on all debugging */
#define T_DBG_ALL()  memset(TutDebugFlags, TRUE, sizeof(TutDebugFlags))
/* ========================================================================= */
/* turn on one level of debugging */
#define T_DBG_ON(level)  TutDebugFlags[level] = 1
/* ========================================================================= */
/* turn off one level of debugging */
#define T_DBG_OFF(level)  TutDebugFlags[level] = 0
/* ========================================================================= */
/* print message if level is turned on */
#define T_DBG(level, fmt)  if (TutDebugFlags[level]) TutDebug fmt
/* ========================================================================= */
/* execute statement if level is turned on */
#define T_DBG_STMT(level, stmt)  if (TutDebugFlags[level]) stmt
/* ========================================================================= */

#endif /* T_NO_DBG */

#define T_DBG_QUIET(expr)

#define T_DBG_PERROR(str, num)  TutPerror(str)

#endif /* _RTWORKS_DEBUG_H_ */
