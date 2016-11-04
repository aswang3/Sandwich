#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

/**
** Read buffer from FIFO Pipe (sent by Vim)
** "Diff" buffer with original file
** Send changes to TCP server
** Include some kind of metadata describing changes
**/
int main(int argc, char * argv[]){
  char * pipe = "/tmp/pipe";
  
  FILE * fifo = fopen(pipe, "r");
  fseek(fifo, 0, SEEK_END);
  long file_size = ftell(fifo);
  rewind(fifo);

  char * command = calloc(1024, sizeof(char));
  strcpy(command, "diff -c ");
  strcat(command, argv[1]);
  strcat(command, " /tmp/pipe > .diff.txt");  // command = "diff -c filename /tmp/pipe > .diff.txt"

  system(command); // diffs original file and piped file and stores into .diff.txt
  free(command);

  return 0;
}
