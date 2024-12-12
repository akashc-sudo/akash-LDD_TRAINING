#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_FILENAME "my_proc_file"
#define PROC_MAX_SIZE 1024

static char proc_data[PROC_MAX_SIZE];
static struct proc_dir_entry *proc_file;

/*  when proc file in Read mode this function will execute */
static ssize_t proc_read(struct file *file, char __user *user_buffer, size_t size, loff_t *position) {
    return simple_read_from_buffer(user_buffer, size, position, proc_data, strlen(proc_data));
}

/* when proc file in Write mode below function will execute */
static ssize_t proc_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *position) {
    if (size >= PROC_MAX_SIZE)
        return -EINVAL;

    if (copy_from_user(proc_data, user_buffer, size))
        return -EFAULT;

    proc_data[size] = '\0'; // Null-terminate the string
    return size;
}

/* proc File operations structure  */
static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

/* Module initialization */
static int FunInit(void) {
    proc_file = proc_create(PROC_FILENAME, 0666, NULL, &proc_fops);
    if (!proc_file) {
        pr_err("Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }

    strcpy(proc_data, "Hello from procfs!\n");
    pr_info("procfs_example module loaded. File: /proc/%s\n", PROC_FILENAME);
    return 0;
}

/* Module cleanup */
static void __exit FunExit(void) {
    proc_remove(proc_file);
    pr_info("procfs_example module unloaded\n");
}

module_init(FunInit);
module_exit(FunExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akash");
MODULE_DESCRIPTION("A simple procfs driver");
