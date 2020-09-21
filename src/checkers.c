#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nazar Vinnichuk");

static int open(struct inode *inode, struct file *file) {
    try_module_get(THIS_MODULE);
    return 0;
}

static int release(struct inode *inode, struct file *file) {
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t read(struct file *file, char *buffer, size_t size, loff_t *offset) {
    size_t i;
    char *msg = "You are reading /dev/checkers\n";
    size_t msgsize = strlen(msg);
    for (i = 0; i < size; ++i) {
        put_user("You are reading /dev/checkers\n"[i % msgsize], buffer++);
    }
    return size;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = open,
    .release = release,
    .read = read
};
static struct miscdevice dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "checkers",
    .nodename = "checkers",
    .fops = &fops,
    .mode = 0666
};

int init_module(void) {
    if (misc_register(&dev)) {
        printk(KERN_ALERT "Failed to register checkers character device.\n");
        return -1;
    }
    return 0;
}

void cleanup_module(void) {
    misc_deregister(&dev);
}
