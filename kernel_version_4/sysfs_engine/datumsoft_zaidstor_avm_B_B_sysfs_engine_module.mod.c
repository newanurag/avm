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
	{ 0xef165bb0, __VMLINUX_SYMBOL_STR(GET_HASH_ENTRY) },
	{ 0x315cd2d8, __VMLINUX_SYMBOL_STR(kobject_put) },
	{ 0xe0a4b68a, __VMLINUX_SYMBOL_STR(dz_switch_alignment_engine_multi_block_read) },
	{ 0x1e9740c8, __VMLINUX_SYMBOL_STR(align_read_thread_pool) },
	{ 0xafa0f9d5, __VMLINUX_SYMBOL_STR(fec_iocount_reads_single_unaligned_page) },
	{ 0xbf6d71c0, __VMLINUX_SYMBOL_STR(align_iocount_reads_single_unaligned_page) },
	{ 0xe73492e2, __VMLINUX_SYMBOL_STR(fec_iocount_reads_single_page) },
	{ 0xa9abbfb6, __VMLINUX_SYMBOL_STR(bec_cutoff_percent) },
	{ 0x45ace347, __VMLINUX_SYMBOL_STR(dz_switch_alignment_engine) },
	{ 0x86bc7368, __VMLINUX_SYMBOL_STR(fec_active_count_sequence) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0x672edad8, __VMLINUX_SYMBOL_STR(pv_lock_ops) },
	{ 0xe198d8af, __VMLINUX_SYMBOL_STR(dz_avm_log_file_write) },
	{ 0x726ef563, __VMLINUX_SYMBOL_STR(tot_application_iocount_reads) },
	{ 0xd3aa8b4f, __VMLINUX_SYMBOL_STR(dedupe_zeroed_out_page_reads) },
	{ 0xdc7a74c9, __VMLINUX_SYMBOL_STR(metadata_flush_thread_pool) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0xa598d2fb, __VMLINUX_SYMBOL_STR(pdz_bec_counters) },
	{ 0x9a19868e, __VMLINUX_SYMBOL_STR(dedupe_tot_physical_space) },
	{ 0x5feb4ea4, __VMLINUX_SYMBOL_STR(align_iocount_writes_multi_unaligned_page) },
	{ 0xb2d2ec6c, __VMLINUX_SYMBOL_STR(dedupe_disk_reads) },
	{ 0x72d6f9f, __VMLINUX_SYMBOL_STR(fec_iocount_writes_single_aligned_page) },
	{ 0xef6832fe, __VMLINUX_SYMBOL_STR(lba_table_size) },
	{ 0x571b2dff, __VMLINUX_SYMBOL_STR(align_iocount_reads_multi_page) },
	{ 0x95d24c89, __VMLINUX_SYMBOL_STR(fec_flush_ops_count) },
	{ 0x382ddbd1, __VMLINUX_SYMBOL_STR(num_bec_threads) },
	{ 0xac829d48, __VMLINUX_SYMBOL_STR(wdedupe_thread_pool) },
	{ 0x9741a857, __VMLINUX_SYMBOL_STR(fec_iocount_writes_partial_page) },
	{ 0x9a3d333d, __VMLINUX_SYMBOL_STR(TARGET_DEVICE_SIZE) },
	{ 0x772cb655, __VMLINUX_SYMBOL_STR(dz_write_data_to_block_device) },
	{ 0x86ab69f2, __VMLINUX_SYMBOL_STR(dedupe_cur_physical_space) },
	{ 0x2a3f0cdf, __VMLINUX_SYMBOL_STR(time_flush_prepare_start) },
	{ 0xc0a3d105, __VMLINUX_SYMBOL_STR(find_next_bit) },
	{ 0x71446fbf, __VMLINUX_SYMBOL_STR(fec_free_count_sequence) },
	{ 0xce935a56, __VMLINUX_SYMBOL_STR(dz_kfree) },
	{ 0x71f4636b, __VMLINUX_SYMBOL_STR(write_thread_pool) },
	{ 0x9bd7700f, __VMLINUX_SYMBOL_STR(align_iocount_writes_multi_page) },
	{ 0xe1a597e1, __VMLINUX_SYMBOL_STR(dz_switch_passthrough_read) },
	{ 0xdf3df2da, __VMLINUX_SYMBOL_STR(dedupe_deleted_entries) },
	{ 0xa3e0a3f3, __VMLINUX_SYMBOL_STR(read_thread_pool) },
	{ 0xc57904d4, __VMLINUX_SYMBOL_STR(dz_switch_dedupe_engine) },
	{ 0x4f6a962b, __VMLINUX_SYMBOL_STR(dz_get_io_data) },
	{ 0xc5a657da, __VMLINUX_SYMBOL_STR(fec_cur_flush_ops_count) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x502c30d9, __VMLINUX_SYMBOL_STR(align_iocount_reads_single_page) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0x97651e6c, __VMLINUX_SYMBOL_STR(vmemmap_base) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x4ebce310, __VMLINUX_SYMBOL_STR(number_of_longs_for_lba) },
	{ 0x7beb76ca, __VMLINUX_SYMBOL_STR(pdz_basic_counters) },
	{ 0xcacfbf76, __VMLINUX_SYMBOL_STR(sysfs_remove_group) },
	{ 0x15ba50a6, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0x932b32b3, __VMLINUX_SYMBOL_STR(align_iocount_writes_partial_page) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0xb75b320b, __VMLINUX_SYMBOL_STR(kobject_create_and_add) },
	{ 0xf2ea068c, __VMLINUX_SYMBOL_STR(time_flush_end) },
	{ 0xde9d2aa6, __VMLINUX_SYMBOL_STR(fec_forwarded_reads_count) },
	{ 0x6835983d, __VMLINUX_SYMBOL_STR(tot_application_iocount_reads_single_page) },
	{ 0x64739013, __VMLINUX_SYMBOL_STR(fec_iocount_writes_single_unaligned_page) },
	{ 0x3c80c06c, __VMLINUX_SYMBOL_STR(kstrtoull) },
	{ 0x4149710a, __VMLINUX_SYMBOL_STR(num_align_write_threads) },
	{ 0x9b98c6c5, __VMLINUX_SYMBOL_STR(aisa_device_object) },
	{ 0x1aa6f7ab, __VMLINUX_SYMBOL_STR(GET_PBA_ENTRY) },
	{ 0x446c4f88, __VMLINUX_SYMBOL_STR(bec_cache_delete) },
	{ 0x4f26c6b1, __VMLINUX_SYMBOL_STR(fec_iocount_writes_multi_unaligned_page) },
	{ 0x71a448f6, __VMLINUX_SYMBOL_STR(metadata_flush_interval) },
	{ 0xeeb40040, __VMLINUX_SYMBOL_STR(GET_LBA_ENTRY) },
	{ 0x455dd896, __VMLINUX_SYMBOL_STR(tot_application_iocount) },
	{ 0x37befc70, __VMLINUX_SYMBOL_STR(jiffies_to_msecs) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x984e39a0, __VMLINUX_SYMBOL_STR(sysfs_create_group) },
	{ 0xfdfc8c01, __VMLINUX_SYMBOL_STR(fec_iocount_reads_multi_unaligned_page) },
	{ 0x5c3c34b4, __VMLINUX_SYMBOL_STR(metadata_lba_table_size) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0x9166fada, __VMLINUX_SYMBOL_STR(strncpy) },
	{ 0xe9eb9c31, __VMLINUX_SYMBOL_STR(bec_size) },
	{ 0xfa89cf66, __VMLINUX_SYMBOL_STR(bec_cache_miss) },
	{ 0xeeff0455, __VMLINUX_SYMBOL_STR(pba_table_size) },
	{ 0xe8886a14, __VMLINUX_SYMBOL_STR(time_flush_start) },
	{ 0x5a921311, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0xc7bb8865, __VMLINUX_SYMBOL_STR(metadata_lba_lock_table_size) },
	{ 0x8e7aa6b1, __VMLINUX_SYMBOL_STR(dz_free_data_pages) },
	{ 0x6f83a03a, __VMLINUX_SYMBOL_STR(print_object_list) },
	{ 0x7378847c, __VMLINUX_SYMBOL_STR(dz_switch_passthrough_mode) },
	{ 0x35120d42, __VMLINUX_SYMBOL_STR(tot_application_iocount_reads_partial_page) },
	{ 0x3a51335a, __VMLINUX_SYMBOL_STR(dz_switch_alignment_engine_partial_block_read) },
	{ 0x4b45f33b, __VMLINUX_SYMBOL_STR(fec_memory_reads_count) },
	{ 0x887e554f, __VMLINUX_SYMBOL_STR(superblock) },
	{ 0xcdf0bcb1, __VMLINUX_SYMBOL_STR(dz_write_data) },
	{ 0x92710dda, __VMLINUX_SYMBOL_STR(number_of_longs_for_gpba) },
	{ 0x36513a3, __VMLINUX_SYMBOL_STR(bec_cache_hit) },
	{ 0xa5f51a58, __VMLINUX_SYMBOL_STR(print_lba_block) },
	{ 0x7cd8d75e, __VMLINUX_SYMBOL_STR(page_offset_base) },
	{ 0x4a023151, __VMLINUX_SYMBOL_STR(tot_application_iocount_writes_single_page) },
	{ 0x9beb970, __VMLINUX_SYMBOL_STR(align_iocount_writes_single_unaligned_page) },
	{ 0x4cd73b7e, __VMLINUX_SYMBOL_STR(bec_cutoff_nodes) },
	{ 0xc9023e9d, __VMLINUX_SYMBOL_STR(align_iocount_disk_read) },
	{ 0xe81152c2, __VMLINUX_SYMBOL_STR(dz_switch_passthrough_write) },
	{ 0x981a41dd, __VMLINUX_SYMBOL_STR(tot_application_iocount_writes) },
	{ 0x585cdd4, __VMLINUX_SYMBOL_STR(fec_iocount_writes_multi_aligned_page) },
	{ 0x1c6637ae, __VMLINUX_SYMBOL_STR(dz_read_data) },
	{ 0x7dc90984, __VMLINUX_SYMBOL_STR(align_iocount_disk_read_internal) },
	{ 0x8657f938, __VMLINUX_SYMBOL_STR(DATA_DISK_DEVICE_NAME) },
	{ 0x2ccfd234, __VMLINUX_SYMBOL_STR(fec_iocount_writes_multi_page) },
	{ 0xb18ac87c, __VMLINUX_SYMBOL_STR(align_iocount_disk_write) },
	{ 0xa916b694, __VMLINUX_SYMBOL_STR(strnlen) },
	{ 0xdb1a1e40, __VMLINUX_SYMBOL_STR(pba_bitmap) },
	{ 0x3508aad8, __VMLINUX_SYMBOL_STR(align_iocount_disk_write_internal) },
	{ 0x5bfd8a11, __VMLINUX_SYMBOL_STR(fec_active_count) },
	{ 0xed5d0e98, __VMLINUX_SYMBOL_STR(align_iocount_writes_single_page) },
	{ 0x3c3825b1, __VMLINUX_SYMBOL_STR(pdz_dde_counters) },
	{ 0x56de9a08, __VMLINUX_SYMBOL_STR(dz_switch_fec_engine) },
	{ 0x87084d57, __VMLINUX_SYMBOL_STR(rdedupe_thread_pool) },
	{ 0x6b2dc060, __VMLINUX_SYMBOL_STR(dump_stack) },
	{ 0xf2e52d81, __VMLINUX_SYMBOL_STR(fec_iocount_reads_multi_page) },
	{ 0x3c525f53, __VMLINUX_SYMBOL_STR(dedupe_cache_hits) },
	{ 0x54730b37, __VMLINUX_SYMBOL_STR(number_of_longs_for_pba) },
	{ 0xb12d4891, __VMLINUX_SYMBOL_STR(fec_tables_flush) },
	{ 0x7f24de73, __VMLINUX_SYMBOL_STR(jiffies_to_usecs) },
	{ 0xefa7d515, __VMLINUX_SYMBOL_STR(bec_pba_page_count) },
	{ 0xd982ed94, __VMLINUX_SYMBOL_STR(GET_MDB_ENTRY) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x3a1b069, __VMLINUX_SYMBOL_STR(global_object_list) },
	{ 0xb366f9e1, __VMLINUX_SYMBOL_STR(dz_switch_target_iopath) },
	{ 0xcbd4898c, __VMLINUX_SYMBOL_STR(fortify_panic) },
	{ 0x96710d5d, __VMLINUX_SYMBOL_STR(tot_application_iocount_reads_multi_page) },
	{ 0x56321ae2, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0xece9769a, __VMLINUX_SYMBOL_STR(dz_double_page) },
	{ 0x8f5136ef, __VMLINUX_SYMBOL_STR(hash_table_size) },
	{ 0xc042fdac, __VMLINUX_SYMBOL_STR(align_write_thread_pool) },
	{ 0x3c8dcd67, __VMLINUX_SYMBOL_STR(time_fec_write_switch) },
	{ 0xea0ec39b, __VMLINUX_SYMBOL_STR(align_iocount_reads_multi_unaligned_page) },
	{ 0xb1bb49d, __VMLINUX_SYMBOL_STR(num_align_read_threads) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xb479cc62, __VMLINUX_SYMBOL_STR(dz_switch_alignment_engine_partial_block_write) },
	{ 0xe1fbbfeb, __VMLINUX_SYMBOL_STR(fec_cur_flush_count) },
	{ 0x76be66f3, __VMLINUX_SYMBOL_STR(print_data_pages) },
	{ 0xe607e98a, __VMLINUX_SYMBOL_STR(dz_switch_bec_engine) },
	{ 0xfe94db38, __VMLINUX_SYMBOL_STR(dedupe_cur_duplicate_space) },
	{ 0xb352177e, __VMLINUX_SYMBOL_STR(find_first_bit) },
	{ 0x3270eb33, __VMLINUX_SYMBOL_STR(dz_switch_alignment_engine_multi_block_write) },
	{ 0x73b7e160, __VMLINUX_SYMBOL_STR(dz_read_data_from_block_device) },
	{ 0x924d328b, __VMLINUX_SYMBOL_STR(align_iocount_reads_partial_page) },
	{ 0x606cf726, __VMLINUX_SYMBOL_STR(fec_iocount_reads_single_aligned_page) },
	{ 0x58cc04a3, __VMLINUX_SYMBOL_STR(dedupe_cur_logical_space) },
	{ 0xe9357926, __VMLINUX_SYMBOL_STR(dz_get_device_size) },
	{ 0xbdab0f63, __VMLINUX_SYMBOL_STR(dz_bec_node_mempool_count) },
	{ 0x7a5ea59b, __VMLINUX_SYMBOL_STR(time_fec_write_start) },
	{ 0xb7d25362, __VMLINUX_SYMBOL_STR(align_iocount_reads_multi_aligned_page) },
	{ 0x86421af2, __VMLINUX_SYMBOL_STR(dedupe_tot_logical_space) },
	{ 0x9957b84f, __VMLINUX_SYMBOL_STR(fec_iocount_reads_multi_aligned_page) },
	{ 0x8b8e7a6e, __VMLINUX_SYMBOL_STR(flush_thread_pool) },
	{ 0x5ce1ff1e, __VMLINUX_SYMBOL_STR(fec_iocount_writes_single_page) },
	{ 0x259d2eb8, __VMLINUX_SYMBOL_STR(align_iocount_reads_single_aligned_page) },
	{ 0xaca46994, __VMLINUX_SYMBOL_STR(sysfs_create_file_ns) },
	{ 0x4074144a, __VMLINUX_SYMBOL_STR(align_iocount_writes_multi_aligned_page) },
	{ 0xaebd92d, __VMLINUX_SYMBOL_STR(dz_kmalloc) },
	{ 0x71341798, __VMLINUX_SYMBOL_STR(tot_application_iocount_writes_partial_page) },
	{ 0x25125d2b, __VMLINUX_SYMBOL_STR(pdz_fec_counters) },
	{ 0x10df2005, __VMLINUX_SYMBOL_STR(align_iocount_writes_single_aligned_page) },
	{ 0x23f1c30d, __VMLINUX_SYMBOL_STR(fec_iocount_reads_partial_page) },
	{ 0xa3ced8eb, __VMLINUX_SYMBOL_STR(tot_application_iocount_writes_multi_page) },
	{ 0x865e055, __VMLINUX_SYMBOL_STR(dedupe_disk_writes) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=datumsoft_zaidstor_avm_6_B_dedupe_engine_module,datumsoft_zaidstor_avm_2_B_infra_engine_module,datumsoft_zaidstor_avm_5_B_metadata_engine_module";


MODULE_INFO(srcversion, "9BF8B93760B97BCE7766BEB");
