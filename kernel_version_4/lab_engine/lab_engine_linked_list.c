/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

PDZ_LIST_NODE 	list_root 	= NULL;
PDZ_LIST_NODE 	list_head	 = NULL;

RVOID dz_list_print_node(PDZ_LIST_NODE node)
{
	BOOL flag = TRUE;
	if (!node) {
		LOGTGT("Node Print: List Node is NULL\n");
		return;
	}


	if (flag == TRUE) {
		LOGTGT("[-------------LIST   NODE----------]\n");
		LOGTGT("[                                  ]\n");
		LOGTGT("[ NODE           = %p]\n", node);
		LOGTGT("[ NODE->data     = %ld]\n", node->data);
		LOGTGT("[ NODE->next     = %p]\n", node->next);
		LOGTGT("[ NODE->prev     = %p]\n", node->prev);
		LOGTGT("[----------------------------------]\n");
	}

}

PDZ_LIST_NODE dz_list_alloc_node(VOID)
{
    PDZ_LIST_NODE node = DZ_LIST_ALLOC_NODE();
	if (!node) {
		LOGTGTE("%s","Error, Unable to allocate memory for interval tree node\n");
		RETURNN;
	}
	MEMSET(node);
	return node;
}

UINT dz_list_length(PDZ_LIST_NODE node)
{
	UINT len = 0;
	while(node) {
		node = node->next;
		len++;
	}
	return len;
}

RVOID dz_list_traverse(PDZ_LIST_NODE node)
{
	while(node) {
		dz_list_print_node(node);
		node = node->next;
	}
}

RVOID dz_list_traverse_from_end(PDZ_LIST_NODE node)
{
	while(node) {
		dz_list_print_node(node);
		node = node->prev;
	}
}

PDZ_LIST_NODE dz_list_find_loop(PDZ_LIST_NODE root)
{

	PDZ_LIST_NODE slow = root;
	PDZ_LIST_NODE fast = root->next;
	UINT perimeter = 0;
	UINT i = 0;

	while(slow) {

		if (slow == fast) {
			//At this point, slow and fast pointer meets, so that means
			//there is a loop in the list.
			//Now calculate the length/perimeter of the loop

			while(fast) {
				fast = fast->next;
				if (fast == slow) {
					break;
				}
				perimeter++;
			}
			LOGTGT ("Loop Encountered. Loop Perimter = %u\n", perimeter);
			break;
		}
		slow = slow->next;
		fast = fast->next->next;
	}

	/*
	Loop present at node 35 :

	20->45->35->80->90->60->110->
			|                   |
			|___________________|

	loop_node->current = 35
	loop_node2->current = 110
	*/

	//Above code tells that loop is found.
	//Now Again traverse to find the loop node;
	//Outer while loop will visit each node one by one
	//Inner while loop will will visit all perimeter nodes for each outer while node
	//If the active node of outer while loop matches with
	//active node of inner while loop then this the node where
	//the actual loop starts

	slow = root;
	//perimeter++;

	while(slow) {
		i = 0;
		fast = slow->next;
		while(i < perimeter) {
			//PRINT_POINTER(fast);
			fast = fast->next;
			i++;
		}

		if (slow == fast) {
			LOGTGT ("Loop Node Found\n");
			dz_list_print_node(slow);
			break;
		}

		slow = slow->next;
	}

	return slow;
}

PDZ_LIST_NODE dz_list_push(PDZ_LIST_NODE root, LONG data)
{
	PDZ_LIST_NODE head = root;
	if (!root) {
		root = dz_list_alloc_node();
		root->data = data;
		return root;

	} else {
		//Go to the end of list
		while(root->next) {
			root = root->next;
		}
		root->next = dz_list_alloc_node();
		root->next->data = data;
		root->next->prev = root;
		return head;
	}
}

PDZ_LIST_RETURN_NODE dz_list_push_with_return_node(PDZ_LIST_NODE root, LONG data)
{
	PDZ_LIST_NODE head = root;
	PDZ_LIST_RETURN_NODE rnode = DZ_ALLOC_NODE(sizeof(DZ_LIST_RETURN_NODE));
	if (!root) {
		root = dz_list_alloc_node();
		root->data = data;
		rnode->head = root;
		rnode->tail = NULL;
		return rnode;

	} else {
		//Go to the end of list
		while(root->next) {
			root = root->next;
		}
		root->next = dz_list_alloc_node();
		root->next->data = data;
		root->next->prev = root;

		rnode->head = head;
		rnode->tail = root->next;
		return rnode;
	}
}

/*
	e.g.
	20->45->35->80->90->60->110->
			|                   |
			|___________________|

*/
RVOID dz_loop_detection(VOID)
{
	PDZ_LIST_RETURN_NODE loop_node = NULL;
	PDZ_LIST_RETURN_NODE loop_node2 = NULL;

	list_root = dz_list_push(list_root, 20);
	list_root = dz_list_push(list_root, 45);
	loop_node = dz_list_push_with_return_node(list_root, 35);
	list_root = dz_list_push(list_root, 80);
	list_root = dz_list_push(list_root, 90);
	list_root = dz_list_push(list_root, 60);
	loop_node2 = dz_list_push_with_return_node(list_root, 110);
	dz_list_traverse(list_root);

	/*
	First create a loop
	e.g.
	20->45->35->80->90->60->110->
			|                   |
			|___________________|

	loop_node->current = 35
	loop_node2->current = 110
	*/


	loop_node2->current_node->next = loop_node->current_node;

	dz_list_find_loop(list_root);
	//dz_list_traverse(list_root);

}

/*

	List L2 = 7->2----------
                           |
	List L1 = 10->39->80->25->78->54->90->17->NULL

	Both the lists merge at node 25
	Rules:
	1. Cant change any of the list 

*/
BOOL dz_find_two_lists_merging(VOID)
{
	PDZ_LIST_RETURN_NODE common_node = NULL;
	PDZ_LIST_RETURN_NODE rnode = NULL;
	PDZ_LIST_NODE list1 = NULL;
	PDZ_LIST_NODE list2 = NULL;
	UINT	list1_len = 0;
	UINT	list2_len = 0;

	//Create List L1 first
	list1 = dz_list_push(list1, 10);
	list1 = dz_list_push(list1, 39);
	list1 = dz_list_push(list1, 80);
	common_node = dz_list_push_with_return_node(list1, 25);
	list1 = dz_list_push(list1, 78);
	list1 = dz_list_push(list1, 54);
	list1 = dz_list_push(list1, 90);
	list1 = dz_list_push(list1, 17);
	LOGTGT("Printing Original List-1\n");
	dz_list_traverse(list1);

	//list2 = dz_list_push(list2, 100);
	//list2 = dz_list_push(list2, 200);
	//list2 = dz_list_push(list2, 300);
	//list2 = dz_list_push(list2, 400);
	list2 = dz_list_push(list2, 7);
	rnode = dz_list_push_with_return_node(list2, 2);
	LOGTGT("Printing Original List-2\n");
	dz_list_traverse(list2);
	//Merge this list with list1 at node common_node
	//rnode->current_node will be 2 and its next pointer will be NuLL.
	//common_node->current_node will point to node 25
	rnode->current_node->next = common_node->current_node;
	//Create List L1 first

	LOGTGT("Printing List-2 after merging with L1\n");
	dz_list_traverse(list2);

	list1_len = dz_list_length(list1);
	list2_len = dz_list_length(list2);
	LOGTGT("Length of List-1 = %u, Length of List-2 = %u\n", list1_len, list2_len);


	if (list1_len > list2_len) {
		while(list1_len > list2_len) {
			list1 = list1->next;
			list1_len--;
		}
	} else if (list1_len < list2_len) {
		while(list2_len > list1_len) {
			list2 = list2->next;
			list2_len--;
		}
	} else {
		list1 = list1;
		list2 = list2;
	}

	while(1) {

		if (list1->data == list2->data) {
			LOGTGT("Merged Node Found\n");
			dz_list_print_node(list1);
			break;
		}
		list1 = list1->next;
		list2 = list2->next;

	}
	RETURNT;

}

PDZ_LIST_NODE dz_merge_two_lists(DZ_LIST_NODE *list1, DZ_LIST_NODE *list2)
{
	PDZ_LIST_NODE head = NULL;
	if (list1 == NULL) {
		return list2;
	}
	if (list2 == NULL) {
		return list1;
	}

	if (list1->data <= list2->data) {

		head = list1;
		head->next = dz_merge_two_lists(list1->next, list2);
	} else {
		head = list2;
		head->next = dz_merge_two_lists(list1, list2->next);
	}

	return head;
}

/*
	Merge Two Sorted lists

	L1 = 10->39->55->75->80->96
	L2 = 1->14->44->89

	Final List = 1->10->14->39->44->55->75->80->89->96
*/
BOOL dz_merge_lists(VOID)
{

	PDZ_LIST_NODE list1 = NULL;
	PDZ_LIST_NODE list2 = NULL;
	UINT	list1_len = 0;
	UINT	list2_len = 0;
	list1 = dz_list_push(list1, 10);
	list1 = dz_list_push(list1, 39);
	list1 = dz_list_push(list1, 55);
	list1 = dz_list_push(list1, 75);
	list1 = dz_list_push(list1, 80);
	list1 = dz_list_push(list1, 96);

	list2 = dz_list_push(list2, 1);
	list2 = dz_list_push(list2, 14);
	list2 = dz_list_push(list2, 44);
	list2 = dz_list_push(list2, 89);

	list1_len = dz_list_length(list1);
	list2_len = dz_list_length(list2);
	LOGTGT("Length of List-1 = %u, Length of List-2 = %u\n", list1_len, list2_len);
	list1 = dz_merge_two_lists(list1, list2);
	dz_list_traverse(list1);
	list1_len = dz_list_length(list1);
	list2_len = dz_list_length(list2);
	LOGTGT("After Merging Length of List-1 = %u, Length of List-2 = %u\n", list1_len, list2_len);
	RETURNT;
}

BOOL dz_process_lab_engine_linked_list(LBA offset, UINT iosize)
{
	LOGTGT("LAB Engine Linked List\n");

	//dz_loop_detection();
	//dz_find_two_lists_merging();
	
	dz_merge_lists();
	RETURNT;
}

EXPORT_SYMBOL(dz_process_lab_engine_linked_list);
