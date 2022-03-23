/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"
#define BINARY_TREE_INSERT(arg_tree_root, arg_data)		\
	{																\
		arg_tree_root = dz_binary_tree_insert(arg_tree_root, arg_data);	\
	}

PDZ_BINARY_NODE 	tree_root 	= NULL;
PDZ_BINARY_NODE 	clone_root 	= NULL;
PDZ_BINARY_NODE 	mirror_root 	= NULL;
PDZ_BINARY_NODE 	head		 = NULL;
PDZ_TREE_NODE		deep_tree_root = NULL;
PDZ_TREE_NODE		deep_clone_root = NULL;

//QUEUE Metadata
static ULONGLONG *local_queue		= NULL;
static	uint local_queue_front		= 0;
static	uint local_queue_rear		= 0;
static	uint local_queue_size	= 32;

//Stack Metadata
static ULONGLONG *local_stack		= NULL;
static	INT local_stack_idx		= 0;
static	uint local_stack_size	= 32;


//Map Metadata
static 	PDZ_MAP	local_map		= NULL;
static	INT 	local_map_idx	= 0;
static	UINT 	local_map_size	= 32;

static UINT sequence = 1;


//Stack Macros
#define IF_EMPTY_STACK_() if (local_stack_idx == -1)
#define IF_STACK_EMPTY_() if (local_stack_idx == -1)

#define CREATE_STACK(arg_s, arg_s_size)	\
	arg_s = dz_binary_tree_create_stack(arg_s_size)

#define	RESET_STACK()	\
	dz_binary_tree_reset_stack()

#define	PUSH(arg_element)	\
	dz_binary_tree_push_stack((void *)(arg_element))

#define	POP(arg_element)	\
	arg_element = (PDZ_BINARY_NODE)dz_binary_tree_pop_stack()

//Queue Macros
#define CREATE_QUEUE(arg_q, arg_q_size)	\
	arg_q = dz_binary_tree_create_queue(arg_q_size)

#define	QUEUE(arg_element)	\
	dz_binary_tree_enqueue((void *)(arg_element))

#define	RESET_QUEUE()	\
	dz_binary_tree_reset_queue()

#define	DQUEUE(arg_element)	\
	arg_element = (PDZ_BINARY_NODE)dz_binary_tree_dequeue()

#define	DEQUEUE(arg_element)	DQUEUE(arg_element)

//Map Macros
#define CREATE_MAP()	\
	local_map = dz_binary_tree_create_map(local_map_size)

#define SET_MAP(arg_key, arg_value)	\
	dz_binary_tree_set_map(arg_key, arg_value)

#define	GET_MAP(arg_key)	\
	(LONG *)dz_binary_tree_get_map(arg_key)

#define	PRINT_MAP()	\
dz_binary_tree_print_map()

PDZ_MAP dz_binary_tree_create_map(uint sz)
{
	local_map = dz_kmalloc(sizeof(ULONGLONG) * sz, GFP_KERNEL);
	if (local_map) {
		LOGTGT("HASHMAP Created of size %u\n", sz);
	}
	dz_binary_tree_reset_map();
	return local_map;
}

VOID dz_binary_tree_reset_map(VOID)
{
	MEMSET(local_map);
	local_map_idx = 0;
	for (local_map_idx = 0; local_map_idx < local_map_size; local_map_idx++) {

		local_map[local_map_idx].key = 0;
		local_map[local_map_idx].value = NULL;
	}
}

PVOID dz_binary_tree_get_map(ULONGLONG key)
{
	for (local_map_idx = 0; local_map_idx < local_map_size; local_map_idx++) {

		if (local_map[local_map_idx].key == key) {
			LOGTGT("MAP->GET: Key[%u] = %llu, Value = %p\n", local_map_idx,
			local_map[local_map_idx].key,
			local_map[local_map_idx].value);
			return local_map[local_map_idx].val;
		}
	}
	
	return NULL;
}

VOID dz_binary_tree_set_map(ULONGLONG key, PVOID value)
{
	for (local_map_idx = 0; local_map_idx < local_map_size; local_map_idx++) {

		//Look for empty space in map and insert the key value there
		if (local_map[local_map_idx].key == 0) {
			local_map[local_map_idx].key = key;
			local_map[local_map_idx].val = value;

			LOGTGT("MAP->SET: Key[%u] = %llu, Value = %p\n", local_map_idx,
			local_map[local_map_idx].key,
			local_map[local_map_idx].value);
			break;
		}
	}
	mb();
}

VOID dz_binary_tree_print_map(VOID)
{
	for (local_map_idx = 0; local_map_idx < local_map_size; local_map_idx++) {

		LOGTGT("MAP->PRINT: Key[%u] = %llu, Value = %p\n", local_map_idx,
		local_map[local_map_idx].key, local_map[local_map_idx].value);
	}
}

ULONGLONG* dz_binary_tree_create_stack(uint sz)
{
	local_stack = dz_kmalloc(sizeof(ULONGLONG) * sz, GFP_KERNEL);
	if (local_stack) {
		LOGTGT("STACK Created of size %u\n", sz);
	}
	dz_binary_tree_reset_stack();
	return local_stack;
}

VOID dz_binary_tree_reset_stack(VOID)
{
	MEMSET(local_stack);
	local_stack_idx = -1;
}


VOID dz_binary_tree_push_stack(PVOID element)
{
	local_stack_idx++;
	local_stack[local_stack_idx] = (ULONGLONG)element;
}

PVOID dz_binary_tree_pop_stack(VOID)
{
	return (PVOID)local_stack[local_stack_idx--];
}

ULONGLONG* dz_binary_tree_create_queue(uint sz)
{
	local_queue = dz_kmalloc(sizeof(ULONGLONG) * sz, GFP_KERNEL);
	if (local_queue) {
		LOGTGT("QUEUE Created of size %u\n", sz);
	}
	return local_queue;
}

VOID dz_binary_tree_reset_queue(VOID)
{
	MEMSET(local_queue);
	local_queue_front = local_queue_rear = 0;
}

VOID dz_binary_tree_enqueue(PVOID element)
{
	local_queue[local_queue_rear++] = (ULONGLONG)element;
}

PVOID dz_binary_tree_dequeue(VOID)
{
	if (local_queue_front == local_queue_rear) return NULL;
	return (PVOID)local_queue[local_queue_front++];
}

RVOID dz_binary_tree_print_node(PDZ_BINARY_NODE node)
{
	BOOL flag = TRUE;
	if (!node) {
		LOGTGT("Node Print: Binary Node is NULL\n");
		return;
	}


	if (flag == TRUE) {
		LOGTGT("[-----------BINARY   NODE----------]\n");
		LOGTGT("[                                  ]\n");
		LOGTGT("[ NODE           = %p]\n", node);
		LOGTGT("[ NODE->data     = %ld]\n", node->data);
		LOGTGT("[ NODE->left     = %ld %p]\n", node->left_data, node->left);
		LOGTGT("[ NODE->right    = %ld %p]\n", node->right_data, node->right);
#ifdef _LAB_ENGINE_BINARY_TREE_H_DEBUG_
		LOGTGT("[ NODE->parent   = %ld %p]\n", node->parent_data, node->parent);
		LOGTGT("[ NODE->root     = %p]\n", node->root);
		LOGTGT("[ NODE->sequence = %u]\n", node->sequence);
		LOGTGT("[ QUEUE->qrear    = %u]\n", local_queue_rear);
		LOGTGT("[ QUEUE->qfront   = %u]\n", local_queue_front);
#endif
		LOGTGT("[----------------------------------]\n");
	}
}

PDZ_BINARY_NODE dz_binary_tree_alloc_node(VOID)
{
    PDZ_BINARY_NODE node = DZ_BINARY_ALLOC_NODE();
	if (!node) {
		LOGTGTE("%s","Error, Unable to allocate memory for interval tree node\n");
		RETURNN;
	}
	MEMSET(node);
	return node;
}

PDZ_TREE_NODE dz_tree_alloc_node(VOID)
{
    PDZ_TREE_NODE node = dz_kmalloc(sizeof(DZ_TREE_NODE), GFP_KERNEL);
	if (!node) {
		LOGTGTE("%s","Error, Unable to allocate memory for interval tree node\n");
		RETURNN;
	}
	MEMSET(node);
	return node;
}

RVOID dz_binary_tree_postorder(PDZ_BINARY_NODE node)
{
	if (node->left) {
		dz_binary_tree_postorder(node->left);
	}
	if (node->right) {
		dz_binary_tree_postorder(node->right);
	}
	LOGTGT("PostOrder Recursion Data = %ld\n", node->data);
	//dz_binary_tree_print_node(node);
}

RVOID dz_binary_tree_preorder(PDZ_BINARY_NODE node)
{
	//dz_binary_tree_print_node(node);
	LOGTGT("PreOrder Recursion Data = %ld\n", node->data);
	if (node->left) {
		dz_binary_tree_preorder(node->left);
	}
	if (node->right) {
		dz_binary_tree_preorder(node->right);
	}
}

RVOID dz_binary_tree_inorder(PDZ_BINARY_NODE node)
{
	if (node->left) {
		dz_binary_tree_inorder(node->left);
	}
	LOGTGT("InOrder Recursion Data = %ld\n", node->data);
	//dz_binary_tree_print_node(node);
	if (node->right) {
		dz_binary_tree_inorder(node->right);
	}
}

UINT dz_binary_tree_height(PDZ_BINARY_NODE root)
{
	UINT lheight = 0;
	UINT rheight = 0;

	if (root == NULL) {
		return 0;
	}

	//lheight = dz_binary_tree_height(root->left) + 1;
	//rheight = dz_binary_tree_height(root->right) + 1;
	lheight = dz_binary_tree_height(root->left);
	rheight = dz_binary_tree_height(root->right);

	if (lheight > rheight) {
		return lheight+1;
	} else {
		return rheight+1;
	}

}
RVOID dz_binary_tree_preorder_iterative(PDZ_BINARY_NODE root)
{
	PDZ_BINARY_NODE	node = root;

	PUSH(root);
	//LOGTGT("Pushing Right Data = %ld, local_stack_idx = %d\n", node->data, local_stack_idx);
	while(node) {

		POP(node);
		if (node) {
			//LOGTGT("Popping Data = %ld local_stack_idx = %d\n", node->data, local_stack_idx);
			LOGTGT("PreOrder Iterative Data = %ld\n", node->data);
		}
		if (node->right) {
			PUSH(node->right);
			//LOGTGT("Pushing Right Data = %ld local_stack_idx = %d\n", node->right->data, local_stack_idx);
		}
		if (node->left) {
			PUSH(node->left);
			//LOGTGT("Pushing Left Data = %ld local_stack_idx = %d\n", node->right->data, local_stack_idx);
		}
		IF_STACK_EMPTY_() break;

	}

}

RVOID dz_binary_tree_level_order(PDZ_BINARY_NODE root)
{
	PDZ_BINARY_NODE	node = root;

	while(node)  {
		LOGTGT("Data = %ld\n", node->data);
		if (node->left) {
			QUEUE(node->left);
			//LOGTGT("ENQUEUE LEFT Front = %u, Rear = %u\n", local_queue_front, local_queue_rear);
		}
		if (node->right) {
			QUEUE(node->right);
			//LOGTGT("ENQUEUE RIGHT Front = %u, Rear = %u\n", local_queue_front, local_queue_rear);
		}
		DEQUEUE(node);
		//LOGTGT("DEQUEUE Front = %u, Rear = %u\n", local_queue_front, local_queue_rear);
	}
	//LOGTGT("QUEUE Front = %u, Rear = %u\n", local_queue_front, local_queue_rear);
	//SL10;
}

//Checks if two trees are mirror image to each other
//Three Conditions are required to check this
BOOL dz_binary_tree_check_mirror(PDZ_BINARY_NODE root, PDZ_BINARY_NODE mirrorroot)
{

	//Condition 1
	if (root == NULL && mirrorroot == NULL) {
		return true;
	}

	//Condition 2
	if ((root == NULL && mirrorroot != NULL) || (root != NULL && mirrorroot == NULL)) {
		return false;
	}

	LOGTGT("Checking root->data = %ld, mirror->data = %ld\n", root->data, mirrorroot->data);

	//Condition 3
	if (root->data == mirrorroot->data) {

		//Condition 3-1: This is very important. Here left subtree of root is matched with right subtree of mirrored one
		//and right subtree is matched with left subtree
		if((dz_binary_tree_check_mirror(root->left, mirrorroot->right) &&  
			dz_binary_tree_check_mirror(root->right, mirrorroot->left)) == true) {
				return true;
			} else {
				return false;
			}
	} else {

		return false;

	}

	RETURNT;
	
}

//We have to traverse the original root in preorder because 
//it will start with root node first
//Finally the mirror image root will be returned
PDZ_BINARY_NODE dz_binary_tree_mirror(PDZ_BINARY_NODE root)
{
	PDZ_BINARY_NODE mirrorroot = NULL;
	
	if (root) {
		mirrorroot = dz_binary_tree_alloc_node();
		MEMSET(mirrorroot);
		mirrorroot->data = root->data;
		mirrorroot->sequence = root->sequence;
		LOGTGT("Mirror Created %ld\n", mirrorroot->data);

	//Root left will be created in mirror's right
		mirrorroot->right = dz_binary_tree_mirror(root->left);
		if (mirrorroot->right) {
		
			mirrorroot->right->parent = mirrorroot;
			mirrorroot->right->parent_data = mirrorroot->data;
			mirrorroot->right_data = mirrorroot->right->data;
			LOGTGT("Mirror root->left->data = %ld, mirror->right->data = %ld\n", root->left->data, mirrorroot->right->data);
		}

		mirrorroot->left = dz_binary_tree_mirror(root->right);
		if (mirrorroot->left) {
			mirrorroot->left->parent = mirrorroot;
			mirrorroot->left->parent_data = mirrorroot->data;
			mirrorroot->left_data = mirrorroot->left->data;
			LOGTGT("Mirror root->right->data = %ld, mirror->left->data = %ld\n", root->right->data, mirrorroot->left->data);
		}
	}

	return mirrorroot;

}

//We have to traverse the original root in preorder because 
//it will start with root node first
//Finally the cloned tree will be returned
//We are doing this without Recursion i.e. throgh Iterative Traversal
//Also using the QUEUE and iterating it level by level
PDZ_BINARY_NODE dz_binary_tree_clone_iterative(PDZ_BINARY_NODE root)
{
	PDZ_BINARY_NODE clone_root = NULL;


	RESET_QUEUE();

	while(root) {

		clone_root = dz_binary_tree_insert(clone_root, root->data);

		if (root->left) {
			QUEUE(root->left);
		}
		if (root->right) {
			QUEUE(root->right);
		}

		DEQUEUE(root);
	}

	return clone_root;
}

VOID dz_binary_tree_deep_copy_cycle(PDZ_TREE_NODE root, PDZ_TREE_NODE clone)
{
	int i = 0;

	if (root->cycle) {
		clone->cycle = (PDZ_TREE_NODE)GET_MAP(root->cycle->data);
	}

	for (i = 0; i < DZ_TREE_NODE_CHILDREN; i++) {
		if (root->childrens[i]) {
			dz_binary_tree_deep_copy_cycle(root->childrens[i], clone->childrens[i]);
		}
	}


}
PDZ_TREE_NODE dz_binary_tree_deep_copy_core(PDZ_TREE_NODE root)
{
	PDZ_TREE_NODE clone_root = NULL;
	int i = 0;

	if (root) {
		clone_root = dz_tree_alloc_node();
		clone_root->data = root->data;


		SET_MAP(clone_root->data, root);

		for (i = 0; i < DZ_TREE_NODE_CHILDREN; i++) {
			if (root->childrens[i]) {
				clone_root->childrens[i] = dz_binary_tree_deep_copy_core(root->childrens[i]);
			}
		}
		
	}

	return clone_root;
}

PDZ_TREE_NODE dz_binary_tree_deep_copy(PDZ_TREE_NODE root)
{
	PDZ_TREE_NODE pnode = root;
	PDZ_TREE_NODE clone;
	if (pnode == NULL) {
		pnode = dz_tree_alloc_node();
		root = pnode;
	}


	root->data  = 101;
	root->cycle = NULL;

	root->childrens[0] = dz_tree_alloc_node();
	root->childrens[0]->data = 101000;

	root->childrens[1] = dz_tree_alloc_node();
	root->childrens[1]->data = 101100;
	root->childrens[1]->childrens[0] = NULL;
	root->childrens[1]->childrens[1] = NULL;
	root->childrens[1]->childrens[2] = NULL;

	root->childrens[2] = dz_tree_alloc_node();
	root->childrens[2]->data = 101200;
	root->childrens[2]->childrens[0] = NULL;
	root->childrens[2]->childrens[1] = NULL;
	root->childrens[2]->childrens[2] = NULL;


	pnode = root->childrens[0];
	pnode->childrens[0] = dz_tree_alloc_node();
	pnode->childrens[0]->data = 201000;
	pnode->childrens[0]->childrens[0] = NULL;
	pnode->childrens[0]->childrens[1] = NULL;
	pnode->childrens[0]->childrens[2] = NULL;



	pnode->childrens[1] = dz_tree_alloc_node();
	pnode->childrens[1]->data = 201100;
	pnode->childrens[1]->childrens[0] = NULL;
	pnode->childrens[1]->childrens[1] = NULL;
	pnode->childrens[1]->childrens[2] = NULL;


	pnode->childrens[2] = dz_tree_alloc_node();
	pnode->childrens[2]->data = 201200;
	pnode->childrens[2]->childrens[0] = NULL;
	pnode->childrens[2]->childrens[1] = NULL;
	pnode->childrens[2]->childrens[2] = NULL;

	//Set the cycle point to first child and vice-versa
	pnode = root->childrens[0];
	pnode->cycle = root;
	root->cycle  = pnode;

	LOGTGT("pnode->cycle->data = %d\n", pnode->cycle->data);
	LOGTGT("root->cycle->data = %d\n", root->cycle->data);

	LOGTGT("Calling copy_core\n");
	clone = dz_binary_tree_deep_copy_core(root);
	dz_binary_tree_print_map();
	LOGTGT("Calling copy_cycle\n");
	dz_binary_tree_deep_copy_cycle(root, clone);
	LOGTGT("clone = %p\n", clone);
	LOGTGT("clone->cycle = %p\n", clone->cycle);
	LOGTGT("clone->cycle->data = %d\n", clone->cycle->data);
	pnode = clone->childrens[0];
	LOGTGT("clone = %p\n", pnode);
	LOGTGT("clone->cycle = %p\n", pnode->cycle);
	LOGTGT("clone->cycle->data = %d\n", pnode->cycle->data);
	return clone;
}

//We have to traverse the original root in preorder because 
//it will start with root node first
//Finally the cloned tree will be returned
//We are doing this using Recursion
PDZ_BINARY_NODE dz_binary_tree_clone(PDZ_BINARY_NODE root)
{
	PDZ_BINARY_NODE clone_root = NULL;
		if (root != NULL) {
			clone_root = dz_binary_tree_alloc_node();
			MEMSET(clone_root);
			clone_root->data = root->data;
			clone_root->sequence = root->sequence;
			clone_root->parent_data = root->parent_data;
			clone_root->left_data = root->left_data;
			clone_root->right_data = root->right_data;
			clone_root->left = clone_root->right = NULL;
			LOGTGT("Created %ld\n", clone_root->data);
	
		//dz_binary_tree_print_node(root);

		clone_root->left = dz_binary_tree_clone(root->left);
		if (clone_root->left) {
			clone_root->left_data = clone_root->left->data;
			clone_root->left->parent = clone_root;
		}

		clone_root->right = dz_binary_tree_clone(root->right);
		if (clone_root->right) {
			clone_root->right_data = clone_root->right->data;
			clone_root->right->parent = clone_root;
		}
	}

	return clone_root;
}


//Three conditions are required to find the inorder predecessor
//Condition - 1 : if node has left subtree then the immediate left node is its predecessor
//Condition - 2 : if node has no left node then
//					start traverse from root
//					and notedown the shifting from current to right node till the desired node
//					The last shifting from current to right node is the predecessor node
//
//Condition - 3 : If node is the first and min value node in the tree then if there
//					are no right turns in the entire search path of that node, then
//					no predecessor found.
PDZ_BINARY_NODE dz_binary_tree_find_inorder_predecessor(PDZ_BINARY_NODE root, INT data)
{

	PDZ_BINARY_NODE	pnode = root;
	PDZ_BINARY_NODE	last_right_turn = NULL;

	while(pnode) {

		if (data < pnode->data) {
			pnode = pnode->left;
		} else if (data == pnode->data) {
				if (pnode->left) { //Condition -1
					pnode = pnode->left;
					while(pnode->right != NULL) {
						pnode = pnode->right;
					}

					LOGTGT("Predecessor Node of %d is %ld\n", data, pnode->data);
					break;
				} else { //Condition -2
					if (!last_right_turn) { //Condition -3
						LOGTGT("NO Parent Predecessor Node of %d Found\n", data);

					} else {
						LOGTGT("Last Right Predecessor Node of %d is %ld\n", data, last_right_turn->data);
					}
		
					break;

				}
		} else {
			last_right_turn = pnode;
			pnode = pnode->right;
		}
	}


	return pnode;

}

//Three conditions are required to find the inorder successor
//Condition - 1 : if node has right subtree then the lowest value node is its successor
//Condition - 2 : if node has no right node then
//					start traverse from root
//					and notedown the shifting from current to left node till the desired node
//					The last shifting from current to left node is the successor node
//Condition - 3 : If node is the last and max value node in the tree then if there
//					are no left turns in the entire search path of that node, then
//					no successor found.
PDZ_BINARY_NODE dz_binary_tree_find_inorder_successor(PDZ_BINARY_NODE root, INT data)
{

	PDZ_BINARY_NODE	pnode = root;
	PDZ_BINARY_NODE	last_left_turn = NULL;

	while(pnode) {

		if (data < pnode->data) {
			last_left_turn = pnode;
			pnode = pnode->left;
		} else if (data == pnode->data) {
				//Now we need to check condition 1
				//The left most node will be the inorder successor
				if (pnode->right) { //Condition -1
					pnode = pnode->right;
					while(pnode->left != NULL) {
						pnode = pnode->left;
					}

					LOGTGT("Successor Node of %d is %ld\n", data, pnode->data);
					break;
				} else { //Condition -2
					if (!last_left_turn) { //Condition -3
						LOGTGT("NO Left Successor Node of %d Found\n", data);

					} else {
						LOGTGT("Last Left Successor Node of %d is %ld\n", data, last_left_turn->data);
					}
		
					break;

				}
		} else {
			pnode = pnode->right;
		}
	}


	return pnode;
}

//Normal insertion in binary tree based on binary search insert
PDZ_BINARY_NODE dz_binary_tree_insert(PDZ_BINARY_NODE root, INT data)
{
	if (root == NULL) {
		root = dz_binary_tree_alloc_node();
		MEMSET(root);
		root->data = data;
		root->sequence = sequence++;
		return root;
	}

	if (data < root->data) {
		root->left = dz_binary_tree_insert(root->left, data);
		root->left_data = root->left->data;
		root->left->parent = root;
		root->left->parent_data = root->data;
	} else {
		root->right = dz_binary_tree_insert(root->right, data);
		root->right_data = root->right->data;
		root->right->parent = root;
		root->right->parent_data = root->data;
	}

	return root; //Always Top of the tree root
}

BOOL dz_process_lab_engine_binary_tree(LBA offset, UINT iosize)
{
	UINT		height = 0;

	CREATE_QUEUE(local_queue, local_queue_size);
	CREATE_STACK(local_stack, local_stack_size);

	BINARY_TREE_INSERT(tree_root, 24);
	BINARY_TREE_INSERT(tree_root, 20);
	BINARY_TREE_INSERT(tree_root, 40);
	BINARY_TREE_INSERT(tree_root, 10);
	BINARY_TREE_INSERT(tree_root, 22);
	BINARY_TREE_INSERT(tree_root, 30);
	BINARY_TREE_INSERT(tree_root, 50);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   Inorder Order\n");
	LOGTGT("###################################################################################\n");
	dz_binary_tree_inorder(tree_root);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   LeveL Order\n");
	LOGTGT("###################################################################################\n");
	dz_binary_tree_level_order(tree_root);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   Cloning\n");
	LOGTGT("###################################################################################\n");
	clone_root = dz_binary_tree_clone(tree_root);
	dz_binary_tree_inorder(clone_root);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   LeveL Order Cloning\n");
	LOGTGT("###################################################################################\n");
	RESET_QUEUE();
	dz_binary_tree_level_order(clone_root);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   Mirroring\n");
	LOGTGT("###################################################################################\n");
	mirror_root = dz_binary_tree_mirror(tree_root);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   LeveL Order Mirror\n");
	LOGTGT("###################################################################################\n");
	RESET_QUEUE();
	dz_binary_tree_level_order(mirror_root);
	dz_binary_tree_inorder(mirror_root);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   Check Mirroring\n");
	LOGTGT("###################################################################################\n");
	if (dz_binary_tree_check_mirror(tree_root, mirror_root)) {
		LOGTGT("Both the Trees are mirror images of each other\n");
	}
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   Inorder Successor \n");
	LOGTGT("###################################################################################\n");
	dz_binary_tree_find_inorder_successor(tree_root, 20);
	dz_binary_tree_find_inorder_successor(tree_root, 22);
	dz_binary_tree_find_inorder_successor(tree_root, 30);
	dz_binary_tree_find_inorder_successor(tree_root, 50);
	dz_binary_tree_find_inorder_successor(tree_root, 10);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   Inorder Predecessor \n");
	LOGTGT("###################################################################################\n");
	dz_binary_tree_find_inorder_predecessor(tree_root, 20);
	dz_binary_tree_find_inorder_predecessor(tree_root, 22);
	dz_binary_tree_find_inorder_predecessor(tree_root, 30);
	dz_binary_tree_find_inorder_predecessor(tree_root, 50);
	dz_binary_tree_find_inorder_predecessor(tree_root, 10);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   Cloning Iteratively\n");
	LOGTGT("###################################################################################\n");
	clone_root = dz_binary_tree_clone_iterative(tree_root);
	dz_binary_tree_inorder(clone_root);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   LeveL Order Cloning Iteratively \n");
	LOGTGT("###################################################################################\n");
	RESET_QUEUE();
	dz_binary_tree_level_order(clone_root);

	LOGTGT("###################################################################################\n");
	LOGTGT("                                   PreOrder Iteratively \n");
	LOGTGT("###################################################################################\n");
	dz_binary_tree_preorder(tree_root);
	dz_binary_tree_preorder_iterative(tree_root);
	//RESET_QUEUE();
	//dz_binary_tree_level_order(tree_root);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   Tree Height\n");
	LOGTGT("###################################################################################\n");
	height = dz_binary_tree_height(tree_root);
	LOGTGT("Height of the tree is %u\n", height);
	LOGTGT("###################################################################################\n");
	LOGTGT("                                   DeepCopy With Cycle/Random Pointer              \n");
	LOGTGT("###################################################################################\n");
	CREATE_MAP();	
	PRINT_MAP();
	deep_clone_root = dz_binary_tree_deep_copy(deep_tree_root);
	
	RETURNT;
}

EXPORT_SYMBOL(dz_process_lab_engine_binary_tree);

/*
 * 1. Binary Tree Insertion
 * 2. Binary Tree Clone (Generate two trees)
 * 3. Binary Tree Mirror (Generate two tree, Original and original's mirror)
 * 4. Binary Tree Clone Mirror (Generate two tree, Original and clone of original's mirror)
 * 2. Binary Tree Clone with rand pointer (Generate two trees)
 * 5. Binary Tree Find Inorder Successor
 * 5. Binary Tree Find Inorder Predecessor
 
 */
