#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main(int argc, char**argv[]) //argv 1 is name of file
{

	if(argc != 2)
	{
		printf("Usage: ./server <filename>\n");
		return -1;
	}

	//Create master updated file
	char* master = malloc(strlen("cp .SANDWICH.GOD ")+1+strlen(argv[1]));
	strcpy(master, "cp ");
	strcat(master, argv[1]);
	strcat(master, " .SANDWICH.GOD");
	int check = system(master);
	if(WEXITSTATUS(check) != 0)
        {
                printf("Error in file: %s\n", argv[1]);
		return -1;
        }

	//create pipe
	mkfifo(".connect_pipe", S_IXUSR);
	check = open(".connect_pipe", O_RDONLY);
	dup2(check, STDIN_FILENO);
	
	//initialize variables to be read from pipe
	char*patch =malloc(15);
	int *timestamp=malloc(sizeof(int));
	
	//keeps track of most recently updated
	int mastertime;

	//keeps reading from pipe
	while(scanf("%s %d", patch, timestamp)!=EOF)
	{

		printf("patch received\n");

//TODO Perform comparison. If passed, update
		mastertime = *timestamp;
		char patch_command [strlen(patch) +15];
		strcpy(patch_command, "patch < ");
		strcat(patch_command, patch);
		system(patch_command);

		printf("patch applied\n");
	}


	//destroy pipe and master updated copy
	unlink(".connect_pipe");
	strcpy(master, "rm .SANDWICH.GOD");
	system(master);

	//take care of memory
	free(patch);
	free(timestamp);
	free(master);
	return 0;

	}
