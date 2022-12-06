#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
my_readline(int fd, char *buf)
{
	char ch;
	int count = 0;
	while(read(fd, &ch, sizeof(ch)))
	{
		if(ch == '\n')
			break;
		buf[count++] = ch;
	}
	buf[count] = '\0';
	return count;
}

int
main(int argc, char *argv[])
{
    char *cmd;
    char *my_argv[MAXARG];
    char buf[512];

    if(argc < 2){
        fprintf(2, "Usage: xargs needs one more parameter\n");
        exit(1);
    }

    // parse arguments following xargs
    cmd = argv[1];
    for(int i = 1; i < argc; i++){
        my_argv[i-1] = argv[i];
    }

    // read from input and add the input to argv list
    while(my_readline(0, buf)){
        my_argv[argc - 1] = buf;
        if(fork() == 0)
            exec(cmd, my_argv);
        else
            wait((int *)0);
        // remember to zero buf
        memset(buf, 0, sizeof(buf));
    }

    exit(0);
}