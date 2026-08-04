#include "list.h"



void List_Init(List *list)
{
    list->head = 0;
    list->tail = 0;
    list->count = 0;
}



void List_Insert(List *list, ListNode *node)
{

    node->next = 0;
    node->prev = list->tail;


    if(list->tail != 0)
    {
        list->tail->next = node;
    }

    else
    {
        list->head = node;
    }


    list->tail = node;

    list->count++;

}



void List_Remove(List *list, ListNode *node)
{

    if(node->prev)
    {
        node->prev->next = node->next;
    }

    else
    {
        list->head = node->next;
    }



    if(node->next)
    {
        node->next->prev = node->prev;
    }

    else
    {
        list->tail = node->prev;
    }


    list->count--;


    node->next = 0;
    node->prev = 0;

}