#include "BST.h"
#include<stdio.h>


bst_t* create_bst(void)
{
    bst_t* p_new_bst = (bst_t*) xcalloc(sizeof(bst_t));

    p_new_bst->nr_elements = 0;
    p_new_bst->p_root_node = NULL;

    return p_new_bst;
}

status_t show(bst_t* p_bst,char* msg)
{
    puts(msg);
    __show(p_bst->p_root_node);

    return SUCCESS;
}

void __show(bst_node_t* bst_node)
{
    if(bst_node == NULL)
        return;
    
    __show(bst_node->left);
    bst_node->doShow(bst_node);
    __show(bst_node->right);
}

status_t insert_bst(bst_t* p_bst,bst_node_t* p_new_node)
{
    if(p_bst->p_root_node == NULL)
    {
        p_bst->p_root_node = p_new_node;
        p_bst->nr_elements++;
        return SUCCESS;
    }

    bst_node_t* p_run = NULL;
    p_run = p_bst->p_root_node;

    while(1)
    {
        if(p_new_node->key < p_run->key)
        {
            if(p_run->left == NULL)
            {
                p_run->left = p_new_node;
                p_new_node->parent = p_run;
                p_bst->nr_elements++;
                break;
            }

            p_run = p_run->left;
        }
        else
        {
            if(p_run->right == NULL)
            {
                p_run->right = p_new_node;
                p_new_node->parent = p_run;
                p_bst->nr_elements++;
                break;

            }
            
            p_run = p_run->right;
        }
    }
    return SUCCESS;
}

status_t remove_bst(bst_t* p_bst,key_t key)
{
    return SUCCESS;
}

status_t search_key(bst_t* p_bst,key_t key,bst_node_t** out_node)
{
    bst_node_t* p_run = NULL;
    p_run = p_bst->p_root_node;

    while(1)
    {
        if(p_run->key == key)
        {
            *out_node = p_run;
            break;
        }
        else if(key < p_run->key)
        {
            if(p_run->left == NULL)
            {
                *out_node = NULL;
                return BST_KEY_NOT_FOUND;
            }
            p_run = p_run->left;
        }
        else
        {   
            if(p_run->right == NULL)
            {
                *out_node = NULL;
                return BST_KEY_NOT_FOUND;
            }
            p_run = p_run->right;
        }   
    }

    return SUCCESS;

}

status_t destroy_bst(bst_t** pp_bst)
{
    return __destroy_bst((*pp_bst)->p_root_node);
}

status_t __destroy_bst(bst_node_t* p_root_node)
{
    if(p_root_node == NULL)
        return SUCCESS;
    
    __destroy_bst(p_root_node->left);
    __destroy_bst(p_root_node->right);

    p_root_node->doFree(p_root_node);
    return SUCCESS;
}