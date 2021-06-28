/******************************************************************************
* Version     : OPP_PLATFORM V100R001C01B001D001                              *
* File        : Opp_Queu.h                                                *
* Description :                                                               *
*               OPPLE���ܿ��ƹ�������ͷ�ļ�                                   *
* Note        : (none)                                                        *
* Author      : ���ܿ����з���                                                *
* Date:       : 2016-10-27                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __OPP_QUEU_H__
#define __OPP_QUEU_H__

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

/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
typedef enum
{
    OPP_SUCCESS = 0, 
    OPP_FAILURE, 
    OPP_OVERFLOW, 
    OPP_NO_PRIVILEGE, 
    OPP_EXISTED, 
    /*后续继续扩展*/
    OPP_UNKNOWN
}OPP_ERROR_CODE;

#pragma pack(1)
typedef struct
{
    uint32_t head;
    uint32_t rear;
    uint32_t unitSize;
    uint8_t num;
    uint8_t maxNum; 
    uint8_t *pBuf;
} oppQueu_t;
#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                              Declarations                                   *
******************************************************************************/
void oppInitQuen(oppQueu_t *pQueu, uint32_t unitSize, uint8_t maxNum, uint8_t *pBuf);
void oppClearQuen(oppQueu_t *pQueu);
uint8_t oppQuenIsFull(oppQueu_t *pQueu);
uint8_t oppFindFromQuen(oppQueu_t *pQueu, uint32_t index, uint8_t* buf);
uint8_t oppReadFromQuen(oppQueu_t *pQueu, uint8_t* buf);
OPP_ERROR_CODE oppWritetoQuen(oppQueu_t *pQueu, uint8_t* buf, uint32_t len);
OPP_ERROR_CODE oppWritetoQuenHeader(oppQueu_t *pQueu, uint8_t* buf, uint32_t len);
uint8_t oppQuenGetNum(oppQueu_t *pQueu);
#ifdef __cplusplus
}
#endif

#endif /*__QUEU_H__*/


