/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _LBA_ENGINE_INTERVAL_H_
#define _LBA_ENGINE_INTERVAL_H_

typedef struct dz_interval_s {
    union {
        LBA  low;
        LBA  val;
        LBA  value;
        LBA  data;
        LBA  key;
    };  
    LBA    high; //high is calculated as low + len

}DZ_INTERVAL, *PDZ_INTERVAL;

typedef struct interval_node_s {

    DZ_INTERVAL     interval;
    LBA         max; // Max value of high in both left and right subtree
    UINT        num_nodes; //Total n. of nodes present in the tree

    struct interval_node_s *left;
    struct interval_node_s *right;
    struct interval_node_s *parent;
    struct interval_node_s *root;

} DZ_INTERVAL_NODE, *PDZ_INTERVAL_NODE, DZ_INTERVAL_TREE_NODE, *PDZ_INTERVAL_TREE_NODE;


BOOL dz_process_lab_engine_interval(LBA offset, UINT iosize);
#define DZ_INTERVAL_ALLOC_NODE()	DZ_ALLOC_NODE(sizeof(DZ_INTERVAL_NODE))

#endif //_LBA_ENGINE_INTERVAL_H_
