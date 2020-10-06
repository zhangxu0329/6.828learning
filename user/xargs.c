#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// read stdin feed xargs func

#define MAXSIZE 32

int
main(int argc, char * argv[]) {
  char args[MAXSIZE][MAXSIZE];
  char *pass[MAXSIZE];
  // 指针数组如果想要赋值还得重新开辟空间
  for(int j = 0; j < MAXSIZE; ++j) {
    pass[j] = args[j];
  }
  int i = 1;
  for(; i < argc; ++i) {
    strcpy(args[i - 1], argv[i]);
  }
  
  --i;
  int n = -1;
  char c = 'a';
  char *p = args[i];
  while((n = read(0, &c, sizeof(c))) == sizeof(c)) {
    if(c == ' ' || c == '\n') {
      *p = '\0';
      p = args[++i];
    }else {
      *p++ = c;
    }
  }
  if(n < 0) {
    fprintf(2, "%s", "xargs input error\n");
    exit(0);
  }
  pass[i] = 0;

  if(fork() == 0) {
    exec(pass[0], pass);
  }
  wait(0);
  exit(0);
}

