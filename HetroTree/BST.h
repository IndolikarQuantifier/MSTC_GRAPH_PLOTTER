#pragma once

#include<stdlib.h>
#include "../Error/Errors.h"

#define creator void* (*OnCreate) (void)


#define OFFSET_OF(type,mem) ((unsigned long int)(&((type*)0)->mem))
#define CONTAINER_OF(addr,type,mem) ((void*)(((unsigned long int)addr) - OFFSET_OF(type,mem)))

#define BST_KEY_NOT_FOUND 10

typedef int status_t;
typedef unsigned long int key_t;


typedef struct bst bst_t;
typedef struct node bst_node_t;

struct bst
{
    size_t nr_elements;
    struct node* p_root_node;  
};

struct node
{
    key_t key;

    status_t (__stdcall *doFree)(bst_node_t*);
    void (*doShow)(bst_node_t*);

    struct node* parent;
    struct node* left;
    struct node* right;
};



bst_t* create_bst(void);
status_t insert_bst(bst_t* p_bst,bst_node_t* p_new_node);
status_t remove_bst(bst_t* p_bst,key_t key,bst_node_t* rm_node);
status_t search_key(bst_t* p_bst,key_t key,bst_node_t** out_node);
status_t show(bst_t* p_bst,char* msg);

status_t destroy_bst(bst_t** p_bst);


status_t __destroy_bst(bst_node_t* p_root_node);
void __show(bst_node_t* bst_node);