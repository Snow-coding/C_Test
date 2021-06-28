/******************************************************************************
* Version     : OPP_PLATFORM V100R001C01B001D001                              *
* File        : oppList.c                                                *
* Description :                                                               *
*               OPPLE list ����Դ�ļ�                                  *
* Note        : (none)                                                        *
* Author      : ���ܿ����з���                                                *
* Date:       : 2016-03-11                                                    *
* Mod History : (none)                                                        *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                Includes                                     *
******************************************************************************/
#include "oppList.h"
#include "dlist.h"
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
#if 0
/******************************************************************************
 Function    : __list_add
 Description : ��prev��next֮������µ�Ԫ��
 Note        : (none)
 Input Para  : new -- ��Ԫ��
               prev -- ǰ��Ԫ��
               next -- ����Ԫ��
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static inline void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/******************************************************************************
 Function    : __list_del
 Description : ��prev��next֮������µ�Ԫ��
 Note        : (none)
 Input Para  : new -- ��Ԫ��
               prev -- ǰ��Ԫ��
               next -- ����Ԫ��
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    prev->next = next;
    next->prev = prev;
}
#endif
/******************************************************************************
 Function    : oppListAdd
 Description : ��listͷ�����������µ�Ԫ��
 Note        : (none)
 Input Para  : new -- ��Ԫ��
               head -- listͷ��
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void oppListAdd(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

/******************************************************************************
 Function    : oppListAddTail
 Description : ��listͷ��ǰ�������µ�Ԫ��
 Note        : (none)
 Input Para  : new -- ��Ԫ��
               head -- listͷ��
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void oppListAddTail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

/******************************************************************************
 Function    : oppListDel
 Description : ��list���н����������ɾ��
 Note        : (none)
 Input Para  : entry -- ���������
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void oppListDel(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

/******************************************************************************
 Function    : oppListMove
 Description : ��list���н�����������ƶ���ָ����ں���
 Note        : (none)
 Input Para  : entry -- ���������
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void oppListMove(struct list_head *list, struct list_head *head)
{
    //__list_del(entry->prev, entry->next);
    oppListAdd(list, head);
}

/******************************************************************************
 Function    : oppListMove
 Description : ��list���н�����������ƶ���ָ�����ǰ��
 Note        : (none)
 Input Para  : entry -- ���������
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void oppListMoveTail(struct list_head *list, struct list_head *head)
{
    //__list_del(entry->prev, entry->next);
    oppListAddTail(list, head);
}

/******************************************************************************
 Function    : oppListEmpty
 Description : ���list���ǿյģ����ط���
 Note        : (none)
 Input Para  : head -- list�����
 Output Para : (none)
 Return      : empty : ��0��else: 0
 Other       : (none)
******************************************************************************/
int oppListEmpty(struct list_head *head)
{
    return (head->next == head);
}

/******************************************************************************
 Function    : oppListEmpty
 Description : ��head�ĺ������list���ϲ���������
 Note        : (none)
 Input Para  : list -- Ҫ�ϲ�������
               head -- ������
 Output Para : (none)
 Return      : (none)
 Other       : (none)
******************************************************************************/
void oppListSplice(struct list_head *list, struct list_head *head)
{
    head->prev->next = list;
    list->prev->next = head;
    head->prev = list->prev;
    list->prev = head->prev;
}

#ifdef __cplusplus
}
#endif

