/* 
 *  FILE        : SimpleIoctlDriver.c
 *  DESCRIPTION : Implementing simple Ioctl mechanism on Raspberrypi4 
 */
 
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#define DEVICE_NAME "IoctlDevice"
#define GPIO_PIN  529        /*GPIO-17 pin is mapped to the unique number 529 check@ /sys/kerenl/debug/gpio*/  

/* intialize the command for Ioctl api it consist 3 arguments
 * arg_1 : struct file 
 * arg_2 : command which is recieved from user space based on that it will operates in kernel space
 * arg_3 : argument   
 */


#define GPIO_MAGIC     'A'
#define TO_SET_DIR   _IOW(GPIO_MAGIC, 0, int)  // Set direction (input/output)
#define TO_WRITE     _IOW(GPIO_MAGIC, 1, int)  // Write value to GPIO
#define TO_READ      _IOR(GPIO_MAGIC, 2, int)  // Read value from GPIO

static dev_t dev;
static struct cdev cdev_device;

/*	Fun prototypes	   */
static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = device_ioctl,
    .open           = device_open,
    .release        = device_release,
};

/*   Open the device   */
static int device_open(struct inode *inode, struct file *file)
{
    pr_info("device opened successfully!\n");
    return 0;
}


/*   Release the device    */
static int device_release(struct inode *inode, struct file *file)
{
    pr_info("device closed!\n");
    return 0;
}

/*	 ioctl handler     */
static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int value;
    switch (cmd) {
    case TO_SET_DIR : if (arg == 0)
		      {
                          gpio_direction_input(GPIO_PIN);
                          pr_info("Set GPIO %d as input\n", GPIO_PIN);
		      }
		      else
		      {
                          gpio_direction_output(GPIO_PIN, 0);
                          pr_info("Set GPIO %d as output\n", GPIO_PIN);
                      }
                      break;

    case TO_WRITE  : if(gpio_direction_output(GPIO_PIN, 0)) {
                     pr_err("Failed to set GPIO %d as output\n", GPIO_PIN);
                     return -EINVAL;
                     }
                     if(copy_from_user(&value,(int __user *)arg, sizeof(value))) {
                     return -EFAULT;
                     }
                     gpio_set_value(GPIO_PIN, value);
                     pr_info("Written %d to GPIO %d\n", value, GPIO_PIN);
                     break;

    case TO_READ  : value = gpio_get_value(GPIO_PIN);
                    if(copy_to_user((int __user *)arg, &value, sizeof(value)))
       		    {
                	 return -EFAULT; 
		    } 
                    pr_info("Read value %d from GPIO %d\n", value, GPIO_PIN);
                    break;

    default      : pr_err("Invalid ioctl command\n");
                   return -EINVAL;
                 }
    return 0;
}

/*
*   Module init function
*/
static int __init FunInit(void)
{
/*  dynamically allocating the major and minor number for given DEVICE_NAME check@ /proc/devices  */
    if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0){
        pr_err("Failed to allocate device number\n");
        return -1;
    }

/* 
 *  Intializing the struct cdev that will helps to create device dynamically 
 *  and adding that to the system  using cdev_add() 
 */
    cdev_init(&cdev_device,&f_ops);

    if (cdev_add(&cdev_device, dev, 1) < 0) {
        pr_err("Failed to add cdev\n");
	//cdev_del(&cdev_device);
        unregister_chrdev_region(dev,1);
        return 0;
    }

/*  gpio_request api requesting to the gpio hardware controller for given GPIO_PIN   */
    if (gpio_request(GPIO_PIN, "gpio_ioctl") < 0) {
        pr_err("Failed to request GPIO pin\n");
        cdev_del(&cdev_device);
        unregister_chrdev_region(dev,1);
	return 0;
    }

    pr_info("Ioctl Driver Loaded Successfully.................!\n");
    return 0;

}

/*
** Module exit function
*/
static void __exit FunExit(void)
{
    gpio_free(GPIO_PIN);
    cdev_del(&cdev_device);
    unregister_chrdev_region(dev, 1);
    pr_info("Ioctl  Driver Unloaded Successfully................!\n");
}

module_init(FunInit);
module_exit(FunExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AKASH");
MODULE_DESCRIPTION("Implemented simple ioctl mechanism for Raspberry Pi4 system");
MODULE_VERSION("6.0.2");
