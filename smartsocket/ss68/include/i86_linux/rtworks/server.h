/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tdf/server/dev/68/src/rtworks/server.h#4 $
 */

#ifndef _RTWORKS_SERVER_H_
#define _RTWORKS_SERVER_H_

#include <rtworks/ioctl.h>
#include <rtworks/sioc.h>

/*
 * Forward declarations
 */
typedef struct T_SRV_LM_XF_STRUCT 
               T_SRV_LM_XF_STRUCT, 
              *T_SRV_LM_XF;
typedef struct T_SRV_IOP_XF_STRUCT 
               T_SRV_IOP_XF_STRUCT, 
              *T_SRV_IOP_XF;
typedef struct T_SRV_IOPM_XF_STRUCT
               T_SRV_IOPM_XF_STRUCT,
              *T_SRV_IOPM_XF;
typedef struct T_SRV_RE_XF_STRUCT
               T_SRV_RE_XF_STRUCT,
              *T_SRV_RE_XF;
typedef struct T_SRV_REFCOUNT_XF_STRUCT 
               T_SRV_REFCOUNT_XF_STRUCT, 
              *T_SRV_REFCOUNT_XF;
typedef struct T_SRV_CLIENT_INFO_STRUCT 
               T_SRV_CLIENT_INFO_STRUCT, 
              *T_SRV_CLIENT_INFO;
typedef struct T_SRV_CONN_NAMES_STRUCT    
               T_SRV_CONN_NAMES_STRUCT,
              *T_SRV_CONN_NAMES;
typedef struct T_SRV_SERVER_INFO_STRUCT 
               T_SRV_SERVER_INFO_STRUCT, 
              *T_SRV_SERVER_INFO;
typedef struct T_SRV_GROUP_INFO_STRUCT 
               T_SRV_GROUP_INFO_STRUCT, 
              *T_SRV_GROUP_INFO;
typedef struct T_SRV_PEER_INFO_STRUCT 
               T_SRV_PEER_INFO_STRUCT, 
              *T_SRV_PEER_INFO;
typedef struct T_SRV_CHANNEL_INFO_STRUCT 
               T_SRV_CHANNEL_INFO_STRUCT, 
              *T_SRV_CHANNEL_INFO;


/*
 * Generic reference counting methods
 */
typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_ADDREF_FUNC)
T_TYPEDEF((T_PTR));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_RELEASE_FUNC)
T_TYPEDEF((T_PTR *));

/*
 * The License Manager interface methods
 */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_LM_XF_GET_SERVER_COUNT_FUNC)
T_TYPEDEF((T_PTR, T_INT4 *));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_LM_XF_GET_CLIENT_COUNT_FUNC)
T_TYPEDEF((T_PTR, T_INT4 *));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_LM_XF_IS_SERVER_ALLOWED_FUNC)
T_TYPEDEF((T_PTR, T_INT4));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_LM_XF_IS_CLIENT_ALLOWED_FUNC)
T_TYPEDEF((T_PTR, T_INT4));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_LM_XF_IS_ADVANCED_FUNC)
T_TYPEDEF((T_PTR));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_LM_XF_IS_MULTIPROC_FUNC)
T_TYPEDEF((T_PTR));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_LM_XF_IS_GROUP_FUNC)
T_TYPEDEF((T_PTR));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_LM_XF_IS_LOCAL_FUNC)
T_TYPEDEF((T_PTR));


/*
 * The I/O PORT interface methods
 */
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_IOP_XF_SET_USER_PROP_FUNC)
T_TYPEDEF((T_PTR, T_PTR));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_IOP_XF_GET_USER_PROP_FUNC)
T_TYPEDEF((T_PTR, T_PTR *));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_IOP_XF_MSG_WRITE_FUNC)
T_TYPEDEF((T_PTR, T_MSG));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_IOP_XF_MSGQ_WRITE_FUNC)
T_TYPEDEF((T_PTR, T_SIOC_MSG_OQ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_IOP_XF_IOCTL_FUNC)
T_TYPEDEF((T_PTR, T_SIOC_IOCTL));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_IOP_XF_CLOSE_FUNC)
T_TYPEDEF((T_PTR));

/*
 * The I/O PORT Manager interface methods. 
 */        
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_IOPM_XF_ON_PORT_OPEN_FUNC)
T_TYPEDEF((T_PTR, T_SRV_CHANNEL_INFO, T_SRV_IOP_XF));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_IOPM_XF_ON_MSG_READ_FUNC)
T_TYPEDEF((T_PTR, T_SRV_IOP_XF, T_MSG));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_IOPM_XF_ON_MSGQ_READ_FUNC)
T_TYPEDEF((T_PTR, T_SRV_IOP_XF, T_SIOC_MSG_IQ));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_IOPM_XF_ON_IOCTL_FUNC)
T_TYPEDEF((T_PTR, T_SRV_IOP_XF, T_SIOC_IOCTL));

typedef void (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_IOPM_XF_ON_PORT_CLOSE_FUNC)
T_TYPEDEF((T_PTR, T_SRV_IOP_XF));
                                                
/*
 * Names of the ROUTING ENGINE interface methods
 */
#if !defined(T_OS_OSS) && !defined(T_OS_MVS_OE) && !defined(T_OS_VMS)
#define T_SRV_RE_INIT_FUNC_NAME           "TreInit"
#define T_SRV_RE_INIT_THREADS_FUNC_NAME   "TreInitThreads"
#define T_SRV_RE_CREATE_FUNC_NAME         "TreCreate"
#define T_SRV_RE_DESTROY_FUNC_NAME        "TreDestroy"
#define T_SRV_RE_CLOSE_FUNC_NAME          "TreClose"
#else
#define T_SRV_RE_INIT_FUNC_NAME           TreInit
#define T_SRV_RE_INIT_THREADS_FUNC_NAME   TreInitThreads
#define T_SRV_RE_CREATE_FUNC_NAME         TreCreate
#endif  /* T_OS_OSS  T_OS_MVS_OE T_OS_VMS */

/*
 * The ROUTING ENGINE interface methods. 
 */        
typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_RE_XF_INIT_FUNC)
T_TYPEDEF((void));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_RE_XF_INIT_THREADS_FUNC)
T_TYPEDEF((void));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_RE_XF_CREATE_FUNC)
T_TYPEDEF((T_STR, T_SRV_LM_XF, T_SRV_IOPM_XF *));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_RE_XF_DESTROY_FUNC)
T_TYPEDEF((T_SRV_IOPM_XF));

typedef T_BOOL (T_ENTRY1 * T_ENTRY2 T_EXPORT T_SRV_RE_XF_CLOSE_FUNC)
T_TYPEDEF((void));

/*                                    
 * The License Manager interface   
 */                                   
struct T_SRV_LM_XF_STRUCT {
  T_PTR                                   ptr;
  T_SRV_LM_XF_GET_SERVER_COUNT_FUNC       get_server_count;
  T_SRV_LM_XF_GET_CLIENT_COUNT_FUNC       get_client_count;
  T_SRV_LM_XF_IS_SERVER_ALLOWED_FUNC      is_server_allowed;
  T_SRV_LM_XF_IS_CLIENT_ALLOWED_FUNC      is_client_allowed;
  T_SRV_LM_XF_IS_ADVANCED_FUNC            is_advanced;
  T_SRV_LM_XF_IS_MULTIPROC_FUNC           is_multiproc;
  T_SRV_LM_XF_IS_GROUP_FUNC               is_group;
  T_SRV_LM_XF_IS_LOCAL_FUNC               is_local;
};

/*                                    
 * The I/O PORT interface   
 */                                   
struct T_SRV_IOP_XF_STRUCT {
  T_PTR                            ptr;
  T_SRV_ADDREF_FUNC                addref;
  T_SRV_IOP_XF_SET_USER_PROP_FUNC  set_user_prop;
  T_SRV_IOP_XF_GET_USER_PROP_FUNC  get_user_prop;
  T_SRV_IOP_XF_MSG_WRITE_FUNC      msg_write;
  T_SRV_IOP_XF_MSGQ_WRITE_FUNC     msgq_write;
  T_SRV_IOP_XF_IOCTL_FUNC          ioctl;
  T_SRV_IOP_XF_CLOSE_FUNC          close;
  T_SRV_RELEASE_FUNC               release;
};

/*
 * The I/O PORT MANAGER interface
 */        
struct T_SRV_IOPM_XF_STRUCT {
  T_PTR                              ptr;
  T_SRV_IOPM_XF_ON_PORT_OPEN_FUNC    on_port_open;
  T_SRV_IOPM_XF_ON_MSG_READ_FUNC     on_msg_read;
  T_SRV_IOPM_XF_ON_MSGQ_READ_FUNC    on_msgq_read;
  T_SRV_IOPM_XF_ON_IOCTL_FUNC        on_ioctl;
  T_SRV_IOPM_XF_ON_PORT_CLOSE_FUNC   on_port_close;
};

/*
 * The ROUTING ENGINE interface
 */
struct T_SRV_RE_XF_STRUCT {
  T_PTR                             ptr;
  T_SRV_RE_XF_INIT_FUNC             init;
  T_SRV_RE_XF_INIT_THREADS_FUNC     init_threads;
  T_SRV_RE_XF_CREATE_FUNC           create;
  T_SRV_RE_XF_DESTROY_FUNC          destroy;
  T_SRV_RE_XF_CLOSE_FUNC            close;
};

/*
 * Type information object root interface
 */
struct T_SRV_REFCOUNT_XF_STRUCT {
  T_SRV_ADDREF_FUNC                   addref;
  T_SRV_RELEASE_FUNC                  release;
};                                    

/*
 * Peer generic information
 */
struct T_SRV_PEER_INFO_STRUCT {
  T_SRV_ADDREF_FUNC   addref;          /* Refcount interface method         */
  T_SRV_RELEASE_FUNC  release;         /* Refcount interface method         */
  T_STR               ident;           /* Peer RTmon identifier             */
  T_STR               usid;            /* Peer Unique Subject ID            */
  T_STR               node;            /* Peer node name                    */
  T_INT4              pid;             /* Peer pid                          */
  T_STR               user;            /* Peer user name                    */
  T_STR               arch;            /* Peer $RTARCH                      */
  T_INT2              version;         /* Peer protocol version             */
  T_INT4              refcount;
};

/*
 * Peer type specific information 
 */
struct T_SRV_CLIENT_INFO_STRUCT {
  T_SRV_ADDREF_FUNC   addref;          /* Refcount interface method         */
  T_SRV_RELEASE_FUNC  release;         /* Refcount interface method         */
  T_STR               usid;            /* Client Unique Subject ID          */
  T_STR               project;         /* Client project name               */
  T_STR               dsp;             /* Client default subject prefix     */
  T_INT2              disconnect_mode; /* Client disconnect mode            */
  T_INT4              num_subjects;    /* Client's subject subscribe count  */
  T_STR              *subject_list;    /* Client subject subscribe list     */
  T_INT4             *lb_list;         /* Client lb override list           */
  T_INT4              cap;             /* Client's capabilties              */
  T_INT2              connect_version; /* Client's CONNECT CALL version     */
  T_INT4              refcount;
};

struct T_SRV_CONN_NAMES_STRUCT {
  T_STR               server_name;     /* Name of server                    */
  T_PTR_ARY           conn_names_list; /* List of conn names                */
};

struct T_SRV_SERVER_INFO_STRUCT {
  T_SRV_ADDREF_FUNC   addref;          /* Refcount interface method         */
  T_SRV_RELEASE_FUNC  release;         /* Refcount interface method         */
  T_STR               name;            /* Server name                       */
  T_SRV_CONN_NAMES    conn_names;      /* Server's conn names               */ 
  T_PTR_ARY           server_list;     /* Server's servers conn names       */
  T_INT4              weight;          /* Server connection weight          */
  T_INT4              refcount;        /* Server info reference count       */
};

struct T_SRV_GROUP_INFO_STRUCT {
  T_SRV_ADDREF_FUNC   addref;          /* Refcount interface method         */
  T_SRV_RELEASE_FUNC  release;         /* Refcount interface method         */
  T_STR               name;            /* Group name                        */
  T_INT4              id[4];           /* Group unique identifier           */
  T_INT4              threshold;       /* Threshold for group               */
  T_INT4              weight;          /* Connection weight (?)             */
  T_INT4              refcount;        /* Group info reference count        */
};

/* 
 * Peer types
 */ 
typedef enum {
  T_SRV_PT_UNKNOWN,                     /* Peer unknown                     */
  T_SRV_PT_CLIENT,                      /* Peer client                      */
  T_SRV_PT_SERVER,                      /* Peer server                      */
  T_SRV_PT_GROUP                        /* Peer group                       */
} T_SRV_PEER_TYPE;

/* 
 * Peer connection modes
 */ 
typedef enum {
  T_SRV_PEM_CONNECT,                    /* Point-Point active connect       */
  T_SRV_PEM_ACCEPT                      /* Point-Point passive connect      */
} T_SRV_PEER_ESTABLISH_MODE;

/*
 * I/O channel information
 */
struct T_SRV_CHANNEL_INFO_STRUCT {
  T_SRV_ADDREF_FUNC         addref;     /* Refcount interface method        */
  T_SRV_RELEASE_FUNC        release;    /* Refcount interface method        */
  T_INT4                    id;         /* Channel id                       */
  T_EDM                     edm;        /* Channel EDM                      */
  T_SRV_PEER_INFO           pinfo;      /* Peer generic information         */  
  T_SRV_PEER_TYPE           type;       /* Peer type                        */
  T_SRV_PEER_ESTABLISH_MODE mode;       /* Peer establishment mode          */
  union  {     
    T_PTR                   ptr;        /* Type information                 */ 
    T_SRV_SERVER_INFO       server;     /* Server information               */
    T_SRV_CLIENT_INFO       client;     /* Client information               */
    T_SRV_GROUP_INFO        group;      /* Group information                */
  } tinfo;                              /* Peer type specific information   */
  T_MUTEX                   mutex;      /* Protection mutex                 */
  T_INT4                    refcount;   /* Reference count                  */
  T_PROPERTIES              properties; /* Properties of the channel        */
};
 
#endif /* _RTWORKS_SERVER_H_ */
