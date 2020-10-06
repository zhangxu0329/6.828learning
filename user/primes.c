#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void primes(int * p) {
  close(p[1]);
  int i = 0;
  if(read(p[0], &i, sizeof(i)) <= 0) {
    close(p[0]);
    return;
  }
  int fd[2];
  pipe(fd);
  printf("prime %d\n", i);
  if(fork() == 0) {
    primes(fd);
  }else {
    close(fd[0]);
    int num = 1;
    while(read(p[0], &num, sizeof(num)) > 0) {
      if(num % i)
        write(fd[1], &num, sizeof(num));
    }
    close(fd[1]);
    close(p[0]);
  }
}

int
main(int argc, char *argv[]) {
  int fd[2];
  pipe(fd);
  printf("start\n");
  if(fork() == 0) {
    primes(fd);
  }else {
    close(fd[0]);
    int i = 2;
    for(; i <= 35; ++i) {
      write(fd[1], &i, sizeof(i));
    }
    close(fd[0]);
    close(fd[1]);
    // printf("end\n");
    // close(fd[0]);
  }
  wait(0);
  exit(0);
}