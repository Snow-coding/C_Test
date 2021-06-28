#include <stdio.h>
#include "opple_sigmesh_application.h"
#include "stdbool.h"
#include "time.h"
#include <stdlib.h>
#include <unistd.h>
#include "oppQueu.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
 #include <pthread.h>
 #include "oppList.h"
 #include "dlist.h"

#ifndef LOGLEVEL
#define LOGLEVEL DEBUG
#endif

#define uint16_low(a)  (uint8_t)(a & 0x00ff)
#define uint16_high(a) (uint8_t)((a >> 0x08) & 0x00ff)
#define FALSE false
#define TRUE  true
#define OPPLE_DUMMY_LOG mylog

static const char* s_loginfo[] = {
    [ERROR] = "ERROR",
    [WARN]  = "WARN",
    [INFO]  = "INFO",
    [DEBUG] = "DEBUG",
};

static void get_timestamp(char *buffer)
{
    time_t t;
    struct tm *p;
    struct timeval tv;
    int len;
    int millsec;

    t = time(NULL);
    p = localtime(&t);

    gettimeofday(&tv, NULL);
    millsec = (int)(tv.tv_usec / 1000);

    /* 时间格式：[2011-11-15 12:47:34:888] */
    len = snprintf(buffer, 32, "[%04d-%02d-%02d %02d:%02d:%02d:%03d] ",
        p->tm_year+1900, p->tm_mon+1,
        p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, millsec);

    buffer[len] = '\0';
}

void mylog1(const char* filename, int line, enum LogLevel level, const char* fmt, ...)
{
    if(level > LOGLEVEL)
        return;

    va_list arg_list;
    char buf[1024];
    memset(buf, 0, 1024);
    va_start(arg_list, fmt);
    vsnprintf(buf, 1024, fmt, arg_list);
    char time[32] = {0};

    // 去掉*可能*存在的目录路径，只保留文件名
    const char* tmp = strrchr(filename, '/');
    if (!tmp) tmp = filename;
    else tmp++;
    get_timestamp(time);

    printf("%s[%s] [%s:%d] %s\n", time, s_loginfo[level], tmp, line, buf);

    va_end(arg_list);
}

void print_time(void)
{
	time_t now;     
	struct tm *timenow;     
	time(&now);
  
	timenow = localtime(&now);  
  
	printf("%s ",asctime(timenow)); 
}

static void test_1(void)
{
	printf("Test start:\n");
	uint8_t msg[250] = {0x22,0x22,0x33,0x33,0x44,0x55,0x55,0x66};
    uint8_t len = 8;
    uint8_t testData = 0;
	msg[len ++] = uint16_low(VENDOR_ATTR_LIGHT_COLOUR_MODE);
    msg[len ++] = uint16_high(VENDOR_ATTR_LIGHT_COLOUR_MODE);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_LIGHT_COLOUR);
    msg[len ++] = uint16_high(VENDOR_ATTR_LIGHT_COLOUR);
	msg[len ++] = testData ++;
    msg[len ++] = testData ++;
    msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_SCENE_PANEL_KEY_NUM);
    msg[len ++] = uint16_high(VENDOR_ATTR_SCENE_PANEL_KEY_NUM);
	msg[len ++] = testData ++;
    msg[len ++] = uint16_low(VENDOR_ATTR_CURTAIN_MODE);
    msg[len ++] = uint16_high(VENDOR_ATTR_CURTAIN_MODE);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_CURTAIN_PRESENT_PERCENT);
    msg[len ++] = uint16_high(VENDOR_ATTR_CURTAIN_PRESENT_PERCENT);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_CURTAIN_TARGET_PERCENT);
    msg[len ++] = uint16_high(VENDOR_ATTR_CURTAIN_TARGET_PERCENT);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_AUD_VIS_ALARM_LIGHT_SWITCH);
    msg[len ++] = uint16_high(VENDOR_ATTR_AUD_VIS_ALARM_LIGHT_SWITCH);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_AUD_VIS_ALARM_TIME);
    msg[len ++] = uint16_high(VENDOR_ATTR_AUD_VIS_ALARM_TIME);
	msg[len ++] = testData ++;
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_AUD_VIS_ALARM_VOLUME);
    msg[len ++] = uint16_high(VENDOR_ATTR_AUD_VIS_ALARM_VOLUME);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_SENSOR_DISMANTLE_ALARM);
    msg[len ++] = uint16_high(VENDOR_ATTR_SENSOR_DISMANTLE_ALARM);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_SENSOR_ALARM);
    msg[len ++] = uint16_high(VENDOR_ATTR_SENSOR_ALARM);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_PIR_SENSOR_EVENT);
    msg[len ++] = uint16_high(VENDOR_ATTR_PIR_SENSOR_EVENT);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_PIR_SENSOR_STATUS);
    msg[len ++] = uint16_high(VENDOR_ATTR_PIR_SENSOR_STATUS);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_DOOR_SENSOR_EVENT);
    msg[len ++] = uint16_high(VENDOR_ATTR_DOOR_SENSOR_EVENT);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_DOOR_SENSOR_STATUS);
    msg[len ++] = uint16_high(VENDOR_ATTR_DOOR_SENSOR_STATUS);
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_TEM_HUM_SENSOR_TEM);
    msg[len ++] = uint16_high(VENDOR_ATTR_TEM_HUM_SENSOR_TEM);
	msg[len ++] = testData ++;
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_TEM_HUM_SENSOR_HUM);
    msg[len ++] = uint16_high(VENDOR_ATTR_TEM_HUM_SENSOR_HUM);
	msg[len ++] = testData ++;
	msg[len ++] = testData ++;
	msg[len ++] = uint16_low(VENDOR_ATTR_BATTERY_LEVEL);
    msg[len ++] = uint16_high(VENDOR_ATTR_BATTERY_LEVEL);
	msg[len ++] = testData ++;
    msg[len ++] = uint16_low(VENDOR_ATTR_BATTERY_ALARM);
    msg[len ++] = uint16_high(VENDOR_ATTR_BATTERY_ALARM);
	//msg[len ++] = testData ++;
    printf("msg len: %d\n",len);
    printf("testData: %d\n",testData);
	//app_mesh_notify_vendor_events(0x2222,msg,len);
    printf("Test end\n");
	uint8_t out[50];
	uint16_t outLen = 0;
	VENDOR_LIGHT_COLOUR_MODE_GET_T colourMode;
	colourMode.attr_type = 0x1234;
	sigmesh_vendor_light_colour_mode_get_pack(0x7777,&colourMode,out,50,&outLen);
	printf("colour mode outLen: %d,out: ",outLen);
	for(uint8_t i = 0;i < outLen; i ++)
	{
		printf("%02x ",out[i]);
    }
	printf("out end\n");
}

static void test_2(void)
{
	uint8_t out[50];
	uint16_t outLen = 0;
	VENDOR_LIGHT_COLOUR_SET_T colour;
	colour.attr_type = 0x5678;
	colour.red = 0x10;
	colour.green = 0x11;
	colour.blue = 0x12;

	sigmesh_vendor_light_colour_set_pack(0x8888,&colour,out,50,&outLen);
	printf("colour outLen: %d,out: ",outLen);
	for(uint8_t i = 0;i < outLen; i ++)
	{
		printf("%02x ",out[i]);
    }
	printf("out end\n");

}
#if 0
static void test_set(uint8_t *in,uint8_t len)
{
	uint8_t out[50];
	SIG_IN_T    sig_in;

	sig_in.data = in;
	sig_in.data_len = len;
	
	int index = 0;

	memset(out,0,sizeof(out));
	
	while(1){
		index = sig_2_opple_protocol_convert(&sig_in,out,50,index);
		if(index == -1)
		{
			//printf("finish\n");
			break;
		}
		#if 0
        printf("\n**************************\n");
		printf("index: %d\n",index);
		printf("pack type: %d\n",opple_out.pack_type);
		printf("addr: %04x\n",opple_out.addr);
        printf("data len:%d\n",opple_out.data_len);
		printf("opple out data :");
		for(uint8_t i = 0;i < opple_out.data_len;i++)
		{
			printf("%02x",opple_out.data[i]);
		}
		printf("\n**************************\n");
        #else
	    printf("opple out data :");
		for(uint8_t i = 0;i < ((OPPLE_OUT_T *)out)->data_len;i++)
		{
			printf("%02x",((OPPLE_OUT_T *)out)->data[i]);
		}
		printf("\n");
        #endif
	}
}
#else 
static int test_set(uint8_t *rx_data,uint8_t data_len)
{
    uint8_t out_buffer[64] = {0};
    uint8_t out_len = 0;
    int ret = 0;
    int index = 0;
	int len   = 0;
    SIG_IN_T sig_in;
    sig_in.data = rx_data;
    sig_in.data_len = (uint8_t)(data_len);
    
    while (1) {
		memset(out_buffer,0xff,64);
        ret = sig_2_opple_protocol_convert(&sig_in, (void *)out_buffer, 64, index);
        if (ret < 0) {
			printf("all finish\n");
            break;
        }
		if(ret != 0)
		{
			printf("opple out data :");
			for(uint8_t i = 0;i < ((OPPLE_OUT_T *)out_buffer)->data_len;i++)
			{
				printf("%02x",((OPPLE_OUT_T *)out_buffer)->data[i]);
			}
			printf("\n");
		}
		else
		{
			printf("one frame finish\n");
		}
        index = ret;
    }

	return len;
}

#endif

static void test_3(void)
{
	#if 0
	uint8_t in_array[][20] = {
		{0x01,0x02,0x03,0x04,0x82,0x02,0x00,0x00,0x00,0x01},
		{0x01,0x02,0x03,0x04,0x82,0x04,0x00,0x00,0x00},
		{0x01,0x02,0x03,0x04,0x82,0x4c,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x82,0x4e,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x82,0x64,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x82,0x66,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x82,0x60,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x80,0x1b,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x80,0x1c,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x80,0x1f,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x82,0x46,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x82,0x45,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x82,0x42,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x5e,0x00,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x82,0x9e,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x80,0x49,0x23,0x01,0x00},
		{0x01,0x02,0x03,0x04,0x80,0x4a,0x23,0x01,0x00},
		
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x04,0x01,0x01},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x49,0x05,0x01},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x06,0xe0,0x01},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x48,0x05,0x01},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x01,0x04,0x01},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x1f,0x04,0x01},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x01,0xe1,0x01},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x03,0xe1,0x01},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x0d,0x01,0x01},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x0f,0x01,0x01},
		{0x00}
	};
	#else
	uint8_t in_array[][20] = {
		//{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x04,0x01,0x33,0x49,0x05,0x01},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x49,0x05,0x01,0x04,0x01,0x33},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x1F,0x04,0x11},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x01,0xE1,0x12},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x02,0xE1,0x13},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x01,0xEE,0x14,0x14,0xF4},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x02,0xEE,0x15},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x03,0xEE,0x16},
		{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x04,0xEE,0x17},
		//{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x04,0x01,0x33},
		//{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x49,0x05,0x01},
		//{0x01,0x02,0x03,0x04,0xd0,0x01,0x03,0x00,0x49,0x05,0x01},
		{0}
	};
	#endif

	for(uint8_t i = 0;in_array[i][0] != 0;i++)
	{
		test_set(in_array[i],14);
	}
}

typedef struct
{
	uint8_t len;
	uint8_t *data;
}test_out_t;

void test_4(void)
{
	uint8_t buf[10] = {0,1,2,3,4,5,6,7,8,9};

	test_out_t *out = (test_out_t *)buf;
	
	out->data = &buf[4];

	printf("out: %d",((test_out_t *)buf)->data[0]);
}
#define OPPLE_DEV_IDENTIFY_NUM_MAX          3 
#define OPPLE_DEV_BLINK_TASK_SACN_TIME      200
#define OPPLE_BLINK_QUEU_BUF_NUM_MAX        20
#define OPPLE_BLINK_INTERVAL_TIME_MS        1000
enum{
    OPPLE_DEV_BLINK_OFF = 0,
    OPPLE_DEV_BLINK_ON,
};
typedef struct{
    uint8_t  empty : 1;
    uint8_t  cur_onoff : 1;
    uint8_t  rsv : 6;
    uint8_t  single_times;
    uint8_t  single_times_cnt;
    uint16_t interval_time;
    uint16_t interval_time_cnt;
} OPPLE_DEV_BLINK_PARAM_T;

typedef struct{
    uint8_t  onoff;
    uint16_t addr;
} OPPLE_DEV_PARAM_T;

typedef void (* DEV_BLINK_FUNC_T)(OPPLE_DEV_PARAM_T *dev);

typedef struct{
    OPPLE_DEV_PARAM_T       dev_list[OPPLE_DEV_IDENTIFY_NUM_MAX];
    OPPLE_DEV_BLINK_PARAM_T blink_list[OPPLE_DEV_IDENTIFY_NUM_MAX];
    DEV_BLINK_FUNC_T        dev_blink_on;
    DEV_BLINK_FUNC_T        dev_blink_off;
    uint16_t                scan_time_ms;
}OPPLE_DEV_BLINK_T;

static OPPLE_DEV_BLINK_T opple_dev_blink_mgr;
static OPPLE_DEV_PARAM_T opple_blink_action_queu_buf[OPPLE_BLINK_QUEU_BUF_NUM_MAX];
static oppQueu_t         opple_blink_action_queu;

static void opple_device_blink_scan(void)
{
    OPPLE_DEV_PARAM_T       *dev_list      = opple_dev_blink_mgr.dev_list;
    OPPLE_DEV_BLINK_PARAM_T *blink_list    = opple_dev_blink_mgr.blink_list;
	#if 0 //reserve
    DEV_BLINK_FUNC_T         dev_blink_on  = opple_dev_blink_mgr.dev_blink_on;
    DEV_BLINK_FUNC_T         dev_blink_off = opple_dev_blink_mgr.dev_blink_off;

    if(dev_blink_on == NULL || dev_blink_off == NULL){
        return;
    }
	#endif

    for(uint8_t i = 0;i < OPPLE_DEV_IDENTIFY_NUM_MAX;i ++){
        if(blink_list[i].empty == FALSE){
            if(-- blink_list[i].interval_time_cnt == 0){
                if(blink_list[i].cur_onoff == OPPLE_DEV_BLINK_ON){
                    dev_list[i].onoff       = OPPLE_DEV_BLINK_OFF;
                    blink_list[i].cur_onoff = OPPLE_DEV_BLINK_OFF;
                }else{
                    dev_list[i].onoff       = OPPLE_DEV_BLINK_ON;
                    blink_list[i].cur_onoff = OPPLE_DEV_BLINK_ON;
                }

                oppWritetoQuen(&opple_blink_action_queu,&dev_list[i],sizeof(OPPLE_DEV_PARAM_T));

                blink_list[i].interval_time_cnt = blink_list[i].interval_time;

                if(-- blink_list[i].single_times_cnt == 0){
                    blink_list[i].empty = TRUE;
                }
            }
        }
    }
}

static void opple_device_blink_action_execute(void)
{
    if(oppQuenGetNum(&opple_blink_action_queu) == 0){
		return;
	}

    OPPLE_DEV_PARAM_T dev_param;

    oppReadFromQuen(&opple_blink_action_queu,(uint8_t *)&dev_param);

    if(dev_param.addr == 0){
        return;
    }

    //opple_put_switch(dev_param.addr,(OPPLE_LIGHT_ONOFF_T *)&dev_param.onoff);
	mylog("exe addr:%02x,onoff:%d",dev_param.addr,dev_param.onoff);
}

static OPPLE_DEV_BLINK_PARAM_T *opple_device_blink_find(uint16_t dev_addr)
{
    OPPLE_DEV_PARAM_T       *dev_list   = opple_dev_blink_mgr.dev_list;
    OPPLE_DEV_BLINK_PARAM_T *blink_list = opple_dev_blink_mgr.blink_list;

    for(uint8_t i = 0;i < OPPLE_DEV_IDENTIFY_NUM_MAX;i ++){
        if(dev_addr == dev_list[i].addr){
            return &blink_list[i];
        }
    }

    return NULL;
}

void opple_device_blink_delete(uint16_t dev_addr)
{
    OPPLE_DEV_BLINK_PARAM_T *dev_blink = NULL;
    OPPLE_DEV_PARAM_T       *dev_param = opple_blink_action_queu_buf;

    dev_blink = opple_device_blink_find(dev_addr);
    if(dev_blink != NULL){
        dev_blink->empty = TRUE;
    }
    
    for(uint8_t i = 0;i < OPPLE_BLINK_QUEU_BUF_NUM_MAX;i ++){
        if(dev_addr == dev_param[i].addr){
            dev_param[i].addr = 0;
        }
    }
}

static int opple_device_blink_insert(bool cur_onoff,uint8_t blink_times,uint16_t interval_time,OPPLE_DEV_PARAM_T *dev_param)
{
    if(dev_param == NULL){
        return -1;
    }

    OPPLE_DEV_BLINK_PARAM_T *blink_list = opple_dev_blink_mgr.blink_list;
    OPPLE_DEV_PARAM_T       *dev_list   = opple_dev_blink_mgr.dev_list;
    uint16_t                 scan_time  = opple_dev_blink_mgr.scan_time_ms;

    for(uint8_t i = 0;i < OPPLE_DEV_IDENTIFY_NUM_MAX;i ++){
        if(blink_list[i].empty == FALSE && 
            dev_list[i].addr == dev_param->addr){
            blink_list[i].cur_onoff         = cur_onoff;
            blink_list[i].single_times      = blink_times << 0x01;
			blink_list[i].single_times_cnt  = blink_list[i].single_times;
            blink_list[i].interval_time     = interval_time / scan_time;
            blink_list[i].interval_time_cnt = blink_list[i].interval_time;

            return i;
        }
    }

    for(uint8_t i = 0;i < OPPLE_DEV_IDENTIFY_NUM_MAX;i ++){
        if(blink_list[i].empty == TRUE){
            blink_list[i].cur_onoff         = cur_onoff;
            blink_list[i].single_times      = blink_times << 0x01;
			blink_list[i].single_times_cnt  = blink_list[i].single_times;
            blink_list[i].interval_time     = interval_time / scan_time;
            blink_list[i].interval_time_cnt = blink_list[i].interval_time;
            blink_list[i].empty             = FALSE;

            dev_list[i].addr                = dev_param->addr;
            return i;
        }
    }

    return -1;
}

void opple_device_identify(uint16_t addr,uint8_t times)
{
    OPPLE_DEV_PARAM_T dev_param;
    //app_device_state_info_struct *dev_state;

    //dev_state      = oppDeviceGetStateInfoByDevAddr(addr);
    dev_param.addr = addr;

    if(opple_device_blink_insert(0,times,OPPLE_BLINK_INTERVAL_TIME_MS,&dev_param) < 0){
		OPPLE_DUMMY_LOG("indentify set fail:%04x",addr);
	}
}

int opple_device_blink_init(void)
{
    memset(&opple_dev_blink_mgr,0,sizeof(OPPLE_DEV_BLINK_T));

    for(uint8_t i = 0;i < OPPLE_DEV_IDENTIFY_NUM_MAX;i ++){
        opple_dev_blink_mgr.blink_list[i].empty     = TRUE;
    }

    opple_dev_blink_mgr.dev_blink_on  = NULL;
    opple_dev_blink_mgr.dev_blink_off = NULL;
    opple_dev_blink_mgr.scan_time_ms  = OPPLE_DEV_BLINK_TASK_SACN_TIME;

    oppInitQuen(&opple_blink_action_queu,sizeof(OPPLE_DEV_PARAM_T),OPPLE_BLINK_QUEU_BUF_NUM_MAX,opple_blink_action_queu_buf);
    return 0;
}

uint8_t opple_blink_empty(void)
{
	for(uint8_t i = 0;i < OPPLE_DEV_IDENTIFY_NUM_MAX;i ++){
		if(opple_dev_blink_mgr.blink_list[i].empty == false)  return 1;
	}

	return 0;
}

static void test_5(void)
{
	opple_device_blink_init();

	opple_device_identify(0x01,3);
	opple_device_identify(0x02,3);
	opple_device_identify(0x03,3);
	opple_device_identify(0x04,3);
	opple_device_identify(0x05,3);
	opple_device_identify(0x06,3);
	opple_device_identify(0x07,3);
	/*opple_device_identify(0x08,3);
	opple_device_identify(0x09,3);
	opple_device_identify(0x0a,3);*/
static uint16_t cnt = 0;
	for(uint8_t i= 0; i < 100;i ++){
		opple_device_blink_scan();
		opple_device_blink_action_execute();
		if(oppQuenGetNum(&opple_blink_action_queu) == 0 &&
		opple_blink_empty() == 0) break;

		/*if(++cnt == 20){
			mylog("delete");
			opple_device_blink_delete(0x02);
		}*/
		usleep(OPPLE_DEV_BLINK_TASK_SACN_TIME * 1000);
	}
}

static void test_6(void)
{
	oppQueu_t quen;
	OPPLE_DEV_PARAM_T  Dev[5];
	OPPLE_DEV_PARAM_T *pDev[5];

	oppInitQuen(&quen,sizeof(OPPLE_DEV_PARAM_T *),5,pDev);

	
	oppWritetoQuen(&opple_blink_action_queu,&pDev[0],sizeof(OPPLE_DEV_PARAM_T));

	OPPLE_DEV_PARAM_T dev1;

	oppReadFromQuen(&opple_blink_action_queu,(uint8_t *)&dev1);

	mylog("addr:%02x,onoff:%d",dev1.addr,dev1.onoff);
}
uint8_t buf[5] = {1,2,3,4,5};
	uint32_t pBuf[5];
	uint64_t ppp;
	uint32_t p;
static void test_7(void)
{
	oppQueu_t quen;
	
	ppp = &buf[0];

	//oppInitQuen(&quen,4,5,pBuf);

	mylog("ppp:%d",ppp);
	mylog("ppp:%d",*(uint8_t *)ppp);
	//oppWritetoQuen(&quen,&ppp,4);

	//oppReadFromQuen(&quen,(uint8_t *)&p);

	//mylog("p:%d",*(uint32_t *)p);
}

int * thread(void * arg)
{
        pthread_t newthid;
        newthid = pthread_self();
        //while(1)
		{
			sleep(2);
			opple_device_blink_delete(0x02);
			mylog("delete");
			opple_device_identify(0x04,3);
			opple_device_identify(0x03,3);
		}
		pthread_exit(newthid);
}

void test_thread(void)
{
	pthread_t thid;
	printf("main thread,ID is %lu\n",pthread_self());  //pthread_self()获取本线程id。
	if(pthread_create(&thid,NULL,(void *)thread,NULL)!=0) //创建线程
	{
			printf("thread creation failed\n");
			exit(1);
	}
}

static OPPLE_DEV_STATUS_SYN_T opple_device_status_syn;

/* 设备状态获取消息下发 */
static opple_device_status_get(OPPLE_DEV_STATUS_PARAM_T *dev)
{
	if(dev == NULL){
		return;
	}

	if(dev->dev_addr == 0 || dev->status_index >= OPPLE_DEV_STATUS_NUM){
		return;
	}

	if(dev->status_index == 0){
		//opple_put_onoff_get(dev->dev_addr);
		jx_hal_log_notice("status onoff:%04x",dev->dev_addr);
	}else if(dev->status_index == 1){
		//opple_put_lum_get(dev->dev_addr);
		jx_hal_log_notice("status lum:%04x",dev->dev_addr);
	}else{
		//opple_put_cct_get(dev->dev_addr);
		jx_hal_log_notice("status cct:%04x",dev->dev_addr);
	}
}

/* 设备状态更新 */
void opple_device_status_updata(uint16_t dev_addr,uint8_t status_index)
{
	if(dev_addr == 0 || status_index >= OPPLE_DEV_STATUS_NUM){
		jx_hal_log_notice("status updata fail");
		return;
	}

	OPPLE_DEV_STATUS_PARAM_T *dev_list = opple_device_status_syn.dev_list;

	for(uint8_t i = 0;i < GATEWAY_DEVICE_MAX_NUM;i ++){
		if(dev_list[i].dev_addr == dev_addr){
			dev_list[i].status.byte &= ~(1 << status_index);
			jx_hal_log_notice("status updata:%04x,%d",dev_addr,status_index);
		}
	}
}
uint8_t dev_adding_num = 0;
uint8_t dev_sub_num = 0;
uint8_t dev_get_num = 0;
/* 判断是否有设备处于配网状态 */
void opple_device_pair_nwk_status_scan(void)
{
	//opp_gw_get_dev_adding();

	if(dev_adding_num){
		opple_device_status_set(OPPLE_DEV_SYS_STATUS_NWK);
		jx_hal_log_notice("status dev adding:%d",dev_adding_num);
	}
}

/* 空闲状态判断 */
void opple_device_status_scan(void)
{
	opple_device_pair_nwk_status_scan();

	if(opple_device_status_syn.sys_status & OPPLE_DEV_SYS_STATUS_NWK){
		uint8_t  dev_wait_sub;
		jx_hal_log_notice("opple_device_status_syn.sys_status:%d",opple_device_status_syn.sys_status);
		if(dev_get_num >= dev_sub_num){
			dev_wait_sub = dev_get_num - dev_sub_num;
		}else{
			jx_hal_log_notice("status nwk dev num ERROR!");
			dev_wait_sub = 0;
		}

		if(dev_wait_sub == 0){
			opple_device_status_syn.sys_status &= ~OPPLE_DEV_SYS_STATUS_NWK;
		}else{
			uint16_t time = dev_wait_sub * OPPLE_DEV_NWK_SINGLE_TIMEOUT_PERIOD;
			if(opple_device_status_syn.nwk_timeout_period != time){
				opple_device_status_syn.nwk_timeout_period = time;
				opple_device_status_syn.nwk_timeout_cnt    = 0;
				jx_hal_log_notice("nwk_timeout_cnt updata:%d",time);
			}

			if(++ opple_device_status_syn.nwk_timeout_cnt > opple_device_status_syn.nwk_timeout_period){
				opple_device_status_syn.sys_status &= ~OPPLE_DEV_SYS_STATUS_NWK;
				opple_device_status_syn.nwk_timeout_cnt = 0;
				jx_hal_log_notice("nwk_timeout_cnt timeout:%d",opple_device_status_syn.sys_status);
			}
			jx_hal_log_notice("status dev sub:%d",dev_wait_sub);
		}
	}
	
	if(opple_device_status_syn.sys_status & OPPLE_DEV_SYS_STATUS_MSG){
		if(++ opple_device_status_syn.msg_timeout_cnt > opple_device_status_syn.msg_timeout_period){
			opple_device_status_syn.sys_status &= ~OPPLE_DEV_SYS_STATUS_MSG;
			opple_device_status_syn.msg_timeout_cnt = 0;
		}
	}
}


/* 设置状态 */
void opple_device_status_set(uint8_t status)
{
	if(status == OPPLE_DEV_SYS_STATUS_MSG){
		opple_device_status_syn.sys_status     |= OPPLE_DEV_SYS_STATUS_MSG;
		opple_device_status_syn.msg_timeout_cnt = 0;
	}else if(status == OPPLE_DEV_SYS_STATUS_NWK){
		opple_device_status_syn.sys_status     |= OPPLE_DEV_SYS_STATUS_NWK;
		opple_device_status_syn.nwk_timeout_cnt = 0;
	}

	jx_hal_log_notice("status set:%d",status);
}

/* 设备状态同步扫描，判断待同步设备，并下发状态获取消息 */
int opple_device_status_syn_scan(void)
{
	OPPLE_DEV_STATUS_PARAM_T *dev_list         = opple_device_status_syn.dev_list;
	OPPLE_DEV_STATUS_GET_T    status_get_func  = opple_device_status_syn.status_get_func;
	uint8_t                  *dev_index        = &opple_device_status_syn.dev_index;

	/* 无待同步设备 */
	if(opple_device_status_syn.dev_wait_syn_num == 0){
		return -1;
	}

	/* 空闲判断 */
	if(opple_device_status_syn.sys_status != OPPLE_DEV_SYS_STATUS_IDLE){
		jx_hal_log_notice("status not in idle:%d",opple_device_status_syn.sys_status);
		return -2;
	}

	jx_hal_log_notice("status syn:%d,%d",*dev_index,dev_list[*dev_index].status_index);

	/* 遍历待同步设备表，将onoff/lum/色温三种状态进行逐个同步，扫描逻辑为先扫描单个设备的所有状态，再依次扫描所有设备，按照该逻辑retry */
	for(uint8_t i = 0;i < opple_device_status_syn.dev_syn_num;){
		/* 扫描单设备状态 */
		if(dev_list[*dev_index].status.byte & (1 << dev_list[*dev_index].status_index)){
			/* 重发次数满，则强制清空待同步状态 */
			if(++ dev_list[*dev_index].retry_times_cnt >= dev_list[*dev_index].retry_times){
				dev_list[*dev_index].status.byte     = 0;
				dev_list[*dev_index].retry_times_cnt = 0;
			}

			/* 下发状态获取消息 */
			status_get_func(&dev_list[*dev_index]);

			/* 选中单设备下一个状态，若结束，则进行下一个设备*/
			if(++ dev_list[*dev_index].status_index >= OPPLE_DEV_STATUS_NUM){
				dev_list[*dev_index].status_index = 0;
				if(++ (*dev_index) >= GATEWAY_DEVICE_MAX_NUM){
					*dev_index = 0;
				}
				i ++;
			}
			/* 单次循环仅执行一次状态获取 */
			break;
		}

		/* 选中单设备下一个状态，若结束，则进行下一个设备，遍历直到找到下一个待查询的状态*/
		if(++ dev_list[*dev_index].status_index >= OPPLE_DEV_STATUS_NUM){
			dev_list[*dev_index].status_index = 0;
			if(++ (*dev_index) >= GATEWAY_DEVICE_MAX_NUM){
				*dev_index = 0;
			}
			i ++;
		}
	}	

	/* 计算剩余待同步设备数量 */
	opple_device_status_syn.dev_wait_syn_num = 0;
	for(uint8_t i = 0;i < opple_device_status_syn.dev_syn_num;i ++){
		if(dev_list[i].status.byte != 0){
			opple_device_status_syn.dev_wait_syn_num ++;
		}
	}
	jx_hal_log_notice("status syn wait:%d",opple_device_status_syn.dev_wait_syn_num);
	return 0;
}

uint16_t dev_base_list[10] = {1,2,3,4,5,6,7,8,9,10};

/* 设备状态同步设置 */
int opple_device_status_syn_set(void)
{
	OPPLE_DEV_STATUS_PARAM_T    *dev_status_list = opple_device_status_syn.dev_list;
	uint8_t dev_index = 0;

	opple_device_status_syn.dev_index       = 0;

	/* 将所有真实设备添加到同步表，并初始化参数 */
	for(uint8_t i = 0;i < GATEWAY_DEVICE_MAX_NUM;i ++){
		{
			dev_status_list[dev_index].dev_addr         = dev_base_list[i];
			dev_status_list[dev_index].retry_times_cnt  = 0;
			dev_status_list[dev_index].status.byte      = OPPLE_DEV_STATUS_ALL;
			dev_status_list[dev_index].status_index     = 0;
			dev_index ++;
		}
	}

	opple_device_status_syn.dev_syn_num      = dev_index;
	opple_device_status_syn.dev_wait_syn_num = dev_index;

	opple_device_status_set(OPPLE_DEV_SYS_STATUS_MSG);
	jx_hal_log_notice("status syn num set:%d",dev_index);
}

/* 设备状态同步初始化 */
int opple_device_status_syn_init(void)
{
	memset(&opple_device_status_syn,0,sizeof(OPPLE_DEV_STATUS_SYN_T)*GATEWAY_DEVICE_MAX_NUM);

	for(uint8_t i = 0;i < GATEWAY_DEVICE_MAX_NUM;i ++){
		opple_device_status_syn.dev_list[i].retry_times = OPPLE_DEV_STATUS_RETRY_TIME;
	}

	opple_device_status_syn.msg_timeout_period  = OPPLE_DEV_MSG_TIMEOUT_PERIOD;
	opple_device_status_syn.nwk_timeout_period  = OPPLE_DEV_NWK_SINGLE_TIMEOUT_PERIOD * GATEWAY_DEVICE_MAX_NUM;

	opple_device_status_syn.status_get_func     = opple_device_status_get;
}

int *thread_1(void * arg)
{
	pthread_t newthid;
	newthid = pthread_self();
	while(1)
	{
		sleep(5);
		opple_device_status_set(OPPLE_DEV_SYS_STATUS_MSG);
		
	}
	//pthread_exit(newthid);
}
int *thread_2(void * arg)
{
	pthread_t newthid;
	newthid = pthread_self();
	//while(1)
	{
		sleep(8);
		
		opple_device_status_syn_set();
	}
	pthread_exit(newthid);
}
int *thread_3(void * arg)
{
	pthread_t newthid;
	newthid = pthread_self();
	//while(1)
	{
		sleep(4);
		opple_device_status_updata(1,0);
		sleep(1);
		opple_device_status_updata(2,1);
		sleep(1);
		opple_device_status_updata(3,0);
		sleep(2);
		opple_device_status_updata(3,1);
		sleep(3);
		opple_device_status_updata(3,2);
	}
	pthread_exit(newthid);
}
int *thread_4(void * arg)
{
	pthread_t newthid;
	newthid = pthread_self();
	//while(1)
	{
		sleep(5);
	    dev_adding_num = 5;
        dev_sub_num = 0;
        dev_get_num = 5;
		sleep(10);
		dev_adding_num = 0;
		dev_sub_num = 1;
		sleep(5);
		dev_sub_num = 2;
		sleep(5);
		dev_sub_num = 3;
		sleep(5);
		dev_sub_num = 4;
	}
	pthread_exit(newthid);
}

void thread_test(void)
{
	pthread_t thid_1;
	pthread_t thid_2;
	pthread_t thid_3;
	pthread_t thid_4;
	/*if(pthread_create(&thid_1,NULL,(void *)thread_1,NULL)!=0) //创建线程
	{
			printf("thread creation failed\n");
			exit(1);
	}*/
	/*printf("main thread,ID is %lu\n",pthread_self());  //pthread_self()获取本线程id。
	if(pthread_create(&thid_2,NULL,(void *)thread_2,NULL)!=0) //创建线程
	{
			printf("thread creation failed\n");
			exit(1);
	}*/
	/*if(pthread_create(&thid_3,NULL,(void *)thread_3,NULL)!=0) //创建线程
	{
			printf("thread creation failed\n");
			exit(1);
	}*/
	if(pthread_create(&thid_4,NULL,(void *)thread_4,NULL)!=0) //创建线程
	{
			printf("thread creation failed\n");
			exit(1);
	}
}

void test_8(void)
{
	opple_device_status_syn_init();

	thread_test();
	opple_device_status_syn_set();
	
	while(1)
	{
		sleep(1);
		//mylog("status updata");
		opple_device_status_scan();

		if(opple_device_status_syn_scan() == -1){
			exit(1);
		}
		
	}
}

typedef struct
{
	uint8_t data;
	struct  list_head list_node;
}test_data_t;

struct  list_head myList_head;

void test_9(void)
{
	INIT_LIST_HEAD(&myList_head);

	test_data_t *myList = malloc(sizeof(test_data_t));

	myList->data = 5;

	list_add_tail(&myList->list_node,&myList_head);

	myList = malloc(sizeof(test_data_t));
	myList->data = 6;

	list_add_tail(&myList->list_node,&myList_head);

	myList = malloc(sizeof(test_data_t));
	myList->data = 7;

	list_add(&myList->list_node,&myList_head);

	struct  list_head *node;
	test_data_t *data;
	struct  list_head *next;

	list_for_each(node,&myList_head){
		data = list_entry(node,test_data_t,list_node);
		printf("list 1:%d\n",data->data);

		if(data->data == 5){
			next = &data->list_node;
			break;
		}
	}

	list_for_each(node,next){
		data = list_entry(node,test_data_t,list_node);
		printf("list 2:%d\n",data->data);
		if(data->list_node.next == &myList_head){
			printf("NULL\n");
			break;
		}
		
	}
}

void test_10(void)
{
	uint16_t opp_time = M_OPP_NTOHS(0xd007);//2000;//*(uint16_t *)msg->payload;
	uint8_t sig_time  = (1 << 6) | ((opp_time / 1000) & (0xff >> 2));
	printf("%02x\n",sig_time);
}

void test_11(void)
{
	char str[] = "BBFFCCDD,11223344,1234ABCD";
	char *ss;
	uint8_t pwd[8];
	uint32_t ppp = strtol(str,&ss,16);
	printf("pwd:");
	for(uint8_t i =0;i <8;i++){
		pwd[i] = (ppp >> ((7 - i) * 4))& 0x0f;
		printf("%02x ",pwd[i]);
	}
	printf("\r\n");
	printf("p:%08x\r\n",strtol(str,&ss,16));
	printf("s:%s\r\n",ss);
}

int main(void)
{
	//test_thread();
	//test_1();
	//test_2();
	//test_3();
	//test_4();
	//test_5();
	//test_6();
	//test_7();
	//test_8();
	//test_9();
	//test_10();
	test_11();
	return 0;
}

typedef struct
{
	char    name[20];
	uint8_t id;
	uint8_t level;
	uint8_t department;
}DORCTOR_T;

typedef struct
{
	char     name[20];
	uint32_t id;
	uint8_t  weight;
	uint8_t  age;
} PATIENT_T;

typedef struct
{
	uint8_t department_id;
	uint8_t level;
}APPLY_INFO_T;


typedef struct 
{
	char    name[20];
	uint8_t id;
	uint8_t common_cur_number;
	uint8_t common_max_number;
	uint8_t expert_cur_number;
	uint8_t expert_max_number;
} DEPARTMENT_T;

typedef struct
{
	char         name[20];
	uint8_t      id;
	uint8_t      level;
	uint8_t      department_number;
	DEPARTMENT_T department[100];
} HOSPITAL_T;

static HOSPITAL_T hospital_list[10] = {
	{"first hospital",1,1,10},
};

void hospital_system_init(void)
{
	
}


int hospital_registration_apply( PATIENT_T *patient,APPLY_INFO_T *apply_info,HOSPITAL_T *hospital)
{
	HOSPITAL_T *h_list = hospital_list;
	for(uint8_t i = 0;h_list[i].id != NULL;i ++){
		if(hospital->id != h_list->id){
			continue;
		}

		DEPARTMENT_T *department = h_list[i].department;
		for(uint8_t j = 0;department[j].id != NULL;j ++){
			if(apply_info->department_id != department[j].id){
				continue;
			}

			if(apply_info->level == 0){
				return ++ department[j].common_cur_number;
			}else if(apply_info->level == 1){
				if(++ department[j].expert_cur_number > department[j].expert_max_number){
					return -1;
				}
				return department[j].expert_cur_number;
			}
			
		}
	}
}
