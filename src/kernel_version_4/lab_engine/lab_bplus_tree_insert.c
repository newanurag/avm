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

//PBPSPLIT dz_bplus_tree_split_node(PPAGE root_page, PPAGE lpage, PPAGE rpage)
INT dz_bplus_tree_split_node(PPAGE root_page, PBPSPLIT psplit)
{
	INT		ret				= 0;
	UINT 	brk_cnt 		= 0;
	//PVOID  	pagebuf		= NULL;
	PVOID  	left_pagebuf	= NULL;
	PVOID  	right_pagebuf	= NULL;
	PPAGE	left_page 		= NULL;
	PPAGE	right_page 		= NULL;
	PVOID   root_pagebuf	= NULL;
	BPNODE	*pnode 			= NULL;
	BPNODE 	*root_node		= NULL;
	BOOL	done			= FALSE;
	//USHORT is used because all the internal elements in bst are stored at USHORT index
	USHORT 	*stack 			= NULL;
	
	ATOMIC 	stack_idx;
	USHORT	last_element 	= 0;
	KEY		midkey			= 0;
	PBPNODE	midnode			= NULL;

	//Middle element will be at alloc cnt of 125/2  + 1 i.e. 63
	UINT	middle_idx =  (DZ_BPLUS_TREE_MAX_KEYS/2) + 1;
	//LOGTGT("Middle Index = %u\n", middle_idx);

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
			//LOGTGT("Pushed Node Key = %llu, Stack[%d][%d], self = %hu, left = %hu, right = %hu \n",
			//root_node->key, ATOMIC_READ(stack_idx)-1,stack[ATOMIC_READ(stack_idx) -1],
			//root_node->self, root_node->left, root_node->right);
		}

		if (root_node && (root_node->left != BPNODE_NULL)) {
			if (root_node->left == BPNODE_NULL) {
				root_node = NULL;
			} else {
				root_node = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root_node->left]);
			}
		} else {

			IF_NOT_EMPTY_STACK(stack_idx) {
				brk_cnt++;
				POP_STACK(stack, stack_idx, last_element);

				//Fetch the last element node
				pnode = &(((PBPNODE)root_pagebuf)[last_element]);

				if (brk_cnt == middle_idx) {
					//LOGTGT("Split Node Found @ middle_idx = %u\n",middle_idx);
					//LOGTGT("Popped Node Key = %llu, Stack[%d][%d], self = %hu, left = %hu, right = %hu, parent = %hu, brk_cnt = %d \n",
					//pnode->key, ATOMIC_READ(stack_idx)-1,stack[ATOMIC_READ(stack_idx) -1],
					//pnode->self, pnode->left, pnode->right, pnode->parent, brk_cnt);
					done = TRUE;
					break;
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

	//Now at this point we have got our split node and its index
	//LOGTGT("Congratulations ! Split Node Found @ middle_idx = %u\n",middle_idx);
	//dz_bplus_tree_print_node(pnode);
	//NOw to PreOrder Traversal
	root_node = (PBPNODE)root_pagebuf;
	//dz_bplus_tree_traverse_page_preorder(root_pagebuf, root_node);


	//Do the preorder traveral till N/2 + 1 node is encountered
	left_page = dz_page_alloc();
	left_pagebuf = kmap(left_page);
	memset(left_pagebuf, 0, BPLUS_TREE_PAGE_SIZE);
	kunmap(left_page);

	right_page = dz_page_alloc();
	right_pagebuf = kmap(right_page);
	memset(right_pagebuf, 0, BPLUS_TREE_PAGE_SIZE);
	kunmap(right_page);

	//Initialize stack for preorder traversal
	last_element 	= 0;
	MEMSET(stack);
	ATOMIC_SET(stack_idx, 0);
	PUSH_STACK(stack, stack_idx, root_node->self);

	//Set the middle key and node

	midnode = pnode;
	midkey  = midnode->key;

	while(TRUE) {

		IF_NOT_EMPTY_STACK(stack_idx) { //Check if stack is not empty
				POP_STACK(stack, stack_idx, last_element);
				//Fetch the popped node
				pnode = &(((PBPNODE)root_pagebuf)[last_element]);
			
				if (pnode->key < midkey) {
					ret = dz_bplus_tree_insert_key_data(left_page, pnode->key, NULL );
				}

				if (pnode->key >= midkey) {
					ret = dz_bplus_tree_insert_key_data(right_page, pnode->key, NULL );
				}
				if (pnode->right != BPNODE_NULL) {
					root_node = &(((PBPNODE)root_pagebuf)[pnode->right]);
					PUSH_STACK(stack, stack_idx, root_node->self);
				}
				if (pnode->left != BPNODE_NULL) {
					root_node = &(((PBPNODE)root_pagebuf)[pnode->left]);
					PUSH_STACK(stack, stack_idx, root_node->self);
				}

		} else {
			break;
		}
	}
	kunmap(root_page);
	//Now at this point, we have got both left and right subtree in left_page and right_page
	psplit->left_page 	= left_page;
	psplit->right_page 	= right_page;
	psplit->split_node 	= midnode;
	return SUCCESS;
}

//Since we are allocating from page, so we dont want recursion, because
//it will unnecessarily lead to multiple kmap/kunmap functions.
//Insted we will create binary tree iteratively.
//Also better not to keep pointer as left or right.
//We will keep the index in page as left or right
//A mixture of construction of bst in 4k total array size where each element
//is of 64 bytes size.

INT dz_bplus_tree_insert_key_data(PPAGE root_page, KEY key, PDZ_BPLUS_LEAF_NODE data )
{
	PDZ_BPLUS_TREE_NODE 		pnode			= NULL;
	PDZ_BPLUS_TREE_NODE			root 			= NULL;
	PDZ_BPLUS_TREE_NODE			parent 			= NULL;
	PVOID   					root_pagebuf	= NULL;
	PDZ_BPLUS_METADATA_NODE		pmetad			= NULL;			


	root_pagebuf = kmap(root_page);

	//Get Metadata header address
	pmetad = (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);

	//PRINT_UINT(pmetad->alloc_cnt);
	if (pmetad->alloc_cnt == DZ_BPLUS_TREE_MAX_KEYS) {
		kunmap(root_page);

		return BPLUS_NODE_PAGE_FULL;
	}
	

	if (pmetad->alloc_cnt == 0) { //Root is not yet present. This is the first time

		//Set Data
		root = (PDZ_BPLUS_TREE_NODE)root_pagebuf;
		MEMSET(root);	//It will reset only node size
		root->key 		= key;
		root->self		= 0;
		root->left 		= BPNODE_NULL;
		root->right 	= BPNODE_NULL;
		root->parent 	= 0; //Special case. Root is at index zero

		if (data) {
			root->data		= data; //Data is always set at root
			SET_BPNODE_TYPE(root->type, BPNODE_TYPE_LEAF_ONLY);
		} else {
			root->data		= NULL;
			SET_BPNODE_TYPE(root->type, BPNODE_TYPE_INTERNAL_FIRST);
		}

		//Update Metadata
		pmetad->alloc_cnt++;
		pmetad->minkey = key;
		pmetad->maxkey = key;
		pmetad->curr_page = root_page;
		pmetad->level = 0;
		kunmap(root_page);
		return SUCCESS;
	}

	//Iteratively inserting the node in this tree to avoid recursion in kernel
	//because kernel stack is limited
	//Root will always be at starting position of the page
	root = (PDZ_BPLUS_TREE_NODE)root_pagebuf;

	pnode = root;

	while(true) {
		if(key < pnode->key) {
			parent = pnode;
			if (pnode->left == BPNODE_NULL) {
				break;
			}
			pnode = &(root[pnode->left]);
		} else {
			parent = pnode;
			if (pnode->right == BPNODE_NULL) {
				break;
			}
			pnode = &(root[pnode->right]);
		}
	}

	//Allocate a new internal node from the page.
	//alloc_cnt will be used to serve that purpose
	pnode = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[pmetad->alloc_cnt]);


	//Update Data
	MEMSET(pnode);
	pnode->key 		= key;
	pnode->self 	= pmetad->alloc_cnt;
	pnode->parent 	= parent->self;
	pnode->left		= BPNODE_NULL;
	pnode->right	= BPNODE_NULL;

	if(key < parent->key) {
		parent->left = pnode->self;
	} else {
		parent->right = pnode->self;
	}

	//Update Metadata
	//Set the min and max keys
	pmetad->alloc_cnt++;
	if (key < pmetad->minkey) {
		pmetad->minkey = key;
	} else  if (key > pmetad->maxkey) {
		pmetad->maxkey = key;

	}
	if (data) {
		pnode->data		= data;
		SET_BPNODE_TYPE(root->type, BPNODE_TYPE_LEAF_ONLY);
	} else {
		pnode->data		= NULL;
		if (pmetad->alloc_cnt == DZ_BPLUS_TREE_MAX_KEYS) {
			SET_BPNODE_TYPE(root->type, BPNODE_TYPE_INTERNAL_ONLY);
		} else {
			SET_BPNODE_TYPE(root->type, BPNODE_TYPE_INTERNAL_LAST);
		}
	}
	kunmap(root_page);

	return SUCCESS; //Always Top of the tree root
}

PVOID	dz_bplus_tree_insert_data(PPAGE root_page, PDZ_BPLUS_LEAF_NODE data )
{
	BPSPLIT 	split;
	KEY			split_key;
	UINT		level;
	PPAGE 		data_level_page;
	KEY			minkey;
	KEY			maxkey;
	KEY			key;
	INT 		ret 			= SUCCESS;
	PPAGE		tmp_root_page 	= root_page;
	PPAGE		root_parent 	= NULL;

	PVOID   	root_pagebuf	= NULL;
	PBPMTDT		pmetad			= NULL;			
	PBPNODE 	root_node;
	PBPNODE 	current_node;
	PBPNODE		prev_node;

	root_parent = dz_bplus_tree_get_parent_page(root_page);
	PRINT_POINTER(root_parent);

	level = dz_bplus_tree_get_page_level(root_page);
	//Simplifying things:
	//If we are level zero then it means at data level, i.e. data nodes will be inserted
	if (level == 0) {
		ret = dz_bplus_tree_insert_key_data(tmp_root_page, data->key, data );
		if (ret == BPLUS_NODE_FULL) {
			ret = dz_bplus_tree_split_node(tmp_root_page, &split);
			LOGTGT("Maximum keys encountered. Split Key = %llu\n",split.split_node->key );
			//Now we have got the node splitted and got both left and right pages
			//We need to insert the current key in the right page
			ret = dz_bplus_tree_insert_key_data(split.right_page, data->key, data );

			//Since we are at level zero so its parent will always be NULL
			split_key  = split.split_node->key;
		
			dz_bplus_tree_reset_page(root_page);

			//Set the root page left and right pointers
			dz_bplus_tree_set_next_page(root_page, split.right_page);
			dz_bplus_tree_set_prev_page(root_page, split.left_page);

			//Now Set both left and right parent as root_page
			dz_bplus_tree_set_parent_page(split.right_page, root_page);
			dz_bplus_tree_set_parent_page(split.left_page, root_page);
			
			//Now Set root parent to NULL
			dz_bplus_tree_set_parent_page(root_page, NULL);

			//Move the middle node to root, but with NULL data pointer
			ret = dz_bplus_tree_insert_key_data(root_page, split_key, NULL );
				
			//Set the level of root to 1
			dz_bplus_tree_set_page_level(root_page, 1);

			LOGTGT("Printing root after moving up\n");
			dz_bplus_tree_print_page(root_page);

			LOGTGT("Printing LEFT Subtree @ page %p after root moved up\n", split.left_page);
			dz_bplus_tree_traverse_page(split.left_page, ENUM_TRAVERSAL_TYPE_PREORDER);

			LOGTGT("Printing RIGHT Subtree @ page %p after root moved up\n", split.right_page);
			dz_bplus_tree_traverse_page(split.right_page, ENUM_TRAVERSAL_TYPE_PREORDER);

		} //End of IF Node full

		return root_page;

	} //End of if level == 0



	//Now we have to insert data by going through multiple levels
	//So we have to start searching from top/root and then go to the lowest
	//page i.e. level 0 where the current data can be kept.
	//Once we insert the data page at the lowest level, then we have to see if
	//the higher levels needs splitting and insertion, until we go to the top again.
	//In between it may required that we might have to split internal non-leaf nodes
	
	data_level_page = dz_bplus_tree_find_page_at_data_level(root_page, data->key);
	tmp_root_page = data_level_page;
	LOGTGT("Searching from level %u. Data Page Found %p\n", level, data_level_page);

	ret = dz_bplus_tree_insert_key_data(tmp_root_page, data->key, data );

	if (ret != BPLUS_NODE_FULL) {

		//Data is properly inserted at data level pages/leaf nodes
		return tmp_root_page;

	} else {

		ret = dz_bplus_tree_split_node(tmp_root_page, &split);
		LOGTGT("Maximum keys encountered. Split Key = %llu\n",split.split_node->key );

		//Now we have got the node splitted and got both left and right pages
		//We need to insert the current key in the right page
		ret = dz_bplus_tree_insert_key_data(split.right_page, data->key, data );
			
		root_parent = dz_bplus_tree_get_parent_page(tmp_root_page);
		split_key  = split.split_node->key;
		level = dz_bplus_tree_get_page_level(tmp_root_page);

		//Now we have both left and right pages allocated, and save the parent pointer as well
		//We can free the tmp_root_page
		dz_page_free(tmp_root_page);

	}

	tmp_root_page = root_parent;

	root_pagebuf = kmap(tmp_root_page);

	//Get Metadata header address
	pmetad 	= (PDZ_BPLUS_METADATA_NODE)(root_pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);
	minkey 	= pmetad->minkey;
	maxkey 	= pmetad->maxkey;
	key		= split_key;

	//Now we need to attached these pages to the parent in their respective down page pointer
	//First attach prev and next pages
	if (split_key < minkey) {
		dz_bplus_tree_set_prev_page(root_page, split.left_page);
		dz_bplus_tree_set_parent_page(split.left_page, tmp_root_page);
	} else {
		dz_bplus_tree_set_next_page(root_page, split.right_page);
		dz_bplus_tree_set_parent_page(split.right_page, tmp_root_page);
	}


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








	//Move the middle node to root, but with NULL data pointer
	ret = dz_bplus_tree_insert_key_data(root_page, split_key, NULL );
			
	//Increment the level of root
	level++;
	dz_bplus_tree_set_page_level(root_page, level);
		

	return tmp_root_page;
}

PPAGE dz_bplus_tree_insert(PPAGE root_page, PDZ_BPLUS_LEAF_NODE data)
{
	if (root_page == NULL) {
		root_page = dz_page_alloc();
		dz_bplus_tree_reset_page(root_page);
		BPTREE_ROOT = root_page;
	}

	dz_bplus_tree_insert_data(root_page, data);

	return root_page; //Always Top of the tree root
}
