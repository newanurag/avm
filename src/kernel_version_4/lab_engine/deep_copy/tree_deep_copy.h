#ifndef _TREE_DEEP_COPY_H_
#define _TREE_DEEP_COPY_H_

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/kernel.h> 
#include <linux/fs.h>
#include <linux/errno.h> 
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

#define CH_MODULE_LICENSE   "GPL"
#define CH_MODULE_AUTHOR    "Cohesity"

typedef char                CHAR;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned long long  ULONGLONG;
typedef void *              PVOID;
typedef void                VOID;
typedef atomic_t            ATOMIC; 

#define CH_TREE_NODE_CHILDREN	3

//Tree Node structure supports DZ_TREE_NODE_CHILDRENs along with one cycle pointer
typedef struct tree_node_s {
	CHAR ch;
	INT data;
	struct tree_node_s *childrens[CH_TREE_NODE_CHILDREN];
	struct tree_node_s *cycle;

}  __attribute__((__packed__)) CH_TREE_NODE, *PCH_TREE_NODE;

//Map Datastructure
typedef struct map_s {
	INT key;
	union {
		PVOID		value;
		PVOID		val;
	};
} __attribute__((__packed__)) CH_MAP, *PCH_MAP;



#define LOG(fmt, args...) do { \
    printk (KERN_ERR"[DeepCopy]:%d:" fmt ,__LINE__, ## args); \
} while (0)



#define PRINT_ORIGINAL_TREE_NODE(node) \
	ch_tree_print_node((node), "Original")

#define PRINT_DEEPCOPY_TREE_NODE(node) \
	ch_tree_print_node((node), "DeepCopy")

#define PRINT_DELETED_NODE(node) \
	ch_tree_print_node((node), "Deleted")

#define PRINT_ALLOCATED_NODE(node) \
	ch_tree_print_node((node), "Allocated")

#define	CLONE_NODE(arg_node, arg_data)	\
*((arg_node) = ch_tree_alloc_node()) = (CH_TREE_NODE){ .ch = 'C', .data = (arg_data), .childrens[0] = NULL, NULL, NULL, NULL}

#define	INSERT_NODE(arg_node, arg_data)	\
*((arg_node) = ch_tree_alloc_node()) = (CH_TREE_NODE){ .ch = 'O', .data = (arg_data), .childrens[0] = NULL, NULL, NULL, NULL}

//Atomic variables
#define ATOMIC_SET(var, val)        atomic_set(&(var), val)
#define ATOMIC_INC(var)             atomic_inc(&(var))
#define ATOMIC_DEC(var)             atomic_dec(&(var))
#define ATOMIC_READ(var)            atomic_read(&(var))

//General Tree functions
VOID 			ch_tree_init(VOID);
VOID 			ch_tree_destroy(VOID);
PCH_TREE_NODE 	ch_tree_create(PCH_TREE_NODE root);
VOID 			ch_tree_delete(PCH_TREE_NODE root);
PCH_TREE_NODE 	ch_tree_alloc_node(VOID);
VOID 			ch_tree_delete_node(PCH_TREE_NODE node);
VOID			ch_tree_main(VOID);
VOID 			ch_tree_print_node(PCH_TREE_NODE node, char *marker);
VOID 			ch_tree_print_level_order(PCH_TREE_NODE root);
VOID 			ch_tree_main(VOID);
INT 			ch_tree_get_alloc_count(VOID);
VOID 			ch_tree_print_level_order_line_by_line(PCH_TREE_NODE root);

//Approach Map
VOID 			ch_tree_deep_copy_approach_map(VOID);
PCH_TREE_NODE 	ch_tree_deep_copy_with_map(PCH_TREE_NODE root);
VOID 			ch_tree_deep_copy_cycle_with_map(PCH_TREE_NODE root, PCH_TREE_NODE clone);
PCH_TREE_NODE 	ch_tree_deep_copy_core_with_map(PCH_TREE_NODE root);

//Approach Modify Left Child
VOID 			ch_tree_deep_copy_approach_modify_left_child(VOID);
PCH_TREE_NODE 	ch_tree_deep_copy_with_modify_left(PCH_TREE_NODE root);
PCH_TREE_NODE 	ch_tree_deep_copy_core_with_modify_left(PCH_TREE_NODE root);
VOID 			ch_tree_deep_copy_cycle_with_modify_left(PCH_TREE_NODE root, PCH_TREE_NODE clone);
VOID 			ch_tree_deep_copy_restore_with_modify_left(PCH_TREE_NODE root, PCH_TREE_NODE clone);

//Approach Modify and queue
VOID 			ch_tree_deep_copy_approach_modify_and_queue(VOID);
PCH_TREE_NODE 	ch_tree_deep_copy_with_modify_and_queue(PCH_TREE_NODE root);
PCH_TREE_NODE 	ch_tree_deep_copy_core_with_modify_and_queue(PCH_TREE_NODE root);
VOID		 	ch_tree_deep_copy_cycle_with_modify_and_queue(PCH_TREE_NODE root, PCH_TREE_NODE clone_root);


//MAP functions prototype
PCH_MAP 		ch_tree_create_map(UINT sz);
VOID 			ch_tree_reset_map(PCH_MAP mp);
PVOID 			ch_tree_get_map(ULONGLONG Key);
VOID 			ch_tree_set_map(INT key, PVOID Value);
VOID 			ch_tree_print_map(VOID);
VOID 			ch_tree_delete_map(PCH_MAP mp);

//MAP Macros
#define	MAP_SIZE					32
#define CREATE_MAP()	\
	ch_tree_map = ch_tree_create_map(ch_tree_map_size)

#define SET_MAP(arg_key, arg_value)	\
	ch_tree_set_map(arg_key, arg_value)

#define	GET_MAP(arg_key)	\
	ch_tree_get_map(arg_key)

#define	PRINT_MAP()	\
	ch_tree_print_map()

#define	DELETE_MAP(arg_map)	\
	ch_tree_delete_map(arg_map)

//QUEUE functions prototype
ULONGLONG* 		ch_tree_create_queue(UINT sz);
VOID 			ch_tree_enqueue(PVOID element);
PVOID 			ch_tree_dequeue(VOID);
VOID 			ch_tree_reset_queue(VOID);
VOID			ch_tree_delete_queue(PVOID arg_q);
INT 			ch_tree_get_queue_count(PVOID arg_q);

//Queue Macros
#define CREATE_QUEUE(arg_q, arg_q_size)	\
	arg_q = ch_tree_create_queue(arg_q_size)

#define	QUEUE(arg_element)	\
	ch_tree_enqueue((void *)(arg_element))

#define	RESET_QUEUE()	\
	ch_tree_reset_queue()

#define	DQUEUE(arg_element)	\
	arg_element = (PCH_TREE_NODE)ch_tree_dequeue()

#define	DEQUEUE(arg_element)	DQUEUE(arg_element)

#define	DELETE_QUEUE(arg_q)	\
	ch_tree_delete_queue(arg_q)

#define	QUEUE_COUNT(arg_q)	\
	ch_tree_get_queue_count(arg_q)

#endif //_TREE_DEEP_COPY_H_
