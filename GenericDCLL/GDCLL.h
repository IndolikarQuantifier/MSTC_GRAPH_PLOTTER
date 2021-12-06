#pragma once

#include "../Error/Errors.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define OFFSET_OF(type,mem) ((unsigned long int)(&((type*)0)->mem))
#define CONTAINER_OF(addr,type,mem) ((void*)(((unsigned long int)addr) - OFFSET_OF(type,mem)))

#define cleaner status_t (*doFree) (LPNODE rm)
#define display void (doShow) (LPNODE show_node)

#define LIST_DATA_NOT_FOUND 0
#define LIST_EMPTY 1


typedef int status_t;
typedef int bool_t;


typedef struct tagNODE
{
    size_t nr_elements;

    struct tagNODE* next;
    struct tagNODE* prev;
}NODE,*LPNODE,*PNODE,DCLL,*LPDCLL,*PDCLL;

LPDCLL CreateDCLL(void);
status_t InsertBegin(LPDCLL p_list,LPNODE new_node);
status_t InsertEnd(LPDCLL p_list,LPNODE new_node);
status_t ShowList(LPDCLL p_list,display);

status_t GetBegin(LPDCLL p_list,LPNODE* OutNode);

status_t RemoveBegin(LPDCLL p_list,cleaner);
status_t RemoveEnd(LPDCLL p_list,cleaner);

status_t DestoryDCLL(LPDCLL* pp_list,cleaner);

static status_t GenericInsert(LPNODE beg,LPNODE mid,LPNODE end);
static status_t GenericDelete(LPNODE rm_node,cleaner);
static bool_t IsListEmpty(LPDCLL p_list);

void* xmalloc(size_t nr_bytes);