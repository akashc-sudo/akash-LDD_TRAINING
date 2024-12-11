/*
 * 	Implementing Basic Sysfs driver in an x86_64arc  
 */



#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/slab.h>

volatile int device_value = 0;

static struct kobject *ref_kobject;
//static int led_state = 0;


/*	Function Prototypes	*/

static int __init FunInit(void);
static void __exit FunExit(void);

/************************ Sysfs Function Prototype ***********************************/
static ssize_t sysfs_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf);
static ssize_t sysfs_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count);

/*
 * __ATTR macro helps to create kernel object attributes
 *  sysfs_show will call when the sysfs file reads 
 *  sysfs_store will call when the sysfs file writes 
 */
struct kobj_attribute device_attribute =  __ATTR(device_value, 0777, sysfs_show,sysfs_store);


static ssize_t sysfs_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{
pr_info("sysfs - Read..!!\n");
return sprintf(buf,"%d",device_value);
}


static ssize_t sysfs_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count)
{
	pr_info("sysfs - Write..!!\n");
	sscanf(buf,"%d",&device_value);
	return count;
}
// Initialize module
static int __init FunInit(void) {

    pr_info("Sysfs Module Loaded Successfully..................!");
    int ret;
    /*
     * Creating sysfs kernel object and adding that under /sys/kernel/ using kobject_create_and_add api
     * @pra_1 : Name/(own_directory_creating_under_/sys/)  creating  under /sys/kernel/"sysfs_device"/
     * @pra_2 : Representing parent directory for "sysfs_device"(own directory)
     *          can use firmware_kobj,fs_kobj instead of kernel_kobj 
     **/
    ref_kobject = kobject_create_and_add("sysfs_device", kernel_kobj);
    if (!ref_kobject) {
        pr_err("Failed to create sysfs kobject\n");
        return -ENOMEM;
    }

    /*
     * Creating the Sysfs File
     **/
    ret = sysfs_create_file(ref_kobject, &device_attribute.attr);
    if (ret) {
        pr_err("Failed to create sysfs file\n");
        goto r_kobject;
        return ret;
    }
    return 0;


r_kobject :
kobject_put(ref_kobject);
sysfs_remove_file(kernel_kobj,&device_attribute.attr);
return 0;


}

// Exit module
static void __exit FunExit(void) {
    kobject_put(ref_kobject);
    sysfs_remove_file(kernel_kobj,&device_attribute.attr);
    pr_info("Sysfs Module Unloaded Successfully.............!\n");
}

module_init(FunInit);
module_exit(FunExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AKASH");

