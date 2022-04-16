/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

#define DZ_HEAP_ALLOC_NODE()	DZ_ALLOC_NODE(sizeof(DZ_HEAP_NODE))

PDZ_HEAP_NODE 	heap_root = NULL;
PDZ_HEAP_NODE 	heap_head = NULL;

RVOID dz_heap_print_node(PDZ_HEAP_NODE node)
{
	BOOL flag = TRUE;
	if (!node) {
		LOGTGT("Node Print: Heap Node is NULL\n");
		return;
	}


	if (flag == TRUE) {
		LOGTGT("[-----------HEAP     NODE----------]\n");
		LOGTGT("[                                  ]\n");
		LOGTGT("[ NODE           = %p]\n", node);
		LOGTGT("[ NODE->data     = %ld]\n", node->data);
		LOGTGT("[ NODE->left     = %p]\n", node->left);
		LOGTGT("[ NODE->right    = %p]\n", node->right);
		LOGTGT("[ NODE->parent   = %p]\n", node->parent);
		LOGTGT("[ NODE->root     = %p]\n", node->root);
		LOGTGT("[----------------------------------]\n");
	}

}

PDZ_HEAP_NODE dz_heap_alloc_node(VOID)
{
    PDZ_HEAP_NODE node = DZ_HEAP_ALLOC_NODE();
	if (!node) {
		LOGTGTE("%s","Error, Unable to allocate memory for heap node\n");
		RETURNN;
	}
	MEMSET(node);
	return node;
}

RVOID dz_heap_postorder(PDZ_HEAP_NODE node)
{

	dz_heap_print_node(node);

	if (node->left) {
		//dz_heap_postorder(node->left);
	}

	if (node->right) {
		//dz_heap_postorder(node->right);
	}
}

RVOID dz_heap_preorder(PDZ_HEAP_NODE node)
{
	dz_heap_print_node(node);

	if (node->left) {
		//dz_heap_preorder(node->left);
	}

	if (node->right) {
		//dz_heap_preorder(node->right);
	}
}

RVOID dz_heap_inorder(PDZ_HEAP_NODE node)
{
	if (node->left) {
		//dz_heap_inorder(node->left);
	}
	dz_heap_print_node(node);

	if (node->right) {
		//dz_heap_inorder(node->right);
	}
}

UINT	dz_heap_height(PDZ_HEAP_NODE root)
{
	UINT lheight = 0;
	UINT rheight = 0;

	if (root) {
		//lheight = dz_heap_height(root->left);
		//rheight = dz_heap_height(root->right);

		if (lheight > rheight) {
			return (lheight + 1);
		} else {
			return (rheight + 1);
		}
	} else {
		return 0;
	}
}

PDZ_HEAP_NODE dz_heap_max_insert(PDZ_HEAP_NODE root, INT data)
{
	if (root == NULL) {
		root = dz_heap_alloc_node();
		root->data = data;
		return root;
	}

	if (data < root->data) {
		root->left = dz_heap_max_insert(root->left, data);
	} else {
		root->right = dz_heap_max_insert(root->right, data);
		//Since it is a heap max
		//Swap with root data
		data = root->data;
		root->data = root->right->data;
		root->right->data = data;

	}

	return root; //Always Top of the tree root
}


BOOL dz_process_lab_engine_heap(LBA offset, UINT iosize)
{
	RETURNT;
}

EXPORT_SYMBOL(dz_process_lab_engine_heap);
