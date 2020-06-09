/*
 * hello-3.c	-	Illustrating the __init, __initdata and __exit macros.
 */


#include <linux/module.h>	/*	Needed for all modules		*/
#include <linux/kernel.h>	/*	Needed for KERN_INFO		*/
#include <linux/init.h>		/*	Needed for the macros		*/

static int hello3_data __initdata = 3;

static int __init hello_3_init(void) {
	pr_info("Hello, world %d\n", hello3_data);
	return 0;
}

static void __exit hello_3_exit(void) {
	pr_info("Goodbye, world 3\n");
}

module_init(hello_3_init);
module_exit(hello_3_exit);

MODULE_LICENSE("GPL");

/*
 *
 *
 *
 *
 * root@deb:/usr/src/linux-headers-4.19.0-9-common/include/linux# pwd
 * /usr/src/linux-headers-4.19.0-9-common/include/linux
 * root@deb:/usr/src/linux-headers-4.19.0-9-common/include/linux# head -10 init.h
 * * SPDX-License-Identifier: GPL-2.0
 * #ifndef _LINUX_INIT_H
 * #define _LINUX_INIT_H

 * #include <linux/compiler.h>
 * #include <linux/types.h>

 *  Built-in __init functions needn't be compiled with retpoline *
 * #if defined(__noretpoline) && !defined(MODULE)
 * #define __noinitretpoline __noretpoline
 * root@deb:/usr/src/linux-headers-4.19.0-9-common/include/linux# 
 */
