#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *fib(void *param);


/*used to pass pointer to arr and size*/
struct thread_data
{
    int size;
    int* arr;
};

int main(int ac, char **av)
{
    pthread_t tid;
    pthread_attr_t attr;
    if (ac == 2)
    {

        int size = atoi(av[1]);

        if (size < 0)
        {
            fprintf(stderr, "Argument %d doit etre plus grand que 0\n", size); 
            return -1;
        }

        struct thread_data td;
        td.size = size;
        td.arr = (int *)malloc(size * sizeof(int));

        pthread_attr_init(&attr);

        /* create thread */
        pthread_create(&tid, &attr, fib, &td);

        /* wait for thread to exit */
        pthread_join(tid, NULL);

        int i;
        for (i = 0; i < size; i++)
        {
            printf("%d ", td.arr[i]);
        }
        printf("\n");
    }
    else
    {
        fprintf(stderr, "Nombre d'arguments doit etre 2 incluant le nom du programme\n"); 
        return -1;
    }
}

/*thread*/
void *fib(void *param)
{
    struct thread_data *td = (struct thread_data *)param;
    int size = td->size;
    int *arr = td->arr;

    if (size >= 1)
    {
        arr[0] = 0;
    }
    if (size >= 2)
    {
        arr[1] = 1;
    }

    int i;
    for (i = 2; i < size; i++)
    {
        arr[i] = arr[i - 1] + arr[i - 2];
    }

    pthread_exit(0);
}