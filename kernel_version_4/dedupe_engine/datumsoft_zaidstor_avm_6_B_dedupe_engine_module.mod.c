#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xe783361, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x9f9a3495, __VMLINUX_SYMBOL_STR(dz_bio_page_free) },
	{ 0x94d683f, __VMLINUX_SYMBOL_STR(kmem_cache_destroy) },
	{ 0x7aadc27a, __VMLINUX_SYMBOL_STR(DEDUPE_WRITE_THREAD_POOL_SHORT_NAME) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0xe2decbaf, __VMLINUX_SYMBOL_STR(dz_generate_hash) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0x672edad8, __VMLINUX_SYMBOL_STR(pv_lock_ops) },
	{ 0xe198d8af, __VMLINUX_SYMBOL_STR(dz_avm_log_file_write) },
	{ 0xd3aa8b4f, __VMLINUX_SYMBOL_STR(dedupe_zeroed_out_page_reads) },
	{ 0x2570cae1, __VMLINUX_SYMBOL_STR(_RELEASE_LBA_LOCK_READ) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x5e1f6b50, __VMLINUX_SYMBOL_STR(dz_bio_alloc) },
	{ 0x9a19868e, __VMLINUX_SYMBOL_STR(dedupe_tot_physical_space) },
	{ 0x9edbbbaa, __VMLINUX_SYMBOL_STR(reduce_memory_usage) },
	{ 0xb2d2ec6c, __VMLINUX_SYMBOL_STR(dedupe_disk_reads) },
	{ 0xef6832fe, __VMLINUX_SYMBOL_STR(lba_table_size) },
	{ 0x9a3d333d, __VMLINUX_SYMBOL_STR(TARGET_DEVICE_SIZE) },
	{ 0x86ab69f2, __VMLINUX_SYMBOL_STR(dedupe_cur_physical_space) },
	{ 0x75afd1db, __VMLINUX_SYMBOL_STR(log_memory_usage) },
	{ 0xc3d2c1fd, __VMLINUX_SYMBOL_STR(dz_memory_reduction) },
	{ 0x4c7a8fae, __VMLINUX_SYMBOL_STR(mempool_destroy) },
	{ 0xaeeff187, __VMLINUX_SYMBOL_STR(dedupe_hash_table) },
	{ 0xce935a56, __VMLINUX_SYMBOL_STR(dz_kfree) },
	{ 0xdf3df2da, __VMLINUX_SYMBOL_STR(dedupe_deleted_entries) },
	{ 0x5bab484b, __VMLINUX_SYMBOL_STR(_ACQUIRE_PBA_LOCK) },
	{ 0xc57904d4, __VMLINUX_SYMBOL_STR(dz_switch_dedupe_engine) },
	{ 0x964b32b0, __VMLINUX_SYMBOL_STR(dz_read_page_sync) },
	{ 0xb686300d, __VMLINUX_SYMBOL_STR(_RELEASE_LBA_LOCK_WRITE) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x118950c1, __VMLINUX_SYMBOL_STR(dz_bec_proceed_pba_page_delete) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0xb175e60d, __VMLINUX_SYMBOL_STR(dz_write_page_alloc) },
	{ 0x97651e6c, __VMLINUX_SYMBOL_STR(vmemmap_base) },
	{ 0x4ebce310, __VMLINUX_SYMBOL_STR(number_of_longs_for_lba) },
	{ 0x2266e6c6, __VMLINUX_SYMBOL_STR(DEDUPE_READ_THREAD_POOL_NAME) },
	{ 0x56bc3d41, __VMLINUX_SYMBOL_STR(_DZ_DDE_RELEASE_HBA_LOCK) },
	{ 0x3f4b90ba, __VMLINUX_SYMBOL_STR(gpba_bitmap) },
	{ 0xa6682fdd, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0x183fa88b, __VMLINUX_SYMBOL_STR(mempool_alloc_slab) },
	{ 0xb1904934, __VMLINUX_SYMBOL_STR(wait_for_completion) },
	{ 0x638b14e6, __VMLINUX_SYMBOL_STR(dedupe_num_threads) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xf2a2c67d, __VMLINUX_SYMBOL_STR(dz_destroy_thread_pool) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x449ad0a7, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0x42f23226, __VMLINUX_SYMBOL_STR(bio_add_page) },
	{ 0x48b52b5d, __VMLINUX_SYMBOL_STR(dedupe_pba_table) },
	{ 0x7089ad04, __VMLINUX_SYMBOL_STR(dedupe_lba_table) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0xeeff0455, __VMLINUX_SYMBOL_STR(pba_table_size) },
	{ 0x1a58118e, __VMLINUX_SYMBOL_STR(_ACQUIRE_LBA_LOCK_READ) },
	{ 0xd3db4fe9, __VMLINUX_SYMBOL_STR(generic_make_request) },
	{ 0x8a99a016, __VMLINUX_SYMBOL_STR(mempool_free_slab) },
	{ 0x92710dda, __VMLINUX_SYMBOL_STR(number_of_longs_for_gpba) },
	{ 0xc25419c9, __VMLINUX_SYMBOL_STR(bio_endio) },
	{ 0xefa08cc4, __VMLINUX_SYMBOL_STR(bio_put) },
	{ 0xf11543ff, __VMLINUX_SYMBOL_STR(find_first_zero_bit) },
	{ 0x4a79de8a, __VMLINUX_SYMBOL_STR(dz_metadata_update) },
	{ 0x7cd8d75e, __VMLINUX_SYMBOL_STR(page_offset_base) },
	{ 0x23844cec, __VMLINUX_SYMBOL_STR(dz_bec_proceed_pba_page_add_read) },
	{ 0xd24da7de, __VMLINUX_SYMBOL_STR(dz_get_memory_usage) },
	{ 0x1d1dcfee, __VMLINUX_SYMBOL_STR(dz_memory_consumption) },
	{ 0x9a6f2471, __VMLINUX_SYMBOL_STR(mempool_alloc) },
	{ 0xdb1a1e40, __VMLINUX_SYMBOL_STR(pba_bitmap) },
	{ 0x4a9865a8, __VMLINUX_SYMBOL_STR(dz_create_thread_pool) },
	{ 0x772aeb24, __VMLINUX_SYMBOL_STR(dz_write_biodone_sync) },
	{ 0x5635a60a, __VMLINUX_SYMBOL_STR(vmalloc_user) },
	{ 0x3c3825b1, __VMLINUX_SYMBOL_STR(pdz_dde_counters) },
	{ 0xae569e6c, __VMLINUX_SYMBOL_STR(dz_page_free) },
	{ 0x1c0529b1, __VMLINUX_SYMBOL_STR(dz_io_free) },
	{ 0xb7a0a67e, __VMLINUX_SYMBOL_STR(print_biom) },
	{ 0x696727a5, __VMLINUX_SYMBOL_STR(mempool_create) },
	{ 0x6b2dc060, __VMLINUX_SYMBOL_STR(dump_stack) },
	{ 0x7ff6484c, __VMLINUX_SYMBOL_STR(lba_bitmap) },
	{ 0xf5ef1c13, __VMLINUX_SYMBOL_STR(_ACQUIRE_HBA_LOCK) },
	{ 0x3c525f53, __VMLINUX_SYMBOL_STR(dedupe_cache_hits) },
	{ 0x54730b37, __VMLINUX_SYMBOL_STR(number_of_longs_for_pba) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xe5c6ae21, __VMLINUX_SYMBOL_STR(mempool_free) },
	{ 0x56321ae2, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x8f5136ef, __VMLINUX_SYMBOL_STR(hash_table_size) },
	{ 0x961d8335, __VMLINUX_SYMBOL_STR(kmem_cache_create) },
	{ 0x6bb4641f, __VMLINUX_SYMBOL_STR(_ACQUIRE_LBA_LOCK_WRITE) },
	{ 0xc238146f, __VMLINUX_SYMBOL_STR(dz_bec_pba_page_lookup) },
	{ 0x5c6d5076, __VMLINUX_SYMBOL_STR(dz_io_alloc) },
	{ 0xda2af4e7, __VMLINUX_SYMBOL_STR(DEDUPE_READ_THREAD_POOL_SHORT_NAME) },
	{ 0x273a080c, __VMLINUX_SYMBOL_STR(DEDUPE_WRITE_THREAD_POOL_NAME) },
	{ 0x2ac56908, __VMLINUX_SYMBOL_STR(DATA_DISK_DEVICE) },
	{ 0xfe94db38, __VMLINUX_SYMBOL_STR(dedupe_cur_duplicate_space) },
	{ 0x2b000831, __VMLINUX_SYMBOL_STR(_DZ_DDE_RELEASE_PBA_LOCK) },
	{ 0x74b99461, __VMLINUX_SYMBOL_STR(iorequest_get) },
	{ 0xfe50d590, __VMLINUX_SYMBOL_STR(dz_convert_lba_to_sector) },
	{ 0x58cc04a3, __VMLINUX_SYMBOL_STR(dedupe_cur_logical_space) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x86421af2, __VMLINUX_SYMBOL_STR(dedupe_tot_logical_space) },
	{ 0x775a6ce6, __VMLINUX_SYMBOL_STR(iorequest_put) },
	{ 0xae9629e2, __VMLINUX_SYMBOL_STR(dz_bio_page_alloc) },
	{ 0xaebd92d, __VMLINUX_SYMBOL_STR(dz_kmalloc) },
	{ 0x72aa95db, __VMLINUX_SYMBOL_STR(dz_read_biodone_sync) },
	{ 0x865e055, __VMLINUX_SYMBOL_STR(dedupe_disk_writes) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=datumsoft_zaidstor_avm_2_B_infra_engine_module,datumsoft_zaidstor_avm_5_B_metadata_engine_module,datumsoft_zaidstor_avm_4_B_bec_engine_module";


MODULE_INFO(srcversion, "748D8058EE06FDEC13C666A");
