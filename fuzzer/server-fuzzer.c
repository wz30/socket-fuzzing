#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 7077
#define SA struct sockaddr

#define CHAT_TIMES 3

// Function designed for chat between client and server.
void func(int sockfd)
{
	char buff[MAX];
	long n=0;
	// infinite loop for chat
	while (n<CHAT_TIMES) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(sockfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\n ", buff);
		bzero(buff, MAX);
		// copy server message in the buffer
		// while ((buff[n++] = getchar()) != '\n')
		// 	;
		sprintf(buff, "%ld", n++);
		// strcpy(buff,"hi");
		// and send that buffer to client
		write(sockfd, buff, sizeof(buff));

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

int server_fuzzer(const char *Data, long long size)
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
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
int LLVMFuzzerTestOneInput(const char *Data, long long Size) {
  server_fuzzer(Data, Size);
  return 0;
}
