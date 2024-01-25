/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/build/dev/68/src/rtworks/config.h#9 $
 */

/* =================================================================== */
/*                                                                     */
/*              config.h -- system configuration definitions           */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_CONFIG_H_
#define _RTWORKS_CONFIG_H_

/* ---------------------------------------------------------------- */
/* This file is the "kitchen-sink" configuration header file.  It   */
/* should be included in every RTworks file.  When porting RTworks  */
/* to a new platform, this is the first file to look at, as this    */
/* contains most if not all system-dependent configuration info.    */
/*                                                                  */
/* I have tried to set up our source code so that is it as portable */
/* as possible, making it easy to do a "quick" port that may not    */
/* be an "optimal" port.  The capabilities that every machine may   */
/* not necessarily have are listed below with #define statements.   */
/* ---------------------------------------------------------------- */

/* ------------------------------------------------- */
/* Make sure T_${RTARCH_UC} is defined.  SMW 2/18/93 */
/* ------------------------------------------------- */

/* ----------------------------------------------------------------- */
/* Only do this for Talarian source code in $RTHOME/src.  This fixes */
/* bug 2550.  SMW 04/08/94                                           */
/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/* On VAX/VMS, this is just too painful, since VAX C only lets you use one */
/* /define on the command line.  Let's just go ahead and define what we    */
/* need.  SMW 4/18/93                                                      */
/* ----------------------------------------------------------------------- */
#if defined(vms) || defined(__VMS)
# if defined(__alpha) && !defined(T_ALPHA_VMS)
#  define T_ALPHA_VMS
# elif !defined(T_VAX_VMS)
#  define T_VAX_VMS
# endif
#endif /* vms */

#ifdef T_RTWORKS_SRC /* defined in imake.tmpl */
/* ---------------------------------------------------- */
/* Must break this up a bit for VAX C 3.1.  SMW 8/16/93 */
/* ---------------------------------------------------- */
# if !defined(T_HP700_HPUX) && !defined(T_IRIS_IRIX) && !defined(T_RS6000_AIX) && !defined(T_IA64_HPUX)
#  if !defined(T_SUN4_SOLARIS) && !defined(T_VAX_VMS) && !defined(T_ALPHA_VMS) && !defined(T_ALPHA_OSF)
#   if !defined(T_I86_SOLARIS) && !defined(T_I86_LINUX) && !defined(T_IA64_LINUX)
#    if !defined(T_I86_W32) && !defined(T_ALPHA_W32) && !defined(T_RS6000_AIX51)
#     if !defined(T_HP700_HPUX9) && !defined(T_RS6000_AIX3) && !defined(T_TANDEM_OSS)
config.h unknown RTARCH
#     endif
#    endif
#   endif
#  endif
# endif
#endif /* T_RTWORKS_SRC */

/* ----------------------- */
/* Version-number defines. */
/* ----------------------- */
#define T_RTWORKS40               /* unique define for RTworks 4.0 */
#define T_RTWORKS35_COMPAT    /* we support 3.5 compatibility */

/* -------------------------------------- */
/* Define T_OS_OSS if this is Tandem OSS. */
/* -------------------------------------- */
#if defined(_OSS_TARGET)
# define T_OS_OSS
/*
 * We need _TANDEM_SOURCE to get sockets to work.  We could force our
 * customers to define it, but if we need it then it's probably easier to
 * define it ourselves.  SMW 10/26/97
 */
# ifndef _TANDEM_SOURCE
#  define _TANDEM_SOURCE
# endif
/* Added this to get correct prototypes for Tandem OSS port. 12/5/00 dav */
# ifndef _XOPEN_SOURCE_EXTENDED
#  define _XOPEN_SOURCE_EXTENDED 1
# endif
#endif

/* ---------------------------------------------------------------------- */
/* VxWorks needs T_OS_UNIX to be set for the target code.  However,       */
/* the development host may not be a Unix system.  The VxWorks makefiles, */
/* makefile.vx sets this value manually.                                  */
/* ---------------------------------------------------------------------- */
/* -------------------------------------------- */
/* Define T_OS_UNIX if this is a Unix-based OS. */
/* -------------------------------------------- */
#if defined(unix) || defined(_unix) || defined(__unix) || defined(__unix__)
# define T_OS_UNIX
#endif
#if defined(_AIX) || defined(T_OS_OSS)
# define T_OS_UNIX
#endif

/*
 * Define T_OS_LINUX if this is Linux.
 */
#if defined(linux) || defined(_linux) || defined(__linux) || defined(__linux__)
# define T_OS_LINUX
# define T_OS_UNIX
# if defined(T_IA64_LINUX) || defined(_LP64)
#  define T_OS_LINUX_64BIT
#  define T_ARCH_64BIT
# endif
#endif

/* ----------------------------------- */
/* Define T_OS_SUNOS if this is SunOS. */
/* ----------------------------------- */
#if defined(sun) || defined(__sun) || defined(__sun__)
# define T_OS_SUNOS

/* ------------------------------------------------------------------- */
/* Define T_OS_SOLARIS2 if this is Solaris 2.X (which seems to include */
/* SunOS 5.X).  Perhaps we should define T_OS_SUNOS5 instead.          */
/* ------------------------------------------------------------------- */
# ifndef T_SKIP_TYPEDEFS
#  include <sys/types.h>
# endif
# if defined(T_SUN4_SOLARIS) || defined(T_I86_SOLARIS) || defined(_SYS_TYPES_H)
#  define T_OS_SOLARIS2
#  if defined(_LP64)
#   define T_OS_SOLARIS2_64BIT
#   define T_ARCH_64BIT
#  endif
# endif

#endif

/* --------------------------------- */
/* Define T_OS_IRIX if this is IRIX. */
/* --------------------------------- */
#if defined(sgi) || defined(__sgi)
# define T_OS_IRIX
#endif

/* --------------------------------------- */
/* Define T_OS_TANDEM if this is a Tandem. */
/* --------------------------------------- */
#if defined(__nonstopux)
# define T_OS_TANDEM
#endif

/* ---------------------------------- */
/* Define T_OS_HPUX if this is HP-UX. */
/* ---------------------------------- */
#if defined(hpux) || defined(__hpux)
# define T_OS_HPUX
/*
 * We need _HPUX_SOURCE to get sockets to work.  We could force our
 * customers to define it, but if we need it then it's probably easier to
 * define it ourselves.  SMW 09/01/95
 */
# ifndef _HPUX_SOURCE
#  define _HPUX_SOURCE
# endif
/* For HPUX 64-bit OS */
# if defined(__LP64__)
#  define T_OS_HPUX_64BIT
#  define T_ARCH_64BIT
# endif
#endif

/* ------------------------------- */
/* Define T_OS_AIX if this is AIX. */
/* ------------------------------- */
#if defined(_AIX)
# define T_OS_AIX
/* For AIX 64-bit OS */
# if defined(__64BIT__)
#  define T_OS_AIX_64BIT
#  define T_ARCH_64BIT
# endif
#endif

/* --------------------------------- */
/* Define T_OS_OSF if this is OSF/1. */
/* --------------------------------- */
#if defined(__osf__)
# define T_OS_OSF
# define T_ARCH_64BIT /* OSF/1 on Alpha is 64-bit OS */
#endif

/* ------------------------------- */
/* Define T_OS_VMS if this is VMS. */
/* ------------------------------- */
#if defined(VMS) || defined(vms) || defined(__VMS)
# define T_OS_VMS

/*
 * VMS 6 vs VMS 7.  TNX 02/18/99
 */
# ifdef __VMS_VER
#  if __VMS_VER < 70000000
#   define T_OS_VMS_6
#  endif
# else
/* __VMS_VER not defined, assume older VMS (really means older compiler). */
#  define T_OS_VMS_6
# endif

#endif

/* --------------------------------- */
/* Define T_OS_VAX if this is a VAX. */
/* --------------------------------- */
#if defined(VAX) || defined(vax) || defined(__vax) || defined(__vax__)
# define T_OS_VAX
#endif

/* ------------------------------------- */
/* Define T_OS_SPARC if this is a SPARC. */
/* ------------------------------------- */
#if defined(sparc) || defined(__sparc) || defined(__sparc__)
# define T_OS_SPARC
#endif

/* ----------------------------------------- */
/* Define T_OS_ALPHA if this is a Alpha AXP. */
/* ----------------------------------------- */
#if defined(ALPHA) || defined(alpha) || defined(__alpha) || defined(__alpha__)
# define T_OS_ALPHA
#endif

/* --------------------------------------------------------- */
/* Define T_OS_SVR4 if this is System V, Release 4.          */
/* IRIX 5.2 and CX/UX 7.1 are close, but not close enough to */
/* use this.  T_OS_SVR4 might be too confusing anyway (kinda */
/* like BSD vs SysV).  SMW 11/11/94                          */
/* --------------------------------------------------------- */
#if defined(T_OS_SOLARIS2)
# define T_OS_SVR4
#endif

#ifndef T_OS_UNIX
/*
 * Define T_OS_WINDOWS if this is Microsoft Windows.
 */
# if defined(_WINDOWS) || defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#  define T_OS_WINDOWS
#  define T_OS_WIN32
#  if defined(WIN64) || defined(_WIN64)
#   define T_OS_WIN64
#   define _P64
#   define T_ARCH_64BIT
#  endif
#  if defined(_ALPHA_)
#   define T_OS_ALPHA_WIN32
#  endif
# endif /* WINDOWS... */

/* ---------------------------------------------------------------------- */
/* These two lines (__STDC__ and __cplusplus) may be wrong, but we don't  */
/* know yet how to turn them on automatically.  We can turn on __STDC__   */
/* with /Za, but then <winnt.h> (included by <winsock.h>) won't compile   */
/* due to C++ comments in it (a not-so-subliminal shove to C++ by         */
/* Microsoft).  Defining __STDC__ and __cplusplus may hose other          */
/* product's header files.  SMW 08/05/95                                  */
/*                                                                        */
/* Defining __STDC__ to nothing caused problems in header files that used */
/* "#if __STDC__".  Defining __STDC__ to 1 seemed to enable strict ANSI   */
/* mode, so some POSIXish prototypes like getpid got lost.  We'll try     */
/* setting __STDC__ to 0 (which is like not having __STDC__ set for #if   */
/* but NOT for #ifdef which is what we use).  SMW 08/05/95                */
/* ---------------------------------------------------------------------- */
/*#define __STDC__ 0*/  /* see T_ANSI_C below.  SMW 08/16/95 */
/* ----------------------------------------------------------------------- */
/* __cplusplus seems to be wrong.  I think we are going to need a T_ANSI_C */
/* define that we use instead of __STDC__.  SMW 08/05/95                   */
/* ----------------------------------------------------------------------- */
/*#define __cplusplus*/
#endif /* T_OS_UNIX */

/*
 * Rudimentary support for compiling on CYGWIN/INTERIX; intended for imake ONLY
 * - MW 2006-03-30
 */
#if defined(__CYGWIN__) || defined(__INTERIX)
# if defined(T_OS_UNIX)
#  undef T_OS_UNIX
# endif
# if defined(T_OS_LINUX)
#  undef T_OS_LINUX
# endif
# define T_OS_WINDOWS
# define T_OS_WIN32
# define T_OS_UNIX_ON_WINDOWS
#endif

/* ------------------------------------------------------------------------ */
/* Define T_ASM_<arch> where <arch> is the instruction set for this RTARCH. */
/* ------------------------------------------------------------------------ */
/* T_ASM_IA32 */
#if defined(T_I86_SOLARIS) || defined(T_I86_W32) || defined(T_I86_LINUX)
# define T_ASM_IA32
# if defined(T_ARCH_64BIT)
#  define T_ASM_IA32_X64
# endif
/* T_ASM_IA64 */
#elif defined(T_IA64_LINUX) || defined(T_IA64_HPUX)
# define T_ASM_IA64
/* T_ASM_SPARC */
#elif defined(T_SUN4_SOLARIS)
# define T_ASM_SPARC
/* T_ASM_ALPHA */
#elif defined(T_ALPHA_OSF)
# define T_ASM_ALPHA
/* T_ASM_PARISC */
#elif defined(T_HP700_HPUX)
# define T_ASM_PARISC
/* T_ASM_MIPS */
#elif defined(T_IRIS_IRIX)
# define T_ASM_MIPS
/* T_ASM_POWER */
#elif defined(T_RS6000_AIX)
# define T_ASM_POWER
#endif

/* ---------------------------------------------------------------------- */
/* If we have ANSI C features (e.g., function prototypes, const keyword,  */
/* ANSI C headers, etc.), then define T_ANSI_C.  Some (older) platforms   */
/* won't define __STDC__ unless it is a strict ANSI C env, which is too   */
/* limiting for us.  Some platforms set __STDC__ to 0, which is OK as     */
/* long as we were using "#ifdef __STDC__".  A new concept is using a C++ */
/* compiler (Visual C++ on Windows) to compile our source.  Visual C++ (I */
/* don't know the version number) doesn't define __STDC__ nor             */
/* __cplusplus, which is really confusing.  SMW 08/16/95                  */
/* ---------------------------------------------------------------------- */
#if defined(__STDC__) || defined(_ANSI_C_SOURCE) || defined(__cplusplus) || defined(__STDC_EXT__)
# define T_ANSI_C
#endif
#if defined(T_OS_WINDOWS)
# define T_ANSI_C
#endif

/* --------------------------------------------------------------- */
/* Right here we need to define the largest possible numeric value */
/* (usually the largest possible double precision floating point   */
/* number), and the largest possible time (usually the largest     */
/* possible single precision floating point number).  If we change */
/* the size of numeric triples values or time tags, this might     */
/* break.                                                          */
/* --------------------------------------------------------------- */
#ifdef T_ANSI_C
# define T_MAXINT INT_MAX
# define T_MAXNUMERIC DBL_MAX
# define T_MAXTIME FLT_MAX
#else
# ifdef T_OS_WINDOWS
#  define T_MAXINT INT_MAX
#  define T_MAXNUMERIC MAXDOUBLE
#  define T_MAXTIME MAXFLOAT
# else
#  define T_MAXINT MAXINT
#  define T_MAXNUMERIC MAXDOUBLE
#  define T_MAXTIME MAXFLOAT
# endif
#endif

#if defined(T_OS_OSS)
# define vsnprintf(b,s,f,p) vsprintf(b,f,p)
#endif

/* --------------------------------------------------------------- */
/* If this machine has <stdarg.h>, define T_HAS_STDARG_H.  Usually */
/* the existence of <stdarg.h> implies function prototypes.        */
/* --------------------------------------------------------------- */
/* This MUST be on one line for VAX C 3.1.  SMW 6/23/93 */
#if defined(T_OS_VMS) || defined(T_OS_IRIX)
# define T_HAS_STDARG_H
#endif
#if defined(T_OS_AIX) || defined(T_ANSI_C)
# define T_HAS_STDARG_H
#endif

/* -------------------------------------------------------------- */
/* Define T_HAS_PROTOTYPES if this system has function prototypes */
/* These all have to be on one line for the VMS C compiler!!!     */
/* -------------------------------------------------------------- */
#if defined(T_HAS_STDARG_H)
# define T_HAS_PROTOTYPES
#endif

/* -------------------------------------------------------------- */
/* If we have prototypes, then define XTFUNCPROTO.  X11R4.18 does */
/* not define it as it should.  SMW 1/16/92                       */
/* -------------------------------------------------------------- */
#ifdef T_HAS_PROTOTYPES
# ifndef XTFUNCPROTO
#  define XTFUNCPROTO
# endif
#endif

/* ----------------------------------------------------------------- */
/* If we do not have prototypes, then define _NO_PROTO. Motif 1.1    */
/* uses this, and DataViews 9.0 also appears to have picked this up. */
/* ----------------------------------------------------------------- */
#ifndef T_HAS_PROTOTYPES
# ifndef _NO_PROTO
#  define _NO_PROTO
# endif
#endif

/* ------------------------------------------------------------- */
/* If this machine has the ANSI C const keyword, define T_CONST. */
/* ------------------------------------------------------------- */
#if defined(T_ANSI_C) || defined(T_GENERATE_PROTOTYPES)
# define T_CONST const
#else
# define T_CONST /* nothing */
#endif

/* ------------------------------------------------------------------- */
/* If this machine has Unix-domain sockets, define T_HAS_UNIX_DOMAIN. */
/* ------------------------------------------------------------------- */
#if !defined(T_OS_WINDOWS) && !defined(T_OS_OSS)
# define T_HAS_UNIX_DOMAIN
#endif

/*
 * Enable multi-threading for those platforms with an MT API.
 */

#if defined(T_OS_SOLARIS2) || defined (T_OS_LINUX) || defined(T_OS_WIN32) || defined(T_OS_OSF) || defined(T_OS_AIX) || defined(T_OS_VMS) || defined(T_OS_IRIX_62) || defined(T_OS_OSS) || (defined(T_OS_HPUX) && defined(T_ANSI_C))
# ifndef T_NO_THREADS /* allow way to force threads off.  SMW 11/12/97 */
#  define T_HAS_THREADS
#  if defined(T_OS_HPUX_11_00) || defined(T_OS_HPUX_64BIT) || defined(T_OS_AIX_64BIT) || defined(T_OS_LINUX_64BIT)

/*
 * Turn on Posix threads for HP.  We needed the reentrant functions
 * getpwuid_r.  The pwd.h header required that _REENTRANT be set.
 * _REENTRANT is set in sys/stdsyms.h by setting _POSIX_C_SOURCE to
 * the value below (or greater).  BMQ, MK 08/15/98
 *
 * Updated from 199506 to 199506L -- as of SS5.2 Rev 3
 *
 */
#   define _POSIX_C_SOURCE   199506L
#  endif

# endif
#endif /* MT-capable platforms */

/* --------------------------------------------------------------------- */
/* BSD Unix introduced the concept of a signal handler staying installed */
/* after a signal has been delivered.  Otherwise, the signal handler     */
/* must be reinstalled each time (bleh).                                 */
/* --------------------------------------------------------------------- */
#if defined(T_OS_SOLARIS1) || defined(T_OS_OSF) || defined(T_OS_LINUX)
# define T_SIGNALS_STAY_CAUGHT
#endif

/* ---------------------------------------------------------------------- */
/* The times function returns CPU usage in system clock ticks, where each */
/* clock tick is 1/HZ seconds long.  The two most common values for HZ    */
/* are 60 and 100.  This is only used for benchmarks.  We really need to  */
/* start using sysconf() to determine this value.  SMW 2/20/92            */
/* ---------------------------------------------------------------------- */
#ifdef T_OS_VMS /* VMS does not have sysconf().  SMW 02/28/94 */
# define T_CLOCK_TICKS_PER_SECOND 100
#elif defined(T_ANSI_C) || defined(T_IRIS_IRIX) /* try this.  SMW 02/27/94 */
# define T_CLOCK_TICKS_PER_SECOND sysconf(_SC_CLK_TCK)
#elif defined(T_OS_IRIX) || defined(__hp9000s700) || defined(T_OS_AIX)
# define T_CLOCK_TICKS_PER_SECOND 100
#elif defined(__hp9000s300)
# define T_CLOCK_TICKS_PER_SECOND 50
#else
# define T_CLOCK_TICKS_PER_SECOND 60
#endif

/*
 * Define T_PTR_SIZE to the number of bytes in a pointer.  This is
 * necessary because we can't use sizeof(T_PTR) in preprocessor
 * expressions. SMW 08/06/94
 */
#if defined(T_ARCH_64BIT)
# define T_PTR_SIZE 8
#else
# define T_PTR_SIZE 4
#endif

/* ------------------------------------------------------------------ */
/* Define T_OPTIMIZE_CODE to disable run-time debugging capabilities. */
/* ------------------------------------------------------------------ */
/*#define T_OPTIMIZE_CODE*/

/* -------------------------------------------------------------------- */
/* I have broken up the features of T_OPTIMIZE_CODE a bit so that I can */
/* turn off/on individual pieces a bit easier with code coverage tools. */
/* SMW 02/12/94                                                         */
/* -------------------------------------------------------------------- */
#ifdef T_OPTIMIZE_CODE
# define T_NO_ASSERTS
# define T_NO_DBG
/* --------------------------- */
/* See bug 3183.  SMW 09/01/95 */
/* --------------------------- */
/*#define T_NO_CHECK_ARGS*/
#endif

/* ---------------------------------------------------------------------- */
/* Define whatever types are needed for various sized integers and reals. */
/* We have to use typedefs instead of #define's so that the generated     */
/* prototypes are correct.  This requires us to have two typedefs for     */
/* some: one for signed, and one for unsigned (you cannot "typedef int    */
/* my_int" and then declare a "unsigned my_int x;".  Or we could also add */
/* extra typedefs for unsigned integers.  Should T_INT2 be named          */
/* T_OS_INT2 instead? SMW 7/16/92                                         */
/* ---------------------------------------------------------------------- */
#ifndef T_SKIP_TYPEDEFS /* useful when including from D or Imakefile */
/* ---------------------------------------------------------------------- */
/* We really need T_CHAR to be signed, otherwise bit-shifting code breaks */
/* if the default for "char" with a compiler is unsigned.  SMW 8/25/93    */
/*                                                                        */
/* Making T_CHAR explicitly signed causes problems when mixing T_CHAR *   */
/* and T_STR.  Making the T_STR typedef use signed T_CHAR causes problems */
/* when mixing T_STR and char *.  I guess what I really want is T_INT1    */
/* and T_UINT1 for those cases when I logically need an integer, not      */
/* a character.  SMW 8/25/93                                              */
/* ---------------------------------------------------------------------- */
typedef char T_CHAR;
typedef unsigned char T_UCHAR;
/* ------------------------------------------------------------- */
/* Our HP-UX 9.01 C++ doesn't have signed keyword.  SMW 10/13/95 */
/* ------------------------------------------------------------- */
# if defined(T_ANSI_C) && !(defined(T_OS_HPUX) && defined(__cplusplus))
typedef signed char T_INT1;
# else
typedef char T_INT1;
# endif
typedef unsigned char T_UINT1;
typedef short T_INT2;
typedef unsigned short T_UINT2;
# if defined(T_ARCH_64BIT)
typedef int T_INT4;
typedef unsigned int T_UINT4;
# else
typedef long T_INT4;
typedef unsigned long T_UINT4;
# endif
typedef float T_REAL4; /* needed for generated prototypes */
typedef double T_REAL8;
/* Use TutRealToStr (see bug1289) instead.  SMW 03/14/97 */
/*#define T_REAL8_SPEC "%f"*/
typedef char *T_STR;
typedef T_CONST char *T_CSTR;
/*
 * This T_TEXT typedef is just too painful, because we always have to
 * cast T_STR to T_TEXT, and thus we are constantly making coding
 * mistakes by casting things like T_STR * to T_TEXT and then getting
 * crashes.  SMW 10/31/96
 */
# define T_TEXT T_STR
/* support for unicode - utf translations, redeclared as wchar_t in common.h SMV 10/22/98 */
/* typedef T_UINT2 T_WCHAR; */   /* Unicode chararacter 11/12/96 RDH            */
typedef T_UCHAR *T_ISTR;    /* Since T_TEXT is no longer an unsigned char  */
                            /* and that's what's needed to be 8 bit clean  */
                            /* 11/12/96 RDH                                */

/* T_PTR is defined in <arph/nameser.h> (see bug 4131).  SMW 02/08/96 */
# ifdef T_PTR
#  undef T_PTR
# endif
typedef void *T_PTR;

typedef struct T_STR8 T_STR8;
struct T_STR8 {
  T_STR str; /* clumsy, but makes cross-platform usage a bit easier. */
# if T_PTR_SIZE != 8
  T_CHAR pad[4];
# endif
};

typedef struct T_PTR8 T_PTR8;
struct T_PTR8 {
  T_PTR ptr; /* clumsy, but makes cross-platform usage a bit easier. */
# if T_PTR_SIZE != 8
  T_CHAR pad[4];
# endif
};

typedef struct T_STR_ARRAY8 T_STR_ARRAY8;
struct T_STR_ARRAY8 {
  T_STR *str_array; /* clumsy, but makes cross-platform usage a bit easier. */
# if T_PTR_SIZE != 8
  T_CHAR pad[4];
# endif
};

/*
 * Define T_NINT and T_UNINT. T_NINT and T_UNINT are signed/unsigned integers
 * where sizeof(T_NINT)==sizeof(T_PTR). This gives us an easy way to do pointer
 * arithmetic, and is also needed for T_ATOM, as atomics are likely to only
 * work on native-register-sized values. For now, the typedef is is simply
 * '[unsigned] long'.
 *
 * ...Except on Windows. Figures.
 */
#if defined(WIN64) || defined(_WIN64)
typedef __int64 T_NINT;
typedef unsigned __int64 T_UNINT;
#else
typedef long T_NINT;
typedef unsigned long T_UNINT;
#endif

/*
 * Define T_ATOM. T_ATOM is a pointer to an integer intended to be used with
 * atomic operations (which also means sizeof(T_ATOM_TYPE)==sizeof(T_PTR)).
 * T_ATOM_TYPE may be used to declare such integers.
 */
typedef volatile T_NINT T_ATOM_TYPE, * T_ATOM;

/*
 * Define T_NATIVE_INT8 if we can use T_INT8 w/o using structures.  We skip
 * the T_PTR_SIZE trick because we want T_INT8 to be a native type if
 * possible.  Native support varies by platform and compiler, so we can't
 * assume much (some compilers choke on "long long", for example).  Users
 * can #define T_NATIVE_INT8 to force native type.
 * SMW 09/16/96
 */
# if defined(T_OS_WIN32) || defined(T_OS_NW)
#  define T_NATIVE_INT8  /* MSVC has __int64, GCC has long long */
# endif

# if defined(T_OS_ALPHA) && defined(T_OS_OSF)
#  define T_NATIVE_INT8  /* Digital UNIX has 8-byte integers */
# endif

# if defined(T_OS_HPUX) || defined(T_OS_LINUX) || defined(T_OS_IRIX) || defined(T_OS_AIX) || defined(T_OS_OSS)
#  define T_NATIVE_INT8  /* These OS's support long long */
# endif

/*
 * __DECC_VER is defined for DEC C 5.0 and higher, which has long long.
 * We decided to not use __int64 with DEC C 4.0, which it does not
 * fully implement (cannot use large integer constants in code).  SMW
 * BM 08/14/97
 */
# if defined(T_ALPHA_VMS) && defined(__DECC_VER)
#  define T_NATIVE_INT8   /* ALPHA_VMS supports long long too. BM 08/12/97 */
# endif

/*
 * Detecting Solaris 2.x at compile time is not easy, so user might have
 * to define T_NATIVE_INT8.
 *
 * Force INT8 - don't use -Xc (or use GCC). If you MUST compile on something
 * that doesn't have (or doesn't like) a native INT8, then define
 * T_HIDE_NATIVE_INT8. Otherwise, 6.7 requires it, so just try to define it
 * even if it might cause a compiler error. - MW 2005-05-13
 */
/* #if defined(T_OS_SOLARIS2) && (__STDC__ == 0) // not allowed with acc -Xc */
# if defined(T_OS_SOLARIS2)
#  define T_NATIVE_INT8  /* Solaris 2.x has 8-byte integers */
# endif

# ifdef T_HIDE_NATIVE_INT8  /* allow user to force off */
#  ifdef T_NATIVE_INT8
#   undef T_NATIVE_INT8
#  endif
# endif

# ifdef T_NATIVE_INT8

#  if T_PTR_SIZE == 8 && !defined(T_OS_WINDOWS)
     /*
      * safe bet that pointer and long same size
      *
      * Not on Windows! They just HAD to be different! - MW 2006-04-27
      */
     typedef long T_INT8;
     typedef unsigned long T_UINT8;
#  else
#   if (defined(T_OS_WINDOWS) || defined(T_OS_NW)) && !defined(__GNUC__)
#    if defined(T_GENERATE_PROTOTYPES)
         typedef long double long T_INT8;  /* cproto doesn't recognize __int64 */
         typedef unsigned long double long T_UINT8;
#    else
         typedef __int64 T_INT8;
         typedef unsigned __int64 T_UINT8;
#    endif
#   else
       typedef long long T_INT8;
#    if !defined(T_OS_OSS) /* OSS doesn't like unsigned long long */
       typedef unsigned long long T_UINT8;
#    endif
#   endif
#  endif

#  if defined(T_OS_HPUX_64BIT) || defined(T_OS_LINUX_64BIT) || (defined(T_OS_ALPHA) && defined(T_OS_OSF))
#   define T_INT8_CONST(a) a
#  else
#   if defined(T_OS_WINDOWS)
#    define T_INT8_CONST(a) a ## i64
#   elif defined(T_OS_OSS)
#    define T_INT8_CONST(a) a ## L
#   else
#    define T_INT8_CONST(a) a ## LL
#   endif
#  endif

#  define T_MAX_INT8 T_INT8_CONST(0x7FFFFFFFFFFFFFFF)
#  define T_MIN_INT8 T_INT8_CONST(0x8000000000000000)

# else

/* 6.7 needs a native INT8 - MW 2005-05-13 */
#  ifdef T_HIDE_NATIVE_INT8
     typedef struct T_INT8 T_INT8;
     struct T_INT8 {
         T_CHAR data[8];
     };
#  else
config.h unable to compile on platform without native INT8
#  endif

/*#define T_INT8_SPEC ??? */ /* not printable */

# endif /* T_NATIVE_INT8 */

/*
 * Define T_NATIVE_REAL16 if we can use T_REAL16 w/o using structures.  We skip
 * the T_PTR_SIZE trick because we want T_REAL16 to be a native type if
 * possible.  Native support varies by platform and compiler, so we can't
 * assume much (some compilers choke on "long long", for example).  Users
 * can #define T_NATIVE_REAL16 to force native type.
 * SMW 09/16/96
 */

/*
 * Detecting Solaris 2.x at compile time is not easy, so user might have
 * to define T_NATIVE_REAL16.
 */
# if defined(T_OS_SOLARIS2) && (defined(sparc) || defined(__sparc) || defined(__sparc__))
#  define T_NATIVE_REAL16  /* Solaris 2.x has 16-byte reals */
# endif

# if (defined(T_OS_AIX) && defined(__LONGDOUBLE128))
#  define T_NATIVE_REAL16  /* AIX 4. has longdouble, precision supported, but not range */
# endif

# if defined (T_OS_HPUX)
#  if (defined(__STDC_EXT__)) || (defined (__cplusplus))
/*
 * HP-UX 10.20 also supports long double but this is an ANSI Extension
 * Mode feature. My previous comment version 1.72 was wrong
 * so I am correcting in this - BM 08/05/97
 *
 * and cc -Ae support long double, to work around this with we are using
 *  __STDC__EXT__, which -Ae defines. from Scott's previous comment
 */
#   define T_NATIVE_REAL16
#  endif
# endif


# if defined(T_ALPHA_VMS)
#  define T_NATIVE_REAL16  /* VMS 6.2 has 16-byte reals (BM 06/06/97) */
# endif

# if 0 /* the gcc 2.5.5 that we have core dumps with long double :-( */
#  if (defined(T_OS_SUNOS) && defined(__GNUC__))
#  define T_NATIVE_REAL16  /* gcc on SunOS has 16-byte reals */
#  endif
# endif

# ifdef T_HIDE_NATIVE_REAL16  /* allow user to force off */
#  ifdef T_NATIVE_REAL16
#   undef T_NATIVE_REAL16
#  endif
# endif

# ifdef T_NATIVE_REAL16

typedef long double T_REAL16;
#  if defined (T_ALPHA_VMS) || defined (T_OS_HPUX) || defined (T_OS_AIX)
/*
 * on VMS it has to be %Le to get similar results to the
 * ones on Solaris (BM 06/06/97). On HP_UX 10.2 it's the same. (BM 08/05/97)
 */
#   define T_REAL16_SPEC "%Le"
#  else
#   define T_REAL16_SPEC "%Lf"
#  endif

# else

typedef struct T_REAL16 T_REAL16;
struct T_REAL16 {
  T_CHAR data[16];
};

/*#define T_REAL16_SPEC ??? */ /* not printable */

# endif

/* Windows callback types.  SMW 03/13/96 */
# if defined T_OS_WINDOWS && !defined (T_GENERATE_PROTOTYPES)
#  define T_WIN_CB_FUNC_RETURN LRESULT CALLBACK
# else
typedef int T_WIN_CB_FUNC_RETURN; /* mainly exists for cproto */
# endif

/*
 * For the PC platforms, we compile our code with a certain calling convention,
 * thus we need to use this convention in our function prototypes and function
 * typedefs for safety.  SMW 01/24/97
 */
/*
 * for windows, signal system call requires function pointers to be of
 * type __cdecl and not __stdcall. So we have to change some proto
 * types to use T_ENTRY_VA which is __cdecl.
 * NOTE: If any of the windows system call require a __cdecl function
 * pointer make sure you have a function pointer which uses T_ENTRY_VA1
 * T_ENTRY_VA2 and not T_ENTRY1 T_ENTRY2.
 * We introduced a new T_ENTRY_MAIN which is defined as __cdecl on WIN32
 * and nothing on other platforms. This is required so can differentiate
 * between T_ENTRY_VA which is for var_arg functions and main which needs
 * to be __cdecl on windows. KRM, BM 06/22/98
 */

# if !defined(T_GENERATE_PROTOTYPES)

#  ifdef T_OS_WIN32
#   define T_ENTRY_VA __cdecl
#   define T_ENTRY_MAIN __cdecl
#   define T_ENTRY_VA1 T_ENTRY_VA /* Win32 wants entry before * on typedef. */
#   define T_ENTRY_VA2
#  else
#   define T_ENTRY_VA
#   define T_ENTRY_MAIN
#   define T_ENTRY_VA1
#   define T_ENTRY_VA2
#  endif

#  ifdef T_OS_WIN32
#   define T_ENTRY __stdcall
#   define T_ENTRY1 T_ENTRY /* Win32 wants entry before * on tyepdef. */
#   define T_ENTRY2
/* Dataviews requires __cdecl calling convention.  So, all the HCI callback */
/* functions called by Dataviews functions should be __cdecl.  MK 11/27/98. */
#   define T_DV_ENTRY __cdecl
#   define T_DV_ENTRY1 T_DV_ENTRY
#   define T_DV_ENTRY2
#  else
#   define T_ENTRY
#   define T_ENTRY1
#   define T_ENTRY2
#   define T_DV_ENTRY
#   define T_DV_ENTRY1
#   define T_DV_ENTRY2
#  endif /* T_OS_WIN32 */

#  ifdef __cplusplus
#   define T_ENTRY_C "C"
#  else
#   define T_ENTRY_C
#  endif

#  define T_EXPORT

# else  /* defined(T_GENERATE_PROTOTYPES) */

#  define T_ENTRY
#  define T_ENTRY_MAIN
#  define T_EXPORT
#  define T_ENTRY1
#  define T_DV_ENTRY1
#  define T_DV_ENTRY2
#  define T_ENTRY2
#  define T_ENTRY_VA
#  define T_ENTRY_VA1
#  define T_ENTRY_VA2
#  define T_ENTRY_C

/*
 * This conditional block should let us continue to generate prototypes on
 * Solaris until I can get our modified cproto built and installed there
 * too. -- KLC 4/7/99
 */
#  if !defined(T_OS_WIN32)
#   define T_ENTRY
#   define T_ENTRY1
#   define T_ENTRY2
#   define T_DV_ENTRY
#   define T_DV_ENTRY1
#   define T_DV_ENTRY2

#   define T_EXPORT
#  endif

/*
 * We don't include system header files when generating prototypes, because
 * cproto cannot handle some of the exotic compiler extensions that are used
 * in them on some platforms (notably Windows). Types that would normally be
 * picked up from system headers a typedef'd here just to keep cproto happy.
 * These serve no other purpose. --KLC 12/23/98
 */
struct in_addr { };
struct timeval { };
typedef struct { } FILE;
typedef int fd_set;
typedef int jmp_buf;
typedef int u_long;
typedef int va_list;
typedef int wchar_t;
typedef int BOOL;
typedef int (*FARPROC)(void);
typedef int HANDLE;
typedef int HINSTANCE;
typedef int HKEY;
typedef int HMODULE;
typedef int HWND;
typedef int SOCKET;
typedef int UINT;
typedef int WPARAM;
typedef int LPARAM;

# endif /* !defined(T_GENERATE_PROTOTYPES) */

# define T_STATIC_VAR(stmt) static stmt

# define T_CDECL T_ENTRY T_EXPORT /* for compatibility */

# define T_STRUCT_TYPEDEF(name) typedef struct name ## _STRUCT \
                                               name ## _STRUCT, \
                                              *name

#endif /* T_SKIP_TYPEDEFS */

/* ------------------------------------------------------------------------ */
/* Define the printf format specs needed to print our data types.  It's too */
/* bad there's no standard way to do this.  SMW 12/3/93                     */
/* ------------------------------------------------------------------------ */

#define T_INT_SPEC "%d"
#define T_UINT_SPEC "%u"
#define T_ENUM_SPEC "%d" /* for enumerated types */
#define T_INT2_SPEC "%d" /* will be cast to (int) on most machines */
#define T_UINT2_SPEC "%u" /* will be cast to (uint) on most machines */
#if T_PTR_SIZE == 8 || defined(T_OS_VMS)
# define T_INT4_SPEC_SUFFIX "d" /* DEC C 1.2 on OpenVMS AXP 1.0 does not have */
# define T_UINT4_SPEC_SUFFIX "u" /* 'l'.  SMW 03/19/94 */
# define T_HEX4_SPEC_SUFFIX "x"
# define T_HEXU4_SPEC_SUFFIX "X"
# if defined(T_OS_WINDOWS)
#  define T_INT8_SPEC_SUFFIX "I64d"
#  define T_HEX8_SPEC_SUFFIX "I64x"
#  define T_HEXU8_SPEC_SUFFIX "I64X"
# else
#  define T_INT8_SPEC_SUFFIX "ld"
#  define T_HEX8_SPEC_SUFFIX "lx"
#  define T_HEXU8_SPEC_SUFFIX "lX"
# endif
#else
# define T_INT4_SPEC_SUFFIX "ld"
# define T_UINT4_SPEC_SUFFIX "lu"
# define T_HEX4_SPEC_SUFFIX "lx"
# define T_HEXU4_SPEC_SUFFIX "lX"
# if defined(T_OS_WINDOWS)
#  define T_INT8_SPEC_SUFFIX "I64d"
#  define T_UINT8_SPEC_SUFFIX "I64u"
#  define T_HEX8_SPEC_SUFFIX "I64x"
#  define T_HEXU8_SPEC_SUFFIX "I64X"
# elif defined(T_OS_OSS)
#  define T_INT8_SPEC_SUFFIX "Ld"
#  define T_UINT8_SPEC_SUFFIX "Lu"
#  define T_HEX8_SPEC_SUFFIX "Lx"
#  define T_HEXU8_SPEC_SUFFIX "LX"
# else
#  define T_INT8_SPEC_SUFFIX "lld"
#  define T_UINT8_SPEC_SUFFIX "llu"
#  define T_HEX8_SPEC_SUFFIX "llx"
#  define T_HEXU8_SPEC_SUFFIX "llX"
# endif /* T_OS_WINDOWS */
#endif

#define T_NINT_SPEC "%ld"
#define T_NINT_FSPEC(s) "%" s "ld"
#define T_UNINT_SPEC "%lu"
#define T_UNINT_FSPEC(s) "%" s "lu"

#define T_NHEX_SPEC "%lx"
#define T_NHEX_FSPEC(s) "%" s "lx"
#define T_NHEXU_SPEC "%lX"
#define T_NHEXU_FSPEC(s) "%" s "lX"

#define T_INT4_SPEC "%" T_INT4_SPEC_SUFFIX
#define T_UINT4_SPEC "%" T_UINT4_SPEC_SUFFIX
#define T_INT4_FSPEC(s) "%" s T_INT4_SPEC_SUFFIX
#define T_UINT4_FSPEC(s) "%" s T_UINT4_SPEC_SUFFIX

#define T_INT8_SPEC "%" T_INT8_SPEC_SUFFIX
#define T_UINT8_SPEC "%" T_UINT8_SPEC_SUFFIX
#define T_INT8_FSPEC(s) "%" s T_INT8_SPEC_SUFFIX
#define T_UINT8_FSPEC(s) "%" s T_UINT8_SPEC_SUFFIX

#define T_HEX4_SPEC "%" T_HEX4_SPEC_SUFFIX
#define T_HEXU4_SPEC "%" T_HEXU4_SPEC_SUFFIX
#define T_HEX4_FSPEC(s) "%" s T_HEX4_SPEC_SUFFIX
#define T_HEXU4_FSPEC(s) "%" s T_HEXU4_SPEC_SUFFIX

#define T_8HEX4_SPEC "%08" T_HEX4_SPEC_SUFFIX
#define T_8HEXU4_SPEC "%08" T_HEXU4_SPEC_SUFFIX

#define T_HEX8_SPEC "%" T_HEX8_SPEC_SUFFIX
#define T_HEXU8_SPEC "%" T_HEXU8_SPEC_SUFFIX
#define T_HEX8_FSPEC(s) "%" s T_HEX8_SPEC_SUFFIX
#define T_HEXU8_FSPEC(s) "%" s T_HEXU8_SPEC_SUFFIX

#define T_16HEX8_SPEC "%016" T_HEX8_SPEC_SUFFIX
#define T_16HEXU8_SPEC "%016" T_HEXU8_SPEC_SUFFIX

/*
 * Forget test case results. Print pointers the way they SHOULD look; that is,
 * prefixed with '0x', with leading zeros to make them 2*sizeof(void*) digits.
 * Before, what we had was some platforms that would add the '0x', some that
 * had an extra 'x' after them (what the heck is '%px' anyway?), etc.
 * "Consistent" results? I didn't see the consistency... - MW 2005-10-11
 *
 * We'll also save some effort by using our existing T_*HEX*_SPEC macros. That
 * way we only have to maintain the one set, rather than two.
 */
#if T_PTR_SIZE == 8
# define T_PTR_SPEC "0x" T_16HEX8_SPEC
# define T_PTRU_SPEC "0x" T_16HEXU8_SPEC
#else
# define T_PTR_SPEC "0x" T_8HEX4_SPEC
# define T_PTRU_SPEC "0x" T_8HEXU4_SPEC
#endif

#if defined(T_OS_WIN64)
# define T_SOCK_SPEC T_INT8_SPEC
#else
# define T_SOCK_SPEC T_INT4_SPEC
#endif

/* ------------------------------------------------------------ */
/* Pickup PT modifications for all platforms if PT_KIT defined  */
/* ------------------------------------------------------------ */
#if defined(PT_KIT)
# define T_MODIFY_PT
# define T_PT_MODIFY_LM
static char *ptCompiled = "Compile Stamp = " __FILE__ ", " __DATE__ " " __TIME__;
#endif

/* ----------------------------- */
/* Setup character substitutions */
/* ----------------------------- */
#define T_LEFTB "["
#define T_RIGHTB "]"

#define T_CONTACT_CO "TIBCO Software Inc"

/* ------------------------------------------- */
/* SNA should work on the following platforms: */
/* (KEL 8/3/99)                                */
/* Can't define UNIX yet because we don't have */
/* have the header files. (KEL 12/7/1999)      */
/* When we have the header files add:          */
/*  || (defined(T_OS_SOLARIS2) && defined(T_OS_SPARC))  */
/* ------------------------------------------- */

#if defined(T_OS_SOLARIS2) || defined(T_OS_LINUX) || defined(T_HP700_HPUX)

# define T_USE_POLL

# if 0
# define T_FD_SET_MAY_GROW
/*
 * For solaris, redefinition of FD_SETSIZE to a value larger than 1024 is
 * required to enable select to use a large set size.
 */
# define FD_SETSIZE 1032
# endif

#endif

#endif /* _RTWORKS_CONFIG_H_ */
