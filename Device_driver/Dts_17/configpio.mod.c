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
	{ 0x929eb52e, "of_node_put" },
	{ 0xb0d2a97, "gpio_to_desc" },
	{ 0x3524cbe9, "gpiod_set_raw_value" },
	{ 0xfe990052, "gpio_free" },
	{ 0x122c3a7e, "_printk" },
	{ 0xd7840abd, "of_find_node_by_name" },
	{ 0xb9ef3539, "of_get_next_child" },
	{ 0xb3829c29, "of_node_name_eq" },
	{ 0x1b141d47, "of_get_named_gpio" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xb161b235, "gpiod_direction_output_raw" },
	{ 0xf9a482f9, "msleep" },
	{ 0x67a35d9, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "A07C4D1255F8B7D6AC0A2E0");
