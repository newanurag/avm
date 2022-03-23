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
	{ 0xe3c64865, __VMLINUX_SYMBOL_STR(num_flush_threads) },
	{ 0xafa0f9d5, __VMLINUX_SYMBOL_STR(fec_iocount_reads_single_unaligned_page) },
	{ 0x206c61d2, __VMLINUX_SYMBOL_STR(_ACQUIRE_FEC_LOCK_READ) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x36286558, __VMLINUX_SYMBOL_STR(dz_read_page_alloc) },
	{ 0xe73492e2, __VMLINUX_SYMBOL_STR(fec_iocount_reads_single_page) },
	{ 0x86bc7368, __VMLINUX_SYMBOL_STR(fec_active_count_sequence) },
	{ 0x672edad8, __VMLINUX_SYMBOL_STR(pv_lock_ops) },
	{ 0xe198d8af, __VMLINUX_SYMBOL_STR(dz_avm_log_file_write) },
	{ 0xc857168c, __VMLINUX_SYMBOL_STR(dz_convert_sector_to_lba) },
	{ 0x5e1f6b50, __VMLINUX_SYMBOL_STR(dz_bio_alloc) },
	{ 0x9edbbbaa, __VMLINUX_SYMBOL_STR(reduce_memory_usage) },
	{ 0x72d6f9f, __VMLINUX_SYMBOL_STR(fec_iocount_writes_single_aligned_page) },
	{ 0x44ba2a60, __VMLINUX_SYMBOL_STR(dz_thread_exit) },
	{ 0x2876aabb, __VMLINUX_SYMBOL_STR(fec_iocount_reads_multi_tail_unaligned_page) },
	{ 0x95d24c89, __VMLINUX_SYMBOL_STR(fec_flush_ops_count) },
	{ 0x9741a857, __VMLINUX_SYMBOL_STR(fec_iocount_writes_partial_page) },
	{ 0x2a3f0cdf, __VMLINUX_SYMBOL_STR(time_flush_prepare_start) },
	{ 0xc3896daa, __VMLINUX_SYMBOL_STR(READ_THREAD_POOL_SHORT_NAME) },
	{ 0x75afd1db, __VMLINUX_SYMBOL_STR(log_memory_usage) },
	{ 0x71446fbf, __VMLINUX_SYMBOL_STR(fec_free_count_sequence) },
	{ 0xc3d2c1fd, __VMLINUX_SYMBOL_STR(dz_memory_reduction) },
	{ 0xce935a56, __VMLINUX_SYMBOL_STR(dz_kfree) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0xa3e0a3f3, __VMLINUX_SYMBOL_STR(read_thread_pool) },
	{ 0xbfeed88e, __VMLINUX_SYMBOL_STR(fec_active_threshold_perc) },
	{ 0x72d95cce, __VMLINUX_SYMBOL_STR(WRITE_THREAD_POOL_SHORT_NAME) },
	{ 0xc5a657da, __VMLINUX_SYMBOL_STR(fec_cur_flush_ops_count) },
	{ 0x9afff994, __VMLINUX_SYMBOL_STR(WRITE_THREAD_POOL_NAME) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0xb7a6abe, __VMLINUX_SYMBOL_STR(print_iorequest_lock) },
	{ 0x97651e6c, __VMLINUX_SYMBOL_STR(vmemmap_base) },
	{ 0xd46119a2, __VMLINUX_SYMBOL_STR(_RELEASE_FEC_LOCK_WRITE) },
	{ 0xc4b19d63, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x15ba50a6, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0x9534db0, __VMLINUX_SYMBOL_STR(_ACQUIRE_FEC_LOCK_WRITE) },
	{ 0xf2ea068c, __VMLINUX_SYMBOL_STR(time_flush_end) },
	{ 0xde9d2aa6, __VMLINUX_SYMBOL_STR(fec_forwarded_reads_count) },
	{ 0xa6682fdd, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xb1904934, __VMLINUX_SYMBOL_STR(wait_for_completion) },
	{ 0xdf101094, __VMLINUX_SYMBOL_STR(dedupe_thread_pool) },
	{ 0x64739013, __VMLINUX_SYMBOL_STR(fec_iocount_writes_single_unaligned_page) },
	{ 0x8df65063, __VMLINUX_SYMBOL_STR(fec_table_size) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xf2a2c67d, __VMLINUX_SYMBOL_STR(dz_destroy_thread_pool) },
	{ 0x4f26c6b1, __VMLINUX_SYMBOL_STR(fec_iocount_writes_multi_unaligned_page) },
	{ 0xeeb40040, __VMLINUX_SYMBOL_STR(GET_LBA_ENTRY) },
	{ 0x4ff67fab, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb5275728, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0xfdfc8c01, __VMLINUX_SYMBOL_STR(fec_iocount_reads_multi_unaligned_page) },
	{ 0x42f23226, __VMLINUX_SYMBOL_STR(bio_add_page) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0x99645325, __VMLINUX_SYMBOL_STR(dz_dedupe_read_io) },
	{ 0xc83b8a40, __VMLINUX_SYMBOL_STR(FLUSH_THREAD_POOL_NAME) },
	{ 0xc35666f5, __VMLINUX_SYMBOL_STR(fec_tables_active) },
	{ 0x4b45f33b, __VMLINUX_SYMBOL_STR(fec_memory_reads_count) },
	{ 0xc25419c9, __VMLINUX_SYMBOL_STR(bio_endio) },
	{ 0xefa08cc4, __VMLINUX_SYMBOL_STR(bio_put) },
	{ 0x3d94a5af, __VMLINUX_SYMBOL_STR(fec_flush_count) },
	{ 0xf11543ff, __VMLINUX_SYMBOL_STR(find_first_zero_bit) },
	{ 0x7cd8d75e, __VMLINUX_SYMBOL_STR(page_offset_base) },
	{ 0xd24da7de, __VMLINUX_SYMBOL_STR(dz_get_memory_usage) },
	{ 0xd8944221, __VMLINUX_SYMBOL_STR(num_read_threads) },
	{ 0x133f6a9c, __VMLINUX_SYMBOL_STR(print_iorequest) },
	{ 0x585cdd4, __VMLINUX_SYMBOL_STR(fec_iocount_writes_multi_aligned_page) },
	{ 0x1d1dcfee, __VMLINUX_SYMBOL_STR(dz_memory_consumption) },
	{ 0x2ccfd234, __VMLINUX_SYMBOL_STR(fec_iocount_writes_multi_page) },
	{ 0x4a9865a8, __VMLINUX_SYMBOL_STR(dz_create_thread_pool) },
	{ 0x834d015b, __VMLINUX_SYMBOL_STR(num_write_threads) },
	{ 0xf4f37145, __VMLINUX_SYMBOL_STR(dz_read_page_free) },
	{ 0x5bfd8a11, __VMLINUX_SYMBOL_STR(fec_active_count) },
	{ 0xbcc8b4cd, __VMLINUX_SYMBOL_STR(fec_active_threshold) },
	{ 0xc028c237, __VMLINUX_SYMBOL_STR(fec_read_thread_pool) },
	{ 0x3fc45e93, __VMLINUX_SYMBOL_STR(dz_q_child_iorequest_to_parent_q) },
	{ 0x5635a60a, __VMLINUX_SYMBOL_STR(vmalloc_user) },
	{ 0x56de9a08, __VMLINUX_SYMBOL_STR(dz_switch_fec_engine) },
	{ 0x8ddd8aad, __VMLINUX_SYMBOL_STR(schedule_timeout) },
	{ 0x1c0529b1, __VMLINUX_SYMBOL_STR(dz_io_free) },
	{ 0xb7a0a67e, __VMLINUX_SYMBOL_STR(print_biom) },
	{ 0x1f44babd, __VMLINUX_SYMBOL_STR(_RELEASE_FEC_LOCK_READ) },
	{ 0x6b2dc060, __VMLINUX_SYMBOL_STR(dump_stack) },
	{ 0xf2e52d81, __VMLINUX_SYMBOL_STR(fec_iocount_reads_multi_page) },
	{ 0xb12d4891, __VMLINUX_SYMBOL_STR(fec_tables_flush) },
	{ 0x70ffc26b, __VMLINUX_SYMBOL_STR(dz_bec_proceed_pba_page_add_write) },
	{ 0x13bb1b43, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0x8e8a34c0, __VMLINUX_SYMBOL_STR(dz_q_iorequest_thread_pool) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x56321ae2, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0xb3f7646e, __VMLINUX_SYMBOL_STR(kthread_should_stop) },
	{ 0x5c6d5076, __VMLINUX_SYMBOL_STR(dz_io_alloc) },
	{ 0x17e4b4a6, __VMLINUX_SYMBOL_STR(fec_flush_thread_pool) },
	{ 0x3f8ce405, __VMLINUX_SYMBOL_STR(dz_q_iorequest) },
	{ 0x3c8dcd67, __VMLINUX_SYMBOL_STR(time_fec_write_switch) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xbc1fd179, __VMLINUX_SYMBOL_STR(print_bio) },
	{ 0xe1fbbfeb, __VMLINUX_SYMBOL_STR(fec_cur_flush_count) },
	{ 0xd3131b67, __VMLINUX_SYMBOL_STR(FLUSH_THREAD_POOL_SHORT_NAME) },
	{ 0xed9eada3, __VMLINUX_SYMBOL_STR(fec_write_thread_pool) },
	{ 0x74b99461, __VMLINUX_SYMBOL_STR(iorequest_get) },
	{ 0xfe50d590, __VMLINUX_SYMBOL_STR(dz_convert_lba_to_sector) },
	{ 0x606cf726, __VMLINUX_SYMBOL_STR(fec_iocount_reads_single_aligned_page) },
	{ 0x19cf472b, __VMLINUX_SYMBOL_STR(complete) },
	{ 0x8562547a, __VMLINUX_SYMBOL_STR(bdget) },
	{ 0x7a5ea59b, __VMLINUX_SYMBOL_STR(time_fec_write_start) },
	{ 0xe13924c9, __VMLINUX_SYMBOL_STR(dz_init_child_bio) },
	{ 0x9957b84f, __VMLINUX_SYMBOL_STR(fec_iocount_reads_multi_aligned_page) },
	{ 0x8b8e7a6e, __VMLINUX_SYMBOL_STR(flush_thread_pool) },
	{ 0x5ce1ff1e, __VMLINUX_SYMBOL_STR(fec_iocount_writes_single_page) },
	{ 0x7f02188f, __VMLINUX_SYMBOL_STR(__msecs_to_jiffies) },
	{ 0x775a6ce6, __VMLINUX_SYMBOL_STR(iorequest_put) },
	{ 0x5a656236, __VMLINUX_SYMBOL_STR(READ_THREAD_POOL_NAME) },
	{ 0xaebd92d, __VMLINUX_SYMBOL_STR(dz_kmalloc) },
	{ 0x25125d2b, __VMLINUX_SYMBOL_STR(pdz_fec_counters) },
	{ 0x2da3b904, __VMLINUX_SYMBOL_STR(print_data) },
	{ 0x76ee3ee1, __VMLINUX_SYMBOL_STR(fec_iocount_reads_multi_both_unaligned_page) },
	{ 0x3afd005, __VMLINUX_SYMBOL_STR(fec_iocount_reads_multi_head_unaligned_page) },
	{ 0x23f1c30d, __VMLINUX_SYMBOL_STR(fec_iocount_reads_partial_page) },
	{ 0x6f887134, __VMLINUX_SYMBOL_STR(dz_merge_parent_local_q_to_threadpool_q) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=datumsoft_zaidstor_avm_2_B_infra_engine_module,datumsoft_zaidstor_avm_6_B_dedupe_engine_module,datumsoft_zaidstor_avm_4_B_bec_engine_module";


MODULE_INFO(srcversion, "DF77B9049922A7ADBDCE5F5");
