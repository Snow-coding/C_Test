#ifndef _opple_common_h_
#define _opple_common_h_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

#define OPPLE_DUMMY_PROXY_SCAN_TIME         200
#define OPPLE_DEV_STATUS_SCAN_TIME          1000
#define OPPLE_DEV_STATUS_NUM                0x03
#define OPPLE_DEV_NWK_SINGLE_TIMEOUT_PERIOD ((3 * 1000UL) / OPPLE_DEV_STATUS_SCAN_TIME)
#define OPPLE_DEV_MSG_TIMEOUT_PERIOD        ((2 * 1000) / OPPLE_DEV_STATUS_SCAN_TIME)
#define OPPLE_DEV_STATUS_RETRY_TIME         ((2 * OPPLE_DEV_STATUS_NUM))
#define OPPLE_DEV_STATUS_ALL                (~(0xff << OPPLE_DEV_STATUS_NUM))
#define GATEWAY_DEVICE_MAX_NUM  3
enum{
	OPPLE_DEV_SYS_STATUS_IDLE = 0,
	OPPLE_DEV_SYS_STATUS_MSG  = (1 << 0),
	OPPLE_DEV_SYS_STATUS_NWK  = (1 << 1),	
};

typedef struct{
	union {
		uint8_t  onoff_syn    : 1;
		uint8_t  bright_syn   : 1;
		uint8_t  cct_syn      : 1;
		uint8_t  rsv_1        : 5;
		uint8_t  byte;
	}status;
	uint8_t  status_index;
	uint8_t  retry_times;
	uint8_t  retry_times_cnt;
	uint16_t dev_addr;
} OPPLE_DEV_STATUS_PARAM_T;

typedef void (* OPPLE_DEV_STATUS_GET_T)(OPPLE_DEV_STATUS_PARAM_T *dev);
typedef struct{
	OPPLE_DEV_STATUS_PARAM_T dev_list[GATEWAY_DEVICE_MAX_NUM];
	OPPLE_DEV_STATUS_GET_T   status_get_func;
	uint8_t  dev_wait_syn_num;
	uint8_t  dev_syn_num;
	uint8_t  dev_index;
	uint8_t  sys_status;
	uint16_t msg_timeout_period;
	uint16_t msg_timeout_cnt;
	uint16_t nwk_timeout_period;
	uint16_t nwk_timeout_cnt;
} OPPLE_DEV_STATUS_SYN_T;

enum LogLevel
{
    ERROR = 1,
    WARN  = 2,
    INFO  = 3,
    DEBUG = 4,
};

void mylog1(const char* filename, int line, enum LogLevel level, const char* fmt, ...) __attribute__((format(printf,4,5)));

//#define mylog(level, format, ...) mylog1(__FILE__, __LINE__, level, format, ## __VA_ARGS__)
#define mylog(format, ...) mylog1(__FILE__, __LINE__, DEBUG, format, ## __VA_ARGS__)

#define jx_hal_log_notice mylog

#define OPPLE_MEMSET memset
#define OPPLE_MEMCPY memcpy

#define uint8_t  unsigned char
#define uint16_t unsigned short int
#define int16_t  short int
#define uint32_t unsigned int
#define u8       uint8_t
#define u16      uint16_t
#define u32      uint32_t

#ifdef __cplusplus
}
#endif

#endif
