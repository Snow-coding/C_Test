/******************************************************************************
* Version     : OPP_PLATFORM V100R001C01B001D001                              *
* File        : Opp_DataMng.c                                                *
* Description :                                                               *
*               OPPLE智能控制数据管理中心                                  *
* Note        : (none)                                                        *
* Author      : 智能控制研发部                                                *
* Date:       : 2016-03-11                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "oppQueu.h"
#include <string.h>
/******************************************************************************
*                                Defines                                      *
******************************************************************************/

/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/

/******************************************************************************
*                                Globals                                      *
******************************************************************************/

/******************************************************************************
*                          Extern Declarations                                *
******************************************************************************/

/******************************************************************************
*                             Declarations                                    *
******************************************************************************/

/******************************************************************************
*                            Implementations                                  *
******************************************************************************/
/******************************************************************************
 Function    : OppClearRFQuen
 Description : 清空指定RF队列
 Note        : (none)
 Input Para  : pQueu -- 指定队列
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void oppInitQuen(oppQueu_t *pQueu, uint32_t unitSize, uint8_t maxNum, uint8_t *pBuf)
{
    pQueu->head = 0;
    pQueu->rear = 0;
    pQueu->num = 0;
    pQueu->unitSize = unitSize;
    pQueu->maxNum = maxNum;
    pQueu->pBuf = pBuf;
}

/******************************************************************************
 Function    : OppClearRFQuen
 Description : 清空指定RF队列
 Note        : (none)
 Input Para  : pQueu -- 指定队列
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void oppClearQuen(oppQueu_t *pQueu)
{
    pQueu->head = 0;
    pQueu->rear = 0;
    pQueu->num = 0;
}

/******************************************************************************
 Function    : oppQuenIsFull
 Description : 指定队列是否满
 Note        : (none)
 Input Para  : pQueu -- 指定队列
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
uint8_t oppQuenIsFull(oppQueu_t *pQueu)
{
    if(pQueu->num >= pQueu->maxNum)
    {
        return 1;
    }
    return 0;
}

/******************************************************************************
 Function    : oppQuenGetNum
 Description : get指定队列存储数
 Note        : (none)
 Input Para  : pQueu -- 指定队列
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
uint8_t oppQuenGetNum(oppQueu_t *pQueu)
{
    if(!pQueu)
    {
        return 0;
    }
    return pQueu->num;
}

/******************************************************************************
 Function    : OppReadFromRFQuen
 Description : 读取RF队列
 Note        : (none)
 Input Para  : pQueu -- 指定队列
               buf --读取缓存
 Output Para : (none)
 Return      : 读取到的长度
 Other       : (none)
******************************************************************************/
/*
uint8_t OppReadFromRFQuen(OPP_RF_Queu_t *pQueu, uint8_t* buf)
{
    uint8_t len = 0;

    if(!pQueu || !buf)
    {
        return 0;
    }
    if(pQueu->num == 0)
    {
        return 0;
    }    
    len = pQueu->buf[pQueu->head][0] < OPP_RF_QUEN_ITEM_SIZE ? pQueu->buf[pQueu->head][0] : 0; 
    if(len > 0)
    {
        memcpy(buf, &(pQueu->buf[pQueu->head][1]),len);
    }
    pQueu->num--;
    pQueu->head++;
    if(pQueu->head >= OPP_RF_QUEN_SIZE)
    {
        pQueu->head = 0;
    }
    return len;
}
*/
/******************************************************************************
 Function    : OppWritetoRFQuen
 Description : 写入RF队列
 Note        : (none)
 Input Para  : pQueu -- 指定队列
               buf -- 数据缓存
               len -- 数据长度
 Output Para : (none)
 Return      : OPP_FAILURE -- 失败， OPP_SUCCESS -- 成功
 Other       : (none)
******************************************************************************/
/*
OPP_ERROR_CODE OppWritetoRFQuen(OPP_RF_Queu_t *pQueu, uint8_t* buf, uint8_t len)
{
    if(!pQueu || !buf || len > OPP_RF_QUEN_ITEM_SIZE)
    {
        return OPP_FAILURE;
    }
    if(pQueu->num >= OPP_RF_QUEN_SIZE)
    {
        pQueu->num = OPP_RF_QUEN_SIZE;
        return OPP_FAILURE;
    }
    memcpy((uint8_t*)&pQueu->buf[pQueu->rear][1], buf, len);
    pQueu->buf[pQueu->rear][0] = len;

    pQueu->num++;
    pQueu->rear++;
    if(pQueu->rear >= OPP_RF_QUEN_SIZE)
    {
        pQueu->rear = 0;
    }
    return OPP_SUCCESS;
}
*/

/******************************************************************************
 Function    : oppFindFromQuen
 Description : 查找队列
 Note        : (none)
 Input Para  : pQueu -- 指定队列
               index -- 位置索引
               buf --读取缓存
 Output Para : (none)
 Return      : 读取到的长度
 Other       : (none)
******************************************************************************/
uint8_t oppFindFromQuen(oppQueu_t *pQueu, uint32_t index, uint8_t* buf)
{
    uint8_t len = 0;

    if(!pQueu || !buf || index >= pQueu->num) {
        return 0;
    }

    if(pQueu->num == 0) {
        return 0;
    }    

    len = pQueu->unitSize;
    if(len > 0){
        memcpy(buf, &(pQueu->pBuf[(pQueu->head+index)%pQueu->maxNum * pQueu->unitSize]),len);
    }
    return len;
}

/******************************************************************************
 Function    : OppReadFromQuen
 Description : 读取队列
 Note        : (none)
 Input Para  : pQueu -- 指定队列
               buf --读取缓存
 Output Para : (none)
 Return      : 读取到的长度
 Other       : (none)
******************************************************************************/
uint8_t oppReadFromQuen(oppQueu_t *pQueu, uint8_t* buf)
{
    uint32_t len = 0;

    if(!pQueu || !buf)
    {
        return 0;
    }
    if(pQueu->num == 0)
    {
        return 0;
    }    
    //len = pQueu->pBuf[pQueu->head * pQueu->unitSize] < pQueu->unitSize ? pQueu->pBuf[pQueu->head * pQueu->unitSize] : 0; 
    len = pQueu->unitSize;
    if(len > 0)
    {
        memcpy(buf, &(pQueu->pBuf[pQueu->head * pQueu->unitSize]),len);
    }
    pQueu->num--;
    pQueu->head++;
    if(pQueu->head >= pQueu->maxNum)
    {
        pQueu->head = 0;
    }
    return len;
}

/******************************************************************************
 Function    : OppWritetoQuen
 Description : 写入队列
 Note        : (none)
 Input Para  : pQueu -- 指定队列
               buf -- 数据缓存
               len -- 数据长度
 Output Para : (none)
 Return      : OPP_FAILURE -- 失败， OPP_SUCCESS -- 成功
 Other       : (none)
******************************************************************************/
OPP_ERROR_CODE oppWritetoQuen(oppQueu_t *pQueu, uint8_t* buf, uint32_t len)
{
    if(!pQueu || !buf || len > pQueu->unitSize)
    {
        return OPP_FAILURE;
    }
    if(pQueu->num >= pQueu->maxNum)
    {
        pQueu->num = pQueu->maxNum;
        return OPP_FAILURE;
    }
    memcpy((uint8_t*)&pQueu->pBuf[pQueu->rear * pQueu->unitSize], buf, len);
    //pQueu->pBuf[pQueu->rear * pQueu->unitSize] = len;

    pQueu->num++;
    pQueu->rear++;
    if(pQueu->rear >= pQueu->maxNum)
    {
        pQueu->rear = 0;
    }
    return OPP_SUCCESS;
}

/******************************************************************************
 Function    : oppWritetoQuenHeader
 Description : 写入队列
 Note        : (none)
 Input Para  : pQueu -- 指定队列
               buf -- 数据缓存
               len -- 数据长度
 Output Para : (none)
 Return      : OPP_FAILURE -- 失败， OPP_SUCCESS -- 成功
 Other       : (none)
******************************************************************************/
OPP_ERROR_CODE oppWritetoQuenHeader(oppQueu_t *pQueu, uint8_t* buf, uint32_t len)
{
    if(!pQueu || !buf || len > pQueu->unitSize)
    {
        return OPP_FAILURE;
    }
    if(pQueu->num >= pQueu->maxNum)
    {
        pQueu->num = pQueu->maxNum;
        if(pQueu->rear > 0)
        {
            pQueu->rear--;
        }
        else
        {
            pQueu->rear = pQueu->maxNum-1;
        }
        pQueu->num--;
    }
    if(pQueu->head > 0)
    {
        pQueu->head--;
    }
    else
    {
        pQueu->head = pQueu->maxNum-1;
    }
    memcpy((uint8_t*)&pQueu->pBuf[pQueu->head * pQueu->unitSize], buf, len);
    //pQueu->pBuf[pQueu->head * pQueu->unitSize] = len;
    pQueu->num++;
    return OPP_SUCCESS;
}

#ifdef __cplusplus
}
#endif

