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
	{ 0x60d37c1, __VMLINUX_SYMBOL_STR(dz_fec_engine_exit) },
	{ 0x822efedb, __VMLINUX_SYMBOL_STR(avm_metadata_bdev) },
	{ 0x1e9740c8, __VMLINUX_SYMBOL_STR(align_read_thread_pool) },
	{ 0x96acd127, __VMLINUX_SYMBOL_STR(dz_dq_object) },
	{ 0x6487676d, __VMLINUX_SYMBOL_STR(dz_check_memory_leak) },
	{ 0xe198d8af, __VMLINUX_SYMBOL_STR(dz_avm_log_file_write) },
	{ 0xb89c7178, __VMLINUX_SYMBOL_STR(dz_switch_fake_read) },
	{ 0x726ef563, __VMLINUX_SYMBOL_STR(tot_application_iocount_reads) },
	{ 0xeab6af1b, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0xc857168c, __VMLINUX_SYMBOL_STR(dz_convert_sector_to_lba) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x5445c211, __VMLINUX_SYMBOL_STR(dz_avm_counters) },
	{ 0x6880be12, __VMLINUX_SYMBOL_STR(dm_get_device) },
	{ 0xa57b5c08, __VMLINUX_SYMBOL_STR(dz_bdev_close) },
	{ 0xa7ebc8a3, __VMLINUX_SYMBOL_STR(blk_queue_max_hw_sectors) },
	{ 0xa598d2fb, __VMLINUX_SYMBOL_STR(pdz_bec_counters) },
	{ 0x7ab989c3, __VMLINUX_SYMBOL_STR(dz_total_memory_consumption) },
	{ 0x117c19e0, __VMLINUX_SYMBOL_STR(dz_fec_engine_init) },
	{ 0x6d0f1f89, __VMLINUX_SYMBOL_STR(dm_table_get_mode) },
	{ 0x9a3d333d, __VMLINUX_SYMBOL_STR(TARGET_DEVICE_SIZE) },
	{ 0x43e9f918, __VMLINUX_SYMBOL_STR(METADATA_DISK_DEVICE) },
	{ 0x4cbcd7ff, __VMLINUX_SYMBOL_STR(dz_bec_engine_init) },
	{ 0x7ae031fd, __VMLINUX_SYMBOL_STR(dz_bdev_open) },
	{ 0xaeeff187, __VMLINUX_SYMBOL_STR(dedupe_hash_table) },
	{ 0xce935a56, __VMLINUX_SYMBOL_STR(dz_kfree) },
	{ 0x71f4636b, __VMLINUX_SYMBOL_STR(write_thread_pool) },
	{ 0xe1a597e1, __VMLINUX_SYMBOL_STR(dz_switch_passthrough_read) },
	{ 0x7d1f704e, __VMLINUX_SYMBOL_STR(dz_fec_counters) },
	{ 0xa3e0a3f3, __VMLINUX_SYMBOL_STR(read_thread_pool) },
	{ 0xd7f8efd, __VMLINUX_SYMBOL_STR(dm_register_target) },
	{ 0xab5923e8, __VMLINUX_SYMBOL_STR(dz_dedupe_engine_init) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0x7beb76ca, __VMLINUX_SYMBOL_STR(pdz_basic_counters) },
	{ 0x6835983d, __VMLINUX_SYMBOL_STR(tot_application_iocount_reads_single_page) },
	{ 0xa23b345f, __VMLINUX_SYMBOL_STR(dz_metadata_engine_exit) },
	{ 0x9ba0175, __VMLINUX_SYMBOL_STR(metadata_lba_lock_table) },
	{ 0x9b98c6c5, __VMLINUX_SYMBOL_STR(aisa_device_object) },
	{ 0x5d739c59, __VMLINUX_SYMBOL_STR(pdz_align_counters) },
	{ 0x455dd896, __VMLINUX_SYMBOL_STR(tot_application_iocount) },
	{ 0x14a5b2a0, __VMLINUX_SYMBOL_STR(dz_infra_engine_exit) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x5e795dad, __VMLINUX_SYMBOL_STR(print_cli_args) },
	{ 0x449ad0a7, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0x48b52b5d, __VMLINUX_SYMBOL_STR(dedupe_pba_table) },
	{ 0x74acfbdf, __VMLINUX_SYMBOL_STR(dm_unregister_target) },
	{ 0xee6eeeab, __VMLINUX_SYMBOL_STR(dz_fec_init_counters) },
	{ 0x7089ad04, __VMLINUX_SYMBOL_STR(dedupe_lba_table) },
	{ 0xd6e2c036, __VMLINUX_SYMBOL_STR(tot_free_space) },
	{ 0x9166fada, __VMLINUX_SYMBOL_STR(strncpy) },
	{ 0x95f2fe16, __VMLINUX_SYMBOL_STR(dz_infra_engine_init) },
	{ 0x7325e05b, __VMLINUX_SYMBOL_STR(dz_align_engine_init) },
	{ 0xfe4d1361, __VMLINUX_SYMBOL_STR(dz_align_counters) },
	{ 0x7378847c, __VMLINUX_SYMBOL_STR(dz_switch_passthrough_mode) },
	{ 0x35120d42, __VMLINUX_SYMBOL_STR(tot_application_iocount_reads_partial_page) },
	{ 0x887e554f, __VMLINUX_SYMBOL_STR(superblock) },
	{ 0xc25419c9, __VMLINUX_SYMBOL_STR(bio_endio) },
	{ 0x56c0b315, __VMLINUX_SYMBOL_STR(dz_dedupe_engine_exit) },
	{ 0x61d021bc, __VMLINUX_SYMBOL_STR(dz_bec_counters) },
	{ 0x4a023151, __VMLINUX_SYMBOL_STR(tot_application_iocount_writes_single_page) },
	{ 0xe81152c2, __VMLINUX_SYMBOL_STR(dz_switch_passthrough_write) },
	{ 0x981a41dd, __VMLINUX_SYMBOL_STR(tot_application_iocount_writes) },
	{ 0x8657f938, __VMLINUX_SYMBOL_STR(DATA_DISK_DEVICE_NAME) },
	{ 0x69bf6b1c, __VMLINUX_SYMBOL_STR(bdevname) },
	{ 0xc028c237, __VMLINUX_SYMBOL_STR(fec_read_thread_pool) },
	{ 0x3c3825b1, __VMLINUX_SYMBOL_STR(pdz_dde_counters) },
	{ 0x56de9a08, __VMLINUX_SYMBOL_STR(dz_switch_fec_engine) },
	{ 0x6b2dc060, __VMLINUX_SYMBOL_STR(dump_stack) },
	{ 0x7baee8cb, __VMLINUX_SYMBOL_STR(dz_metadata_engine_init) },
	{ 0xbfc8b005, __VMLINUX_SYMBOL_STR(pdz_avm_counters) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x527091dc, __VMLINUX_SYMBOL_STR(dz_dde_counters) },
	{ 0x3a1b069, __VMLINUX_SYMBOL_STR(global_object_list) },
	{ 0xcbd4898c, __VMLINUX_SYMBOL_STR(fortify_panic) },
	{ 0x96710d5d, __VMLINUX_SYMBOL_STR(tot_application_iocount_reads_multi_page) },
	{ 0x6daa3cdf, __VMLINUX_SYMBOL_STR(metadata_lba_table) },
	{ 0xc042fdac, __VMLINUX_SYMBOL_STR(align_write_thread_pool) },
	{ 0x7af3ecf9, __VMLINUX_SYMBOL_STR(dz_bec_engine_exit) },
	{ 0x2a8a4715, __VMLINUX_SYMBOL_STR(dz_q_object) },
	{ 0x5c6d5076, __VMLINUX_SYMBOL_STR(dz_io_alloc) },
	{ 0x3f8ce405, __VMLINUX_SYMBOL_STR(dz_q_iorequest) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x75453efa, __VMLINUX_SYMBOL_STR(dz_basic_counters) },
	{ 0x5aa333a5, __VMLINUX_SYMBOL_STR(METADATA_DISK_DEVICE_NAME) },
	{ 0x2ac56908, __VMLINUX_SYMBOL_STR(DATA_DISK_DEVICE) },
	{ 0xed9eada3, __VMLINUX_SYMBOL_STR(fec_write_thread_pool) },
	{ 0xd45c1417, __VMLINUX_SYMBOL_STR(dm_put_device) },
	{ 0xf272aced, __VMLINUX_SYMBOL_STR(dz_align_engine_exit) },
	{ 0x619b40c4, __VMLINUX_SYMBOL_STR(dz_switch_fake_write) },
	{ 0x86421af2, __VMLINUX_SYMBOL_STR(dedupe_tot_logical_space) },
	{ 0xaebd92d, __VMLINUX_SYMBOL_STR(dz_kmalloc) },
	{ 0x71341798, __VMLINUX_SYMBOL_STR(tot_application_iocount_writes_partial_page) },
	{ 0x25125d2b, __VMLINUX_SYMBOL_STR(pdz_fec_counters) },
	{ 0xa3ced8eb, __VMLINUX_SYMBOL_STR(tot_application_iocount_writes_multi_page) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=datumsoft_zaidstor_avm_8_B_fec_engine_module,datumsoft_zaidstor_avm_2_B_infra_engine_module,datumsoft_zaidstor_avm_4_B_bec_engine_module,datumsoft_zaidstor_avm_6_B_dedupe_engine_module,datumsoft_zaidstor_avm_5_B_metadata_engine_module,datumsoft_zaidstor_avm_7_B_alignment_engine_module";


MODULE_INFO(srcversion, "828B901CBFD138D03E0FC5A");
