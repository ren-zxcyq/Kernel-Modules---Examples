/*
 *	First attempt to build a Kernel Module
 */

#include <linux/module.h>	/*	Needed by all Kernel Modules	*/
#include <linux/kernel.h>	/*	Needed for KERN_INFO		*/

int init_module(void) {
	pr_info(KERN_INFO "Hello World 1.\n");

	/*
	 *	A non 0 return means	init_module failed; module can't be loaded
	 */
	return 0;
}

void cleanup_module(void) {
	pr_info(KERN_INFO "Goodbye World 1.\n");
}

MODULE_LICENSE("GPL");

/*
 *	Modules MUST have 2 functions:		- "start"	init_module()	on insmod
 *						- "end"		cleanup_module() on rmmod
 *	include		linux/module.h		
 *			linx/kernel.h		only for macro expansion of	pr_alert() log_level, KERN_ALERT
 *	printk()	was not meant to comm info to the user even thought we used it here for that very reason
 *	
 *	****pr_info() REPLACED printk()
 *
 *						it used to be printk followed by a priority
 *							such as:	KERN_INFO
 *									KERN_DEBUG
 *					NOW:
 *						pr_info()
 *						pr_debug()
 *
 *								| they both live in linux/printk.h
 *
 *
 *
 *
 *
 *			it is meant to log info
 *		kernel has 8 LOG LEVELS		->	linux/kernel.h
 *			declared when using printk()		<1> and KERN_ALERT
 *		DEFAULT_MESSAGE_LOGLEVEL will be used if we don't declare a priority
 *			always use the MACRO instead of the Number, like	KERN_WARNING
 *
 *
 *		IF	(prio < int console_loglevel)	{
 *					the message is printed on your current terminal
 *		IF BOTH		(syslogd && klogd == are running) {
 *					message will also be APPENDED TO:	/var/log/messages
 *
 *		here we use a high priority -> KERN_ALERT	to make sure the printk() messages are printed to the console
 *
 */

/*
 *
 * After installing the linux-headers-$(uname -r) -> /usr/linux/kernel.h
 */



/*
 * 	Now we need a MAKEFILE
 *
 * 	Using the one in MAKEFILE	->		After installing linux-headers-$(uname -r)
 *
 * 	make
 *
 *
 *
 * 	Built our kernel module!
 *
 * 			->	Find it using:			sudo modinfo hello-1.ko
 *
 * debby@deb:~/develop/kernel/hello-1$ su -
 * Password: 
 * root@deb:~# cd /home/debby/develop/kernel/hello-1/
 * root@deb:/home/debby/develop/kernel/hello-1# ls
 * hello-1.c  hello-1.ko  hello-1.mod.c  hello-1.mod.o  hello-1.o  Makefile  modules.order  Module.symvers
 * root@deb:/home/debby/develop/kernel/hello-1# modinfo hello-1.ko
 * filename:       /home/debby/develop/kernel/hello-1/hello-1.ko
 * license:        GPL
 * depends:        
 * retpoline:      Y
 * name:           hello_1
 * vermagic:       4.19.0-9-amd64 SMP mod_unload modversions 
 */

/*
 * root@deb:/home/debby/develop/kernel/hello-1# lsmod | grep hello
 * root@deb:/home/debby/develop/kernel/hello-1# insmod hello-1.ko 
 * root@deb:/home/debby/develop/kernel/hello-1# lsmod | grep hello
 * hello_1                16384  0
 * root@deb:/home/debby/develop/kernel/hello-1# rmmod hello_1 
 * root@deb:/home/debby/develop/kernel/hello-1# journalctl --since "1 hour ago" | grep kernel
 * Jun 09 02:42:12 deb sudo[7322]:    debby : user NOT in sudoers ; TTY=pts/2 ; PWD=/home/debby/develop/kernel/hello-1 ; USER=root ; COMMAND=/usr/sbin/modinfo hello-1.ko
 * Jun 09 02:59:03 deb sudo[9861]:    debby : user NOT in sudoers ; TTY=pts/2 ; PWD=/home/debby/develop/kernel/hello-1 ; USER=root ; COMMAND=/usr/sbin/modinfo hello-1.ko
 * Jun 09 02:59:11 deb sudo[9863]:    debby : user NOT in sudoers ; TTY=pts/2 ; PWD=/home/debby/develop/kernel/hello-1 ; USER=root ; COMMAND=/usr/sbin/modinfo hello-1.ko
 * Jun 09 03:02:14 deb kernel: hello_1: loading out-of-tree module taints kernel.
 * Jun 09 03:02:14 deb kernel: hello_1: module verification failed: signature and/or required key missing - tainting kernel
 * Jun 09 03:02:14 deb kernel: Hello World 1.
 * Jun 09 03:02:29 deb kernel: Goodbye World 1.
 *
 */




/*
 * In the old days, Kernel module compile settings were found in Makefiles, but many redundant
 * settings accumulated in sublevel Makefiles nad made them large and rather difficult to maintain
 *
 * NOW:		kbuild		manages these
 *
 *
 * kbuild	the build process for external loadable modules is now fully integrated into the
 * 		standard kernel build mechanism.
 *
 *
 *
 * 		We had to use		init_module
 * 					cleanup_module		functions
 *
 *
 * 		But these days:
 * 					module_init
 * 					module_exit
 * 								macros allow you to define the
 * 								function names
 *
 * 								(They are defined in linux/init.h)
 * 					init and cleanup functions must be defined before
 * 					calling the macros. Check hello-2.c
 *
 *
 *
 * 					We can just append another line of
 * 						obj-m += hello-2.c
 *
 * 						in Makefile to build both modules.
 */
