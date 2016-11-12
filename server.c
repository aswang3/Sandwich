#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main(int argc, char**argv[]) //argv 1 is name of file
{
	//Create master updated file
	char* master = malloc(sizeof(char)*(strlen("cp .SANDWICH.GOD ")+1+strlen(argv[1])));
	strcpy(master, "cp ");
	strcat(master, argv[1]);
	strcat(master, " .SANDWICH.GOD");
	system(master);

	//create pipe
	mkfifo(".connect_pipe", S_IXUSR);
	int check = open("connect_pipe", "r");
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
