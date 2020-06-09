#include <linux/kernel.h>	/*	We're doing kernel work		*/
#include <linux/module.h>	/*	Specifically, a module		*/
#include <linux/init.h>

void cleanup_module() {
	pr_info("Short is the life of a kernel module");
}

MODULE_LICENSE("GPL");

/*
 * root@deb:/home/debby/develop/kernel/hello-1# vim hello-5.c
 * root@deb:/home/debby/develop/kernel/hello-1# make clean
 * make -C /lib/modules/4.19.0-9-amd64/build M=/home/debby/develop/kernel/hello-1 clean
 * make[1]: Entering directory '/usr/src/linux-headers-4.19.0-9-amd64'
 * make[1]: Leaving directory '/usr/src/linux-headers-4.19.0-9-amd64'
 * root@deb:/home/debby/develop/kernel/hello-1# make
 * make -C /lib/modules/4.19.0-9-amd64/build M=/home/debby/develop/kernel/hello-1 modules
 * make[1]: Entering directory '/usr/src/linux-headers-4.19.0-9-amd64'
 *   CC [M]  /home/debby/develop/kernel/hello-1/hello-1.o
 *     CC [M]  /home/debby/develop/kernel/hello-1/hello-2.o
 *       CC [M]  /home/debby/develop/kernel/hello-1/hello-3.o
 *         CC [M]  /home/debby/develop/kernel/hello-1/hello-4.o
 *           CC [M]  /home/debby/develop/kernel/hello-1/hello-5.o
 *             Building modules, stage 2.
 *               MODPOST 5 modules
 *                 CC      /home/debby/develop/kernel/hello-1/hello-1.mod.o
 *                   LD [M]  /home/debby/develop/kernel/hello-1/hello-1.ko
 *                     CC      /home/debby/develop/kernel/hello-1/hello-2.mod.o
 *                       LD [M]  /home/debby/develop/kernel/hello-1/hello-2.ko
 *                         CC      /home/debby/develop/kernel/hello-1/hello-3.mod.o
 *                           LD [M]  /home/debby/develop/kernel/hello-1/hello-3.ko
 *                             CC      /home/debby/develop/kernel/hello-1/hello-4.mod.o
 *                               LD [M]  /home/debby/develop/kernel/hello-1/hello-4.ko
 *                                 CC      /home/debby/develop/kernel/hello-1/hello-5.mod.o
 *                                   LD [M]  /home/debby/develop/kernel/hello-1/hello-5.ko
 *                                   make[1]: Leaving directory '/usr/src/linux-headers-4.19.0-9-amd64'
 *                                   root@deb:/home/debby/develop/kernel/hello-1# insmod hello-5.ko mystring="bebop" mybyte=243 myintArray=-1
 *                                   root@deb:/home/debby/develop/kernel/hello-1# lsmod | grep hello
 *                                   hello_5                16384  0
 *                                   root@deb:/home/debby/develop/kernel/hello-1# rmmod hello_5
 *                                   root@deb:/home/debby/develop/kernel/hello-1# journalctl --since "1 hour ago" | grep kernel
 *                                   Jun 09 03:55:16 deb kernel: Hello, world 2
 *                                   Jun 09 03:55:30 deb kernel: Goodbye, world 2
 *                                   Jun 09 03:56:27 deb kernel: Hello, world 3
 *                                   Jun 09 03:56:50 deb kernel: Goodbye, world 3
 *                                   Jun 09 04:07:39 deb kernel: Hello, world 4
 *                                   Jun 09 04:07:46 deb kernel: Goodbye, world 4
 *                                   Jun 09 04:41:00 deb kernel: hello_5: unknown parameter 'mybyte' ignored
 *                                   Jun 09 04:41:00 deb kernel: Hello, world 5
 *                                   Jun 09 04:41:00 deb kernel: myshort is a short integer: 1
 *                                   Jun 09 04:41:00 deb kernel: myint is an integer: 42
 *                                   Jun 09 04:41:00 deb kernel: mylong is a long integer: 9999
 *                                   Jun 09 04:41:00 deb kernel: mystring is a string: bebop
 *                                   Jun 09 04:41:00 deb kernel: myintArray[0] = -1
 *                                   Jun 09 04:41:00 deb kernel: myintArray[1] = -1
 *                                   Jun 09 04:41:00 deb kernel: got 1 arguments for myintArray.
 *                                   Jun 09 04:41:21 deb kernel: Goodbye, world 5
 */
