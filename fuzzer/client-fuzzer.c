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
		ssize_t ret_send = send(sockfd, buff, sizeof(buff), 0);
		printf("send return value: %d\n", ret_send);
		bzero(buff, sizeof(buff));
		recv(sockfd, buff, sizeof(buff), 0);
		printf("From Server : %s\n", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

int client_fuzzer(const char *Data, long long size)
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		// exit(0);
		return -1;
	}
	else
		printf("Socket successfully created..\n");
	
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
		error("setsockopt(SO_REUSEADDR) failed");

	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		// exit(0);
		return -1;
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}



// int socket_test(const char *Data, long long size)
// {
// 	int socket_desc;
// 	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	
// 	if (socket_desc == -1)
// 	{
// 		printf("Could not create socket");
// 	}
// 	else {
// 		printf("--create socket");	
// 	}
	
// 	return 0;
// }
int count = 0;
int LLVMFuzzerTestOneInput(const char *Data, long long Size) {
  printf("---------%d-------------\n", count++);
  client_fuzzer(Data, Size);
  // why sleep 1 second: so client can connect properly
//   sleep(1);
  return 0;
}
