#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>


typedef struct changed_{
  size_t line_num;
  char * line;
} changed;

typedef struct appended_{
  size_t line_num;
  char * line;
} appended;

typedef struct deleted_{
  size_t line_num;
  char * line;
} deleted;

/**
** Returns a copy of the line that changed excluding the first character and space
**/
char * get_line(char * line){
  return strdup(line + 2);
}

/**
** Adds deleted line to deleted list
**/
void add_deletes(char * line, deleted ** list, size_t linenum){
  
}

/**
** Adds appended line to list of appends
**/
void add_appends(char * line, appended ** list, size_t linenum){

}

/**
** Adds changed line to list of changes
**/
void add_changes(char * line, changed ** list, size_t linenum){

}

/**
** Helper function to parse diff -c output
** Stores changes, appends, and deletions in respective data structures
**/
void parse(char * filename, changed ** c_list, appended ** a_list, deleted ** d_list){
  FILE * file = fopen(filename, "r");
  
  char * line = NULL;
  size_t size = 0;
  ssize_t c;
  int dash_flag = 0;
  size_t linenum = 0;
  while((c = getline(&line, &size, file)) != -1){
    if(*line == '-'){
      if(line[1] == '-'){
        if(dash_flag) break;
        else dash_flag = 1;
      }
      else add_deletes(line, d_list, linenum);
    }
    if(*line == '!')
      add_changes(line, c_list, linenum);
    if(*line == '+')
      add_appends(line, a_list, linenum);
    linenum++;
  }
  
  free(line);
  fclose(file);
}

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
