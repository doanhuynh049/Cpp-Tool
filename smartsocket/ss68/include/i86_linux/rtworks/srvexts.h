/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tre/dev/68/src/rtworks/srvexts.h#1 $
 */

/*
 * Server extensions backward compatibility file
 */
 
#ifndef _RTWORKS_SERVER_EXTENSIONS_H_
#define _RTWORKS_SERVER_EXTENSIONS_H_

#include <rtworks/smr.h>

/*
 * Extension entry points
 */ 
#define T_SRV_EXT_LIBRARY_OPEN_FUNC   T_SMR_EXT_LIBRARY_OPEN_FUNC  /* (TsrvExtOpenFunc)   */
#define T_SRV_EXT_LIBRARY_CLOSE_FUNC  T_SMR_EXT_LIBRARY_CLOSE_FUNC /* (TsrvExtCloseFunc)  */

/*
 * Interface IDs 
 */
typedef enum {
  T_SRV_IID_XF_FCTRY_ID         = T_SMR_IID_XF_FCTRY_ID,        /* Interface factory interface id   */
  T_SRV_IID_CLIENT_FCTRY_XF_ID  = T_SMR_IID_CLIENT_FCTRY_XF_ID, /* Client factory interface id      */
  T_SRV_IID_CLIENT_XF_ID        = T_SMR_IID_CLIENT_XF_ID        /* Client interface id              */
} T_SRV_INTERFACE_ID;

/*
 * Msg direction during in filter function call
 */
typedef enum {
  T_SRV_MFD_IN  = T_SMR_MFD_IN,   /* Message received from client    */
  T_SRV_MFD_OUT = T_SMR_MFD_OUT   /* Message being sent to client    */
} T_SRV_MSG_FILTER_DIRECTION;
 
/*
 * Opaque types.
 */
typedef struct T_SMR_CLIENT_STRUCT
               T_SRV_CLIENT_STRUCT,
              *T_SRV_CLIENT;
typedef struct T_SMR_CLIENT_CREATE_CB_DATA_STRUCT
               T_SRV_CLIENT_CREATE_CB_DATA_STRUCT,
              *T_SRV_CLIENT_CREATE_CB_DATA;
typedef struct T_SMR_CLIENT_DESTROY_CB_DATA_STRUCT
               T_SRV_CLIENT_DESTROY_CB_DATA_STRUCT,
              *T_SRV_CLIENT_DESTROY_CB_DATA;
typedef struct T_SMR_MSG_DISPATCH_DATA_STRUCT
               T_SRV_MSG_DISPATCH_DATA_STRUCT, 
              *T_SRV_MSG_DISPATCH_DATA;
typedef struct T_SMR_MSG_FILTER_DATA_STRUCT
               T_SRV_MSG_FILTER_DATA_STRUCT,
              *T_SRV_MSG_FILTER_DATA;
typedef struct T_SMR_CLIENT_FCTRY_XF_STRUCT
               T_SRV_CLIENT_FCTRY_XF_STRUCT, 
              *T_SRV_CLIENT_FCTRY_XF;
typedef struct T_SMR_CLIENT_XF_STRUCT
               T_SRV_CLIENT_XF_STRUCT, 
              *T_SRV_CLIENT_XF;
typedef struct T_SMR_XFFCTRY_XF_STRUCT
               T_SRV_XFFCTRY_XF_STRUCT, 
              *T_SRV_XFFCTRY_XF;
typedef struct T_SMR_EXT_LIBRARY_INFO_STRUCT
               T_SRV_EXT_LIBRARY_INFO_STRUCT, 
              *T_SRV_EXT_LIBRARY_INFO;

/*
 * Callback function definitions
 */
typedef T_SMR_CLIENT_CREATE_CB_FUNC           T_SRV_CLIENT_CREATE_CB_FUNC;
typedef T_SMR_CLIENT_TRAV_FUNC                T_SRV_CLIENT_TRAV_FUNC;
typedef T_SMR_CLIENT_DESTROY_CB_FUNC          T_SRV_CLIENT_DESTROY_CB_FUNC;
typedef T_SMR_MSG_DISPATCH_FUNC               T_SRV_MSG_DISPATCH_FUNC;
typedef T_SMR_MSG_FILTER_FUNC                 T_SRV_MSG_FILTER_FUNC;

/*
 * Client factory interface method definitions
 */
typedef T_SMR_CL_FCTRY_CREATE_CB_FUNC         T_SRV_CL_FCTRY_CREATE_CB_FUNC;
typedef T_SMR_CL_FCTRY_CREATE_CB_LOOKUP_FUNC  T_SRV_CL_FCTRY_CREATE_CB_LOOKUP_FUNC;
typedef T_SMR_CL_FCTRY_TRAVERSE_FUNC          T_SRV_CL_FCTRY_TRAVERSE_FUNC;

/*
 * Client interface method definitions
 */
typedef T_SMR_CL_XF_DESTROY_CB_CREATE_FUNC    T_SRV_CL_XF_DESTROY_CB_CREATE_FUNC;
typedef T_SMR_CL_XF_DESTROY_CB_LOOKUP_FUNC    T_SRV_CL_XF_DESTROY_CB_LOOKUP_FUNC;
typedef T_SMR_CL_XF_FILTER_CREATE_FUNC        T_SRV_CL_XF_FILTER_CREATE_FUNC;
typedef T_SMR_CL_XF_DISPATCH_CREATE_FUNC      T_SRV_CL_XF_DISPATCH_CREATE_FUNC;
typedef T_SMR_CL_XF_GET_NAME_FUNC             T_SRV_CL_XF_GET_NAME_FUNC;
typedef T_SMR_CL_XF_GET_DISCONNECT_MODE_FUNC  T_SRV_CL_XF_GET_DISCONNECT_MODE_FUNC;
typedef T_SMR_CL_XF_GET_IDENT_FUNC            T_SRV_CL_XF_GET_IDENT_FUNC;
typedef T_SMR_CL_XF_GET_PROJECT_FUNC          T_SRV_CL_XF_GET_PROJECT_FUNC;
typedef T_SMR_CL_XF_SEND_FUNC                 T_SRV_CL_XF_SEND_FUNC;
typedef T_SMR_CL_XF_DESTROY_FUNC              T_SRV_CL_XF_DESTROY_FUNC;
typedef T_SMR_CL_XF_PRINT_FUNC                T_SRV_CL_XF_PRINT_FUNC;

/*
 * Interface factory interface method definitions
 */
typedef T_SMR_CREATE_INTERFACE_FUNC           T_SRV_CREATE_INTERFACE_FUNC;
typedef T_SMR_DESTROY_INTERFACE_FUNC          T_SRV_DESTROY_INTERFACE_FUNC;

/*
 * Server extension library exports definitions
 */
typedef T_SMR_EXT_OPEN_FUNC                   T_SRV_EXT_OPEN_FUNC;
typedef T_SMR_EXT_CLOSE_FUNC                  T_SRV_EXT_CLOSE_FUNC;

#endif /* _RTWORKS_SERVER_EXTENSIONS_H_ */

