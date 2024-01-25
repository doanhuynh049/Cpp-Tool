/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tsi/security_drivers/basic/dev/68/src/rtworks/sdbasic.h#1 $
 */

#ifndef _RTWORKS_SD_BASIC_H_
#define _RTWORKS_SD_BASIC_H_

/*
 * Basic Credentials. 
 */
typedef struct T_SD_BASIC_CREDENTIALS_STRUCT {
  T_CHAR  username[64];
  T_UINT1 encoded_password[20];
} T_SD_BASIC_CREDENTIALS_STRUCT, *T_SD_BASIC_CREDENTIALS;

/*
 * ACL data type.
 */
typedef struct T_SD_ACL_STRUCT
               T_SD_ACL_STRUCT,
              *T_SD_ACL;

/*
 * ACL permission types.
 */
typedef enum {
  T_SD_ACL_PERMISSION_TYPE_INVALID    = -1,
  T_SD_ACL_PERMISSION_TYPE_SERVER     =  0,
  T_SD_ACL_PERMISSION_TYPE_CLIENT     =  1,
  T_SD_ACL_PERMISSION_TYPE_MEMBERSHIP =  2,
  T_SD_ACL_PERMISSION_TYPE_PUBLISH    =  3,
  T_SD_ACL_PERMISSION_TYPE_SUBSCRIBE  =  4,
  T_SD_ACL_PERMISSION_TYPE_COUNT      =  5  /* must always be last */
} T_SD_ACL_PERMISSION_TYPE;

#include <rtworks/sdbasipr.h>

#endif /* _RTWORKS_SD_BASIC_H_ */
