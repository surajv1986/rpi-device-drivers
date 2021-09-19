#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/kthread.h>
#include<linux/wait.h>

uint32_t read_count = 0;
static struct task_struct *wait_thread;

DECLARE_WAIT_QUEUE_HEAD(wait_queue_etx);

dev_t dev = 0;

static struct class *dev_class;
static struct cdev etx_cdev;
int wait_queue_flag = 0;

/* Function Prototypes */
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);

static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);

/* File operations structure */
static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.read = etx_read,
	.write = etx_write,
	.open = etx_open,
	.release = etx_release,
};

/* Thread function i.e task */
static int wait_function(void *unused)
{
	while(1) {
		pr_info("Waiting for event...\n");
		wait_event_interruptible(wait_queue_etx, wait_queue_flag != 0);
		if (wait_queue_flag == 2) {
			pr_info("Event came from the exit function\n");
			return 0;
		}
		pr_info("Event came from read function %d\n", ++read_count);
		wait_queue_flag = 0;
	}
	do_exit(0);

	return 0;
}

/* This function will be called when open the device file  */
static int etx_open(struct inode *inode, struct file *file)
{
	pr_info("Device file opened.....\n");
	return 0;
}

/* This function will be called when we close the device file */
static int etx_release(struct inode *inode, struct file *file)
{
	pr_info("Device file closed....\n");
	return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	pr_info("Read Function\n");
	wait_queue_flag = 1;
	wake_up_interruptible(&wait_queue_etx);

	return 0;
}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
	pr_info("Write function\n");
	return len;
}

static int __init etx_driver_init(void)
{
	/* Allocating major number */
	if ((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) < 0) {
		pr_info("Cannot allocate major number\n");
		return -1;
	}
	pr_info("Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

	/* Creating cdev structure */
	cdev_init(&etx_cdev, &fops);
	etx_cdev.owner = THIS_MODULE;
	etx_cdev.ops = &fops;

	/* Adding character device to the system */
	if ((cdev_add(&etx_cdev, dev, 1)) < 0) {
		pr_info("Cannot add the device o the system\n");
		goto r_class;
	}
	
	/* Creating struct class */
	if ((dev_class = class_create(THIS_MODULE, "etx_class")) == NULL) {
		pr_info("Cannot create the struct class\n");
		goto r_class;
	}

	/* Creating device */
	if ((device_create(dev_class, NULL, dev, NULL, "etx_device")) == NULL) {
		pr_info("Cannot create the device\n");
		goto r_device;
	}

	/* Create kernel thread with name 'mythread' */
	wait_thread = kthread_create(wait_function, NULL, "WaitThread");
	if (wait_thread) {
		pr_info("Thread created successfully\n");
		wake_up_process(wait_thread);
	} else
		pr_info("Thread creation failed\n");

	pr_info("Device driver insert ... Done!!!\n");

	return 0;


r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev, 1);
	return -1;
}

static void __exit etx_driver_exit(void)
{
	wait_queue_flag = 2;
	wake_up_interruptible(&wait_queue_etx);
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&etx_cdev);
	unregister_chrdev_region(dev, 1);	
	pr_info("Device driver Remove... Done !!!\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embetronicx <embetronicx@gmail.com>");
MODULE_DESCRIPTION("Simple Linux driver (Waitqueue static method)");
MODULE_VERSION("1.7");

