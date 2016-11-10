#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>


int main () {
  char * myfifo = "/tmp/myfifo";

  mkfifo(myfifo, 0666);
  int fd = open(myfifo, O_WRONLY); // FIFO PIPE

  char* line = NULL;
  size_t len = 0;

  char* in = malloc(0);

  while(getline(&line, &len, stdin) != -1){
    in = realloc(strlen(in) + strlen(line) + 1);
    in = strcat(in, line);
  }

  write(fd, &in, strlen(in) + 1);
  
  free(in);

  unlink(myfifo);
  close(fd);

  return 0;
}
