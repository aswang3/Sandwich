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

	printf("EKAjfr;oiwajfrowsjfr;osirj\n");
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

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1024];
    struct sockaddr_in serv_addr; 
    makeFifo();

    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

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
//	read(0, &sendString, strlen(".diff.txt")); 
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

    return 0;
}
