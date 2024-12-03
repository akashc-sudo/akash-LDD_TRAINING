#include"header.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akash <akashc7619@gmail.com>");
MODULE_DESCRIPTION("Simple driver");


static int __init FunInit(void)
{
	printk(KERN_INFO "Module inserted successfully\n");
	return 0;

}


static void __exit FunExit(void)
{
	printk("Module Removed Successfully\n");
}

module_init(FunInit);
module_exit(FunExit);

