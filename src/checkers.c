#include <linux/module.h>
#include <linux/kernel.h>
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

const char *msg = "You are reading /dev/checkers\n";
static ssize_t read(struct file *file, char *buffer, size_t size, loff_t *offset) {
    const char *cp = msg;
    while (*cp && (cp - msg < size)) {
        put_user(*cp++, buffer++);
    }
    return cp - msg;
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

module_misc_device(dev);
