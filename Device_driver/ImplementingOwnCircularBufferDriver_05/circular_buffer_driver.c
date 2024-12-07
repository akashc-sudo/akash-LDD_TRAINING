/*	driver about implementing the simple circular buffer	*/


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // For copy_to_user and copy_from_user
#include<linux/init.h>

#define DEVICE_NAME "SimpleDevice"
#define BUFFER_SIZE 1024
#define BUFFER_CAPACITY 5
#define MSG_SIZE 256

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akash");
MODULE_DESCRIPTION("Implementing Circular BUffer\n");


static int major_number;
static char device_buffer[BUFFER_CAPACITY][MSG_SIZE];
static int count = 0;       /*	to track number of messages */
static int head = 0;        /*  head is handling to the oldest msg */
static int tail = 0;        /*  tail is handing the new incomming msg */

// Function prototypes for device operations
static int device_open(struct inode *inode, struct file *file);
static int device_close(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset);

// File operations structure

static struct file_operations fops = {
    .open=device_open,
    .release=device_close,
    .read=device_read,
    .write=device_write
};

static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_device:opened successfully..........!\n");
    return 0;
}

static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_device:closed.............!\n");
    return 0;
}

// Called when data is read from the device

static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    size_t len = strnlen(device_buffer[tail],MSG_SIZE)+1;
    if(*offset >= len) // Check if all data has been read
        return 0;

    if(size > len - *offset) // Adjust size to avoid reading beyond buffer
        size = len - *offset;
   
    if(count == 0)
    {
	    pr_info("BUffer is Empty\n");
	    return 0;
    }

    if(copy_to_user(user_buffer,device_buffer[tail], size)) {
        return -EFAULT;
    }

    printk(KERN_INFO "simple_device: Sent %zu bytes to the user\n", size);
   // *offset += size;
    printk(KERN_INFO "simple_device: device_buffer[tail] data = %s\n",device_buffer[tail]);

    tail = (tail + 1) % BUFFER_CAPACITY;
    count--;
    return size;
}

// Called when data is written to the device
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    if (size > MSG_SIZE - 1) // Limit size to buffer capacity
        size = MSG_SIZE - 1;

    /*creating temporary buffer to hold user_buffer data*/
    char temp_buffer[MSG_SIZE];
    
    if (copy_from_user(temp_buffer,user_buffer,size)) {
        return -EFAULT;
    }

    temp_buffer[size] = '\0'; // Null-terminate the string

    printk(KERN_INFO "simple_device: Received %zu bytes from the user\n",size);
    printk(KERN_INFO "simple_device: Received data = %s\n",temp_buffer);

    /* copying the temporary buffer data to device_buffer */
    strscpy(device_buffer[head],temp_buffer,MSG_SIZE);
    head = (head + 1) % BUFFER_CAPACITY;

    if(count == BUFFER_SIZE)
    {
	    tail = (tail + 1) % BUFFER_SIZE;
    }
    else 
    {
	    count++;      //number of msg in an buffer
    }
    return size;
}

// Module initialization

static int __init FunInit(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "simple_device: Failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "simple_device: Registered with major number %d\n", major_number);
    return 0;
}

//module initialization : 

static void __exit FunExit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "simple_device: Unregistered device\n");
}

module_init(FunInit);
module_exit(FunExit);
