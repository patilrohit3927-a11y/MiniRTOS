#ifndef LIST_H
#define LIST_H

#include <stdint.h>


typedef struct ListNode
{
    struct ListNode *next;
    struct ListNode *prev;

    void *data;

}ListNode;



typedef struct
{
    ListNode *head;
    ListNode *tail;

    uint32_t count;

}List;



void List_Init(List *list);

void List_Insert(List *list, ListNode *node);

void List_Remove(List *list, ListNode *node);


#endif