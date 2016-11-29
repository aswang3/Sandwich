#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

/**
** Reads in vim buffer from stdin and stores into a file
** pid is the first line of the buffer
** Create timestamp
** Diff vim buffer with god file
** Create patch file 
** Pipe to server in the format:
**     patch_file_name timestamp
**/
int main(){
  // Use fopen to create new temp file
  // Use getline to read from stdin
  // Use fwrite to store in temp file

  FILE * temp = fopen(".temp.txt", "r+");

  char * line = NULL;
  size_t size = 0;
  int pid = 0;
  if(getline(&line, &size, stdin) != -1){
    pid = atoi(line);
  }
  else{
    fprintf(stderr, "No pid given\n");
    return 42;
  }

  ssize_t bytes;
  while((bytes = getline(&line, &size, stdin)) != EOF){
//    fwrite(line, 1, bytes, temp);
    fprintf(temp, "%s", line);
  }

  // Create timestamp
  time_t time_stamp = time(NULL);

  // Generate patchfile name
  char p[52];
  sprintf(p, "%d", pid);

  // Generate diff command
  char command[1024];
  strcpy(command, "diff -c .temp.txt .SANDWICH.GOD > ");
  strcat(command, p);
  
  // Create patch file
  system(command);

  // Create buffer to send to pipe
  char buffer[2048];
  strcpy(buffer, p);
  strcat(buffer, " ");
  char d[52];
  sprintf(d, "%d", (int)time_stamp);
  strcat(buffer, d);

  // Write to FIFO pipe
  int fd = open(".connect_pipe", O_WRONLY);
//  fwrite(buffer, 1, strlen(buffer), fd);
  write(fd, buffer, strlen(buffer)+1);

  // Clean up
  fclose(temp);
  close(fd);
  free(line);
  
  return 0;
}
