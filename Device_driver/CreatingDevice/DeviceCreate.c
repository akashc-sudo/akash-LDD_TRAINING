/*	 Creating the device dynamically with help of cdev structure 	*/

#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/err.h>
#include<linux/device.h>

#define DEVICE_NAME "my_driver"

MODULE_AUTHOR("Akash");
MODULE_LICENSE("GPL");
// major and minor number intialization
dev_t dev = 0;
static struct cdev device_cdev;
static struct class *dev_class;


static int Device_open(struct inode *inode, struct file *file);
static int Device_release(struct inode *inode,struct file *file);
ssize_t Device_read(struct file *flip,char __user *buffer,size_t size,loff_t *offset);
ssize_t Device_write(struct file *flip,const char __user *buffer,size_t size,loff_t *offset);


static struct file_operations fops = {.owner = THIS_MODULE, .open = Device_open, .release = Device_release, .read = Device_read, .write = Device_write};


static int Device_open(struct inode *inode, struct file *file)
{
	pr_info("Device opened!\n");
	return 0;
}

static int Device_release(struct inode *inode,struct file *file)
{
	pr_info("device closed!\n");
	return 0;
}

ssize_t Device_read(struct file *flip,char __user *buffer,size_t size,loff_t *offset)
{
	pr_info("device In  Read Mode!\n");
	return size;
}

ssize_t Device_write(struct file *flip,const char __user *buffer,size_t size,loff_t *offset)
{
	pr_info("device In Write MOde!\n");
	return size;
}


static int __init FunInit(void)
{
	 
	pr_info("kernel Module inserted Successfully......\n");
	if(alloc_chrdev_region(&dev,0,1,DEVICE_NAME) < 0)
	{
		pr_info("Not possible to allocate the major Number\n");
		return -1;
	}
	pr_info("MAJOR = %d MINOR = %d\n",MAJOR(dev),MINOR(dev));
	
	//creatig cdev struct var 
	cdev_init(&device_cdev,&fops);
       
	//adding the device to the system 
	
	if(cdev_add(&device_cdev,dev,1) < 0 )
	{
		pr_info("not able to add the device to the system...\n");
	        goto r_class;
	}
        if(IS_ERR(dev_class = class_create("akash_class")))
         {
		 pr_info("not able to create class....\n");
		 goto r_class;
	 }

	if(IS_ERR(device_create(dev_class,NULL,dev,NULL,DEVICE_NAME)))
	{
		pr_info("Not able to create device.....\n");
		goto r_device;
	}
	return 0;

r_device:
        class_destroy(dev_class);

r_class : 
         unregister_chrdev_region(dev,1);
	 return -1;
}

static void __exit FunExit(void)
{
	unregister_chrdev_region(dev,1);
	pr_info("Kernel Module Removed SuccessFully....!\n");

}

module_init(FunInit);
module_exit(FunExit);
	
