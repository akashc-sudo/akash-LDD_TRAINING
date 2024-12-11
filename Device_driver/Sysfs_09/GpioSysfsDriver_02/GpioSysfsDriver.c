
/*
 * 	Implementing Sysfs gpio driver for raspberrypi4   
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

/* Gpio-17 num is mapped to unique number 529  check@ /sys/kernel/gpio	*/
#define GPIO_PIN 529 

/* create variable type of int to hold the state of led */
volatile int led_value = 0;

static struct kobject *ref_kobject;


/*	Function Prototypes	*/

static int __init FunInit(void);
static void __exit FunExit(void);

/***********************Gpio Function Prototype*************************************/

void gpio_init(void);
void gpio_clean(void);

/************************Sysfs Function Prototype ***********************************/
static ssize_t sysfs_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf);
static ssize_t sysfs_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count);

/*
 * __ATTR macro helps to create kernel object attributes
 *  sysfs_show will call when the sysfs file reads 
 *  sysfs_store will call when the sysfs file writes 
 */
struct kobj_attribute device_attribute =  __ATTR(led_value,0644,sysfs_show,sysfs_store);


int gpio_init(void)
{
	if(!gpio_is_valid(GPIO_PIN))
	{
		return -1;
	}
        /*  requesting to Gpio hardware controller for specific gpio_number/pin  */
	int request_status = gpio_request(GPIO_PIN,"Gpio_req");
	if(request_status){
		pr_info("Gpio Request Failed........\n");
		return -1;
	}
	int output_status = gpio_direction_output(GPIO_PIN,0);
	if(output_status < 0)
	{
		pr_info("Not able set Direction......failed!\n");
		return -1;
	}
	return 1;
}

void gpio_clean(void)
{
	gpio_free(GPIO_PIN);
}
static ssize_t sysfs_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{
pr_info("sysfs - Read..!!\n");
return sprintf(buf,"%d",led_value);
}


static ssize_t sysfs_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count)
{
	pr_info("sysfs - Write..!!\n");
	sscanf(buf,"%d",&led_value);
        //pr_info("DATA : %d",led_value);
        gpio_set_value(GPIO_PIN,led_value);
	return count;
}
// Initialize module
static int __init FunInit(void) {
    int status;
    pr_info("Sysfs Module Loaded Successfully..................!");
    status = gpio_init();
    if(status < 0)
    {
	   gpio_clean();
	   return 0;
    }

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
     *Creating the Sysfs File
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

//Exit module
static void __exit FunExit(void) {
    gpio_clean();
    kobject_put(ref_kobject);
    sysfs_remove_file(kernel_kobj,&device_attribute.attr);
    pr_info("Sysfs Module Unloaded Successfully.............!\n");
}

module_init(FunInit);
module_exit(FunExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AKASH");


