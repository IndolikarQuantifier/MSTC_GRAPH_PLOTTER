#include "MemoryAllocator.h"
#include <stdio.h>

static bst_t* gbst = NULL;



bst_t* GetResourceAllocator()
{
    if(gbst != NULL)
        return gbst;
    
    gbst = create_bst();

    return gbst;
}

key_t GenerateKey(unsigned long int seed)
{
    srand(time(0));

    key_t key = rand() + seed;

    return key;
}

status_t GetGraphResource(key_t key,bst_node_t** OutNode)
{
    bst_node_t* search_node = NULL;
    status_t status = search_key(gbst,key,&search_node);
    if(status == MEMORY_NOT_FOUND)
        return MEMORY_NOT_FOUND;
    
    *OutNode = search_node;
    return SUCCESS;
}

status_t Insert(bst_node_t* new_node)
{
    insert_bst(GetResourceAllocator(),new_node);
    return SUCCESS;
}

status_t DeAllocate(key_t rm_key)
{
    return remove_bst(GetResourceAllocator(),rm_key);
}

// int main(void)
// {
//     struct A* inA;
//     struct B* inB;

//     key_t key1[10],key2[10];
    
//     // GraphResourceAlloc(struct A,treelink,sizeof(struct A),inA,key1,DestroyA,show_a);
//     // GraphResourceAlloc(struct B,treelink,sizeof(struct B),inB,key2,DestroyB,show_b);
    
//     struct A* search_a = NULL;
//     bst_node_t* out_node = NULL;

    

//     for(int i=0;i<10;i++)
//     {
//         GraphResourceAlloc(struct A,treelink,sizeof(struct A),inA,key1[i],DestroyA,show_a);
//         GraphResourceAlloc(struct B,treelink,sizeof(struct B),inB,key2[i],DestroyB,show_b);

//         inA->a = i + 34;
//         inA->b = 'V';
//         inA->d = 3.14 * i;

//         inB->a = i * 10;
//         inB->d = 0.01 * i;
//     }
//     for(int i=0;i<10;i++)
//         printf("i : %d key1 : %lu\n",i,key1[i]);

//     show(gbst,"Before Deletion:");

//     status_t stat = DeAllocate(key1[5]);

//     show(gbst,"After Deletion:");

//     destroy_bst(&gbst);

//     return 1;
// }

// PVOID* xcalloc(size_t size)
// {
//     PVOID newMemory = calloc(1,size);
//     if(newMemory == NULL)
//     {
//         // MessageBox((HWND)NULL,TEXT("Not able to allocate the memory !!!"),TEXT("xcalloc()"),MB_ICONERROR);
//         ExitProcess(GetLastError());
//     }

//     return newMemory;
// }

// void show_a(bst_node_t* tree_node)
// {
//     struct A* a = CONTAINER_OF(tree_node,struct A,treelink);
//     printf("key : %lu int : %d char : %c double : %lf\n",tree_node->key,a->a,a->b,a->d);
// }

// struct A* create_a(int a,char b,double d)
// {
//     struct A* inA = (struct A*)xcalloc(sizeof(struct A));
//     inA->a = a;
//     inA->b = b;
//     inA->d = d;

//     inA->treelink.doFree = DestroyA;
//     inA->treelink.key = a;
//     inA->treelink.doShow = show_a;

//     return inA;
// }

// status_t __stdcall DestroyA(bst_node_t* rm)
// {
//     struct A* rm_a = CONTAINER_OF(rm,struct A,treelink);

//     free(rm_a);
//     rm_a = NULL;

//     return SUCCESS;
// }


// struct B* create_b(int a,double d)
// {
//     struct B* inB = (struct B*) xcalloc(sizeof(struct B));
//     inB->a = a;
//     inB->d = d;

//     inB->treelink.doFree = DestroyB;
//     inB->treelink.key = a;
//     inB->treelink.doShow = show_b;

//     return inB;
// }

// status_t __stdcall DestroyB(bst_node_t* rm)
// {
//     struct B* rm_b = CONTAINER_OF(rm,struct B,treelink);
//     free(rm_b);
//     rm_b = NULL;

//     return SUCCESS;
// }

// void show_b(bst_node_t* tree_node)
// {
//     struct B* inB = CONTAINER_OF(tree_node,struct B,treelink);
//     printf("key : %lu a : %d d : %lf\n",inB->treelink.key,inB->a,inB->d);
// }