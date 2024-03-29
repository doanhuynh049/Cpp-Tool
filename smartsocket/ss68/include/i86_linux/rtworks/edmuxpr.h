/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tiof/biof/dev/68/src/rtworks/edmuxpr.h#1 $
 */

/* This file is generated by cproto - do not edit! */

/* ../rtworks/edmuxpr.h -- generated prototypes */

#ifndef T_EDM_API
# ifndef T_OS_WIN32
# define T_EDM_API
# else
# ifndef T_STATIC_LINK
# ifndef T_EDM_SOURCE
# if !defined T_GENERATE_PROTOTYPES
# define T_EDM_API __declspec(dllimport)
# else
# define T_EDM_API
# endif
# else
# if !defined T_GENERATE_PROTOTYPES
# define T_EDM_API __declspec(dllexport)
# else
# define T_EDM_API
# endif
# endif
# else
# define T_EDM_API
# endif /* T_STATIC_LINK */
# endif /* T_OS_WIN32 */
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* edmsel.c */
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmSelectIsObjectFDReady T_PROTO((
T_EDM_OBJECT object,
fd_set * ready_read_set,
fd_set * ready_write_set,
fd_set * ready_except_set
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmSelectCreate T_PROTO((
T_EDM edm
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmSelectBlock T_PROTO((
T_EDM edm,
T_REAL8 timeout,
T_UINT4 * flags
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmSelectWakeUp T_PROTO((
T_EDM edm
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmSelectEnable T_PROTO((
T_EDM edm,
T_EDM_OBJECT object,
T_BOOL enable
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmSelectIsObjectReady T_PROTO((
T_EDM edm,
T_EDM_OBJECT object
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmSelectOnObjectDispatch T_PROTO((
T_EDM edm,
T_EDM_OBJECT object
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmSelectPrint T_PROTO((
T_EDM edm,
T_OUT_FUNC out_func
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmSelectDestroy T_PROTO((
T_EDM edm
));

/* edmpoll.c */
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmPollCreate T_PROTO((
T_EDM edm
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmPollBlock T_PROTO((
T_EDM edm,
T_REAL8 timeout,
T_UINT4 * flags
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmPollWakeUp T_PROTO((
T_EDM edm
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmPollEnable T_PROTO((
T_EDM edm,
T_EDM_OBJECT object,
T_BOOL enable
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmPollIsObjectReady T_PROTO((
T_EDM edm,
T_EDM_OBJECT object
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmPollOnObjectDispatch T_PROTO((
T_EDM edm,
T_EDM_OBJECT object
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmPollPrint T_PROTO((
T_EDM edm,
T_OUT_FUNC out_func
));
T_EDM_API T_BOOL T_ENTRY T_EXPORT TedmPollDestroy T_PROTO((
T_EDM edm
));

#ifdef __cplusplus
}
#endif
