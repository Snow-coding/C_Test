/******************************************************************************
* Version     : OPP_PLATFORM V100R001C01B001D001                              *
* File        : oppList.h                                                *
* Description :                                                               *
*               OPPLE list ����ͷ�ļ�                                   *
* Note        : (none)                                                        *
* Author      : ���ܿ����з���                                                *
* Date:       : 2019-01-18                                                    *
* Mod History : (none)                                                        *
******************************************************************************/
#ifndef __OPP_LIST_H__
#define __OPP_LIST_H__

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
#define LIST_HEAD_INIT(name)                               { &(name), &(name) }

#define LIST_HEAD(name)                                    struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr)                                do { \
                                                                (ptr)->next = (ptr); (ptr)->prev = (ptr); \
                                                              } while (0)

#define list_entry(ptr, type, member)                      container_of(ptr, type, member)

#define container_of(ptr, type, member)                    ({ \
                                                                const typeof(((type*)0)->member) *__mptr = (ptr); \
                                                                (type*)((char*)__mptr - offsetOf(type, member)); \
                                                            })

#define list_for_each(pos, head)                           for (pos = (head)->next; pos != (head); pos = pos->next)
/******************************************************************************
*                                Typedefs                                     *
******************************************************************************/
//#pragma pack(1)
#if 0
struct list_head
{
    struct list_head *next, *prev;
};
#endif
//#pragma pack()
/******************************************************************************
*                           Extern Declarations                               *
******************************************************************************/

/******************************************************************************
*                              Declarations                                   *
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /*__OPP_LIST_H__*/


