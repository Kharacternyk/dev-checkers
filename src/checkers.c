#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nazar Vinnichuk");

#define BLACK_FIELD "◞"
#define WHITE_FIELD "█"
#define DARK_STONE  "◕"
#define LIGHT_STONE "◴"
#define BOARD_HEIGHT 8
#define BOARD_WIDTH  8
#define BOARD_SIZE ((BOARD_WIDTH+1)*3)*BOARD_HEIGHT

static char board[BOARD_SIZE];

static void init_board(void) {
    size_t i;

    for (i = BOARD_WIDTH * 3; i < BOARD_SIZE; i += (BOARD_WIDTH + 1) * 3) {
        strncpy(&board[i], "  \n", 3);
    }
    for (i = 0; i < BOARD_SIZE; i += 6) {
        if (!board[i]) {
            strncpy(&board[i], BLACK_FIELD, 3);
        }
    }
    for (i = 3; i < BOARD_SIZE; i += 6) {
        if (!board[i]) {
            strncpy(&board[i], WHITE_FIELD, 3);
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

static ssize_t write(struct file *file, const char *buffer, size_t size,
                     loff_t *offset) {
    size_t read_count = 0;
    while (read_count++ < size) {
        char c;
        get_user(c, buffer);
        c -= '0';
        if (c < BOARD_SIZE) {
            board[(size_t)c] = DARK_STONE;
        }
    }
    return size;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = read,
    .write = NULL
};

static struct miscdevice dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "checkers",
    .nodename = "checkers",
    .fops = &fops,
    .mode = 0666
};

module_misc_device(dev);
