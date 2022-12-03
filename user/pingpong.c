#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    // create a pair of pipes
    int p[2];
    pipe(p);

    // some random character
    char c = 'a';

    // child process
    if(fork() == 0)
    {
        read(p[0], &c, sizeof(c));
        printf("%d: received ping\n", getpid());
        write(p[1], &c, sizeof(c));
        close(p[0]);
        close(p[1]);
        exit(0);
    }
    else // parent process
    {
        write(p[1], &c, sizeof(c));
        wait(0);
        read(p[0], &c, sizeof(c));
        printf("%d: received pong\n", getpid());
        close(p[0]);
        close(p[1]);    
    }

    exit(0);
}