#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

/**
** Reads buffer from stdin and writes into FIFO pipe
**/
int main(){
  char * myfifo = "/tmp/myfifo";

  mkfifo(myfifo, 0666);
  int fd = open(myfifo, O_WRONLY);i // FIFO PIPE

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
void read(){
  char * myfifo = "/tmp/myfifo";
  char buf[1024];

  int fd = open(myfifo, O_RDONLY);
  read(fd, buf, 1024);
  
  close(fd);

}
