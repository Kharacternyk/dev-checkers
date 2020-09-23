#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nazar Vinnichuk");

#define BOARD_HEIGHT 8
#define BOARD_WIDTH  8
#define BOARD_SIZE (BOARD_WIDTH+1)*BOARD_HEIGHT
#define BLACK_FIELD ' '
#define WHITE_FIELD '#'

static char board[BOARD_SIZE];

static void init_board(void) {
    size_t i;

    for (i = BOARD_WIDTH; i < BOARD_SIZE; i += BOARD_WIDTH + 1) {
        board[i] = '\n';
    }
    for (i = 0; i < BOARD_SIZE; i += 2) {
        if (!board[i]) {
            board[i] = BLACK_FIELD;
        }
    }
    for (i = 1; i < BOARD_SIZE; i += 2) {
        if (!board[i]) {
            board[i] = WHITE_FIELD;
        }
    }
}

static ssize_t read(struct file *file, char *buffer, size_t size, loff_t *offset) {
    ssize_t length = BOARD_SIZE - *offset;
    if (board[0] == '\0') {
        init_board();
    }
    if (length <= 0) {
        return 0;
    }
    if (size < length) {
        length = size;
    }
    if (copy_to_user(buffer, board + *offset, length)) {
        return -EFAULT;
    }
    *offset += length;
    return length;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
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
