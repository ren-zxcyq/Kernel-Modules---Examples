/*
 * hello-2.c	-	Demo the module_init() & module_exit() macros.
 * This is preferred over using init_module() & cleanup_module().
 */
#include <linux/module.h>	/*	Needed by all modules	*/
#include <linux/kernel.h>	/*	Needed for KERN_INFO	*/
#include <linux/init.h>		/*	Needed for the macros	*/

static int __init hello_2_init(void) {
	pr_info("Hello, world 2\n");
	return 0;
}

static void __exit hello_2_exit(void) {
	pr_info("Goodbye, world 2\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);

MODULE_LICENSE("GPL");


/*
 *	linux/drivers/char/Makefile	contains a real world example of a Makefile
 */
