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
	{ 0x96acd127, __VMLINUX_SYMBOL_STR(dz_dq_object) },
	{ 0xe198d8af, __VMLINUX_SYMBOL_STR(dz_avm_log_file_write) },
	{ 0xc857168c, __VMLINUX_SYMBOL_STR(dz_convert_sector_to_lba) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x6880be12, __VMLINUX_SYMBOL_STR(dm_get_device) },
	{ 0x9edbbbaa, __VMLINUX_SYMBOL_STR(reduce_memory_usage) },
	{ 0xdc3421e5, __VMLINUX_SYMBOL_STR(_RELEASE_TRANSACTION_LOCK) },
	{ 0x6d0f1f89, __VMLINUX_SYMBOL_STR(dm_table_get_mode) },
	{ 0x75afd1db, __VMLINUX_SYMBOL_STR(log_memory_usage) },
	{ 0xce935a56, __VMLINUX_SYMBOL_STR(dz_kfree) },
	{ 0x71f4636b, __VMLINUX_SYMBOL_STR(write_thread_pool) },
	{ 0x5bab484b, __VMLINUX_SYMBOL_STR(_ACQUIRE_PBA_LOCK) },
	{ 0xa3e0a3f3, __VMLINUX_SYMBOL_STR(read_thread_pool) },
	{ 0xb686300d, __VMLINUX_SYMBOL_STR(_RELEASE_LBA_LOCK_WRITE) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0xd7f8efd, __VMLINUX_SYMBOL_STR(dm_register_target) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0x56bc3d41, __VMLINUX_SYMBOL_STR(_DZ_DDE_RELEASE_HBA_LOCK) },
	{ 0x2656c8f7, __VMLINUX_SYMBOL_STR(dz_find_object) },
	{ 0x45fd611e, __VMLINUX_SYMBOL_STR(dz_target_create) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x9b98c6c5, __VMLINUX_SYMBOL_STR(aisa_device_object) },
	{ 0x960f1a52, __VMLINUX_SYMBOL_STR(dz_put_lba_index) },
	{ 0x1aa6f7ab, __VMLINUX_SYMBOL_STR(GET_PBA_ENTRY) },
	{ 0xeeb40040, __VMLINUX_SYMBOL_STR(GET_LBA_ENTRY) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x5e795dad, __VMLINUX_SYMBOL_STR(print_cli_args) },
	{ 0x74acfbdf, __VMLINUX_SYMBOL_STR(dm_unregister_target) },
	{ 0xd6e2c036, __VMLINUX_SYMBOL_STR(tot_free_space) },
	{ 0x9166fada, __VMLINUX_SYMBOL_STR(strncpy) },
	{ 0x659f6d63, __VMLINUX_SYMBOL_STR(dz_demo_passthrough_io) },
	{ 0xc30c0e34, __VMLINUX_SYMBOL_STR(_ACQUIRE_TRANSACTION_LOCK) },
	{ 0x7378847c, __VMLINUX_SYMBOL_STR(dz_switch_passthrough_mode) },
	{ 0xc4455f86, __VMLINUX_SYMBOL_STR(dz_get_free_lba_index) },
	{ 0xc25419c9, __VMLINUX_SYMBOL_STR(bio_endio) },
	{ 0x5635a60a, __VMLINUX_SYMBOL_STR(vmalloc_user) },
	{ 0x6b2dc060, __VMLINUX_SYMBOL_STR(dump_stack) },
	{ 0xf5ef1c13, __VMLINUX_SYMBOL_STR(_ACQUIRE_HBA_LOCK) },
	{ 0xa426266d, __VMLINUX_SYMBOL_STR(GET_HBA_ENTRY) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x3a1b069, __VMLINUX_SYMBOL_STR(global_object_list) },
	{ 0x2a8a4715, __VMLINUX_SYMBOL_STR(dz_q_object) },
	{ 0x6bb4641f, __VMLINUX_SYMBOL_STR(_ACQUIRE_LBA_LOCK_WRITE) },
	{ 0x5c6d5076, __VMLINUX_SYMBOL_STR(dz_io_alloc) },
	{ 0x3f8ce405, __VMLINUX_SYMBOL_STR(dz_q_iorequest) },
	{ 0x2b000831, __VMLINUX_SYMBOL_STR(_DZ_DDE_RELEASE_PBA_LOCK) },
	{ 0x4e316e14, __VMLINUX_SYMBOL_STR(dz_target_destroy) },
	{ 0xd45c1417, __VMLINUX_SYMBOL_STR(dm_put_device) },
	{ 0xaebd92d, __VMLINUX_SYMBOL_STR(dz_kmalloc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=datumsoft_zaidstor_avm_2_B_infra_engine_module,datumsoft_zaidstor_avm_9_B_target_engine_module,datumsoft_zaidstor_avm_6_B_dedupe_engine_module";


MODULE_INFO(srcversion, "1F9980B656B07EBCF301EF0");
