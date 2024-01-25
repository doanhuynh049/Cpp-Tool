/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tiof/biof/dev/68/src/rtworks/ioctl.h#1 $
 */

#ifndef _RTWORKS_IOCTL_H__
#define _RTWORKS_IOCTL_H__

typedef enum {
  T_IOCTL_GETPROP,                   /* get properties                      */
  T_IOCTL_SETPROP,                   /* set properties                      */
  T_IOCTL_EXEC,                      /* execute a user readable command     */
  T_IOCTL_GETBUFFERINFO,             /* get i/o channel buffer information  */
  T_IOCTL_SUSPENDINFLOW,             /* suspend inbound data flow           */
  T_IOCTL_RESUMEINFLOW,              /* resume inbound data flow            */
  T_IOCTL_ONOUTFLOWHWM,              /* outbound flow HWM reached           */
  T_IOCTL_ONOUTFLOWLWM,              /* outbound flow LWM reached           */
  T_IOCTL_SETOUTBOUNDRATEPARAMS,     /* set bandwidth control parameters    */
  T_IOCTL_SERVERCONGESTIONSETWATCH   /* server buffer set watch parameters  */
} T_IOCTL_COMMANDS;
 
typedef struct T_IOCTL_BUFFER_INFO_STRUCT
               T_IOCTL_BUFFER_INFO_STRUCT, 
              *T_IOCTL_BUFFER_INFO; 
struct T_IOCTL_BUFFER_INFO_STRUCT {
  T_UINT4   rq_msgs;         /* Number of messages in read queue   */
  T_UINT4   rq_bytes;        /* Number of bytes in the read queue  */
  T_UINT4   rbuf_size;       /* Read buffer size                   */
  T_UINT4   rbuf_max_size;   /* Maximum read buffer size           */
  T_UINT4   wbuf_size;       /* Write buffer size                  */
  T_UINT4   wbuf_max_size;   /* Maximum write buffer size          */
};

typedef struct T_IOCTL_OUTBOUND_RATE_PARAMS_STRUCT
               T_IOCTL_OUTBOUND_RATE_PARAMS_STRUCT, 
              *T_IOCTL_OUTBOUND_RATE_PARAMS; 
struct T_IOCTL_OUTBOUND_RATE_PARAMS_STRUCT {
  T_STR	   connection;    /* name of connection whose params are being set*/
  T_INT4   token_rate;      /* rate (bytes/sec) at which tokens accumulate */
  T_INT4   max_tokens;      /* maximum number of tokens that may accumulate */
  T_REAL8  burst_interval;  /* burst interval in number of seconds */
};

typedef struct T_IOCTL_SERVER_CONGESTION_PARAMS_STRUCT
               T_IOCTL_SERVER_CONGESTION_PARAMS_STRUCT,
              *T_IOCTL_SERVER_CONGESTION_PARAMS; 
struct T_IOCTL_SERVER_CONGESTION_PARAMS_STRUCT {
  T_INT4                  high_water;   /* high water threshold              */
  T_INT4                  low_water;    /* high water threshold              */
  T_BOOL                  watch_status; /* watch status                      */
  T_STR                   client_name;  /* name of client doing the watching */
  T_STR                   server_name;  /* name server of client doing the   */
                                        /* watching                          */
  T_STR                   watch_key;    /* key that uniquely ID's the watch  */
};

typedef struct T_IOCTL_SERVER_CONGESTION_STATUS_STRUCT
               T_IOCTL_SERVER_CONGESTION_STATUS_STRUCT,
              *T_IOCTL_SERVER_CONGESTION_STATUS; 
struct T_IOCTL_SERVER_CONGESTION_STATUS_STRUCT {
  T_IOCTL_SERVER_CONGESTION_STATUS  left;
  T_IOCTL_SERVER_CONGESTION_STATUS  right;
  T_INT4   size;                    /* size of the buffer           */
  T_INT4   threshold;               /* threshold value              */
  T_STR    client_name;             /* client the reply is going to */
  T_STR    server_name;             /* server the reply is going to */
  T_STR    connected_process_name;  /* conn name that was triggered */
  T_STR    watch_key;               /* key that uniquely ID's watch */
};

#endif /* _RTWORKS_LINK_H__ */

