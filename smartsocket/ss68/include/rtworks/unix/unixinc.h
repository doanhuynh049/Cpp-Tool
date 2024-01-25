/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/unix/unixinc.h#2 $
 */

/* =================================================================== */
/*                                                                     */
/*             unixinc.h -- headers for UNIX c compiler                */
/*                                                                     */
/* =================================================================== */

#ifndef _RTWORKS_UNIX_INC_H_
#define _RTWORKS_UNIX_INC_H_

/* ---------------------------------------------------------------- */
/* All the Unix compilers I have seen so far are pretty much the    */
/* same: they are all based on PCC (Portable C Compiler).  BellTech */
/* claims this is "the most advanced, most reliable C compiler for  */
/* Intel processors".  Yeah, right.  It is also the most archaic    */
/* and has been hacked to pieces the most.  I long for Turbo C      */
/* or Lightspeed C.  SMW 2/1/89                                     */
/* ---------------------------------------------------------------- */

/* ------------------------------------------------------------------ */
/* Well, things have improved somewhat over the last two years.  Sun, */
/* SGI, and HP now offer ANSI C compilers.  System V.4 has an ANSI C  */
/* compiler.  Unfortunately, this has complicated things since we     */
/* have to support both ANSI C and older C.  SMW 10/9/91              */
/* ------------------------------------------------------------------ */

#if defined(T_ANSI_C) || defined(T_OS_LINUX_64BIT)
#include <stdlib.h>
#endif

/* ------------------------------- */
/* Unix header files are a MESS!!! */
/* ------------------------------- */

#if defined(T_OS_LINUX) || defined(T_OS_SUNOS)
#include <sys/param.h> /* For MAXPATHLEN JWA, RJ 08/18/99 */
#endif
#include <sys/types.h> /* for times and caddr_t */
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#ifndef T_OS_OSS
#include <sys/file.h>
#endif
#include <sys/times.h> /* for times (used in TutGetCpuTime) */
#include <sys/time.h>  /* timeval */
#ifndef T_OS_SUNOS
#include <time.h> /* struct tm */
#endif
#if defined(T_OS_SPARC) && !defined(__cplusplus) && defined(T_OS_SOLARIS1)
#include <vfork.h>
#endif

/* MJC 12/01/00 Tandem OSS port */
#if defined(T_OS_OSS)
#define MAXPATHLEN 1024
#include <arpa/inet.h>
#endif

#include <errno.h> /* global error number and defines */
#include <stdio.h> /* fprintf(), NULL, stderr */
#include <string.h> /* str*, mem* */
#ifdef T_OS_IRIX
#include <bstring.h> /* BSD b*, needed for cc -fullwarn */
#include <sys/param.h>
#endif
#include <unistd.h>

/*
 * We checked on most of our unix platforms and it seems to be existing in
 * same place. For now leave it here and when we get to porting
 * we can change it. This is for new file src/util/expire.c for timebomb
 * stuff (BBM, BM 06/09/98)
 */
#include <utime.h>

/* For dynamic library loading calls dl{open, close, sym,error} -BM 06/11/98 */
#if defined (T_OS_SOLARIS2) || defined (T_OS_AIX) || defined(T_OS_OSF) || defined (T_OS_IRIX) || defined(T_OS_LINUX) || defined(T_OS_SUNOS)
#include <dlfcn.h>
#elif defined(T_OS_HPUX)
#include <dlfcn.h>
#include <dl.h>
#endif
/* --------------------------------------------------------------------- */
/* HP-UX 7.0 doesn't have this header, but we really don't support HP-UX */
/* 7.0 anymore, so blow off checking.  SMW 2/20/92                       */
/* --------------------------------------------------------------------- */
#if !defined(T_ANSI_C) && !defined(T_OS_TANDEM) && (!defined(T_OS_SUNOS) || defined(T_OS_SOLARIS1))
#include <strings.h> /* Berkeley string stuff */
#endif

#ifdef T_HAS_STDARG_H
#include <stdarg.h> /* no more va_dcl!!! */

/* ------------------------------------------------------------------------- */
/* Keep OpenWindows 2.0 (specifically $OPENWINHOME/include/xview/pkg.h) from */
/* including <varargs.h>.                                                    */
/* ------------------------------------------------------------------------- */
#ifdef T_OS_SUNOS
#define _sys_varargs_h
#endif

#else
/* -------------------------------------------------------------- */
/* If we don't have <stdarg.h>, then just include <varargs.h>.  I */
/* have removed our own non-portable stdarg.h.  SMW 02/22/94      */
/* -------------------------------------------------------------- */
#include <varargs.h>
/*#include <rtworks/stdarg.h>*/ /* no more va_dcl!!! */
#endif /* T_HAS_STDARG_H */

/* ----------------------------------------------------------------------- */
/* If we are generating function prototypes on SunOS 4, then undef the K&R */
/* version of the va_start macro (our source code uses the ANSI version).  */
/* SMW 04/26/94                                                            */
/* ----------------------------------------------------------------------- */
#if defined(T_GENERATE_PROTOTYPES) && defined(va_start)
#undef va_start
#endif

#include <math.h> /* floating-point math routines */

#ifndef T_ANSI_C
#include <malloc.h> /* external function declarations */
#endif

#ifdef T_ANSI_C
#include <limits.h>
#include <float.h>
#else
#include <values.h> /* MAXDOUBLE */
#endif

#include <ctype.h> /* toupper(), tolower() */
#include <signal.h> /* SIGPIPE, etc. (signals are a joke...) */
#include <setjmp.h> /* for jmp_buf (longjmp is an awesome function!) */

/* --------------------------------------------------------------- */
/* Unix systems do not seem to define TRUE and FALSE in convenient */
/* places.  These are needed just to get off the ground.           */
/* --------------------------------------------------------------- */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0  /* should this be OL or (void *)0 instead ? */
#endif

/* --------------------------------------------------------------------- */
/* As we port to more non-UNIX-like operating systems, we cannot rely on */
/* the "normal" C run-time library being the same.  We therefore define  */
/* our own macros for the C functions we need.  SMW 4/15/93              */
/*                                                                       */
/* This kinda clashes with our T_{M,C,RE}ALLOC macros, but it's too late */
/* to change those now.  SMW 4/23/93                                     */
/* --------------------------------------------------------------------- */
#ifdef T_OS_SVR4
#define T_C_GETTIMEOFDAY(tv) gettimeofday(tv)
#endif
/* -------------------------------------------------------------------- */
/* T_MEMMOVE is kinda tricky because the SunOS 4.1.X bundled C compiler */
/* does not have memmove, therefore Talarian internal code cannot       */
/* use memmove on SunOS 4.1.X.  SMW 4/30/93                             */
/* -------------------------------------------------------------------- */
#ifdef T_OS_SUNOS
#ifdef T_OS_SOLARIS2
#else
#define T_C_MEMMOVE(dest, src, size) bcopy((void *)(src),(void *)(dest),(size))
#endif
#endif
/* ------------------------------------------------------------------ */
/* Missing prototypes for system calls and RTL functions.             */
/* Yuck.  SMW 8/6/93                                                  */
/* These prototypes cause too many problems for our customers, which  */
/* have a wide variety of compilers.  They are useful for Talarian,   */
/* which has a specific compiler configuration, though.  SMW 08/25/94 */
/* ------------------------------------------------------------------ */
#if defined(T_RTWORKS_SRC) && !defined(T_HIDE_MISSING_PROTO)

/* ---------------------------------- */
/* Prototypes missing from Sun C 1.1. */
/* ---------------------------------- */
#if defined(T_SUN4_SUNOS) && defined(T_ANSI_C)
int _flsbuf(int /*x*/, FILE * /*p*/); /* used by putc */
int _filbuf(FILE * /*p*/); /* used by getc */
void bcopy(char * /*b1*/, char * /*b2*/, int length);
void bzero(char * /*b*/, int /*length*/);
int fsync(int /*fd*/);
/* Use long, not int, for Sun C++ (bug 2626).  SMW 04/21/94 */
long gethostid(void);
int gethostname(char * /*name*/, int /*namelen*/);
int gettimeofday(struct timeval * /*tp*/, struct timezone * /*tzp*/);
int strftime(char * /*buf*/, int /*bufsize*/, char * /*fmt*/, struct tm * /*tm*/);
int ioctl(int /*fd*/, int /*request*/, caddr_t /*arg*/);
int lstat(const char * /*path*/, struct stat * /*buf*/);
int on_exit(void (* /*procp*/)(int /*status*/, caddr_t /*arg*/),
	    caddr_t /*arg*/);
int readlink(const char * /*path*/, char * /*buf*/, int /*bufsiz*/);
caddr_t sbrk(int /*incr*/);
int sigvec(int /*sig*/, struct sigvec * /*vec*/, struct sigvec * /*ovec*/);
long strtol(char * /*str*/, char** /*ptr*/, int /*base*/);

/* -------------------------------------------------- */
/* Prototypes not needed for GCC 2.4.5.  SMW 05/11/94 */
/* -------------------------------------------------- */
#ifndef __GNUC__
FILE *fdopen(int /*fd*/, const char * /*mode*/); /* acc -Xc hides this. */
#else
/* ------------------------------------------------- */
/* Additional prototypes for GCC 2.4.5.  SMW 8/11/93 */
/* ------------------------------------------------- */
int fclose(FILE * /*stream*/);
int fflush(FILE * /*stream*/);
int fgetc(FILE * /*stream*/);
int fprintf(FILE * /*strm*/, const char * /*format*/, ...);
int fputc(int /*c*/, FILE * /*stream*/);
int fputs(const char * /*s*/, FILE * /*stream*/);
size_t fread(void * /*ptr*/, size_t /*size*/, size_t /*nitems*/, FILE * /*stream*/);
int fscanf(FILE * /*stream*/, const char * /*format*/, ...);
int fseek(FILE * /*stream*/, long /*offset*/, int /*ptrname*/);
size_t fwrite(const void * /*ptr*/, size_t /*size*/, size_t /*nitems*/,
	      FILE * /*stream*/);
 void *memset(void * /*s*/, int /*c*/, size_t /*n*/);
time_t mktime(struct tm * /*timeptr*/);
int pclose(FILE * /*stream*/);
int printf(const char * /*format*/, ... );
int putenv(char * /* string */);
int rename(const char * /*old*/, const char * /*new*/);
void rewind(FILE *stream);
void setbuf(FILE *, char *);
int sscanf(const char * /*s*/, const char * /*format*/, ...);
int system(const char * /*string*/);
time_t time(time_t * /*tloc*/);
int vfork(void);
int vfprintf(FILE * /*stream*/, const char * /*format*/, va_list /*ap*/);
int vprintf(const char * /*format*/, va_list /*ap*/);
int vsprintf(char * /*s*/, const char * /*format*/, va_list /*ap*/);
#endif /* __GNUC__ */

#endif

/* ---------------------------------- */
/* Prototypes missing from SunOS 5.x. */
/* ---------------------------------- */
#if defined(T_OS_SOLARIS2) && defined(T_ANSI_C) && !defined(BSD) && !defined(T_I86_SOLARIS)
/* ------------------------------------------------------------------- */
/* Gettimeofday is a little tricky, since the BSD version takes two    */
/* arguments, and the SVR4 version takes one argument.  Sun wimped out */
/* and left out the prototype!  SMW 8/19/93                            */
/* ------------------------------------------------------------------- */
int gettimeofday(struct timeval * /*tp*/);
#if 0
  removed because Solaris 2.7 broke this proto  JAM 4/6/99
int putenv(const char * /* string */);
#endif
/* ----------------------------------------------------------------------- */
/* We need to use "cc -Xc" to get code that uses X11R5 to compile cleanly, */
/* but this hides POSIX stuff like fileno and kill.  Enabling POSIX        */
/* mode hides things like sockets, which we also need.  We just can't      */
/* win with Solaris 2!  SMW 03/16/94                                       */
/*                                                                         */
/* Solaris 2.4 has __EXTENSIONS__, which we can use to get POSIX signals   */
/* enabled (I was having problems with signals being delivered very        */
/* slowly to non MT-illegal libraries).  I also tried using                */
/* -D_POSIX_SOURCE, but that turns off things like timeval in sys/time.h!  */
/* SMW 03/12/96                                                            */
/* ----------------------------------------------------------------------- */
#if __STDC__ == 1 && !defined(__cplusplus) /*&& !defined(__EXTENSIONS__)*/
FILE *fdopen(int /*fd*/, const char * /*mode*/);
#ifndef fileno /* needed for gcc in src/imake.  SMW 06/24/96 */
int fileno(FILE *);
#endif
int kill(pid_t, int);
#endif
#endif

/* ---------------------------------- */
/* Prototypes missing from AIX 3.2.5. */
/* Fixed in AIX 4.1.3.  SMW 05/09/96  */
/* ---------------------------------- */
#if defined(T_RS6000_AIX3) && defined(T_ANSI_C)
char *sbrk(int /*incr*/);
#endif

/* --------------------------------- */
/* Prototypes missing from IRIX 5.3. */
/* --------------------------------- */
#if defined(T_IRIS_IRIX) && defined(T_ANSI_C)
unsigned sysid(unsigned char /*id*/[16]);
#endif

#if defined(T_I86_SOLARIS)
int putenv(char * /* string */);
#endif

#endif /* T_RTWORKS_SRC && !T_HIDE_MISSING_PROTO */

#endif  /* _RTWORKS_UNIX_INC_H_ */
