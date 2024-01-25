/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tiof/siof/dev/68/src/rtworks/sioc.h#1 $
 */

#ifndef _RTWORKS_SIOC_H_
#define _RTWORKS_SIOC_H_

#include <rtworks/edm.h>
#include <rtworks/msg.h>

/*
 * SIOC trace bindings
 */
#define T_SIOC_TRACE_LEVEL_OPTION      "_trace_level_sioc"
#define T_SIOC_TRACE_FILE_OPTION       "_trace_file_sioc"

/*
 * Opaque data types.
 */
typedef struct T_SIOC_MSG_IQ_STRUCT     T_SIOC_MSG_IQ_STRUCT,
                                       *T_SIOC_MSG_IQ;
typedef struct T_SIOC_MSG_OQ_STRUCT     T_SIOC_MSG_OQ_STRUCT,
                                       *T_SIOC_MSG_OQ;
typedef struct T_SIOC_IOCTL_STRUCT      T_SIOC_IOCTL_STRUCT,
                                       *T_SIOC_IOCTL;
   
/*
 * Traversal function types.
 */
typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_SIOC_MSG_IQ_TRAV_FUNC)
T_TYPEDEF((T_SIOC_MSG_IQ,
           T_MSG, 
           T_PTR));

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_SIOC_MSG_IQ_OQ_TRAV_FUNC)
T_TYPEDEF((T_SIOC_MSG_IQ,
           T_SIOC_MSG_OQ, 
           T_PTR));

typedef T_PTR (T_ENTRY1 * T_ENTRY2 T_SIOC_MSG_OQ_TRAV_FUNC)
T_TYPEDEF((T_SIOC_MSG_OQ,
           T_MSG, 
           T_PTR));

#include <rtworks/siocpr.h>

#endif /* _RTWORKS_SIOC_H_ */


