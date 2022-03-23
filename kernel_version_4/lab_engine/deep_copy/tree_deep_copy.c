#include "tree_deep_copy.h"

//Root of original tree
PCH_TREE_NODE		deep_tree_root 	= NULL;

//Root of destination tree i.e. copy of original tree
PCH_TREE_NODE		deep_clone_root = NULL;

//Map Metadata / Placeholders
extern PCH_MAP	ch_tree_map;

//QUEUE Metadata
extern ULONGLONG *ch_local_queue;

//Starting function for copying the original tree to cloned tree
PCH_TREE_NODE ch_tree_deep_copy_with_map(PCH_TREE_NODE root)
{
	PCH_TREE_NODE clone;

	clone = ch_tree_deep_copy_core_with_map(root);
	ch_tree_deep_copy_cycle_with_map(root, clone);
	return clone;
}

//Copying the original tree node to cloned node. Here cycle pointer is not being
//copied. Instead a map is used to store the clone node address as the value of
//root->data key
PCH_TREE_NODE ch_tree_deep_copy_core_with_map(PCH_TREE_NODE root)
{
	PCH_TREE_NODE clone_node = NULL;
	int i = 0;

	if (root) {
		CLONE_NODE(clone_node, root->data);

		//Here the key is inserted as root->data and node which is linked is clone_node
		SET_MAP(root->data, clone_node);

		for (i = 0; i < CH_TREE_NODE_CHILDREN; i++) {
			if (root->childrens[i]) {
				clone_node->childrens[i] = ch_tree_deep_copy_core_with_map(root->childrens[i]);
			}
		}
	}
	return clone_node;
}
//Copy the cycle pointer from the map by using the root->cycle->data as key
VOID ch_tree_deep_copy_cycle_with_map(PCH_TREE_NODE root, PCH_TREE_NODE clone)
{
	int i = 0;

	if (root->cycle) {
		clone->cycle = (PCH_TREE_NODE)GET_MAP(root->cycle->data);
	}

	for (i = 0; i < CH_TREE_NODE_CHILDREN; i++) {
		if (root->childrens[i]) {
			ch_tree_deep_copy_cycle_with_map(root->childrens[i], clone->childrens[i]);
		}
	}
}


//Starting function for copying the original tree to cloned tree
PCH_TREE_NODE ch_tree_deep_copy_with_modify_left(PCH_TREE_NODE root)
{
	PCH_TREE_NODE clone;

	clone = ch_tree_deep_copy_core_with_modify_left(root);
	ch_tree_print_level_order_line_by_line(root);
	ch_tree_deep_copy_cycle_with_modify_left(root, clone);
	ch_tree_deep_copy_restore_with_modify_left(root, clone);
	return clone;
}


//Copying the original tree node to cloned node. Here cycle pointer is not being
//copied. Instead a map is used to store the clone node address as the value of
//root->data key
PCH_TREE_NODE ch_tree_deep_copy_core_with_modify_left(PCH_TREE_NODE root)
{
	PCH_TREE_NODE clone_node = NULL;

	PCH_TREE_NODE	temp = NULL;
	int i = 0;

	if (root) {

		temp = root->childrens[0];
		CLONE_NODE(clone_node, root->data);

		root->childrens[0] = clone_node;

		root->childrens[0]->childrens[0] = temp;

		if (temp) {
			temp->childrens[0] = ch_tree_deep_copy_core_with_modify_left(temp);
		}

		for (i = 1; i < CH_TREE_NODE_CHILDREN; i++) {
			clone_node->childrens[i] = ch_tree_deep_copy_core_with_modify_left(root->childrens[i]);
		}
	} else {
		return NULL;
	}
	return root->childrens[0];
}

//Copy the cycle pointer from the map by using the root->cycle->data as key
VOID ch_tree_deep_copy_cycle_with_modify_left(PCH_TREE_NODE root, PCH_TREE_NODE clone)
{
	int i = 0;

	if (root == NULL)
		return;

	if (root->cycle) {
		clone->cycle = root->cycle->childrens[0];
	} else {
		clone->cycle = NULL;
	}

	if (root->childrens[0] && clone->childrens[0]) {
		ch_tree_deep_copy_cycle_with_modify_left(root->childrens[0]->childrens[0], clone->childrens[0]->childrens[0]);
	}
	for (i = 1; i < CH_TREE_NODE_CHILDREN; i++) {
		ch_tree_deep_copy_cycle_with_modify_left(root->childrens[i], clone->childrens[i]);
	}
}


VOID ch_tree_deep_copy_restore_with_modify_left(PCH_TREE_NODE root, PCH_TREE_NODE clone)
{
	PCH_TREE_NODE node = NULL;
	int i = 0;
	if (root == NULL)
		return;

	if (clone->childrens[0]) {
		node = clone->childrens[0]->childrens[0];
		root->childrens[0] = clone->childrens[0];
		clone->childrens[0] = node;
	} else {
		root->childrens[0] = NULL;
	}

	for (i = 0; i < CH_TREE_NODE_CHILDREN; i++) {
		ch_tree_deep_copy_restore_with_modify_left(root->childrens[i], clone->childrens[i]);
	}
}

PCH_TREE_NODE 	ch_tree_deep_copy_with_modify_and_queue(PCH_TREE_NODE root)
{
	PCH_TREE_NODE clone;

	clone = ch_tree_deep_copy_core_with_modify_and_queue(root);
	LOG("Printing only clone tree after queueing\n");
	RESET_QUEUE();
	ch_tree_print_level_order_line_by_line(clone);

	LOG("END:Printing only clone tree after queueing\n");
	ch_tree_deep_copy_cycle_with_modify_and_queue(root, clone);
	LOG("Printing only clone tree after copy cycling\n");
	RESET_QUEUE();
	ch_tree_print_level_order_line_by_line(clone);

	ch_tree_deep_copy_restore_with_modify_and_queue(root);
	LOG("Printing only root tree after restoring\n");
	RESET_QUEUE();
	ch_tree_print_level_order_line_by_line(root);
	return clone;
}



PCH_TREE_NODE ch_tree_deep_copy_core_with_modify_and_queue(PCH_TREE_NODE root)
{
	PCH_TREE_NODE clone_node = NULL;
	int i = 0;

	if (root) {
		QUEUE(root->cycle);
		CLONE_NODE(clone_node, root->data);

		//Save the root's cycle pointer to clone_node->cycle
		clone_node ->cycle = root->cycle;
		//Now Point the root->cycle pointer to clone_node
		root->cycle = clone_node;

		for (i = 0; i < CH_TREE_NODE_CHILDREN; i++) {
			if (root->childrens[i]) {
				clone_node->childrens[i] = ch_tree_deep_copy_core_with_modify_and_queue(root->childrens[i]);
			}
		}
	}
	return clone_node;

}

VOID ch_tree_deep_copy_cycle_with_modify_and_queue(PCH_TREE_NODE root, PCH_TREE_NODE clone)
{
	int i = 0;

	if (!root && !clone) return;

	if (root->cycle && clone->cycle) {
		
		clone->cycle = clone->cycle->cycle;
	}

	for (i = 0; i < CH_TREE_NODE_CHILDREN; i++) {
		if (root->childrens[i]) {
			ch_tree_deep_copy_cycle_with_modify_and_queue(root->childrens[i], clone->childrens[i]);
		}
	}
}

VOID ch_tree_deep_copy_restore_with_modify_and_queue(PCH_TREE_NODE root)
{
	int i = 0;
	if (root) {
		DEQUEUE(root->cycle);
		for (i = 0; i < CH_TREE_NODE_CHILDREN; i++) {
			if (root->childrens[i]) {
				ch_tree_deep_copy_restore_with_modify_and_queue(root->childrens[i]);
			}
		}
	}
}

/* Function manually creating a Tree of 3 childrens in which one child also has 3 childrens + 1 cycle pointer.

                                            [root] - 11
                    ___________________________|________________
                   /                   /              \         \
                  /                   /                \         \
               ch1                   ch2               ch3       cycle (This cycle pointer points chA)
         (childrens[0])        (childrens[1])    (childrens[2])    \
            ___|____              ___|____                          \
           /  /  \  \            /  /  \  \                          \
          /  /    \  \          /  /    \  \                          \
       chA  chB  chC  cycle   chD chE  chF  cycle                     chA
*/
PCH_TREE_NODE ch_tree_create(PCH_TREE_NODE root)
{
	if (!root) {
		INSERT_NODE(root, 11);				 //root created
	}

	INSERT_NODE(root->childrens[0], 1121); //ch1 child created
	INSERT_NODE(root->childrens[1], 1122); //ch2 	""
	INSERT_NODE(root->childrens[2], 1123); //ch3 	""

	INSERT_NODE(root->childrens[0]->childrens[0], 2131); //chA child created
	INSERT_NODE(root->childrens[0]->childrens[1], 2132); //chB
	INSERT_NODE(root->childrens[0]->childrens[2], 2133); //chC

	INSERT_NODE(root->childrens[1]->childrens[0], 2231); //chD child created
	INSERT_NODE(root->childrens[1]->childrens[1], 2232); //chE
	INSERT_NODE(root->childrens[1]->childrens[2], 2233); //chF

	root->childrens[0]->cycle = root;	//11
	root->childrens[1]->cycle = root;	//11
	root->cycle = root->childrens[0]->childrens[0]; //chA
	return root;
}

VOID ch_tree_deep_copy_approach_map(VOID)
{
	//PCH_TREE_NODE pnode = NULL;
	PCH_TREE_NODE original = NULL;

	//DESTROY Original Tree
	ch_tree_delete(deep_tree_root);
	deep_tree_root 	= NULL;

	//DESTROY DeepCopy Tree
	ch_tree_delete(deep_clone_root);
	deep_clone_root = NULL;

	RESET_QUEUE();

	LOG("##########################################################\n");
	LOG("                 MAP: Original Tree Begin                \n");
	LOG("##########################################################\n");

	//First create original tree
	deep_tree_root = ch_tree_create(deep_tree_root);
	LOG("MAP:Original: Tree Created\n");
	original = deep_tree_root;

	//For verification purpose we can use below function to print the node and its childrens
	
	LOG("MAP:Original: Printing Original Tree Root Node\n");
	PRINT_ORIGINAL_TREE_NODE(deep_tree_root); //Print Just root node only
	LOG("MAP:Original: Printing Original Nodes at all levels \n");
	ch_tree_print_level_order_line_by_line(deep_tree_root); //Print data at all levels
	LOG("MAP:Original: Original Tree Memory Allocation Count = %d\n", ch_tree_get_alloc_count());

	LOG("##########################################################\n");
	LOG("                MAP: DeepCopy Begin                      \n");
	LOG("##########################################################\n");

	//Now DeepCopy original tree and return its pointer to deep_clone_root
	deep_clone_root = ch_tree_deep_copy_with_map(deep_tree_root);
	LOG("MAP:DeepCopy: Tree Created\n");
	LOG("MAP:DeepCopy: Printing DeepCopy Tree Root Node\n");
	PRINT_DEEPCOPY_TREE_NODE(deep_clone_root);
	LOG("MAP:DeepCopy: Printing DeepCopy Nodes at all levels \n");
	RESET_QUEUE();
	ch_tree_print_level_order_line_by_line(deep_clone_root);

	LOG("MAP:DeepCopy: DeepCopy Tree Memory Allocation Count = %d\n", ch_tree_get_alloc_count());

	return;
}

VOID ch_tree_deep_copy_approach_modify_left_child(VOID)
{
	//PCH_TREE_NODE pnode = NULL;
	PCH_TREE_NODE original = NULL;

	//DESTROY Original Tree
	ch_tree_delete(deep_tree_root);
	deep_tree_root 	= NULL;

	//DESTROY DeepCopy Tree
	ch_tree_delete(deep_clone_root);
	deep_clone_root = NULL;

	RESET_QUEUE();


	LOG("##########################################################\n");
	LOG("            MODIFY_LEFT: Original Tree Begin              \n");
	LOG("##########################################################\n");

	//First create original tree
	deep_tree_root = ch_tree_create(deep_tree_root);
	LOG("ML:Original: Tree Created\n");
	original = deep_tree_root;

	//For verification purpose we can use below function to print the node and its childrens
	
	LOG("ML:Original: Printing Original Tree Root Node\n");
	PRINT_ORIGINAL_TREE_NODE(deep_tree_root); //Print Just root node only
	LOG("ML:Original: Printing Original Nodes at all levels \n");
	ch_tree_print_level_order_line_by_line(deep_tree_root); //Print data at all levels
	LOG("ML:Original: Original Tree Memory Allocation Count = %d\n", ch_tree_get_alloc_count());

	LOG("##########################################################\n");
	LOG("                MODIFY_LEFT: DeepCopy Begin               \n");
	LOG("##########################################################\n");

	//Now DeepCopy original tree and return its pointer to deep_clone_root
	deep_clone_root = ch_tree_deep_copy_with_modify_left(deep_tree_root);
	LOG("ML:DeepCopy: Tree Created\n");
	LOG("ML:DeepCopy: Printing DeepCopy Tree Root Node\n");
	PRINT_DEEPCOPY_TREE_NODE(deep_clone_root);
	LOG("ML:DeepCopy: Printing DeepCopy Nodes at all levels \n");
	RESET_QUEUE();
	ch_tree_print_level_order_line_by_line(deep_clone_root);

	LOG("ML:DeepCopy: DeepCopy Tree Memory Allocation Count = %d\n", ch_tree_get_alloc_count());

	return;
}

VOID ch_tree_deep_copy_approach_modify_and_queue(VOID)
{
	//PCH_TREE_NODE pnode = NULL;
	PCH_TREE_NODE original = NULL;

	//DESTROY Original Tree
	ch_tree_delete(deep_tree_root);
	deep_tree_root 	= NULL;

	//DESTROY DeepCopy Tree
	ch_tree_delete(deep_clone_root);
	deep_clone_root = NULL;

	RESET_QUEUE();

	LOG("##########################################################\n");
	LOG("                 MODIFY_QUEUE: Original Tree Begin        \n");
	LOG("##########################################################\n");

	//First create original tree
	deep_tree_root = ch_tree_create(deep_tree_root);
	LOG("MQ:Original: Tree Created\n");
	original = deep_tree_root;

	//For verification purpose we can use below function to print the node and its childrens
	
	LOG("MQ:Original: Printing Original Tree Root Node\n");
	PRINT_ORIGINAL_TREE_NODE(deep_tree_root); //Print Just root node only
	LOG("MQ:Original: Printing Original Nodes at all levels \n");
	ch_tree_print_level_order_line_by_line(deep_tree_root); //Print data at all levels
	LOG("MQ:Original: Original Tree Memory Allocation Count = %d\n", ch_tree_get_alloc_count());

	LOG("##########################################################\n");
	LOG("                MODIFY_QUEUE: DeepCopy Begin              \n");
	LOG("##########################################################\n");

	//Now DeepCopy original tree and return its pointer to deep_clone_root
	deep_clone_root = ch_tree_deep_copy_with_modify_and_queue(deep_tree_root);
	LOG("MQ:DeepCopy: Tree Created\n");
	LOG("MQ:DeepCopy: Printing DeepCopy Tree Root Node\n");
	PRINT_DEEPCOPY_TREE_NODE(deep_clone_root);
	LOG("MQ:DeepCopy: Printing DeepCopy Nodes at all levels \n");
	RESET_QUEUE();
	ch_tree_print_level_order_line_by_line(deep_clone_root);

	LOG("MQ:DeepCopy: DeepCopy Tree Memory Allocation Count = %d\n", ch_tree_get_alloc_count());

	return;
}

//Main processing for DeepCopy starts from here
VOID ch_tree_main(VOID)
{

	ch_tree_deep_copy_approach_modify_and_queue();

	ch_tree_deep_copy_approach_modify_left_child();
	ch_tree_deep_copy_approach_map();
	return;
}

