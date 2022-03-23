/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"
extern PPAGE			BPTREE_ROOT;

//Return Right Page after splitting the bst present in PAGE
//First do inorder traversal. Visualize it as a sorted list
//Go to the middle of the list.
//That middle of the list is the bst node where splitting is required
//Return that node index
PBPNODE dz_bplus_tree_find_split_node(PPAGE root_page)
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
			LOGTGT("Pushed Node Key = %llu, Stack[%d][%d], self = %hu, left = %hu, right = %hu \n",
			root_node->key, ATOMIC_READ(stack_idx)-1,stack[ATOMIC_READ(stack_idx) -1],
			root_node->self, root_node->left, root_node->right);
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

				if (brk_cnt == middle_idx) {
					LOGTGT("Split Node Found @ middle_idx = %u\n",middle_idx);
					LOGTGT("Popped Node Key = %llu, Stack[%d][%d], self = %hu, left = %hu, right = %hu, parent = %hu, brk_cnt = %d \n",
					pnode->key, ATOMIC_READ(stack_idx)-1,stack[ATOMIC_READ(stack_idx) -1],
					pnode->self, pnode->left, pnode->right, pnode->parent, brk_cnt);
					kunmap(root_page);
					done = TRUE;
					return pnode;
					//break;
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

	return NULL;
}

//This function searched the down page pointer from the node where this key
//can easily fit.
//Traversing in an inorder fashion and checking the current key
//with current node and prev node.
//The current node and prev node role varies when traversing left or right

PPAGE dz_bplus_tree_find_down_page(PPAGE root_page, KEY key)
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			
	PBPNODE 	root_node;
	PBPNODE 	current_node;
	PBPNODE	prev_node;

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);

	root_node = (PDZ_BPLUS_TREE_NODE)root_pagebuf;
	prev_node = root_node;
	current_node = root_node;
	
	while(true) {
		if (key < current_node->key && current_node->left != BPLUS_NODE_NULL) {
				prev_node = current_node;
				current_node = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[current_node->left]);

				if (key > prev_node->key && key <= current_node->key) {
					return prev_node->down;
				} else {
					continue;
				}

		} else if (key > current_node->key && current_node->right != BPLUS_NODE_NULL) {
				prev_node = current_node;
				current_node = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[current_node->right]);
				if (key >= prev_node->key && key < current_node->key) {
					return prev_node->down;
				} else {
					continue;
				}

		} else {
			BUG_ON(1);
		}
	}
}


//This function will give the page to which all the data nodes are connected
//It means, that this page will be a level 0
PPAGE dz_bplus_tree_find_page_at_data_level(PPAGE root_page, KEY key)
{	
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			
	KEY maxkey = 0;
	KEY minkey = 0;
	PPAGE	next_page = NULL;
	PPAGE	prev_page = NULL;
	//PPAGE	head_page = root_page;
	UINT	level = 0;


	while(true) {
		root_pagebuf = kmap(root_page);

		//Get Metadata header address
		pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
		maxkey = pmetad->maxkey;
		minkey = pmetad->minkey;
		PRINT_KEY(minkey);
		PRINT_KEY(maxkey);

		if (key < minkey) {
			prev_page = pmetad->prev_page;
			//For safer side check the level
			level = pmetad->level;
			kunmap(root_page);

			if (prev_page == NULL) {
				//We have to insert the data node here
				LOGTGT("Returning Left Data Page %p @ level = %u\n", root_page, level);
				return root_page;
			} else {
				root_page = prev_page;
				LOGTGT("Continuing to left side page %p\n", root_page);
				continue;
			}
		} //End of if key < minkey

		if (key > maxkey) {
			next_page = pmetad->next_page;
			//For safer side check the level
			level = pmetad->level;
			kunmap(root_page);

			if (next_page == NULL) {
				//We have to insert the data node here
				LOGTGT("Returning Right Data Page %p @ level = %u\n", root_page, level);
				return root_page;

			} else {
				root_page = next_page;
				LOGTGT("Continuing to right side page %p\n", root_page);
				continue;
			}

		} //End of if key > minkey

		if (key >= minkey && key <= maxkey) {

			kunmap(root_page);
			//Here we have to traverse the internal binary search tree and check its down nodes
			return dz_bplus_tree_find_down_page(root_page, key);
		}

	} //End of while loop

}

INT dz_bplus_tree_find_key(PPAGE root_page, KEY key)
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			
	KEY maxkey = 0;
	KEY minkey = 0;
	BPNODE 	*root_node;

	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	maxkey = pmetad->maxkey;
	minkey = pmetad->minkey;
	if (key < minkey) {
		kunmap(root_page);
		return BPLUS_NODE_KEY_IS_LESSER;
	}

	if (key > maxkey) {
		kunmap(root_page);
		return BPLUS_NODE_KEY_IS_GREATER;
	}

	root_node = (PDZ_BPLUS_TREE_NODE)root_pagebuf;
	
	while(true) {
		if (key == root_node->key) {
			kunmap(root_page);
			return	BPLUS_NODE_KEY_FOUND;

		} else if (key < root_node->key && root_node->left != BPLUS_NODE_NULL) {
				root_node = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root_node->left]);

		} else if (key > root_node->key && root_node->right != BPLUS_NODE_NULL) {
				root_node = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root_node->right]);

		} else if ((root_node->left == BPLUS_NODE_NULL) && (root_node->right == BPLUS_NODE_NULL)) {
			kunmap(root_page);
			return BPLUS_NODE_KEY_NOT_FOUND;

		} else {
			BUG_ON(1);
		}
	}
}

INT dz_bplus_tree_search_key(PPAGE root_page, KEY key)
{
	//PPAGE prev_page = NULL;
	//PPAGE next_page = NULL;
	PPAGE root		= root_page;
	INT ret = 0;

	while(true) {
		ret = dz_bplus_tree_find_key(root, key);

		switch (ret) {

			case BPLUS_NODE_KEY_FOUND: {
				return ret;
			}

			case BPLUS_NODE_KEY_NOT_FOUND: {
				return ret;
			}

			case BPLUS_NODE_KEY_IS_LESSER: {
				root = dz_bplus_tree_get_prev_page(root);
				if (!root) {
					return BPLUS_NODE_KEY_NOT_FOUND;
				} else {
					continue;
				}
			}

			case BPLUS_NODE_KEY_IS_GREATER: {
				root = dz_bplus_tree_get_next_page(root);
				if (!root) {
					return BPLUS_NODE_KEY_NOT_FOUND;
				} else {
					continue;
				}
			}
		} //End of Switch
	} //End of While
}
