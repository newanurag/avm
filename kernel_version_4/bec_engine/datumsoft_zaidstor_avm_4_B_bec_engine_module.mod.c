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
	{ 0x53326531, __VMLINUX_SYMBOL_STR(mempool_alloc_pages) },
	{ 0x94d683f, __VMLINUX_SYMBOL_STR(kmem_cache_destroy) },
	{ 0xa9abbfb6, __VMLINUX_SYMBOL_STR(bec_cutoff_percent) },
	{ 0xe198d8af, __VMLINUX_SYMBOL_STR(dz_avm_log_file_write) },
	{ 0xa598d2fb, __VMLINUX_SYMBOL_STR(pdz_bec_counters) },
	{ 0x9edbbbaa, __VMLINUX_SYMBOL_STR(reduce_memory_usage) },
	{ 0x382ddbd1, __VMLINUX_SYMBOL_STR(num_bec_threads) },
	{ 0x75afd1db, __VMLINUX_SYMBOL_STR(log_memory_usage) },
	{ 0xc3d2c1fd, __VMLINUX_SYMBOL_STR(dz_memory_reduction) },
	{ 0x4c7a8fae, __VMLINUX_SYMBOL_STR(mempool_destroy) },
	{ 0xce935a56, __VMLINUX_SYMBOL_STR(dz_kfree) },
	{ 0x8631ddaf, __VMLINUX_SYMBOL_STR(_ACQUIRE_BEC_LOCK_WRITE) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0x97651e6c, __VMLINUX_SYMBOL_STR(vmemmap_base) },
	{ 0x32eac50b, __VMLINUX_SYMBOL_STR(bec_radix_tree) },
	{ 0x183fa88b, __VMLINUX_SYMBOL_STR(mempool_alloc_slab) },
	{ 0x26313fef, __VMLINUX_SYMBOL_STR(bec_thread_pool) },
	{ 0xf2a2c67d, __VMLINUX_SYMBOL_STR(dz_destroy_thread_pool) },
	{ 0x446c4f88, __VMLINUX_SYMBOL_STR(bec_cache_delete) },
	{ 0x4ff67fab, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0xe9eb9c31, __VMLINUX_SYMBOL_STR(bec_size) },
	{ 0xfa89cf66, __VMLINUX_SYMBOL_STR(bec_cache_miss) },
	{ 0xd985dc99, __VMLINUX_SYMBOL_STR(mempool_free_pages) },
	{ 0x8a99a016, __VMLINUX_SYMBOL_STR(mempool_free_slab) },
	{ 0xc25419c9, __VMLINUX_SYMBOL_STR(bio_endio) },
	{ 0x7e880422, __VMLINUX_SYMBOL_STR(radix_tree_delete) },
	{ 0xefa08cc4, __VMLINUX_SYMBOL_STR(bio_put) },
	{ 0x36513a3, __VMLINUX_SYMBOL_STR(bec_cache_hit) },
	{ 0x7cd8d75e, __VMLINUX_SYMBOL_STR(page_offset_base) },
	{ 0xd24da7de, __VMLINUX_SYMBOL_STR(dz_get_memory_usage) },
	{ 0xc25c3c0d, __VMLINUX_SYMBOL_STR(bec_list_head) },
	{ 0x4cd73b7e, __VMLINUX_SYMBOL_STR(bec_cutoff_nodes) },
	{ 0x1d1dcfee, __VMLINUX_SYMBOL_STR(dz_memory_consumption) },
	{ 0x9a6f2471, __VMLINUX_SYMBOL_STR(mempool_alloc) },
	{ 0x4a9865a8, __VMLINUX_SYMBOL_STR(dz_create_thread_pool) },
	{ 0x519b2bd6, __VMLINUX_SYMBOL_STR(BEC_THREAD_POOL_SHORT_NAME) },
	{ 0x56de9a08, __VMLINUX_SYMBOL_STR(dz_switch_fec_engine) },
	{ 0x4d834179, __VMLINUX_SYMBOL_STR(BEC_THREAD_POOL_NAME) },
	{ 0x696727a5, __VMLINUX_SYMBOL_STR(mempool_create) },
	{ 0x6b2dc060, __VMLINUX_SYMBOL_STR(dump_stack) },
	{ 0xefa7d515, __VMLINUX_SYMBOL_STR(bec_pba_page_count) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xec018b66, __VMLINUX_SYMBOL_STR(__radix_tree_insert) },
	{ 0xe5c6ae21, __VMLINUX_SYMBOL_STR(mempool_free) },
	{ 0x92f41a66, __VMLINUX_SYMBOL_STR(_RELEASE_BEC_LOCK_WRITE) },
	{ 0x961d8335, __VMLINUX_SYMBOL_STR(kmem_cache_create) },
	{ 0x3f8ce405, __VMLINUX_SYMBOL_STR(dz_q_iorequest) },
	{ 0x9754ec10, __VMLINUX_SYMBOL_STR(radix_tree_preload) },
	{ 0xe607e98a, __VMLINUX_SYMBOL_STR(dz_switch_bec_engine) },
	{ 0x92a6f160, __VMLINUX_SYMBOL_STR(radix_tree_lookup) },
	{ 0x74b99461, __VMLINUX_SYMBOL_STR(iorequest_get) },
	{ 0xbdab0f63, __VMLINUX_SYMBOL_STR(dz_bec_node_mempool_count) },
	{ 0x64865f89, __VMLINUX_SYMBOL_STR(_ACQUIRE_BEC_LOCK_READ) },
	{ 0x775a6ce6, __VMLINUX_SYMBOL_STR(iorequest_put) },
	{ 0xaebd92d, __VMLINUX_SYMBOL_STR(dz_kmalloc) },
	{ 0xa01dd995, __VMLINUX_SYMBOL_STR(_RELEASE_BEC_LOCK_READ) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=datumsoft_zaidstor_avm_2_B_infra_engine_module";


MODULE_INFO(srcversion, "DC7EF8799CEEA2F459EC778");
