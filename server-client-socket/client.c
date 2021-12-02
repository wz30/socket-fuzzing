#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 7077
#define SA struct sockaddr

#define CHAT_TIMES 3
#define SERVER_IP "152.3.52.136"
void func(int sockfd)
{
	char buff[MAX];
	long n=0;
	while (n < CHAT_TIMES) {
		bzero(buff, sizeof(buff));
		// printf("Enter the string : ");
		
		// while ((buff[n++] = getchar()) != '\n')
		// 	;
		sprintf(buff, "%ld", n++);
		// strcpy(buff,"hi");
		send(sockfd, buff, sizeof(buff), 0);
		bzero(buff, sizeof(buff));
		recv(sockfd, buff, sizeof(buff), 0);
		printf("From Server : %s\n", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("152.3.52.136");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}
