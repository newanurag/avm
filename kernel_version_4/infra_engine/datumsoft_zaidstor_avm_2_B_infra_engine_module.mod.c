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
	{ 0xf6f2d21, __VMLINUX_SYMBOL_STR(kernel_write) },
	{ 0x914e33d2, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x48a11710, __VMLINUX_SYMBOL_STR(blkdev_ioctl) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0xd28020dd, __VMLINUX_SYMBOL_STR(up_read) },
	{ 0x54e34ad6, __VMLINUX_SYMBOL_STR(blk_status_to_errno) },
	{ 0x6882ced7, __VMLINUX_SYMBOL_STR(bio_alloc_bioset) },
	{ 0x672edad8, __VMLINUX_SYMBOL_STR(pv_lock_ops) },
	{ 0x4c7a8fae, __VMLINUX_SYMBOL_STR(mempool_destroy) },
	{ 0x89ac392d, __VMLINUX_SYMBOL_STR(filp_close) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0xa5ede869, __VMLINUX_SYMBOL_STR(vfs_fsync) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0x97651e6c, __VMLINUX_SYMBOL_STR(vmemmap_base) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xc4b19d63, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x15ba50a6, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0xf00fcaa1, __VMLINUX_SYMBOL_STR(lookup_bdev) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0xd83bc98c, __VMLINUX_SYMBOL_STR(down_read) },
	{ 0xea9f6313, __VMLINUX_SYMBOL_STR(complete_all) },
	{ 0xa6682fdd, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0x183fa88b, __VMLINUX_SYMBOL_STR(mempool_alloc_slab) },
	{ 0xb1904934, __VMLINUX_SYMBOL_STR(wait_for_completion) },
	{ 0xa350d089, __VMLINUX_SYMBOL_STR(kernel_read) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x4ff67fab, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb5275728, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0x42f23226, __VMLINUX_SYMBOL_STR(bio_add_page) },
	{ 0xcd484d18, __VMLINUX_SYMBOL_STR(wait_for_completion_interruptible) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0x9166fada, __VMLINUX_SYMBOL_STR(strncpy) },
	{ 0xd985dc99, __VMLINUX_SYMBOL_STR(mempool_free_pages) },
	{ 0xd3db4fe9, __VMLINUX_SYMBOL_STR(generic_make_request) },
	{ 0x8a99a016, __VMLINUX_SYMBOL_STR(mempool_free_slab) },
	{ 0xc06cf893, __VMLINUX_SYMBOL_STR(up_write) },
	{ 0xdc0a2218, __VMLINUX_SYMBOL_STR(down_write) },
	{ 0xfe487975, __VMLINUX_SYMBOL_STR(init_wait_entry) },
	{ 0xc25419c9, __VMLINUX_SYMBOL_STR(bio_endio) },
	{ 0xefa08cc4, __VMLINUX_SYMBOL_STR(bio_put) },
	{ 0xf11543ff, __VMLINUX_SYMBOL_STR(find_first_zero_bit) },
	{ 0x7cd8d75e, __VMLINUX_SYMBOL_STR(page_offset_base) },
	{ 0xb601be4c, __VMLINUX_SYMBOL_STR(__x86_indirect_thunk_rdx) },
	{ 0x3d159270, __VMLINUX_SYMBOL_STR(blkdev_put) },
	{ 0x9a6f2471, __VMLINUX_SYMBOL_STR(mempool_alloc) },
	{ 0x69bf6b1c, __VMLINUX_SYMBOL_STR(bdevname) },
	{ 0x8ddd8aad, __VMLINUX_SYMBOL_STR(schedule_timeout) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0x696727a5, __VMLINUX_SYMBOL_STR(mempool_create) },
	{ 0x6b2dc060, __VMLINUX_SYMBOL_STR(dump_stack) },
	{ 0x2ea2c95c, __VMLINUX_SYMBOL_STR(__x86_indirect_thunk_rax) },
	{ 0x13bb1b43, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xbb2d3099, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xe5c6ae21, __VMLINUX_SYMBOL_STR(mempool_free) },
	{ 0x56321ae2, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x961d8335, __VMLINUX_SYMBOL_STR(kmem_cache_create) },
	{ 0xfe768495, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0xb3f7646e, __VMLINUX_SYMBOL_STR(kthread_should_stop) },
	{ 0x237a015a, __VMLINUX_SYMBOL_STR(prepare_to_wait_event) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xd4fa5c30, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x19cf472b, __VMLINUX_SYMBOL_STR(complete) },
	{ 0x8562547a, __VMLINUX_SYMBOL_STR(bdget) },
	{ 0xb01b44c1, __VMLINUX_SYMBOL_STR(blkdev_get) },
	{ 0x14f6cb5f, __VMLINUX_SYMBOL_STR(bdget_disk) },
	{ 0x5c7574a1, __VMLINUX_SYMBOL_STR(vsprintf) },
	{ 0x80dba18, __VMLINUX_SYMBOL_STR(bdput) },
	{ 0xb0079855, __VMLINUX_SYMBOL_STR(filp_open) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "072B164958EF29CC8D6A6B2");
