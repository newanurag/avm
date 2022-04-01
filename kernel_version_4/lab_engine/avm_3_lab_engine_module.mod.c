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
	{ 0xd28020dd, __VMLINUX_SYMBOL_STR(up_read) },
	{ 0x672edad8, __VMLINUX_SYMBOL_STR(pv_lock_ops) },
	{ 0xe198d8af, __VMLINUX_SYMBOL_STR(dz_avm_log_file_write) },
	{ 0x57a590f8, __VMLINUX_SYMBOL_STR(dz_page_alloc) },
	{ 0xfc18b0b8, __VMLINUX_SYMBOL_STR(_raw_read_lock) },
	{ 0xce935a56, __VMLINUX_SYMBOL_STR(dz_kfree) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0x97651e6c, __VMLINUX_SYMBOL_STR(vmemmap_base) },
	{ 0xd83bc98c, __VMLINUX_SYMBOL_STR(down_read) },
	{ 0x4ff67fab, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0x3f3dd36, __VMLINUX_SYMBOL_STR(_raw_write_lock) },
	{ 0x587c8d3f, __VMLINUX_SYMBOL_STR(down) },
	{ 0xc06cf893, __VMLINUX_SYMBOL_STR(up_write) },
	{ 0xdc0a2218, __VMLINUX_SYMBOL_STR(down_write) },
	{ 0x7cd8d75e, __VMLINUX_SYMBOL_STR(page_offset_base) },
	{ 0xae569e6c, __VMLINUX_SYMBOL_STR(dz_page_free) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0x6b2dc060, __VMLINUX_SYMBOL_STR(dump_stack) },
	{ 0x13bb1b43, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x56321ae2, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x17c8215e, __VMLINUX_SYMBOL_STR(up) },
	{ 0xaebd92d, __VMLINUX_SYMBOL_STR(dz_kmalloc) },
	{ 0xe9e93c47, __VMLINUX_SYMBOL_STR(__init_rwsem) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=avm_2_infra_engine_module";


MODULE_INFO(srcversion, "FC01C1B1BF87FE40E8A8E15");
