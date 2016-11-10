#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){

	startclient();

}

int startclient(){

	struct addrinfo hints, *results;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int result = getaddrinfo(NULL, "1234", &hints, &results);
	int x = connect(results->ai_socktype, results->ai_addr, results->ai_addrlen);

	if(x==-1)
		printf("HI?");
}
