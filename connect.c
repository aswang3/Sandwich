#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>


int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Usage: ./sandwich <file name>\n");
    exit(2);
  }

  FILE* f_main = fopen(argv[1], "r");

  // NEEDS: temp file is going to be in shared space, not current directory

//mapping from input file to hidden file
	char filehold [strlen(argv[1])+2];
	strcpy(filehold, ".");
	strcat(filehold, argv[1]);


//try to open the file. if it is null, create it

  FILE* f_temp = fopen(filehold, "r+b");

	if(!f_temp)
	{
		f_temp = fopen(filehold , "a+");

		  // NEEDS: should check if someone is already editing before trying to overwrite temp

		  char* txt;
		  size_t len = 0;
		  ssize_t line = 0;
		  while ((line = getline(&txt, &len, f_main)) != EOF) {
		  fprintf(f_temp, "%s", txt);
	 	}

	}


  fclose(f_main);
  fclose(f_temp);

  // fork into vim

  int status = 0;
  pid_t p = fork();

  if (!p) {
    execlp("vim", "vim", filehold, (char*) NULL);
//needs to pipe in automatic E to vim

//	fprintf(stdin, "E\n", 3);

    printf("Cannot open temp file\n");
    exit(1);
  }

  waitpid(p, &status, 0);

  // NEEDS: should overwrite main files if last one out

  // Idea: main file on sandwich server (or whatever), can pull the temp file from sandwich
  
  return 0;
}
