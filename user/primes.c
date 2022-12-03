#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
pipeline(int fd_read, int num)
{
    int pd[2];
    pipe(pd);
    int i;

    if(fork() == 0)
    {
        while(read(fd_read, &i, sizeof(i)))
        {
            if(i % num != 0)
                write(pd[1], &i, sizeof(i));
        }
        close(pd[0]);
        close(pd[1]);
        exit(0);
    }
    wait(0);
    close(pd[1]);
    return pd[0];
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    int prime;
    int fd_read = p[0];
    
    // the main process feeds number 2-35
    for(int i = 2; i <= 35; i++)
    {
        write(p[1], &i, sizeof(int));
    }
    close(p[1]);

    while(read(fd_read, &prime, sizeof(prime)))
    {
        printf("prime %d\n", prime);
        fd_read = pipeline(fd_read, prime);
    }

    exit(0);
}