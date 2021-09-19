#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/proc_fs.h>

#define LINUX_KERNEL_VERSION 510

#define WR_VALUE _IOW('a', 'a', int32_t*)
#define RD_VALUE _IOR('a', 'b', int32_t*)


int32_t value = 0;
char etx_array[20] = "try_proc_array\n";
static int len = 1;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
static struct proc_dir_entry *parent;

/* Function prototypes  */
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);


/* Procfs functions  */
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
/* Driver Functions*/
static int open_proc(struct inode *inode, struct file *file);
static int release_proc(struct inode *inode, struct file *file);
static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length, loff_t *offset);
static ssize_t write_proc(struct file *filp, const char *buff, size_t len, loff_t *off);

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.read = etx_read,
	.write = etx_write,
	.open = etx_open,
	.unlocked_ioctl = etx_ioctl,
	.release = etx_release,

};

#if (LINUX_KERNEL_VERSION > 505)

static struct proc_ops proc_ops = {
	.proc_open = open_proc,
	.proc_read = read_proc,
	.proc_write = write_proc,
	.proc_release = release_proc

};

#else

static struct file_operations proc_fops = {
	.open = open_proc,
	.read = read_proc,
	.write = write_proc,
	.release = release_proc

};

#endif


static int open_proc(struct inode *inode, struct file *file)
{
	pr_info("proc file opened.....\t");
	return 0;

}

static int release_proc(struct inode *inode, struct file *file)
{
	pr_info("proc file released......\n");
	return 0;
}

static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
	pr_info("proc file read .....\n");
	if (len) {
		len = 0;
	} else {
		len = 1;
		return 0;
	}

	if (copy_to_user(buffer, etx_array, 20)) {
		pr_err("Data Send: Err\n");
	
	}

	return length;
}

static ssize_t write_proc(struct file *filp, const char *buff, size_t len, loff_t *off)
{
	pr_info("proc file wrote....\n");
	
	if (copy_from_user(etx_array, buff, len)) {
		pr_err("Data write: Err\n ");
	}	

	return len;
}

static int etx_open(struct inode *inode, struct file *file) 
{
	pr_info("Device file opened...\n");
	return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
	pr_info("Device File closed....!\n");
	return 0;

}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	pr_info("Read function.....\n");
	return 0;

}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
	pr_info("Write Function.....");
	return len;
}

static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd) {
		case WR_VALUE:
			if (copy_from_user(&value, (int32_t*) arg, sizeof(value)))
			{
				pr_err("Data Read: Err\n");

			
			}
			break;
		case RD_VALUE:
			if (copy_to_user((int32_t *) arg, &value, sizeof(value)))
			{
				pr_err("Data Read : Err\n");
			}
			break;
		default:
			pr_info("Default\n");
			break;
	
	}
	return 0;
}

static int __init etx_driver_init(void)
{
	/* Allocating major number */
	if (alloc_chrdev_region(&dev, 0, 1, "etx_Dev") < 0) {
		pr_info("Cannot allocate major number\n");
		return -1;
	
	}
	pr_info("Major = %d, Minor = %d\n", MAJOR(dev), MINOR(dev));

	/* Creating cdev structure */
	cdev_init(&etx_cdev, &fops);


	/* Adding character device to the system */
	if ((cdev_add(&etx_cdev, dev, 1) < 0)) {
		pr_info("Cannot add the device to the system\n");
		goto r_class;
	}


	/* Creating struct class */
	if ((dev_class = class_create(THIS_MODULE, "etx_class")) == NULL) {
		pr_info("Cannot create the struct class\n");
		goto r_class;
	}

	/* Creating device */
	if (device_create(dev_class, NULL, dev, NULL, "etx_device") == NULL) {
		pr_info("Cannot create the device 1\n");
		goto r_device;
	}

	parent = proc_mkdir("etx", NULL);

	if (parent == NULL)
	{
		pr_info("Error creating entry");
		goto r_device;

	}

	if (parent == NULL) {
		pr_info("Error creating proc entry");
		goto r_device;
	}
	proc_create("etx_proc", 0666, parent, &proc_ops);

	pr_info("Device Driver Insert ... Done!!!\n");
	return 0;
r_device:
	class_destroy(dev_class);

r_class:
	unregister_chrdev_region(dev, 1);
	return -1;


}

static void __exit etx_driver_exit(void)
{
	proc_remove(parent);

	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&etx_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("Device Driver Remove...Done!!!\n");

}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embetronicx <embetronicx@gmail.com>");
MODULE_DESCRIPTION("Simple Linux device driver(profs)");
MODULE_VERSION("1.6");
