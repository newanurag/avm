#ifndef AISA_MACROS_H
#define AISA_MACROS_H

#include <linux/device-mapper.h>

#define	GET_CPU_ID()				smp_processor_id()
#define	GET_CPUID()					smp_processor_id()
#define AISA_VIRTUAL_FLASH_VOLUME   "AISA_VIRTUAL_FLASH_VOLUME"
#define AISA_LOGICAL_FLASH_VOLUME   "AISA_LOGICAL_FLASH_VOLUME"

#define	DEVICE_TYPE_AISA						"DEVICE_TYPE_AISA"
#define	DEVICE_TYPE_DATA_VOLUME					"DEVICE_TYPE_DATA_VOLUME"
#define	DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_READ	"DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_READ"
#define	DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_WRITE	"DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_WRITE"
#define	DEVICE_TYPE_VM_VOLUME					"DEVICE_TYPE_VM_VOLUME"
#define	DEVICE_TYPE_VM_VOLUME_SNAPSHOT_READ		"DEVICE_TYPE_VM_VOLUME_SNAPSHOT_READ"
#define	DEVICE_TYPE_VM_VOLUME_SNAPSHOT_WRITE	"DEVICE_TYPE_VM_VOLUME_SNAPSHOT_WRITE"

#define	DEVICE_TYPE_SNAPSHOT		DEVICE_TYPE_SNAPSHOT_READ

#define	AISA_METADATA_DEVICE_NAME	"/dev/md0"

#define DZ_ERROR  	1
#define DZ_WARN 	2
#define DZ_INFO 	3
#define DZ_SHOW 	4
#define DZ_DBG  	5
#define DZ_FLOW 	6

#define SUCCESS 	0
#define FAILURE 	-1

#define DZ_SUCCESS 	0
#define DZ_FAILURE 	-1

#define	ENABLED		1
#define DISABLED	0

#define	DZ_ENABLED	1
#define	DZ_ENABLE 	1
#define DZ_DISABLED	0
#define DZ_DISABLE 	0

#define TRUE		true
#define FALSE		false

#define	INLINE		inline

#define	KB			(1024)
#define	MB			(1024 * KB)
#define	GB			(1024 * MB)
#define	TB			1024 * GB	// TerraByte
#define	PB			1024 * TB	// PetaByte
#define	EB			1024 * PB	// ExaByte
#define	ZB			1024 * EB	// ZettaByte
#define	YB			1024 * ZB	// YottaByte
#define	BB			1024 * YB	// BrontoByte

#define	TO_KB(X)	(X * KB)

#define _1K		TO_KB(1)
#define _2K		TO_KB(2)
#define _3K		TO_KB(3)
#define _4K		TO_KB(4)
#define _5K		TO_KB(5)
#define _6K		TO_KB(6)
#define _7K		TO_KB(7)
#define _8K		TO_KB(8)
#define _8KB	TO_KB(8)
#define _9K		TO_KB(9)
#define _10K	TO_KB(10)
#define _11K	TO_KB(11)
#define _12K	TO_KB(12)
#define _13K	TO_KB(13)
#define _14K	TO_KB(14)
#define _15K	TO_KB(15)
#define _16K	TO_KB(16)
#define _16KB	TO_KB(16)

#define _32K	TO_KB(32) 
#define _32KB	TO_KB(32) 

#define _64K	TO_KB(64)
#define _64KB	TO_KB(64)

#define _128K	TO_KB(128)
#define _128KB	TO_KB(128)

#define _256K	TO_KB(256)
#define _256KB	TO_KB(256)

#define _512K	TO_KB(512)
#define _512KB	TO_KB(512)

#define _1024K	TO_KB(1024)
#define _1024KB	TO_KB(1024)

#define ONE_KB			TO_KB(1)
#define TWO_KB			TO_KB(2)
#define THREE_KB		TO_KB(3)
#define FOUR_KB			TO_KB(4)
#define FIVE_KB			TO_KB(5)					
#define SIX_KB			TO_KB(6)
#define SEVEN_KB		TO_KB(7)
#define EIGHT_KB		TO_KB(8)
#define NINE_KB			TO_KB(9)
#define TEN_KB			TO_KB(10)
#define ELEVEN_KB		TO_KB(11)
#define TWELVE_KB		TO_KB(12)
#define THIRTEEN_KB		TO_KB(13)
#define FOURTEEN_KB		TO_KB(14)
#define FIFTEEN_KB		TO_KB(15)
#define SIXTEEN_KB		TO_KB(16)

#ifndef SECTOR_SIZE
#define SECTOR_SIZE							512
#endif
#define LBA_BLOCK_SIZE  					PAGE_SIZE // Must be 4K
#define PBA_BLOCK_SIZE  					LBA_BLOCK_SIZE // Must be 4K
#define HBA_BLOCK_SIZE  					LBA_BLOCK_SIZE // Must be 4K
#define HASH_BLOCK_SIZE						HBA_BLOCK_SIZE
#define SECTORS_PER_PAGE  					8 // Based on 4K/512
#define SECTOR_PER_PAGE  					8
#define SUPERBLOCK_RESERVED_SECTORS			2 * SECTORS_PER_PAGE
#define SUPERBLOCK_SECTOR					0
#define SUPERBLOCK_SIZE						PAGE_SIZE
#define DZ_BEC_PAGE_SIZE					PAGE_SIZE
#define DZ_SYSFS_PAGE_SIZE					PAGE_SIZE

#define LOGR(fmt, args...) do { \
	printk (KERN_ERR"" fmt , ## args); \
} while (0)

#define LOGF(fmt, args...) do { \
	printk (KERN_ERR"AVM:[%s:%d]" fmt ,__func__, __LINE__, ## args); \
} while (0)


//printk (KERN_ERR"AVM:[%s:%d]" fmt ,__func__, __LINE__, ## args);

#define LOGA(fmt, args...) do { \
	printk (KERN_ERR"AVM:[ALERT]" fmt , ## args); \
} while (0)

#define LOGI(fmt, args...) do { \
	printk (KERN_ERR"AVM:[INFO]" fmt , ## args); \
} while (0)

//Log to Memory i.e. Circular buffer in kernel
#define SINGNATURE(str1, str2)	str1""str2

#define LOGM(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[MEMLOG]",fmt), ##args);				\
} while (0)

//Log to Memory i.e. Circular buffer in kernel and /var/log/message both

#define LOGFM(fmt, args...) do { \
	dz_avm_log_file_write(fmt, ## args);				\
	printk (KERN_ERR"AVM:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

#define LOGMF		LOGFM

///////////////////////////////////////////////////////////
//Infrastrucutre Engine  File and Memory LOG macros
///////////////////////////////////////////////////////////
#define	LOGINF(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[INFRAS]",fmt), ##args);				\
} while (0)

//Memory and file /var/log/messages
#define	LOGINFF(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[INFRAS]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][INFRAS]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

//Infrastructure for Errors. Log it in both memory and file /var/log/messages
#define	LOGINFE(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[INFRAS:ERROR]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][INFRAS:ERROR]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

//Infrastructure for Warnings. Log it in both memory and file /var/log/messages
#define	LOGINFW(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[INFRAS:WARN]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][INFRAS:WARN]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

///////////////////////////////////////////////////////////
//BEC File and Memory LOG macros
///////////////////////////////////////////////////////////
//Only Memory
#define	LOGBEC(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[BCACHE]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][BCACHE]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

//Memory and file /var/log/messages
#define	LOGBECF(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[BCACHE]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][BCACHE]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

//BEC for Errors. Log it in both memory and file /var/log/messages
#define	LOGBECE(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[BCACHE:ERROR]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][BCACHE:ERROR]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

//BEC for Warnings. Log it in both memory and file /var/log/messages
#define	LOGBECW(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[BCACHE:WARN]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][BCACHE:WARN]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

///////////////////////////////////////////////////////////
//Metadata Engine (MET) File and Memory LOG macros
///////////////////////////////////////////////////////////
//Only Memory
#define	LOGMET(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[METDAT]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][METDAT]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

//Memory and file /var/log/messages
#define	LOGMETF(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[METDAT]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][METDAT]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

//Metadata for Errors. Log it in both memory and file /var/log/messages
#define	LOGMETE(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[METDAT:ERROR]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][METDAT:ERROR]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

//Metadata for Warnings. Log it in both memory and file /var/log/messages
#define	LOGMETW(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[METDAT:WARN]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][METDAT:WARN]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)


///////////////////////////////////////////////////////////
//DeDupe Engine (DDE) File and Memory LOG macros
///////////////////////////////////////////////////////////
//Only Memory
#define	LOGDDE(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[DEDUPE]",fmt), ##args);				\
} while (0)

//Memory and file /var/log/messages
#define	LOGDDEF(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[DEDUPE]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][DEDUPE]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

//DDE for Errors. Log it in both memory and file /var/log/messages
#define	LOGDDEE(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[DEDUPE:ERROR]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][DEDUPE:ERROR]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

//DDE for Warnings. Log it in both memory and file /var/log/messages
#define	LOGDDEW(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[DEDUPE:WARN]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][DEDUPE:WARN]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)



///////////////////////////////////////////////////////////
//Alignment Engine File and Memory LOG macros
///////////////////////////////////////////////////////////
//Only Memory
#define	LOGALIGND(fmt, args...) 

#define	LOGALIGN(fmt, args...) do { \
	printk (KERN_ERR"[AVM][ALIGN ]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

//Memory and file /var/log/messages
#define	LOGALIGNF(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[ALIGN ]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][ALIGN ]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

//Alginment Engine for Errors. Log it in both memory and file /var/log/messages
#define	LOGALIGNE(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[ALIGN:ERROR]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][ALIGN:ERROR]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

//Alignment Engine for Warnings. Log it in both memory and file /var/log/messages
#define	LOGALIGNW(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[ALIGN:WARN]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][ALIGN:WARN]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)




///////////////////////////////////////////////////////////
//FEC File and Memory LOG macros
///////////////////////////////////////////////////////////
//Only Memory
#define	LOGFEC(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[FCACHE]",fmt), ##args);				\
} while (0)

//Memory and file /var/log/messages
#define	LOGFECF(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[FCACHE]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][FCACHE]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

//FEC for Errors. Log it in both memory and file /var/log/messages
#define	LOGFECE(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[FCACHE:ERROR]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][FCACHE:ERROR]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

//FEC for Warnings. Log it in both memory and file /var/log/messages
#define	LOGFECW(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[FCACHE:WARN]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][FCACHE:WARN]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

///////////////////////////////////////////////////////////
//TARGET File and Memory LOG macros
///////////////////////////////////////////////////////////
//Only Memory
#define	LOGTGT(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[TARGET]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][TARGET]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

//Memory and file /var/log/messages
#define	LOGTGTF(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[TARGET]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][TARGET]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

//Target for Errors. Log it in both memory and file /var/log/messages
#define	LOGTGTE(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[TARGET:ERROR]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][TARGET:ERROR]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

//FEC for Warnings. Log it in both memory and file /var/log/messages
#define	LOGTGTW(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[TARGET:WARN]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][TARGET:WARN]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)



///////////////////////////////////////////////////////////
//Volume Engine File and Memory LOG macros
///////////////////////////////////////////////////////////
#define	LOGVOL(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[VOLUME]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][VOLUME]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

#define	LOGVOLF(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[VOLUME]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][VOLUME]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

#define	LOGVOLE(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[VOLUME]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][VOLUME:ERROR]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

#define	LOGVOLW(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[VOLUME]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][VOLUME:WARN]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

///////////////////////////////////////////////////////////
//Sysfs Engine File and Memory LOG macros
///////////////////////////////////////////////////////////
#define	LOGSYS(fmt, args...) do { \
	printk (KERN_ERR"[AVM][SYSFS ]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

#define	LOGSYSF(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[SYSFS ]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][SYSFS ]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

#define	LOGSYSE(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[SYSFS ]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][SYSFS :ERROR]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)

#define	LOGSYSW(fmt, args...) do { \
	dz_avm_log_file_write(SINGNATURE("[SYSFS ]",fmt), ##args);				\
	printk (KERN_ERR"[AVM][SYSFS :WARN]:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
	dump_stack();															\
} while (0)


#define LOG(fmt, args...) do { \
	printk (KERN_ERR"AVM:%d:%d:" fmt ,__LINE__,GET_CPUID(), ## args); \
} while (0)

/*
#define LOGD(fmt, args...) do { \
	printk (KERN_ERR"AVM:[DEBUG]:%s:%s:%d:" fmt ,__FILE__, __func__, __LINE__, ## args); \
} while (0)
*/
#define LOGD(fmt, args...) 

// Logger for Errors only
#define LOGE(fmt, args...) do { \
	printk (KERN_ERR"AVM:[ERROR]:%s:%d:" fmt ,__func__, __LINE__, ## args); \
	dump_stack();															\
} while (0)

#define LOGC(fmt, args...) do { \
	printk (KERN_ERR"AVM:[CERROR]:%s:%d:" fmt ,__func__, __LINE__, ## args); \
} while (0)

#define LOGW(fmt, args...) do { \
	printk (KERN_ERR"AVM:[WARN]:%s:%d:" fmt ,__func__, __LINE__, ## args); \
	dump_stack();															\
} while (0)

#define MARK	LOG("%s:%d\n",__func__, __LINE__)

#define SLEEP(x)        					msleep( x * 1000)
#define MSLEEP(x)       					msleep(x)

#define	SIZE_OF(ptr)			sizeof(*(ptr))
#define	SIZEOF(ptr)				SIZE_OF(ptr)

#define	DEFINE_RETURN	INT ret = SUCCESS
#define	RETURNS			return SUCCESS
#define	SRETURN			return SUCCESS
#define	RETURNF			return FAILURE
#define	FRETURN			return FAILURE
#define	RETURNN			return NULL;
#define	NRETURN			return NULL;
#define	RETURN 			return ret
#define	RETURNT 		return true
#define	RETURNFA 		return false
#define	VRETURN 		return //Used in VOID
#define	RETURNV 		return //Used in VOID

#define	GET_TIMESTAMP(t)					t = get_jiffies_64()
#define	GET_TIME_STAMP(t)					GET_TIMESTAMP(t)
#define	GET_CURRENT_TIME()					get_jiffies_64()
#define	GET_CURRENT_TIME_IN_SECONDS(j)		jiffies_to_msecs(j)
#define	GET_TIME_IN_SECONDS(j)				jiffies_to_msecs(j)
#define	GET_TIME_DIFF_IN_SECONDS(t1, t2)	jiffies_to_msecs(t2 - t1)

//extern PBIO_SET        dz_mempool_bio;        
//extern PMEMPOOL        dz_mempool_slab;        
//extern PMEMPOOL        dz_mempool_page;        
//extern PMEMPOOL        dz_mempool_read_page;        
//extern PMEMPOOL        dz_mempool_write_page;

#define DZ_MEMPOOL_READ_PAGE_ALLOC(ptr)				\
do {												\
	ptr =mempool_alloc(dz_mempool_read_page, GFP_NOIO);\
	if(!ptr) {										\
		ptr = NULL;									\
	}												\
} while(0)

#define DZ_MEMPOOL_READ_PAGE_FREE(ptr)		\
do {										\
	mempool_free(ptr, dz_mempool_read_page);	\
}while(0) 

//#define DZ_BLOCK_DEVICE_MODE (FMODE_READ | FMODE_WRITE | FMODE_EXCL)
#define DZ_BLOCK_DEVICE_MODE (FMODE_READ | FMODE_WRITE)

#define DZ_NUMBER_OF_WRITE_PAGES_IN_A_BIO	64
#define DZ_NUMBER_OF_READ_PAGES_IN_A_BIO	64

#define MEMSET(ptr)			memset(ptr, 0, SIZEOF(ptr))

#define	MEMSET_PAGE(page) 			\
do {								\
	PVOID   pagebuf = NULL;			\
	pagebuf = kmap(page);			\
	memset(pagebuf, 0, PAGE_SIZE);	\
	kunmap(page);					\
}while(0) 	
#define	WRITE_PAGE(page, str, len)	\
do {								\
	PVOID   pagebuf = NULL;			\
	pagebuf = kmap(page);			\
	memset(pagebuf, 0, PAGE_SIZE);	\
	memcpy(pagebuf, str, (len > PAGE_SIZE) ? PAGE_SIZE : len);		\
	kunmap(page);					\
}while(0) 	

#define WRITE_MODULE_DESC	"Write Module for issuing writes using bio framework"
#define READ_MODULE_DESC	"Read  Module for issuing reads using bio framework"
#define	AISA_MODULE_AUTHOR	"DatumSoft"

#define DZ_QUIESCE_IO           true
#define DZ_ACTIVE_IO            false

#define	SL1						SLEEP(1) 	// Sleep for 1 sec
#define	SL2						SLEEP(2)
#define	SL3						SLEEP(3)
#define	SL5						SLEEP(5)
#define	SL10					SLEEP(10)	// Sleep for 10 seconds
#define	SL20					SLEEP(20)
#define	SL30					SLEEP(30)
#define	SL60					SLEEP(60)
#define	SL100					SLEEP(100)
#define	SL180					SLEEP(180)	// Sleep for 180 seconds or 3 mins
#define	SL300					SLEEP(300)
#define	SL600					SLEEP(600)

// Look for inttypes.h for getting the format specifier
#define	PRINT_ATOMIC(var)		LOG("%s = %d, 0x%x\n", #var, atomic_read(&(var)), atomic_read(&(var)))
#define	PRINT_ATOMIC64(var)		LOG("%s = %li, 0x%lx\n", #var, atomic64_read(&(var)), atomic64_read(&(var)))
#define	PRINT_STRING(str)		LOG("%s = %s\n", #str, str)
#define	PRINT_LONG(var)			LOG("%s = %ld, 0x%lx\n", #var, var, var)
#define	PRINT_ULONG(var)		LOG("%s = %lu, 0x%lx\n", #var, var, var)
#define	PRINT_ULONGLONG(var)	LOG("%s = %llu, 0x%llx\n", #var, var, var)
#define	PRINT_U64(var)			LOG("%s = %lli, 0x%llx\n", #var, var, var)
#define	PRINT_S64(var)			LOG("%s = %lld, 0x%llx\n", #var, var, var)
#define	PRINT_U32(var)			LOG("%s = %u, 0x%x\n", #var, var, var)
#define	PRINT_INT(var)			LOG("%s = %d, 0x%x\n", #var, var, var)
#define	PRINT_UINT(var)			PRINT_U32(var)
#define	PRINT_SHORT(var)		LOG("%s = %hi, 0x%x\n", #var, var, var)
#define	PRINT_USHORT(var)		LOG("%s = %hu, 0x%x\n", #var, var, var)
#define	PRINT_POINTER(var)		LOG("%s = %p\n", #var, var)
#define	PRINT_POINTERS(var, str) LOG("%s:%s = %p\n",str, #var, var)
#define	PRINT_SIZE(var)			LOG("%s = %zu, 0x%zx\n", #var, var, var)
#define	PRINT_SSIZE(var)		LOG("%s = %zd, 0x%zx\n", #var, var, var)
#define	PRINT_KEY(var)			LOG("%s = %llu, 0x%llx\n", #var, var, var)
#define	PRINT_KEYS(var, str) 	LOG("%s:%s = %llu\n",str, #var, var)
#define	PRINT_KEY_RAW(var)		LOG("%llu ",var)
#define	PRINT_KEY_RAWS(var,str)	LOG("%s: = %llu\n",str, var)

#define	PRINT_COUNT(var)		PRINT_ULONGLONG(var)
#define	PRINT_LBA(var)			PRINT_U64(var)
#define	PRINT_PBA(var)			PRINT_U64(var)
#define	PRINT_NAME(var)			PRINT_STRING(var)
#define	PRINT_UUID(var)			PRINT_STRING(var)
#define	PRINT_FLAGS(var)		PRINT_U32(var)
#define	PRINT_SECTOR(var)		LOG("%s = %lli, 0x%llx\n", #var, (U64)var, (U64)var)
#define	PRINT_SECTORS(var, str)		LOG("%s: %s = %lli, 0x%llx\n",str, #var, (U64)var, (U64)var)
#define	PRINT_LBAS(var, str)		LOG("%s: %s = %lli, 0x%llx\n",str, #var, var, var)

#define	PRINT_U64_V2(var1, var2)	LOG("%s = 0x%llx, %s = 0x%llx\n", #var1, var1, #var2, var2)
#define	PRINT_U64_V3(var1, var2)	LOG("%s = 0x%lli, %s = 0x%lli\n", #var1, var1, #var2, var2)
#define	PRINT_U32_V2(var1, var2, var3, var4)	LOG("%s = 0x%x, %s = 0x%x, %s = 0x%x, %s = 0x%x\n", #var1, var1, #var2, var2, #var3, var3, #var4, var4)
#define	PRINT_HASH64(hash)	 	PRINT_U64_V3((hash)->hash64[0], (hash)->hash64[1])
#define	PRINT_HASH(hash)	 	\
		PRINT_U32_V2((hash)->hash[0], (hash)->hash[1], (hash)->hash[2], (hash)->hash[3]);\
		PRINT_HASH64(hash)

#define IGNORE_INVALID_BVEC(cur_bvec)												\
	if((cur_bvec->bv_page == NULL) || (cur_bvec->bv_len == 0) ||					\
		(cur_bvec->bv_len > PAGE_SIZE) || (cur_bvec->bv_offset > PAGE_SIZE)) {		\
		LOGA("Invalid bvec len:%u off:%u, skipping",								\
		cur_bvec->bv_len, cur_bvec->bv_offset);										\
		continue;																	\
	}

//TODO: Need to free previous iorequests if they are allocated initially
//TODO: Need to free previous child bios if they are allocated initially

#define	CHECK_ADD_PAGE_STATUS(ret)					\
	if (!ret) {										\
		LOGE("Unable to add Page to child bio\n");	\
		bio_put(ciorequest->bio); 					\
		dz_iorequest_free(ciorequest); 				\
		RETURNF;									\
	} else {										\
		LOG("Child request getting queued\n");		\
	} 

#define GET_DISK_SECTOR()	\
			spin_lock(&aisa_device_object->pba_disk_offset_lock);	\
			aisa_device_object->pba_disk_offset;						\
			spin_unlock(&aisa_device_object->pba_disk_offset_lock);

#define DZ_BIO_SET_DEV(bio, bdev)				bio_set_dev((bio), (bdev))	
#define DZ_BIO_GET_DEV(bio)						bdget(bio_dev(bio))	

#define DZ_BIO_SET_BDEV(bio, bdev)				bio_set_dev((bio), (bdev))	
#define DZ_BIO_GET_BDEV(bio)					bdget(bio_dev(bio))	

#define DZ_BIO_SET_STATUS(bio, status)			(bio)->bi_status = (status)
#define DZ_BIO_GET_STATUS(bio)					(bio)->bi_status


#define DZ_BIO_SET_SIZE(bio, size)				(bio)->bi_iter.bi_size = (size)
#define DZ_BIO_GET_SIZE(bio)					(bio)->bi_iter.bi_size

#define DZ_BIO_SET_SECTOR(bio, sector)			(bio)->bi_iter.bi_sector = (sector)
#define DZ_BIO_GET_SECTOR(bio)					(bio)->bi_iter.bi_sector

#define DZ_BIO_SET_IDX(bio, idx)				(bio)->bi_iter.bi_idx = (idx)
#define DZ_BIO_GET_IDX(bio)						(bio)->bi_iter.bi_idx
#define DZ_BIO_SET_ZIDX(bio)					DZ_BIO_SET_IDX(bio, 0)

#define DZ_BIO_GET_CNT(bio)						(bio)->__bi_cnt

#define DZ_BIO_SET_WRITE(bio)					(bio)->bi_opf = REQ_OP_WRITE
#define DZ_BIO_SET_READ(bio)					(bio)->bi_opf = REQ_OP_READ
	
#define DZ_BIO_SET_NEXT(bio, n)					(bio)->bi_next = (n)
#define DZ_BIO_SET_NEXTN(bio)					(bio)->bi_next = NULL
#define DZ_BIO_GET_NEXT(bio)					(bio)->bi_next

#define DZ_BIO_SET_ENDIO(bio, fun)				(bio)->bi_end_io = (fun)
#define DZ_BIO_SET_BI_END_IO(bio, fun)			(bio)->bi_end_io = (fun)
#define DZ_BIO_SET_BIODONE(bio, fun)			(bio)->bi_end_io = (fun)
#define DZ_BIO_GET_ENDIO(bio)					(bio)->bi_end_io

#define DZ_BIO_SET_END_IO(bio, fun)				(bio)->bi_end_io = (fun)
#define DZ_BIO_GET_END_IO(bio)					(bio)->bi_end_io

#define DZ_BIO_SET_PRIVATE(bio, priv)			(bio)->bi_private = (priv)

#define DZ_IOREQUEST_GET_SECTOR(piorequest)		(piorequest)->ior_bio->bi_iter.bi_sector

#define BIO_DONE(bio, error)					bio_endio(bio)
#define IO_DONE(iorequest)						bio_endio((iorequest)->ior_bio)
#define READ_IO_DONE(iorequest)					bio_endio((iorequest)->ior_bio)
#define READ_IO_DONE_STATUS(iorequest, error)	bio_endio((iorequest)->ior_bio)
#define WRITE_IO_DONE_STATUS(iorequest, error)	bio_endio((iorequest)->ior_bio)
#define WRITE_IO_DONE(iorequest)				bio_endio((iorequest)->ior_bio)
#define FAKE_IO_DONE(iorequest)					bio_endio((iorequest)->ior_bio)
#define FAKE_BIO_DONE(bio)						bio_endio((bio), SUCCESS)
#define FAKE_BIODONE(bio)						bio_endio((bio), SUCCESS)

#define BIODONE(bio, error)						BIO_DONE(bio, error)
#define IODONE(iorequest)						IO_DONE(iorequest)

#define IO_DONE_ERROR(iorequest)				bio_io_error((iorequest)->ior_bio); LOGE("IOREQUEST IO_DONE ERROR\n")
#define IOREQUEST_DONE(iorequest)				bio_endio((iorequest)->ior_bio)
#define IOREQUEST_DONE_ERROR(iorequest)			bio_io_error((iorequest)->ior_bio); LOGE("IOREQUEST IO_DONE ERROR\n")
#define IO_DONE_BUSY(iorequest)					(iorequest)->ior_bio->bi_status = BLK_STS_DM_REQUEUE;bio_endio((iorequest)->ior_bio); LOGE("IO_DONE BUSY\n")
#define IOREQUEST_DONE_STATUS(iorequest)		bio_endio((iorequest)->ior_bio)


#define DZ_DDE_IOREQUEST_DONE(iorequest)				IOREQUEST_DONE(iorequest)
#define DZ_DDE_IOREQUEST_DONE_ERROR(iorequest)			IOREQUEST_DONE_ERROR((iorequest))

#define DZ_FEC_IOREQUEST_DONE(iorequest)				IOREQUEST_DONE(iorequest)
#define DZ_FEC_IOREQUEST_DONE_ERROR(iorequest, error)	IOREQUEST_DONE_ERROR((iorequest), (error))

#define DZ_BEC_IOREQUEST_DONE(iorequest)				IOREQUEST_DONE(iorequest)
#define DZ_BEC_IOREQUEST_DONE_ERROR(iorequest, error)	IOREQUEST_DONE_ERROR((iorequest), (error))

#define DZ_ALIGN_IOREQUEST_DONE(iorequest)				IOREQUEST_DONE(iorequest)
#define DZ_ALIGN_IOREQUEST_DONE_ERROR(iorequest, error)	IOREQUEST_DONE_ERROR((iorequest))


#define IODONE_BUSY(iorequest)			IO_DONE_BUSY(iorequest)

#define IO_WRITE_START(iorequest) 							\
	/*
	spin_lock(&aisa_device_object->pba_disk_offset_lock);	\
	(iorequest)->bio->bi_sector = dz_convert_lba_to_sector(aisa_device_object->pba_disk_offset++);	\
	spin_unlock(&aisa_device_object->pba_disk_offset_lock);	\
	*/ \
	(iorequest)->bio->bi_sector = (iorequest)->lba_entry->sector;	\
	generic_make_request((iorequest)->bio)

#define IO_WRITE_METADATA_START(iorequest, pba_index, dd_sector) 		\
	dz_metadata_write(iorequest, pba_index, dd_sector)

#define IO_WRITE_METADATA(iorequest, pba_index, dd_sector) 		\
	dz_metadata_write(iorequest, pba_index, dd_sector)

#define IO_METADATA_WRITE(iorequest, pba_index, dd_sector) 		\
	dz_metadata_write(iorequest, pba_index, dd_sector)

#define IO_METADATA_UPDATE(iorequest, pba_index, dd_sector) 		\
	dz_metadata_update(iorequest, pba_index, dd_sector)


#define IO_READ_START(iorequest) 							\
	generic_make_request((iorequest)->bio)

#define	IO_ERROR_EIO		-EIO
#define	IO_ERROR_EINVAL  	-EINVAL
#define	IO_ERROR_ENOMEM		-ENOMEM
#define	IO_ERROR_ENOSYS		-ENOSYS
#define	IO_RETRY			-EAGAIN

#define	DELAY_MICRO_SECONDS(x)	udelay(x)
#define	DELAY_NANO_SECONDS(x)	ndelay(x)
#define	DELAY_MILLI_SECONDS(x)	mdelay(x)
#define	SLEEP_MILLI_SECONDS(x)	msleep(x)

#define	MARK_MEMORY_USAGE() \
	U64 memory_usage_count = dz_get_memory_usage()

#define PRINT_MEMORY_USAGE(final_mem, marker) \
	dz_memory_consumption(memory_usage_count, &(final_mem), (marker))

#define PRINT_MEMORY_FREED(final_mem, marker) \
	dz_memory_reduction(memory_usage_count, &(final_mem), (marker))

#define ATOMIC_SET(var, val)		atomic_set(&(var), val)
#define ATOMIC64_SET(var, val)		atomic64_set(&(var), val)

#define	ATOMIC_INC(var)				atomic_inc(&(var))
#define	ATOMIC64_INC(var)			atomic64_inc(&(var))

#define	ATOMIC_DEC(var)				atomic_dec(&(var))
#define	ATOMIC64_DEC(var)			atomic64_dec(&(var))

#define	ATOMIC_ADD(var, val)		atomic_add(val, &(var))
#define	ATOMIC64_ADD(var, val)		atomic64_add(val, &(var))

#define	ATOMIC_SUB(var, val)		atomic_sub(val, &(var))
#define	ATOMIC64_SUB(var, val)		atomic64_sub(val, &(var))

#define	ATOMIC_READ(var)			atomic_read(&(var))
#define	ATOMIC64_READ(var)			atomic64_read(&(var))

//#define IF_WRITE_BIO(bio)			if ((bio->bi_rw & WRITE) == WRITE)
//#define IF_READ_BIO(bio)			if ((bio->bi_rw & READ ) == READ )
#define IF_WRITE_BIO(bio)			if (bio_op(bio) == REQ_OP_WRITE)
#define IS_WRITE_BIO(bio)			if (bio_op(bio) == REQ_OP_WRITE)
#define IF_BIO_WRITE(bio)			if (bio_op(bio) == REQ_OP_WRITE)
#define IS_BIO_WRITE(bio)			if (bio_op(bio) == REQ_OP_WRITE)

#define IF_READ_BIO(bio)			if (bio_op(bio) == REQ_OP_READ)
#define IS_READ_BIO(bio)			if (bio_op(bio) == REQ_OP_READ)
#define IF_BIO_READ(bio)			if (bio_op(bio) == REQ_OP_READ)
#define IS_BIO_READ(bio)			if (bio_op(bio) == REQ_OP_READ)

#define IF_BIO_ERROR(bio)			if (bio->bi_status != BLK_STS_OK)
#define IS_BIO_ERROR(bio)			if (bio->bi_status != BLK_STS_OK)

#define DZ_BIO_SET_ERROR_STATUS(bio)	bio->bi_status = BLK_STS_IOERR

#define DZ_BIO_SET_ERROR(bio, error)	bio->bi_status = error

#define IF_BIO_ERROR_SET_IO_ERROR(bio, error)		if (bio->bi_status != BLK_STS_OK) { error = -EIO;}
#define IF_BIO_ERROR_SET_NOMEM_ERROR(bio, error)	if (bio->bi_status != BLK_STS_OK) { error = -ENOMEM;}
#define IF_BIO_ERROR_SET_NOSPC_ERROR(bio, error)	if (bio->bi_status != BLK_STS_OK) { error = -ENOSPC;}

#define IF_WRITE_IOREQUEST(iorequest)		if ((iorequest)->ior_bio == NULL)

#define DZ_BIO_GET_PIN_COUNT(bio)		ATOMIC_READ((bio)->__bi_cnt)

#define	INC_COUNTER32(var)			ATOMIC_INC((var))
#define	INC_COUNTER64(var)			ATOMIC64_INC((var))
#define	INC_COUNTER(var)			INC_COUNTER64((var))

#define	DEC_COUNTER32(var)			ATOMIC_SUB((var))
#define	DEC_COUNTER64(var)			ATOMIC64_SUB((var))
#define	DEC_COUNTER(var)			DEC_COUNTER64((var))

#define	READ_COUNTER32(var)			ATOMIC_READ((var))
#define	READ_COUNTER64(var)			ATOMIC64_READ((var))
#define	READ_COUNTER(var)			READ_COUNTER64((var))

#define SET_COUNTER32(var)			ATOMIC_SET((var), 0)
#define SET_COUNTER64(var)			ATOMIC64_SET((var), 0)
#define SET_COUNTER(var)			SET_COUNTER64((var))


#define DZ_OS_KERNEL_BIO_PUT(arg)					bio_put((arg))
#define DZ_OS_KERNEL_BIO_GET(arg)					bio_get((arg))
#define DZ_OS_KERNEL_GENERIC_MAKE_REQUEST(arg)		generic_make_request((arg))
#define DZ_OS_KERNEL_SUBMIT_BIO(arg1, arg2)			submit_bio((arg1), (arg2))

#endif //For #ifndef AISA_MACROS_H
