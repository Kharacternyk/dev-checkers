#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
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
    .open = open,
    .release = release,
    .read = read
};
static dev_t maj_min;
static struct class *dev_class;
static struct device *dev;

int init_module(void) {
    int major;
    if ((major = register_chrdev(0, "checkers", &fops)) < 0) {
        goto failure;
    }

    maj_min = MKDEV(major, 0);
    dev_class = class_create(THIS_MODULE, "checkers");
    if (IS_ERR(dev_class)) {
        goto failure;
    }
    dev = device_create(dev_class, NULL, maj_min, NULL, "checkers");
    if (IS_ERR(dev)) {
        class_destroy(dev_class);
        goto failure;
    }

    return 0;

failure:
    unregister_chrdev(major, "checkers");
    printk(KERN_ALERT "Failed to register checkers character device.\n");
    return -1;
}

void cleanup_module(void) {
    device_destroy(dev_class, maj_min);
    class_destroy(dev_class);
    unregister_chrdev(MAJOR(maj_min), "checkers");
}
