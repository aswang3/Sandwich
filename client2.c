#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <sys/stat.h>
#include <fcntl.h>

/**
** Makes the fifo pipe
**/
void makeFifo(){
  char * myfifo = "/tmp/pipe";
  mkfifo(myfifo, 0666);
}

/**
** Read buffer from FIFO Pipe (sent by Vim)
** "Diff" buffer with original file
** Creates patch file and sends to server
**/
void createPatch(char*arg)
{
  char * pipe = "/tmp/pipe";

  int fd = open(pipe, O_RDONLY);
  char buf[2];
  read(fd, buf, 1);

  printf("blockt\n");  
/*  FILE * fifo = fopen(pipe, "r");
  fseek(fifo, 0, SEEK_END);
  long file_size = ftell(fifo);
  rewind(fifo);
*/

  char * command = calloc(1024, sizeof(char));
  strcpy(command, "diff -c ");
  strcat(command, arg);
  strcat(command, " /tmp/pipe > .diff.txt");  // command = "diff -c filename /tmp/pipe > .diff.txt"

  system(command); // diffs original file and piped file and stores into .diff.txt

//  system("patch < .diff.txt"); // patches changes into the original file

  free(command);
  

//	return ".diff.txt";

}

int main(int argc, char *argv[]) // MAIN NEEDS COMMENTS PLZ
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1024];
    struct sockaddr_in serv_addr; 
    makeFifo();

    if(argc != 4)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    // could port connect.c to here . . .

    /*

    FILE* f_main = fopen(argv[3], "r"); // argv[3] would be file name (what is argv[2]?)

    char file[strlen(argv[1])+1];	// mapping from input file to hidden file
    strcpy(file, argv[1]);
    char filehold [strlen(argv[1])+2];
    strcpy(filehold, ".");
    strcat(filehold, argv[1]);

    FILE* f_temp = fopen(filehold. "r+b");  // try to open the file. if it is null, create it

    if(!f_temp) {                           // creating temp file if it doesn't exist
        f_temp = fopen(filehold , "a+");
        char* txt;
        size_t len = 0;
        ssize_t line = 0;
        while ((line = getline(&txt, &len, f_main)) != EOF) fprintf(f_temp, "%s", txt);
    }

    fclose(f_main);      // closing both main and temp so vim can read
    fclose(f_temp);

    // didn't port section for multiple users yet (idk how it works atm)

    int status = 0;
    pid_t p = fork();

    if (!p) {
        execlp("vim", "vim", filehold, "-u", "sandrc",(char*) NULL); // sandrc is vim sript/plugin
        printf("Cannot open temp file\n");
        exit(1);
    }

    */

    // . . . and i did

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    while( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
    }

	createPatch(argv[2]);

	char sendString[strlen(".diff.txt\n")+1];
	strcpy(sendString, ".diff.txt\n");
	printf("%s", sendString);
	int x = send(sockfd, sendString, strlen(sendString)+1,0);

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }

	createPatch(argv[2]);

	char sendString[strlen(".diff.txt")+1];
	strcpy(sendString, ".diff.txt");
	printf("%s", sendString);
	read(0, sendString, strlen(".diff.txt")); 
	int x = send(sockfd, sendString, strlen(sendString)+1,0);
 
    }  

    if(n < 0)
    {
        printf("\n Read error \n");
    }

    // remainder of connect port

    /*

    waitpid(p, &status, 0);

    f_temp = fopen(filehold, "r");
    f_main = fopen(file, "w+");

    char*txt;
    size_t len = 0;
    ssize_t line = 0;
    while((line = getline(&txt, &len, f_temp))!=EOF) fprintf(f_main, "%s", txt);

    fclose(f_main);
    fclose(f_temp);

    remove(filehold);

    */

    return 0;
}
