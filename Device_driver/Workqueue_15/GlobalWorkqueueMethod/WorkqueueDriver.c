/*
 * \Dir     :  GlobalWorkqueueMethod
 *
 * \file    :  WorkqueueDriver.c
 *
 * \details :  Whenevery trying to read the Devicefile interrupt handler will execute and it schedule the work
 *             using Global Workqueue - static method
 *
 * \author : Akash
 *
 **/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include<linux/sysfs.h>
#include<linux/kobject.h>
#include <linux/interrupt.h>
#include <asm/io.h>                    
#include <linux/workqueue.h>            // Required for workqueues
#include <linux/err.h>

#define IRQ_NO 12


void workqueue_fn(struct work_struct *work);

/*Creating work by Static Method */
DECLARE_WORK(workqueue,workqueue_fn);

/*Workqueue Function*/
void workqueue_fn(struct work_struct *work)
{
        printk(KERN_INFO "Executing Workqueue Function\n");
}


/*
 * Interrupt handler for IRQ 11.
 * Scheduling the work using schedule_work() API
 */
static irqreturn_t irq_handler(int irq,void *dev_id) {
        printk(KERN_INFO "Shared IRQ: Interrupt Occurred");
        schedule_work(&workqueue);

        return IRQ_HANDLED;
}


volatile int device_value = 0;


dev_t dev = 0;
static struct class *dev_class;
static struct cdev dev_cdev;
struct kobject *kobj_ref;

/*
** Function Prototypes
*/
static int __init FunInit(void);
static void __exit FunExit(void);

/*************** Driver Fuctions **********************/
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t device_write(struct file *filp,const char *buf, size_t len, loff_t * off);

/*************** Sysfs Fuctions **********************/
static ssize_t sysfs_show(struct kobject *kobj,
                struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj,
                struct kobj_attribute *attr,const char *buf, size_t count);

struct kobj_attribute etx_attr = __ATTR(device_value, 0660, sysfs_show, sysfs_store);

/*
** File operation sturcture
*/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = device_read,
        .write          = device_write,
        .open           = device_open,
        .release        = device_release,
};

/*
** This function will be called when we read the sysfs file
*/
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
        printk(KERN_INFO "Sysfs - Read!!!\n");
        return sprintf(buf, "%d", device_value);
}

/*
** This function will be called when we write the sysfsfs file
*/
static ssize_t sysfs_store(struct kobject *kobj,
                struct kobj_attribute *attr,const char *buf, size_t count)
{
        printk(KERN_INFO "Sysfs - Write!!!\n");
        sscanf(buf,"%d",&device_value);
        return count;
}

/*
** This function will be called when we open the Device file
*/
static int device_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}

/*
** This function will be called when we close the Device file
*/
static int device_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}

/*
** This function will be called when we read the Device file
*/
static ssize_t device_read(struct file *filp,
                char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Read function\n");
        asm("int $0x3B");  // Corresponding to irq 11
        return 0;
}

/*
** This function will be called when we write the Device file
*/
static ssize_t device_write(struct file *filp,
                const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Write Function\n");
        return len;
}

/*
** Module Init function
*/
static int __init FunInit(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "work_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        /*Creating cdev structure*/
        cdev_init(&dev_cdev,&fops);

        /*Adding character device to the system*/
        if((cdev_add(&dev_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }

        /*Creating struct class*/
        if(IS_ERR(dev_class = class_create("my_class"))){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }

        /*Creating device*/
        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"SimpleDevice"))){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }

        /*Creating a directory in /sys/kernel/ */
        kobj_ref = kobject_create_and_add("etx_sysfs",kernel_kobj);

        /*Creating sysfs file for etx_value*/
        if(sysfs_create_file(kobj_ref,&etx_attr.attr)){
                printk(KERN_INFO"Cannot create sysfs file......\n");
                goto r_sysfs;
        }
        if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "SimpleDevice", (void *)(irq_handler))) {
            printk(KERN_INFO "my_device: cannot register IRQ ");
                    goto irq;
        }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
        return 0;

irq:
        free_irq(IRQ_NO,(void *)(irq_handler));

r_sysfs:
        kobject_put(kobj_ref);
        sysfs_remove_file(kernel_kobj, &etx_attr.attr);

r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        cdev_del(&dev_cdev);
        return -1;
}

/*
** Module exit function
*/
static void __exit FunExit(void)
{
        free_irq(IRQ_NO,(void *)(irq_handler));
        kobject_put(kobj_ref);
        sysfs_remove_file(kernel_kobj, &etx_attr.attr);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&dev_cdev);
        unregister_chrdev_region(dev, 1);
        printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}

module_init(FunInit);
module_exit(FunExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akash");
MODULE_DESCRIPTION("Simple Linux device driver (Global Workqueue - Static method)");
