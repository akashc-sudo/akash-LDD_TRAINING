#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

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
	{ 0xee9d5dd1, "i2c_get_adapter" },
	{ 0x900c9157, "i2c_new_client_device" },
	{ 0xfe266025, "i2c_register_driver" },
	{ 0xb1a12415, "i2c_put_adapter" },
	{ 0x122c3a7e, "_printk" },
	{ 0xc8f72763, "i2c_unregister_device" },
	{ 0xb02e77f7, "i2c_del_driver" },
	{ 0x86c9432a, "i2c_transfer_buffer_flags" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xf9a482f9, "msleep" },
	{ 0x67a35d9, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:ETX_OLED");

MODULE_INFO(srcversion, "D2D20D19D57FA17710CE49B");
