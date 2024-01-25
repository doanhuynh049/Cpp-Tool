/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/utcat.h#1 $
 */

#ifndef _RTWORKS_TUTCAT_H_
#define _RTWORKS_TUTCAT_H_

/*
 * Opaque data types
 */
typedef struct T_CAT_STRUCT T_CAT_STRUCT, *T_CAT;
typedef struct T_CATF_STRUCT T_CATF_STRUCT, *T_CATF;
typedef struct T_CAT_HASH_MSG_STRUCT T_CAT_HASH_MSG_STRUCT, *T_CAT_HASH_MSG;

/*
 * Settable flags within a catalog object.
 */
#define T_CAT_FLAG_ID                            0x01

/*
 * Default trace bindings
 */
#define T_TRACE_FILENAME_OPTION                  "cat_file"

typedef enum {
  T_CAT_SEVERITY_ALWAYS = 0,
  T_CAT_SEVERITY_NEVER,
  T_CAT_SEVERITY_ERR,
  T_CAT_SEVERITY_WARN,
  T_CAT_SEVERITY_INFO,
  T_CAT_SEVERITY_VERBOSE,
  T_CAT_SEVERITY_DEBUG
} T_CAT_SEVERITY;

#endif /* _RTWORKS_TUTCAT_H_ */
