/*
 * File : kthreadDriver.c 
 * Details : Simple driver(kernel thread) 
 **/


#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/kthread.h>             //kernel threads
#include <linux/sched.h>               //task_struct 
#include <linux/delay.h>
#include <linux/err.h>

dev_t dev = 0;
static struct class *dev_class;
static struct cdev cdev_device;
static int count = 0;
static struct task_struct *ref_thread;

/********************Fun prototypes***************/
static int __init FunInit(void);
static void __exit FunExit(void);

static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *filp,char __user *buf, size_t len,loff_t * off);
static ssize_t device_write(struct file *filp,const char *buf, size_t len, loff_t * off);

int thread_function(void *msg);

int thread_function(void *msg)
{
     while(!kthread_should_stop())
     {
  	pr_info("In thread function..... %d\n",count++);	
	msleep(1000);
     }
     return 0;
}
/*File operation*/

struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open  = device_open,
	.release = device_release,
	.read  = device_read,
	.write = device_write,
};
/*
** This function will be called when we open the Device file
*/  
static int device_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}
/*
** This function will be called when we close the Device file
*/   
static int device_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}
/*
** This function will be called when we read the Device file
*/
static ssize_t device_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Read function\n");
        return len;
}
/*
** This function will be called when we write the Device file
*/
static ssize_t device_write(struct file *filp,const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Write Function\n");
        return len;
}

/* Module Intialization
 * creating the thread using thread_create api or thread_run api 
 * thread_create api creates thread only but not executed so for that we have to use wake_up_process api 
 * thread_run api creates thread as well as run
 * @para_1 : thread haldler-fun 
 * @para_2 : the pointer which is pointing to task_struct we can write as  NULL
 * @para_3 : 
 * @para_4 : name formats
 * */
static int __init FunInit(void)
{


        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "thread_Dev")) <0){
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&cdev_device,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&cdev_device,dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if(IS_ERR(dev_class = class_create("device_class"))){
            pr_err("Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"thread_device"))){
            pr_err("Cannot create the Device \n");
            goto r_device;
        } 
        ref_thread =kthread_create(thread_function,NULL,"my_thread");
	if(ref_thread)
	{
		wake_up_process(ref_thread); 
	}
	pr_info("kthread Module Loaded Successfully\n");
        return 0;


r_device:
class_destroy(dev_class);
device_destroy(dev_class,dev);

r_class:
unregister_chrdev_region(dev,1);
return -1;

}

static void __exit FunExit(void)
{
        kthread_stop(ref_thread);
        class_destroy(dev_class);
	device_destroy(dev_class,dev);
        cdev_del(&cdev_device);
        unregister_chrdev_region(dev, 1);
	pr_info("kthread Module Unloaded Successfully\n");
}

module_init(FunInit);
module_exit(FunExit);

/********module info********/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("AKASH");
MODULE_DESCRIPTION("Creating thread and executing till removing the module");














