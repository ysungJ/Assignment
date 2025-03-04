#include <asm/io.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/ioport.h>

MODULE_LICENSE("GPL");

#define base_lwFPGA 0xFF200000
#define len_lwFPGA 0x00200000

#define addr_SW 0x40

#define SW_DEVMAJOR 238
#define SW_DEVNAME  "sw"

static int sw_open(struct inode* minode, struct file* mfile);
static int sw_release(struct inode* minode, struct file* mfile);
static int sw_write(struct inode* minode, struct file* mfile);
static ssize_t sw_read(struct file* file, char __user* buf, size_t count, loff_t* f_ops);
static int __init sw_init(void);
static void __exit sw_exit(void);

static struct file_operations sw_fops;
static void* mem_base;
static void* sw_addr;

// open operation
static int sw_open(struct inode* minode, struct file* mfile)
{
    // do nothing
    return 0;
}

// release operation
static int sw_release(struct inode* minode, struct file* mfile)
{
    // do nothing
    return 0;
}

// write operation
static int sw_write(struct inode* minode, struct file* mfile)
{
    // do nothing
    return 0;
}

// read operation
// read data from sw and store it in the buffer
static ssize_t sw_read(struct file* file, char __user* buf, size_t count, loff_t* f_ops)
{
    unsigned int sw_data = 0;
    int ret;

    sw_data = ioread32(sw_addr);

    ret = copy_to_user(buf, &sw_data, count);
    if (ret) {
        return -EFAULT;
    }

    return sizeof(sw_data);
}

// file operation initialize
static struct file_operations sw_fops = {
    .read = sw_read,
    .write = sw_write,
    .open = sw_open,
    .release = sw_release
};

// driver init
static int __init sw_init(void)
{
    int res;

    res = register_chrdev(SW_DEVMAJOR, SW_DEVNAME, &sw_fops);

    if (res < 0) {
        printk(KERN_ERR "sw: failed to register device.\n");
        return res;
    }

    // mapping address
    mem_base = ioremap_nocache(base_lwFPGA, len_lwFPGA);

    if (!mem_base) {
        printk("Error mapping memory.\n");
        release_mem_region(base_lwFPGA, len_lwFPGA);
        return -EBUSY;
    }

    sw_addr = mem_base + addr_SW;

    printk("Device: %s Major: %d\n", SW_DEVNAME, SW_DEVMAJOR);
    return 0;
}

static void __exit sw_exit(void)
{
    unregister_chrdev(SW_DEVMAJOR, SW_DEVNAME);
    printk("%s unregistered.\n", SW_DEVNAME);
    iounmap(mem_base); // virtual mem release
}

module_init(sw_init);
module_exit(sw_exit);