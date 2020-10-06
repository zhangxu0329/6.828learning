#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char*
findname(char *path) {
  char *p;
  for(p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  ++p;
  return p;
} 

void
find(char *path, char *name) {
  char buf[512];
  char *p;
  int fd;
  struct dirent de;
  struct stat st;
  // 0 readonly
  if((fd = open(path, 0)) < 0)  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  switch(st.type) {
    case T_FILE:
      if(strcmp(name, findname(path)) == 0) 
        printf("%s\n", path);
      break;
    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("ls: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf + strlen(buf);
      // char * tmp = p;
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
          continue;
        if(de.inum == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        find(buf, name);
      }
      break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("usage find : find path name\n");
    exit(0);
  }else if(argc == 2){
    find(".", argv[1]);
  }
  find(argv[1], argv[2]);
  exit(0);
}