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
	{ 0xe0a4b68a, __VMLINUX_SYMBOL_STR(dz_switch_alignment_engine_multi_block_read) },
	{ 0x1e9740c8, __VMLINUX_SYMBOL_STR(align_read_thread_pool) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0xbf6d71c0, __VMLINUX_SYMBOL_STR(align_iocount_reads_single_unaligned_page) },
	{ 0x36286558, __VMLINUX_SYMBOL_STR(dz_read_page_alloc) },
	{ 0xe198d8af, __VMLINUX_SYMBOL_STR(dz_avm_log_file_write) },
	{ 0xc857168c, __VMLINUX_SYMBOL_STR(dz_convert_sector_to_lba) },
	{ 0x5e1f6b50, __VMLINUX_SYMBOL_STR(dz_bio_alloc) },
	{ 0xc374fe49, __VMLINUX_SYMBOL_STR(align_iocount_reads_multi_tail_unaligned_page) },
	{ 0x5feb4ea4, __VMLINUX_SYMBOL_STR(align_iocount_writes_multi_unaligned_page) },
	{ 0x571b2dff, __VMLINUX_SYMBOL_STR(align_iocount_reads_multi_page) },
	{ 0xc3d2c1fd, __VMLINUX_SYMBOL_STR(dz_memory_reduction) },
	{ 0x9bd7700f, __VMLINUX_SYMBOL_STR(align_iocount_writes_multi_page) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0xd162e5d, __VMLINUX_SYMBOL_STR(dz_bec_page_free) },
	{ 0xc57904d4, __VMLINUX_SYMBOL_STR(dz_switch_dedupe_engine) },
	{ 0xa28b816f, __VMLINUX_SYMBOL_STR(ALIGN_WRITE_THREAD_POOL_SHORT_NAME) },
	{ 0x502c30d9, __VMLINUX_SYMBOL_STR(align_iocount_reads_single_page) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0x97651e6c, __VMLINUX_SYMBOL_STR(vmemmap_base) },
	{ 0xd46119a2, __VMLINUX_SYMBOL_STR(_RELEASE_FEC_LOCK_WRITE) },
	{ 0x932b32b3, __VMLINUX_SYMBOL_STR(align_iocount_writes_partial_page) },
	{ 0x9534db0, __VMLINUX_SYMBOL_STR(_ACQUIRE_FEC_LOCK_WRITE) },
	{ 0x9dec6a13, __VMLINUX_SYMBOL_STR(align_iocount_reads_multi_both_unaligned_page) },
	{ 0xa6682fdd, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xb1904934, __VMLINUX_SYMBOL_STR(wait_for_completion) },
	{ 0xdf101094, __VMLINUX_SYMBOL_STR(dedupe_thread_pool) },
	{ 0x4149710a, __VMLINUX_SYMBOL_STR(num_align_write_threads) },
	{ 0xf2a2c67d, __VMLINUX_SYMBOL_STR(dz_destroy_thread_pool) },
	{ 0x5d739c59, __VMLINUX_SYMBOL_STR(pdz_align_counters) },
	{ 0xeeb40040, __VMLINUX_SYMBOL_STR(GET_LBA_ENTRY) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x42f23226, __VMLINUX_SYMBOL_STR(bio_add_page) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0xd3db4fe9, __VMLINUX_SYMBOL_STR(generic_make_request) },
	{ 0x3a51335a, __VMLINUX_SYMBOL_STR(dz_switch_alignment_engine_partial_block_read) },
	{ 0xc25419c9, __VMLINUX_SYMBOL_STR(bio_endio) },
	{ 0xefa08cc4, __VMLINUX_SYMBOL_STR(bio_put) },
	{ 0x7cd8d75e, __VMLINUX_SYMBOL_STR(page_offset_base) },
	{ 0xd24da7de, __VMLINUX_SYMBOL_STR(dz_get_memory_usage) },
	{ 0x9beb970, __VMLINUX_SYMBOL_STR(align_iocount_writes_single_unaligned_page) },
	{ 0xc9023e9d, __VMLINUX_SYMBOL_STR(align_iocount_disk_read) },
	{ 0x1d1dcfee, __VMLINUX_SYMBOL_STR(dz_memory_consumption) },
	{ 0x7dc90984, __VMLINUX_SYMBOL_STR(align_iocount_disk_read_internal) },
	{ 0xb18ac87c, __VMLINUX_SYMBOL_STR(align_iocount_disk_write) },
	{ 0x4a9865a8, __VMLINUX_SYMBOL_STR(dz_create_thread_pool) },
	{ 0x3508aad8, __VMLINUX_SYMBOL_STR(align_iocount_disk_write_internal) },
	{ 0xf4f37145, __VMLINUX_SYMBOL_STR(dz_read_page_free) },
	{ 0xed5d0e98, __VMLINUX_SYMBOL_STR(align_iocount_writes_single_page) },
	{ 0x3fc45e93, __VMLINUX_SYMBOL_STR(dz_q_child_iorequest_to_parent_q) },
	{ 0x56de9a08, __VMLINUX_SYMBOL_STR(dz_switch_fec_engine) },
	{ 0xde97063d, __VMLINUX_SYMBOL_STR(dz_bec_page_alloc) },
	{ 0x1c0529b1, __VMLINUX_SYMBOL_STR(dz_io_free) },
	{ 0xb7a0a67e, __VMLINUX_SYMBOL_STR(print_biom) },
	{ 0x6b2dc060, __VMLINUX_SYMBOL_STR(dump_stack) },
	{ 0xdc8bfb8f, __VMLINUX_SYMBOL_STR(ALIGN_READ_THREAD_POOL_NAME) },
	{ 0x8e8a34c0, __VMLINUX_SYMBOL_STR(dz_q_iorequest_thread_pool) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xc042fdac, __VMLINUX_SYMBOL_STR(align_write_thread_pool) },
	{ 0xc238146f, __VMLINUX_SYMBOL_STR(dz_bec_pba_page_lookup) },
	{ 0x5c6d5076, __VMLINUX_SYMBOL_STR(dz_io_alloc) },
	{ 0x3f8ce405, __VMLINUX_SYMBOL_STR(dz_q_iorequest) },
	{ 0xea0ec39b, __VMLINUX_SYMBOL_STR(align_iocount_reads_multi_unaligned_page) },
	{ 0xb1bb49d, __VMLINUX_SYMBOL_STR(num_align_read_threads) },
	{ 0xe8ad84f7, __VMLINUX_SYMBOL_STR(align_iocount_reads_multi_head_unaligned_page) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xbc1fd179, __VMLINUX_SYMBOL_STR(print_bio) },
	{ 0xe607e98a, __VMLINUX_SYMBOL_STR(dz_switch_bec_engine) },
	{ 0x2ac56908, __VMLINUX_SYMBOL_STR(DATA_DISK_DEVICE) },
	{ 0xf1423b4f, __VMLINUX_SYMBOL_STR(ALIGN_READ_THREAD_POOL_SHORT_NAME) },
	{ 0x74b99461, __VMLINUX_SYMBOL_STR(iorequest_get) },
	{ 0xfe50d590, __VMLINUX_SYMBOL_STR(dz_convert_lba_to_sector) },
	{ 0x924d328b, __VMLINUX_SYMBOL_STR(align_iocount_reads_partial_page) },
	{ 0x19cf472b, __VMLINUX_SYMBOL_STR(complete) },
	{ 0x8562547a, __VMLINUX_SYMBOL_STR(bdget) },
	{ 0xb7d25362, __VMLINUX_SYMBOL_STR(align_iocount_reads_multi_aligned_page) },
	{ 0xe13924c9, __VMLINUX_SYMBOL_STR(dz_init_child_bio) },
	{ 0x259d2eb8, __VMLINUX_SYMBOL_STR(align_iocount_reads_single_aligned_page) },
	{ 0x775a6ce6, __VMLINUX_SYMBOL_STR(iorequest_put) },
	{ 0x4074144a, __VMLINUX_SYMBOL_STR(align_iocount_writes_multi_aligned_page) },
	{ 0x10df2005, __VMLINUX_SYMBOL_STR(align_iocount_writes_single_aligned_page) },
	{ 0x72aa95db, __VMLINUX_SYMBOL_STR(dz_read_biodone_sync) },
	{ 0x6f887134, __VMLINUX_SYMBOL_STR(dz_merge_parent_local_q_to_threadpool_q) },
	{ 0x28c41c25, __VMLINUX_SYMBOL_STR(ALIGN_WRITE_THREAD_POOL_NAME) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=datumsoft_zaidstor_avm_2_B_infra_engine_module,datumsoft_zaidstor_avm_4_B_bec_engine_module,datumsoft_zaidstor_avm_6_B_dedupe_engine_module";


MODULE_INFO(srcversion, "50F7CB2D4621EC19F7AD749");
