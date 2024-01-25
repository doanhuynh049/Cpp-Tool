/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tsi/sm/dev/68/src/rtworks/sm.h#1 $
 */

#ifndef _RTWORKS_SM_H_
#define _RTWORKS_SM_H_

#include <rtworks/edm.h>

/*
 * Error codes
 */
#define T_ERR_SM_VERSION_INVALID            (T_ERR_SM_START + 0)
#define T_ERRNM_SM_VERSION_INVALID          "Incompatible driver version"
#define T_ERR_SM_NOINIT                     (T_ERR_SM_START + 1)
#define T_ERRNM_SM_NOINIT                   "SM module not initialized"
#define T_ERR_SM_NOOPEN                     (T_ERR_SM_START + 2)
#define T_ERRNM_SM_NOOPEN                   "SM not opened"
#define T_ERR_SM_NOCHALLENGE                (T_ERR_SM_START + 3)
#define T_ERRNM_SM_NOCHALLENGE              "SM responds with a challenge"
#define T_ERR_SM_CHALLENGE_NOT_ACCEPT       (T_ERR_SM_START + 4)
#define T_ERRNM_SM_CHALLENGE_NOT_ACCEPT     "SM did not accept the module challenge"
#define T_ERR_SM_DRIVER_INVALID             (T_ERR_SM_START + 5)
#define T_ERRNM_SM_DRIVER_INVALID           "Invalid security driver module"
#define T_ERR_SM_DRIVER_CHALLENGE_FAILED    (T_ERR_SM_START + 6)
#define T_ERRNM_SM_DRIVER_CHALLENGE_FAILED  "Security driver module challenge failed"
#define T_ERR_SM_DRIVER_DOESNT_EXIST        (T_ERR_SM_START + 7)
#define T_ERRNM_SM_DRIVER_DOESNT_EXIST      "Security driver module does not exist"
#define T_ERR_SM_DRIVER_REGISTER_FAILED     (T_ERR_SM_START + 8)
#define T_ERRNM_SM_DRIVER_REGISTER_FAILED   "Security driver failed to register"
#define T_ERR_SM_NO_BOUND_DEVICE            (T_ERR_SM_START + 9)
#define T_ERRNM_SM_NO_BOUND_DEVICE          "No bound device"

/*
 * Message types
 */

/*
 * SM trace bindings
 */
#define T_SM_TRACE_LEVEL_OPTION            "_trace_level_sm"
#define T_SM_TRACE_FILE_OPTION             "_trace_file_sm"

/*
 * Opaque data types.
 */
typedef struct T_SM_DRIVER_DEVICE_STRUCT T_SM_DRIVER_DEVICE_STRUCT, 
                                        *T_SM_DRIVER_DEVICE;
typedef struct T_SM_DRIVER_STRUCT        T_SM_DRIVER_STRUCT, 
                                        *T_SM_DRIVER;
typedef struct T_SM_STRUCT               T_SM_STRUCT, 
                                        *T_SM;

/*
 * Transparent data types
 */
/*
 * T_SM_CREDENTIALS_STRUCT defines the structure of authentication credentials
 */ 
struct T_SM_CREDENTIALS_STRUCT {
  T_INT4 api;              /* Authentication Policy Identifier   */
  T_PTR  data;             /* Policy specific credential data    */
  T_INT4 data_len;         /* Length of policy specific data     */
};
typedef struct T_SM_CREDENTIALS_STRUCT T_SM_CREDENTIALS_STRUCT, 
                                      *T_SM_CREDENTIALS;

/*
 * The following define the allotted credential APIs. Any new credential 
 * type in the system needs a new API and should be noted here to avoid 
 * collisions
 */

/*
 * No credentials
 */
#define T_SM_CREDENTIALS_API_NONE     0
/*
 * Barclays Credentials API
 */
#define T_SM_CREDENTIALS_API_BARCLAYS 1
/*
 * Basic Credentials API
 */
#define T_SM_CREDENTIALS_API_BASIC    2

/*
 * T_SM_RESOURCE_ID uniquely identifies authorizable resources
 *
 * The format of the enumeration is as follows
 *
 *  typedef enum {
 *    T_SMRID_INVALID = -1,
 *    T_SMRID_<RESOURCE1>,
 *    T_SMRID_<RESOURCE2>,
 *    .
 *    .
 *    .
 *    T_SMRID_<RESOURCEN>,
 *  } T_SM_RESOURCE_ID;
 *
 */
typedef enum {
  T_SMRID_INVALID  = -1,    /* Invalid ID                         */
  T_SMRID_SUBJECT  =  0,    /* Subject resource ID                */
  T_SMRID_CONNECT  =  1,    /* Connection resource ID             */
  T_SMRID_MSGTYPE  =  2     /* Message type resource ID           */
} T_SM_RESOURCE_ID;

/*
 * Each resource has its own structure of data used in the authorization 
 * process e.g. T_SMRID_SUBJECT has a different data structure as opposed
 * to, say, T_SMRID_PROJECT. Each of the following defines the structure
 * of the data for a different resource
 */
struct T_SM_RESOURCE_DATA_INVALID_STRUCT {
  T_INT4  data;
};
typedef struct T_SM_RESOURCE_DATA_INVALID_STRUCT 
               T_SM_RESOURCE_DATA_INVALID_STRUCT, 
              *T_SM_RESOURCE_DATA_INVALID;

struct T_SM_RESOURCE_DATA_SUBJECT_STRUCT {
  T_STR name;
  T_STR host;
};
typedef struct T_SM_RESOURCE_DATA_SUBJECT_STRUCT 
               T_SM_RESOURCE_DATA_SUBJECT_STRUCT, 
              *T_SM_RESOURCE_DATA_SUBJECT;

struct T_SM_RESOURCE_DATA_CONNECT_STRUCT {
  T_STR name;
  T_STR host;
};
typedef struct T_SM_RESOURCE_DATA_CONNECT_STRUCT 
               T_SM_RESOURCE_DATA_CONNECT_STRUCT, 
              *T_SM_RESOURCE_DATA_CONNECT;

struct T_SM_RESOURCE_DATA_MSGTYPE_STRUCT {
  T_INT4 num;
  T_STR  host;
};
typedef struct T_SM_RESOURCE_DATA_MSGTYPE_STRUCT 
               T_SM_RESOURCE_DATA_MSGTYPE_STRUCT, 
              *T_SM_RESOURCE_DATA_MSGTYPE;

/*
 * T_SM_RESOURCE_STRUCT defines the structure of an authorizable resource
 * 
 * The format of the resource structure is as follows
 *
 *  struct T_SM_RESOURCE_STRUCT {
 *    T_SM_RESOURCE_ID id;
 *    union {
 *      T_SM_RESOURCE_DATA_INVALID_STRUCT     invalid;
 *      T_SM_RESOURCE_DATA_<RESOURCE1>_STRUCT <resource1>;
 *      T_SM_RESOURCE_DATA_<RESOURCE2>_STRUCT <resource2>;
 *      .
 *      .
 *      .
 *      T_SM_RESOURCE_DATA_<RESOURCE1>_STRUCT <resourceN>;
 *    } data;
 *  }
 *
 */
struct T_SM_RESOURCE_STRUCT {
  T_SM_RESOURCE_ID id;     /* Unique identifier of the resource  */
  union {
    T_SM_RESOURCE_DATA_INVALID_STRUCT invalid;
    T_SM_RESOURCE_DATA_SUBJECT_STRUCT subject;
    T_SM_RESOURCE_DATA_CONNECT_STRUCT connect;
    T_SM_RESOURCE_DATA_MSGTYPE_STRUCT msgtype;
  } data;
};

typedef struct T_SM_RESOURCE_STRUCT 
               T_SM_RESOURCE_STRUCT, 
              *T_SM_RESOURCE;

/*
 * The following defines pertain to access types. Each resource type is assigned
 * a range of 100 numbers to define the various actions that can be performed 
 * on that resource. (RAID = Resource Access ID>
 *
 * The following depicts the allocation strategy
 *
 *  #define T_SM_RAID_<RESOURCE1>_<ACCESS1>    1
 *  #define T_SM_RAID_<RESOURCE1>_<ACCESS2>    2
 *  .
 *  .
 *  .
 *  #define T_SM_RAID_<RESOURCE1>_<ACCESSM>    1 + M (M < 100)
 *  #define T_SM_RAID_<RESOURCE2>_<ACCESS1>    101
 *  #define T_SM_RAID_<RESOURCE2>_<ACCESS2>    102
 *  .
 *  .
 *  .
 *  #define T_SM_RAID_<RESOURCE2>_<ACCESSM>    101 + M (M < 100)
 *  .
 *  .
 *  .
 *  #define T_SM_RAID_<RESOURCEN>_<ACCESS1>    100*(N-1) + 1
 *  #define T_SM_RAID_<RESOURCEN>_<ACCESS2>    100*(N-1) + 2
 *  .
 *  .
 *  .
 *  #define T_SM_RAID_<RESOURCEN>_<ACCESSM>    100*(N-1) + M (M < 100)
 *  
 */
#define T_SM_RAID_INVALID               0
#define T_SM_RAID_SUBJECT_PUBLISH       1
#define T_SM_RAID_SUBJECT_SUBSCRIBE     2
#define T_SM_RAID_CONNECT_SERVER        101
#define T_SM_RAID_CONNECT_CLIENT        102
#define T_SM_RAID_CONNECT_GROUP         103
#define T_SM_RAID_MSGTYPE_PUBLISH       201
#define T_SM_RAID_MSGTYPE_SUBSCRIBE     202

/*
 * Async request completion function and data definitions
 */
typedef struct T_SM_AUTHENTICATE_COMPLETION_DATA_STRUCT 
               T_SM_AUTHENTICATE_COMPLETION_DATA_STRUCT , 
              *T_SM_AUTHENTICATE_COMPLETION_DATA;

typedef struct T_SM_AUTHORIZE_COMPLETION_DATA_STRUCT 
               T_SM_AUTHORIZE_COMPLETION_DATA_STRUCT , 
              *T_SM_AUTHORIZE_COMPLETION_DATA;

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SM_AUTHENTICATE_COMPLETION_FUNC)
T_TYPEDEF((T_SM, T_SM_AUTHENTICATE_COMPLETION_DATA, T_PTR));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SM_AUTHORIZE_COMPLETION_FUNC)
T_TYPEDEF((T_SM, T_SM_AUTHORIZE_COMPLETION_DATA, T_PTR));

struct T_SM_AUTHENTICATE_COMPLETION_DATA_STRUCT {
  T_BOOL           status;
  T_INT4           result_code;
  T_EDM            edm;
  T_SM_CREDENTIALS credentials;
  T_PTR            reserved1;
  T_PTR            reserved2;
};

struct T_SM_AUTHORIZE_COMPLETION_DATA_STRUCT {
  T_BOOL              status;
  T_INT4              result_code;
  T_EDM               edm;
  T_INT4              num_resources;
  T_SM_RESOURCE      *resource_arr;
  T_INT4             *raid_arr;
  T_SM_CREDENTIALS    credentials;
  T_INT4             *result_arr;
  T_PTR               reserved1;
  T_PTR               reserved2;
};

/*
 * Security Driver interface versions
 */
#define T_SM_DRIVER_VERSION       T_SM_DRIVER_VERSION_START
#define T_SM_DRIVER_VERSION_START 1

/*
 * Name and definition of Security Driver initialization function entry point
 */
#define T_SM_DRIVER_INIT_FUNC_NAME  "TsdInit"
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SM_DRIVER_INIT_FUNC)
T_TYPEDEF((T_INT4, T_PTR, T_INT4, T_PTR, T_INT4 *));

/*
 * Definitions for the entry points into a Security Driver
 */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SM_DRIVER_XF_CREATE_FUNC)
T_TYPEDEF((T_PTR *));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SM_DRIVER_XF_AUTHENTICATE_FUNC)
T_TYPEDEF((T_PTR, T_SM_CREDENTIALS));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SM_DRIVER_XF_AUTHORIZE_FUNC)
T_TYPEDEF((T_PTR, T_INT4, T_SM_RESOURCE *, T_INT4 *, T_SM_CREDENTIALS, T_INT4 *));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SM_DRIVER_XF_DESTROY_FUNC)
T_TYPEDEF((T_PTR *));

#include <rtworks/smpr.h>

#endif /* _RTWORKS_SM_H_ */


