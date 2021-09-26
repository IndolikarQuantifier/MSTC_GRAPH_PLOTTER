#include "BST.h"

// int main(void)
// {
//     PBST pbst = CreateBST();

//     InsertBST(pbst,create_A,delete_A);
//     InsertBST(pbst,create_A,delete_A);
//     InsertBST(pbst,create_A,delete_A);
//     KEY key = InsertBST(pbst,create_A,delete_A);

//     PNODE node = NULL;
//     SearchNode(pbst,key,&node);
//     show(pbst,"First Traversal !!!");
//     RemoveAll(pbst,pbst->Head);
// }

PBST CreateBST()
{
    PBST newBST = (PBST) calloc(1,sizeof(newBST));
    if(newBST == NULL)
    {
        printf("Not able to allocate memory !!!");
        ExitProcess(EXIT_FAILURE);
    }

    newBST->Head = NULL;
    newBST->nElements = 0;

    return newBST;
}

// Called from xcalloc()

Status InsertBST
(
    PBST pBST,
    PNODE (__stdcall *OnCreate)(),
    Status (__stdcall *OnDelete)(PVOID DeleteNode)
)
{
    PNODE NewNode = OnCreate();
    KEY key = GenerateKey((SEED)NewNode);
    PNODE Walk = pBST->Head;
    
    if(pBST->Head == NULL)
    {
        
        SetNode(&NewNode,key,NULL,NULL,NULL,OnDelete);
        pBST->Head = NewNode;

        return NewNode->Key;
    }
    
    while(1)
    {
        
        if(Walk->Key <= key)
        {
            if(Walk->left == NULL)
            {
                Walk->left = NewNode;
                SetNode(&NewNode,key,NULL,NULL,Walk,OnDelete);
                return NewNode->Key;
            }
            Walk = Walk->left;
        }
        else
        {
            if(Walk->right == NULL)
            {
                Walk->right = NewNode;
                SetNode(&NewNode,key,NULL,NULL,Walk,OnDelete);
                return NewNode->Key;
            }
            Walk = Walk->right;
        }
    }
}

Status Remove(PBST pBST,KEY Key)
{
    return 0;
}

Status RemoveAll(PBST pBST,PNODE CurrentNode)
{
    if(CurrentNode == NULL)
        return 0;

    RemoveAll(pBST,CurrentNode->left);
    RemoveAll(pBST,CurrentNode->right);

    CurrentNode->OnDelete(CurrentNode);

    return 0;   
}

Status SearchNode(PBST pBST,KEY key,PNODE* Node)
{
    if(pBST->Head == NULL)
        return TREE_EMPTY;
    
    PNODE Walk = pBST->Head;
    while(1)
    {
        if(Walk->Key == key)
        {
            *Node = Walk;
            return KEY_FOUND;
        }

        else if(Walk->Key < key)
        {
            if(Walk->left == NULL)
                return KEY_NOT_FOUND;
            Walk = Walk->left;
        }
        else
        {
            if(Walk->right == NULL)
                return KEY_NOT_FOUND;
            Walk = Walk->right;
        }
    }
}

KEY GenerateKey(SEED Seed)
{
    srand(time(0));
    KEY newKey = (unsigned long long int)rand() + Seed;
    return newKey;
}

void SetNode
(
    PNODE* node,
    KEY key,
    PNODE left,
    PNODE right,
    PNODE parent,
    Status (__stdcall *OnDelete)(PVOID DeleteNode)
)
{
    (*node)->Key = key;
    (*node)->left = left;
    (*node)->right = right;
    (*node)->parent = parent;
    (*node)->OnDelete = OnDelete;
}

void show(PBST pBST,char* msg)
{
    puts(msg);
    PNODE Walk = pBST->Head;
    InOrder(pBST,Walk);
}

void InOrder(PBST pBST,PNODE Node)
{
    if(Node == NULL)
        return;
    
    InOrder(pBST,Node->left);
    InOrder(pBST,Node->right);
}