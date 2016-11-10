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

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1024];
    struct sockaddr_in serv_addr; 

    if(argc != 2)
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
//continue;
//       return 1;
    }

//while(1)
//{
	char sendString[10000];
	read(0, &sendString, 10000); 
	int x = send(sockfd, sendString, strlen(sendString)+1,0);
//	printf("%d", x);

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
//	n = read(sockfd, recvBuff, sizeof(recvBuff)-1);

//	if(n<=0)
//		continue;
//	strcpy(sendBuff, "Hello");
//	send(sockfd, "Hello", strlen("Hello")+1,0);
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
//	send(sockfd, "Hello", strlen("Hello")+1,0);
//	else
//	{
 //	close(sockfd);
//	while( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){}
 	char tosend[10000];
	read(0, &tosend, 10000);
//	if(fputs(tosend, stdout)==EOF)
//		break;
        int x = send(sockfd, tosend, strlen(tosend)+1,0);
//	}

 
   } 

    if(n < 0)
    {
        printf("\n Read error \n");
    } 
//}
    return 0;
}
