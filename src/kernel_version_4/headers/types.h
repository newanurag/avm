#ifndef AISA_TYPES_H
#define AISA_TYPES_H

#define LINUX_VERSION_CODE 199168
//#define LINUX_VERSION_CODE 331008 // For Kernel version 5.13.0-30-generic on Ubuntu 21.04
//#define LINUX_VERSION_CODE 265999 // For Kernel version 4.14.271 on Ubuntu 18.04 downloaded from kernel.org
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define RHEL_MAJOR 7
#define RHEL_MINOR 0
#define RHEL_RELEASE_VERSION(a,b) (((a) << 8) + (b))
#define RHEL_RELEASE_CODE 1792
#define RHEL_RELEASE "123"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,14,0)
#define KERN_VER_3_14
#elif LINUX_VERSION_CODE == KERNEL_VERSION(3,13,6)
#define KERN_VER_3_13
#elif LINUX_VERSION_CODE == KERNEL_VERSION(2,6,36)
#define KERN_VER_2_6_36
#endif
#include <linux/types.h>
#include <linux/spinlock_types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/bio.h>
#include <linux/device-mapper.h>
#include <linux/slab.h>
#include <linux/delay.h> 
#include <linux/sysfs.h> 
#include <linux/fs.h> 
#include <linux/configfs.h> 
#include <linux/kobject.h> 
#include <linux/completion.h> 
#include <linux/kthread.h> 
#include <stdbool.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/syscalls.h>

#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#include <trace/events/block.h>


#include "about.h"
#include "hash_engine_murmur3.h"

#define	DATE_LENGTH			8
#define	TIME_LENGTH			6
#define	NAME_LENGTH			BDEVNAME_SIZE
#define	UUID_LENGTH			( 2 * NAME_LENGTH )
#define	GUID_LENGTH			( 2 * NAME_LENGTH )
#define	MAGICNO_LENGTH		( 2 * NAME_LENGTH )

typedef char 				S8;
typedef char 				CHAR;
typedef char 		  *		PCHAR;
typedef unsigned char 		UCHAR;
typedef unsigned char 		U8;
typedef unsigned char * 	PUCHAR;

typedef short 				S16;
typedef short 				SHORT;
typedef unsigned short 		U16;
typedef unsigned short 		USHORT;

typedef int 				S32;
typedef int 				INT;
typedef unsigned int 		UINT;
typedef unsigned int 		U32;
typedef int					RINT; // Useful for function returns

typedef long 				LONG;
typedef long long 			S64;
typedef long long 			LONGLONG;
typedef unsigned long 		ULONG;
typedef unsigned long long 	U64;
typedef unsigned long long 	ULONGLONG;
typedef ULONGLONG			KEY;

typedef void 				VOID;
typedef void 				RVOID;
typedef	void * 				PVOID;
typedef	void * 				RPVOID;
typedef bool				BOOL;



typedef size_t				SIZE;
typedef size_t 				LBA_SIZE;
typedef size_t 				PBA_SIZE;
typedef size_t				DATA_SIZE;
typedef size_t				LBA_DATA_SIZE;	// Size of Data inside LBA
typedef size_t				PBA_DATA_SIZE;	// Size of Data inside PBA

typedef spinlock_t 			LOCK;
typedef spinlock_t 			SPINLOCK;
typedef	ULONGLONG			TIME;
typedef	ULONGLONG			CHECKSUM;
typedef	ULONGLONG			CRC;
typedef	ULONGLONG			SHA1;
typedef	ULONGLONG			SHA256;
typedef	ULONGLONG			DATA_HASH;
typedef	UINT				FLAG;
typedef	UINT				FLAGS;
typedef unsigned long long 	COUNT;
typedef unsigned long long 	INDEX;
typedef void *				LBA_LIST;
typedef void *				PBA_LIST;
typedef void *				FLASH_VOLUMES_LIST;
typedef void *				NEXT;
typedef void *				PREV;

typedef unsigned char 		NAME [NAME_LENGTH   ];
typedef	unsigned char		UUID [UUID_LENGTH   ];
typedef	unsigned char		GUID [GUID_LENGTH   ];
typedef	unsigned char		MAGC [MAGICNO_LENGTH];

#define	MAGICNO				MAGC

typedef struct list_head	LIST_ITEM;
typedef struct list_head	LIST_HEAD;

typedef struct dm_dev		DM_DEV;
typedef struct dm_dev *		PDM_DEV;


typedef	mempool_t			MEMPOOL;
typedef	mempool_t			*PMEMPOOL;

typedef struct bio			BIO;
typedef struct bio			*PBIO;
typedef struct bio_set 		BIO_SET;
typedef struct bio_set 		*PBIO_SET;

typedef gfp_t				GFP;


typedef struct page			PAGE;
typedef struct page			*PPAGE;
typedef struct bio_vec		BIOVEC;
typedef struct bio_vec		*PBIOVEC;
typedef struct bio_vec		BIO_VEC;
typedef struct bio_vec		*PBIO_VEC;

typedef sector_t			SECTOR;
typedef struct block_device	BLOCK_DEVICE;
typedef struct block_device	*PBLOCK_DEVICE;

typedef U64					LBA;
typedef U64					PBA;

typedef atomic_t			ATOMIC;
typedef atomic_t			ATOMIC32;
typedef atomic64_t			ATOMIC64;

typedef struct dm_target 	DM_TARGET;
typedef struct dm_target 	*PDM_TARGET;

typedef struct completion   COMPLETION;
typedef struct task_struct  THREAD;
typedef bio_end_io_t		BIO_CALLBACK;
typedef bio_end_io_t		*BIODONE;
typedef bio_end_io_t		*BIO_END_IO;
typedef struct radix_tree_root	RADIX_TREE_ROOT;
typedef struct radix_tree_root	*PRADIX_TREE_ROOT;

typedef void (iorequest_done_t) (PVOID , int);
typedef iorequest_done_t	*IOREQUEST_DONE;

typedef struct bvec_iter	ITER;
typedef struct bvec_iter	BIOITER;
typedef struct bvec_iter	ITERATOR;

typedef struct bvec_iter	*PITER;
typedef struct bvec_iter	*PBIOITER;
typedef struct bvec_iter	*PITERATOR;
#endif  // Types
