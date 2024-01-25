/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/msgmt.h#1 $
 */

#ifndef _RTWORKS_MSGMT_H_
#define _RTWORKS_MSGMT_H_


/*
 * Message number mapping:
 *           0 to         -99   reserved for general messages(1)
 *        -100 to        -199   reserver for DEV messages
 *        -200 to        -299   reserver for component-specific DEV messages
 *        -300 to        -399   reserved for RTserver
 *        -400 to        -499   reserved for monitoring messages
 *        -500 to        -599   reserved for RThci X event messages
 *        -600 to        -649   reserved for archive/playback
 *        -650 to        -699   unknown; treat as reserved
 *        -700 to        -799   reserved for SmartQueue
 *        -800 to        -899   reserved (properties?)
 *        -900 to        -909   reserved for encryption message types
 *        -910 to        -999   reserved for JMS message types
 *       -1000 to       -1099   reserved for SPC
 *       -1100 to       -1199   reserved for SVC
 *       -1200 to       -1299   reserved for SPSS
 *       -1300 to       -1399   reserved for Gateway
 *       -1400 to       -1499   reserved for Gateway MSL
 *       -1500 to       -1799   reserved for monitoring messages
 *       -1800 to       -2399   UNUSED
 *       -2400 to       -6399   reserved for Gateway MSL drivers
 *       -6400 to       -9999   reserved for Gateway CI and drivers
 *      -10000 to      -10999   "reserved" for consulting products
 *          -10050 to      -10069   used by SmartCache!!
 *                                  (cpsi/src/rtworks/scacheum.h)
 *      -11000 to      -11099   UNUSED
 *      -11100 to      -11199   reserved for tdbi package
 *      -11200 to      -11399   UNUSED
 *      -11400 to      -11499   reserved for PERF tool
 *      -11500 to      -11599   reserved for Lite Client Arch
 *      -11600 to      -11699   reserved for RTserver(1)
 *      -11700 to      -11799   reserved for admin messages
 *      -11800 to      -19999   UNUSED
 *      -20000 to      -20599   reserved for RMS messages
 *      -20600 to      -20999   UNUSED
 *      -21000 to      -21099   reserved for AMD (Assured Message Deliver)
 *      -21100 to      -21199   reserved for TibCache API
 *      -21200 to -2147483648   UNUSED
 *
 * Please keep this list up to date!
 *
 * (1) Be VERY careful when adding messages within this block, as they are not
 * well ordered in this file!
 */

/*
 * RTserver/client messages
 */
#define T_MT_CONNECT_RESULT_66                  (-1)  /* Client connect reply from RTserver       */
#define T_MT_CONNECT_CALL_66                    (-2)  /* Client connect call to RTserver          */
#define T_MT_CONNECT_RESULT_74                  (-80) /* Connect result (for rpc) version 74      */
#define T_MT_CONNECT_CALL_74                    (-81) /* Connect version 74                       */
#define T_MT_CONNECT_RESULT                     (-74) /* Client connect reply from RTserver       */
#define T_MT_CONNECT_CALL                       (-75) /* Client connect call to RTserver          */
#define T_MT_CLIENT_CONNECT_RESULT              (-82) /* Connect result (for rpc) auth version    */
#define T_MT_CLIENT_CONNECT_CALL                (-83) /* Connect auth version                     */
#define T_MT_CLIENT_OPEN_RESULT                 (-84) /* Client open call reply                   */
#define T_MT_CLIENT_OPEN_CALL                   (-85) /* Client open call                         */
#define T_MT_CLIENT_AUTH_CHALLENGE_RESULT       (-86) /* Client authentication challenge reply    */
#define T_MT_CLIENT_AUTH_CHALLENGE_CALL         (-87) /* Client authentication challenge call     */
#define T_MT_SERVER_STOP_RESULT                 (-3)  /* Stop RTserver result                     */
#define T_MT_SERVER_STOP_CALL                   (-4)  /* Stop RTserver call                       */
#define T_MT_DISCONNECT                         (-5)  /* Explicit disconnect from RTserver        */
#define T_MT_CLIENT_CLOSE_RESULT                (-88) /* Client close reply from RTserver         */
#define T_MT_CLIENT_CLOSE_CALL                  (-89) /* Client close call to RTserver            */

/*
 * Group messages
 */
#define T_MT_GRP_CONNECT_CALL                   (-90)
#define T_MT_GRP_CONNECT_RESULT                 (-91)
#define T_MT_GRP_MEMBER_UP                      (-92)
#define T_MT_GRP_MEMBER_DOWN                    (-93)
#define T_MT_GRP_MEMBER_PEER_INFO_77            (-94)
#define T_MT_GRP_MEMBER_PEER_INFO               (-95)
#define T_MT_GRP_PEER_INFO                      (-96)
#define T_MT_GRP_DOWN                           (-97)
#define T_MT_GRP_SET_THRESHOLD                  (-98)
#define T_MT_GRP_STOP_RTGMS                     (-99)
#define T_MT_GRP_MEMBER_UP_RESULT               (-11602)
#define T_MT_GRP_MEMBER_UP_CALL                 (-11603)
#define T_MT_GRP_MEMBER_DOWN_RESULT             (-11604)
#define T_MT_GRP_MEMBER_DOWN_CALL               (-11605)
#define T_MT_GRP_ADMIN_SET_OUTBOUND_RATE_PARAMS (-11606)
#define T_MT_GRP_ADMIN_GET_OUTBOUND_RATE_PARAMS (-11607)
#define T_MT_GRP_ADMIN_SET_PGM_OPTIONS          (-11608)
#define T_MT_GRP_ADMIN_GET_PGM_OPTIONS          (-11609)

/*
 * Connection messages
 */
#define T_MT_CONN_INIT                          (-10) /* Connection init                    */
#define T_MT_GMD_ACK                            (-11) /* GMD acknowledgement                */
#define T_MT_GMD_NACK                           (-12) /* GMD negative ack                   */
#define T_MT_GMD_FAILURE                        (-13) /* GMD delivery failure               */
#define T_MT_GMD_DELETE                         (-14) /* GMD cancel                         */
#define T_MT_GMD_STATUS_RESULT                  (-15) /* GMD status                         */
#define T_MT_GMD_STATUS_CALL                    (-16) /* GMD status query                   */
#define T_MT_GMD_INIT_RESULT                    (-77) /* GMD start result                   */
#define T_MT_GMD_INIT_CALL                      (-78) /* GMD start call                     */
#define T_MT_KEEP_ALIVE_RESULT                  (-18) /* Connection keep alive reply        */
#define T_MT_KEEP_ALIVE_CALL                    (-19) /* Connect keep alive call            */

/*
 * Data messages
 */
#define T_MT_BOOLEAN_DATA                       (-20)
#define T_MT_ENUM_DATA                          (-21)
#define T_MT_NUMERIC_DATA                       (-22)
#define T_MT_STRING_DATA                        (-23)
#define T_MT_HISTORY_BOOLEAN_DATA               (-24)
#define T_MT_HISTORY_ENUM_DATA                  (-25)
#define T_MT_HISTORY_NUMERIC_DATA               (-26)
#define T_MT_HISTORY_STRING_DATA                (-27)
#define T_MT_END_OF_FRAME                       (-28)
#define T_MT_TIME                               (-29)
#define T_MT_CONTROL                            (-30)
#define T_MT_VAR_VALUE_RESULT                   (-31)
#define T_MT_VAR_VALUE_CALL                     (-32)
#define T_MT_INFO                               (-40)
#define T_MT_ALERT                              (-41)
#define T_MT_CANCEL_ALERT                       (-42)
#define T_MT_WARNING                            (-43)
#define T_MT_CANCEL_WARNING                     (-44)

/*
 * Note : -45 to -51  used by RThci
 */
#define T_MT_OBJ_FG                             (-45)
#define T_MT_OBJ_BG                             (-46)
#define T_MT_VAR_FG                             (-47)
#define T_MT_VAR_BG                             (-48)
#define T_MT_POPUP                              (-49)
#define T_MT_POPDOWN                            (-50)
#define T_MT_VIEW_DATA                          (-51)

/*
 * Note : -54 used by T_MT_OBJECT_DATA
 */
#define T_MT_OBJECT_DATA                        (-54)

/*
 * Note : -60 to -61  used by RTie
 */
#define T_MT_RULE_CREATE                        (-60)
#define T_MT_RULE_DESTROY                       (-61)

/*
 * Subject messages
 */
#define T_MT_SUBJECT_SET_SUBSCRIBE_66           (-70)     /* Start/stop subject subscribe       */
#define T_MT_SUBJECT_SET_SUBSCRIBE              (-76)     /* modified for load balancing info   */
#define T_MT_BATCH_SUBJECT_SET_SUBSCRIBE_RESULT (-11600)  /* Batch subscription response        */
#define T_MT_BATCH_SUBJECT_SET_SUBSCRIBE        (-11601)  /* Batch subscription request         */
#define T_MT_SUBJECT_RETRIEVE                   (-73)     /* retrieve subject values            */

/*
 * Generic message type
 */
#define T_MT_ANY                                (-79)

/*
 * -80 to -99 reserved for IPC.
 */
#define T_IPC_FIRST_MT                          (-80)

/*
 * Note : -100 to -200 used for DEV messages
 */
#define T_DEV_FIRST_MT                          (-100) /* generic dev. env. message types */
#define T_DEVDAQ_FIRST_MT                       (-200) /* RTdaq-specific */
#define T_DEVIE_FIRST_MT                        (-200) /* RTie-specific */
#define T_DEVMON_FIRST_MT                       (-200) /* RTmon-specific */
#define T_DEVVAR_FIRST_MT                       (-200) /* RTvar-specific */

/*
 * -300 to -399 reserved for RTserver
 */
#define T_SRV_FIRST_MT                          (-300)

/*
 * -400 to -499, -1500 to -1799 reserved for Monitoring message types
 */
#define T_MON_FIRST_MT                                 (-400)
#define T_MON_EX_FIRST_MT                              (-1500)
#define T_MT_MON_PROJECT_NAMES_SET_WATCH               (T_MON_FIRST_MT - 0)
#define T_MT_MON_PROJECT_NAMES_STATUS                  (T_MON_FIRST_MT - 1)
#define T_MT_MON_CLIENT_NAMES_SET_WATCH                (T_MON_FIRST_MT - 2)
#define T_MT_MON_CLIENT_NAMES_STATUS                   (T_MON_FIRST_MT - 3)
#define T_MT_MON_SERVER_NAMES_SET_WATCH                (T_MON_FIRST_MT - 4)
#define T_MT_MON_SERVER_NAMES_STATUS                   (T_MON_FIRST_MT - 5)
#define T_MT_MON_SUBJECT_NAMES_SET_WATCH               (T_MON_FIRST_MT - 6)
#define T_MT_MON_SUBJECT_NAMES_STATUS                  (T_MON_FIRST_MT - 7)
#define T_MT_MON_SUBJECT_SUBSCRIBE_SET_WATCH           (T_MON_FIRST_MT - 8)
#define T_MT_MON_SUBJECT_SUBSCRIBE_STATUS              (T_MON_FIRST_MT - 9)
#define T_MT_MON_CLIENT_SUBSCRIBE_SET_WATCH            (T_MON_FIRST_MT - 10)
#define T_MT_MON_CLIENT_SUBSCRIBE_STATUS               (T_MON_FIRST_MT - 11)
#define T_MT_MON_CLIENT_TIME_SET_WATCH                 (T_MON_FIRST_MT - 12)
#define T_MT_MON_CLIENT_TIME_STATUS                    (T_MON_FIRST_MT - 13)
#define T_MT_MON_CLIENT_BUFFER_SET_WATCH               (T_MON_FIRST_MT - 14)
#define T_MT_MON_CLIENT_BUFFER_STATUS                  (T_MON_FIRST_MT - 15)
#define T_MT_MON_CLIENT_MSG_RECV_SET_WATCH             (T_MON_FIRST_MT - 16)
#define T_MT_MON_CLIENT_MSG_RECV_STATUS                (T_MON_FIRST_MT - 17)
#define T_MT_MON_CLIENT_MSG_SEND_SET_WATCH             (T_MON_FIRST_MT - 18)
#define T_MT_MON_CLIENT_MSG_SEND_STATUS                (T_MON_FIRST_MT - 19)
#define T_MT_MON_SERVER_CONN_SET_WATCH                 (T_MON_FIRST_MT - 20)
#define T_MT_MON_SERVER_CONN_STATUS                    (T_MON_FIRST_MT - 21)
#define T_MT_MON_PROJECT_NAMES_POLL_RESULT             (T_MON_FIRST_MT - 30)
#define T_MT_MON_PROJECT_NAMES_POLL_CALL               (T_MON_FIRST_MT - 31)
#define T_MT_MON_CLIENT_NAMES_POLL_RESULT              (T_MON_FIRST_MT - 32)
#define T_MT_MON_CLIENT_NAMES_POLL_CALL                (T_MON_FIRST_MT - 33)
#define T_MT_MON_SERVER_NAMES_POLL_RESULT              (T_MON_FIRST_MT - 34)
#define T_MT_MON_SERVER_NAMES_POLL_CALL                (T_MON_FIRST_MT - 35)
#define T_MT_MON_SUBJECT_NAMES_POLL_RESULT             (T_MON_FIRST_MT - 36)
#define T_MT_MON_SUBJECT_NAMES_POLL_CALL               (T_MON_FIRST_MT - 37)
#define T_MT_MON_SUBJECT_SUBSCRIBE_POLL_RESULT         (T_MON_FIRST_MT - 38)
#define T_MT_MON_SUBJECT_SUBSCRIBE_POLL_CALL           (T_MON_FIRST_MT - 39)
#define T_MT_MON_CLIENT_SUBSCRIBE_POLL_RESULT          (T_MON_FIRST_MT - 40)
#define T_MT_MON_CLIENT_SUBSCRIBE_POLL_CALL            (T_MON_FIRST_MT - 41)
#define T_MT_MON_CLIENT_GENERAL_POLL_RESULT            (T_MON_FIRST_MT - 42)
#define T_MT_MON_CLIENT_GENERAL_POLL_CALL              (T_MON_FIRST_MT - 43)
#define T_MT_MON_CLIENT_TIME_POLL_RESULT               (T_MON_FIRST_MT - 44)
#define T_MT_MON_CLIENT_TIME_POLL_CALL                 (T_MON_FIRST_MT - 45)
#define T_MT_MON_CLIENT_BUFFER_POLL_RESULT             (T_MON_FIRST_MT - 46)
#define T_MT_MON_CLIENT_BUFFER_POLL_CALL               (T_MON_FIRST_MT - 47)
#define T_MT_MON_CLIENT_OPTION_POLL_RESULT             (T_MON_FIRST_MT - 48)
#define T_MT_MON_CLIENT_OPTION_POLL_CALL               (T_MON_FIRST_MT - 49)
#define T_MT_MON_CLIENT_MSG_TYPE_POLL_RESULT           (T_MON_FIRST_MT - 50)
#define T_MT_MON_CLIENT_MSG_TYPE_POLL_CALL             (T_MON_FIRST_MT - 51)
#define T_MT_MON_CLIENT_CB_POLL_RESULT                 (T_MON_FIRST_MT - 52)
#define T_MT_MON_CLIENT_CB_POLL_CALL                   (T_MON_FIRST_MT - 53)
#define T_MT_MON_CLIENT_MSG_TRAFFIC_POLL_RESULT        (T_MON_FIRST_MT - 54)
#define T_MT_MON_CLIENT_MSG_TRAFFIC_POLL_CALL          (T_MON_FIRST_MT - 55)
#define T_MT_MON_CLIENT_SUBJECT_POLL_RESULT            (T_MON_FIRST_MT - 56)
#define T_MT_MON_CLIENT_SUBJECT_POLL_CALL              (T_MON_FIRST_MT - 57)
#define T_MT_MON_SERVER_GENERAL_POLL_RESULT            (T_MON_FIRST_MT - 58)
#define T_MT_MON_SERVER_GENERAL_POLL_CALL              (T_MON_FIRST_MT - 59)
#define T_MT_MON_SERVER_TIME_POLL_RESULT               (T_MON_FIRST_MT - 60)
#define T_MT_MON_SERVER_TIME_POLL_CALL                 (T_MON_FIRST_MT - 61)
#define T_MT_MON_SERVER_BUFFER_POLL_RESULT             (T_MON_FIRST_MT - 62)
#define T_MT_MON_SERVER_BUFFER_POLL_CALL               (T_MON_FIRST_MT - 63)
#define T_MT_MON_SERVER_OPTION_POLL_RESULT             (T_MON_FIRST_MT - 64)
#define T_MT_MON_SERVER_OPTION_POLL_CALL               (T_MON_FIRST_MT - 65)
#define T_MT_MON_SERVER_CONN_POLL_RESULT               (T_MON_FIRST_MT - 66)
#define T_MT_MON_SERVER_CONN_POLL_CALL                 (T_MON_FIRST_MT - 67)
#define T_MT_MON_SERVER_ROUTE_POLL_RESULT              (T_MON_FIRST_MT - 68)
#define T_MT_MON_SERVER_ROUTE_POLL_CALL                (T_MON_FIRST_MT - 69)
#define T_MT_MON_SERVER_MSG_TRAFFIC_POLL_RESULT        (T_MON_FIRST_MT - 70)
#define T_MT_MON_SERVER_MSG_TRAFFIC_POLL_CALL          (T_MON_FIRST_MT - 71)
#define T_MT_MON_CLIENT_NAMES_NUM_POLL_RESULT          (T_MON_FIRST_MT - 72)
#define T_MT_MON_CLIENT_NAMES_NUM_POLL_CALL            (T_MON_FIRST_MT - 73)
#define T_MT_MON_CLIENT_SUBSCRIBE_NUM_POLL_RESULT      (T_MON_FIRST_MT - 74)
#define T_MT_MON_CLIENT_SUBSCRIBE_NUM_POLL_CALL        (T_MON_FIRST_MT - 75)
#define T_MT_MON_CLIENT_CPU_POLL_RESULT                (T_MON_FIRST_MT - 76)
#define T_MT_MON_CLIENT_CPU_POLL_CALL                  (T_MON_FIRST_MT - 77)
#define T_MT_MON_SERVER_CPU_POLL_RESULT                (T_MON_FIRST_MT - 78)
#define T_MT_MON_SERVER_CPU_POLL_CALL                  (T_MON_FIRST_MT - 79)
#define T_MT_MON_CLIENT_INFO_POLL_RESULT               (T_MON_FIRST_MT - 80)
#define T_MT_MON_CLIENT_INFO_POLL_CALL                 (T_MON_FIRST_MT - 81)
#define T_MT_MON_SERVER_CONGESTION_SET_WATCH           (T_MON_FIRST_MT - 82)
#define T_MT_MON_SERVER_CONGESTION_STATUS              (T_MON_FIRST_MT - 83)
#define T_MT_MON_SERVER_MAX_CLIENT_LICENSES_SET_WATCH  (T_MON_FIRST_MT - 84)
#define T_MT_MON_SERVER_MAX_CLIENT_LICENSES_STATUS     (T_MON_FIRST_MT - 85)
#define T_MT_MON_CLIENT_CONGESTION_SET_WATCH           (T_MON_FIRST_MT - 86)
#define T_MT_MON_CLIENT_CONGESTION_STATUS              (T_MON_FIRST_MT - 87)

/*
 * Keep T_MT_MON_EXT_POLL_* around for backward compatability.
 */
#define T_MT_MON_EXT_POLL_RESULT                       (T_MON_FIRST_MT - 88)
#define T_MT_MON_EXT_POLL_CALL                         (T_MON_FIRST_MT - 89)
#define T_MT_MON_CLIENT_EXT_POLL_RESULT                (T_MON_FIRST_MT - 88)
#define T_MT_MON_CLIENT_EXT_POLL_CALL                  (T_MON_FIRST_MT - 89)

/*
 * New version monitoring message types (6.7)
 */
#define T_MT_MON_CLIENT_VERSION_POLL_RESULT            (T_MON_FIRST_MT - 90)
#define T_MT_MON_CLIENT_VERSION_POLL_CALL              (T_MON_FIRST_MT - 91)
#define T_MT_MON_SERVER_VERSION_POLL_RESULT            (T_MON_FIRST_MT - 92)
#define T_MT_MON_SERVER_VERSION_POLL_CALL              (T_MON_FIRST_MT - 93)
#define T_MT_MON_SERVER_START_TIME_POLL_RESULT         (T_MON_FIRST_MT - 94)
#define T_MT_MON_SERVER_START_TIME_POLL_CALL           (T_MON_FIRST_MT - 95)

/*
 * New INT8 flavors of traffic monitoring message types (6.7)
 */
#define T_MT_MON_CLIENT_MSG_TYPE_EX_POLL_RESULT        (T_MON_EX_FIRST_MT - 0)
#define T_MT_MON_CLIENT_MSG_TYPE_EX_POLL_CALL          (T_MON_EX_FIRST_MT - 1)
#define T_MT_MON_CLIENT_SUBJECT_EX_POLL_RESULT         (T_MON_EX_FIRST_MT - 2)
#define T_MT_MON_CLIENT_SUBJECT_EX_POLL_CALL           (T_MON_EX_FIRST_MT - 3)
#define T_MT_MON_SERVER_MSG_TRAFFIC_EX_POLL_RESULT     (T_MON_EX_FIRST_MT - 4)
#define T_MT_MON_SERVER_MSG_TRAFFIC_EX_POLL_CALL       (T_MON_EX_FIRST_MT - 5)


/*
 * Note : -500 to -532 used by RThci X event messages
 *        -600 to -626 used by archive/playback
 *        -650         used by T_MT_BURST
 *        -700 to -799 reserved for SmartQueue
 */
#define T_MT_BUTTON_PRESS_EVENT                 (-500) /* button press event */
#define T_MT_BUTTON_RELEASE_EVENT               (-501) /* button release event */
#define T_MT_CIRCULATE_NOTIFY_EVENT             (-502) /* circulate notify event */
#define T_MT_CIRCULATE_REQUEST_EVENT            (-503) /* circulate request event */
#define T_MT_CLIENT_MESSAGE_EVENT               (-504) /* client message event */
#define T_MT_COLORMAP_NOTIFY_EVENT              (-505) /* colormap notify event */
#define T_MT_CONFIGURE_NOTIFY_EVENT             (-506) /* configure notify event */
#define T_MT_CONFIGURE_REQUEST_EVENT            (-507) /* configure request event */
#define T_MT_CREATE_NOTIFY_EVENT                (-508) /* create notify event */
#define T_MT_DESTROY_NOTIFY_EVENT               (-509) /* destroy notify event */
#define T_MT_ENTER_NOTIFY_EVENT                 (-510) /* enter notify event */
#define T_MT_LEAVE_NOTIFY_EVENT                 (-511) /* leave notify event */
#define T_MT_EXPOSE_EVENT                       (-512) /* expose event */
#define T_MT_FOCUS_IN_EVENT                     (-513) /* focus in event */
#define T_MT_FOCUS_OUT_EVENT                    (-514) /* focus out event */
#define T_MT_GRAPHICS_EXPOSE_EVENT              (-515) /* graphics expose event */
#define T_MT_NOEXPOSE_EVENT                     (-516) /* no expose event */
#define T_MT_GRAVITY_NOTIFY_EVENT               (-517) /* gravity notify event */
#define T_MT_KEYMAP_NOTIFY_EVENT                (-518) /* keymap notify event */
#define T_MT_KEY_PRESS_EVENT                    (-519) /* key press event */
#define T_MT_KEY_RELEASE_EVENT                  (-520) /* key release event */
#define T_MT_MAP_NOTIFY_EVENT                   (-521) /* map notify event */
#define T_MT_UNMAP_NOTIFY_EVENT                 (-522) /* unmap notify event */
#define T_MT_MAPPING_NOTIFY_EVENT               (-523) /* mapping notify event */
#define T_MT_MAP_REQUEST_EVENT                  (-524) /* map request event */
#define T_MT_POINTER_MOTION_EVENT               (-525) /* pointer motion event */
#define T_MT_PROPERTY_NOTIFY_EVENT              (-526) /* property notify event */
#define T_MT_REPARENT_NOTIFY_EVENT              (-527) /* reparent notify event */
#define T_MT_RESIZE_REQUEST_EVENT               (-528) /* resize request event */
#define T_MT_SELECTION_CLEAR_EVENT              (-529) /* selection clear event */
#define T_MT_SELECTION_NOTIFY_EVENT             (-530) /* selection notify event */
#define T_MT_SELECTION_REQUEST_EVENT            (-531) /* selection request event */
#define T_MT_VISIBILITY_NOTIFY_EVENT            (-532) /* visibility notify event */

#define T_MT_PB_START                           (-600) /* data started flowing from a select */
#define T_MT_PB_END_DATA                        (-601) /* end time of select reached */
#define T_MT_PB_WARNING                         (-602) /* non-fatal playback error */
#define T_MT_PB_RANGE                           (-603) /* range of archive file reported */
#define T_MT_PB_ERROR                           (-604) /* fatal playback error */
#define T_MT_PB_EOF                             (-605) /* playback reached end of archive */
#define T_MT_PB_BAD_CURSOR                      (-606) /* fetch referenced and invalid cursor */
#define T_MT_PB_CLOSE_CURSOR                    (-607) /* close the requested cursor */

#define T_MT_ARC_PB_ACTIVE                      (-620) /* archive/playback is active */
#define T_MT_ARC_PB_REQ_GRANT                   (-621) /* request granted for archive/playback*/
#define T_MT_ARC_PB_DONE                        (-622) /* process is done in archive/playback */
#define T_MT_ARC_PB_SUBJECT_CREATE              (-623) /* new subject created in archive */
#define T_MT_ARC_PB_VAR_CREATE                  (-624) /* new variable created in archive */
#define T_MT_ARC_PB_BACKUP                      (-625) /* backup occured in archive */
#define T_MT_ARC_PB_FLUSH                       (-626) /* flush occured in archive */

#define T_MQS_FIRST_MT                          (-700)

/*
 * Message types for properties
 */
#define T_MT_PROPERTIES_START                   (-800)
#define T_MT_PROPERTIES                         (T_MT_PROPERTIES_START - 1)

/*
 * Encryption message type
 */
#define T_MT_ENCRYPT                            (-900)

/*
 * JMS message types
 */
#define T_MT_JMS_STREAM                         (-910)
#define T_MT_JMS_MAP                            (-911)
#define T_MT_JMS_TEXT                           (-912)
#define T_MT_JMS_OBJECT                         (-913)
#define T_MT_JMS_BYTES                          (-914)

/*
 * Message types -1000 to -1099 reserved for SPC
 */
#define T_MT_SPC_START                          (-1000)

/*
 * Message types -1100 to -1199 reserved for SVC
 */
#define T_MT_SVC_START                          (-1100)

/*
 * Message types -1200 to -1299 reserved for SPSS
 */
#define T_MT_SPSS_START                         (-1200)

/*
 * Message types -1300 to -1399 reserved for GATEWAY
 */
#define T_MT_GW_START                           (-1300)

/*
 * Message types -1400 to -1499 reserved for GATEWAY MSL
 * Message types -1500 to -1799 reserved for Monitoring (see above)
 * Message types -2400 to -6399 reserved for GATEWAY MSL drivers
 */
#define T_MT_GW_MSL_START                       (-1400)

/*
 * Message types -6400 to -9999 reserved for GATEWAY CI and drivers
 */
#define T_MT_GW_CI_START                        (-6400)

/*
 * -10000 to -11000 reserved for consulting products.
 *
 * NOTE: -10050 to      -10069 are used by SmartCache!!
 * See tpsi/cpsi/src/rtworks/scacheum.h for definitions
 */

/*
 * Message types -11100 to -11199 reserved for tdbi package
 */
#define T_MT_DBI_START                          (-11100)
#define T_MT_DBI_ADAPTER_REPLY                  (T_MT_DBI_START-0)
#define T_MT_DBI_ADAPTER_REQUEST                (T_MT_DBI_START-1)
#define T_MT_DBI_NOTIFIER_UPDATE                (T_MT_DBI_START-2)

/*
 * Message types -11400 to -11499 reserved for PERF tool
 */
#define T_PERF_FIRST_MT                         (-11400)


/*
 * Message types -11500 to -11599 reserved for Lite Client Arch
 * This MUST be kept in sync with src/lca/lite_lib/rtworks/mt.h
 */
#define T_MT_LCA_START                          (-11500)

/*
 * Message types -11600 to -11699 are reserved for additional
 * handshake messages handled by RTserver
 */

/*
 * Message types -11700 to -11799 are reserivert for admin messages
 */
#define T_ADMIN_FIRST_MT                        (-11700)
#define T_MT_ADMIN_SET_OUTBOUND_RATE_PARAMS     (T_ADMIN_FIRST_MT - 0)

/*
  * Message types -20000 to -20599 are reserved for RMS
  * messages
  */
#define T_RMS_FIRST_MT                           (-20000)

#define T_MT_RMS_SUBJECT_SET_DURABLE_SUBSCRIBE     (T_RMS_FIRST_MT - 0)
#define T_MT_RMS_SUBJECT_SET_DURABLE_SUBSCRIBE_ACK (T_RMS_FIRST_MT - 1)
#define T_MT_RMS_DURABLE_SUBSCRIBE_MESSAGE         (T_RMS_FIRST_MT - 2)
#define T_MT_RMS_DURABLE_SUBSCRIBE_MESSAGE_ACK     (T_RMS_FIRST_MT - 3)
#if 0
#define T_MT_RMS_DISCOVER_PREVIOUS_SUBSCRIBE       (T_RMS_FIRST_MT - 4)
#define T_MT_RMS_DISCOVER_PREVIOUS_SUBSCRIBE_ACK   (T_RMS_FIRST_MT - 5)
#endif
#define T_MT_RMS_DISCOVER_PRIMARY_RMS              (T_RMS_FIRST_MT - 200)
#define T_MT_RMS_DISCOVER_PRIMARY_RMS_ACK          (T_RMS_FIRST_MT - 201)

#define T_MT_RMS_BEGIN_CLIENT_SESSION              (T_RMS_FIRST_MT - 100)
#define T_MT_RMS_BEGIN_CLIENT_SESSION_ACK          (T_RMS_FIRST_MT - 101)
#define T_MT_RMS_SUSPEND_CLIENT_SESSION            (T_RMS_FIRST_MT - 102)
#define T_MT_RMS_RESUME_CLIENT_SESSION             (T_RMS_FIRST_MT - 103)
#define T_MT_RMS_RESUME_CLIENT_SESSION_ACK         (T_RMS_FIRST_MT - 104)
#define T_MT_RMS_TERMINATE_CLIENT_SESSION          (T_RMS_FIRST_MT - 105)
#define T_MT_RMS_COMMIT_CLIENT_SESSION             (T_RMS_FIRST_MT - 106)
#define T_MT_RMS_COMMIT_CLIENT_SESSION_ACK         (T_RMS_FIRST_MT - 107)

#define T_MT_RMS_PUBLISH_PERSISTENT                (T_RMS_FIRST_MT - 300)
#define T_MT_RMS_PUBLISH_PERSISTENT_ACK            (T_RMS_FIRST_MT - 301)

#define T_MT_RMS_RECOVER_MESSAGES                  (T_RMS_FIRST_MT - 400)
#define T_MT_RMS_RECOVER_MESSAGES_ACK              (T_RMS_FIRST_MT - 401)

#define T_MT_RMS_QUEUE_CREATE                      (T_RMS_FIRST_MT - 500)
#define T_MT_RMS_QUEUE_CREATE_ACK                  (T_RMS_FIRST_MT - 501)
#define T_MT_RMS_QUEUE_CONNECT                     (T_RMS_FIRST_MT - 502)
#define T_MT_RMS_QUEUE_CONNECT_ACK                 (T_RMS_FIRST_MT - 503)
#define T_MT_RMS_QUEUE_CLOSE                       (T_RMS_FIRST_MT - 504)
#define T_MT_RMS_QUEUE_CLOSE_ACK                   (T_RMS_FIRST_MT - 505)
#define T_MT_RMS_QUEUE_DELETE                      (T_RMS_FIRST_MT - 506)
#define T_MT_RMS_QUEUE_DELETE_ACK                  (T_RMS_FIRST_MT - 507)
#define T_MT_RMS_QUEUE_LIST                        (T_RMS_FIRST_MT - 508)
#define T_MT_RMS_QUEUE_LIST_ACK                    (T_RMS_FIRST_MT - 509)
#define T_MT_RMS_QUEUE_BROWSE                      (T_RMS_FIRST_MT - 510)
#define T_MT_RMS_QUEUE_BROWSE_ACK                  (T_RMS_FIRST_MT - 511)
#define T_MT_RMS_QUEUE_GETSIZE                     (T_RMS_FIRST_MT - 512)
#define T_MT_RMS_QUEUE_GETSIZE_ACK                 (T_RMS_FIRST_MT - 513)
#define T_MT_RMS_QUEUE_RECEIVE                     (T_RMS_FIRST_MT - 514)
#define T_MT_RMS_QUEUE_RECEIVE_ACK                 (T_RMS_FIRST_MT - 515)
#define T_MT_RMS_QUEUE_SEND                        (T_RMS_FIRST_MT - 516)
#define T_MT_RMS_QUEUE_SEND_ACK                    (T_RMS_FIRST_MT - 517)


/*
 * Message types -21000 to -21099 reserved for Assured Message Deliver (AMD)
 */
#define T_MT_AMD_START                             (-21000)       /* -21000 -> -21099 */

/*
 * Message types -21100 to -21199 reserved for
 * TibCache API - v2 of SmartCache API.
 */
#define T_MT_TIB_CACHE_FIRST_MT                    (-21100)       /* -21100 -> -21199 */
#define T_MT_TIB_CACHE_RESPONSE                    (T_MT_TIB_CACHE_FIRST_MT - 0)
#define T_MT_TIB_CACHE_REQUEST                     (T_MT_TIB_CACHE_FIRST_MT - 1)


typedef enum {
  T_MT_LOG_DATA,     /* variable values and time */
  T_MT_LOG_STATUS,   /* RThci status messages */
  T_MT_LOG_INTERNAL  /* all other standard message types */
} T_MT_LOG_TYPE;

#endif /* _RTWORKS_MSGMT_H_ */
