/**
 * This code is based on the file shm-posix-producer.c.doc on brightspace.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int catalan(int n);

int main(int ac, char **av)
{

    if (ac == 2)
    {
        int n = atoi(av[1]);

        if (n < 0)
        {
            fprintf(stderr, "Argument %d doit etre plus grand que 0\n", n);
            return -1;
        }

        /* name of shared memory */
        const char *name = "OS";
        const int SIZE = 4096;

        /* file descriptor for fd */
        int shm_fd;

        /* pointer to shared memory */
        void *ptr;

        /* create the shared memory segment */
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

        /* configure the size of the shared memory segment */
        ftruncate(shm_fd, SIZE);

        /* memory map the shared memory */
        ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED)
        {
            printf("Map failed\n");
            return -1;
        }

        /**
         * Now write to the shared memory region.
         *
         * Note we must increment the value of ptr after each write.
         */

        int i;

        for (i = 1; i <= n; i++)
        {
            char str[20];
            sprintf(str, "%d ", catalan(i));
            sprintf(ptr, "%s", str);
            ptr += strlen(str);
            fprintf(stderr, "Produced C%d: %s\n", i, str);
        }

        return 0;
    }
    else
    {
        fprintf(stderr, "Nombre d'arguments doit etre 2 incluant le nom du programme\n");
        return -1;
    }
}

int catalan(int n)
{
    // Base case
    if (n <= 1)
    {
        return 1;
    }
    //Cn = (2n)!/(n+1)!n!
    int cn = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        cn += catalan(i) * catalan(n - i - 1);
    }
    return cn;
}