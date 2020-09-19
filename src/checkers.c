#include <linux/module.h>
#include <linux/kernel.h>
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

static int major;
static struct file_operations fops = {
    .open = open,
    .release = release,
    .read = read
};

int init_module(void) {
    if ((major = register_chrdev(0, "checkers", &fops)) < 0) {
        printk(KERN_ALERT "Failed to register checkers character device.\n");
        return -1;
    }

    printk(KERN_INFO "Major number for checkers devices: %d.\n", major);

    return 0;
}

void cleanup_module(void) {
    unregister_chrdev(major, "checkers");
}
