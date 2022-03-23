/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"
extern PPAGE			BPTREE_ROOT;

//Get the current middle key 
KEY dz_bplus_tree_get_mid_key(PPAGE root_page)
{
	UINT brk_cnt = 0;
	PVOID   root_pagebuf	= NULL;
	BPNODE	*pnode 			= NULL;
	BPNODE 	*root_node;
	BOOL	done			= FALSE;
	//USHORT is used because all the internal elements in bst are stored at USHORT index
	USHORT 	*stack 			= NULL;
	
	ATOMIC 	stack_idx;
	USHORT	last_element = 0;

	//Middle element will be at alloc cnt of 125/2  + 1 i.e. 63
	UINT	middle_idx =  (DZ_BPLUS_TREE_MAX_KEYS/2) + 1;
	LOGTGT("Middle Index = %u\n", middle_idx);

	//Now Traverse the bst tree iteratively using a stack created by kmalloc
	//USHORT is taken because it is the type of left,right and parent in BPNODE
	stack = dz_kmalloc(middle_idx, sizeof(USHORT));
	MEMSET(stack);
	ATOMIC_SET(stack_idx, 0);
	//Check for NULLs
	

	//Get the first node address
	root_pagebuf = kmap(root_page);
	root_node = (PBPNODE)root_pagebuf;

	while(!done) {

		if (root_node) {
			PUSH_STACK(stack, stack_idx, root_node->self);
			//LOGTGT("Pushed Key = %llu\n", root_node->key);
		}

		if (root_node && (root_node->left != BPNODE_NULL)) {
			if (root_node->left == BPNODE_NULL) {
				root_node = NULL;
			} else {
				root_node = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root_node->left]);
			}
		} else {

			IF_NOT_EMPTY_STACK(stack_idx) { //Check if stack is not empty
				brk_cnt++;
				POP_STACK(stack, stack_idx, last_element);

				//Fetch the last element node
				pnode = &(((PBPNODE)root_pagebuf)[last_element]);
				//LOGTGT("Popped Key = %llu\n", pnode->key);

				if (brk_cnt == middle_idx) {
					kunmap(root_page);
					return pnode->key;
				}

				if (pnode->right == BPNODE_NULL) {
					root_node = NULL;
				} else {
					root_node = &(((PBPNODE)root_pagebuf)[pnode->right]);
				}
			} else {
				done = TRUE;
			}
		}
	}

	return BPLUS_NODE_NOT_FOUND;
}


//Get the min key from the bst page
KEY dz_bplus_tree_get_min_key(PPAGE root_page) 
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			
	KEY minkey = 0;

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	minkey = pmetad->minkey;
	kunmap(root_page);

	return minkey;
}

//Get the max key from the bst page
KEY dz_bplus_tree_get_max_key(PPAGE root_page) 
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			
	KEY maxkey = 0;

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	maxkey = pmetad->maxkey;
	kunmap(root_page);

	return maxkey;
}

UINT dz_bplus_tree_get_num_keys(PPAGE root_page)
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			
	UINT cnt = 0;

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	cnt = pmetad->alloc_cnt;
	kunmap(root_page);

	return cnt;

}

//This is buggy. Idealy parent key will be from another page because 
//parent will be respect to key and not bst parent
INT dz_bplus_tree_get_parent_key(PPAGE root_page, KEY key, KEY *parent_key)
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			
	KEY maxkey = 0;
	KEY minkey = 0;
	BPNODE 	*root_node;
	BPNODE 	*parent_node;

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	maxkey = pmetad->maxkey;
	minkey = pmetad->minkey;
	if (key < minkey || key > maxkey) {
		kunmap(root_page);
		return BPLUS_NODE_NOT_FOUND;
	}

	root_node = (PDZ_BPLUS_TREE_NODE)root_pagebuf;
	
	while(true) {
		if (key == root_node->key) {
			parent_node = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root_node->parent]);
			kunmap(root_page);
			*parent_key = parent_node->key;
			return SUCCESS;

		} else if (key < root_node->key && root_node->left != BPLUS_NODE_NULL) {
				root_node = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root_node->left]);

		} else if (key > root_node->key && root_node->right != BPLUS_NODE_NULL) {
				root_node = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root_node->right]);

		} else if ((root_node->left == BPLUS_NODE_NULL) && (root_node->right == BPLUS_NODE_NULL)) {
			kunmap(root_page);
			return BPLUS_NODE_NOT_FOUND;

		} else {
			BUG_ON(1);
		}
	}


	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	key = pmetad->maxkey;
	kunmap(root_page);

	return maxkey;
}
//Get the Next page pointer from this bst page
PPAGE dz_bplus_tree_get_next_page(PPAGE root_page) 
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			
	PPAGE page = NULL;

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	page = pmetad->next_page;
	kunmap(root_page);

	return page;
}

//Set the Next page pointer 
VOID dz_bplus_tree_set_next_page(PPAGE root_page, PPAGE next_page) 
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	pmetad->next_page = next_page;
	kunmap(root_page);
	return;
}

//Get the Previous page pointer from this bst page
PPAGE dz_bplus_tree_get_prev_page(PPAGE root_page) 
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			
	PPAGE page = NULL;

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	page = pmetad->prev_page;
	kunmap(root_page);
	return page;
}

//Set the Previous page pointer
VOID dz_bplus_tree_set_prev_page(PPAGE root_page, PPAGE prev_page) 
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	pmetad->prev_page = prev_page;
	kunmap(root_page);
	return;
}

//Get the Parent page pointer of this bst page
PPAGE dz_bplus_tree_get_parent_page(PPAGE root_page) 
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			
	PPAGE page = NULL;

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	page = pmetad->parent_page;
	kunmap(root_page);
	return page;
}

//Set the Parent page pointer of this page
VOID dz_bplus_tree_set_parent_page(PPAGE root_page, PPAGE parent_page) 
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	pmetad->parent_page = parent_page;
	kunmap(root_page);
	return;
}

//Get the Level of this bst page
UINT dz_bplus_tree_get_page_level(PPAGE root_page) 
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			
	UINT level = 0;

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	level = pmetad->level;
	kunmap(root_page);
	return level;
}

//Set the Level of this bst page
VOID dz_bplus_tree_set_page_level(PPAGE root_page, UINT level) 
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	pmetad->level = level;
	kunmap(root_page);
	return ;
}

//Reset the page contents
VOID dz_bplus_tree_reset_page(PPAGE root_page) 
{
	PVOID   					root_pagebuf= NULL;

	root_pagebuf = kmap(root_page);
	memset(root_pagebuf, 0, BPLUS_TREE_PAGE_SIZE);
	kunmap(root_page);
	return;
}
