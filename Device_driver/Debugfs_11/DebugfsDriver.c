#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>

#define MODULE_NAME "debugfs_device_dir"

static struct dentry *dir,*file;
static int debugfs_value = 0; /*Creating variable under /debug*/

/* Read function */
static ssize_t debugfs_read(struct file *fp, char __user *user_buffer, size_t count, loff_t *position) {
    char buffer[64];
    int len = snprintf(buffer, sizeof(buffer), "%d\n", debugfs_value);
    
    pr_info("debugfs-read!!\n");
    return simple_read_from_buffer(user_buffer, count, position, buffer, len);
}

/* Write function */
static ssize_t debugfs_write(struct file *fp, const char __user *user_buffer, size_t count, loff_t *position) {
    char buffer[64];

    pr_info("debugfs - write!!\n");
    if (count >= sizeof(buffer))
        return -EINVAL;

    if (copy_from_user(buffer, user_buffer, count))
        return -EFAULT;

    buffer[count] = '\0';
    if (kstrtoint(buffer, 10, &debugfs_value))
        return -EINVAL;

    return count;
}

/* File operations structure */
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = debugfs_read,
    .write = debugfs_write,
};

/* Module initialization 
 * Creating directory with name "debugfs_device_dir" under /sys/kernel/debug/debugfs_device_dir
 * with the help of debugfs_create_dir api 
 * @para_1 : The name of dir which is going to be create as mentioned above
 * @para_2 : parent dir name for our new dir,NULL represents creating dir under sys/kernel/debug/---
 **/

static int __init FunInit(void) {
	
    dir = debugfs_create_dir(MODULE_NAME, NULL);
    if (!dir) {
        pr_err("Failed to create debugfs directory\n");
        return -ENOMEM;
    }

/*  Creating file under /sys/kernel/debug/debugfs_device_dir/value */
    file = debugfs_create_file("debugfs_value", 0644, dir, NULL, &fops); 
    if (!file) {
        pr_err("Failed to create debugfs file\n");
        debugfs_remove_recursive(dir);
        return -ENOMEM;
    }

    pr_info("debugfs module loaded successfully...............!\n");
    return 0;
}

/* Module exit */
static void __exit FunExit(void) {
    debugfs_remove_recursive(dir); // Remove all debugfs entries
    pr_info("debugfs module unloaded successfully..............!\n");
}

module_init(FunInit);
module_exit(FunExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("akash");
MODULE_DESCRIPTION("A simple debugfs driver");
