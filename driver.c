/*
 *
 *
 * */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

/* Memory Size */
#define mem_size 1024

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
uint8_t *kernel_buffer;

/* Function prototypes */
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);

/* File operations structure */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = etx_read,
	.write = etx_write,
	.open = etx_open,
	.release = etx_release,
};

static int etx_open(struct inode *inode, struct file *file)
{
	/* Creating physical memory */
	if ((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0) {
		pr_info("Cannot allocate memory in kernel\n");
		return -1;
	}
	pr_info("Device file opened\n");
	return 0;

}

static int etx_release(struct inode *inode, struct file *file)
{
	kfree(kernel_buffer);
	pr_info("Device File closed...\n");
	return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	if (copy_to_user(buf, kernel_buffer, mem_size)) {
		pr_err("Data read error \n");
	}
	pr_info("Data read: Done!\n");

	return mem_size;

}

static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
	if (copy_from_user(kernel_buffer, buf, len)) {
		pr_err("Data write: Error\n");
	}
	pr_info("Data write : Done");

	return len;
}

static int __init etx_driver_init(void)
{
	if ((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) < 0 ) {
		pr_info("Cannot allocate major number\n");
		return -1;
	}

	pr_info("Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));
	
	/* Creating cdev structure */
	cdev_init(&etx_cdev, &fops);

	/* Adding character device to the system */
	if ((cdev_add(&etx_cdev, dev, 1)) < 0) {
		pr_info("Cannot add the device to the system\n");
		goto r_class;
	
	}

	/* Creating Struct class */
	if ((dev_class = class_create(THIS_MODULE, "etx_class")) == NULL) {
		pr_info("Cannot create the struct class\n");
		goto r_class;
	}


	/* Creating device */
	if ((device_create(dev_class, NULL, dev, NULL, "etx_device")) == NULL) {
		pr_info("Cannot create the Device 1 \n");
		goto r_device;	
	}

	pr_info("Device Driver Insert... Done!!!\n");

	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev, 1);

	return -1;

}

static void __exit etx_driver_exit(void)
{
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&etx_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("Device Driver Remove...Done!!!\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicx<embetronicx@gmail.com>");
MODULE_DESCRIPTION("Simple Linux Device Driver for rpi  \n");
MODULE_VERSION("1.4");

