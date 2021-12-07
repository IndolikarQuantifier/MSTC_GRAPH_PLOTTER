#include "BST.h"

#define MEMORY_NOT_FOUND BST_KEY_NOT_FOUND

static bst_t* GetResourceAllocator();
key_t GenerateKey(unsigned long int seed);
status_t GetGraphResource(key_t key,bst_node_t** OutNode);
status_t Insert(bst_node_t* new_node);
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