/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

//static PDZ_BPLUS_TREE_NODE 	tree_root 	= NULL;
PPAGE			BPTREE_ROOT = NULL;

RVOID dz_bplus_tree_print_node_lite(PDZ_BPLUS_TREE_NODE node)
{
	if (!node) {
		LOGTGT("Node Print: BPlus Tree Node is NULL\n");
		return;
	}
	LOGTGT("[ NODE      = %p]\n", node);
	LOGTGT("[ NODE->key = %llu, NODE->Self = %hu ]\n", node->key, node->self);
}

RVOID dz_bplus_tree_print_node(PDZ_BPLUS_TREE_NODE node)
{
	BOOL flag = TRUE;
	if (!node) {
		LOGTGT("Node Print: BPlus Tree Node is NULL\n");
		return;
	}


	if (flag == TRUE) {
		LOGTGT("[---------BPLUS TREE NODE----------]\n");
		LOGTGT("[                                  ]\n");
		LOGTGT("[ NODE           		= %p]\n", node);
		LOGTGT("[ NODE->key      		= %llu]\n", node->key);
		LOGTGT("[ NODE->self      		= %hu]\n", node->self);
		LOGTGT("[ NODE->parent     		= %u]\n", node->parent);
		if (node->left == BPNODE_NULL) {
			LOGTGT("[ NODE->left     		= NULL]\n");
		} else {
			LOGTGT("[ NODE->left     		= %hi]   [%p]\n", node->left,  (node + node->left));
		}

		if (node->right == BPNODE_NULL) {
			LOGTGT("[ NODE->right     		= NULL]\n");
		} else {
			LOGTGT("[ NODE->right     		= %hi ] [%p]\n", node->right, (node + node->right));
		}

		IF_BPNODE_TYPE(node->type, BPNODE_TYPE_INTERNAL_ONLY) {
			LOGTGT("[ NODE->type = BPNODE_TYPE_INTERNAL_ONLY	= %x]\n", (node->type));
		}
		IF_BPNODE_TYPE(node->type, BPNODE_TYPE_INTERNAL_FIRST) {
			LOGTGT("[ NODE->type = BPNODE_TYPE_INTERNAL_FIRST	= %x]\n", (node->type));
		}
		IF_BPNODE_TYPE(node->type, BPNODE_TYPE_INTERNAL_LAST) {
			LOGTGT("[ NODE->type = BPNODE_TYPE_INTERNAL_LAST	= %x]\n", (node->type));
		}
		IF_BPNODE_TYPE(node->type, BPNODE_TYPE_LEAF_ONLY) {
			LOGTGT("[ NODE->type = BPNODE_TYPE_LEAF_ONLY	= %x]\n", (node->type));
		}
	}
}

PDZ_BPLUS_TREE_LEAF_NODE dz_bplus_tree_alloc_leaf_node(VOID)
{
    PDZ_BPLUS_TREE_LEAF_NODE node = DZ_BPLUS_TREE_ALLOC_LEAF_NODE();
	if (!node) {
		LOGTGTE("%s","Error, Unable to allocate memory for interval tree node\n");
		RETURNN;
	}
	MEMSET(node);
	return node;
}

RVOID dz_bplus_tree_print_page(PPAGE root)
{
	int i = 0;
	PVOID   					pagebuf= NULL;
	PDZ_BPLUS_TREE_NODE 		pnode;
	PDZ_BPLUS_METADATA_NODE		head_md			= NULL;			
	pagebuf = kmap(root);
	head_md = (PDZ_BPLUS_METADATA_NODE)(pagebuf + BPLUS_TREE_PAGE_SIZE - DZ_BPLUS_NODE_SIZE);

	//for (i = 0; i < DZ_BPLUS_TREE_ORDER; i++) {
	for (i = 0; i < head_md->alloc_cnt; i++) {

		//LOGTGT("Printing Node at index %d\n", i);
		pnode = &(((PDZ_BPLUS_TREE_NODE)pagebuf)[i]);
		if (false) {
			dz_bplus_tree_print_node_lite(pnode);
		} else {
			dz_bplus_tree_print_node(pnode);
		}
	}
	LOGTGT("[ METADATA->level       = %u\n",   head_md->level);
	LOGTGT("[ METADATA->alloc_cnt   = %u\n",   head_md->alloc_cnt);
	LOGTGT("[ METADATA->minkey      = %llu\n", head_md->minkey);
	LOGTGT("[ METADATA->maxkey      = %llu\n", head_md->minkey);
	LOGTGT("[ METADATA->curr_page   = %p\n", head_md->curr_page);
	LOGTGT("[ METADATA->parent_page = %p\n", head_md->parent_page);
	LOGTGT("[ METADATA->next_page   = %p\n", head_md->next_page);
	LOGTGT("[ METADATA->prev_page   = %p\n", head_md->prev_page);
	kunmap(root);
}

BOOL dz_process_lab_engine_bplus_tree(LBA offset, UINT iosize)
{
	//INT ret = 0;
	PDZ_BPLUS_TREE_LEAF_NODE leaf = NULL;
	//KEY key = 0;
	LOGTGT("DZ_BPLUS_NODE_SIZE     	= %lu\n", DZ_BPLUS_NODE_SIZE);
	LOGTGT("DZ_BPLUS_TREE_ORDER    	= %lu\n", (ULONG)DZ_BPLUS_TREE_ORDER);
	LOGTGT("DZ_BPLUS_TREE_MAX_KEYS 	= %lu\n", (ULONG)DZ_BPLUS_TREE_MAX_KEYS);
	LOGTGT("sizeof(BPNODE)         	= %lu\n", sizeof(BPNODE));
	LOGTGT("sizeof(BPMTDT)         	= %lu\n", sizeof(BPMTDT));
	LOGTGT("sizeof(SPINLOCK)        = %lu\n", sizeof(SPINLOCK));
	//KEY parent_key = 0;

	// Last two arguments are key and data
	//INSERT_KEY(leaf, BPTREE_ROOT, 101, 10101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 95, 95101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 98, 98101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 125, 125101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 119, 119101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 55, 55101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 157, 157101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 105, 105101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 140, 140101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 235, 235101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 120, 120101);
	//
	//INSERT_KEY(leaf, BPTREE_ROOT, 101, 10101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 55, 20101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 150, 30101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 40, 40101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 90, 50101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 120, 60101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 180, 70101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 30, 80101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 130, 90101);
	//
	
	INSERT_KEY(leaf, BPTREE_ROOT, 16, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 8, 90101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 24, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 4, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 12, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 20, 90101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 28, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 2, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 6, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 10, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 14, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 18, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 22, 90101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 26, 90101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 30, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 1, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 3, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 5, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 7, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 9, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 11, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 13, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 15, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 17, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 19, 90101);
	INSERT_KEY(leaf, BPTREE_ROOT, 21, 90101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 23, 90101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 25, 90101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 27, 90101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 29, 90101);
	//INSERT_KEY(leaf, BPTREE_ROOT, 31, 90101);


	LOGTGT("Printing Page in Sequential Order\n");
	dz_bplus_tree_print_page(BPTREE_ROOT);
	INSERT_KEY(leaf, BPTREE_ROOT, 23, 90101);
	//LOGTGT("Printing Page in BST In-Order Traversal\n");
	//dz_bplus_tree_traverse_page(BPTREE_ROOT, ENUM_TRAVERSAL_TYPE_INORDER);
	//ret = dz_bplus_tree_get_num_keys(BPTREE_ROOT);
	//LOGTGT("NUM KEYS               	= %d\n", ret);
	//key = dz_bplus_tree_get_mid_key(BPTREE_ROOT);
	//LOGTGT("MID KEY  				= %llu\n", key);

	RETURNT;
}

EXPORT_SYMBOL(dz_process_lab_engine_bplus_tree);
