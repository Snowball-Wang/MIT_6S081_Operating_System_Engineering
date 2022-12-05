#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *dirname, char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(dirname, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", dirname);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", dirname);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    break;

  case T_DIR:
    strcpy(buf, dirname);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      // ignore "." and ".."
      if(!strcmp(de.name, "."))
        continue;
      if(!strcmp(de.name, ".."))
        continue;
    
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      if(st.type != T_DIR){
        if(!strcmp(de.name, filename)) // compare name strings
          printf("%s\n", buf);
      }else{
        find(buf, filename); // recursively find files in sub-directory
      }
    }
    break;
  }
  close(fd);
}
int
main(int argc, char *argv[])
{
  char *dirname;
  char *filename;

  // check CLI paramenters
  if(argc != 3){
    fprintf(2, "usage: find [directory] [file]\n");
    exit(1);
  }

  dirname = argv[1];
  filename = argv[2];

  // call find to find wanted file
  find(dirname, filename);

  exit(0);
}
