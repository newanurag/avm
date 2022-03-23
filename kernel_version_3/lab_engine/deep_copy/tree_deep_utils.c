#include "tree_deep_copy.h"

//Root of original tree
extern PCH_TREE_NODE		deep_tree_root;

//Root of destination tree i.e. copy of original tree
extern PCH_TREE_NODE		deep_clone_root;

ATOMIC ch_tree_alloc_node_cnt;

//Map Metadata / Placeholders
PCH_MAP	ch_tree_map;
INT 	ch_tree_map_idx;
UINT 	ch_tree_map_size;

//QUEUE Metadata
ULONGLONG *ch_local_queue;
UINT ch_local_queue_front;
UINT ch_local_queue_rear;
UINT ch_local_queue_size;

//Memory allocation and initialization of node
//Also storing the allocation count in an atomic variable
PCH_TREE_NODE ch_tree_alloc_node(VOID)
{
    PCH_TREE_NODE node = kmalloc(sizeof(CH_TREE_NODE), GFP_KERNEL);
	if (!node) {
		LOG("%s","Error, Unable to allocate memory for tree node\n");
		return NULL;
	}
	memset(node, 0, sizeof(*ch_tree_map));
	ATOMIC_INC(ch_tree_alloc_node_cnt);
	return node;
}

//Memory deallocation of tree node
VOID ch_tree_delete_node(PCH_TREE_NODE node)
{
	if (!node) BUG_ON(1); //Panic if already a freed node
	kfree(node);
	node = NULL;
	ATOMIC_DEC(ch_tree_alloc_node_cnt);

	if(ATOMIC_READ(ch_tree_alloc_node_cnt) < 0) {
		LOG("WARNING:Alloc count negative\n");
	}
	
}

//Returns total no. of allocation count node by tree
INT ch_tree_get_alloc_count(VOID) {
	return ATOMIC_READ(ch_tree_alloc_node_cnt);
}

//Creating a linear map for simplification purpose. 
//Ideally a map should be created with some self balancing binary search trees
PCH_MAP ch_tree_create_map(UINT sz)
{
	PCH_MAP mp = kmalloc(sizeof(CH_MAP) * sz, GFP_KERNEL);
	if (mp) {
		LOG("MAP Created of size %u\n", sz);
	}
	ch_tree_reset_map(mp);
	return mp;
}

//Deleting a Map
VOID ch_tree_delete_map(PCH_MAP mp)
{
	kfree(mp);
	mp = NULL;
}

//Resetting the map with key as 0 and value as NULL;
VOID ch_tree_reset_map(PCH_MAP mp)
{
	for (ch_tree_map_idx = 0; ch_tree_map_idx < ch_tree_map_size; ch_tree_map_idx++) {
		mp[ch_tree_map_idx].key = 0;
		mp[ch_tree_map_idx].value = NULL;
	}
}

//Searching will happen linearly. Just for simplification purpose.
PVOID ch_tree_get_map(ULONGLONG key)
{
	for (ch_tree_map_idx = 0; ch_tree_map_idx < ch_tree_map_size; ch_tree_map_idx++) {

		if (ch_tree_map[ch_tree_map_idx].key == key) {
			return ch_tree_map[ch_tree_map_idx].val;
		}
	}
	return NULL;
}

//Insertion in this map will happen linearly. Just for simplification purpose.
VOID ch_tree_set_map(INT key, PVOID value)
{
	PCH_MAP head_map = ch_tree_map;
	for (ch_tree_map_idx = 0; ch_tree_map_idx < ch_tree_map_size; ch_tree_map_idx++) {

		//Look for empty space in map and insert the key value there
		if (ch_tree_map[ch_tree_map_idx].key == 0) {

			*(head_map) = (CH_MAP) { .key = key, .val = value };
			break;
		}
		head_map++;
	}
}

VOID ch_tree_print_map(VOID)
{
	for (ch_tree_map_idx = 0; ch_tree_map_idx < ch_tree_map_size; ch_tree_map_idx++) {

		LOG("MAP->Print: Self = %p,  Key[%u] = %d, Value = %p\n", &ch_tree_map[ch_tree_map_idx], ch_tree_map_idx,
		ch_tree_map[ch_tree_map_idx].key, ch_tree_map[ch_tree_map_idx].value);
	}
}

ULONGLONG* ch_tree_create_queue(uint sz)
{
	ch_local_queue = kmalloc(sizeof(ULONGLONG) * sz, GFP_KERNEL);
	if (ch_local_queue) {
		LOG("QUEUE Created of size %u\n", sz);
	}
	return ch_local_queue;
}

VOID ch_tree_delete_queue(PVOID arg_q)
{
	kfree(arg_q);
	arg_q = NULL;
}

INT ch_tree_get_queue_count(PVOID arg_q)
{
	return (ch_local_queue_rear - ch_local_queue_front);
}

//Reset the queue i.e. all slots are initialized to NULL
VOID ch_tree_reset_queue(VOID)
{
	int i = 0;
	memset(ch_local_queue, 0, sizeof(*ch_local_queue));
	ch_local_queue_front = ch_local_queue_rear = 0;
	for (i = 0; i < ch_local_queue_size; i++) {
		ch_local_queue[i] = (ULONGLONG)NULL;
	}
}

//Enqueue the pointer. Note the typecasting
VOID ch_tree_enqueue(PVOID element)
{
	ch_local_queue[ch_local_queue_rear++] = (ULONGLONG)element;
}

//Dequeing element from front
PVOID ch_tree_dequeue(VOID)
{
	if (ch_local_queue_front == ch_local_queue_rear) return NULL;
	return (PVOID)ch_local_queue[ch_local_queue_front++];
}

//Print the node along with user supplied marker
//Also printing the total memory allocation info
VOID ch_tree_print_node(PCH_TREE_NODE node, char *marker)
{
	int i = 0;
	LOG("%s:Node->self         = %p\n", marker, node      );
	if (node) {
		LOG("%s:Node->data         = %d\n", marker, node->data);
		LOG("%s:Node->cycle        = %p\n", marker, node->cycle);
		if (node->cycle) {
			LOG("%s:Node->cycle->data  = %d\n", marker, node->cycle->data);
		}
		for (i = 0; i < CH_TREE_NODE_CHILDREN; i++) {
			LOG("%s:Node->children[%d] = %p\n", marker, i, node->childrens[i]);
		}
	}
	LOG("%s:Tree: No. of Allocated Nodes = %d\n",marker, ATOMIC_READ(ch_tree_alloc_node_cnt));
}

//Printing the tree in level line by line by traversing the tree in PreOrder.
//Using a queue to store the nodes temporarily
VOID ch_tree_print_level_order_line_by_line(PCH_TREE_NODE root)
{
	PCH_TREE_NODE	node = root;
	INT i = 0;
	INT	qcnt = 0;
	INT	level = 0;
	

	QUEUE(root);

	while(true) {
		qcnt = QUEUE_COUNT(ch_local_queue);

		LOG("qcnt = %d i.e. @ Level = %d\n", qcnt, level++);

		if (qcnt == 0) break;

		while(qcnt != 0)  {
			DEQUEUE(node);
			if (node->cycle) {
				LOG("[%c Data = %d Self = %p ] [%c  Cycle = %d Cycle->Self = %p ]\n",
					 node->ch, node->data, node, node->cycle->ch, node->cycle->data, node->cycle);
			} else {
				LOG("[%c Data = %d Self = %p ]\n",node->ch, node->data, node);
			}
	
			for (i = 0; i < CH_TREE_NODE_CHILDREN; i++) {
				if (node->childrens[i]) {
					QUEUE(node->childrens[i]);
				}
			}
			qcnt--;
		}
	}
}
//Printing the tree in level by traversing the tree in PreOrder.
//Using a queue to store the nodes temporarily
VOID ch_tree_print_level_order(PCH_TREE_NODE root)
{
	PCH_TREE_NODE	node = root;
	INT i = 0;
	while(node)  {
		if (node->cycle) {
			LOG("[%c Data = %d Self = %p ] [%c  Cycle = %d Cycle->Self = %p ]\n", node->ch, node->data, node, node->cycle->ch, node->cycle->data, node->cycle);
		} else {
			LOG("[%c] Data = %d Self = %p\n", node->ch, node->data, node);
		}

		for (i = 0; i < CH_TREE_NODE_CHILDREN; i++) {
			if (node->childrens[i]) {
				QUEUE(node->childrens[i]);
			}
		}
		DEQUEUE(node);
	}
}

//Destroy/freeup all the resources
VOID ch_tree_destroy(VOID)
{
	//DESTROY Original Tree
	ch_tree_delete(deep_tree_root);
	deep_tree_root 	= NULL;

	//DESTROY DeepCopy Tree
	ch_tree_delete(deep_clone_root);
	deep_clone_root = NULL;

	//DESTROY_MAP();	
	DELETE_MAP(ch_tree_map);

	//DESTROY_QUEUE
	DELETE_QUEUE(ch_local_queue);
}

//Initialize all resources for this module
VOID ch_tree_init(VOID)
{
	deep_tree_root 	= NULL;

	deep_clone_root = NULL;

	//Initialize the alloc counter
	ATOMIC_SET(ch_tree_alloc_node_cnt, 0);

	//Initialize map
	ch_tree_map				= NULL;
	ch_tree_map_idx			= 0;
	ch_tree_map_size		= MAP_SIZE;

	//Initialize Map
	CREATE_MAP();	

	//QUEUE Metadata
	ch_local_queue			= NULL;
	ch_local_queue_front	= 0;
	ch_local_queue_rear		= 0;
	ch_local_queue_size		= MAP_SIZE;
	//Initialize Queue. It will be used for level order printing to verify the correct cycle pointers 
	CREATE_QUEUE(ch_local_queue, ch_local_queue_size);


	//Initialize the alloc counter
	ATOMIC_SET(ch_tree_alloc_node_cnt, 0);
}

//Deleting the tree with postorder traversal
VOID ch_tree_delete(PCH_TREE_NODE root)
{
	int i = 0;
	if (root == NULL) {
		return;
	}
	
	//First delete its subtrees
	for (i = 0; i < CH_TREE_NODE_CHILDREN; i++) {
		if (root->childrens[i]) {
			ch_tree_delete(root->childrens[i]);
		}
	}
	
	//Now finally delete the root
	ch_tree_delete_node(root);
}


static int ch_tree_module_init(void)
{
    LOG(" ______________________________________________________ \n");
    LOG("|                                                      |\n");
    LOG("|           Tree DeepCopy Module Loaded                |\n");
    LOG("|______________________________________________________|\n");

	//Initialize all necessary resources/variables
	ch_tree_init();

	//Start processing of deep copy
	ch_tree_main();
    return 0;
}

static void ch_tree_module_exit(void)
{
	//Free up the resources
	ch_tree_destroy();
	LOG("Tree Memory Allocation Count = %d\n", ATOMIC_READ(ch_tree_alloc_node_cnt));
	//This should be zero
	
    LOG(" ______________________________________________________ \n");
    LOG("|                                                      |\n");
    LOG("|           Tree DeepCopy Module UnLoaded              |\n");
    LOG("|______________________________________________________|\n");
}

module_init(ch_tree_module_init);
module_exit(ch_tree_module_exit);
MODULE_LICENSE(CH_MODULE_LICENSE);
MODULE_AUTHOR(CH_MODULE_AUTHOR);
