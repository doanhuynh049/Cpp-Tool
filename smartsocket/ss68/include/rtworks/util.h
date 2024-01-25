/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/util.h#6 $
 */

/* =================================================================== */
/*                                                                     */
/*              util.h -- utility library header file                  */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_UTIL_H_
#define _RTWORKS_UTIL_H_


#define T_TRUE  ((T_BOOL)1)
#define T_FALSE ((T_BOOL)0)

#ifdef T_NULL
#undef T_NULL
#endif
#define T_NULL ((T_PTR)0)
/*
 * can't mix function and data ptrs, thus can't use T_NULL.
 */
#define T_NULL_FUNC (0)

#define T_GUID_STR_SIZE (39+1)  /* String GUID length */
/* Define T_GUID for DCE-style Globally Unique Identifiers. --KLC 5/31/98 */
typedef struct T_GUID_STRUCT T_GUID_STRUCT, T_GUID, *T_GUID_PTR;
struct T_GUID_STRUCT {
  T_UINT4 d1;     /* Elements are opaque quantities */
  T_UINT2 d2;
  T_UINT2 d3;
  T_UCHAR d4[8];
};


#define T_ASSIGN_GUID(guid,v1,v2,v3,d40,d41,d42,d43,d44,d45,d46,d47)  \
{                       \
  guid.d1 = (v1); guid.d2 = (v2); guid.d3 = (v3);     \
  guid.d4[0]=(d40);guid.d4[1]=(d41);guid.d4[2]=(d42);guid.d4[3]=(d43);  \
  guid.d4[4]=(d44);guid.d4[5]=(d45);guid.d4[6]=(d46);guid.d4[7]=(d47);  \
}

/* T_SWAP_GUID_BYTES swaps a GUID in place. */
#define T_SWAP_GUID_BYTES(ptr)      \
{           \
  T_CHAR tmp_val;       \
  T_ASSERT(ptr != NULL);      \
  /* Swap the first and fourth bytes. */  \
  tmp_val = ((T_STR)(ptr))[0];      \
  ((T_STR)(ptr))[0] = ((T_STR)(ptr))[3];  \
  ((T_STR)(ptr))[3] = tmp_val;      \
  /* Swap the second and third bytes. */  \
  tmp_val = ((T_STR)(ptr))[1];      \
  ((T_STR)(ptr))[1] = ((T_STR)(ptr))[2];  \
  ((T_STR)(ptr))[2] = tmp_val;      \
  /* Swap the fifth and sixth bytes. */   \
  tmp_val = ((T_STR)(ptr))[4];      \
  ((T_STR)(ptr))[4] = ((T_STR)(ptr))[5];  \
  ((T_STR)(ptr))[5] = tmp_val;      \
  /* Swap the seventh and eighth bytes. */      \
  tmp_val = ((T_STR)(ptr))[6];      \
  ((T_STR)(ptr))[6] = ((T_STR)(ptr))[7];  \
  ((T_STR)(ptr))[7] = tmp_val;      \
}

typedef T_INT4 T_BOOL;   /* for functions that return TRUE/FALSE */
#define T_BOOL_SPEC T_INT4_SPEC /* for printing T_BOOL as integer */

#ifdef T_OS_WINDOWS
typedef SOCKET T_SOCK;
#else
typedef T_INT4 T_SOCK;
#endif

#ifndef T_OS_WINDOWS
typedef T_UINT4 DWORD;
#endif

/* Compute the size in elements of a statically allocated array. */
#define T_ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

/* Round up to nearest 1, 2, 4, 8, etc. May rely on 2's compliment. */
#define T_ROUND_UP(num, base)  (((num) + (base) - 1) & ~((base) - 1))

/* Zero padding bytes. */
#define T_ZERO_PADDING(ptr, size, base) \
if ((size) % (base) != 0) {             \
  memset((ptr) + (size), 0,             \
         (base) - ((size) % (base)));   \
}

/*
 * T_BUF_STRUCT implements a generic resizable buffer.
 */
typedef struct T_BUF_STRUCT T_BUF_STRUCT, *T_BUF;
struct T_BUF_STRUCT {
  /* this is a char pointer so that we can do pointer arithmetic */
  T_STR data;     /* actual data */
  /* restrictions: 0 <= current <= size <= max */
  T_INT4 current;   /* "current" data (specific to use) */
  T_INT4 size;      /* number of bytes used by data */
  T_INT4 max;     /* num. bytes allocated for data */
  T_BOOL statik;    /* TRUE if buffer is fixed-size */
}; /* T_BUF_STRUCT */

#define T_DEFAULT_BUFFER_SIZE (2048)


/* ------------------------------------- */
/* T_GBUF macros -- Get Buffer field */
/* ------------------------------------- */
#define T_GBUF_DATA(buf)        ((buf).data)
#define T_GBUF_CURRENT(buf)     ((buf).current)
#define T_GBUF_CURRENT_PTR(buf) (T_GBUF_DATA(buf) + T_GBUF_CURRENT(buf))
#define T_GBUF_SIZE(buf)        ((buf).size)
#define T_GBUF_SIZE_PTR(buf)    (T_GBUF_DATA(buf) + T_GBUF_SIZE(buf))
#define T_GBUF_MAX(buf)         ((buf).max)
#define T_GBUF_FREE(buf)        (T_GBUF_MAX(buf) - T_GBUF_SIZE(buf))
#define T_GBUF_REMAINING(buf) (T_GBUF_SIZE(buf) - T_GBUF_CURRENT(buf))
#define T_GBUF_STATIC(buf)      ((buf).statik)

#ifdef T_NO_ASSERTS
# define T_SBUF_VAL_WRAP(expr, assert_expr) (expr)
#else
# define T_SBUF_VAL_WRAP(expr, assert_expr) (expr), (assert_expr)
#endif


/* ----------------------------------- */
/* SBUF macros -- Set Buffer field */
/* ----------------------------------- */
#define T_SBUF_DATA(buf, val)         (T_GBUF_DATA(buf) = (val))
#define T_SBUF_CURRENT(buf, val) \
  T_SBUF_VAL_WRAP(T_GBUF_CURRENT(buf) = (val), \
                  T_ASSERT(0 <= T_GBUF_CURRENT(buf))) /* check for wrap */
#define T_SBUF_SIZE(buf, val)  \
  T_SBUF_VAL_WRAP(T_GBUF_SIZE(buf) = (val), \
                  T_ASSERT(0 <= T_GBUF_SIZE(buf))) /* check for wrap */
#define T_SBUF_MAX(buf, val)  \
  T_SBUF_VAL_WRAP(T_GBUF_MAX(buf) = (val), \
                  T_ASSERT(0 <= T_GBUF_MAX(buf))) /* check for wrap */
#define T_SBUF_STATIC(buf, val)       (T_GBUF_STATIC(buf) = (val))


/* ----------------------- */
/* Other buffer macros */
/* ----------------------- */

#define T_BUF_POS(buf, ptr)   (((T_STR)(ptr)) - T_GBUF_DATA(buf))
#define T_BUF_PTR(buf, pos, typ)  ((typ)(T_GBUF_DATA(buf) + (pos)))


#define T_BUF_INIT(buf, init_max) \
{                                 \
  T_SBUF_CURRENT(buf, 0);         \
  T_SBUF_SIZE(buf, 0);            \
  /*T_ASSERT(0 < (init_max));*/   \
  T_SBUF_MAX(buf, init_max);      \
}

#define T_BUF_RESET(buf)  \
{                         \
  T_SBUF_DATA(buf, NULL); \
  T_SBUF_CURRENT(buf, 0); \
  T_SBUF_SIZE(buf, 0);    \
  T_SBUF_MAX(buf, 0);     \
}

#define T_BUF_CHECK_VALID(buf) (T_NULL != T_GBUF_DATA(buf))

/* --------------------------------------------------------------- */
/* T_BUF_CREATE initalizes a buffer with a max size of <init_max>. */
/* --------------------------------------------------------------- */
#define T_BUF_CREATE(buf, init_max)                   \
{                                                     \
  /* should we round up init_max to a power of 2? */  \
  T_MALLOC(T_GBUF_DATA(buf), init_max, T_CHAR *);     \
  T_BUF_INIT(buf, init_max);                          \
  T_SBUF_STATIC(buf, T_FALSE);                        \
}

#define T_BUF_CREATE_STATIC(buf, data, init_max)      \
{                                                     \
  T_SBUF_DATA(buf, data);                             \
  T_BUF_INIT(buf, init_max);                          \
  T_SBUF_STATIC(buf, T_TRUE);                         \
}

/* ---------------------------------------------------------------- */
/* T_TRY_BUF_CREATE is a failure-tolerant versions of T_BUF_CREATE. */
/* ---------------------------------------------------------------- */
#define T_TRY_BUF_CREATE(buf, init_max)               \
{                                                     \
  T_TRY_MALLOC(T_GBUF_DATA(buf), init_max, T_CHAR *); \
  T_BUF_INIT(buf, init_max);                          \
  T_SBUF_STATIC(buf, T_FALSE);                        \
}

/* -------------------------------- */
/* T_BUF_DESTROY destroys a buffer. */
/* -------------------------------- */
#define T_BUF_DESTROY(buf)      \
{                               \
  if (!T_GBUF_STATIC(buf)) {    \
    T_FREE(T_GBUF_DATA(buf));   \
  }                             \
  T_BUF_RESET(buf);             \
}

#define T_TRY_BUF_DESTROY(buf)  \
{                               \
  if (!T_GBUF_STATIC(buf)       \
   && T_BUF_CHECK_VALID(buf)) { \
    T_FREE(T_GBUF_DATA(buf));   \
  }                             \
  T_BUF_RESET(buf);             \
}

/* -------------------------------------------------------------------- */
/* T_BUF_RECYCLE re-uses the storage already allocated for a buffer if  */
/* it's already at least <size> bytes.                                  */
/* -------------------------------------------------------------------- */
#define T_BUF_RECYCLE(buf, size)                        \
{                                                       \
  if (T_GBUF_STATIC(buf) || (size) > T_GBUF_MAX(buf)) { \
    T_BUF_DESTROY(buf);                                 \
    T_BUF_CREATE(buf, size);                            \
  }                                                     \
  else {                                                \
    T_SBUF_CURRENT(buf, 0);                             \
    T_SBUF_SIZE(buf, 0);                                \
  }                                                     \
}

#define T_TRY_BUF_RECYCLE(buf, size)                    \
{                                                       \
  if (T_GBUF_STATIC(buf) || (size) > T_GBUF_MAX(buf)) { \
    T_BUF_DESTROY(buf);                                 \
    T_TRY_BUF_CREATE(buf, size);                        \
  }                                                     \
  else {                                                \
    T_SBUF_CURRENT(buf, 0);                             \
    T_SBUF_SIZE(buf, 0);                                \
  }                                                     \
}

/* ------------------------------------------------------------------------ */
/* T_BUF_RESIZE resizes a buffer so that it can hold at least <desired_max> */
/* bytes of data.                                                           */
/* ------------------------------------------------------------------------ */

/* NOTE
 * T_BUF_RESIZE_CALC_MAX is for use in T_[TRY_]BUF_RESIZE macros, and should
 * not be used elsewhere.
 */

#if 1

# define T_BUF_RESIZE_CALC_MAX(buf, desired_max)                            \
  T_REAL8 resize_factor, resize_value;                                      \
  T_ASSERT(!T_GBUF_STATIC(buf));                                            \
  T_ASSERT(T_GBUF_MAX(buf) > 0);                                            \
  T_SBUF_MAX(buf, desired_max);                                             \
  resize_factor = TutBufGetResizeFactor();                                  \
  resize_value = 1.0 + resize_factor/log10((double)T_GBUF_MAX(buf));        \
  T_SBUF_MAX(buf, T_ROUND_UP((T_INT4)(T_GBUF_MAX(buf) * resize_value),16));

#else

# define T_BUF_RESIZE_CALC_MAX(buf, desired_max)                            \
  T_REAL8 resize_value;                                                     \
  T_ASSERT(!T_GBUF_STATIC(buf));                                            \
  T_ASSERT(T_GBUF_MAX(buf) > 0);                                            \
  T_SBUF_MAX(buf, desired_max);                                             \
  resize_value = (T_GBUF_MAX(buf) <= 100 * 1024) ? 1.25 :                   \
                 (T_GBUF_MAX(buf) <= 1024 * 1024) ? 1.15 :                  \
                 (T_GBUF_MAX(buf) <= 10 * 1024 * 1024) ? 1.09 : 1.05;       \
  T_SBUF_MAX(buf, (T_INT4)(T_GBUF_MAX(buf) * resize_value));                \

#endif

#define T_BUF_RESIZE(buf, desired_max)                      \
{                                                           \
  T_BUF_RESIZE_CALC_MAX(buf, desired_max);                  \
  T_REALLOC(T_GBUF_DATA(buf), T_GBUF_MAX(buf), T_CHAR *);   \
}

#define T_TRY_BUF_RESIZE(buf, desired_max)                  \
{                                                           \
  T_PTR data = T_GBUF_DATA(buf);                            \
  T_INT4 old_max = T_GBUF_MAX(buf);                         \
  T_BUF_RESIZE_CALC_MAX(buf, desired_max);                  \
  data = TutTryRealloc(T_GBUF_DATA(buf), T_GBUF_MAX(buf));  \
  if (T_NULL == data) {                                     \
    T_SBUF_MAX(buf, old_max);                               \
  }                                                         \
  else {                                                    \
    T_SBUF_DATA(buf, (T_STR)data);                          \
  }                                                         \
}

/*
 * T_BUF_ALIGN_{CURRENT,SIZE} aligns the appropriate offset to an
 * align-byte boundary-- with no error checking.
 */
#define T_BUF_ALIGN_CURRENT(buf, align)         \
{                 \
  T_SBUF_CURRENT((buf), T_ROUND_UP(T_GBUF_CURRENT(buf), (align)));  \
}

#define T_BUF_ALIGN_SIZE(buf, align)          \
{                 \
  T_SBUF_SIZE((buf), T_ROUND_UP(T_GBUF_SIZE(buf), (align)));    \
}

/*
 * T_BUF_APPEND_PREALIGNED sets ptr to an area of sizeof(*ptr) bytes
 * and adjusts the buffer's size accordingly.  Use with caution-- any
 * attempt to grow a static buffer will trigger a T_ASSERT!
 */
#define T_BUF_APPEND_PREALIGNED(buf, ptr, type)       \
{                 \
  if (T_GBUF_FREE(buf) < (T_INT4)sizeof(*(ptr))) {      \
    T_BUF_RESIZE(buf, T_GBUF_SIZE(buf) + (T_INT4)sizeof(*(ptr))); \
  }                 \
  (ptr) = (type)T_GBUF_SIZE_PTR(buf);         \
  T_GBUF_SIZE(buf) += (T_INT4)sizeof(*(ptr));       \
  T_ASSERT(0 <= T_GBUF_SIZE(buf)); /* check for wrap */     \
} /* T_BUF_APPEND_PREALIGNED */


/*
 * T_BUF_APPEND_PREALIGNED_ARRAY sets ptr to an area of
 * count * sizeof(*ptr) bytes and adjusts the buffer's size.  Again,
 * the same caution as above applies.
 */
#define T_BUF_APPEND_PREALIGNED_ARRAY(buf, ptr, count, type)        \
{                     \
  if (T_GBUF_FREE(buf) < (count) * (T_INT4)sizeof(*(ptr))) {        \
    T_BUF_RESIZE(buf, T_GBUF_SIZE(buf) + (count) * (T_INT4)sizeof(*(ptr))); \
  }                     \
  (ptr) = (type)T_GBUF_SIZE_PTR(buf);             \
  T_GBUF_SIZE(buf) += (count) * (T_INT4)sizeof(*(ptr));         \
  T_ASSERT(0 <= T_GBUF_SIZE(buf)); /* check for wrap */         \
} /* T_BUF_APPEND_PREALIGNED_ARRAY */


/* ----------------------------------------------------------------- */
/* T_BUF_APPEND_SCALAR appends a scalar <data> of type <type>.  Note */
/* that we need the third parameter to ensure that the data is       */
/* written in the proper size (such as a short).                     */
/* T_BUF_APPEND_SCALAR does not do any aligning of <data>.           */
/* ----------------------------------------------------------------- */
#define T_BUF_APPEND_SCALAR(buf, data, type)        \
{                 \
  if (T_GBUF_SIZE(buf) + (T_INT4)sizeof(type) > T_GBUF_MAX(buf)) {  \
    T_BUF_RESIZE(buf, T_GBUF_SIZE(buf) + (T_INT4)sizeof(type));   \
  } /* if */                \
  *(type *)T_GBUF_SIZE_PTR(buf) = (type)data;       \
  T_GBUF_SIZE(buf) += (T_INT4)sizeof(type);       \
  T_ASSERT(0 <= T_GBUF_SIZE(buf)); /* check for wrap */     \
} /* T_BUF_APPEND_SCALAR */


/* ------------------------------------------------------------------ */
/* Name of environment that defines top-level directory for RTworks.  */
/* ------------------------------------------------------------------ */
#define T_HOME_DIR_NAME "RTHOME"

/*
 * On VMS (VAX C), we have to use the VMS-specific globalref and
 * globaldef storage-class specifiers.  (SMW 08/09/96: From what I
 * remember, the default was to generate a shared psect/symbol, which
 * meant that global variables in shareable images were like shared
 * memory: this was easy to detect because when you tried to run a
 * program linked with such a shared library, VMS would complain that
 * the shareable image had to be installed writable).
 *
 * We've been using DEC C 4.0 for a while with globalref and
 * globaldef, and are now trying to use DEC C++ 5.3 and maybe DEC C
 * 5.3, too.
 *
 * In DEC C 4.X and above, globalref and globaldef exist only for
 * compatibility with VAX C, and are only available with the VAX C
 * compatibility mode (which is included with the default
 * /standard=relaxed_ansi89).  FWIW, /standard=ansi89 is pretty bogus
 * on VMS because it complains about things like SS$_NORMAL being
 * non-standard.  Thus for DEC C we have been pretty safe to keep
 * using globalref and globaldef.
 *
 * DEC C++ 5.3 does not seem to support globalref and globaldef at
 * all.  Thus the global variables in our C code (mainly the UTIL lib
 * for now) are a problem.
 *
 * The default for DEC C 4.0 and above seems to be to not treat global
 * variable like VAX C did (DEC C calls the old way the "common block"
 * model).  Thus it would seem that we really don't need to use
 * globaldef and globalref anymore (the doc says we will get more
 * psects, however by experimenting with link/map/full on OpenVMS VAX
 * 6.2 and DEC C 4.0. I only see psects if the global variable is
 * initialized at compile time)!  Longer term, we need to get rid of
 * the global variables in all libraries that we want to make
 * multithreaded.
 *
 * Chapters 4 and 5 of the DEC C 5.2 User's Guide have a good discussion
 * of all this stuff.  The header files ctype.h and errno.h are also
 * very illuminating.  SMW 08/09/96
 */
#ifdef T_OS_VMS_OBSOLETE
# define T_GLOBALREF globalref
# define T_GLOBALDEF globaldef noshare
#else
# if defined(T_OS_WINDOWS) && !defined(T_STATIC_LINK)
#  ifdef T_UT_SOURCE
#   define T_GLOBALREF extern _declspec(dllexport)
#   define T_GLOBALDEF _declspec(dllexport)
#  else
#   define T_GLOBALREF _declspec(dllimport)
#   define T_GLOBALDEF _declspec(dllexport)
#  endif
# else
#  define T_GLOBALREF extern
#  define T_GLOBALDEF
# endif
#endif

/* --------------------------------------------------------------- */
/* If T_UT_GLOBAL isn't defined, define it to be T_GLOBALREF.  The */
/* IPC lib, IE lib, DAQ lib, and HCI lib have similar defines.     */
/* --------------------------------------------------------------- */
#ifndef T_UT_GLOBAL
# define T_UT_GLOBAL T_GLOBALREF
#endif

/* ------------------------------------------------------------------- */
/* T_PROTO is used to produce function prototypes that can be disabled */
/* for compilers without prototypes.                                   */
/* ------------------------------------------------------------------- */
#ifdef T_HAS_PROTOTYPES
# define T_PROTO(args)  args
#else
# define T_PROTO(args)  ()
#endif

/* --------------------------------------------------------- */
/* This macro makes it easier to declare function prototypes */
/* in typedefs.                                              */
/* --------------------------------------------------------- */
#define T_TYPEDEF(args) T_PROTO(args)

/* ------------------------------------------ */
/* Define exit codes for failure and success. */
/* ------------------------------------------ */
#ifdef T_OS_VMS
# define T_EXIT_SUCCESS 1
#else
# define T_EXIT_SUCCESS 0
#endif
#define T_EXIT_FAILURE 2

/* ------------------------------------------------------------------- */
/* T_EXIT_MAIN can be used to get compilers to shut up that don't know */
/* that exit() never returns.  We need to use exit on VMS though.      */
/* ------------------------------------------------------------------- */
#ifdef T_OS_VMS
# define T_EXIT_MAIN(code)         T_C_EXIT((code))
#else
# define T_EXIT_MAIN(code)         return (code)
#endif

/* --------------------------------------------------------- */
/* Return # of elements in a non-dynamically-allocated array */
/* --------------------------------------------------------- */
#define T_ARRAY_ELEMENTS(array) (sizeof(array) / sizeof(array[0]))

/* -------------------------------------------------- */
/* T_STRLEN safely determines the lenght of a string. */
/* -------------------------------------------------- */
#define T_STRLEN(str) (T_NULL == str ? 0 : strlen(str))
#define T_STRFP(str)  (T_NULL == str ? 0 : strlen(str)+1)

/* ---------------------------------------------------------------- */
/* T_STRDUP can be used to dynamically allocate a copy of a string. */
/* ---------------------------------------------------------------- */
#define T_STRDUP(dest, src)                   \
{                                             \
  T_MALLOC((dest), strlen(src) + 1, char *);  \
  strcpy((dest), (src));                      \
}

/* --------------------------------------------------------------------- */
/* T_STRDCAT can be used to dynamically allocate a string concatenation. */
/* --------------------------------------------------------------------- */
/* HACK
 * This cast is ugly and undesirable, but T_MALLOC only takes T_INT4; better
 * hope we never have to allocate a string bigger than 2 GB!! - MW 2006-04-27
 */
#define T_STRDCAT(dest, src1, src2)                     \
{                                                       \
  T_NINT _l1 = T_STRLEN(src1), _l2 = T_STRLEN(src2);    \
  T_MALLOC((dest), (T_INT4)(_l1+_l2+1), char *);        \
  *dest = 0;                                            \
  if (T_NULL != src1) { strncat((dest), (src1), _l1); } \
  if (T_NULL != src2) { strncat((dest), (src2), _l2); } \
}

/* ------------------------------------------------------------------------ */
/* T_STRCOPY can be used to dynamically copy a string and free the old one. */
/* ------------------------------------------------------------------------ */
#define T_STRCOPY(dest, src)        \
{             \
  if (dest != T_NULL) {         \
    T_FREE(dest);         \
  }             \
  if (src == T_NULL) {          \
    /* cast needed for SunOS C++.  SMW BM 07/25/97 */ \
    (dest) = (T_STR)T_NULL;       \
  }             \
  else {            \
    T_STRDUP(dest, src);        \
  }             \
} /* T_STRCOPY */

/* -------------------------------------------------------------------- */
/* T_STRCOPY_NON_NULL can be used to dynamically copy a non-NULL string */
/* and free the old one.                                                */
/* -------------------------------------------------------------------- */
#define T_STRCOPY_NON_NULL(dest, src)   \
{           \
  T_ASSERT(T_NULL != src);      \
  if (dest != T_NULL) {       \
    T_FREE(dest);       \
  }           \
  T_STRDUP(dest, src);        \
} /* T_STRCOPY_NON_NULL */


/* -------------------------------------------------------------------- *
 * T_STRCOPY_OR_NULL sets dest to NULL if src is an empty string.  A  *
 * free is done on any previous contents of dest.     *
 * -------------------------------------------------------------------- */
#define T_STRCOPY_OR_NULL(dest, src)      \
{             \
  if (T_NULL != (dest)) {       \
    T_FREE(dest);         \
  }             \
  if (T_NULL == (src) || '\0' == *((T_STR)src)) { \
    /* cast needed for SunOS C++.  SMW BM 07/25/97 */ \
    (dest) = (T_STR)T_NULL;       \
  }             \
  else {            \
    T_STRDUP(dest, src);        \
  }             \
} /* T_STRCOPY_OR_NULL */


/* --------------------------------------------------------------------- */
/* LEGACY                                                                */
/* T_QUOTE_STRING tries to do preprocessor quoting of strings that works */
/* with both ANSI and non-ANSI compilers. However, by now, '#' ought to  */
/* work with all our compilers (and we're already assuming it does in    */
/* some places), so don't use macro. - MW 2005-12-20                     */
/*                                                                       */
/* Note that T_QUOTE_STRING can have issues with macro nesting.          */
/* --------------------------------------------------------------------- */
#if defined(T_ANSI_C) || defined(T_OS_IRIX)
# define T_QUOTE_STRING(str)  #str
#else
# define T_QUOTE_STRING(str)  "str"
#endif

/* ------------------------------------------------------------------------- */
/* T_MALLOC, T_CALLOC, and T_REALLOC are macros that do checking for NULL.   */
/* Use helper functions Tut{M|C|Re}allocCheck to make object code coverage   */
/* tools (like PureCoverage) more accurate.  SMW 03/04/95                    */
/* ------------------------------------------------------------------------- */

/* NOTE
 * Support for lint and VxWorks is removed as of 6.8 (file revision #34).
 * Refer to a previous branch/revision if you need this code.
 */
#define T_MALLOC(var, size, type) \
  (var) = (type)TutMallocCheck((T_UINT4)(size), #var, __FILE__, __LINE__)
#define T_CALLOC(var, size, type) \
  (var) = (type)TutCallocCheck((T_UINT4)(size), #var, __FILE__, __LINE__)
#define T_REALLOC(var, size, type) \
  (var) = (type)TutReallocCheck((var), (T_UINT4)(size), \
                                #var, __FILE__, __LINE__)
#define T_FREE(var) {                            \
  TutFreeCheck((var), #var, __FILE__, __LINE__); \
  (var) = NULL;                                  \
}
#define T_CFREE(var)   T_FREE(var)

#if defined(T_OS_WINDOWS) || defined(T_OS_LINUX)
/* Cast to T_PTR for OS's that bitch about the 'volatile' */
# define T_FREE_VOLATILE(var) {                          \
  TutFreeCheck((T_PTR)(var), #var, __FILE__, __LINE__); \
  (var) = NULL;                                         \
}
#else
# define T_FREE_VOLATILE(var) T_FREE(var)
#endif

#define T_TRY_MALLOC(var, size, type) (var) = (type)TutTryMalloc((size))
#define T_TRY_CALLOC(var, size, type) (var) = (type)TutTryCalloc((size))

#define T_C_MALLOC(size)            malloc(size)
#define T_C_CALLOC(numelem, size)   calloc(numelem, size)
#define T_C_REALLOC(ptr, size)      realloc(ptr, size)
#define T_C_FREE(ptr)               free(ptr)

/* ----------------------------------------------------------------- */
/* T_FREE_IF_NON_NULL handles cases where it is OK if value is NULL. */
/* ----------------------------------------------------------------- */
#define T_FREE_IF_NON_NULL(var) if ((var) != T_NULL) { T_FREE(var); }

/* ------------------------------------------------------------ */
/* T_[TRY_]NEW[C] are convenience macros for Scott, who keeps   */
/* forgetting what arguments T_MALLOC takes :-).                */
/* ------------------------------------------------------------ */
#define T_NEW(s)      T_MALLOC(s, sizeof(*s), T_PTR)
#define T_NEWC(s)     T_CALLOC(s, sizeof(*s), T_PTR)
#define T_TRY_NEW(s)  T_TRY_MALLOC(s, sizeof(*s), T_PTR)
#define T_TRY_NEWC(s) T_TRY_CALLOC(s, sizeof(*s), T_PTR)

/* ------------------------------------------------------------ */
/* T_CLEAR clears a structure (convenience macro for Scott, who */
/* keeps forgetting what arguments memset takes :-).            */
/* ------------------------------------------------------------ */
#define T_CLEAR(ptr) memset((T_STR)ptr, '\0', sizeof(*ptr))

#define T_IN_RANGE(n, lo, hi)   ((lo) <= (n) && (n) <= (hi))

/* --------------------------- */
/* Maximum and Minimum Numbers */
/* --------------------------- */
#define T_MAX(a,b)     ((a) > (b) ? (a) : (b))
#define T_MIN(a,b)     ((a) < (b) ? (a) : (b))
#if defined T_OS_WIN64
# define T_LABS(a) _abs64(a)
#else
# define T_LABS(a) labs(a)
#endif

#define T_FTOA(floatval, buffer) strcpy((buffer), TutRealToStr((floatval)))
#define T_ITOA(intval, buffer) sprintf((buffer), T_INT4_SPEC, (T_INT4)(intval))

#define T_ITOAN(intval, buffer, bufsize) \
  T_C_SNPRINTF((buffer), (bufsize), T_INT4_SPEC, (T_INT4)(intval))

/* --------------------------------------------------------------- */
/* Character Conversion Macros -- These are needed for portability */
/* --------------------------------------------------------------- */
#define T_TOLOWER(c)  (isupper(c) ? ((c) - 'A' + 'a') : (c))
#define T_TOUPPER(c)  (islower(c) ? ((c) - 'a' + 'A') : (c))

/* -------------------------- */
/* State manipulation macros. */
/* -------------------------- */
#define T_STATE_SET(state, value) (state) = (value)
#define T_STATE_GET(state)        (state)

/* ------------------------------------- */
/* Macro for checking if an id is a slot */
/* ------------------------------------- */
#define T_ID_IS_SLOT(id)        (strchr(id, '.') != NULL)

/* ---------------------------------------------------------------------- */
/* String comparison macros.  These macros used to try to special case    */
/* comparison of the first character (idea gotten from old blue Plum C    */
/* book).  The special case code screws up code coverage tools (by adding */
/* more branches).  The special case code was faster when the first       */
/* character does match (or not match), but slower otherwise.  I think    */
/* the "otherwise" will be more common.  I also think we will be better   */
/* off trying to apply code coverage testing techniques than              */
/* micro-performance-optimiziation techniques.  SMW 03/04/95              */
/* ---------------------------------------------------------------------- */
#define T_STREQ(s1,s2)  (strcmp(s1,s2) == 0)
#define T_STRNEQ(s1,s2)  (strcmp(s1,s2) != 0)

#define T_STREQN(s1,s2,n)  (strncmp(s1,s2,n) == 0)
#define T_STRNEQN(s1,s2,n)  (strncmp(s1,s2,n) != 0)

#define T_STREQI(s1,s2)  (TutStrCmpi(s1,s2) == 0)
#define T_STRNEQI(s1,s2)  (TutStrCmpi(s1,s2) != 0)

#define T_STREQIN(s1,s2,n)  (TutStrNCmpi(s1,s2,n) == 0)
#define T_STRNEQIN(s1,s2,n)  (TutStrNCmpi(s1,s2,n) != 0)

#define T_STRLT(s1,s2)  (strcmp(s1,s2) < 0)
#define T_STRGT(s1,s2)  (strcmp(s1,s2) > 0)

#define T_STRGTEQ(s1,s2)  (strcmp(s1,s2) >= 0)
#define T_STRLTEQ(s1,s2)  (strcmp(s1,s2) <= 0)

/* String prefix macros - s2 must be a literal */
/* TODO - replace internal use of TutStr[i]StartsWith with T_STRPFX */
#define T_STRPFX(s1,s2)  T_STREQN(s1,s2,sizeof(s2)-1)
#define T_STRPFXI(s1,s2)  T_STREQIN(s1,s2,sizeof(s2)-1)

/* -------------- */
/* Boolean macros */
/* -------------- */
#define T_IS_BOOLEAN(var)  ((var) == TRUE || (var) == FALSE)
#define T_BOOLEAN_TO_STRING(var)  ((var) ? "TRUE" : "FALSE")

#define T_MAXSTRSIZE 255 /* max # of chars in a "string" */
typedef T_CHAR T_STRING[T_MAXSTRSIZE + 1];

#define T_MAXIDSIZE 63 /* max # of chars in an identifier */
typedef T_CHAR T_ID[T_MAXIDSIZE + 1];

/* ----------------------- */
/* Function for TutAtExit. */
/* ----------------------- */
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_AT_EXIT_FUNC) T_TYPEDEF((void));

/*
 * Linked list of funcion pointer which would be used by our
 * TutExit to call atexit handlers in reverse order
 * KRM & BM 06/19/98
 */
typedef struct T_EXIT_FUNC_ELEMENT_STRUCT T_EXIT_FUNC_ELEMENT_LIST_STRUCT, *T_EXIT_FUNC_ELEMENT;
struct T_EXIT_FUNC_ELEMENT_STRUCT {
   T_EXIT_FUNC_ELEMENT next;
   T_AT_EXIT_FUNC func_ptr;
};

/* ---------------------- */
/* Linked list of strings */
/* ---------------------- */
typedef struct T_STR_LIST_STRUCT T_STR_LIST_STRUCT, *T_STR_LIST;
struct T_STR_LIST_STRUCT {
  T_STR_LIST next;
  T_STR string;
};
typedef struct T_CSTR_LIST_STRUCT T_CSTR_LIST_STRUCT, *T_CSTR_LIST;
struct T_CSTR_LIST_STRUCT {
  T_CSTR_LIST next;
  T_CSTR string;
};
#define T_STRINGNODE T_STR_LIST_STRUCT
#define T_STRING_LIST_STRUCT T_STR_LIST_STRUCT
#define T_STRING_LIST T_STR_LIST

/* ----------------------- */
/* Linked list of integers */
/* ----------------------- */
typedef struct T_INT4_LIST_STRUCT T_INT4_LIST_STRUCT, *T_INT4_LIST;
struct T_INT4_LIST_STRUCT {
  T_INT4_LIST next;
  T_INT4 inum;
};
#define T_INTNODE T_INT4_LIST_STRUCT

/* ----------------------- */
/* Linked list of doubles. */
/* ----------------------- */
typedef struct T_NUMNODE T_NUMNODE;
struct T_NUMNODE {
  T_NUMNODE *next;
  T_REAL8 value;
};

typedef struct T_RANDOMNESS_STRUCT T_RANDOMNESS_STRUCT, *T_RANDOMNESS_PTR;
struct T_RANDOMNESS_STRUCT {
  T_REAL8 wall_time1;
  T_PTR current_sbrk;
  T_PTR initial_sbrk;
  T_INT4 cpu_time;
  T_CHAR node_name[256];
  T_STRING arch;
  T_PTR ptr;
#if defined(T_OS_UNIX)
  long total_phys_pages;
  long total_phys_pages_not_used;
#endif
  T_REAL8 wall_time2;
};

typedef struct T_ENCRYPT_KEY_STRUCT T_ENCRYPT_KEY_STRUCT, *T_ENCRYPT_KEY;
struct T_ENCRYPT_KEY_STRUCT {
  T_PTR key;
  T_INT4 key_size;
};

/*
 * T_PATH_FILE_SEP is the separator between directory paths and file names.
 * T_PATH_DIR_SEP is the separator between directories in a path.
 */
#if defined(T_OS_UNIX)
# define T_PATH_FILE_SEP "/"
# define T_PATH_DIR_SEP "/"
#elif defined(T_OS_WINDOWS)
# define T_PATH_FILE_SEP "\\"
# define T_PATH_DIR_SEP "\\"
#elif defined(T_OS_VMS)
# define T_PATH_FILE_SEP "" /* no extra separator needed */
# define T_PATH_DIR_SEP "."
#else
T_PATH_*_SEP unknown OS
#endif

/* For compatibility with old definition... --KLC 5/30/98 */
#define T_DIR_FILE_SEP T_PATH_FILE_SEP

/* defines for TutReadDir */
typedef enum {
  T_FILE_TYPE_ALL,
  T_FILE_TYPE_DIR,
  T_FILE_TYPE_REG
} T_FILE_TYPE;

#if defined (T_OS_WIN32) && defined (_MAX_PATH)
# define T_MAX_PATH_LEN _MAX_PATH
#elif defined (PATH_MAX)
# if (PATH_MAX) > 8192
#   define T_MAX_PATH_LEN 8192
# elif defined (MAXPATHLEN)
#  if (MAXPATHLEN) > (PATH_MAX)
#   define T_MAX_PATH_LEN (MAXPATHLEN)
#  else
#   define T_MAX_PATH_LEN (PATH_MAX)
#  endif
# else
#  define T_MAX_PATH_LEN (PATH_MAX)
# endif
#elif defined (MAXPATHLEN)
# define T_MAX_PATH_LEN (MAXPATHLEN)
#else
# define T_MAX_PATH_LEN 1024
#endif

typedef struct T_DIRENT_STRUCT T_DIRENT_STRUCT, *T_DIRENT;
struct T_DIRENT_STRUCT {
#if defined (T_OS_UNIX)
  T_CHAR name[MAXPATHLEN]; /* either dirname of file name */
#elif defined (T_OS_WIN32)
  T_CHAR name[_MAX_PATH];
#endif
  T_FILE_TYPE type; /* type of file [dir|| file] */
};

typedef struct T_DIR_INTERNAL_STRUCT T_DIR_INTERNAL, *T_DIR;
struct T_DIR_INTERNAL_STRUCT {
  T_STR dir_name;
#if defined T_OS_WIN32
  HANDLE handle;
  T_BOOL open_dir_called;
#elif defined T_OS_UNIX
  DIR* dirp;
#endif
};

/*
 * These registry keys are used to search for fallback environment variable
 * settings in the Windows registry. The software specific registry keys
 * are used to search for fallback environment variables if we don't find
 * it in common place.
 */
#define T_REGISTRY_KEY "SOFTWARE\\TIBCO Software Inc.\\Common"
#define T_SS_REGISTRY_KEY "Software\\TIBCO Software Inc.\\SmartSockets"
#define T_MQX_REGISTRY_KEY "Software\\TIBCO Software Inc.\\SmartMQ"

/*
 * Fake some data types so cproto will work.  SMW 09/25/95
 */
#if defined(T_GENERATE_PROTOTYPES) && defined(T_OS_WIN32)
typedef int BOOL;
typedef int HWND;
typedef int SOCKET;
typedef int HKEY;
typedef struct in_addr T_IN_ADDR_STRUCT;
#endif


/*
 * For src/util/loadlib.c for utilities for dynamic library loading
 * ..BM 06/11/98
 */
#ifdef T_OS_WIN32
typedef HINSTANCE T_LIBRARY;
typedef HMODULE   T_LIBRARY;
typedef FARPROC   T_FUNCTION_PTR;
#else
typedef T_PTR T_LIBRARY;
typedef T_PTR T_FUNCTION_PTR;
#endif

/*
 * Compression data types.
 */
typedef struct T_COMPRESSOR_STRUCT
               T_COMPRESSOR_STRUCT,
              *T_COMPRESSOR;

typedef struct T_COMPRESSOR_STATS_STRUCT {
  T_STR   name;
  T_STR   version;
  T_UINT4 bytes_in;
  T_UINT4 bytes_out;
} T_COMPRESSOR_STATS_STRUCT, *T_COMPRESSOR_STATS;

typedef enum {
  T_COMPRESSOR_FLUSH_NONE,
  T_COMPRESSOR_FLUSH_SYNC,
  T_COMPRESSOR_FLUSH_FINAL
} T_COMPRESSOR_FLUSH;

/*
 * Compression library entry points.
 */
#define T_COMPRESSOR_INIT_FUNC_NAME "TcompressorDriverInit"

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_COMPRESSOR_INIT_FUNC)
T_TYPEDEF((void));

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_COMPRESSOR_CREATE_FUNC)
T_TYPEDEF((T_STR /* args */));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_COMPRESSOR_COMPRESS_FUNC)
T_TYPEDEF((T_PTR,             /* context */
           T_BUF,             /* dest    */
           T_BUF,             /* src     */
           T_COMPRESSOR_FLUSH /* flush   */));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_COMPRESSOR_UNCOMPRESS_FUNC)
T_TYPEDEF((T_PTR,             /* context */
           T_BUF,             /* dest    */
           T_BUF,             /* src     */
           T_COMPRESSOR_FLUSH /* flush   */));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_COMPRESSOR_GET_STATS_FUNC)
T_TYPEDEF((T_PTR,             /* context    */
           T_COMPRESSOR_STATS /* statistics */));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_COMPRESSOR_RESET_FUNC)
T_TYPEDEF((T_PTR /* context */));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_COMPRESSOR_DESTROY_FUNC)
T_TYPEDEF((T_PTR /* context */));

/* --------------------------------- */
/* Structure for table of open files */
/* --------------------------------- */
typedef struct T_FILE_STRUCT T_FILE_STRUCT, *T_FILE;
struct T_FILE_STRUCT {
  T_FILE_STRUCT *left;  /* for deque routines */
  T_FILE_STRUCT *right; /* for deque routines */
  T_STR fname;          /* file name */
  T_STR mode;           /* mode the file was opened in */
  FILE *file;           /* stdio file pointer */
  T_INT4 ref_count;     /* reference count */
  T_INT4 size;          /* file size */
};

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_FILE_TRAV_FUNC) T_TYPEDEF((T_FILE /*file*/, T_PTR /*arg*/));
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_DAEMON_MAIN_FUNC)
  T_TYPEDEF((
       T_INT4,    /* argc */
       T_STR *    /* argv */
     ));
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_DAEMON_INIT_FUNC)
  T_TYPEDEF((
       T_INT4,    /* argc */
       T_STR *    /* argv */
     ));
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_DAEMON_START_FUNC)T_TYPEDEF((void));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_DAEMON_STOP_FUNC)T_TYPEDEF((void));
typedef struct T_DAEMON_INFO_STRUCT T_DAEMON_INFO_STRUCT, *T_DAEMON_INFO;

struct T_DAEMON_INFO_STRUCT {
  T_DAEMON_INIT_FUNC init_func;
  T_DAEMON_START_FUNC start_func;
  T_DAEMON_STOP_FUNC stop_func;
  T_INT4 init_argc;
  T_STR *init_argv;
};

typedef FILE *(T_ENTRY1 * T_ENTRY2 T_EXPORT T_FILE_OPEN_FUNC) T_TYPEDEF((T_CONST T_STR /*fname*/,
               T_CONST T_STR /*mode*/));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_FILE_CLOSE_FUNC) T_TYPEDEF((FILE * /*strm*/));
typedef T_INT4 (T_ENTRY1 * T_ENTRY2 T_EXPORT T_FILE_READ_FUNC) T_TYPEDEF((T_PTR /*buff*/,
                T_UINT4 /*size*/,
                FILE * /*strm*/));
/* ----------------------------------------- */
/* Safe strings -- strings that grow to fit. */
/* ----------------------------------------- */
#define T_SAFE_STR_QUANTUM 256 /* number of bytes to grow safe strings by */

typedef struct T_SAFE_STR_STRUCT T_SAFE_STR_STRUCT, *T_SAFE_STR;

struct T_SAFE_STR_STRUCT {
  T_STR str; /* C string (NUL terminated) */
  T_INT4 size; /* number of bytes allocated for str field */
  T_INT4 length; /* length of str field */
};

/* -------------------------------------------------------------- */
/* T_SAFE_STR_CREATE takes a pointer and allocates a safe string. */
/* -------------------------------------------------------------- */
#define T_SAFE_STR_CREATE(ptr) \
{ \
  T_MALLOC(ptr, sizeof(T_SAFE_STR_STRUCT), T_SAFE_STR_STRUCT *); \
  T_ASSERT(ptr != NULL); \
  *ptr = TutSafeStringCreate(); \
} /* T_SAFE_STR_CREATE */

#define T_GSAFE_STR(ptr)    (ptr)->str

/* ----------------------------------------------- */
/* utility types -- common types between processes */
/* ----------------------------------------------- */
typedef struct T_USER_TYPE_STRUCT T_USER_TYPE_STRUCT, *T_USER_TYPE;

/*
 * T_USER_TYPE_ASSIGN_FUNC is for assigning one user type to another
 * where both are passed in as ptrs to the user type
 */
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_USER_TYPE_ASSIGN_FUNC) T_TYPEDEF
((T_PTR *, /* target */
  T_PTR *, /* source */
  T_UINT4 /* modifier */));

/*
 * T_USER_TYPE_RETURN_FUNC is for calling the return function "method"
 */
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_USER_TYPE_RETURN_FUNC) T_TYPEDEF
((T_USER_TYPE,  /* uttype */
  T_PTR,  /* user-defined argument */
  T_PTR * /* value being returned */ ));

/*
 * T_USER_TYPE_TRAV_FUNC is for traversing all user-defined types
 */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_USER_TYPE_TRAV_FUNC) T_TYPEDEF
((T_STR    /* name */,
  T_USER_TYPE /* user type (opaque)*/,
  T_PTR    /* arg */ ));

/*
 * utility user-defined functions
 */
typedef struct T_USER_FUNC_STRUCT T_USER_FUNC_STRUCT, *T_USER_FUNC;

/*
 * For traversing the instances of a user-defined function
 */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_USER_FUNC_TRAV_FUNC) T_TYPEDEF
((T_USER_FUNC /* instance name */,
  T_PTR      /* arg */ ));

/*
 *  For traversing all user-defined function names
 */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_USER_FUNC_TRAV_ALL_FUNC) T_TYPEDEF
((T_STR      /* function name */,
  T_PTR      /* arg */ ));

/*
 * Definition for how the C code should look for user functions
 */
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_USER_FUNC_C_FUNC) T_TYPEDEF
((T_USER_FUNC /* instance name */,
  T_PTR      /* arg */ ));

/*
 * pointer arrays -- arrays of pointers that grow to fit.
 */
typedef struct T_PTR_ARY_STRUCT T_PTR_ARY_STRUCT, *T_PTR_ARY;

/*
 * Other pointer array functions
 */
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_PTR_ARY_USER_FUNC) T_TYPEDEF
((T_INT4 /* index */,
  T_PTR /* item */,
  T_PTR /* arg */));

typedef T_INT4 (T_ENTRY1 * T_ENTRY2 T_EXPORT T_PTR_ARY_CMP_FUNC) T_TYPEDEF
((T_PTR /* item 1*/,
  T_PTR /* item 2*/));

/*
 * graphs - vertices and edges
 */
/* Opaque graph descriptor */
typedef struct T_GRAPH_STRUCT T_GRAPH_STRUCT, *T_GRAPH;

/* Vertex node in a graph */
typedef struct T_GRAPH_VERTEX_STRUCT T_GRAPH_VERTEX_STRUCT, *T_GRAPH_VERTEX;

/* Edge node in an adjacency list */
typedef struct T_GRAPH_EDGE_STRUCT T_GRAPH_EDGE_STRUCT, *T_GRAPH_EDGE;
typedef struct T_CHAR_FORMAT_INFO_STRUCT T_CHAR_FORMAT_INFO_STRUCT,  *T_CHAR_FORMAT_INFO;

/* see callback.h for T_GRAPH_CHANGE_CB_* defines and types */

/* --------------------------------------- */
/* Forward declaration of lexer structure. */
/* --------------------------------------- */
typedef struct T_LEX_STRUCT T_LEX_STRUCT, *T_LEX;

/* ----------------------- */
/* Command feedback levels */
/* ----------------------- */
typedef enum {
  T_FEEDBACK_INTERACTIVE,
  T_FEEDBACK_NEVER,
  T_FEEDBACK_ALWAYS
} T_FEEDBACK_LEVEL;

/* ---------------- */
/* Generic options. */
/* ---------------- */
typedef enum {
  T_OPT_TYPE_BOOLEAN,
  T_OPT_TYPE_ENUM,
  T_OPT_TYPE_NUMERIC,
  T_OPT_TYPE_STRING,
  T_OPT_TYPE_LIST_ENUM,
  T_OPT_TYPE_LIST_NUMERIC,
  T_OPT_TYPE_LIST_STRING,
  T_OPT_TYPE_LIST_ENUM2 /* T_OPT_TYPE_LIST_ENUM, but allow '!' prefix */
} T_OPT_TYPE;

#define T_OPT_TYPE_FIRST T_OPT_TYPE_BOOLEAN
#define T_OPT_TYPE_LAST T_OPT_TYPE_LIST_ENUM2

typedef struct T_OPTION_STRUCT T_OPTION_STRUCT, *T_OPTION;
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_OPTION_TRAV_FUNC)
  T_TYPEDEF((T_STR /*name*/,
       T_OPTION /*option*/,
       T_PTR /*arg*/));
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_OPTION_LEGVAL_TRAV_FUNC)
  T_TYPEDEF((T_STR /*name*/,
       T_STR /*name2*/,
       T_PTR /*arg*/));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_OPTION_VERIFY_FUNC)
  T_TYPEDEF((T_OPTION /*opt*/,
       T_PTR /*new_value*/,
       T_PTR /*arg*/));

typedef struct T_COMMAND_STRUCT T_COMMAND_STRUCT, *T_COMMAND;
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_COMMAND_PARSE_FUNC)
  T_TYPEDEF((T_LEX /*lex*/));

#define T_NO_MORE_ARGS(lexer) \
  (T_GLEX_TOKEN(lexer) == T_TOK_EOF || T_GLEX_TOKEN(lexer) == T_TOK_SEMICOLON)

/* ------------------------------------- */
/* Generic printf-style output function. */
/* ------------------------------------- */

/*
 * Changed to T_ENTRY_VA1 * T_ENTRY_VA2 since we moved to
 * __stdcall on WIN32. T_ENTRY_VA1 and T_ENTRY_VA2 are defined
 * as __cdecl. varargs function pointers and prototypes need to
 * be __cdecl on WIN32. BM 06/21/98
 */

typedef void (T_ENTRY_VA1 * T_ENTRY_VA2 T_EXPORT T_OUT_FUNC)
     T_TYPEDEF((
               T_STR /*format_str*/,
               ...
               ));
typedef void (T_ENTRY_VA1 * T_ENTRY_VA2 T_EXPORT T_OUT_VA_FUNC)
     T_TYPEDEF((
                T_STR /*format_str*/,
    va_list /*var_arg_ptr*/
               ));

typedef void (T_ENTRY_VA1 * T_ENTRY_VA2 T_EXPORT T_OUT_VA_FLUSH_FUNC)
     T_TYPEDEF((
                T_STR /*format_str*/,
    va_list /*var_arg_ptr*/
               ));
/* -------------------------------------- */
/* Forward declaration of lobe structure. */
/* -------------------------------------- */
typedef struct T_LOBE_STRUCT T_LOBE_STRUCT, *T_LOBE;

#define T_LOBE_SET(p, l, v) (TutLobeSet((T_STR)(p), (l), (T_STR)(v)))
#define T_LOBE_GET(p, l, v) (TutLobeGet((T_STR)(p), (l), (T_STR)(v)))
#define T_LOBENUM_SET(p, n, v) (T_LOBE_SET((p), TutLobeLookup(n), (v)))
#define T_LOBENUM_GET(p, n, v) (T_LOBE_GET((p), TutLobeLookup(n), (v)))
#define T_LOBENUM_EXISTS(p, n) (TutLobeFindValue((T_STR)(p), TutLobeLookup(n)) != NULL)
/* ---------------------------------------------------------- */
/* Define starting lobe numbers for each process type.        */
/* This is needed to maintain separation between lobe values. */
/* ---------------------------------------------------------- */
#define T_LOBE_UTIL_START    0         /* 0       -> -199   */
#define T_LOBE_IPC_START     (-200)    /* -200    -> -399   */
#define T_LOBE_IE_START      (-400)    /* -400    -> -599   */
#define T_LOBE_HCI_START     (-600)    /* -600    -> -799   */
#define T_LOBE_DAQ_START     (-800)    /* -800    -> -999   */
#define T_LOBE_ARCH_START    (-1000)   /* -1000   -> -1199  */
#define T_LOBE_PLAY_START    (-1200)   /* -1200   -> -1399  */
#define T_LOBE_SRV_START     (-1400)   /* -1400   -> -1599  */
#define T_LOBE_MQS_START     (-1600)   /* -1600   -> -1799  */
#define T_LOBE_SUT_START     (-1800)   /* -1800   -> -1999  */
#define T_LOBE_SPC_START     (-2000)   /* -2000   -> -2199  */
#define T_LOBE_SVC_START     (-2200)   /* -2200   -> -2399  */
#define T_LOBE_SPSS_START    (-2400)   /* -2400   -> -2599  */
#define T_LOBE_GW_START      (-2600)   /* -2600   -> -2799  */
#define T_LOBE_GW_MSL_START  (-2800)   /* -2800   -> -7799  */
#define T_LOBE_GW_CI_START   (-7800)   /* -7800   -> -12799 */
#define T_LOBE_SMR_START     (-13000)  /* -13000  -> -13199 */
#define T_LOBE_SMR_SVC_START (-13200)  /* -13200  -> -13299 */
#define T_LOBE_GM_START      (-13300)  /* -13300  -> -13499 */

/* ------------------------------------------------ */
/* Forward declaration of time converter structure. */
/* ------------------------------------------------ */
typedef struct T_TIME_CVT_STRUCT T_TIME_CVT_STRUCT, *T_TIME_CVT;
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_TIME_CVT_NUM_TO_STR_FUNC)
  T_TYPEDEF((T_REAL8 /*time_num*/,
       T_STR * /*time_str_return*/));
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_TIME_CVT_STR_TO_NUM_FUNC)
  T_TYPEDEF((T_STR /*time_str*/,
       T_REAL8 * /*time_num_return*/));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_TIME_CVT_DESTROY_FUNC)
  T_TYPEDEF((void));
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_TIME_CVT_TRAV_FUNC)
  T_TYPEDEF((T_STR/*name*/,
       T_TIME_CVT /*time_cvt*/,
       T_PTR /*arg*/));
typedef T_WCHAR* (T_ENTRY1 * T_ENTRY2 T_EXPORT T_FORMAT_UT_TO_UNI_FUNC)
  T_TYPEDEF((
             T_STR /*str*/,
       T_CHAR_FORMAT_INFO_STRUCT * /*format pointer*/));
typedef T_STR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_FORMAT_UT_FROM_UNI_FUNC)
  T_TYPEDEF((
             T_WCHAR* /*wchar*/,
       T_CHAR_FORMAT_INFO_STRUCT * /*format pointer*/));

/* ---------------------------- */
/* Declaration of a search path */
/* ---------------------------- */
typedef struct T_SEARCH_PATH T_SEARCH_PATH;
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SEARCH_PATH_DIR_TRAV_FUNC)
  T_TYPEDEF((T_STR /*path_name*/,
       T_PTR /*arg*/));

/* -------------------------- */
/* Generic traversal function */
/* -------------------------- */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_EXPORT T_TRAV_FUNC) T_TYPEDEF((void));

/* --------------- */
/* Integer format. */
/* --------------- */
typedef enum {
  T_INT_FORMAT_INVALID = -1,
  T_INT_LITTLE_ENDIAN,
  T_INT_BIG_ENDIAN
} T_INT_FORMAT;

/* ------------------------------------ */
/* Floating-point (real number) format. */
/* ------------------------------------ */
typedef enum {
  T_REAL_FORMAT_INVALID = -1,
  T_REAL_DEC_D, /* DEC D format (VAX default) */
  T_REAL_DEC_G, /* DEC G format (Alpha AXP default) */
  T_REAL_IEEE, /* IEEE format */
  T_REAL_IBM_370 /* IBM 370 format */
} T_REAL_FORMAT;

/*
**=================[ Internationalization Support ]=================
**
** Added in multi-byte character sets 10/9/96 RDH
**
*/

/* ---------------------------------------------------------------------- */
/* T_CHAR_FORMAT_* -- the character set (format) macros, drivers and such */
/* ---------------------------------------------------------------------- */

/*
 * T_CHAR_FORMAT -- A character set identifier
 * For our needs, an INT2 is suitable to hold all possible IBM code page
 * IDs plus other character sets.
 */
typedef T_INT2 T_CHAR_FORMAT;

/* -------------------------------------------------------------- */
/* T_CHAR_FORMAT_DRIVER -- Character set mapping driver interface */
/* -------------------------------------------------------------- */
typedef struct T_CHAR_FORMAT_DRIVER_STRUCT {
  T_CHAR_FORMAT src_format; /* input format */
  T_CHAR_FORMAT dest_format; /* output format */
  T_UCHAR map_8bit_tbl[256];  /* 8-bit-to-8-bit mapping table */
} T_CHAR_FORMAT_DRIVER_STRUCT, *T_CHAR_FORMAT_DRIVER;

/* ========================================================================= */
/*..T_CHAR_EL_STRUCT -- Character element sturcture                          */
typedef struct T_CHAR_EL_STRUCT {
  T_INT2  NumBytes;
  T_UINT2 SrcCp;
  T_WCHAR Uni;
} T_CHAR_EL_STRUCT, *T_CHAR_EL;

#define T_ERR_CHAR_EL_NOT_FOUND      -1

/* ========================================================================= */
/*..T_CHAR_FORMAT_INFO -- Character set mapping driver interface             */
/*  for transcoding that uses Unicode as an intermediate form.               */
struct T_CHAR_FORMAT_INFO_STRUCT {
  T_STR name;               /* Character Set Name                     */
  T_CHAR_FORMAT number;     /* Character Set ordinal                  */
  T_STR CsFname;            /* Character Set File Name                */
  T_INT2 CsMode;            /* Character Set Mode type                     */
  T_UINT2 Sub;              /* Substitution character ASCII(26) EBCDIC(63) */
  T_INT4 MinBytes;          /* Minimum number of bytes per character  */
  T_INT4 MaxBytes;          /* Maximum number of bytes per character  */
  T_INT4 TotNumChar;        /* Total number of characters in set      */

  T_CHAR_EL *ToUniTbl;      /* This set to Unicode Xcode table               */
  T_FORMAT_UT_TO_UNI_FUNC utToUni;
                            /* Unicode INcode function                       */
  T_CHAR_EL *FromUniTbl;    /* Unicode to this set table                     */
  T_FORMAT_UT_FROM_UNI_FUNC utFromUni;
                            /* Unicode DEcode function                       */
};

/* ========================================================================= */
/*..Character Set Mode Types                                                 */
#define T_CS_MODE_INVALID         -1
#define T_CS_MODE_FIXED           1
#define T_CS_MODE_MB_MODAL        2
#define T_CS_MODE_MB_NON_MODAL    3

/* -------------------------------------------------------------- */

#define T_CHAR_FORMAT_INVALID (-1)  /* Undefined character set  */

  /*
  ** DO NOT ever use zero (0) for a T_CHAR_FORMAT_xxx value. It will clash with
  ** the hash table lookup return value of zero (0) when the lookup can't find
  ** a valid entry.
  */

  /* The following three entries are for testing */
#ifdef TEST_MSG_GROWTH
# define T_CHAR_FORMAT_TEST1     (1)
# define T_CHAR_FORMAT_TEST2     (2)
# define T_CHAR_FORMAT_TEST3     (3)
#endif
#define T_CHAR_FORMAT_CP_TEST   (1) /* ASCII test (swap case)       */

  /* EBCDIC (US, Canada, Netherlands, Portugal, Brazil, New Zealand, Australia) */
#define T_CHAR_FORMAT_CP00037   (37)
  /* EBCDIC (Austria, Germany) */
#define T_CHAR_FORMAT_CP00273   (273)
  /* EBCDIC (Denmark, Norway) */
#define T_CHAR_FORMAT_CP00277   (277)
  /* EBCDIC (Finland, Sweden) */
#define T_CHAR_FORMAT_CP00278   (278)
  /* EBCDIC (Italy) */
#define T_CHAR_FORMAT_CP00280   (280)
  /* EBCDIC (Spain, Latin America) */
#define T_CHAR_FORMAT_CP00284   (284)
  /* EBCDIC (United Kingdom) */
#define T_CHAR_FORMAT_CP00285   (285)
  /* EBCDIC (France) */
#define T_CHAR_FORMAT_CP00297   (297)
  /* EBCDIC (Arabic) */
#define T_CHAR_FORMAT_CP00420   (420)
  /* EBCDIC (Greek) */
#define T_CHAR_FORMAT_CP00423   (423)
  /* EBCDIC (Hebrew) */
#define T_CHAR_FORMAT_CP00424   (424)
  /* PC Data; PC Base (US) */
#define T_CHAR_FORMAT_CP00437   (437)
  /* EBCDIC (Belgium, Canada, Switzerland, International Latin-1) */
#define T_CHAR_FORMAT_CP00500   (500)
  /* PC Data (Greek) */
#define T_CHAR_FORMAT_CP00737   (737)
  /* PC Data (Baltic) */
#define T_CHAR_FORMAT_CP00775   (775)
  /* PC Data (MLP 222 Latin Alphabet 1) */
#define T_CHAR_FORMAT_CP00850   (850)
  /* PC Data (Greek) */
#define T_CHAR_FORMAT_CP00851   (851)
  /* PC Data (Latin-2) */
#define T_CHAR_FORMAT_CP00852   (852)
  /* PC Data (ROECE Cyrillic) */
#define T_CHAR_FORMAT_CP00855   (855)
  /* PC Data (Latin-5 Turkey) */
#define T_CHAR_FORMAT_CP00857   (857)
  /* PC Data (Portugal) */
#define T_CHAR_FORMAT_CP00860   (860)
  /* PC Data (Iceland) */
#define T_CHAR_FORMAT_CP00861   (861)
  /* PC Data (Hebrew) */
#define T_CHAR_FORMAT_CP00862   (862)
  /* PC Data (Canada) */
#define T_CHAR_FORMAT_CP00863   (863)
  /* PC Data (Arabic) */
#define T_CHAR_FORMAT_CP00864   (864)
  /* PC Data (Denmark, Norway) */
#define T_CHAR_FORMAT_CP00865   (865)
  /* PC Data (Cyrillic #2) */
#define T_CHAR_FORMAT_CP00866   (866)
  /* PC Data (Greek) */
#define T_CHAR_FORMAT_CP00869   (869)
  /* EBCDIC (Latin-2 Multilingual) */
#define T_CHAR_FORMAT_CP00870   (870)
  /* EBCDIC (Iceland */
#define T_CHAR_FORMAT_CP00871   (871)
  /* PC Data (Thai) */
#define T_CHAR_FORMAT_CP00874   (874)
  /* EBCDIC (Greece) */
#define T_CHAR_FORMAT_CP00875   (875)
  /* EBCDIC (Cyrillic Multilingual) */
#define T_CHAR_FORMAT_CP00880   (880)
  /* PC Data (Korean; non-extended) */
#define T_CHAR_FORMAT_CP00891   (891)
  /* PC Data (Simplified Chinese) */
#define T_CHAR_FORMAT_CP00903   (903)
  /* PC Data (Traditional Chinese) */
#define T_CHAR_FORMAT_CP00904   (904)
  /* EBCDIC (Latin-3 Turkey) */
#define T_CHAR_FORMAT_CP00905   (905)
  /* Multibyte PC Data (Japanese mixed) */
#define T_CHAR_FORMAT_CP00932   (932)
  /* Shifted EBCDIC (Korean; extended range; 1880 UDC) */
#if 0
#define T_CHAR_FORMAT_CP00933   (933)
  /* Multibyte PC Data (Korean) */
#define T_CHAR_FORMAT_CP00934   (934)
  /* Shifted EBCDIC (Simplified Chinese; extended range) */
#define T_CHAR_FORMAT_CP00935   (935)
  /* Multibyte PC Data (Korean National Standard Graphic Character Set; */
  /* 1800 UDC) */
#endif
#define T_CHAR_FORMAT_CP00949   (949)
  /* Multibyte PC Data (Big 5 Traditional Chinese mixed) */
#define T_CHAR_FORMAT_CP00950   (950)
  /* ISO646 IRV */
#define T_CHAR_FORMAT_CP01009   (1009)
  /* EBCDIC (Turkey Latin 5 CECP) */
#define T_CHAR_FORMAT_CP01026   (1026)
  /* PC Data (Traditional Chinese, Taiwan Industry Graphic Character Set (Big 5)) */
#if 0
#define T_CHAR_FORMAT_CP01114   (1114)
  /* PC Data (Simplified Chinese, People's Republic of China National Standard) */
#define T_CHAR_FORMAT_CP01115   (1115)
  /* EBCDIC (Estonia) */
#define T_CHAR_FORMAT_CP01122   (1122)
  /* ISO8859-2 Modified (Latin-2; C1=VDC) */
#endif
#define T_CHAR_FORMAT_CP01250   (1250)
  /* ISO8859-5 Modified (Cyrillic; C1=VDC) */
#define T_CHAR_FORMAT_CP01251   (1251)
  /* ISO8859-1 Modified (Latin-1; C1=VDC) */
#define T_CHAR_FORMAT_CP01252   (1252)
  /* ISO8859-7 Modified (Greek; C1=VDC) */
#define T_CHAR_FORMAT_CP01253   (1253)
  /* ISO8859-9 Modified (Latin-9; Turkish C1=VDC) */
#define T_CHAR_FORMAT_CP01254   (1254)
  /* ISO8859-8 Modified (Hebrew; C1=VDC) */
#define T_CHAR_FORMAT_CP01255   (1255)
  /* ISO8859-6 Modified (Arabic; C1=VDC) */
#define T_CHAR_FORMAT_CP01256   (1256)
  /* ISO8859-4 Modified (Latin-4 Baltic; C1=VDC) */
#define T_CHAR_FORMAT_CP01257   (1257)
  /* Double Byte PC Data (Simplified Chinese, People's Republic of China */
  /* National Standard) */
#if 0
#define T_CHAR_FORMAT_CP01380   (1380)
  /* Multibyte PC Data (Simplified Chinese, People's Republic of China National Standard) */
#define T_CHAR_FORMAT_CP01381   (1381)
  /* BIG5 Traditional Chinese */
#endif
#define T_CHAR_FORMAT_ISO10646  (25000)
  /* BIG5 Traditional Chinese */
#define T_CHAR_FORMAT_BIG5      (25001)
  /* C, same as POSIX Portable */
#define T_CHAR_FORMAT_C         (25002)
  /* ISO646:IRV 1983 */
#define T_CHAR_FORMAT_ISO646IRV (25003)
  /* ISO8859-1 Latin-1 (Western Europe) */
#define T_CHAR_FORMAT_ISO8859_1 (25004)
  /* ISO8859-2 Latin-2 (Eastern Europe) */
#define T_CHAR_FORMAT_ISO8859_2 (25005)
  /* ISO8859-3 Latin-3 (Maltese, Catalan, Galician, Esperanto) */
#define T_CHAR_FORMAT_ISO8859_3 (25006)
  /* ISO8859-4 Latin-4 (Baltic and Nordic) */
#define T_CHAR_FORMAT_ISO8859_4 (25007)
  /* ISO8859-5 Cyrillic */
#define T_CHAR_FORMAT_ISO8859_5 (25008)
  /* ISO8859-6 Arabic */
#define T_CHAR_FORMAT_ISO8859_6 (25009)
  /* ISO8859-7 Greek */
#define T_CHAR_FORMAT_ISO8859_7 (25010)
  /* ISO8859-8 Hebrew */
#define T_CHAR_FORMAT_ISO8859_8 (25011)
  /* ISO8859-9 Latin-5 (Latin-1 less Icelandic plus Turkish) */
#define T_CHAR_FORMAT_ISO8859_9 (25012)
  /* POSIX Portable Code Set */
#define T_CHAR_FORMAT_POSIX     (25013)
  /* Shift JIS (Japanese) */
#define T_CHAR_FORMAT_SJIS      (25014)
  /* JIS X 0201-1976 */
#define T_CHAR_FORMAT_JIS0201   (25015)
  /* JIS X 0208-1990 */
#define T_CHAR_FORMAT_JIS0208   (25016)
  /* JIS X 0212-1990 */
#define T_CHAR_FORMAT_JIS0212   (25017)
  /* GB 1988-80 */
#define T_CHAR_FORMAT_GB1988    (25018)
  /* GB 2312-80 */
#define T_CHAR_FORMAT_GB2312    (25019)
  /* KSC 5601-1992 */
#define T_CHAR_FORMAT_KSC5601   (25020)
  /* KSC 5636 (ISO646:KR) */
#define T_CHAR_FORMAT_KSC5636   (25021)

/* -------------------------------------------------------------------- */
/* T_CHAR_FORMAT_{ASCII,EBCDIC} -- Short-hand for the system default  */
/* character set on ASCII and EBCDIC systems        */
/* -------------------------------------------------------------------- */
#define T_CHAR_FORMAT_ASCII   T_CHAR_FORMAT_CP00437
#define T_CHAR_FORMAT_EBCDIC  T_CHAR_FORMAT_CP00037

/* -------------------------------------------------------------------- */
/* T_ERR_CF_{...} -- Character formatting error values                  */
/* -------------------------------------------------------------------- */
/* T_ERR_CF_START defined in <rtworks/config.h> as 5000                 */

#define T_ERR_CF_INVALID_DRIVER           T_ERR_CF_START + 1  /* 5001 */
#define T_ERR_CF_INVALID_SRC_FORMAT       T_ERR_CF_START + 2  /* 5002 */
#define T_ERR_CF_INVALID_DEST_FORMAT      T_ERR_CF_START + 3  /* 5003 */
#define T_ERR_CF_SRC_TBL_LOAD_FAILED      T_ERR_CF_START + 4  /* 5004 */
#define T_ERR_CF_DEST_TBL_LOAD_FAILED     T_ERR_CF_START + 5  /* 5005 */
#define T_ERR_CF_CONVERT_TO_UNI_FAILED    T_ERR_CF_START + 6  /* 5006 */
#define T_ERR_CF_CONVERT_FROM_UNI_FAILED  T_ERR_CF_START + 7  /* 5007 */

/*
**==================================================================
*/

/* ------------------------ */
/* Generic I/O check modes. */
/* ------------------------ */
typedef enum {
  T_IO_CHECK_READ,
  T_IO_CHECK_WRITE
} T_IO_CHECK_MODE;
#ifdef T_RTWORKS35_COMPAT /* compat defines */
# define T_SOCKET_CHECK_READ T_IO_CHECK_READ
# define T_SOCKET_CHECK_WRITE T_IO_CHECK_WRITE
# define T_SOCKET_CHECK_MODE T_IO_CHECK_MODE
#endif

/* ------------------------ */
/* SelectX source types.    */
/* ------------------------ */
typedef enum {
  T_IO_SOURCE_CONN,
  T_IO_SOURCE_FD,
  T_IO_SOURCE_ECB
} T_IO_SOURCE;

/* ----------------------------------------------------------- */
/* T_TIMEOUT_FOREVER is used to specify an indefinite timeout. */
/* ----------------------------------------------------------- */
#define T_TIMEOUT_FOREVER ((T_REAL8)(-1.0))
#define T_TIMEOUT_DRAIN ((T_REAL8)(-2.0)) /* undocumented */
/*
 * Special timeout values used from the thread function in the protocol driver.
 * These values tell the called function that it was called from the protocol
 * driver thread function.  dlc  9-12-96
 */
#define T_TIMEOUT_THREAD_CHECK ((T_REAL8)(-10.0))

/* --------------------------------- */
/* Macros for byte-swapping numbers. */
/* --------------------------------- */

/* -------------------------------------------------- */
/* T_SWAP_TWO_BYTES swaps a two-byte number in place. */
/* -------------------------------------------------- */
#define T_SWAP_TWO_BYTES(ptr) \
{ \
  T_CHAR tmp_val; \
  T_ASSERT(ptr != NULL); \
  tmp_val = ((T_STR)(ptr))[0]; \
  ((T_STR)(ptr))[0] = ((T_STR)(ptr))[1]; \
  ((T_STR)(ptr))[1] = tmp_val; \
} /* T_SWAP_TWO_BYTES */

/* ---------------------------------------------------- */
/* T_SWAP_FOUR_BYTES swaps a four-byte number in place. */
/* ---------------------------------------------------- */
#define T_SWAP_FOUR_BYTES(ptr) \
{ \
  T_CHAR tmp_val; \
  T_ASSERT(ptr != NULL); \
  /* Swap the first and fourth bytes. */ \
  tmp_val = ((T_STR)(ptr))[0]; \
  ((T_STR)(ptr))[0] = ((T_STR)(ptr))[3]; \
  ((T_STR)(ptr))[3] = tmp_val; \
  /* Swap the second and third bytes. */ \
  tmp_val = ((T_STR)(ptr))[1]; \
  ((T_STR)(ptr))[1] = ((T_STR)(ptr))[2]; \
  ((T_STR)(ptr))[2] = tmp_val; \
} /* T_SWAP_FOUR_BYTES */

/* ------------------------------------------------------ */
/* T_SWAP_EIGHT_BYTES swaps a eight-byte number in place. */
/* ------------------------------------------------------ */
#define T_SWAP_EIGHT_BYTES(ptr) \
{ \
  T_CHAR tmp_val; \
  T_ASSERT(ptr != NULL); \
  /* Swap the first and eighth bytes. */ \
  tmp_val = ((T_STR)(ptr))[0]; \
  ((T_STR)(ptr))[0] = ((T_STR)(ptr))[7]; \
  ((T_STR)(ptr))[7] = tmp_val; \
  /* Swap the second and seventh bytes. */ \
  tmp_val = ((T_STR)(ptr))[1]; \
  ((T_STR)(ptr))[1] = ((T_STR)(ptr))[6]; \
  ((T_STR)(ptr))[6] = tmp_val; \
  /* Swap the third and sixth bytes. */ \
  tmp_val = ((T_STR)(ptr))[2]; \
  ((T_STR)(ptr))[2] = ((T_STR)(ptr))[5]; \
  ((T_STR)(ptr))[5] = tmp_val; \
  /* Swap the fourth and fifth bytes. */ \
  tmp_val = ((T_STR)(ptr))[3]; \
  ((T_STR)(ptr))[3] = ((T_STR)(ptr))[4]; \
  ((T_STR)(ptr))[4] = tmp_val; \
} /* T_SWAP_EIGHT_BYTES */

/*
 * T_SWAP_SIXTEEN_BYTES swaps a sixteen-byte number in place.
 */
#define T_SWAP_SIXTEEN_BYTES(ptr) \
{ \
  T_CHAR tmp_val; \
  T_ASSERT(ptr != NULL); \
  /* Swap the first and sixteenh bytes. */ \
  tmp_val = ((T_STR)(ptr))[0]; \
  ((T_STR)(ptr))[0] = ((T_STR)(ptr))[15]; \
  ((T_STR)(ptr))[15] = tmp_val; \
  /* Swap the second and fifteenth bytes. */ \
  tmp_val = ((T_STR)(ptr))[1]; \
  ((T_STR)(ptr))[1] = ((T_STR)(ptr))[14]; \
  ((T_STR)(ptr))[14] = tmp_val; \
  /* Swap the third and fourteenth bytes. */ \
  tmp_val = ((T_STR)(ptr))[2]; \
  ((T_STR)(ptr))[2] = ((T_STR)(ptr))[13]; \
  ((T_STR)(ptr))[13] = tmp_val; \
  /* Swap the fourth and thirteenth bytes. */ \
  tmp_val = ((T_STR)(ptr))[3]; \
  ((T_STR)(ptr))[3] = ((T_STR)(ptr))[12]; \
  ((T_STR)(ptr))[12] = tmp_val; \
  /* Swap the fifth and thirteenth bytes. */ \
  tmp_val = ((T_STR)(ptr))[4]; \
  ((T_STR)(ptr))[4] = ((T_STR)(ptr))[11]; \
  ((T_STR)(ptr))[11] = tmp_val; \
  /* Swap the sixth and thirteenth bytes. */ \
  tmp_val = ((T_STR)(ptr))[5]; \
  ((T_STR)(ptr))[5] = ((T_STR)(ptr))[10]; \
  ((T_STR)(ptr))[10] = tmp_val; \
  /* Swap the seventh and thirteenth bytes. */ \
  tmp_val = ((T_STR)(ptr))[6]; \
  ((T_STR)(ptr))[6] = ((T_STR)(ptr))[9]; \
  ((T_STR)(ptr))[9] = tmp_val; \
  /* Swap the eighth and ninth bytes. */ \
  tmp_val = ((T_STR)(ptr))[7]; \
  ((T_STR)(ptr))[7] = ((T_STR)(ptr))[8]; \
  ((T_STR)(ptr))[8] = tmp_val; \
} /* T_SWAP_SIXTEEN_BYTES */

/* ------------------------------------------------------------------- */
/* It turns out that there is not enough precision in double precision */
/* math do keep the accuracy required.  This time structure is used    */
/* to maintain the necessary accuracy.                                 */
/* ------------------------------------------------------------------- */
typedef struct T_TIME_STRUCT {
  T_INT4 sec;
  T_INT4 usec;
} T_TIME_STRUCT, *T_TIME;

#define T_MILLION_FLOAT 1e6
#define T_MILLION_INT   1000000
#define T_USEC_SPEC     "%06ld"
#define T_TIME_SPEC     T_INT4_SPEC "." T_USEC_SPEC

/* cast needed for /W3 on MSVC++ 4.1 */
#define T_TIME_SPLIT(tp, t)  (tp).sec = (T_INT4)floor(t); \
        (tp).usec = (T_INT4)(((((t)-(tp).sec)*T_MILLION_FLOAT*10) + 1)/10)
#define T_TIME_SET(t, s, u)  (t).sec = (T_INT4)(s); (t).usec = (u)
#define T_TIME_INCR(t, s, u) \
  (t).usec += u; \
  (t).sec = (t).sec + s + (t).usec/T_MILLION_INT; \
  (t).usec = (t).usec % T_MILLION_INT
#define T_TIME_INCRT(t, ti) T_TIME_INCR(t, (ti).sec, (ti).usec)
#define T_TIME_ADD(tr, t1, t2) tr = t1; T_TIME_INCRT(tr, t2)
#define T_TIME_EQC(t, s, u) ((t).sec == (s) && (t).usec == (u))
#define T_TIME_EQ(t1, t2) T_TIME_EQC((t1), (t2).sec, (t2).usec)
#define T_TIME_GTC(t1, s, u) \
  ((t1).sec > (s) ||((t1).sec == (s) && (t1).usec > (u)))
#define T_TIME_GT(t1, t2) T_TIME_GTC((t1), (t2).sec, (t2).usec)
#define T_TIME_GE(t1, t2) (T_TIME_GT(t1, t2) || T_TIME_EQ(t1, t2))
#define T_TIME_GEC(t1, s, u) (T_TIME_GTC(t1, s, u) || T_TIME_EQC(t1, s, u))
#define T_TIME_LTC(t1, s, u) \
  ((t1).sec < (s) || ((t1).sec == (s) && (t1).usec < (u)))
#define T_TIME_LT(t1, t2) T_TIME_LTC((t1), (t2).sec, (t2).usec)
#define T_TIME_LEC(t1, s, u) (T_TIME_LTC(t1, s, u) || T_TIME_EQC(t1, s, u))
#define T_TIME_LE(t1, t2) (T_TIME_LEC(t1, (t2).sec, (t2).usec))
#define T_TIME_DOUBLE(t)  ((t).sec + (t).usec / T_MILLION_FLOAT)
/* ------------------------------------------------ */
/* This macro implements the equation  tr = t1 - t2 */
/* WARNING: It assumes that t1 is greater than t2.  */
/* ------------------------------------------------ */
#define T_TIME_SUB(tr, t1, t2) \
  (tr).usec = (t1).usec - (t2).usec; \
  (tr).sec = ((tr).usec < 0) ? ((t1).sec-(t2).sec - 1) : ((t1).sec-(t2).sec); \
  (tr).usec = ((tr).usec < 0) ? ((tr).usec + T_MILLION_INT) : ((tr).usec)
#define T_TIME_MS(t)  ((t).sec * 1000 + (t).usec / 1000)

/* ------------------------------------------------------------------- */
/* T_FORK_FOR_EXEC forks a child process as best as possible to exec() */
/* a new command.  SMW 10/31/94                                        */
/* ------------------------------------------------------------------- */
#if defined(T_OS_IRIX) || defined(T_OS_AIX) || defined(T_OS_OSS)
# define T_FORK_FOR_EXEC fork
#else
# define T_FORK_FOR_EXEC vfork
#endif

/* ---------------------------------------------------------------- */
/* Quarks (used to convert strings to/from integers).  Quarks are   */
/* sorta like "string objects".  Usually you can just do string     */
/* comparisons directly, but sometimes you can't (like for callback */
/* arguments).                                                      */
/* ---------------------------------------------------------------- */
typedef T_INT4 T_QUARK; /* Zero is NOT a valid quark. */

/* ---------------------------------------------------------------------- */
/* We need some extra params on VMS to open a file so that it can be open */
/* for writing and yet be read also (like on UNIX).  The default sharing  */
/* changed in the OpenVMS AXP 1.5 C RTL according to Ken Cowan            */
/* (cowan@rtl.enet.dec.com).  SMW 09/11/94                                */
/* Using "shr=put" everywhere seems to work pretty well.  The DEC C 4.0   */
/* manual doesn't say you can chain together shr values (e.g.             */
/* "shr=get,put"), but it does seem to work.  Using put sharing           */
/* on all opens seems to allow get sharing.  Perhaps the sharing          */
/* levels are nested.  We need put sharing when open the file for         */
/* writing/appending so that multiple processes can write/append.         */
/* SMW 10/04/94                                                           */
/* Hmmm...we seem to use "shr=put" for all args, so perhaps we can        */
/* simplify the T_FOPEN_*_ARGS defines.  I am also starting to use        */
/* the fopen "update" modes ({r,w,a}+, and I want file sharing there      */
/* too, so I have added T_FOPEN_GENERIC_ARGS.  SMW 01/14/95               */
/* ---------------------------------------------------------------------- */

#define T_FOPEN_WRITE_TEXT_FLAG "w"
#define T_FOPEN_APPEND_TEXT_FLAG "a"

#ifdef T_OS_WINDOWS
/* ----------------------------------------------------------- */
/* NT can't seek in a file in text mode so open them in binary */
/* ----------------------------------------------------------- */
# define T_FOPEN_READ_FLAG "r+b"
# define T_FOPEN_READ_ONLY_FLAG "rb"
# define T_FOPEN_WRITE_FLAG "w+b"
# define T_FOPEN_APPEND_FLAG "a+b"
#else
# define T_FOPEN_READ_FLAG "r"
# define T_FOPEN_READ_ONLY_FLAG "r"
# define T_FOPEN_WRITE_FLAG "w"
# define T_FOPEN_APPEND_FLAG "a"
#endif

#ifdef T_OS_VMS
# define T_FOPEN_READ_ARGS , "shr=put"
/* Let VMS/NFS pick the file format, not us.  SMW 09/11/94 */
# define T_FOPEN_WRITE_ARGS , /*"rfm=var", "rat=cr",*/ "shr=put"
# define T_FOPEN_APPEND_ARGS , "shr=put"
# define T_FOPEN_GENERIC_ARGS , "shr=put" /* for any other fopen */
#else
# define T_FOPEN_READ_ARGS /* nothing */
# define T_FOPEN_WRITE_ARGS /* nothing */
# define T_FOPEN_APPEND_ARGS /* nothing */
# define T_FOPEN_GENERIC_ARGS /* nothing */
#endif

/* -------------------------------------------------------- */
/* T_GET_HEAP_BOUNDARY is used to get memory heap boundary. */
/* -------------------------------------------------------- */
#define T_GET_HEAP_BOUNDARY() (TutGetCurrentSbrk())

/* -------------------------------------------------------------- */
/* T_LICENSE is used to identify the counted licenses (as opposed */
/* to the "all-or-none" interface licenses).                      */
/*                                                                */
/* Note: see comment in lm/lm_license.c for instructions on how   */
/* to add new counted license type to RTlm.  SMW 04/28/95         */
/* -------------------------------------------------------------- */
typedef enum {
  T_LICENSE_DEVELOPMENT_RTDAQ,
  T_LICENSE_RUNTIME_RTDAQ,
  T_LICENSE_DEVELOPMENT_RTIE,
  T_LICENSE_RUNTIME_RTIE,
  T_LICENSE_RTDRAW,
  T_LICENSE_RTHCI,
  T_LICENSE_REMOTE_RTHCI_DISPLAY,
  T_LICENSE_RTARCHIVE,
  T_LICENSE_RTPLAYBACK,
  T_LICENSE_RTSERVER,
  T_LICENSE_RTCLIENT, /* clients of RTserver */
  T_LICENSE_DEVELOPMENT_RTMON,
  T_LICENSE_RUNTIME_RTMON
#if defined(T_PT_MODIFY_LM)
  , T_LICENSE_EXTRAS
# define T_LICENSE_NUM_COUNTED (T_LICENSE_EXTRAS + 1)
#else
# define T_LICENSE_NUM_COUNTED (T_LICENSE_RUNTIME_RTMON + 1)
#endif
} T_LICENSE;

/* ------------------------------------------------------------------- */
/* Directory types returned by TutVmsDirectoryType.  Used to determine */
/* which type of directory (UNIX or VMS) a particular string matches.  */
/* ------------------------------------------------------------------- */
typedef enum {
  T_DIRECTORY_UNKNOWN,
  T_DIRECTORY_VMS,
  T_DIRECTORY_UNIX,
  T_DIRECTORY_MVS
} T_DIRECTORY_TYPE;

#if defined(T_OS_UNIX)
# define T_RTHOME_DIRECTORY_TYPE T_DIRECTORY_UNIX
#else
# define T_RTHOME_DIRECTORY_TYPE T_DIRECTORY_UNKNOWN
#endif

/* ---------------------------------------------- */
/* global variables -- see util.c for definitions */
/* ---------------------------------------------- */
#ifdef T_OS_VMS
# pragma nostandard /* needed for global{d,r}ef */
#endif

#define TutFrameCount (*TutFrameCountGetPtr())
#define TutDebugFlags (TutGetDebugFlags())
#define TutErrNum (*TutErrNumGetPtr())
#define TutErrJumpBuffer (*TutErrGetJumpBufferPtr())

#ifdef T_OS_VMS
# pragma standard /* needed for global{d,r}ef */
#endif


/*
 * This stuff belongs in <rtworks/ipc.h>, but we need some of it for verifying
 * the Server_Subject_Prefix option.  SMW 05/22/96
 */

/*
 * Do NOT change these subject values to be character constants
 * instead of string constants, even if the delimiter is a
 * one-character string.
 */
#define T_IPC_SUBJECT_DELIMITER "/"
#define T_IPC_SUBJECT_WILDCARD_ONE_LEVEL "*"
#define T_IPC_SUBJECT_WILDCARD_MANY_LEVELS "..."

#define T_IPC_SUBJECT_HAS_WILDCARDS(subject) \
  (T_NULL != strstr((subject), T_IPC_SUBJECT_WILDCARD_ONE_LEVEL) \
   || T_NULL != strstr((subject), T_IPC_SUBJECT_WILDCARD_MANY_LEVELS))

#define T_IPC_SUBJECT_IS_ABSOLUTE(subject) \
  ((subject) == strstr((subject), T_IPC_SUBJECT_DELIMITER))

#define T_IPC_SUBJECT_IS_DESCENDANT(child_subject, parent_subject) \
  (!T_STREQI((child_subject), (parent_subject)) /* cannot be equal */ \
   && TutStriStartsWith((child_subject), (parent_subject)))

/*
 * Define reentrant functions interface.
 */
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) || defined(_POSIX_REENTRANT_FUNCTIONS)
# define T_THREAD_SAFE_FUNCTIONS

/* define T_C_GETPWUID for the threaded UNIX platforms */
/* AIX getpwuid_r dosn't understand NIS :-( */
# ifndef T_C_GETPWUID
#  if defined(T_OS_SOLARIS2)
#   define T_C_GETPWUID_BUFLEN 2048
#   define T_C_GETPWUID(uid, pwd, buffer, buflen) \
            getpwuid_r((uid), (pwd), (buffer), (buflen))
#  else
#   if defined(T_OS_HPUX_11_00) || defined(T_OS_HPUX_64BIT) || defined(T_OS_ALPHA_OSF_40) || defined(T_OS_IRIX_62) || defined(T_OS_LINUX)
#    define T_C_GETPWUID_BUFLEN 2048
#    define T_C_GETPWUID(uid, pwd, buffer, buflen, result) \
             getpwuid_r((uid), (pwd), (buffer), (buflen), (result))
#   endif
#  endif
# endif

/* define T_C_STRTOK for the threaded UNIX platforms */
# ifndef T_C_STRTOK
#  define T_C_STRTOK(str, seperator, lasts) strtok_r((str), (seperator), (lasts))
# endif

/* define T_C_CTIME for the threaded UNIX platforms */
# ifndef T_C_CTIME
#  define T_C_CTIME_BUFLEN 32
#  if defined(T_OS_AIX) || defined(T_OS_HPUX_11_00) || defined(T_OS_HPUX_64BIT) || defined(T_OS_ALPHA_OSF_40) || defined(T_OS_IRIX_62) || defined(T_OS_LINUX)
#   define T_C_CTIME(clock, buf)                ctime_r((clock), (buf))
#  else /* returned value is different on OSF 3.2 and Solaris */
#   define T_C_CTIME(clock, buf, buflen)        ctime_r((clock), (buf), (buflen))
#  endif
# endif

/* define T_C_GETHOSTBYNAME for the threaded UNIX platforms */
# ifndef T_C_GETHOSTBYNAME
#  define T_C_GETHOSTBYNAME_BUFLEN 2048
#  if defined(T_OS_AIX)
#   define T_C_GETHOSTBYNAME(name, host_ent, host_ent_data) \
            gethostbyname_r((name), (host_ent), (host_ent_data))
#  else /* for now this means, just solaris2 */
#   if defined(T_OS_ALPHA_OSF_40) || defined(T_OS_IRIX_62) || defined(T_OS_LINUX)
#    define T_C_GETHOSTBYNAME(name) \
             gethostbyname((name))
#   else
#    if !defined(T_OS_OSF) && !defined(T_OS_HPUX_11_00) && !defined(T_OS_HPUX_64BIT) && !defined(T_OS_IRIX_62)
/* OSF 3.2 gethostbyname_r is broken.  On HPUX 11.00, gethostbyname */
/* is thread safe. */
#     define T_C_GETHOSTBYNAME(name, result, buffer, buflen, h_errnop) \
              gethostbyname_r((name), (result), (buffer), (buflen), (h_errnop))
#    endif
#   endif
#  endif
# endif

/* define T_C_GETSERVBYNAME for the threaded UNIX platforms */
# ifndef T_C_GETSERVBYNAME
#  define T_C_GETSERVBYNAME_BUFLEN 2048
#  if defined(T_OS_AIX)
#   define T_C_GETSERVBYNAME(name, proto, serv, serv_data) \
            getservbyname_r((name), (proto), (serv), (serv_data))
#  else
#   if defined(T_OS_HPUX_11_00) || defined(T_OS_HPUX_64BIT) || defined(T_OS_ALPHA_OSF_40) || defined(T_OS_IRIX_62) || defined(T_OS_LINUX)
/* On HPUX 11.00, getservbyname is thread safe. */
#    define T_C_GETSERVBYNAME(name, proto)   getservbyname((name), (proto))
#   else
#    define T_C_GETSERVBYNAME(name, proto, result, buffer, buflen) \
             getservbyname_r((name), (proto), (result), (buffer), (buflen))
/* for now this means, just solaris2 */
/* On Redhat Linux 6.0, netdb.h calls out 6 params, but getXXbyYY_r.c
 * and getsrvbynm_r.c use 5 */
#   endif /* T_OS_HPUX_11_00 || defined(T_OS_HPUX_64BIT) || T_OS_ALPHA_OSF_40 || T_OS_IRIX_62 */
#  endif /* defined(T_OS_AIX) */
# endif /* T_C_GETSERVBYNAME */

/* define T_C_LOCALTIME for the threaded UNIX platforms */
/* returned value is different on OSF 3.2 and Solaris/AIX */
# ifndef T_C_LOCALTIME
#  define T_C_LOCALTIME(clock, res)         localtime_r((clock), (res))
# endif

#endif /* _POSIX_THREAD_SAFE_FUNCTIONS || _POSIX_REENTRANT_FUNCTIONS */

#ifndef T_C_MKDIR
# if defined (T_OS_UNIX)
#  define T_C_MKDIR(dirname) mkdir((dirname), (00777))
# else
#  if defined (T_OS_VMS)
#   define T_C_MKDIR(dirname) mkdir((dirname), (00777))
#  elif defined (T_OS_WINDOWS)
#   define T_C_MKDIR(dirname) mkdir((dirname))
#  endif
# endif
#endif

#ifndef T_C_RMDIR
# if defined (T_OS_UNIX) || defined (T_OS_VMS)
#  define T_C_RMDIR(dirname) rmdir((dirname))
# elif defined (T_OS_WIN32)
#  define T_C_RMDIR(dirname) _rmdir((dirname))
# endif
#endif

#ifndef T_C_CHDIR
# if defined (T_OS_UNIX) || defined (T_OS_VMS)
#  define T_C_CHDIR(dirname) chdir((dirname))
# elif defined (T_OS_WIN32)
#  define T_C_CHDIR(dirname) _chdir((dirname))
# endif
#endif

/* --------------------------------------------------------------------- */
/* As we port to more non-UNIX-like operating systems, we cannot rely on */
/* the "normal" C run-time library being the same.  We therefore define  */
/* our own macros for the C functions we need.  SMW 4/15/93              */
/*                                                                       */
/* This kinda clashes with our T_{M,C,RE}ALLOC macros, but it's too late */
/* to change those now.  SMW 4/23/93                                     */
/*                                                                       */
/* Removing VxWorks support as of revision #34 (it was broken anyway!)   */
/* MW 2006-04-28                                                         */
/* --------------------------------------------------------------------- */

#ifndef T_C_ACCEPT
# define T_C_ACCEPT(s, addr, len)              accept((s), (addr), (len))
#endif
#ifndef T_C_BIND
# define T_C_BIND(fd, name, len)               bind((fd), (name), (len))
#endif
#ifndef T_C_CLOSE
# define T_C_CLOSE(fd)                         close((fd))
#endif
#ifndef T_C_CLOSESOCKET
# define T_C_CLOSESOCKET(fd)                   close((fd))
#endif
#ifndef T_C_CONNECT
# define T_C_CONNECT(s, addr, len)             connect((s), (addr), (len))
#endif
#ifndef T_C_CTIME
# define T_C_CTIME(clock, buf, buflen)         ctime((clock))
#endif
#ifndef T_C_EXIT
# define T_C_EXIT(status)                      TutExit((status))
#endif
#ifndef T_C_FCLOSE
# define T_C_FCLOSE(file)                      fclose(file)
#endif
#ifndef T_C_FOPEN
# define T_C_FOPEN(filename, mode)             fopen(filename, mode)
#endif
#ifndef T_C_FCNTL
#define T_C_FCNTL(fd, cmd, arg)                fcntl((fd), (cmd), (arg))
#endif
#ifndef T_C_GETENV
# define T_C_GETENV(var)                       getenv((var))
#endif
#ifndef T_C_PUTENV
# define T_C_PUTENV(var)                       putenv((var))
#endif
#ifndef T_C_GETPID
# define T_C_GETPID()                          (T_INT4)getpid()
#endif
#ifndef T_C_GETHOSTBYNAME
# define T_C_GETHOSTBYNAME(name)               gethostbyname((name))
#endif
#ifndef T_C_GETPWUID
# define T_C_GETPWUID(uid)                     getpwuid((uid))
#endif
#ifndef T_C_GETSERVBYNAME
# define T_C_GETSERVBYNAME(name, proto)        getservbyname((name), (proto))
#endif

#ifndef T_C_GETSOCKOPT
# if defined(T_OS_AIX_64BIT)
#  define T_C_GETSOCKOPT(s, level, optname, optval, optlen) \
    getsockopt((s), (level), (optname), (optval), (unsigned int *)(optlen))
# elif defined(T_OS_AIX)
#  define T_C_GETSOCKOPT(s, level, optname, optval, optlen) \
    getsockopt((s), (level), (optname), (optval), (size_t *)(optlen))
# else
#  define T_C_GETSOCKOPT(s, level, optname, optval, optlen) \
    getsockopt((s), (level), (optname), (optval), (optlen))
# endif
#endif

#ifndef T_C_GETTIMEOFDAY
# define T_C_GETTIMEOFDAY(tv)                  gettimeofday(tv, NULL)
#endif
#ifndef T_C_HTONL
# define T_C_HTONL(x)                          htonl((x))
#endif
#ifndef T_C_HTONS
# define T_C_HTONS(x)                          htons((x))
#endif
#ifndef T_C_INET_ADDR
# define T_C_INET_ADDR(cp)                     inet_addr((cp))
#endif
#ifndef T_C_INET_LNAOF
# define T_C_INET_LNAOF(in)                    inet_lnaof((in))
#endif
#ifndef T_C_INET_MAKEADDR
# define T_C_INET_MAKEADDR(net, lna)           inet_makeaddr((net), (lna))
#endif
#ifndef T_C_INET_NETOF
# define T_C_INET_NETOF(in)                    inet_netof((in))
#endif
#ifndef T_C_INET_NETWORK
# define T_C_INET_NETWORK(cp)                  inet_network((cp))
#endif
#ifndef T_C_INET_NTOA
# define T_C_INET_NTOA(in)                     inet_ntoa((in))
#endif
#ifndef T_C_IOCTLSOCKET
# define T_C_IOCTLSOCKET(fd, request, arg) \
          ioctl((fd), (request), (char *)(arg))
#endif
#ifndef T_C_LISTEN
# define T_C_LISTEN(fd, backlog)               listen((fd), (backlog))
#endif
#ifndef T_C_LOCALTIME
# define T_C_LOCALTIME(clock)                  localtime((clock))
#endif
#ifndef T_C_MEMCMP
# define T_C_MEMCMP(s1, s2, size)              memcmp((s1), (s2), (size))
#endif
#ifndef T_C_MEMCPY
# define T_C_MEMCPY(dest, src, size)           memcpy((dest), (src), (size))
#endif
#ifndef T_C_MEMMOVE
# define T_C_MEMMOVE(dest, src, size)          memmove((dest), (src), (size))
#endif
#ifndef T_C_NTOHL
# define T_C_NTOHL(x)                          ntohl((x))
#endif
#ifndef T_C_NTOHS
# define T_C_NTOHS(x)                          ntohs((x))
#endif
#ifndef T_C_RECV
# define T_C_RECV(fd, msg, len, flags) \
          recv((fd), (msg), (len), (flags))
#endif

#ifndef T_C_RECVFROM
#if defined(T_OS_AIX_64BIT)
#define T_C_RECVFROM(fd, msg, len, flags, from, fromlen)  \
  recvfrom((fd), (msg), (len), (flags), (from), (unsigned int *)(fromlen))
#elif defined(T_OS_AIX)
#define T_C_RECVFROM(fd, msg, len, flags, from, fromlen)  \
  recvfrom((fd), (msg), (len), (flags), (from), (unsigned long *) (fromlen))
#elif defined(T_OS_OSS)
#define T_C_RECVFROM(fd, msg, len, flags, from, fromlen)  \
  recvfrom((fd), (msg), (len), (flags), (from), (size_t *) (fromlen))
#else
#define T_C_RECVFROM(fd, msg, len, flags, from, fromlen)  \
  recvfrom((fd), (msg), (len), (flags), (from), (fromlen))
#endif
#endif
#ifndef T_C_RENAME
# define T_C_RENAME(src, dest)                 rename((src), (dest))
#endif

#ifndef T_C_SELECT
# if defined(T_OS_WINDOWS)
/* nfds not used on Windows - MW 2006-04-28 */
#  define T_C_SELECT(width, readfds, writefds, exceptfds, timeout) \
           select(0, (readfds), (writefds), (exceptfds), (timeout))
# elif defined(T_OS_OSS)
#  define T_C_SELECT(width, readfds, writefds, exceptfds, timeout) \
           spt_select((width), (readfds), (writefds), (exceptfds), (timeout))
# else
#  define T_C_SELECT(width, readfds, writefds, exceptfds, timeout) \
           select((width), (readfds), (writefds), (exceptfds), (timeout))
#endif
#endif

#ifndef T_C_SEND
# define T_C_SEND(fd, msg, len, flags)        send((fd), (msg), (len), (flags))
#endif
#ifndef T_C_SENDTO
# define T_C_SENDTO(fd, msg, len, flags, to, tolen) \
          sendto((fd), (msg), (len), (flags), (to), (tolen))
#endif
#ifndef T_C_SETSOCKOPT
# define T_C_SETSOCKOPT(s, level, optname, optval, optlen) \
          setsockopt((s), (level), (optname), (optval), (optlen))
#endif
#ifndef T_C_SOCKET
# define T_C_SOCKET(dom, typ, pro)             socket((dom), (typ), (pro))
#endif
#ifndef T_C_STRCPY
# define T_C_STRCPY(dest, src)                 strcpy((dest), (src))
#endif
#ifndef T_C_STRLEN
# define T_C_STRLEN(str)                       (int)strlen((str))
#endif
#ifndef T_C_STRTOK
# define T_C_STRTOK(str, seperator)            strtok((str), (seperator))
#endif
#ifndef T_C_UNLINK
# define T_C_UNLINK(fname)                     unlink((fname))
#endif

#if defined(T_OS_WINDOWS)
# define T_REPLACE_SNPRINTF
# define T_C_SNPRINTF ss_snprintf
# define T_C_VSNPRINTF ss_vsnprintf

# ifndef T_UT_API
#  ifndef T_OS_WIN32
#   define T_UT_API
#  else
#   ifndef T_STATIC_LINK
#    ifndef T_UT_SOURCE
#     if !defined T_GENERATE_PROTOTYPES
#      define T_UT_API __declspec(dllimport)
#     else
#      define T_UT_API
#     endif
#    else
#     if !defined T_GENERATE_PROTOTYPES
#      define T_UT_API __declspec(dllexport)
#     else
#      define T_UT_API
#     endif
#    endif
#   else
#    define T_UT_API
#   endif /* T_STATIC_LINK */
#  endif /* T_OS_WIN32 */
# endif

# ifdef __cplusplus
extern "C" {
# endif
T_UT_API int T_ENTRY T_EXPORT ss_vsnprintf T_PROTO((
char *str,
size_t size,
const char *format,
va_list ap
));
T_UT_API int T_ENTRY T_EXPORT ss_snprintf T_PROTO((
char *str,
size_t size,
const char *format,
...
));
# ifdef __cplusplus
}
# endif

#else
# define T_C_SNPRINTF snprintf
# define T_C_VSNPRINTF vsnprintf
#endif

/*
 * utproc.c
 */
typedef enum {
  T_PRINFO_CPU_UTILIZATION,
  T_PRINFO_START_TIME,
  T_PRINFO_ELAPSED_TIME
} T_PRINFO_TYPE;


/**
 * T_UT_DSMM_TYPE defines the type of mutex's that can be stored
 * by a DSMM.
 */
typedef enum {
  T_UT_DSMM_MUTEX = 0,
  T_UT_DSMM_FAST_MUTEX,
  T_UT_DSMM_RW_MUTEX
} T_UT_DSMM_TYPE;

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_UT_DSMM_COPY_FUNC)
T_TYPEDEF((
T_PTR from, /** key to copy */
T_PTR *to   /** key to create */
));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_UT_DSMM_FREE_FUNC)
T_TYPEDEF((
T_PTR key /** key to free */
));

typedef struct T_UT_DSMM_STRUCT T_UT_DSMM_STRUCT, *T_UT_DSMM;

#endif /* _RTWORKS_UTIL_H_ */

