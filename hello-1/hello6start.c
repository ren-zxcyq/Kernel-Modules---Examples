/*
 * 	start.c		-	Illustration of multi filled modules
 *
 *
 * 					i.e., constructing a module which consists of 2 c files.
 * 					hello6start.c
 * 					hello6end.c
 */
#include <linux/kernel.h>	/*	We're doing kernel work		*/
#include <linux/module.h>	/*	Specifically, a module		*/
#include <linux/init.h>

int init_module() {
	pr_info("Hello, world - This is the kernel module consisting of 2 files - speaking\n");
	return 0;
}

MODULE_LICENSE("GPL");



/*
 *
 *	In our Makefile we declare the module slightly differently.
 *
 *	...
 *	obj-m += hello-5.o
 *	obj-m += hello6starthello6end.o
 *	hello6starthello6end-objs := hello6start.o hello6end.o
 *	...
 */
