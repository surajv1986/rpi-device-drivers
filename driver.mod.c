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
	{ 0x5b6dfd24, "class_destroy" },
	{ 0x4060a0c4, "device_create" },
	{ 0x49d2824e, "__class_create" },
	{ 0x1eacc803, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0x5f754e5a, "memset" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0xbed00335, "kmem_cache_alloc_trace" },
	{ 0x55f8e9c0, "kmalloc_caches" },
	{ 0xc5850110, "printk" },
	{ 0x37a0cba, "kfree" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "3DE46DC3D55C28BCB105B0B");
