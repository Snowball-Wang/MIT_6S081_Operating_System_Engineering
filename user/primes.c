#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
pipeline(int *p)
{
    int pchild[2];
    pipe(pchild);
    int prime;

    // if process reads no more data from pipe, just exits
    if(read(p[0], &prime, sizeof(prime)) == 0)
        exit(0);

    if(fork()) // main process filters the primes
    {
        int n;
        printf("prime: %d\n", prime);
        while(read(p[0], &n, sizeof(n)))
        {
            if(n % prime != 0)
                write(pchild[1], &n, sizeof(n));
        }
        close(p[0]);      // close parent pipe read fd
        close(p[1]);      // close parent pipe write fd
        close(pchild[1]); // close child pipe write fd
        wait(0);
    }
    else // spawn child process to read from filtered prime numbers
    {
        pipeline(pchild);
    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    // main process feeds number 2-35
    if(fork())
    {
        for(int i = 2; i <= 35; i++)
        {
            write(p[1], &i, sizeof(int));
        }
        close(p[0]);
        close(p[1]);
        wait(0);
    }
    else // child process starts the pipeline
    {
        pipeline(p);
    }

    exit(0);
}