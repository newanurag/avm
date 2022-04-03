/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _LAB_ENGINE_LINKED_LIST_H_
#define _LAB_ENGINE_LINKED_LIST_H_

typedef struct dz_list_node_s {
	union {
		LONG data;
		CHAR data_str[8];
	};

	struct dz_list_node_s *next;
	struct dz_list_node_s *prev;

} DZ_LINKED_LIST_NODE, *PDZ_LINKED_LIST_NODE, DZ_LIST_NODE, *PDZ_LIST_NODE;

typedef struct dz_list_return_node_s {
	struct dz_list_node_s *head;
	union {
		struct dz_list_node_s *tail;
		struct dz_list_node_s *current_node;
	};

} DZ_LIST_RETURN_NODE, *PDZ_LIST_RETURN_NODE;

BOOL dz_process_lab_engine_linked_list(LBA offset, UINT iosize);
#define DZ_LIST_ALLOC_NODE()		DZ_ALLOC_NODE(sizeof(DZ_LIST_NODE))

#endif //_LAB_ENGINE_LINKED_LIST_H_
