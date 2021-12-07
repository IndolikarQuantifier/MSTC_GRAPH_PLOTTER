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
    bst_node_t* rm_node = NULL;
    search_key(p_bst,key,&rm_node);
    if(rm_node == NULL)
        return BST_KEY_NOT_FOUND;
    /* 
        case 1:

        Deleted Node has left subtree NULL and right subtree is non-NULL

    */
    if(rm_node->left == NULL)
    {
        if(rm_node->parent == NULL)
            p_bst->p_root_node = rm_node->right;
        else if(rm_node == rm_node->parent->left)
            rm_node->parent->left = rm_node->right;
        else if(rm_node == rm_node->parent->right)
            rm_node->parent->right = rm_node->right;
        
        if(rm_node->right)
            rm_node->right->parent = rm_node->parent;
    }
    /* 
        case 2:

        Deleted Node has right subtree NULL and left subtree is non-NULL
    */
    else if(rm_node->right == NULL)
    {
        if(rm_node->parent == NULL)
            p_bst->p_root_node = rm_node->left;
        else if(rm_node == rm_node->parent->left)
            rm_node->parent->left = rm_node->left;
        else if(rm_node == rm_node->parent->right)
            rm_node->parent->right = rm_node->left;
        
        if(rm_node->left)
            rm_node->left->parent = rm_node->parent;
    }
    // case 3:
    else if(rm_node->left != NULL && rm_node->right != NULL)
    {
        bst_node_t* rm_successor = NULL;
        rm_successor = rm_node->right;
        while(rm_successor->left == NULL)
            rm_successor = rm_successor->left;

        if(rm_successor == rm_node->right)
        {
            rm_successor->parent->left = rm_successor->right;
            if(rm_successor->right)
                rm_successor->right->parent = rm_successor->parent;

            // Take rm_node right subtree
            rm_successor->right = rm_node->right;
            rm_successor->right->parent = rm_successor;

        }        

        // Take rm_node left subtree
        rm_successor->left = rm_node->left;
        rm_successor->left->parent = rm_successor;

        // Take rm_node parent 
        if(rm_node->parent == NULL)
            p_bst->p_root_node = rm_successor;
        else if(rm_node == rm_node->parent->left)
            rm_node->parent->left = rm_successor;
        else if(rm_node == rm_node->parent->right)
            rm_node->parent->right = rm_successor;

        rm_successor->parent = rm_node->parent;
    }

    rm_node->doFree(rm_node);

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