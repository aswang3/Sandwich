#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define MAX_BUF 1024*1024

/**
** Reads buffer from stdin and writes into FIFO pipe
**/
int fifo_main(){
  char * myfifo = "/tmp/myfifo";

  mkfifo(myfifo, 0666);
  int fd = open(myfifo, O_WRONLY); // FIFO PIPE

  char * line = NULL;
  size_t len = 0;

  while(getline(&line, &len, stdin) != -1){
    write(fd, &line, strlen(line));
  }

  unlink(myfifo);
  close(fd);

  return 0;
}

/**
** Code to read from FIFO pipe
**/
void fifo_read(char * buf){
  char * myfifo = "/tmp/myfifo";

  int fd = open(myfifo, O_RDONLY);
  read(fd, buf, MAX_BUF);
  
  close(fd);
}
