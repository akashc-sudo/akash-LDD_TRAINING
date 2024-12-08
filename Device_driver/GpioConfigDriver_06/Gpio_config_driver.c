/**
 * 	
 * 	configuring the raspberry pi gpio-17 pin as output_direction using gpio api's doing operation 
 * 	on that corresponding pin by  user application. you can refer application code @GPIO_07 directory 
 *
 **/

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/slab.h>
#include<linux/err.h>
#include<linux/gpio.h>
#include<linux/interrupt.h>

/**
 *	configuring GPIO17 pin that mapped to unique number 
 *	check@ /sys/kernel/debug/gpio
 **/

//#define 17 529        
#define GPIO_NUM 529         


static dev_t dev=0;
static struct class *dev_class;
static struct cdev cdev_device;
static int op_value = 0;


/*      Function prototypes     */
static int  __init FunInit(void);
static void  __exit FunExit(void);
static int device_open(struct inode *inode,struct file *file);
static int device_close(struct inode *inode,struct file *file);
static ssize_t device_read(struct file *flip,char __user *user_buffer,size_t size,loff_t *offset);
static ssize_t device_write(struct file *flip,const char __user *user_buffer,size_t size,loff_t *offset);



/*      File Operation Structure        */

static struct file_operations f_ops = {
        .owner = THIS_MODULE,
        .open  = device_open,
        .release = device_close,
        .read = device_read,
        .write = device_write,
};

static int device_open(struct inode *inode,struct file *file){
        pr_info("Device opened successfuly.......!\n");
        return 0;
}

static int device_close(struct inode *inode,struct file *file)
{
        pr_info("device closed successfully........!\n");
        return 0;
}

static ssize_t device_read(struct file *flip,char __user *user_buffer,size_t size,loff_t *offset){
        char kernel_buffer[2];
        
	snprintf(kernel_buffer,2,"%d",op_value);

	if(copy_to_user(user_buffer,kernel_buffer,size))
        {
                pr_err("NOt able to copy to usser buffer from kerne; buffer.....\n");
                return -EFAULT;
        }
        pr_info("Kernel_buffer sent %zu(s) bytes to the user.....\n",size);
        return size;
}

static ssize_t device_write(struct file *flip,const char __user *user_buffer,size_t size,loff_t *offset)
{
        char kernel_buffer[2];
	//int kernel_buffer;

        if(copy_from_user(kernel_buffer,user_buffer,size))
        {
                pr_err("Not able to copy from user_buffer to kernel_buffer....\n");
                return  -EFAULT;
        }
        kernel_buffer[size] = '\0';
/**
 ** 	converting string into long integer using simple_strtol	
 */
         op_value = simple_strtol(kernel_buffer,NULL,10);
	 //op_value = kernel_buffer;
	 pr_info("debug :::::::::::::::::::::::::::::::: %d\n",op_value);
       
	if(op_value == 0 || op_value == 1)
	{
		gpio_set_value(GPIO_NUM,op_value);
		pr_info("Gpio value written on device....!\n");
	}
	else 
	{
		pr_info("Invalid value : gpio accept 0 or 1....\n");
		return -EINVAL;
	}

        pr_info("user_buffer recieved %zu bytes from user.......\n",size);
        return size;
}


/*	Module Function Intialization     */


static int __init FunInit(void)
{
        pr_info("Module Loaded Successfully..........!\n");
        if((alloc_chrdev_region(&dev,0,1,"Gpio_device")) < 0){
                pr_err("Not able to create major number and minor number\n.......!\n");
		unregister_chrdev_region(dev,1);
                return -1;
        }
        pr_info("MAJOR = %d/t MINOR = %d\n",MAJOR(dev),MINOR(dev));

/*      Intialize the cdev structure to add to the system       */
        
        cdev_init(&cdev_device,&f_ops);
/*      Add the device to the cdev structure    */

        if((cdev_add(&cdev_device,dev,1)) < 0)
        {
                pr_info("Can't add the device to the system........!\n");
		cdev_del(&cdev_device);
                unregister_chrdev_region(dev,1);
		return 0;
        }

/*      Creating class for device       */

        if(IS_ERR(dev_class = class_create("class_akash")))
        {
                pr_info("Not able to create class for device\n");
		class_destroy(dev_class);
                unregister_chrdev_region(dev,1);
		return 0;
        }
/*      Creating device under "/dev" directory  with the name mentioned @param 4 of device_create()     */

        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"GpioDevice")))
        {
                pr_err("Not able to create device for class\n");        
		class_destroy(dev_class);
		device_destroy(dev_class,dev);
                unregister_chrdev_region(dev,1);
		return 0;
        }


        unsigned int  request_status;	
        request_status = gpio_request(GPIO_NUM,"Gpio_driver");
	       if(request_status)
	       {
	               pr_info("Gpio_request is failed..........!\n");
		       return request_status;
	       } 
	 
	 /*	setting  output_direction as "0" using below api	*/

	  int output_status =  gpio_direction_output(GPIO_NUM,0);
	  if(output_status < 0 ){pr_info("failed to set direction for %d GPIO_NUM,%d output_status",GPIO_NUM,output_status);}
      
	  return 0;
	
}
/*      Module Function Unintialized    */

static void  __exit FunExit(void)
{
/*	Releasing Gpio_num using below gpio api		*/
	gpio_free(GPIO_NUM);

        class_destroy(dev_class);
        
	device_destroy(dev_class,dev);
        
	cdev_del(&cdev_device);
        
	unregister_chrdev_region(dev,1);
        
	pr_info("Module Unloaded Successfully............!\n");
        //return 0;
}

/**      registring Init and Exit fun to the macros,
 *       when insert the module with insmod FunInit will be invoked and
 *       when remove the module with rmmod FunExit will execute 
 **/
module_init(FunInit);
module_exit(FunExit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("AKASH <akashc@gmail.com>");
MODULE_DESCRIPTION("Configuring the Gpio17 pin of an Raspberrypi 4");
MODULE_VERSION("6.0.0");


