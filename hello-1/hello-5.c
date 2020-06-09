/*
 * 	hello-5.c	-	Passing Command Line Arguments to a Module
 */

/*
 *
 *
 *
 * 			Modules can take cmd line args but not with the argc/argv struct
 *
 * 			To allow args to be passed to the module	=>	declare variables that will take
 * 										the values as global
 * 										=>	use MACRO => module_param()
 *
 * 		linux/moduleparam.h
 * 				at runtime insmod will fill the variables with any cmd line args
 *
 * 					./insmode mymodule.ko myvariable=5
 *
 * 				MACRO	module_param takes 3 arguments:	-	name
 * 										type
 * 										permissions for the file in sysfs
 * 						for		arrays
 * 										module_param_array()
 * 										module_param_string()
 *
 *
 *
 *
 *			int myint = 3;
 *			module_param(myint, int, 0);
 *
 *			int myintarray[2];
 *			module_param_array(myintarray, int, NULL, 0);		*	not interested in count		*
 *
 *			short myshortarray[4];
 *			int count;
 *			module_param_array(myshortarray, short, &count, 0);	*	put count into "count" variable *
 */

/*
 * A good use for this is to have the module variable's default set, like an port or an IO address.
 * If the variables contain the default values, then perform autodetection (explained elsewhere).
 * Otherwise, keep the current value. This will be made clear later on.
 *
 *
 *
 *
 *
 *
 *	ALSO THERE IS A MACRO
 *
 * MODULE_PARM_DESC()
 *
 *
 * 				document args that the module can take.
 *
 *
 *
 *
 *				takes 2 params:
 *
 * 					@variable name
 * 					@free form string describing that variable
 *
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zxcyq");

static short int myshort = 1;
static int myint = 42;
static long int mylong = 9999;
static char *mystring = "blah";
static int myintArray[2] = {-1, -1};
static int arr_argc = 0;

/*
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second is it's data type
 * The final arg is the permissions bits,
 * for exposing parameters in sysfs (if non-zero) at a later stage.
 */

module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");

/*
 * module_param_array(name, type, num, perm);
 * first param is the parameter's (in this case the array's) name
 * second data type of the elements of the array
 * third is a pointer to the variable that will store the number
 * 	of elements of the array INITIALIZED by the user at module loading time
 * The fourth argument is the permission bits
 *
 */
module_param_array(myintArray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintArray, "An array of integers");

static int __init hello_5_init(void) {
	int i;
	pr_info("Hello, world 5\n==================\n");
	pr_info("myshort is a short integer: %hd\n", myshort);
	pr_info("myint is an integer: %d\n", myint);
	pr_info("mylong is a long integer: %ld\n", mylong);
	pr_info("mystring is a string: %s\n", mystring);

	for (i=0; i < (sizeof myintArray / sizeof (int)); i++)
	pr_info("myintArray[%d] = %d\n", i, myintArray[i]);

	pr_info("got %d arguments for myintArray.\n", arr_argc);
	return 0;
}

static void __exit hello_5_exit(void) {
	pr_info("Goodbye, world 5\n");
}

module_init(hello_5_init);
module_exit(hello_5_exit);






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
