#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
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
__used __section("__versions") = {
	{ 0xa11440c8, "module_layout" },
	{ 0x7a39daf9, "cdev_del" },
	{ 0x9dd4af8d, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xf61cef89, "wake_up_process" },
	{ 0x1451c5e0, "kthread_create_on_node" },
	{ 0x5b6dfd24, "class_destroy" },
	{ 0x4060a0c4, "device_create" },
	{ 0x49d2824e, "__class_create" },
	{ 0x2686255, "cdev_add" },
	{ 0x1eacc803, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x3dcf1ffa, "__wake_up" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x49970de8, "finish_wait" },
	{ 0x1000e51, "schedule" },
	{ 0x647af474, "prepare_to_wait_event" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "9776DF1FCF035D5D07973E5");
