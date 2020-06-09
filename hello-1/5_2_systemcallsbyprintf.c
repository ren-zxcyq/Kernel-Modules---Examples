/*
 * 	Demonstration of	strace ./filename
 *
 * 		To show syscall behind printf()
 */

#include <stdio.h>

int main(void) {
	printf("Hello");
	return 0;
}

/*
 *	gcc -Wall -o outfile dotcfile.c
 *
 *
 *	strace ./outfile
 *
 * 		towards the end:
 *			write(1,"Hello",5Hello)	->	syscall
 *
 *
 *	man 2 write		shows us the man page for it
 *
 *
 *
 *
 *
 *	man 2		syscalls	(like kill() and read())
 *	man 3		libcalls	(like cosh() and random())
 *
 *
 *
 *	CAN WRITE MODULES TO REPLACE KERNEL's SYSTEM CALLS
 *
 *
 *		-> add complex functionality
 *		or
 *		-> write Tee hee whenever someone tries to delete a file on the system
 *
 */
