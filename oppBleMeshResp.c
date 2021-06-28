/******************************************************************************
* Version     : OPP_IOTCTRL V100R001C01B001D001                               *
* File        : oppBleMeshResp.c                                              *
* Description :                                                               *
*               OPPLE APP层 mesh response 处理源文件                           *
* Note        : (none)                                                        *
* Author      : 智能控制研发部                                                 *
* Date:       : 2020-09-09                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
//#include "oppBleMeshResp.h"
#include "opple_sigmesh_application.h"
/******************************************************************************
*                                Defines                                      *
******************************************************************************/

/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/

/******************************************************************************
*                                Globals                                      *
******************************************************************************/
#if 0
typedef uint8_t (* app_mesh_vendor_msg_pro_t)(uint16_t srcAddr,uint8_t *msg,uint8_t len);

typedef struct{
    uint16_t attr_type;
    app_mesh_vendor_msg_pro_t msg_pro_func;
}MESH_VENDOR_MSG_CONSOLE_T;

static uint8_t app_mesh_vendor_light_colour_mode_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_LIGHT_COLOUR_MODE_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_LIGHT_COLOUR_MODE_STATUS_T light_colour_mode;
    sigmesh_vendor_light_colour_mode_status_unpack(msg,&light_colour_mode);

    jx_hal_log_notice("light_colour_mode: %d", light_colour_mode.mode);  
    return sizeof(VENDOR_LIGHT_COLOUR_MODE_STATUS_T);
}

static uint8_t app_mesh_vendor_light_colour_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_LIGHT_COLOUR_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_LIGHT_COLOUR_STATUS_T light_colour;
    sigmesh_vendor_light_colour_status_unpack(msg,&light_colour);

    jx_hal_log_notice("light_colour blue: %d,green: %d,red: %d", light_colour.blue,light_colour.green,light_colour.red);  
    return sizeof(VENDOR_LIGHT_COLOUR_STATUS_T);
}

/* Vendor scene panel key number status message process */
static uint8_t app_mesh_vendor_scene_panel_key_num_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_SCENE_PANEL_KEY_NUM_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_SCENE_PANEL_KEY_NUM_STATUS_T scene_panel_key_num;
    sigmesh_vendor_scene_panel_key_num_status_unpack(msg,&scene_panel_key_num);

    jx_hal_log_notice("scene_panel_key_num: %d", scene_panel_key_num.key_num);  
    return sizeof(VENDOR_SCENE_PANEL_KEY_NUM_STATUS_T);
}

/* Vendor curtain status message process */
static uint8_t app_mesh_vendor_curtain_mode_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_CURTAIN_MODE_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_CURTAIN_MODE_STATUS_T curtain_mode;
    sigmesh_vendor_curtain_mode_status_unpack(msg,&curtain_mode);

    jx_hal_log_notice("curtain_mode: %d", curtain_mode.mode);  
    return sizeof(VENDOR_CURTAIN_MODE_STATUS_T);
}

static uint8_t app_mesh_vendor_curtain_present_percent_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_CURTAIN_PRESENT_PERCENT_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_CURTAIN_PRESENT_PERCENT_STATUS_T curtain_present_percent;
    sigmesh_vendor_curtain_present_percent_status_unpack(msg,&curtain_present_percent);

    jx_hal_log_notice("curtain_present_percent: %d", curtain_present_percent.percent);  
    return sizeof(VENDOR_CURTAIN_PRESENT_PERCENT_STATUS_T);
}

static uint8_t app_mesh_vendor_curtain_target_percent_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_CURTAIN_TARGET_PERCENT_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_CURTAIN_TARGET_PERCENT_STATUS_T curtain_target_percent;
    sigmesh_vendor_curtain_target_percent_status_unpack(msg,&curtain_target_percent);

    jx_hal_log_notice("curtain_target_percent: %d", curtain_target_percent.percent);  
    return sizeof(VENDOR_CURTAIN_TARGET_PERCENT_STATUS_T);
}

/* Vendor audible and visual alarm status message process */
static uint8_t app_mesh_vendor_aud_vis_alarm_light_switch_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_STATUS_T aud_vis_alarm_light_switch;
    sigmesh_vendor_aud_vis_alarm_light_switch_status_unpack(msg,&aud_vis_alarm_light_switch);

    jx_hal_log_notice("aud_vis_alarm_light_switch: %d", aud_vis_alarm_light_switch.on_off);  
    return sizeof(VENDOR_AUD_VIS_ALARM_LIGHT_SWITCH_STATUS_T);
}

static uint8_t app_mesh_vendor_aud_vis_alarm_time_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_AUD_VIS_ALARM_TIME_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_AUD_VIS_ALARM_TIME_STATUS_T aud_vis_alarm_time;
    sigmesh_vendor_aud_vis_alarm_time_status_unpack(msg,&aud_vis_alarm_time);

    jx_hal_log_notice("aud_vis_alarm_time: %d", aud_vis_alarm_time.time);  
    return sizeof(VENDOR_AUD_VIS_ALARM_TIME_STATUS_T);
}

static uint8_t app_mesh_vendor_aud_vis_alarm_volume_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_AUD_VIS_ALARM_VOLUME_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_AUD_VIS_ALARM_VOLUME_STATUS_T aud_vis_alarm_volume;
    sigmesh_vendor_aud_vis_alarm_volume_status_unpack(msg,&aud_vis_alarm_volume);

    jx_hal_log_notice("aud_vis_alarm_volume: %d", aud_vis_alarm_volume.volume);  
    return sizeof(VENDOR_AUD_VIS_ALARM_VOLUME_STATUS_T);
}

/* Vendor sensor alarm status message process */
static uint8_t app_mesh_vendor_sensor_dismantle_alarm_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_SENSOR_DISMANTLE_ALARM_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_SENSOR_DISMANTLE_ALARM_STATUS_T sensor_dismantle_alarm;
    sigmesh_vendor_sensor_dismantle_alarm_status_unpack(msg,&sensor_dismantle_alarm);

    jx_hal_log_notice("sensor_dismantle_alarm: %d", sensor_dismantle_alarm.alarm);  
    return sizeof(VENDOR_SENSOR_DISMANTLE_ALARM_STATUS_T);
}

static uint8_t app_mesh_vendor_sensor_alarm_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_SENSOR_ALARM_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_SENSOR_ALARM_STATUS_T sensor_alarm;
    sigmesh_vendor_sensor_alarm_status_unpack(msg,&sensor_alarm);

    jx_hal_log_notice("sensor_alarm: %d", sensor_alarm.alarm);  
    return sizeof(VENDOR_SENSOR_ALARM_STATUS_T);
}

/* Vendor PIR sensor status message process */
static uint8_t app_mesh_vendor_pir_sensor_event_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_PIR_SENSOR_EVENT_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_PIR_SENSOR_EVENT_STATUS_T pir_sensor_event;
    sigmesh_vendor_pir_sensor_event_status_unpack(msg,&pir_sensor_event);

    jx_hal_log_notice("pir_sensor_event: %d", pir_sensor_event.event);  
    return sizeof(VENDOR_PIR_SENSOR_EVENT_STATUS_T);
}

static uint8_t app_mesh_vendor_pir_sensor_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_PIR_SENSOR_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_PIR_SENSOR_STATUS_T pir_sensor_status;
    sigmesh_vendor_pir_sensor_status_unpack(msg,&pir_sensor_status);

    jx_hal_log_notice("pir_sensor_status: %d", pir_sensor_status.status);  
    return sizeof(VENDOR_PIR_SENSOR_STATUS_T);
}

/* Vendor door sensor status message process */
static uint8_t app_mesh_vendor_door_sensor_event_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_DOOR_SENSOR_EVENT_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_DOOR_SENSOR_EVENT_STATUS_T door_sensor_event;
    sigmesh_vendor_door_sensor_event_status_unpack(msg,&door_sensor_event);

    jx_hal_log_notice("door_sensor_event: %d", door_sensor_event.event);  
    return sizeof(VENDOR_DOOR_SENSOR_EVENT_STATUS_T);
}
static uint8_t app_mesh_vendor_door_sensor_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_DOOR_SENSOR_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_DOOR_SENSOR_STATUS_T door_sensor_status;
    sigmesh_vendor_door_sensor_status_unpack(msg,&door_sensor_status);

    jx_hal_log_notice("door_sensor_status: %d", door_sensor_status.status);  
    return sizeof(VENDOR_DOOR_SENSOR_STATUS_T);
}

/* Vendor temperature and humidity sensor status message process */
static uint8_t app_mesh_vendor_tem_hum_sensor_tem_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T tem_hum_sensor_tem;
    sigmesh_vendor_tem_hum_sensor_tem_status_unpack(msg,&tem_hum_sensor_tem);

    jx_hal_log_notice("tem_hum_sensor_tem: %d", tem_hum_sensor_tem.temperature);  
    return sizeof(VENDOR_TEM_HUM_SENSOR_TEM_STATUS_T);
}

static uint8_t app_mesh_vendor_tem_hum_sensor_hum_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T tem_hum_sensor_hum;
    sigmesh_vendor_tem_hum_sensor_hum_status_unpack(msg,&tem_hum_sensor_hum);

    jx_hal_log_notice("tem_hum_sensor_hum: %d", tem_hum_sensor_hum.humidity);  
    return sizeof(VENDOR_TEM_HUM_SENSOR_HUM_STATUS_T);
}

static uint8_t app_mesh_vendor_tem_hum_sensor_tem_level_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_TEM_HUM_SENSOR_TEM_LEVEL_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_TEM_HUM_SENSOR_TEM_LEVEL_STATUS_T tem_hum_sensor_tem_level;
    sigmesh_vendor_tem_hum_sensor_tem_level_status_unpack(msg,&tem_hum_sensor_tem_level);

    jx_hal_log_notice("tem_hum_sensor_tem_level: %d", tem_hum_sensor_tem_level.level);  
    return sizeof(VENDOR_TEM_HUM_SENSOR_TEM_LEVEL_STATUS_T);
}

static uint8_t app_mesh_vendor_tem_hum_sensor_hum_level_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_TEM_HUM_SENSOR_HUM_LEVEL_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_TEM_HUM_SENSOR_HUM_LEVEL_STATUS_T hum_sensor_hum_level;
    sigmesh_vendor_tem_hum_sensor_hum_level_status_unpack(msg,&hum_sensor_hum_level);

    jx_hal_log_notice("hum_sensor_hum_level: %d", hum_sensor_hum_level.level);  
    return sizeof(VENDOR_TEM_HUM_SENSOR_HUM_LEVEL_STATUS_T);
}

/* Vendor water sensor alarm status message process */
static uint8_t app_mesh_vendor_water_sensor_alarm_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_WATER_SENSOR_ALARM_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_WATER_SENSOR_ALARM_STATUS_T water_sensor_alarm;
    sigmesh_vendor_water_sensor_alarm_status_unpack(msg,&water_sensor_alarm);

    jx_hal_log_notice("water_sensor_alarm: %d", water_sensor_alarm.alarm);  
    return sizeof(VENDOR_WATER_SENSOR_ALARM_STATUS_T);
}

/* Vendor device battery status message process */
static uint8_t app_mesh_vendor_dev_battery_level_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_DEV_BATTERY_LEVEL_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_DEV_BATTERY_LEVEL_STATUS_T battery_level;
    sigmesh_vendor_dev_battery_level_status_unpack(msg,&battery_level);

    jx_hal_log_notice("battery_level: %d", battery_level.level);  
    return sizeof(VENDOR_DEV_BATTERY_LEVEL_STATUS_T);
}

static uint8_t app_mesh_vendor_dev_battery_alarm_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_DEV_BATTERY_ALARM_STATUS_T battery_alarm;
    sigmesh_vendor_dev_battery_alarm_status_unpack(msg,&battery_alarm);

    jx_hal_log_notice("battery_alarm: %d", battery_alarm.alarm);  
    return sizeof(VENDOR_DEV_BATTERY_ALARM_STATUS_T);
}

static uint8_t app_mesh_vendor_dev_battery_capacity_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_DEV_BATTERY_CAPACITY_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_DEV_BATTERY_CAPACITY_STATUS_T battery_capacity;
    sigmesh_vendor_dev_battery_capacity_status_unpack(msg,&battery_capacity);

    jx_hal_log_notice("battery_capacity: %d", battery_capacity.capacity);  
    return sizeof(VENDOR_DEV_BATTERY_CAPACITY_STATUS_T);
}

static uint8_t app_mesh_vendor_dev_battery_charging_status_msg_pro(uint16_t srcAddr,uint8_t *msg,uint8_t len)
{
    if(len < sizeof(VENDOR_DEV_BATTERY_CHARGING_STATUS_T) || (msg == NULL)){
        return 0;
    }
    VENDOR_DEV_BATTERY_CHARGING_STATUS_T battery_charging;
    sigmesh_vendor_dev_battery_charging_status_unpack(msg,&battery_charging);

    jx_hal_log_notice("battery_charging: %d", battery_charging.charging);  
    return sizeof(VENDOR_DEV_BATTERY_CHARGING_STATUS_T);
}

const MESH_VENDOR_MSG_CONSOLE_T app_mesh_vensor_msg_console[] = {
    {VENDOR_ATTR_LIGHT_COLOUR_MODE,          app_mesh_vendor_light_colour_mode_status_msg_pro},
    {VENDOR_ATTR_LIGHT_COLOUR,               app_mesh_vendor_light_colour_status_msg_pro},
    {VENDOR_ATTR_SCENE_PANEL_KEY_NUM,        app_mesh_vendor_scene_panel_key_num_status_msg_pro},
    {VENDOR_ATTR_CURTAIN_MODE,               app_mesh_vendor_curtain_mode_status_msg_pro},
    {VENDOR_ATTR_CURTAIN_PRESENT_PERCENT,    app_mesh_vendor_curtain_present_percent_status_msg_pro},
    {VENDOR_ATTR_CURTAIN_TARGET_PERCENT,     app_mesh_vendor_curtain_target_percent_status_msg_pro},
    {VENDOR_ATTR_AUD_VIS_ALARM_LIGHT_SWITCH, app_mesh_vendor_aud_vis_alarm_light_switch_status_msg_pro},
    {VENDOR_ATTR_AUD_VIS_ALARM_TIME,         app_mesh_vendor_aud_vis_alarm_time_status_msg_pro},
    {VENDOR_ATTR_AUD_VIS_ALARM_VOLUME,       app_mesh_vendor_aud_vis_alarm_volume_status_msg_pro},
    {VENDOR_ATTR_SENSOR_DISMANTLE_ALARM_SET,     app_mesh_vendor_sensor_dismantle_alarm_status_msg_pro},
    {VENDOR_ATTR_SENSOR_ALARM,               app_mesh_vendor_sensor_alarm_status_msg_pro},
    {VENDOR_ATTR_PIR_SENSOR_EVENT,           app_mesh_vendor_pir_sensor_event_status_msg_pro},
    {VENDOR_ATTR_PIR_SENSOR_STATUS,          app_mesh_vendor_pir_sensor_status_msg_pro},
    {VENDOR_ATTR_DOOR_SENSOR_EVENT,          app_mesh_vendor_door_sensor_event_status_msg_pro},
    {VENDOR_ATTR_DOOR_SENSOR_STATUS,         app_mesh_vendor_door_sensor_status_msg_pro},
    {VENDOR_ATTR_TEM_HUM_SENSOR_TEM,         app_mesh_vendor_tem_hum_sensor_tem_status_msg_pro},
    {VENDOR_ATTR_TEM_HUM_SENSOR_HUM,         app_mesh_vendor_tem_hum_sensor_hum_status_msg_pro},
    //{VENDOR_ATTR_TEM_HUM_SENSOR_TEM_LEVEL, app_mesh_vendor_tem_hum_sensor_tem_level_status_msg_pro},
    //{VENDOR_ATTR_TEM_HUM_SENSOR_HUM_LEVEL, app_mesh_vendor_tem_hum_sensor_hum_level_status_msg_pro},
    {VENDOR_ATTR_BATTERY_LEVEL,              app_mesh_vendor_dev_battery_level_status_msg_pro},
    {VENDOR_ATTR_BATTERY_ALARM,              app_mesh_vendor_dev_battery_alarm_status_msg_pro},
    //{VENDOR_ATTR_BATTERY_CAPACITY,         app_mesh_vendor_dev_battery_capacity_status_msg_pro},
    //{VENDOR_ATTR_BATTERY_CHARGING,         app_mesh_vendor_dev_battery_charging_status_msg_pro},
    {NULL,NULL}
};

#if 1
/******************************************************************************
 Function    : app_mesh_notify_vender_events
 Description : (none)
 Note        : (none)
 Input Para  : (none)
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void app_mesh_notify_vendor_events(uint16_t srcAddr, unsigned char *rx_data, int data_len)
{
    MESH_VENDOR_MSG_CONSOLE_T *console    = app_mesh_vensor_msg_console;
    SIG_MESH_RSP_T            *rsp        = (SIG_MESH_RSP_T *)rx_data;
    SIG_VENDOR_MSG_T          *vendor_rsp = (SIG_VENDOR_MSG_T *)(rsp->data);
    /*rx_data: src(2) + dst(2) + opcode(1) + vendorId(2) + tid(1) + status_msg */
    uint8_t *p_status_msg = vendor_rsp->data;
    uint8_t  remain_len   = data_len - sizeof(SIG_MESH_RSP_T) - sizeof(SIG_VENDOR_MSG_T);
    uint8_t  consume_len  = 0;
    uint16_t status_attr_type;

    while(remain_len >= sizeof(status_attr_type)){
        p_status_msg     += consume_len;
        status_attr_type  = (uint16_t)p_status_msg[1] << 8 | p_status_msg[0];
        consume_len       = 0;

        for(uint8_t i = 0;console[i].msg_pro_func != NULL;i ++){
            if(status_attr_type == console[i].attr_type){
                consume_len = console[i].msg_pro_func(srcAddr,p_status_msg,remain_len);
                if(consume_len == 0){
                    /* msg_pro_func run error：protocol error */
                    return;
                }
                if(remain_len < consume_len){
                    /* remain msg is not enough,and msg_pro_func api design error */
                    return;
                }else{
                    remain_len -= consume_len;
                }
                break;
            }
        }
        if(consume_len == 0){
            /* find no attr_type：protocol error */
            return;
        }
    }
}
#endif
#endif


