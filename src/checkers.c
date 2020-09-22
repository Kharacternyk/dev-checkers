#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nazar Vinnichuk");

static int open(struct inode *inode, struct file *file) {
    return 0;
}

static int release(struct inode *inode, struct file *file) {
    return 0;
}

const char *msg = "You are reading /dev/checkers\n";
static ssize_t read(struct file *file, char *buffer, size_t size, loff_t *offset) {
    ssize_t length = strlen(msg) - *offset;
    if (length <= 0) {
        return 0;
    }
    if (size < length) {
        length = size;
    }
    if (copy_to_user(buffer, msg + *offset, length)) {
        return -EFAULT;
    }
    *offset += length;
    return length;
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
