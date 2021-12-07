#include "BST.h"

#define MEMORY_NOT_FOUND BST_KEY_NOT_FOUND

static bst_t* GetResourceAllocator();
key_t GenerateKey(unsigned long int seed);
status_t GetGraphResource(key_t key,bst_node_t** OutNode);
status_t Insert(bst_node_t* new_node);
status_t DeAllocate(key_t rm_key);

#define GraphResourceAlloc(type,mem,nr_bytes,current_context,outKey,destroy,show) do{\
                                    type* inStruct = (type*) xcalloc(nr_bytes);\
                                    current_context = inStruct;\
                                    key_t _Inkey = GenerateKey((key_t)&(inStruct->mem));\
                                    current_context = inStruct;\
                                    outKey = _Inkey;\
                                    inStruct->mem.key = _Inkey;\
                                    inStruct->mem.doFree = destroy;\
                                    inStruct->mem.doShow = show;\
                                    Insert(&(inStruct->mem));\
                                }while(0)


// struct A
// {
//     int a;
//     char b;
//     double d;

//     bst_node_t treelink;
// };

// struct B
// {
//     int a;
//     double d;

//     bst_node_t treelink;
// };

// PVOID* xcalloc(size_t size);
// void show_a(bst_node_t* tree_node);
// struct A* create_a(int a,char b,double d);
// status_t __stdcall DestroyA(bst_node_t* rm);
// struct B* create_b(int a,double d);
// status_t __stdcall DestroyB(bst_node_t* rm);
// void show_b(bst_node_t* tree_node);