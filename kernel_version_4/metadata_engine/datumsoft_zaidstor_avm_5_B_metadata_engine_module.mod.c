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
	{ 0x822efedb, __VMLINUX_SYMBOL_STR(avm_metadata_bdev) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x83f9155b, __VMLINUX_SYMBOL_STR(num_metadata_flush_threads) },
	{ 0x672edad8, __VMLINUX_SYMBOL_STR(pv_lock_ops) },
	{ 0xe198d8af, __VMLINUX_SYMBOL_STR(dz_avm_log_file_write) },
	{ 0xdc7a74c9, __VMLINUX_SYMBOL_STR(metadata_flush_thread_pool) },
	{ 0x749d81a6, __VMLINUX_SYMBOL_STR(print_metadata_block) },
	{ 0x9edbbbaa, __VMLINUX_SYMBOL_STR(reduce_memory_usage) },
	{ 0x44ba2a60, __VMLINUX_SYMBOL_STR(dz_thread_exit) },
	{ 0xef6832fe, __VMLINUX_SYMBOL_STR(lba_table_size) },
	{ 0xc4407181, __VMLINUX_SYMBOL_STR(metadata_flush_timer_stop) },
	{ 0x43e9f918, __VMLINUX_SYMBOL_STR(METADATA_DISK_DEVICE) },
	{ 0x75afd1db, __VMLINUX_SYMBOL_STR(log_memory_usage) },
	{ 0xc3d2c1fd, __VMLINUX_SYMBOL_STR(dz_memory_reduction) },
	{ 0xce935a56, __VMLINUX_SYMBOL_STR(dz_kfree) },
	{ 0xc57904d4, __VMLINUX_SYMBOL_STR(dz_switch_dedupe_engine) },
	{ 0xca1c25a1, __VMLINUX_SYMBOL_STR(METADATA_FLUSH_THREAD_POOL_NAME) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0xb175e60d, __VMLINUX_SYMBOL_STR(dz_write_page_alloc) },
	{ 0x97651e6c, __VMLINUX_SYMBOL_STR(vmemmap_base) },
	{ 0x7beb76ca, __VMLINUX_SYMBOL_STR(pdz_basic_counters) },
	{ 0xc4b19d63, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x9ba0175, __VMLINUX_SYMBOL_STR(metadata_lba_lock_table) },
	{ 0x9b98c6c5, __VMLINUX_SYMBOL_STR(aisa_device_object) },
	{ 0xf2a2c67d, __VMLINUX_SYMBOL_STR(dz_destroy_thread_pool) },
	{ 0x71a448f6, __VMLINUX_SYMBOL_STR(metadata_flush_interval) },
	{ 0x4ff67fab, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb5275728, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0x5c3c34b4, __VMLINUX_SYMBOL_STR(metadata_lba_table_size) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0xeeff0455, __VMLINUX_SYMBOL_STR(pba_table_size) },
	{ 0xc7bb8865, __VMLINUX_SYMBOL_STR(metadata_lba_lock_table_size) },
	{ 0x7378847c, __VMLINUX_SYMBOL_STR(dz_switch_passthrough_mode) },
	{ 0x887e554f, __VMLINUX_SYMBOL_STR(superblock) },
	{ 0xc21d98ee, __VMLINUX_SYMBOL_STR(METADATA_FLUSH_THREAD_POOL_SHORT_NAME) },
	{ 0x7cd8d75e, __VMLINUX_SYMBOL_STR(page_offset_base) },
	{ 0xd24da7de, __VMLINUX_SYMBOL_STR(dz_get_memory_usage) },
	{ 0x1d1dcfee, __VMLINUX_SYMBOL_STR(dz_memory_consumption) },
	{ 0x4a9865a8, __VMLINUX_SYMBOL_STR(dz_create_thread_pool) },
	{ 0xf4f37145, __VMLINUX_SYMBOL_STR(dz_read_page_free) },
	{ 0x5635a60a, __VMLINUX_SYMBOL_STR(vmalloc_user) },
	{ 0x8ddd8aad, __VMLINUX_SYMBOL_STR(schedule_timeout) },
	{ 0x6b2dc060, __VMLINUX_SYMBOL_STR(dump_stack) },
	{ 0x74da9d82, __VMLINUX_SYMBOL_STR(dz_write_page_free) },
	{ 0x13bb1b43, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x9697ba7d, __VMLINUX_SYMBOL_STR(dz_read_page_from_raw_disk_sync) },
	{ 0x56321ae2, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x3a8c9940, __VMLINUX_SYMBOL_STR(dz_write_page_to_raw_disk_sync) },
	{ 0x6daa3cdf, __VMLINUX_SYMBOL_STR(metadata_lba_table) },
	{ 0x8f5136ef, __VMLINUX_SYMBOL_STR(hash_table_size) },
	{ 0xb3f7646e, __VMLINUX_SYMBOL_STR(kthread_should_stop) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xfad4d81c, __VMLINUX_SYMBOL_STR(MurmurHash3_x64_128) },
	{ 0x2ac56908, __VMLINUX_SYMBOL_STR(DATA_DISK_DEVICE) },
	{ 0x2787fe9d, __VMLINUX_SYMBOL_STR(metadata_flush_timer_thread) },
	{ 0x7f02188f, __VMLINUX_SYMBOL_STR(__msecs_to_jiffies) },
	{ 0xaebd92d, __VMLINUX_SYMBOL_STR(dz_kmalloc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=datumsoft_zaidstor_avm_2_B_infra_engine_module";


MODULE_INFO(srcversion, "CBE5E4B63C4EB5B0B772E46");
