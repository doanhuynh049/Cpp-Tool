/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tiof/biof/dev/68/src/rtworks/ipclpr.h#2 $
 */

/* This file is generated by cproto - do not edit! */

/* ../rtworks/ipclpr.h -- generated prototypes */

#ifndef T_IPCL_API
# ifndef T_OS_WIN32
# define T_IPCL_API
# else
# ifndef T_STATIC_LINK
# ifndef T_IPCL_SOURCE
# if !defined T_GENERATE_PROTOTYPES
# define T_IPCL_API __declspec(dllimport)
# else
# define T_IPCL_API
# endif
# else
# if !defined T_GENERATE_PROTOTYPES
# define T_IPCL_API __declspec(dllexport)
# else
# define T_IPCL_API
# endif
# endif
# else
# define T_IPCL_API
# endif /* T_STATIC_LINK */
# endif /* T_OS_WIN32 */
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ipclink.c */
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkInitThreads T_PROTO((void));
T_IPCL_API T_IPC_LINK_DRIVER T_ENTRY T_EXPORT TipcLinkDriverCreate T_PROTO((
T_STR name,
T_IPC_LINK_MULTIPLEX_MODE multiplex_mode,
T_BOOL network_mode,
T_IPC_LINK_CREATE_SERVER_FUNC create_server_func,
T_IPC_LINK_CREATE_CLIENT_FUNC create_client_func,
T_IPC_LINK_ACCEPT_CLIENT_FUNC accept_client_func,
T_IPC_LINK_DESTROY_FUNC destroy_func,
T_IPC_LINK_CHECK_FUNC check_func,
T_IPC_LINK_SEND_FUNC send_func,
T_IPC_LINK_RECV_FUNC recv_func,
T_IPC_LINK_SET_BLOCK_MODE_FUNC set_block_mode_func,
T_IPC_LINK_GET_MULTIPLEX_SOURCE_FUNC get_multiplex_source_func,
T_IPC_LINK_SET_MULTIPLEX_FUNC set_multiplex_func,
T_PTR set_multiplex_arg
));
T_IPCL_API T_IPC_LINK_DRIVER T_ENTRY T_EXPORT TipcLinkDriverLookup T_PROTO((
T_STR name
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkDriverDestroy T_PROTO((
T_IPC_LINK_DRIVER driver
));
T_IPCL_API T_PTR T_ENTRY T_EXPORT TipcLinkDriverTraverse T_PROTO((
T_IPC_LINK_DRIVER_TRAV_FUNC func,
T_PTR arg
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkDriverGetName T_PROTO((
T_IPC_LINK_DRIVER driver,
T_STR * name_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkDriverGetMultiplexMode T_PROTO((
T_IPC_LINK_DRIVER driver,
T_IPC_LINK_MULTIPLEX_MODE * multiplex_mode_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkDriverGetUserProp T_PROTO((
T_IPC_LINK_DRIVER driver,
T_PTR * user_prop_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkDriverSetUserProp T_PROTO((
T_IPC_LINK_DRIVER driver,
T_PTR user_prop
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkDriverSetConnectBeginFunc T_PROTO((
T_IPC_LINK_DRIVER driver,
T_IPC_LINK_CONNECT_BEGIN_FUNC connect_begin_func
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkDriverSetConnectCompleteFunc T_PROTO((
T_IPC_LINK_DRIVER driver,
T_IPC_LINK_CONNECT_COMPLETE_FUNC connect_complete_func
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkDriverSetSendQueuedFunc T_PROTO((
T_IPC_LINK_DRIVER driver,
T_IPC_LINK_SEND_QUEUED_FUNC send_queued_func
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkDriverSetHandshakeFunc T_PROTO((
T_IPC_LINK_DRIVER driver,
T_IPC_LINK_HANDSHAKE_FUNC handshake_func
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkDriverSetIoctlFunc T_PROTO((
T_IPC_LINK_DRIVER driver,
T_IPC_LINK_IOCTL_FUNC ioctl_func
));
T_IPCL_API T_IPC_LINK_DEFAULT T_ENTRY T_EXPORT TipcLinkDefaultCreate T_PROTO((
T_IPC_LINK_DRIVER driver,
T_STR name,
T_STR value
));
T_IPCL_API T_IPC_LINK_DEFAULT T_ENTRY T_EXPORT TipcLinkDefaultLookup T_PROTO((
T_IPC_LINK_DRIVER driver,
T_STR name
));
T_IPCL_API T_PTR T_ENTRY T_EXPORT TipcLinkNameExpandTraverse T_PROTO((
T_STR short_name,
T_STR default_name,
T_STR_LIST default_protocols,
T_IPC_LINK_NAME_EXPAND_TRAV_FUNC func,
T_PTR arg
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkParseName T_PROTO((
T_STR link_name_copy,
T_IPC_LINK_DRIVER * driver_return,
T_STR * node_return,
T_STR * address_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkParseExtendedName T_PROTO((
T_STR link_name_copy,
T_IPC_LINK_DRIVER * driver_return,
T_STR * node_return,
T_STR * address_return,
T_IPC_LINK_PROPS * props_return
));
T_IPCL_API T_IPC_LINK T_ENTRY T_EXPORT TipcLinkCreateServer T_PROTO((
T_STR link_name
));
T_IPCL_API T_IPC_LINK T_ENTRY T_EXPORT TipcLinkCreateClient T_PROTO((
T_STR link_name
));
T_IPCL_API T_IPC_LINK T_ENTRY T_EXPORT TipcLinkConnectBegin T_PROTO((
T_STR link_name
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkConnectComplete T_PROTO((
T_IPC_LINK client_link,
T_REAL8 timeout
));
T_IPCL_API T_IPC_LINK T_ENTRY T_EXPORT TipcLinkAcceptClient T_PROTO((
T_IPC_LINK server_link
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkDestroy T_PROTO((
T_IPC_LINK lnk
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkCheck T_PROTO((
T_IPC_LINK lnk,
T_IO_CHECK_MODE check_mode,
T_REAL8 timeout
));
T_IPCL_API T_INT4 T_ENTRY T_EXPORT TipcLinkSend T_PROTO((
T_IPC_LINK lnk,
T_PTR data,
T_INT4 size
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSendQueued T_PROTO((
T_IPC_LINK lnk
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkHandshake T_PROTO((
T_IPC_LINK lnk
));
T_IPCL_API T_INT4 T_ENTRY T_EXPORT TipcLinkRecv T_PROTO((
T_IPC_LINK lnk,
T_PTR data,
T_INT4 size
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkGetBlockMode T_PROTO((
T_IPC_LINK lnk,
T_BOOL * block_mode_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSetBlockMode T_PROTO((
T_IPC_LINK lnk,
T_BOOL block_mode
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkIoctl T_PROTO((
T_IPC_LINK lnk,
T_IOCTL_COMMANDS cmd,
T_PTR arg
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkGetDevice T_PROTO((
T_IPC_LINK lnk,
T_PTR * device_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSetDevice T_PROTO((
T_IPC_LINK lnk,
T_PTR device
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkGetMultiplexSource T_PROTO((
T_IPC_LINK lnk,
T_PTR * multiplex_source_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkGetDriver T_PROTO((
T_IPC_LINK lnk,
T_IPC_LINK_DRIVER * driver_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkGetUserProp T_PROTO((
T_IPC_LINK lnk,
T_PTR * user_prop_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSetUserProp T_PROTO((
T_IPC_LINK lnk,
T_PTR user_prop
));
T_IPCL_API T_INT4 T_ENTRY T_EXPORT TipcLinkRecvTimeout T_PROTO((
T_IPC_LINK lnk,
T_PTR data,
T_INT4 size,
T_REAL8 timeout
));
T_IPCL_API T_INT4 T_ENTRY T_EXPORT TipcLinkRecvAll T_PROTO((
T_IPC_LINK lnk,
T_PTR data,
T_INT4 size
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkNameCompare T_PROTO((
T_STR full_name_1,
T_STR full_name_2,
T_BOOL * compare_result
));

/* lnklocal.c */
T_IPCL_API void T_ENTRY T_EXPORT TipcLinkDriverLocalInit T_PROTO((void));

/* lnkproxy.c */
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxyCreateServer T_PROTO((
T_IPC_LINK server_link,
T_STR node,
T_STR address
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxyCreateClient T_PROTO((
T_IPC_LINK client_link,
T_STR node,
T_STR address
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxyConnectBegin T_PROTO((
T_IPC_LINK client_link,
T_STR node,
T_STR address
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxyConnectComplete T_PROTO((
T_IPC_LINK client_link,
T_REAL8 timeout
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxyAcceptClient T_PROTO((
T_IPC_LINK server_link,
T_IPC_LINK accept_link
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxyDestroy T_PROTO((
T_IPC_LINK lnk
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxyCheck T_PROTO((
T_IPC_LINK lnk,
T_IO_CHECK_MODE check_mode,
T_REAL8 timeout
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxySend T_PROTO((
T_IPC_LINK lnk,
T_PTR data,
T_INT4 size
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxyRecv T_PROTO((
T_IPC_LINK lnk,
T_PTR data,
T_INT4 size
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxySendQueued T_PROTO((
T_IPC_LINK lnk
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxySetBlockMode T_PROTO((
T_IPC_LINK lnk,
T_BOOL block_mode
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxyGetMultiplexSource T_PROTO((
T_IPC_LINK lnk,
T_PTR * multiplex_source_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxySetMultiplex T_PROTO((
T_IPC_LINK lnk,
T_IPC_LINK_SOCKET_DEVICE socket_device,
T_IO_CHECK_MODE check_mode,
T_PTR multiplex_data,
T_BOOL start_flag,
T_PTR arg
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkProxyIoctl T_PROTO((
T_IPC_LINK lnk,
T_IOCTL_COMMANDS cmd,
T_PTR arg
));

/* lnksock.c */
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSocketDeviceCreate T_PROTO((
T_SOCK socket_fd,
T_IPC_LINK_SOCKET_DEVICE * device
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSocketDeviceDestroy T_PROTO((
T_IPC_LINK_SOCKET_DEVICE * device
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSocketAcceptClient T_PROTO((
T_IPC_LINK server_link,
T_IPC_LINK accept_link
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSocketDestroy T_PROTO((
T_IPC_LINK lnk
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSocketCheck T_PROTO((
T_IPC_LINK lnk,
T_IO_CHECK_MODE check_mode,
T_REAL8 timeout
));
T_IPCL_API T_INT4 T_ENTRY T_EXPORT TipcLinkSocketSend T_PROTO((
T_IPC_LINK lnk,
T_PTR data,
T_INT4 size
));
T_IPCL_API T_INT4 T_ENTRY T_EXPORT TipcLinkSocketRecv T_PROTO((
T_IPC_LINK lnk,
T_PTR data,
T_INT4 size
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSocketSetBlockMode T_PROTO((
T_IPC_LINK lnk,
T_BOOL block_mode
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSocketGetMultiplexSource T_PROTO((
T_IPC_LINK lnk,
T_PTR * multiplex_source_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkSocketParsePort T_PROTO((
T_STR address,
T_INT4 * port_num_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcWinLinkSocketSetMultiplex T_PROTO((
T_IPC_LINK lnk,
T_IPC_LINK_SOCKET_DEVICE device,
T_IO_CHECK_MODE check_mode,
T_PTR multiplex_data,
T_BOOL start_flag,
T_PTR arg
));

/* lnktcp.c */
T_IPCL_API void T_ENTRY T_EXPORT TipcLinkDriverTcpInit T_PROTO((void));

/* lnkudpbc.c */
T_IPCL_API void T_ENTRY T_EXPORT TipcLinkDriverUdpBroadcastInit T_PROTO((void));

/* lnkprops.c */
T_IPCL_API T_IPC_LINK_PROPS T_ENTRY T_EXPORT TipcLinkPropsCreate T_PROTO((
T_STR props_string
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkPropsDestroy T_PROTO((
T_IPC_LINK_PROPS props
));
T_IPCL_API T_IPC_LINK_PROPS T_ENTRY T_EXPORT TipcLinkPropsCopy T_PROTO((
T_IPC_LINK_PROPS props
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkPropsGetFirstStr T_PROTO((
T_IPC_LINK_PROPS props,
T_STR field_name,
T_STR * str_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkPropsGetFirstInt4 T_PROTO((
T_IPC_LINK_PROPS props,
T_STR field_name,
T_INT4 * int4_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkPropsGetFirstBool T_PROTO((
T_IPC_LINK_PROPS props,
T_STR field_name,
T_BOOL * bool_return
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkPropsGetAllValues T_PROTO((
T_IPC_LINK_PROPS props,
T_STR field_name,
T_PTR_ARY * ptr_ary_return
));
T_IPCL_API T_PTR T_ENTRY T_EXPORT TipcLinkPropsTraverse T_PROTO((
T_IPC_LINK_PROPS props,
T_IPC_LINK_PROPS_TRAV_FUNC trav_func,
T_PTR arg
));

/* lnkcomp.c */
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkCompressionCreate T_PROTO((
T_IPC_LINK lnk
));
T_IPCL_API T_BOOL T_ENTRY T_EXPORT TipcLinkCompressionDestroy T_PROTO((
T_IPC_LINK lnk
));

#ifdef __cplusplus
}
#endif
