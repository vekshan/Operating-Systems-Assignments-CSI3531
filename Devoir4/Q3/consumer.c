/**
 * This code is based on the file shm-posix-consumer.c.doc on brightspace.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
    /* name of shared memory */
	const char *name = "OS";
	const int SIZE = 4096;

     /* file descriptor for fd */
	int shm_fd;
	void *ptr;
	int i;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* memory map the shared memory */
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	/* read from shared memory */
	printf("Here is what is read from shared memory:\n%s\n",(char*)ptr);

	/* remove shared memory */
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}

	return 0;
}