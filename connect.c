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


//mapping from input file to hidden file
	char file[strlen(argv[1])+1];
	strcpy(file, argv[1]);
   	char filehold [strlen(argv[1])+2];
	strcpy(filehold, ".");
	strcat(filehold, argv[1]);

//try to open the file. if it is null, create it
//
	FILE* f_temp = fopen(filehold, "r+b");

  	if(!f_temp)
	{
		f_temp = fopen(filehold , "a+");

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
//TODO needs to pipe in automatic E to vim

		printf("Cannot open temp file\n");
		exit(1);
	}

        waitpid(p, &status, 0);	

// TODO NEEDS: should overwrite main files if last one out
// Currently overwrites no matter what


	f_temp = fopen(filehold, "r");
	f_main = fopen(file, "w+");

	char*txt;
	size_t len = 0;
	ssize_t line = 0;
	while((line = getline(&txt, &len, f_temp))!=EOF)
		fprintf(f_main, "%s", txt);
printf("hello\n");
	fclose(f_main);
	fclose(f_temp);

	

//TODO remove temp file after done IF LAST
//Currently removes no matter what

	remove(filehold);

//TODO update temp file if save is sent to main file, and other user is editing temp



//TODO Should reset file permissionas as well
// Idea: main file on sandwich server (or whatever), can pull the temp file from sandwich
   
        return 0;
}
