/*
 * chardev.c: Creates a read-only char device that says how many times
 * you've read from the dev file
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/poll.h>
#include <linux/cdev.h>

/*
 * Prototypes - this would normally go in a .h file
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev"   /* Dev name as it appears in /proc/devices   */
#define BUF_LEN 80              /* Max length of the message from the device */

/*
 * Global variables are declared as static, so are global within the file.
 */

static int Major;               /* Major number assigned to our device driver */
static int Device_Open = 0;     /* Is device open?
				                    * Used to prevent multiple access to device */
static char msg[BUF_LEN];       /* The msg the device will give when asked */
static char *msg_Ptr;

static struct class *cls;

static struct file_operations chardev_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &chardev_fops);

	if (Major < 0) {
		pr_alert("Registering char device failed with %d\n", Major);
	        return Major;
	}

	pr_info("I was assigned major number %d.\n", Major);

	cls = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(cls, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME);

	pr_info("Device created on /dev/%s\n", DEVICE_NAME);

	return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
	device_destroy(cls, MKDEV(Major, 0));
	class_destroy(cls);

	/*
	 * Unregister the device
	 */
	unregister_chrdev(Major, DEVICE_NAME);
}

/*
 * Methods
 */

/*
 * Called when a process tries to open the device file, like
 *  "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
	static int counter = 0;

	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	sprintf(msg, "I already told you %d times Hello world!\n", counter++);
	msg_Ptr = msg;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;          /* We're now ready for our next caller */

	/*
	 * Decrement the usage count, or else once you opened the file, you'll
	 * never get get rid of the module.
	 */
	module_put(THIS_MODULE);

	return SUCCESS;
}

/*
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(	struct file *filp,   /* see include/linux/fs.h   */
				char *buffer,        /* buffer to fill with data */
				size_t length,       /* length of the buffer     */
				loff_t * offset)
{
	/*
	 * Number of bytes actually written to the buffer
	 */
	int bytes_read = 0;
	/*
	 * If we're at the end of the message,
	 * return 0 signifying end of file
	 */
	if (*msg_Ptr == 0)
		return 0;

	/*
	 * Actually put the data into the buffer
         */
	while (length && *msg_Ptr) {

	/*
	 * The buffer is in the user data segment, not the kernel
	 * segment so "*" assignment won't work.  We have to use
	 * put_user which copies data from the kernel data segment to
	 * the user data segment.
	 */
		put_user(*(msg_Ptr++), buffer++);
		length--;
		bytes_read++;
	}

	/*
	 * Most read functions return the number of bytes put into the buffer
	 */
	return bytes_read;
}

/*
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	pr_alert("Sorry, this operation isn't supported.\n");
	return -EINVAL;
}

MODULE_LICENSE("GPL");

/*
6-3	As discussed earlier, char devices are accessed through device files, usually located in /dev. This is by convention. When writing a driver, it\'s OK to put the device file in your current directory. Just make sure you place it in /dev for a production driver. The major number tells you which driver handles which device file. The minor number is used only by the driver itself to differentiate which device it\'s operating on, just in case the driver handles more than one device.

Adding a driver to your system means registering it with the kernel. This is synonymous with assigning it a major number during the module\'s initialization. You do this by using the register_chrdev function, defined by linux/fs.h.


int register_chrdev(unsigned int major, const char *name, struct proc_ops *fops);


where unsigned int major is the major number you want to request, const char *name is the name of the device as it\'ll appear in /proc/devices and struct proc_ops *fops is a pointer to the proc_ops table for your driver. A negative return value means the registration failed. Note that we didn\'t pass the minor number to register_chrdev. That\'s because the kernel doesn\'t care about the minor number; only our driver uses it.


Now the question is, how do you get a major number without hijacking one that\'s already in use? The easiest way would be to look through Documentation /devices.txt and pick an unused one. That\'s a bad way of doing things because you\'ll never be sure if the number you picked will be assigned later. The answer is that you can ask the kernel to assign you a dynamic major number.


If you pass a major number of 0 to register_chrdev, the return value will be the dynamically allocated major number. The downside is that you can\'t make a device file in advance, since you don\'t know what the major number will be. There are a couple of ways to do this. First, the driver itself can print the newly assigned number and we can make the device file by hand. Second, the newly registered device will have an entry in /proc/devices, and we can either make the device file by hand or write a shell script to read the file in and make the device file. The third method is we can have our driver make the the device file using the device_create function after a successful registration and device_destroy during the call to cleanup_module. ::: :::


[6.4] Unregistering A Device {#org86cd4ec}
	We can\'t allow the kernel module to be rmmod\'ed whenever root feels like it. If the device file is opened by a process and then we remove the kernel module, using the file would cause a call to the memory location where the appropriate function (read/write) used to be. If we\'re lucky, no other code was loaded there, and we\'ll get an ugly error message. If we\'re unlucky, another kernel module was loaded into the same location, which means a jump into the middle of another function within the kernel. The results of this would be impossible to predict, but they can\'t be very positive.

Normally, when you don\'t want to allow something, you return an error code (a negative number) from the function which is supposed to do it. With cleanup_module that\'s impossible because it\'s a void function. However, there\'s a counter which keeps track of how many processes are using your module. You can see what it\'s value is by looking at the 3rd field of /proc/modules. If this number isn\'t zero, rmmod will fail. Note that you don\'t have to check the counter from within cleanup_module because the check will be performed for you by the system call sys_delete_module, defined in linux/module.c. You shouldn\'t use this counter directly, but there are functions defined in linux/module.h which let you increase, decrease and display this counter:

try_module_get(THIS_MODULE): Increment the use count.
			     module_put(THIS_MODULE): Decrement the use count.
						      It\'s important to keep the counter accurate; if you ever do lose track of the correct usage count, you\'ll never be able to unload the module; it\'s now reboot time, boys and girls. This is bound to happen to you sooner or later during a module\'s development. ::: :::

[6.5] chardev.c {#org3dd4817}

The next code sample creates a char driver named chardev. You can cat its device file.

						      cat /proc/devices

						      
(or open the file with a program) and the driver will put the number of times the device file has been read from into the file. We don\'t support writing to the file (like echo \"hi\" > /dev/hello), but catch these attempts and tell the user that the operation isn\'t supported. Don\'t worry if you don\'t see what we do with the data we read into the buffer; we don\'t do much with it. We simply read in the data and print a message acknowledging that we received it.

*/



/*
 *
root@deb:/home/debby/develop/kernel/device_drivers-2# insmod chardev-3.ko 
root@deb:/home/debby/develop/kernel/device_drivers-2# lsmod | grep chardev-3
root@deb:/home/debby/develop/kernel/device_drivers-2# lsmod | grep chardev
chardev_3              16384  0
root@deb:/home/debby/develop/kernel/device_drivers-2# rmmod chardev_3 
root@deb:/home/debby/develop/kernel/device_drivers-2# journalctl --since "1 hour ago" | grep "kernel"
Jun 10 03:04:00 deb kernel: chardev_3: loading out-of-tree module taints kernel.
Jun 10 03:04:00 deb kernel: chardev_3: module verification failed: signature and/or required key missing - tainting kernel
Jun 10 03:04:00 deb kernel: I was assigned major number 244.
Jun 10 03:04:00 deb kernel: Device created on /dev/chardev

*/
