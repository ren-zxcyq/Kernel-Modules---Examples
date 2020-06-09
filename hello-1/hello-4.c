/*
 * 	hello-4.c	-	Demonstrate Module Documentation
 *
 *
 * 	journalctl --since "1 hour ago" | grep kernel
 *
 *
 * 			Shows us that xxxxx.ko will break the kernel: no license
 *
 *
 * 			Macros can be used to define module documentation and licensing
 *
 *
 */
#include <linux/module.h>	/*	Needed by all modules	*/
#include <linux/kernel.h>	/*	Needed for KERN_INFO	*/
#include <linux/init.h>		/*	Needed for macros	*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zxcyq");
MODULE_DESCRIPTION("A sample driver");
MODULE_SUPPORTED_DEVICE("testdevice");

static int __init init_hello_4(void) {
	pr_info("Hello, world 4\n");
	return 0;
}

static void __exit cleanup_hello_4(void) {
       pr_info("Goodbye, world 4\n");
}

module_init(init_hello_4);
module_exit(cleanup_hello_4);






/*
 * root@deb:/home/debby/develop/kernel/hello-1# make
 * make -C /lib/modules/4.19.0-9-amd64/build M=/home/debby/develop/kernel/hello-1 modules
 * make[1]: Entering directory '/usr/src/linux-headers-4.19.0-9-amd64'
 *   CC [M]  /home/debby/develop/kernel/hello-1/hello-1.o
 *     CC [M]  /home/debby/develop/kernel/hello-1/hello-2.o
 *       CC [M]  /home/debby/develop/kernel/hello-1/hello-3.o
 *         CC [M]  /home/debby/develop/kernel/hello-1/hello-4.o
 *           Building modules, stage 2.
 *             MODPOST 4 modules
 *               CC      /home/debby/develop/kernel/hello-1/hello-1.mod.o
 *                 LD [M]  /home/debby/develop/kernel/hello-1/hello-1.ko
 *                   CC      /home/debby/develop/kernel/hello-1/hello-2.mod.o
 *                     LD [M]  /home/debby/develop/kernel/hello-1/hello-2.ko
 *                       CC      /home/debby/develop/kernel/hello-1/hello-3.mod.o
 *                         LD [M]  /home/debby/develop/kernel/hello-1/hello-3.ko
 *                           CC      /home/debby/develop/kernel/hello-1/hello-4.mod.o
 *                             LD [M]  /home/debby/develop/kernel/hello-1/hello-4.ko
 *                             make[1]: Leaving directory '/usr/src/linux-headers-4.19.0-9-amd64'
 *                             root@deb:/home/debby/develop/kernel/hello-1# lsmod |grep hello
 *                             root@deb:/home/debby/develop/kernel/hello-1# insmod hello-4.ko
 *                             root@deb:/home/debby/develop/kernel/hello-1# rmmod hello_4 
 *                             root@deb:/home/debby/develop/kernel/hello-1# journalctl --since "1 hour ago" | grep kernel
 *                             Jun 09 03:55:16 deb kernel: Hello, world 2
 *                             Jun 09 03:55:30 deb kernel: Goodbye, world 2
 *                             Jun 09 03:56:27 deb kernel: Hello, world 3
 *                             Jun 09 03:56:50 deb kernel: Goodbye, world 3
 *                             Jun 09 04:07:39 deb kernel: Hello, world 4
 *                             Jun 09 04:07:46 deb kernel: Goodbye, world 4
 *                             root@deb:/home/debby/develop/kernel/hello-1# modinfo hello-4.ko
 *                             filename:       /home/debby/develop/kernel/hello-1/hello-4.ko
 *                             description:    A sample driver
 *                             author:         zxcyq
 *                             license:        GPL
 *                             depends:        
 *                             retpoline:      Y
 *                             name:           hello_4
 *                             vermagic:       4.19.0-9-amd64 SMP mod_unload modversions
 */
