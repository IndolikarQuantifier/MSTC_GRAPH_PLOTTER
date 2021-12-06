#include "./GDCLL.h"

LPDCLL CreateDCLL(void)
{
    LPDCLL p_new_dcll = NULL;
    p_new_dcll = xmalloc(sizeof(DCLL));

    p_new_dcll->next = p_new_dcll;
    p_new_dcll->prev = p_new_dcll;
    p_new_dcll->nr_elements = 0;


    return p_new_dcll;
}


status_t InsertBegin(LPDCLL p_list,LPNODE new_node)
{
    GenericInsert(p_list,new_node,p_list->next);
    p_list->nr_elements++;

    return SUCCESS;
}

status_t InsertEnd(LPDCLL p_list,LPNODE new_node)
{
    GenericInsert(p_list->prev,new_node,p_list);
    p_list->nr_elements++;

    return SUCCESS;
}

status_t GetBegin(LPDCLL p_list,LPNODE* OutNode)
{
    if(IsListEmpty(p_list) == LIST_EMPTY)
        return LIST_EMPTY;
    
    *OutNode = p_list->next;

    return SUCCESS;
}

status_t RemoveBegin(LPDCLL p_list,cleaner)
{
    if(IsListEmpty(p_list) == LIST_EMPTY)
        return LIST_EMPTY;

    GenericDelete(p_list->next,doFree);
    return SUCCESS;
}
status_t RemoveEnd(LPDCLL p_list,cleaner)
{
    if(IsListEmpty(p_list) == LIST_EMPTY)
        return LIST_EMPTY;

    GenericDelete(p_list->prev,doFree);
    return SUCCESS;
}

status_t ShowList(LPDCLL p_list,display)
{
    LPNODE p_run = p_list->next;

    while(p_run != p_list)
    {
        doShow(p_run);
        p_run = p_run->next;
    }

    return SUCCESS;
}

status_t DestoryDCLL(LPDCLL* pp_list,cleaner)
{
    LPNODE p_run = NULL;
    p_run = (*pp_list)->next;

    while(RemoveEnd((*pp_list),doFree) != LIST_EMPTY);

    (*pp_list)->nr_elements = 0;
    (*pp_list) = NULL;
    return SUCCESS;
}

status_t GenericInsert(LPNODE beg,LPNODE mid,LPNODE end)
{
    beg->next = mid;
    end->prev = mid;

    mid->next = end;
    mid->prev = beg;

    return SUCCESS;
}

status_t GenericDelete(LPNODE rm_node,cleaner)
{
    rm_node->next->prev = rm_node->prev;
    rm_node->prev->next = rm_node->next;

    doFree(rm_node);
    return SUCCESS;
}

bool_t IsListEmpty(LPDCLL p_list)
{
    if(p_list->next == p_list)
        return LIST_EMPTY;
    
    return 0;
}

void* xmalloc(size_t nr_bytes)
{
    void* ptr = malloc(nr_bytes);
    if(ptr == NULL)
    {
        fprintf(stderr,"Not able to allocate memory !!!");
        exit(EXIT_FAILURE);
    }

    memset(ptr,0,nr_bytes);
    return ptr;
}