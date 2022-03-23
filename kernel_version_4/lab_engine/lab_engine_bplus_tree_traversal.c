/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PPAGE			BPTREE_ROOT;

RVOID dz_bplus_tree_traverse_page_postorder(PVOID root_pagebuf, PDZ_BPLUS_NODE root)
{
	PDZ_BPLUS_TREE_NODE 		pnode;

	if (root->left != BPNODE_NULL) {
		pnode = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root->left]);
		dz_bplus_tree_traverse_page_postorder(root_pagebuf, pnode);
	}

	if (root->right != BPNODE_NULL) {
		pnode = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root->right]);
		dz_bplus_tree_traverse_page_postorder(root_pagebuf, pnode);
	}

	//dz_bplus_tree_print_node(root);
	PRINT_KEYS(root->key,"PostOrder: ");
}

RVOID dz_bplus_tree_traverse_page_preorder(PVOID root_pagebuf, PDZ_BPLUS_NODE root)
{
	PDZ_BPLUS_TREE_NODE 		pnode;

	//dz_bplus_tree_print_node(root);
	//PRINT_KEYS(root->key,"PreOrder: ");
	PRINT_KEY_RAWS(root->key,"BPNode->key");

	if (root->left != BPNODE_NULL) {
		pnode = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root->left]);
		dz_bplus_tree_traverse_page_preorder(root_pagebuf, pnode);
	}

	if (root->right != BPNODE_NULL) {
		pnode = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root->right]);
		dz_bplus_tree_traverse_page_preorder(root_pagebuf, pnode);
	}
}

//Inorder traversal of binary search tree which is present in single page
//Note that here the left and right are stored as indexes (integers) and not pointers,
//because page provides contiguous memory space of 4096 bytes
//But left and right nodes are extracted by using those indexes;
RVOID dz_bplus_tree_traverse_page_inorder(PVOID root_pagebuf, PDZ_BPLUS_NODE root)
{
	PDZ_BPLUS_TREE_NODE 		pnode;

	if (root->left != BPNODE_NULL) {
		pnode = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root->left]);
		//This pnode is basically a left node connected to root
		dz_bplus_tree_traverse_page_inorder(root_pagebuf, pnode);
	}
	PRINT_KEYS(root->key,"Inorder: ");

	//dz_bplus_tree_print_node(root);

	if (root->right != BPNODE_NULL) {
		pnode = &(((PDZ_BPLUS_TREE_NODE)root_pagebuf)[root->right]);
		//This pnode is basically a right node connected to root
		dz_bplus_tree_traverse_page_inorder(root_pagebuf, pnode);
	}
}

RVOID dz_bplus_tree_traverse_page(PPAGE root, INT type)
{
	PVOID   					root_pagebuf= NULL;
	PDZ_BPLUS_TREE_NODE 		root_node;
	root_pagebuf = kmap(root);

	//Get the first node address
	root_node = (PDZ_BPLUS_TREE_NODE)root_pagebuf;

	switch(type) {
		case ENUM_TRAVERSAL_TYPE_INORDER:
			dz_bplus_tree_traverse_page_inorder(root_pagebuf, root_node);
			break;

		case ENUM_TRAVERSAL_TYPE_PREORDER:
			dz_bplus_tree_traverse_page_preorder(root_pagebuf, root_node);
			break;

		case ENUM_TRAVERSAL_TYPE_POSTORDER:
			dz_bplus_tree_traverse_page_postorder(root_pagebuf, root_node);
			break;
		case ENUM_TRAVERSAL_TYPE_LEVELORDER:
			LOGTGT("Not Yet Implemented\n");
			break;
	}

	kunmap(root);
}
