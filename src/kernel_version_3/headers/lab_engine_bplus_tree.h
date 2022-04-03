/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _LAB_ENGINE_BPLUS_TREE_H_
#define _LAB_ENGINE_BPLUS_TREE_H_

//This is a node where the core data will be stored, hence called leaf nodes
typedef struct dz_bplus_tree_leaf_node_s {
	KEY	key;
	union {
		LONG data;
		CHAR data_str[8];
		VOID *data_record;
	};
	struct dz_bplus_tree_leaf_node_s *next;
	struct dz_bplus_tree_leaf_node_s *prev;
	struct dz_bplus_tree_leaf_node_s *parent;
} __attribute__((__packed__)) DZ_BPLUS_LEAF_NODE, *PDZ_BPLUS_LEAF_NODE, DZ_BPLUS_NODE_LEAF, *PDZ_BPLUS_NODE_LEAF;
typedef struct dz_bplus_tree_leaf_node_s DZ_BPLUS_TREE_LEAF_NODE, *PDZ_BPLUS_TREE_LEAF_NODE;
typedef struct dz_bplus_tree_leaf_node_s BPLEAF, *PBPLEAF;


//These are intermediate or internal nodes or rather non-leaf nodes.These nodes
//will not contain any data. It will point to a leaf node or internal node
//the size of this structure must always be 32 bytes
typedef struct dz_bplus_tree_node_s {
	KEY		key;						/* 8 bytes */
	USHORT	self;						/* Denotes indexes instead of pointer, because this is a page */		
	USHORT	parent;						/* 8  + 4 = 12 bytes */
	SHORT	left; 						/* 12 + 2 = 14 bytes */
	SHORT	right;						/* 14 + 2 = 16 bytes */
	UCHAR 	type;						/* 16 + 1 = 17 bytes */
#define BPNODE_TYPE_INTERNAL_ONLY		0x1
#define BPNODE_TYPE_INTERNAL_FIRST		0x2
#define BPNODE_TYPE_INTERNAL_LAST		0x4
#define BPNODE_TYPE_LEAF_ONLY			0x8
#define SET_BPNODE_TYPE(type, flag)			(type) |= (flag)
#define RESET_BPNODE_TYPE(type, flag)		(type) &= ~(flag)
#define IF_BPNODE_TYPE(type, flag)			if ((type) & (flag) ) 

	union {
		PBPLEAF	data; //It will get set when type is BPNODE_TYPE_LEAF_ONLY
		PPAGE 	down; //For everything else
	};
	UCHAR	unused1;
	USHORT	unused2;
	INT		unused3;
} __attribute__((__packed__)) DZ_BPLUS_NODE, *PDZ_BPLUS_NODE, DZ_BPLUS_TREE_NODE, *PDZ_BPLUS_TREE_NODE;
typedef struct dz_bplus_tree_node_s BPNODE, *PBPNODE;

//This structure will keep information about how many internal nodes are stored
//in a single page. How much space is free on this page etc.
//One Page can accomodate 126 internal nodes of size 32 bytes
//This structure will get stored in last 64 bytes of the Page
//e.g.
/*
	   PAGE  (Total size 4K or 4096 bytes)
	___________
   |___________| <= For internal nodes of size 32 bytes
   |___________| <= For internal nodes of size 32 bytes
   .           .
   .           .
   |___________|
   |___________| <= For metadata structure of size 64 bytes
*/
typedef struct dz_bplus_tree_node_metadata_s {
	UINT		level;				//On what level this page belongs to in the complete tree
	UINT 		alloc_cnt;			//How much internal nodes have been allocated
	PPAGE		curr_page;			//Address of current page on which this metadata belongs to
	PPAGE 		next_page;
	PPAGE 		prev_page;
	PPAGE 		parent_page;		//Address of Parent Page. Used for splitting
	SPINLOCK	page_lock;		//Lock used while inserting/deleting and splitting this page
								//TODO:We also have to think for Reader/Write lock

	KEY			minkey;				//Starting Key
	KEY			maxkey;				//Max Key
	INT			unused1;
	
} __attribute__((__packed__)) DZ_BPLUS_NODE_METADATA, *PDZ_BPLUS_NODE_METADATA, DZ_BPLUS_METADATA_NODE, *PDZ_BPLUS_METADATA_NODE;
typedef struct dz_bplus_tree_node_metadata_s BPMTDT, *PBPMTDT;

typedef struct dz_bplus_split_node_s {
	PPAGE	left_page;
	PPAGE	right_page;
	PBPNODE	split_node;

} __attribute__((__packed__)) DZ_BLUS_SPLIT_NODE, *PDZ_BPLUS_SPLIT_NODE;
typedef struct dz_bplus_split_node_s BPSPLIT, *PBPSPLIT;

BOOL dz_process_lab_engine_bplus_tree(LBA offset, UINT iosize);
#define DZ_BPLUSTREE_ALLOC_NODE()		DZ_ALLOC_NODE(sizeof(DZ_BPLUS_NODE))
#define DZ_BPLUS_TREE_ALLOC_NODE()		DZ_ALLOC_NODE(sizeof(DZ_BPLUS_NODE))
#define DZ_BPLUS_TREE_ALLOC_LEAF_NODE()	DZ_ALLOC_NODE(sizeof(DZ_BPLUS_TREE_LEAF_NODE))
#define DZ_BPLUS_ALLOC_NODE()			DZ_ALLOC_NODE(sizeof(DZ_BPLUS_NODE))

#define DZ_BPLUS_NODE_SIZE				(sizeof(BPNODE))	//32bytes.
//#define DZ_BPLUS_TREE_ORDER				((PAGE_SIZE - sizeof(BPMTDT)) / DZ_BPLUS_NODE_SIZE)
#define DZ_BPLUS_TREE_ORDER				22
#define DZ_BPLUS_TREE_MAX_KEYS			(DZ_BPLUS_TREE_ORDER - 1)

#define BPLUS_NODE_FULL					2
#define BPLUS_NODE_SPLIT				3
#define BPLUS_NODE_PAGE_FULL			2
#define	BPLUS_NODE_NULL					-1
#define	BPNODE_NULL						BPLUS_NODE_NULL
#define	BPLUS_NODE_NOT_FOUND			-2

//Key Related Error Codes
#define	BPLUS_NODE_KEY_FOUND			0
#define	BPLUS_NODE_KEY_NOT_FOUND		-2
#define	BPLUS_NODE_KEY_IS_LESSER		-3
#define	BPLUS_NODE_KEY_IS_GREATER		-4

#define BPLUS_TREE_PAGE_SIZE			PAGE_SIZE

typedef enum traversal_type_s {
	ENUM_TRAVERSAL_TYPE_INORDER = 1,
	ENUM_TRAVERSAL_TYPE_PREORDER,
	ENUM_TRAVERSAL_TYPE_POSTORDER,
	ENUM_TRAVERSAL_TYPE_LEVELORDER
} TRAVERSAL_TYPE;

//Copy small chunk from buffer to the starting of page
#define COPY_BUFFER_CHUNK_TO_PAGE(page, buffer, chunksz)	\
{															\
	PVOID   	dpagebuf= NULL;								\
	PVOID   	spagebuf= (buffer);							\
	dpagebuf = kmap((page));								\
	PMEMCPY(dpagebuf, spagebuf, chunksz);					\
	kunmap((page));											\
}

//Copy small chunk from starting of page to a buffer
#define COPY_PAGE_CHUNK_TO_BUFFER(buffer, page, chunksz)	\
{															\
	PVOID   	spagebuf= NULL;								\
	spagebuf = kmap((page));								\
	PMEMCPY((buffer), spagebuf, sz);						\
	kunmap((page));											\
}

#define INSERT_KEY(arg_leaf, arg_tree_root, arg_key, arg_data)		\
	{																\
		arg_leaf = dz_bplus_tree_alloc_leaf_node();					\
		arg_leaf->key = (arg_key);									\
		arg_leaf->data = (arg_data);								\
		arg_tree_root = dz_bplus_tree_insert(arg_tree_root, arg_leaf);				\
	}


#define PUSH_STACK(arg_stack, arg_stack_idx, arg_element)		\
	arg_stack[ATOMIC_READ(arg_stack_idx)] = arg_element;		\
	ATOMIC_INC(arg_stack_idx);									\

#define POP_STACK(arg_stack, arg_stack_idx, arg_element)		\
	arg_element = arg_stack[ATOMIC_READ(arg_stack_idx) - 1 ];	\
	arg_stack[ATOMIC_READ(arg_stack_idx) - 1] = 0;				\
	ATOMIC_DEC(arg_stack_idx);									

#define IF_EMPTY_STACK(arg_stack_idx) if (ATOMIC_READ(arg_stack_idx) == 0)
#define IF_STACK_EMPTY(arg_stack_idx) if (ATOMIC_READ(arg_stack_idx) == 0)

#define IF_NOT_EMPTY_STACK(arg_stack_idx) if (!(ATOMIC_READ(arg_stack_idx) == 0))
#define IF_NOT_STACK_EMPTY(arg_stack_idx) if (!(ATOMIC_READ(arg_stack_idx) == 0))


//Delete Functions
PPAGE dz_bplus_tree_delete(PPAGE root, KEY key);

//Insert Functions
//PBPNODE dz_bplus_tree_split_node(PPAGE root_page);
//PBPSPLIT dz_bplus_tree_split_node(PPAGE root_page, PPAGE left_page, PPAGE right_page);
INT dz_bplus_tree_split_node(PPAGE root_page, PBPSPLIT split_node);
INT dz_bplus_tree_insert_key_data(PPAGE root_page, KEY key, PDZ_BPLUS_LEAF_NODE data );
PVOID	dz_bplus_tree_insert_data(PPAGE root_page, PDZ_BPLUS_LEAF_NODE data );
PPAGE dz_bplus_tree_insert(PPAGE root_page, PDZ_BPLUS_LEAF_NODE data);

//Traversal Functions
RVOID dz_bplus_tree_traverse_page_postorder(PVOID root_pagebuf, PDZ_BPLUS_NODE root);
RVOID dz_bplus_tree_traverse_page_preorder(PVOID root_pagebuf, PDZ_BPLUS_NODE root);
RVOID dz_bplus_tree_traverse_page_inorder(PVOID root_pagebuf, PDZ_BPLUS_NODE root);
RVOID dz_bplus_tree_traverse_page(PPAGE root, INT type);

//Find / Search / Get functions
PBPNODE dz_bplus_tree_find_split_node(PPAGE root_page);
INT dz_bplus_tree_find_key(PPAGE root_page, KEY key);
INT dz_bplus_tree_search_key(PPAGE root_page, KEY key);
PPAGE dz_bplus_tree_find_down_page(PPAGE root_page, KEY key);
PPAGE dz_bplus_tree_find_page_at_data_level(PPAGE root_page, KEY key);

//Get / Set Functions
VOID dz_bplus_tree_reset_page(PPAGE root_page) ;
KEY dz_bplus_tree_get_min_key(PPAGE root_page);
KEY dz_bplus_tree_get_max_key(PPAGE root_page); 
KEY dz_bplus_tree_get_mid_key(PPAGE root_page); 
UINT dz_bplus_tree_get_num_keys(PPAGE root_page);
PPAGE dz_bplus_tree_get_next_page(PPAGE root_page);
PPAGE dz_bplus_tree_get_prev_page(PPAGE root_page);
PPAGE dz_bplus_tree_get_parent_page(PPAGE root_page);
VOID dz_bplus_tree_set_next_page(PPAGE root_page, PPAGE next_page);
VOID dz_bplus_tree_set_prev_page(PPAGE root_page, PPAGE prev_page);
VOID dz_bplus_tree_set_parent_page(PPAGE root_page, PPAGE parent_page);
INT dz_bplus_tree_get_parent_key(PPAGE root_page, KEY key, KEY *pkey);
UINT dz_bplus_tree_get_page_level(PPAGE root_page); 
VOID dz_bplus_tree_set_page_level(PPAGE root_page, UINT level) ;

//Print Functions
RVOID dz_bplus_tree_print_node_lite(PDZ_BPLUS_TREE_NODE node);
RVOID dz_bplus_tree_print_node(PDZ_BPLUS_TREE_NODE node);
PDZ_BPLUS_TREE_LEAF_NODE dz_bplus_tree_alloc_leaf_node(VOID);
RVOID dz_bplus_tree_print_page(PPAGE root);

#endif //_LAB_ENGINE_BPLUS_TREE_H_
