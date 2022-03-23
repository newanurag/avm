/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _LAB_ENGINE_HEAP_H_
#define _LAB_ENGINE_HEAP_H_

typedef struct dz_heap_node_s {
	union {
		LONG data;
		CHAR data_str[8];
	};

	struct dz_heap_node_s *left;
	struct dz_heap_node_s *right;
	struct dz_heap_node_s *parent;
	struct dz_heap_node_s *root;

}DZ_HEAP_NODE, *PDZ_HEAP_NODE;

BOOL dz_process_lab_engine_heap(LBA offset, UINT iosize);


#define DZ_HEAP_ALLOC_NODE()		DZ_ALLOC_NODE(sizeof(DZ_HEAP_NODE))

#endif // _LAB_ENGINE_HEAP_H_
