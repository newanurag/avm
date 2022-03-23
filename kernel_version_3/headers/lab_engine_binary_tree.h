/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _LAB_ENGINE_BINARY_TREE_H_
#define _LAB_ENGINE_BINARY_TREE_H_

typedef struct dz_binary_node_s {
	union {
		LONG data;
		CHAR data_str[8];
	};

	struct dz_binary_node_s *left;
	struct dz_binary_node_s *right;
#define _LAB_ENGINE_BINARY_TREE_H_DEBUG_

#ifdef _LAB_ENGINE_BINARY_TREE_H_DEBUG_
	struct dz_binary_node_s *parent;
	LONG 					parent_data;

	LONG					left_data;
	LONG					right_data;
	struct dz_binary_node_s *root;
	UINT	sequence;
#endif

}DZ_BINARY_NODE, *PDZ_BINARY_NODE;


#define DZ_TREE_NODE_CHILDREN	3

typedef struct tree_node_s {
	INT data;
	struct tree_node_s *childrens[DZ_TREE_NODE_CHILDREN];
	struct tree_node_s *cycle;

} DZ_TREE_NODE, *PDZ_TREE_NODE;

typedef struct map_s {
	ULONGLONG	key;
	union {
		PVOID		value;
		PVOID		val;
	};
}DZ_MAP, *PDZ_MAP;


//Cloning
PDZ_BINARY_NODE dz_binary_tree_clone_iterative(PDZ_BINARY_NODE root);
PDZ_BINARY_NODE dz_binary_tree_clone(PDZ_BINARY_NODE root);

//Mirroring
PDZ_BINARY_NODE dz_binary_tree_mirror(PDZ_BINARY_NODE root);
BOOL dz_binary_tree_check_mirror(PDZ_BINARY_NODE root, PDZ_BINARY_NODE mirrorroot);

//Predecessor And Successor
PDZ_BINARY_NODE dz_binary_tree_find_inorder_successor(PDZ_BINARY_NODE root, INT data);
PDZ_BINARY_NODE dz_binary_tree_find_inorder_predecessor(PDZ_BINARY_NODE root, INT data);

//Searching/Find
RVOID dz_binary_tree_print_node(PDZ_BINARY_NODE node);
PDZ_BINARY_NODE dz_binary_tree_alloc_node(VOID);
UINT	dz_binary_tree_height(PDZ_BINARY_NODE root);
PDZ_BINARY_NODE dz_binary_tree_insert(PDZ_BINARY_NODE root, INT data);
BOOL dz_process_lab_engine_binary_tree(LBA offset, UINT iosize);
PDZ_TREE_NODE dz_tree_alloc_node(VOID);

//Traversal
RVOID dz_binary_tree_level_order(PDZ_BINARY_NODE node);
RVOID dz_binary_tree_inorder(PDZ_BINARY_NODE node);
RVOID dz_binary_tree_preorder(PDZ_BINARY_NODE node);
RVOID dz_binary_tree_postorder(PDZ_BINARY_NODE node);
RVOID dz_binary_tree_preorder_iterative(PDZ_BINARY_NODE node);

//QUEUE Metadata
ULONGLONG* dz_binary_tree_create_queue(uint sz);
VOID dz_binary_tree_enqueue(PVOID element);
PVOID dz_binary_tree_dequeue(VOID);
VOID dz_binary_tree_reset_queue(VOID);

//STACK Metadata
ULONGLONG* dz_binary_tree_create_stack(uint sz);
VOID dz_binary_tree_reset_stack(VOID);
VOID dz_binary_tree_push_stack(PVOID element);
PVOID dz_binary_tree_pop_stack(VOID);

//HASHMAP Metadata
PDZ_MAP dz_binary_tree_create_map(uint sz);
VOID dz_binary_tree_reset_map(VOID);
PVOID dz_binary_tree_get_map(ULONGLONG Key);
VOID dz_binary_tree_set_map(ULONGLONG key, PVOID Value);
VOID dz_binary_tree_print_map(VOID);

#define DZ_BINARY_ALLOC_NODE()		DZ_ALLOC_NODE(sizeof(DZ_BINARY_NODE))
#define DZ_BINARY_TREE_ALLOC_NODE()		DZ_ALLOC_NODE(sizeof(DZ_BINARY_NODE))

#endif //_LAB_ENGINE_BINARY_TREE_H_
