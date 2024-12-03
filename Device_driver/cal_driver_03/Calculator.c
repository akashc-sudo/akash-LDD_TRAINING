#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user
#include<linux/init.h>

#define DEVICE_NAME "SimpleDevice"
#define BUFFER_SIZE 1024
#define ARRAY_LEN 3

static int major_number;
static char device_buffer[ARRAY_LEN][BUFFER_SIZE];
static int open_count = 0;
static int value = 0;
static int Num = 0;

// Function prototypes for device operations
static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset);

// File operations structure
static struct file_operations fops = {
    .open = device_open,
    .release = device_close,
    .read = device_read,
    .write = device_write,
};


static int  convert_atoi(char *str)
{
	value = 0;
	static char *tmp = str;
	while(*tmp)
	{
		value = (value * 10) + ( (*tmp) - 48);
		tmp++;
	}
         
	return value;
	//value = (value) * 2;
	//sprintf(device_buffer,"%d",value);
}

static int sum_op(void)
{
	int num1 = convert_atoi(&str[1]);
	int num2 = convert_atoi(&str[2]);

	return (num1 + num2 );
}

static int sub_op(void){}
static int mul_op(void){}
static int div_op(void){}

// Called when the device is opened
static int device_open(struct inode *inode, struct file *file) {
    open_count++;
    printk(KERN_INFO "simple_device: Device opened %d time(s)\n", open_count);
    return 0;
}

// Called when the device is closed
static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_device: Device closed\n");
    return 0;
}

// Called when data is read from the device
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    size_t len = strlen(device_buffer);

    if (*offset >= len) // Check if all data has been read
        return 0;

    if (size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;

    if (copy_to_user(user_buffer, device_buffer + *offset, size)) {
        return -EFAULT;
    }

    *offset += size;
    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    if (size > BUFFER_SIZE - 1) // Limit size to buffer capacity
        size = BUFFER_SIZE - 1;

    if (copy_from_user(device_buffer, user_buffer, size)) {
        return -EFAULT;
    }

    device_buffer[size] = '\0'; // Null-terminate the string
    printk(KERN_INFO "simple_device: Received %zu bytes from the user\n", size);
    printk(KERN_INFO "simple_device: read data = %s\n",device_buffer);

    //Num = convert_atoi();
    for(int i = 0 ; i < 3 ;i++)
    {
    if(strcmp(device_buffer[i],"SUM") == 0)
    {
	    Num = sum_op();
	    break;
    }
    else if(strcmp(device_buffer[i],"SUB") == 0)
    {
	    Num = sub_op();
	    break;
    }
    else if(strcmp(device_buffer[i],"MUL") == 0)
    {
	    Num = mul_op();
	    break;
    }
    else if(strcmp(device_buffer[i],"DIV") == 0)
    {
	    Num = Div_op();
    }
    }
    sprintf(device_buffer,"%d",Num);
    return size;
}

// Module initialization
static int __init simple_driver_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "simple_device: Failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "simple_device: Registered with major number %d\n", major_number);
    return 0;
}

// Module cleanup
static void __exit simple_driver_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "simple_device: Unregistered device\n");
}

module_init(simple_driver_init);
module_exit(simple_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Simple Linux Device Driver");
