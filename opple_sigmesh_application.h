#ifndef __OPPLE_SIGMESH_APPLICATION_H__
#define __OPPLE_SIGMESH_APPLICATION_H__

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "stdint.h"
#include "opple_common.h"

/******************************************************************************
*                                Defines                                      *
******************************************************************************/
#ifndef NULL
#define NULL                        0
#endif

#ifndef WIN32
typedef unsigned int    			opp_size_t;
#endif

#ifndef OPPLE_MEMCPY
#define OPPLE_MEMCPY                memcpy
#endif

#define MAC_LEN 					6
#define KEY_LEN 					16
#define SN_LEN						10
#define OPPLE_OFFSET(s,m) 			(opp_size_t)&(((s *)0)->m)
#define MSG_HEAD_SIZE   		    (OPPLE_OFFSET(OPPLE_MESH_MSG, data) - OPPLE_OFFSET(OPPLE_MESH_MSG, msg_id)) 
#define OPP_VD_HEAD_SIZE   		    (OPPLE_OFFSET(OPPLE_VENDOR_CMD_T, msg_id) - OPPLE_OFFSET(OPPLE_VENDOR_CMD_T, dst_addr)) 

#define OPPLE_CONTAINER_OF(ptr, type, member)  ({              \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - OPPLE_OFFSET(type,member) );}) 

#define M_OPP_NTOHL(x)              ((((x) & 0xff000000) >> 24) \
                                    | (((x) & 0x00ff0000) >> 8) \
                                    | (((x) & 0x0000ff00) << 8) \
                                    | (((x) & 0x000000ff) << 24))

#define M_OPP_NTOHS(x)              ((((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8))


#define M_OPP_HTONL(x)              (M_OPP_NTOHL((x)))

#define M_OPP_HTONS(x)              (M_OPP_NTOHS((x)))

#define OPPLE_SINGLE_PKT_LEN        (13)
#define SIG_MESH_SINGLE_PKT_SIZE    (sizeof(OPPLE_DEV_NOTIFY_EXTEND))

#define MESH_CMD_ACCESS_LEN_MAX     380
#define ACCESS_WITH_MIC_LEN_MAX     (MESH_CMD_ACCESS_LEN_MAX + 4)
#define DEV_PROVSION_SUCCESS		1
#define DEV_PROVSION_FAIL			0

/*OPPLE Mesh commands*/
#define OPPLE_MSG_SEARCH_MAC_ADDR       	        (0x0201)
#define OPPLE_MSG_SEARCH_MAC_ADDR_RSP   	        (0x0202)
#define OPPLE_MSG_MODIFY_DEV_NAME       	        (0x0205)
#define OPPLE_MSG_MODIFY_DEV_NAME_RSP   	        (0x0206)
#define OPPLE_MSG_FACTORY_RESET         	        (0x0207)
#define OPPLE_MSG_FACTORY_RESET_RSP     	        (0x0208)
#define OPPLE_MSG_START_OTA             	        (0x0209)
#define OPPLE_MSG_DEV_REBOOT            	        (0x020D)
#define OPPLE_MSG_DEV_REBOOT_RSP        	        (0x020E)
#define OPPLE_MSG_NOTIFY_RSP                        (0x020F)
#define OPPLE_MSG_QUERY_DEV_NAME        	        (0x0212)
#define OPPLE_MSG_QUERY_DEV_NAME_RSP    	        (0x0213)
#define OPPLE_MSG_MESH_OTA_PROG_RSP                 (0x0220)
#define OPPLE_MSG_NWK_PARA_CONF         	        (0x0221)
#define OPPLE_MSG_SETDEVADDR            	        (0x0223)
#define OPPLE_MSG_SEARCH_DEV_INFO       	        (0x0225)
#define OPPLE_MSG_SEARCH_DEV_INFO_RSP   	        (0x0226)
#define OPPLE_MSG_IFTTT_BINDING                     (0x0227)
#define OPPLE_MSG_IFTTT_BINDING_RSP                 (0x0228)
#define OPPLE_MSG_ADV_NWK_PARA_CONF                 (0x0229)
#define OPPLE_MSG_QUERY_NWK_PARA_CONF               (0x022B)
#define OPPLE_MSG_QUERY_NWK_PARA_CONF_RSP           (0x022C)
#define OPPLE_MSG_IFTTT_ST_SETTING                  (0x022D)
#define OPPLE_MSG_IFTTT_ST_SETTING_RSP              (0x022E)
#define OPPLE_MSG_IFTTT_ITEM_ST_QRY                 (0x022F)
#define OPPLE_MSG_IFTTT_ITEM_ST_QRY_RSP             (0x0230)
#define OPPLE_MSG_IFTTT_ALL_ST_QRY                  (0x0231)
#define OPPLE_MSG_IFTTT_ALL_ST_QRY_RSP              (0x0232)
#define OPPLE_MSG_GATEWAY_ASSIST_CONF   	        (0x0235)
#define OPPLE_MSG_GATEWAY_ASSIST_RSP    	        (0x0236)
#define OPPLE_MSG_SYNC_TIME             	        (0x0237)
#define OPPLE_MSG_TRANSPARENT_QUERY                 (0x0239)
#define OPPLE_MSG_BATCH_CTL_QUERY                   (0x023D)
#define OPPLE_MSG_BATCH_CTL_QUERY_RSP               (0x023E)
#define OPPLE_MSG_OTA_PROGRESS_QUERY                (0x0243)
#define OPPLE_MSG_OTA_PROGRESS_RSP                  (0x0244)
#define OPPLE_MSG_STATUS_ACK_RSP                    (0x0245)
#define OPPLE_MSG_PROV_PROXY_QUERY                  (0x024B)
#define OPPLE_MSG_PROV_PROXY_RSP                    (0x024C)
#define OPPLE_MSG_INIT_BOARD                        (0x024D)
#define OPPLE_MSG_INIT_BOARD_RSP                    (0x024E)
#define OPPLE_MSG_MESH_OTA_START_QUERY              (0x024F)
#define OPPLE_MSG_MESH_OTA_START_RSP                (0x0250)
#define OPPLE_MSG_FIRMWAREUPG_START_QUERY           (0x0251)
#define OPPLE_MSG_FIRMWAREUPG_START_RSP             (0x0252)
#define OPPLE_MSG_FIRMWAREUPG_END_QUERY             (0x0253)
#define OPPLE_MSG_FIRMWAREUPG_END_RSP               (0x0254)
#define OPPLE_MSG_BOARD_ID                          (0x0255)
#define OPPLE_MSG_BOARD_ID_RSP                      (0x0256)
#define OPPLE_MSG_HEART_BEAT_SYNC                   (0x025B)
#define OPPLE_MSG_HEART_BEAT_SYNC_RSP               (0x025C)
#define OPPLE_MSG_CANCEL_PROV                       (0x0263)
#define OPPLE_MSG_CANCEL_PROV_RSP                   (0x0264)
#define OPPLE_MSG_OTA_TERMINATE                     (0x0265)
#define OPPLE_MSG_OTA_TERMINATE_RSP                 (0x0266)
#define OPPLE_MSG_DEV_UPGRADE_IND                   (0x026D)
#define OPPLE_MSG_DEV_UPGRADE_IND_RSP               (0x026E)
#define OPPLE_MSG_PAIR_LOGIN            	        (0x02E0)
#define OPPLE_MSG_PAIR_LOGIN_RSP                    (0x02E1)
#define OPPLE_MSG_MESH_OTA_START                    (0x02F0)

//scene panel command
#define OPPLE_MSG_PAIRSEARCH_REQ                    (0x025D)
#define OPPLE_MSG_PAIRSEARCH_RSP                    (0x025E)
#define OPPLE_MSG_LIGHTSELECT_REQ                   (0x025F)
#define OPPLE_MSG_LIGHTSELECT_RSP                   (0x0260)
#define OPPLE_MSG_DEV_SCAN_REQ				        (0x027E)
#define OPPLE_MSG_DEV_PROVISION_REQ			        (0x0281)
#define OPPLE_MSG_DEV_PROVISION_RSP			        (0x0282)
#define OPPLE_MSG_START_PROVISION_REQ				(0x0285)
#define OPPLE_MSG_START_PROVISION_RSP				(0x0286)
#define OPPLE_MSG_STEPBRIWITHONOFF_REQ              (0x03DC)
#define OPPLE_MSG_STEPBRIWITHONOFF_RSP              (0x03DD)

#define OPPLE_MSG_ADD_GRP                           (0x0303)
#define OPPLE_MSG_ADD_GRP_RSP                       (0x0304)
#define OPPLE_MSG_DEL_GRP                           (0x0305)
#define OPPLE_MSG_DEL_GRP_RSP                       (0x0306)
#define OPPLE_MSG_QUERY_SENCE           	        (0x0307)
#define OPPLE_MSG_QUERY_SENCE_RSP       	        (0x0308)
#define OPPLE_MSG_ADD_SENCE             	        (0x0309)
#define OPPLE_MSG_ADD_SENCE_RSP         	        (0x030A)
#define OPPLE_MSG_DEL_SENCE             	        (0x030B)
#define OPPLE_MSG_DEL_SENCE_RSP         	        (0x030C)
#define OPPLE_MSG_CALL_SENCE            	        (0x030D)
#define OPPLE_MSG_CALL_SENCE_RSP        	        (0x030E)
#define OPPLE_MSG_DEV_STATUS_QUERY                  (0x030F)
#define OPPLE_MSG_DEV_STATUS_QUERY_RSP              (0x0310)
#define OPPLE_MSG_ONOFF                	            (0x0311)
#define OPPLE_MSG_SWITCH                            OPPLE_MSG_ONOFF
#define OPPLE_MSG_ONOFF_RSP            	            (0x0312)
#define OPPLE_MSG_SWITCH_RSP                        OPPLE_MSG_ONOFF_RSP
#define OPPLE_MSG_MODIFY_BRIGHT         	        (0x0313)
#define OPPLE_MSG_MODIFY_BRIGHT_RSP     	        (0x0314)
#define OPPLE_MSG_STEP_MODIFY_BRIGHT    	        (0x0315)
#define OPPLE_MSG_STEP_MODIFY_BRIGHT_RSP            (0x0316)
#define OPPLE_MSG_MODIFY_RGB_COL                    (0x0317)
#define OPPLE_MSG_MODIFY_RGB_COL_RSP                (0x0318)
#define OPPLE_MSG_STEP_MODIFY_RGB_COL               (0x0319)
#define OPPLE_MSG_STEP_MODIFY_RGB_COL_RSP           (0x031A)
#define OPPLE_MSG_MODIFY_CCT                        (0x031B)
#define OPPLE_MSG_MODIFY_CCT_RSP                    (0x031C)
#define OPPLE_MSG_STEP_MODIFY_CCT                   (0x031D)
#define OPPLE_MSG_STEP_MODIFY_CCT_RSP               (0x031E)
#define OPPLE_MSG_CALL_STATUS                       (0x032F)
#define OPPLE_MSG_CALL_STATUS_RSP                   (0x0330)
#define OPPLE_MSG_QUERY_SCENE_NAME                  (0x0354)
#define OPPLE_MSG_QUERY_SCENE_NAME_RSP              (0x0355)
#define OPPLE_MSG_ONOFF_SCENE                       (0x0356)
#define OPPLE_MSG_MODIFY_SCENE_NAME                 (0x0358)
#define OPPLE_MSG_MODIFY_SCENE_NAME_RSP             (0x0359)
#define OPPLE_MSG_CURTAIN_PERCENT_SET				(0x0362)
#define OPPLE_MSG_CURTAIN_PERCENT_RSP				(0x0363)
#define OPPLE_MSG_STATUS_CONVERSE                   (0x036E)
#define OPPLE_MSG_STATUS_CONVERSE_RSP               (0x036F)
#define OPPLE_MSG_DEV_IDENTIFY                      (0x0370)
#define OPPLE_MSG_DEV_IDENTIFY_RSP                  (0x0371)
#define OPPLE_MSG_ADD_GRP_WITH_WINDOW               (0x0372)
#define OPPLE_MSG_ADD_GRP_WITH_WINDOW_RSP           (0x0373)
#define OPPLE_MSG_STEP_BRIGHT    	                (0x0384)
#define OPPLE_MSG_STEP_BRIGHT_RSP                   (0x0385)
#define OPPLE_MSG_STEP_MODIFY_CCT_SHORT             (0x0388)
#define OPPLE_MSG_SET_SPEC_IFTTT_RULE1              (0x0392)
#define OPPLE_MSG_SET_SPEC_IFTTT_RULE1_RSP          (0x0393)
#define OPPLE_DEV_CONFIG_REPORT                     (0x0396)
#define OPPLE_DEV_CONFIG_REPORT_RSP                 (0x0397)
#define OPPLE_DEV_CONFIG_QUERY                      (0x0398)
#define OPPLE_DEV_CONFIG_QUERY_RSP                  (0x0399)
#define OPPLE_DEV_CONFIG_SETTING                    (0x039A)
#define OPPLE_DEV_CONFIG_SETTING_RSP                (0x039B)
#define OPPLE_DEV_DATAPOINT_OUTPUT                  (0x03A4)
#define OPPLE_DEV_DATAPOINT_OUTPUT_RSP              (0x03A5)
#define OPPLE_DEV_DATAPOINT_CONFIG                  (0x03D4)
#define OPPLE_DEV_DATAPOINT_CONFIG_RSP              (0x03D5)
#define OPPLE_DEV_GROUP_CALL                        (0X03D6)
#define OPPLE_DEV_GROUP_CALL_RSP                    (0X03D7)
#define OPPLE_SIG_STEP_BRIGHT                       (0x03FA)
#define OPPLE_SIG_STEP_BRIGHT_RSP                   (0x03FB)
#define OPPLE_CALL_STAT_QUERY                       (0x03FE)
#define OPPLE_CALL_STAT_RSP                         (0x03FF)

#define OPPLE_MSG_TEMHUM_EVENT_REPORT               (0x0502)
#define OPPLE_MSG_PIR_SENSOR_PARA_CONF              (0x0503)
#define OPPLE_MSG_PIR_SENSOR_PARA_CONF_RSP          (0x0504)
#define OPPLE_MSG_LDR_SENSOR_PARA_CONF              (0x0505)
#define OPPLE_MSG_LDR_SENSOR_PARA_CONF_RSP          (0x0506)
#define OPPLE_MSG_PIR_SENSOR_EVT_REPORT             (0x0507)
#define OPPLE_MSG_LDR_SENSOR_DATA_REPORT            (0x0509)
#define OPPLE_MSG_DOOR_SENSOR_DATA_REPORT           (0x0513)
#define OPPLE_MSG_SMOKE_SENSOR_DATA_REPORT          (0x050F)
#define OPPLE_MSG_GAS_SENSOR_DATA_REPORT            (0x050B)
#define OPPLE_MSG_WATER_SENSOR_DATA_REPORT          (0x050D)
#define OPPLE_MSG_TMP_SENSOR_DATA_REPORT            (0x0515)
#define OPPLE_MSG_HDY_SENSOR_DATA_REPORT            (0x0517)
#define OPPLE_MSG_RESET_REPORT                      (0x051B)
#define OPPLE_MSG_ONLINE_REPORT                     (0x0519)
#define OPPLE_MSG_DOOR_UNLOCK_REPORT                (0x0527)                   
#define OPPLE_MSG_DOOR_LOCK_UNUSUAL_REPORT          (0x0529)
#define OPPLE_MSG_DOOR_LOCK_EVENT_REPORT            NULL//TODO
#define OPPLE_MSG_DOOR_LOCK_USER_ADD                NULL//TODO
#define OPPLE_MSG_DOOR_LOCK_USER_DEL                NULL//TODO
#define OPPLE_MSG_DOOR_LOCK_PWD_ADD                 NULL//TODO
#define OPPLE_MSG_DOOR_LOCK_OPEN                    NULL//TODO
#define OPPLE_MSG_DOOR_LOCK_DISMANTLE_ALARM         NULL//TODO
#define OPPLE_MSG_DOOR_LOCK_ON_REPORT                (0x0571)
#define OPPLE_MSG_DOOR_BELL_REPORT                   (0x0573) 

#define OPPLE_MSG_NODE_ONLINE_REPORT                (0x052D)
// #define OPPLE_MSG_TMP_SENSOR_PARA_CONF              (0x0530)
#define OPPLE_MSG_EVENT_REPORT                      (0x0531)
#define OPPLE_MSG_EVENT_REPORT_RSP                  (0x0532)
// #define OPPLE_MSG_HDY_SENSOR_PARA_CONF              (0x0532)
#define OPPLE_MSG_HDY_SENSOR_PARA_CONF_RSP          (0x0533)
#define OPPLE_MSG_BEACON_LOCATE                     (0x0535)
#define OPPLE_MSG_SCENE_PANEL_KEY_REPORT            (0x0567)               

#define OPPLE_MSG_BATTERY_ALARM_REPORT              (0x0575)

#define OPPLE_MSG_DEV_ST_QUERY                      (0x0601)
#define OPPLE_MSG_MESH_STOP_DATA_TX                 (0x0610)
#define OPPLE_MSG_TERMINATE_MSG                     (0x0666)
#define OPPLE_MSG_TERMINATE_MSG_RSP                 (0x0667)

#define OPPLE_MSG_LIB_VERSION_QRY                   (0x0701)
#define OPPLE_MSG_LIB_VERSION_RSP                   (0x0702)

#define OPPLE_MSG_POWER_REQ                         (0x0800)
#define OPPLE_MSG_POWER_RSP                         (0x0801)

#define OPPLE_MSG_VERSION_REPORT                    NULL//TODO

/*SIG MESH control message*/
#define CMD_CTL_ACK                 				(0x00)
#define CMD_CTL_POLL                				(0x01)	//using the friendship security credentials in V1.0
#define CMD_CTL_UPDATE              				(0x02)	//using the friendship security credentials in V1.0
#define CMD_CTL_REQUEST             				(0x03)	//using the master security credentials in V1.0
#define CMD_CTL_OFFER               				(0x04)	//using the master security credentials in V1.0
#define CMD_CTL_CLEAR               				(0x05)	//using the master security credentials in V1.0
#define CMD_CTL_CLR_CONF            				(0x06)	//using the master security credentials in V1.0
#define CMD_CTL_SUBS_LIST_ADD       				(0x07)	//using the friendship security credentials in V1.0
#define CMD_CTL_SUBS_LIST_REMOVE    				(0x08)	//using the friendship security credentials in V1.0
#define CMD_CTL_SUBS_LIST_CONF      				(0x09)	//using the friendship security credentials in V1.0
#define CMD_CTL_HEARTBEAT							(0x0a)

/*SIG Mesh commands*/
#define HEARTBEAT_PUB_GET		                    (0x3880)
#define HEARTBEAT_PUB_SET		                    (0x3980)
#define HEARTBEAT_SUB_GET				            (0x3A80)
#define HEARTBEAT_SUB_SET				            (0x3B80)
#define HEARTBEAT_SUB_STATUS			            (0x3C80)

// config
#define APPKEY_DEL		                            (0x0080)
#define APPKEY_GET		                            (0x0180)
#define APPKEY_LIST		                            (0x0280)
#define APPKEY_STATUS		                        (0x0380)

#define MODE_APP_BIND		                        (0x3D80)
#define MODE_APP_STATUS		                        (0x3E80)
#define MODE_APP_UNBIND		                        (0x3F80)
#define NETKEY_ADD		                            (0x4080)
#define NETKEY_DEL		                            (0x4180)
#define NETKEY_GET		                            (0x4280)
#define NETKEY_LIST		                            (0x4380)
#define NETKEY_STATUS		                        (0x4480)
#define NETKEY_UPDATE		                        (0x4580)
#define NODE_ID_GET		                            (0x4680)
#define NODE_ID_SET		                            (0x4780)
#define NODE_ID_STATUS		                        (0x4880)
#define NODE_RESET		                            (0x4980)
#define NODE_RESET_STATUS		                    (0x4A80)
#define SIG_MODEL_APP_GET		                    (0x4B80)
#define SIG_MODEL_APP_LIST		                    (0x4C80)
#define VENDOR_MODEL_APP_GET		                (0x4D80)
#define VENDOR_MODEL_APP_LIST		                (0x4E80)

#define SIG_MD_G_ONOFF_S                            (0x1000)
#define SIG_MD_G_ONOFF_C                            (0x1001)
#define SIG_MD_G_LEVEL_S                            (0x1002)
#define SIG_MD_G_LEVEL_C                            (0x1003)
#define SIG_MD_LIGHTNESS_S                          (0x1300)
#define SIG_MD_LIGHTNESS_SETUP_S                    (0x1301)
#define SIG_MD_LIGHTNESS_C                          (0x1302)
#define SIG_MD_LIGHT_CTL_S                          (0x1303)
#define SIG_MD_LIGHT_CTL_SETUP_S                    (0x1304)
#define SIG_MD_LIGHT_CTL_C                          (0x1305)
#define SIG_MD_LIGHT_CTL_TEMP_S                     (0x1306)

// op cmd 0xxxxxxx (SIG)
#define APPKEY_ADD		                            (0x00)
#define APPKEY_UPDATE		                        (0x01)
#define COMPOSITION_DATA_STATUS		                (0x02)
#define CFG_MODEL_PUB_SET		                    (0x03)
#define HEALTH_CURRENT_STATUS		                (0x04)
#define HEALTH_FAULT_STATUS		                    (0x05)
#define HEARTBEAT_PUB_STATUS 			            (0x06)
#define G_SCENE_STATUS                              (0x5E)

#define G_ONOFF_GET		                            (0x0182)
#define G_ONOFF_SET		                            (0x0282)
#define G_ONOFF_SET_NOACK		                    (0x0382)
#define G_ONOFF_STATUS		                        (0x0482)

#define G_LEVEL_GET		                            (0x0582)
#define G_LEVEL_SET		                            (0x0682)
#define G_LEVEL_SET_NOACK		                    (0x0782)
#define G_LEVEL_STATUS		                        (0x0882)
#define G_DELTA_SET		                            (0x0982)
#define G_DELTA_SET_NOACK		                    (0x0A82)
#define G_MOVE_SET		                            (0x0B82)
#define G_MOVE_SET_NOACK		                    (0x0C82)

#define G_DEF_TRANS_TIME_GET		                (0x0D82)
#define G_DEF_TRANS_TIME_SET		                (0x0E82)
#define G_DEF_TRANS_TIME_SET_NOACK		            (0x0F82)
#define G_DEF_TRANS_TIME_STATUS		                (0x1082)

#define G_ON_POWER_UP_GET		                    (0x1182)
#define G_ON_POWER_UP_STATUS		                (0x1282)
#define G_ON_POWER_UP_SET		                    (0x1382)
#define G_ON_POWER_UP_SET_NOACK	                    (0x1482)

#define G_POWER_LEVEL_GET		                    (0x1582)
#define G_POWER_LEVEL_SET		                    (0x1682)
#define G_POWER_LEVEL_SET_NOACK		                (0x1782)
#define G_POWER_LEVEL_STATUS		                (0x1882)
#define G_POWER_LEVEL_LAST_GET		                (0x1982)
#define G_POWER_LEVEL_LAST_STATUS		            (0x1A82)
#define G_POWER_DEF_GET		                        (0x1B82)
#define G_POWER_DEF_STATUS		                    (0x1C82)
#define G_POWER_LEVEL_RANGE_GET		                (0x1D82)
#define G_POWER_LEVEL_RANGE_STATUS		            (0x1E82)
#define G_POWER_DEF_SET		                        (0x1F82)
#define G_POWER_DEF_SET_NOACK		                (0x2082)
#define G_POWER_LEVEL_RANGE_SET		                (0x2182)
#define G_POWER_LEVEL_RANGE_SET_NOACK	            (0x2282)

#define G_BATTERY_GET					            (0x2382)
#define G_BATTERY_STATUS				            (0x2482)

#define G_LOCATION_GLOBAL_GET			            (0x2582)
#define G_LOCATION_GLOBAL_STATUS		            (0x40)
#define G_LOCATION_LOCAL_GET			            (0x2682)
#define G_LOCATION_LOCAL_STATUS			            (0x2782)
#define G_LOCATION_GLOBAL_SET			            (0x41)
#define G_LOCATION_GLOBAL_SET_NOACK		            (0x42)
#define G_LOCATION_LOCAL_SET			            (0x2882)
#define G_LOCATION_LOCAL_SET_NOACK		            (0x2982)

#define SCENE_STORE                                 (0x4682)
#define SCENE_DELETE                                (0x9e82)
#define SCENE_CALL                                  (0x4282)
#define SCENE_STATUS		    		            (0x5E)

#define SCENE_REG_STAUTS                            (0x4582)

#define LIGHTNESS_GET		        	            (0x4B82)
#define LIGHTNESS_SET		        	            (0x4C82)
#define LIGHTNESS_SET_NOACK				            (0x4D82)
#define LIGHTNESS_STATUS		    	            (0x4E82)
#define LIGHTNESS_LINEAR_GET		                (0x4F82)
#define LIGHTNESS_LINEAR_SET		                (0x5082)
#define LIGHTNESS_LINEAR_SET_NOACK		            (0x5182)
#define LIGHTNESS_LINEAR_STATUS		                (0x5282)
#define LIGHTNESS_LAST_GET		    	            (0x5382)
#define LIGHTNESS_LAST_STATUS		                (0x5482)
#define LIGHTNESS_DEFULT_GET		                (0x5582)
#define LIGHTNESS_DEFULT_STATUS		                (0x5682)
#define LIGHTNESS_RANGE_GET		    	            (0x5782)
#define LIGHTNESS_RANGE_STATUS		                (0x5882)
#define LIGHTNESS_DEFULT_SET		                (0x5982)
#define LIGHTNESS_DEFULT_SET_NOACK		            (0x5A82)
#define LIGHTNESS_RANGE_SET		    	            (0x5B82)
#define LIGHTNESS_RANGE_SET_NOACK		            (0x5C82)
#define LIGHT_CTL_GET		        	            (0x5D82)
#define LIGHT_CTL_SET		        	            (0x5E82)
#define LIGHT_CTL_SET_NOACK				            (0x5F82)
#define LIGHT_CTL_STATUS		    	            (0x6082)
#define LIGHT_CTL_TEMP_GET		                    (0x6182)
#define LIGHT_CTL_TEMP_RANGE_GET		            (0x6282)
#define LIGHT_CTL_TEMP_RANGE_STATUS		            (0x6382)
#define LIGHT_CTL_TEMP_SET				            (0x6482)
#define LIGHT_CTL_TEMP_SET_NOACK		            (0x6582)
#define LIGHT_CTL_TEMP_STATUS			            (0x6682)
#define LIGHT_CTL_DEFULT_GET			            (0x6782)
#define LIGHT_CTL_DEFULT_STATUS		                (0x6882)
#define LIGHT_CTL_DEFULT_SET			            (0x6982)
#define LIGHT_CTL_DEFULT_SET_NOACK		            (0x6A82)
#define LIGHT_CTL_TEMP_RANGE_SET		            (0x6B82)
#define LIGHT_CTL_TEMP_RANGE_SET_NOACK	            (0x6C82)

#define FW_INFO_GET						            (0x01B6)
#define FW_INFO_STATUS					            (0x02B6)

//group handle
#define CFG_MODEL_PUB_GET                           (0x1880)
#define CFG_MODEL_PUB_STATUS		                (0x1980)
#define CFG_MODEL_PUB_VIRTUAL_ADR_SET               (0x1A80)
#define CFG_MODEL_SUB_ADD                           (0x1B80)
#define CFG_MODEL_SUB_DEL		                    (0x1C80)
#define CFG_MODEL_SUB_DEL_ALL		                (0x1D80)
#define CFG_MODEL_SUB_OVER_WRITE                    (0x1E80)
#define CFG_MODEL_SUB_STATUS		                (0x1F80)
#define CFG_MODEL_SUB_VIRTUAL_ADR_ADD               (0x2080)
#define CFG_MODEL_SUB_VIRTUAL_ADR_DEL               (0x2180)
#define CFG_MODEL_SUB_VIRTUAL_ADR_OVER_WRITE        (0x2280)
#define CFG_MODEL_SUB_GET                           (0x2980)

/******************************************************************************
                                 SIG MESH VENDOR                                     
******************************************************************************/
/* Sigmesh vendor id */
#define VENDOR_ID_OPPLE                             (0x0539)
#define VENDER_ID_HUAWEI                            (0x027D)

/* Sigmesh vendor opcode */
#define VENDOR_OPCODE_GET                           (0xD0)
#define VENDOR_OPCODE_SET_WITH_ACK                  (0xD1)
#define VENDOR_OPCODE_SET_WITH_NOACK                (0xD2)
#define VENDOR_OPCODE_EVENT_NOTIFY_NOACK            (0xD4)
#define VENDOR_OPCODE_EVENT_NOTIFY_ACK              (0xD6)
#define OPPLE_BT_MESH_REGULAR_VENDOR_SEND           (0xF3)
#define OPPLE_BT_MESH_REGULAR_VENDOR_SEND_UNACKED   (0xF4)

/* Sigmesh vendor attr_type common */
#define VENDOR_ATTR_ERROR_CODE                      (0x0000)
#define VENDOR_ATTR_ALL_STATUS                      (0xE001)
#define VENDOR_ATTR_VER_STATUS             			(0xE005)
#define VENDOR_ATTR_TIME_ZONE                       (0xF01E)
#define VENDOR_ATTR_UNIX_TIME                       (0xF01F)

/* Ifttt */
#define VENDOR_ATTR_IFTTT_ADD                       (0x0279)
#define VENDOR_ATTR_IFTTT_ADD_ACK                   (0x027A)
#define VENDOR_ATTR_IFTTT_DEL                       (0x027B)
#define VENDOR_ATTR_IFTTT_DEL_ACK                   (0x027C)

/* Light */
#define VENDOR_ATTR_LIGHT_COLOUR_MODE               (0xE002)
#define VENDOR_ATTR_LIGHT_COLOUR                    (0xE004)

/* Scene panel */
#define VENDOR_ATTR_SCENE_PANEL_KEY_NUM             (0x0601)

/* Curtain */
#define VENDOR_ATTR_CURTAIN_MODE                    (0x0547)
#define VENDOR_ATTR_CURTAIN_PRESENT_PERCENT         (0xE006)
#define VENDOR_ATTR_CURTAIN_TARGET_PERCENT          (0x0548)

/* Audible and visual alarm */
#define VENDOR_ATTR_AUD_VIS_ALARM_LIGHT_SWITCH      (0x0534)
#define VENDOR_ATTR_AUD_VIS_ALARM_TIME              (0x012D)
#define VENDOR_ATTR_AUD_VIS_ALARM_VOLUME            (0x0109)

/* Sensor alarm */
#define VENDOR_ATTR_SENSOR_DISMANTLE_ALARM          (0xE102)
#define VENDOR_ATTR_SENSOR_ALARM                    (0xE103)

/* PIR sensor */
#define VENDOR_ATTR_PIR_SENSOR_EVENT                (0x0401)
#define VENDOR_ATTR_PIR_SENSOR_STATUS               (0x041F)

/* Door sensor */
#define VENDOR_ATTR_DOOR_SENSOR_EVENT               (0xE101)
#define VENDOR_ATTR_DOOR_SENSOR_STATUS              (0x041F)

/* Temperature and humidity sensor */
#define VENDOR_ATTR_TEM_HUM_SENSOR_TEM              (0x010D)
#define VENDOR_ATTR_TEM_HUM_SENSOR_HUM              (0x010F)
//#define VENDOR_ATTR_TEM_HUM_SENSOR_TEM_LEVEL
//#define VENDOR_ATTR_TEM_HUM_SENSOR_HUM_LEVEL

/* Device battery */
#define VENDOR_ATTR_BATTERY_LEVEL                   (0x0104)
#define VENDOR_ATTR_BATTERY_ALARM                   (0x0549)
//#define VENDOR_ATTR_BATTERY_CAPACITY  
//#define VENDOR_ATTR_BATTERY_CHARGING

/* Door lock */
#define VENDOR_ATTR_DOOR_LOCK_EVENT                 (0xE101)
#define VENDOR_ATTR_DOOR_LOCK_STATUS                (0X041F)
#define VENDOR_ATTR_DOOR_LOCK_OPEN_MODE             (0x0588)
#define VENDOR_ATTR_DOOR_LOCK_USER_ADD              (0x058A)
#define VENDOR_ATTR_DOOR_LOCK_USER_DEL              (0x058B)
#define VENDOR_ATTR_DOOR_LOCK_PWD_ADD               (0x058D)
#define VENDOR_ATTR_DOOR_LOCK_OPEN_EVENT            (0xEE01)    
#define VENDOR_ATTR_DOOR_LOCK_ON_EVENT              (0xEE02)
#define VENDOR_ATTR_DOOR_LOCK_ALARM_EVENT           (0xEE03)
#define VENDOR_ATTR_DOOR_BELL_ALARM_EVENT           (0xEE04)

#define VENDOR_ATTR_DOOR_LOCK_STATUS                (0x041F)

/*opple vendor commands*/
#define G_VENDER_ATTR_IFTTT_ADD         			(0x0279)
#define G_VENDER_ATTR_IFTTT_ADD_ACK     			(0x027A)
#define G_VENDER_ATTR_IFTTT_DEL         			(0x027B)
#define G_VENDER_ATTR_IFTTT_DEL_ACK     			(0x027C)

#define OPPLE_BATTERY_LEVEL_INVALID                 (0xffff)
#define OPPLE_SENSOR_STATUS_INVALID                 (0xff)
#define OPPLE_TEM_STATUS_INVALID                    (0xffff)
#define OPPLE_HUM_STATUS_INVALID                    (0xff)
#define OPPLE_LIGHT_ONOFF_INVALID                   (0xff)
#define OPPLE_LIGHT_LIGHTNESS_INVALID               (0xffff)
#define OPPLE_LIGHT_CCT_INVALID                     (0xffff)
#define OPPLE_TIME_STAMP_INVALID                    (0x0)
#define OPPLE_TIME_ZONE_INVALID                     (0xffff)
#define OPPLE_INVALID_VALUE                         (0xffffffff)

#define KELVIN_TO_CELSIUS_VALUE_X100                (27315)
#define KELVIN_TO_CELSIUS_X100(a)                   ((a) - KELVIN_TO_CELSIUS_VALUE_X100)
#define OPPLE_KELVIN_TO_CELSIUS(a)                  ((int16_t)((KELVIN_TO_CELSIUS_X100(a) + 5) / 10))

/******************************************************************************
                        OPPLE GROUP NUMBER HASH OBJ_ID                                     
******************************************************************************/
#define OPP_LIGHT_GPNB_HASH_OBJ_ID                  (0x05) 
#define OPP_SENSOR_GPNB_HASH_OBJ_ID                 (0x01)
#define OPP_PANEL_GPNB_HASH_OBJ_ID                  (0x09)

#pragma pack(1)
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
typedef struct{
	uint32_t pid;
    uint8_t  data_len;
    uint8_t *data;
} SIG_IN_T;

typedef struct{
    uint16_t addr;
    uint8_t  pack_type;
    uint8_t  data_len;
    uint8_t  data[0];
} OPPLE_OUT_T;

typedef int (*protocol_translate)(SIG_IN_T *in,OPPLE_OUT_T *out,uint8_t buf_len);
typedef int (*opple_prov_ret_sync_callback)(uint8_t *, uint8_t);
typedef int (*opple_prov_version_sync_callback)(uint16_t, uint16_t);

typedef struct {
	uint16_t cid;
	uint8_t  pid;
	uint8_t  product_id[4];
	uint8_t  dev_mac[6];
	uint8_t  feature;
	uint16_t rfu;
} oppBleMeshUUID_T;

typedef struct {
    uint8_t  		 flag;	
    uint16_t  		 unicast_addr;   
	uint8_t   		 sn[6];
	oppBleMeshUUID_T uuid;
	uint8_t	 		 dev_key[16];
	uint8_t   		 devProdId[4];
	uint8_t   		 elements;
	uint16_t  		 version;
    uint8_t           index;
    uint8_t           isLowPower;
    uint8_t           isBind;
    uint8_t           isHeatbeatSub;
    uint8_t           isCtlSub;
	uint8_t   		 reserve;
} app_device_base_info_struct_t;

typedef struct {
	uint16_t class;
	uint16_t sku;
	uint8_t seq_no; //0 means offline
	uint8_t state;
	uint8_t para[8];
} OPPLE_DEV_NOTIFY_EXTEND;

/*opple mesh part*/
typedef struct {
	uint16_t class;
	uint16_t sku;
	uint16_t sourcetype;
	uint8_t bd_addr[8];
	uint32_t version;
	uint8_t desc[16];
	uint16_t objectid;
	uint32_t operid_low;
	uint32_t operid_high;
	uint32_t ipaddr;
	uint16_t udp_port;
	uint8_t switch_onoff;
	uint8_t bright;
	uint8_t ctrltype;
	OPPLE_DEV_NOTIFY_EXTEND notify_ext;
} OPPLE_BOARD_NOTIFY_DATA;

typedef struct {
	uint16_t msg_id;
	uint16_t group_number;
	uint8_t data[1];
} OPPLE_MESH_MSG;

typedef struct {
	uint16_t src_addr;
	uint16_t dst_addr;
	uint8_t msg_body_len;
} OPPLE_SESSION2NET;

typedef struct {
	OPPLE_SESSION2NET net_head;
	OPPLE_MESH_MSG msg_body;
} OPPLE_COMPLETE_T;

typedef struct {
	uint16_t opple_msg_id;
	uint16_t sigmesh_msg_id;
	protocol_translate sigmesh_to_opple;
} OPP_SIG_MAPPINT_T;

typedef struct {
	uint16_t nk_idx;
	uint16_t ak_idx;
	uint8_t retry_cnt;   // only for reliable command
	uint8_t rsp_max;     // only for reliable command
	uint16_t adr_dst;
	uint8_t payload[0];
} SIG_MESH_CMD_T;

typedef struct {
	uint16_t src;
	uint16_t dst;
	uint8_t data[0];
} SIG_MESH_RSP_T;

typedef struct{
	uint8_t iniTTL:7;
	uint8_t rfu:1;
	uint16_t fea;
} SIG_MESH_HB_MSG_T;

typedef struct {
	uint16_t src;
	uint16_t dst;
	SIG_MESH_HB_MSG_T hb_msg;
} SIG_MESH_HEARTBEAT_PKT_T;

typedef struct {
	uint8_t mac[MAC_LEN];
    uint8_t key[KEY_LEN];
} OPPLE_RPT_KEY;

typedef struct {
	uint8_t mac[MAC_LEN];
    uint8_t random[KEY_LEN];
    uint8_t cfm[KEY_LEN];
} OPPLE_VRY_CFM_RANDOM;

typedef struct {
	uint16_t node_id;
	uint8_t bd_addr[6];
	uint16_t sku;
	uint16_t class;
	uint32_t version;
} OPPLE_PROVISION_INFO;
/********************************************************************************************************
                                       OPPLE MESH STATUS ENUM
*********************************************************************************************************/
typedef enum{
	OPPLE_SUCCESS                     = 0,
	OPPLE_FAILED                      = 1,
	OPPLE_ERROR_CODE_EXIST            = 2,
	OPPLE_ERROR_CODE_NON_EXIST        = 3, 
	OPPLE_ERROR_CODE_REPEAT           = 4,
	OPPLE_ERROR_CODE_MEMORY_OVERFLOW  = 5,
	OPPLE_ERROR_CODE_ILLEGAL_USER     = 6,
	OPPLE_ERROR_CODE_NO_PERMISSION    = 7,
	OPPLE_ERROR_CODE_NONSUPPORT       = 18
}OPPLE_ERROR_CODE_T;

typedef enum{
	OPPLE_STATE_NORMAL    = 0,
	OPPLE_STATE_FAULT     = 0x10,
	OPPLE_STATE_LIFE_END  = 0X20,
	OPPLE_STATE_LOW_POWER = 0x30,
	OPPLE_STATE_INVALID   = 0xFF
}OPPLE_STATE_MODE_1_T;

typedef enum {
	OPPLE_EVENT = 0x00,
	OPPLE_STATUS
} OPPLE_MESH_DEV_EVENT_TYPE;

typedef enum {
	OPPLE_GRP_ADD = 0x01,
	OPPLE_GRP_DEL,
	OPPLE_OVERWRITE,
} OPPLE_GRP_FAMILY_TYPE;

typedef enum{
	OPPLE_DOOR_LOCK_MECHANICAL_KEY_OPEN_ALARM = 1,
	OPPLE_DOOR_LOCK_MECHANICAL_KEY_OPEN_ALARM_CLEAR,
	OPPLE_DOOR_LOCK_KEY_ERROR,
	OPPLE_DOOR_LOCK_KEY_ERROR_CLEAR,
	OPPLE_DOOR_LOCK_LOCK_ERROR,
	OPPLE_DOOR_LOCK_LOCK_ERROR_CLEAR,
	OPPLE_DOOR_LOCK_HIJACK_ALARM,
	OPPLE_DOOR_LOCK_HIJACK_ALARM_CLEAR,
	OPPLE_DOOR_LOCK_TAMPER_ALARM,
	OPPLE_DOOR_LOCK_TAMPER_ALARM_CLEAR,
	OPPLE_DOOR_LOCK_FINGERPRINT_ERROR,
	OPPLE_DOOR_LOCK_FINGERPRINT_ERROR_CLEAR,
	OPPLE_DOOR_LOCK_CARD_ERROR,
	OPPLE_DOOR_LOCK_CARD_ERROR_CLEAR,
	OPPLE_DOOR_LOCK_MEMORY_OVERFLOW_ALARM,
	OPPLE_DOOR_LOCK_MEMORY_OVERFLOW_ALARM_CLEAR,
	OPPLE_DOOR_LOCK_BATTERY_ALARM,
	OPPLE_DOOR_LOCK_BATTERY_ALARM_CLEAR,
	OPPLE_DOOR_LOCK_OPEN_LONG_TIME_ALARM,
	OPPLE_DOOR_LOCK_OPEN_LONG_TIME_ALARM_CLEAR,
	OPPLE_DOOR_LOCK_OPEN_ERROR_ALARM,
	OPPLE_DOOR_LOCK_OPEN_ERROR_ALARM_CLEAR,
	OPPLE_DOOR_LOCK_KEYHOLE_OPEN_ALARM,
	OPPLE_DOOR_LOCK_KEYHOLE_OPEN_ALARM_CLEAR
} OPPLE_DOOR_LOCK_ERROR_CODE_T;

/********************************************************************************************************
                                       OPPLE MESH MESSAGE STRUCT
*********************************************************************************************************/
typedef struct {
	uint16_t msg_id;
	uint16_t group_number;
	uint8_t  data[0];
} OPPLE_MESH_MSG_T;

typedef struct {
	uint16_t sku;
	uint16_t groupno;
	uint8_t  state;
	uint8_t  para[0];
} OPPLE_DEV_NOTIFY;

/* Generic */
typedef struct {
	uint8_t opcode;
} OPPLE_GENERIC_RSP_T;

typedef struct {
	uint32_t online;
} OPPLE_DEV_ONLINE_T;

typedef struct{
	uint8_t param;
}OPPLE_DEV_RESET_T;

typedef struct {
	uint8_t status;
} OPPLE_DEV_RESET_STATUS_T;

typedef struct{
	uint32_t version;
} OPPLE_DEV_VERSION_T;

typedef struct{
	uint32_t time_stamp;
	int16_t  time_zone;
} OPPLE_DEV_UNIX_TIME_T;

/* Device battery */
typedef struct{
	uint8_t level;
}OPPLE_DEV_BATTERY_LEVEL_T;

typedef struct{
	uint8_t alarm;
}OPPLE_DEV_BATTERY_ALARM_T;

/* Light */
typedef struct {
	uint16_t cct;
} OPPLE_LIGHT_CCT_T;

typedef struct {
	uint8_t onoff;
} OPPLE_LIGHT_ONOFF_T;

typedef OPPLE_LIGHT_ONOFF_T OPPLE_LIGHT_SWITCH_T;

typedef struct {
	uint8_t lightness;
} OPPLE_LIGHTNESS_T;

typedef struct {
	uint8_t  onoff;
	uint16_t ligntness;
	uint16_t cct;
} OPPLE_LIGHT_NOTIFY_PARA_T;

/* Group */
typedef struct{
	uint16_t group_id;
} OPPLE_GROUP_ADD_T;

typedef struct{
	uint16_t group_id;
} OPPLE_GROUP_DELETE_T;

typedef struct {
	uint32_t group_id;
	uint16_t msg_len;
	uint32_t msg_id;
	uint8_t msg_payload[];
} OPPLE_GRP_CALL_T;

/* Scene */
typedef struct{
	uint32_t scene_id;
} OPPLE_SCENE_STORE_T;

typedef struct{
	uint32_t scene_id;
	uint32_t fade_time;
} OPPLE_SCENE_RECALL_T;

typedef struct{
	uint32_t scene_id;
} OPPLE_SCENE_DELETE_T;

typedef struct{
	uint8_t  status;
	uint16_t number;
	uint16_t scene_id[0];
} OPPLE_SCENE_QUERY_RSP_T;

/* Scene panel */
typedef struct{
	uint8_t key_num;
	uint8_t rsv[3];
} OPPLE_SCENE_PANEL_KEY_T;

typedef struct {
	uint8_t  battery_level;
	uint8_t  rsv[4];
} OPPLE_SCENE_PANEL_NOTIFY_PARA_T;

/* Curtain */
typedef struct{
	uint8_t percent;
}OPPLE_CURTAIN_PERCENT_T;

typedef struct {
	uint8_t  percent;
	uint8_t  degree;
	uint8_t  rsv[3];
} OPPLE_CURTAIN_NOTIFY_PARA_T;

/* PIR sensor */
typedef struct{
	uint8_t status;
}OPPLE_PIR_SENSOR_T;

typedef struct {
	uint8_t  status;
	uint8_t  rsv[4];
} OPPLE_PIR_SENSOR_NOTIFY_PARA_T;

/* Door sendor */
typedef struct{
	uint8_t status;
}OPPLE_DOOR_SENSOR_T;

typedef struct {
	uint8_t  status;
	uint8_t  rsv[4];
} OPPLE_DOOR_SENSOR_NOTIFY_PARA_T;

/* Smoke sendor */
typedef struct{
	uint8_t status;
}OPPLE_SMOKE_SENSOR_T;

typedef struct {
	uint8_t  status;
	uint8_t  rsv[4];
} OPPLE_SMOKE_SENSOR_NOTIFY_PARA_T;

/* GAS sendor */
typedef struct{
	uint8_t status;
}OPPLE_GAS_SENSOR_T;

typedef struct {
	uint8_t  status;
	uint8_t  rsv[4];
} OPPLE_GAS_SENSOR_NOTIFY_PARA_T;

/* Water sendor */
typedef struct{
	uint8_t status;
}OPPLE_WATER_SENSOR_T;

typedef struct {
	uint8_t  status;
	uint8_t  rsv[4];
} OPPLE_WATER_SENSOR_NOTIFY_PARA_T;

/* Temperature and humidity sensor */
typedef struct{
	int16_t temperature;
} OPPLE_TEM_HUM_SENSOR_TEM_T;

typedef struct{
	uint8_t humidity;
} OPPLE_TEM_HUM_SENSOR_HUM_T;

typedef struct{
	int16_t temperature;
	uint8_t humidity;
	uint8_t battery_level;
	uint8_t rsv[1];
} OPPLE_TEM_HUM_SENSOR_NOTIFY_PARAM_T;

/* Door lock */
typedef struct{
	uint8_t event;
} OPPLE_DOOR_LOCK_ON_EVENT_T;

typedef struct{
	uint16_t id;
	uint8_t  type;
} OPPLE_DOOR_LOCK_UNLOCK_EVENT_T;
typedef struct{
	uint8_t  open;
} OPPLE_DOOR_LOCK_OPEN_T;

typedef struct{
	uint16_t id;
	uint8_t  mode;
} OPPLE_DOOR_LOCK_OPEN_MODE_T;

typedef struct{
	uint8_t code;
} OPPLE_DOOR_LOCK_UNUSUAL_CODE_T;

typedef struct{
	uint8_t status;
} OPPLE_DOOR_LOCK_STATUS_T;

typedef struct{
	uint8_t  event;
} OPPLE_DOOR_LOCK_EVENT_T;

typedef struct{
	uint8_t  user_id;
	uint8_t  lock_type;
} OPPLE_DOOR_LOCK_USER_ADD_T;

typedef struct{
	uint8_t  user_id;
	uint8_t  lock_type;
} OPPLE_DOOR_LOCK_USER_DEL_T;

typedef struct{
	uint8_t  len;
	uint8_t  password[0];
} OPPLE_DOOR_LOCK_PWD_ADD_T;

typedef struct{
	uint8_t locked;
	uint8_t battery_level;
} OPPLE_DOOR_LOCK_NOTIFY_PARAM_T;

typedef struct{
	uint8_t state;
	uint8_t battery_level;
	uint8_t rsv[3];
} OPPLE_LP_SENSOR_NOTIFY_PARAM_T;

typedef struct{
	uint8_t  alarm;
} OPPLE_SENSOR_DISMANTLE_ALARM_T;

/* Door bell */
typedef struct{
	uint8_t  event;
} OPPLE_DOOR_BELL_EVENT_T;
/********************************************************************************************************
                                       SIG MESH MESSAGE STRUCT
*********************************************************************************************************/
/* Generic onoff */
typedef struct {
	uint16_t opcode;
	uint8_t  onoff;
	uint8_t  tid;
	uint8_t  trans_time;
	uint8_t  delay;
} SIG_GENERIC_ONOFF_SET_T;

typedef struct {
	uint16_t opcode;
} SIG_GENERIC_ONOFF_GET_T;

typedef struct {
	uint16_t opcode;
	uint8_t  present_onoff;
	uint8_t  target_onoff;
	uint8_t  remain_time;
} SIG_GENERIC_ONOFF_STATUS_T;

/* Light Lightness */
typedef struct {
	uint16_t opcode;
} SIG_LIGHT_LIGHTNESS_GET_T;

typedef struct {
	uint16_t opcode;
	uint16_t lightness;
	uint8_t  tid;
	uint8_t  trans_time;
	uint8_t  delay;
} SIG_LIGHT_LIGHTNESS_SET_T;

typedef struct {
	uint16_t opcode;
	uint16_t present_lightness;
	uint16_t target_lightness;
	uint8_t  reamin_time;
} SIG_LIGHT_LIGHTNESS_STATUS_T;

/* Light CTL temperature */
typedef struct {
	uint16_t opcode;
} SIG_LIGHT_CTL_TEMP_GET_T;

typedef struct {
	uint16_t opcode;
	uint16_t temperature;
	uint16_t delta_uv;
	uint8_t  tid;
	uint8_t  trans_time;
	uint8_t  delay;
} SIG_LIGHT_CTL_TEMP_SET_T;

typedef struct {
	uint16_t opcode;
	uint16_t present_temp;
	uint16_t present_delta_uv;
	uint16_t target_temp;
	uint16_t target_delta_uv;
	uint8_t  ramin_time;
} SIG_LIGHT_CTL_TEMP_STATUS_T;

/* Light CTL */
typedef struct {
	uint16_t opcode;
} SIG_LIGHT_CTL_GET_T;

typedef struct {
	uint16_t opcode;
	uint16_t lightness;
	uint16_t temperature;
	uint16_t delta_uv;
	uint8_t  tid;
	uint8_t  trans_time;
	uint8_t  delay;
} SIG_LIGHT_CTL_SET_T;

typedef struct {
	uint16_t opcode;
	uint16_t present_lightness;
	uint16_t present_temp;
	uint16_t target_lightness;
	uint16_t target_temp;
	uint8_t  ramin_time;
} SIG_LIGHT_CTL_STATUS_T;

/* Scene */
typedef struct {
	uint16_t opcode;
	uint16_t scene_id;
} SIG_SCENE_STORE_T;

typedef struct {
	uint16_t opcode;
	uint16_t scene_id;
	uint8_t  tid;
	uint8_t  trans_time;
	uint8_t  delay;
} SIG_SCENE_RECALL_T;

typedef struct {
	uint16_t opcode;
} SIG_SCENE_GET_T;

typedef struct {
	uint16_t opcode;
	uint16_t scene_id;
} SIG_SCENE_DELETE_T;

typedef struct {
	uint8_t opcode;
	uint8_t  status;
	uint16_t current_scene;
	uint16_t target_scene;
	uint8_t  reamin_time;
} SIG_SCENE_STATUS_T;

typedef struct {
	uint16_t opcode;
} SIG_SCENE_REGISTER_GET_T;

typedef struct {
	uint16_t opcode;
	uint8_t  status;
	uint16_t current_scene;
	uint16_t scene_payload[0];
} SIG_SCENE_REGISTER_STATUS_T;

/* Config model app */
typedef struct {
	uint16_t opcode;
	uint16_t element_address;
	uint16_t app_key_index;
	uint16_t identifier; //SIG MODEL ID
} SIG_CFG_MODEL_APP_BIND_T;

typedef struct {
	uint16_t opcode;
	uint16_t element_address;
	uint16_t app_key_index;
	uint16_t identifier; //SIG MODEL ID
} SIG_CFG_MODEL_APP_UNBIND_T;

typedef struct{
	uint16_t opcode;
	uint8_t  status;
	uint16_t element_address;
	uint16_t app_key_index;
	uint16_t indentifier; //SIG Model ID
} SIG_CFG_MODEL_APP_STATUS_T;

/* Config model subscription */
typedef struct {
	uint16_t opcode;
	uint16_t element_address;
	uint16_t address;
	uint16_t indentifier; //SIG MODEL ID
} SIG_CFG_MODEL_SUB_ADD_T;

typedef struct {
	uint16_t opcode;
	uint16_t element_address;
	uint16_t address;
	uint16_t indentifier; //SIG MODEL ID
} SIG_CFG_MODEL_SUB_DELETE_T;

typedef struct {
	uint16_t opcode;
	uint16_t element_address;
	uint16_t address;
	uint16_t indentifier; //SIG MODEL ID
} SIG_CFG_MODEL_SUB_OVERWRITE_T;

typedef struct {
	uint16_t opcode;
	uint16_t element_address;
	uint16_t indentifier; //SIG MODEL ID
} SIG_CFG_MODEL_SUB_GET_T;

typedef struct {
	uint16_t opcode;
	uint8_t  status;
	uint16_t element_address;
	uint16_t address;
	uint16_t indentifier; //SIG MODEL ID
} SIG_CFG_MODEL_SUB_STATUS_T;

/* Config model publication */
typedef struct {
	uint16_t opcode;
	uint16_t element_address;
	uint16_t indentifier; //SIG MODEL ID
} SIG_CFG_MODEL_PUB_GET_T;

typedef struct {
	uint8_t opcode;
	uint16_t element_address;
	uint16_t publish_address;
	uint16_t app_key_index      : 12;
	uint16_t credential_flag    : 1;
	uint16_t rfu                : 3;
	uint8_t  ttl;
	uint8_t  period;
	uint8_t  retrans_cnt        : 3;
	uint8_t  retrans_inter_step : 5;
	uint16_t indentifier; //SIG MODEL ID
} SIG_CFG_MODEL_PUB_SET_T;

typedef struct {
	uint16_t opcode;
	uint8_t  status;
	uint16_t element_address;
	uint16_t publish_address;
	uint16_t app_key_index      : 12;
	uint16_t credential_flag    : 1;
	uint16_t rfu                : 3;
	uint8_t  ttl;
	uint8_t  period;
	uint8_t  retrans_cnt        : 3;
	uint8_t  retrans_inter_step : 5;
	uint16_t indentifier; //SIG MODEL ID
} SIG_CFG_MODEL_PUB_STATUS_T;

/* Config app key */
typedef struct{
	uint8_t opcode;
	uint8_t net_app_key_index[3];
	uint8_t key[16];
} SIG_CFG_APP_KEY_ADD_T;

typedef struct{
	uint16_t opcode;
	uint16_t key_index;
} SIG_CFG_APP_KEY_GET_T;

typedef struct{
	uint16_t opcode;
	uint8_t  net_app_key_index[3];
} SIG_CFG_APP_KEY_DELETE_T;

typedef struct{
	uint16_t opcode;
	uint8_t  status;
	uint8_t  net_app_key_index[3];
} SIG_CFG_APP_KEY_STATUS_T;

/* Config node reset */
typedef struct{
	uint16_t opcode;
} SIG_CFG_NODE_RESET_T;

typedef struct{
	uint16_t opcode;
} SIG_CFG_NODE_RESET_STATUS_T;

/* Config heartbeat */
typedef struct{
	uint16_t opcode;
	uint16_t destination;
	uint8_t  count_log;
	uint8_t  period_log;
	uint8_t  ttl;
	uint16_t features;
	uint16_t net_key_index;
} SIG_CFG_HEARTBEAT_PUB_SET_T;

/* Node state get */
typedef struct{
	uint16_t opcode;
} SIG_NODE_STATUS_GET_T;

/********************************************************************************************************
                    					VENDOR MESSAGE STRUCT
*********************************************************************************************************/
/* Vendor head */
typedef struct{
	uint8_t  opcode;
	uint16_t vendor_id;
	uint8_t  tid;
	uint8_t  data[0];
} SIG_VENDOR_MSG_T;

/* Vendor common */
typedef struct{
	uint16_t attr_type;
	uint8_t  code;
} VENDOR_DEV_ERROR_CODE_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  ver_num[5];
} VENDOR_DEV_VERSION_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  time_zone;
} VENDOR_DEV_TIME_ZONE_T;

typedef struct{
	uint16_t attr_type;
	uint32_t time_stamp;
} VENDOR_DEV_UNIX_TIME_T;

/* Light - colour mode */
typedef struct{
	uint16_t attr_type;
} VENDOR_LIGHT_COLOUR_MODE_GET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  mode;
} VENDOR_LIGHT_COLOUR_MODE_STATUS_T;

/* Light - colour */
typedef struct{
	uint16_t attr_type;
} VENDOR_LIGHT_COLOUR_GET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  red;
	uint8_t  green;
	uint8_t  blue;
} VENDOR_LIGHT_COLOUR_SET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  red;
	uint8_t  green;
	uint8_t  blue;
} VENDOR_LIGHT_COLOUR_STATUS_T;

/* Scene panel */
typedef struct{
	uint16_t attr_type;
	uint8_t  key_num;
	uint8_t  rsv[3];
} VENDOR_SCENE_PANEL_KEY_NUM_STATUS_T;

/* Curtain - mode */
typedef struct{
	uint16_t attr_type;
} VENDOR_CURTAIN_MODE_GET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  mode;
} VENDOR_CURTAIN_MODE_SET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  mode;
} VENDOR_CURTAIN_MODE_STATUS_T;

/* Curtain - present percent */
typedef struct{
	uint16_t attr_type;
} VENDOR_CURTAIN_PRESENT_PERCENT_GET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  percent;
} VENDOR_CURTAIN_PRESENT_PERCENT_STATUS_T;

/* Curtain - target percent */
typedef struct{
	uint16_t attr_type;
} VENDOR_CURTAIN_TARGET_PERCENT_GET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  percent;
} VENDOR_CURTAIN_TARGET_PERCENT_SET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  percent;
} VENDOR_CURTAIN_TARGET_PERCENT_STATUS_T;

/* Audible and visual alarm - light switch*/
typedef struct{
	uint16_t attr_type;
} VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_GET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  on_off;
} VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_SET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  on_off;
} VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_STATUS_T;

/* Audible and visual alarm - time */
typedef struct{
	uint16_t attr_type;
} VENDOR_AUD_VIS_ALARM_TIME_GET_T;

typedef struct{
	uint16_t attr_type;
	uint16_t time;
} VENDOR_AUD_VIS_ALARM_TIME_SET_T;

typedef struct{
	uint16_t attr_type;
	uint16_t time;
} VENDOR_AUD_VIS_ALARM_TIME_STATUS_T;

/* Audible and visual alarm - volume */
typedef struct{
	uint16_t attr_type;
} VENDOR_AUD_VIS_ALARM_VOLUME_GET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  volume;
} VENDOR_AUD_VIS_ALARM_VOLUME_SET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  volume;
} VENDOR_AUD_VIS_ALARM_VOLUME_STATUS_T;

/* Sensor alarm */
typedef struct{
	uint16_t attr_type;
	uint8_t  alarm;
} VENDOR_SENSOR_DISMANTLE_ALARM_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  alarm;
} VENDOR_SENSOR_ALARM_STATUS_T;

/* PIR sensor - event */
typedef struct{
	uint16_t attr_type;
	uint8_t  event;
} VENDOR_PIR_SENSOR_EVENT_STATUS_T;

/* PIR sensor - status */
typedef struct{
	uint16_t attr_type;
} VENDOR_PIR_SENSOR_STATUS_GET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  status;
} VENDOR_PIR_SENSOR_STATUS_T;

/* Door sensor - event */
typedef struct{
	uint16_t attr_type;
	uint8_t  event;
} VENDOR_DOOR_SENSOR_EVENT_STATUS_T;

/* Door sensor - status */
typedef struct{
	uint16_t attr_type;
} VENDOR_DOOR_SENSOR_STATUS_GET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  status;
} VENDOR_DOOR_SENSOR_STATUS_T;

/* Temperature and humidity sensor - temperature */
typedef struct{
	uint16_t attr_type;
} VENDOR_TEM_HUM_SENSOR_TEM_GET_T;

typedef struct{
	uint16_t attr_type;
	/* kelvin */
	uint16_t temperature;
} VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  level;
} VENDOR_TEM_HUM_SENSOR_TEM_LEVEL_STATUS_T;

/* Temperature and humidity sensor - humidity */
typedef struct{
	uint16_t attr_type;
} VENDOR_TEM_HUM_SENSOR_HUM_GET_T;

typedef struct{
	uint16_t attr_type;
	uint16_t humidity;
} VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  level;
} VENDOR_TEM_HUM_SENSOR_HUM_LEVEL_STATUS_T;

/* Water Sensor */
typedef struct{
	uint16_t attr_type;
	uint8_t  alarm;
} VENDOR_WATER_SENSOR_ALARM_STATUS_T;

/* Device battery - level */
typedef struct{
	uint16_t attr_type;
} VENDOR_DEV_BATTERY_LEVEL_GET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  level;
} VENDOR_DEV_BATTERY_LEVEL_STATUS_T;

/* Device battery - alarm */
typedef struct{
	uint16_t attr_type;
} VENDOR_DEV_BATTERY_ALARM_GET_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  alarm;
} VENDOR_DEV_BATTERY_ALARM_STATUS_T;

/* Device battery - capacity */
typedef struct{
	uint16_t attr_type;
	uint16_t capacity;
} VENDOR_DEV_BATTERY_CAPACITY_STATUS_T;

/* Device battery - charging */
typedef struct{
	uint16_t attr_type;
	uint8_t  charging;
} VENDOR_DEV_BATTERY_CHARGING_STATUS_T;

typedef struct {
	uint16_t attr_type;
	uint16_t software_ver;
	uint16_t firmware_ver;
	uint8_t hardware_ver;
} VENDOR_DEV_VERSION_STATUS_T;

/* Door lock */
typedef struct{
	uint16_t attr_type;
	uint8_t  status;
} VENDOR_DOOR_LOCK_STATUS_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  event;
} VENDOR_DOOR_LOCK_ON_EVENT_T;

typedef struct{
	uint16_t attr_type;
	uint16_t id;
	uint8_t  type;
} VENDOR_DOOR_UNLOCK_EVENT_T;
typedef struct{
	uint16_t attr_type;
	uint8_t  event;
} VENDOR_DOOR_LOCK_EVENT_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  alarm;
} VENDOR_DOOR_LOCK_ALARM_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  user_id;
	uint8_t  lock_type;
} VENDOR_DOOR_LOCK_USER_ADD_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  user_id;
	uint8_t  lock_type;
} VENDOR_DOOR_LOCK_USER_DEL_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  len;
	uint8_t  password[0];
} VENDOR_DOOR_LOCK_PWD_ADD_T;

typedef struct{
	uint16_t attr_type;
	uint8_t  mode;
} VENDOR_DOOR_LOCK_OPEN_MODE_T;

/* Door bell */
typedef struct{
	uint16_t attr_type;
	uint8_t  alarm;
} VENDOR_DOOR_BELL_ALARM_T;
/********************************************************************************************************
                    						OPPLE IFTTT 
*********************************************************************************************************/
typedef struct{
	uint8_t  data_len;
	uint16_t msg_id;
	uint16_t ifttt_id;
	uint8_t  data[0];
}OPPLE_IFTTT_ADD_T;

typedef struct{
	uint16_t msg_id;
	uint16_t ifttt_id;
}OPPLE_IFTTT_DELETE_T;

/********************************************************************************************************
 ********************************************************************************************************/


typedef struct {
	uint8_t SN[SN_LEN];		//10
	uint32_t Product_ID;
} opple_dev_scan_info_t;

typedef struct {
	uint16_t msg_id;
	uint16_t group_number;

	uint32_t ItemNum;
	opple_dev_scan_info_t data[50];
} OPPLE_BLE_DEV_SCAN_INFO_T;

#pragma pack()

/* opple put */
int opple_put_switch(uint16_t addr, const OPPLE_LIGHT_ONOFF_T *in);
int opple_put_lightness(uint16_t addr, const OPPLE_LIGHTNESS_T *in);
int opple_put_cct(uint16_t addr, const OPPLE_LIGHT_CCT_T *in);
/*opple put group operation*/
int opple_grp_config(uint16_t addr, OPPLE_GRP_FAMILY_TYPE grp_opr_type, uint8_t grp_no);
int opple_put_grp_switch(uint8_t grp_no, const OPPLE_LIGHT_SWITCH_T *in);
int opple_put_grp_lightness(uint8_t grp_no, const OPPLE_LIGHTNESS_T *in);
int opple_put_grp_cct(uint8_t grp_no, const OPPLE_LIGHT_CCT_T *in);
/*opple put scene operation*/
int opple_put_scene_store(uint16_t addr, OPPLE_SCENE_STORE_T *in);
int opple_put_scene_recall(uint16_t addr, OPPLE_SCENE_RECALL_T *in);
int opple_put_scene_delete(uint16_t addr, OPPLE_SCENE_DELETE_T *in);

int opple_scan_result(uint8_t mac[6], uint32_t prod_id,uint8_t num);
int opple_confirmkey_report(uint8_t mac[6], uint8_t confirm_key[16]);
int opple_confirm_report(uint8_t mac[6], uint8_t dev_confirm[16], uint8_t dev_random[16]);
int opple_prov_ret_sync(uint8_t mac[6], uint8_t error_code);
int opple_prov_version_sync(uint16_t version, uint16_t addr);

int opple_cloud_confirmation_random_callback(const char *mac, const char *cloudConfirmation, const char *cloudRandom);
int opple_cloud_provision_result_callback(const char *mac, int result);
int provision_result_register(opple_prov_ret_sync_callback callback);
int provision_version_register(opple_prov_version_sync_callback callback);

/*******************************************************************************************************
                                    SIG MESH MESSAHE - PACK/UNPACK
********************************************************************************************************/
/* Sigmesh gengeric onoff */
int sigmesh_generic_onoff_set_pack(uint16_t addr, const SIG_GENERIC_ONOFF_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_generic_onoff_status_unpack(const uint8_t *in,uint8_t in_len, uint16_t *addr, SIG_GENERIC_ONOFF_STATUS_T *out);

/* Sigmesh light - lightness */
int sigmesh_light_lightness_set_pack(uint16_t addr, const SIG_LIGHT_LIGHTNESS_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_light_lightness_status_unpack(const uint8_t *in,uint8_t in_len,uint16_t *addr, SIG_LIGHT_LIGHTNESS_STATUS_T *out);

/* Sigmesh light - CTL temperature */
int sigmesh_light_ctl_temp_set_pack(uint16_t addr, const SIG_LIGHT_CTL_TEMP_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_light_ctl_temp_status_unpack(const uint8_t *in,uint8_t in_len,uint16_t *addr, SIG_LIGHT_CTL_TEMP_STATUS_T *out);

/* Sigmesh light - CTL */
int sigmesh_light_ctl_set_pack(uint16_t addr, const SIG_LIGHT_CTL_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_light_ctl_status_unpack(const uint8_t *in,uint8_t in_len, uint16_t *addr, SIG_LIGHT_CTL_STATUS_T *out);

/* Sigmesh scene */
int sigmesh_scene_store_pack(uint16_t addr, const SIG_SCENE_STORE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_scene_recall_pack(uint16_t addr, const SIG_SCENE_RECALL_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_scene_delete_pack(uint16_t addr, const SIG_SCENE_DELETE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_scene_status_unpack(const uint8_t *in,uint8_t in_len, uint16_t *addr, SIG_SCENE_STATUS_T *out);
int sigmesh_scene_reg_status_unpack(const uint8_t *in,uint8_t in_len, uint16_t *addr, SIG_SCENE_REGISTER_STATUS_T *out);

/* Sigmesh config model app */
int sigmesh_cfg_model_app_bind_pack(uint16_t addr, const SIG_CFG_MODEL_APP_BIND_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_model_app_unbind_pack(uint16_t addr, const SIG_CFG_MODEL_APP_UNBIND_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_model_app_status_unpack(const uint8_t *in,uint8_t in_len, uint16_t *addr, SIG_CFG_MODEL_APP_STATUS_T *out);

/* Sigmesh config model subscription */
int sigmesh_cfg_model_sub_add_pack(uint16_t addr, const SIG_CFG_MODEL_SUB_ADD_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_model_sub_delete_pack(uint16_t addr, const SIG_CFG_MODEL_SUB_DELETE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_model_sub_overwrite_pack(uint16_t addr, const SIG_CFG_MODEL_SUB_OVERWRITE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_model_sub_get_pack(uint16_t addr, const SIG_CFG_MODEL_SUB_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_model_sub_status_unpack(const uint8_t *in,uint8_t in_len, uint16_t *addr, SIG_CFG_MODEL_SUB_STATUS_T *out);

/* Sigmesh config model publication */
int sigmesh_cfg_model_pub_set_pack(uint16_t addr, const SIG_CFG_MODEL_PUB_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_model_pub_get_pack(uint16_t addr, const SIG_CFG_MODEL_PUB_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_model_pub_status_unpack(const uint8_t *in,uint8_t in_len, uint16_t *addr, SIG_CFG_MODEL_PUB_STATUS_T *out);

/* Sigmesh config model app key */
int sigmesh_cfg_model_app_key_add_pack(uint16_t addr, const SIG_CFG_APP_KEY_ADD_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_model_app_key_get_pack(uint16_t addr, const SIG_CFG_APP_KEY_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_model_app_key_delete_pack(uint16_t addr, const SIG_CFG_APP_KEY_DELETE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_app_key_status_unpack(const uint8_t *in,uint8_t in_len, uint16_t *addr, SIG_CFG_APP_KEY_STATUS_T *out);

/* Sigmesh config node reset */
int sigmesh_cfg_node_reset_pack(uint16_t addr, const SIG_CFG_NODE_RESET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_cfg_node_reset_status_unpack(const uint8_t *in,uint8_t in_len, uint16_t *addr, SIG_CFG_NODE_RESET_STATUS_T *out);

/* Sigmesh config heartbeat */
int sigmesh_cfg_heartbeat_pub_set_pack(uint16_t addr, const SIG_CFG_HEARTBEAT_PUB_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);

/* Sigmesh node */
int sigmesh_node_status_get_pack(uint16_t addr, const SIG_NODE_STATUS_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_node_g_onoff_get_pack(uint16_t addr, const SIG_GENERIC_ONOFF_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_node_lightness_get_pack(uint16_t addr, const SIG_LIGHT_LIGHTNESS_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_node_ctl_temp_get_pack(uint16_t addr, const SIG_LIGHT_CTL_TEMP_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_node_ctl_get_pack(uint16_t addr, const SIG_LIGHT_CTL_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
/* opple ifttt */
int sigmesh_opple_ifttt_add_pack(uint16_t addr, const OPPLE_IFTTT_ADD_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_opple_ifttt_delete_pack(uint16_t addr, const OPPLE_IFTTT_DELETE_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
/********************************************************************************************************
                                    SIG MESH VENDOR - PACK/UNPACK
********************************************************************************************************/
/* Sigmesh vendor light - colour mode */
int sigmesh_vendor_light_colour_mode_get_pack(uint16_t addr, const VENDOR_LIGHT_COLOUR_MODE_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_light_colour_mode_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_LIGHT_COLOUR_MODE_STATUS_T *out);

/* Sigmesh vendor light - colour */
int sigmesh_vendor_light_colour_get_pack(uint16_t addr, const VENDOR_LIGHT_COLOUR_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_light_colour_set_pack(uint16_t addr, const VENDOR_LIGHT_COLOUR_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_light_colour_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_LIGHT_COLOUR_STATUS_T *out);

/* Sigmesh vendor scene panel */
int sigmesh_vendor_scene_panel_key_num_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_SCENE_PANEL_KEY_NUM_STATUS_T *out);

/* Sigmesh vendor curtain - mode */
int sigmesh_vendor_curtain_mode_get_pack(uint16_t addr, const VENDOR_CURTAIN_MODE_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_curtain_mode_set_pack(uint16_t addr, const VENDOR_CURTAIN_MODE_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_curtain_mode_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_CURTAIN_MODE_STATUS_T *out);

/* Sigmesh vendor curtain - present percent */
int sigmesh_vendor_curtain_present_percent_get_pack(uint16_t addr, const VENDOR_CURTAIN_PRESENT_PERCENT_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_curtain_present_percent_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_CURTAIN_PRESENT_PERCENT_STATUS_T *out);

/* Sigmesh vendor curtain - target percent */
int sigmesh_vendor_curtain_target_percent_get_pack(uint16_t addr, const VENDOR_CURTAIN_TARGET_PERCENT_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_curtain_target_percent_set_pack(uint16_t addr, const VENDOR_CURTAIN_TARGET_PERCENT_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_curtain_target_percent_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_CURTAIN_TARGET_PERCENT_STATUS_T *out);

/* Sigmesh vendor audible and visual alarm - light switch */
int sigmesh_vendor_aud_vis_alarm_light_switch_get_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_aud_vis_alarm_light_switch_set_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_aud_vis_alarm_light_switch_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_STATUS_T *out);

/* Sigmesh vendor audible and visual alarm - time */
int sigmesh_vendor_aud_vis_alarm_time_get_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_TIME_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_aud_vis_alarm_time_set_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_TIME_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_aud_vis_alarm_time_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_AUD_VIS_ALARM_TIME_STATUS_T *out);

/* Sigmesh vendor audible and visual alarm - volume */
int sigmesh_vendor_aud_vis_alarm_volume_get_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_VOLUME_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_aud_vis_alarm_volume_set_pack(uint16_t addr, const VENDOR_AUD_VIS_ALARM_VOLUME_SET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_aud_vis_alarm_volume_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_AUD_VIS_ALARM_VOLUME_STATUS_T *out);

/* Sigmesh vendor sensor alarm */
int sigmesh_vendor_sensor_dismantle_alarm_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_SENSOR_DISMANTLE_ALARM_T *out);
int sigmesh_vendor_sensor_alarm_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_SENSOR_ALARM_STATUS_T *out);

/* Sigmesh vendor PIR sensor - event */
int sigmesh_vendor_pir_sensor_event_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_PIR_SENSOR_EVENT_STATUS_T *out);

/* Sigmesh vendor PIR sensor - status */
int sigmesh_vendor_pir_sensor_status_get_pack(uint16_t addr, const VENDOR_PIR_SENSOR_STATUS_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_pir_sensor_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_PIR_SENSOR_STATUS_T *out);

/* Sigmesh vendor door sensor - event */
int sigmesh_vendor_door_sensor_event_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DOOR_SENSOR_EVENT_STATUS_T *out);

/* Sigmesh vendor door sensor - status */
int sigmesh_vendor_door_sensor_status_get_pack(uint16_t addr, const VENDOR_DOOR_SENSOR_STATUS_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_door_sensor_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DOOR_SENSOR_STATUS_T *out);

/* Sigmesh vendor temperature and humidity sensor - temperature */
int sigmesh_vendor_tem_hum_sensor_tem_get_pack(uint16_t addr, const VENDOR_TEM_HUM_SENSOR_TEM_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_tem_hum_sensor_tem_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T *out);
int sigmesh_vendor_tem_hum_sensor_tem_level_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_TEM_HUM_SENSOR_TEM_LEVEL_STATUS_T *out);

/* Sigmesh vendor temperature and humidity sensor - humidity */
int sigmesh_vendor_tem_hum_sensor_hum_get_pack(uint16_t addr, const VENDOR_TEM_HUM_SENSOR_HUM_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_tem_hum_sensor_hum_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T *out);
int sigmesh_vendor_tem_hum_sensor_hum_level_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_TEM_HUM_SENSOR_HUM_LEVEL_STATUS_T *out);

/* Sigmesh vendor water sensor */
int sigmesh_vendor_water_sensor_alarm_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_WATER_SENSOR_ALARM_STATUS_T *out);

/* Sigmesh vendor device battery - level */
int sigmesh_vendor_dev_battery_level_get_pack(uint16_t addr, const VENDOR_DEV_BATTERY_LEVEL_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_dev_battery_level_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DEV_BATTERY_LEVEL_STATUS_T *out);

/* Sigmesh vendor device battery - alarm */
int sigmesh_vendor_dev_battery_alarm_get_pack(uint16_t addr, const VENDOR_DEV_BATTERY_ALARM_GET_T *input, uint8_t *output, uint16_t out_len, uint16_t *total_len);
int sigmesh_vendor_dev_battery_alarm_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DEV_BATTERY_ALARM_STATUS_T *out);

/* Sigmesh vendor device battery - capacity */
int sigmesh_vendor_dev_battery_capacity_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DEV_BATTERY_CAPACITY_STATUS_T *out);

/* Sigmesh vendor device battery - charging */
int sigmesh_vendor_dev_battery_charging_status_unpack(const uint8_t *in,uint8_t in_len,VENDOR_DEV_BATTERY_CHARGING_STATUS_T *out);

/*sigmesh to opple interfaces*/
int sig_2_opple_heartbeat_status_func(const uint8_t *in, uint8_t *out, uint8_t buffer_len, uint16_t *out_len, uint8_t *addr, uint8_t *pack_type);
int sig_2_opple_online_status_func(uint32_t online, uint8_t *out, uint8_t buffer_len, uint16_t *out_len, uint8_t *pack_type);
/*vendor version status*/
int sigmesh_vendor_dev_version_status_unpack(const uint8_t *in,uint8_t in_len, VENDOR_DEV_VERSION_STATUS_T *out);

int sig_2_opple_protocol_convert(SIG_IN_T *in, void *out, uint8_t buf_len, int index);
#ifdef __cplusplus
}
#endif

#endif


