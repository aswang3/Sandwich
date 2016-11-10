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
  int fd = open(myfifo, O_WRONLY); // FIFO PIPE

  char * line = NULL;
  size_t len = 0;

  char* buf = malloc(0);

  while(getline(&line, &len, stdin) != -1){
    buf = realloc(buf, strlen(buf) + strlen(line) + 1);
    buf = strcat(buf, line);
  }

  free(buf);

  unlink(myfifo);
  close(fd);

  return 0;
}
