#pragma once

#include<stdio.h>
#include<Windows.h>

#define OFFSET_OF(type,member) ((unsigned long long int)(&((type)0)->member))
#define CONTAINER_OF(type,address,member) ((type)((unsigned long long int) address) - OFFSET_OF(type,member))

#define TREE_EMPTY 0
#define KEY_NOT_FOUND 1
#define KEY_FOUND 2

typedef unsigned long int Count;
typedef unsigned long long int KEY;
typedef unsigned long long int SEED;
typedef KEY Status;


typedef struct tagNODE
{   

    KEY Key;

    Status (__stdcall *OnDelete)(PVOID RmNode);

    struct tagNODE* left;
    struct tagNODE* right;
    struct tagNODE* parent;
    
}NODE,*PNODE,*LPNODE;


typedef struct tagBST
{
    PNODE Head;
    Count nElements;
}BST,*PBST,*LPBST;


PBST CreateBST();
Status InsertBST(PBST pBST,PNODE (__stdcall *OnCreate)(),Status (__stdcall *OnDelete)(PVOID DeleteNode));
Status Remove(PBST pBST,KEY Key);
Status RemoveAll(PBST pBST,PNODE CurrentNode);
Status SearchNode(PBST pBST,KEY key,PNODE* Node);
void SetNode(PNODE* node,KEY key,PNODE left,PNODE right,PNODE parent,Status (__stdcall* OnDelete)(PVOID DeleteNode));
void show(PBST pBST,char* msg);
void InOrder(PBST pBST,PNODE Node);

KEY GenerateKey(SEED Seed);