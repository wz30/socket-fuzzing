#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected...........");

    while (1)
    {
        fputs("Input message(Q to quit): ", stdout);
        //fgets(message, BUF_SIZE, stdin);
        int num_tests = 100;
        double sum = 0;
        // test get operation
        for(int i = 2; i<num_tests; i++) {
            std::string str = "set "+std::to_string(i)+" 10"; 
            strcpy(message, str.c_str());
            
            if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
                break;
            std::string user = "[user]:";
            std::string temp = user + std::string(message);
            std::cout << temp << std::endl;

            //std::cout << temp.c_str() <<std::endl;
            auto start = std::chrono::high_resolution_clock::now();
            
            send(sock, temp.c_str(), strlen(temp.c_str()), 0);
            str_len = recv(sock, message, BUF_SIZE - 1, 0);
            
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            sum += duration.count();
            // std::cout << duration.count()/num_tests << ", ";
            message[str_len] = 0;
            printf("Message from server: %s\n", message);
        }
        std::cout << "time" << sum/num_tests << std::endl;
 
	    sleep(5);

        sum = 0;
        // test delete operation
        for(int i = 2; i<num_tests; i++) {
            strcpy(message, ("delete "+std::to_string(i)).c_str());
            
            if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
                break;
            std::string user = "[user]:";
            std::string temp = user + std::string(message);
            std::cout << temp << std::endl;

            //std::cout << temp.c_str() <<std::endl;
            auto start = std::chrono::high_resolution_clock::now();
            
            send(sock, temp.c_str(), strlen(temp.c_str()), 0);
            str_len = recv(sock, message, BUF_SIZE - 1, 0);
            
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            sum += duration.count();
            // std::cout << duration.count()/num_tests << ", ";
            message[str_len] = 0;
            printf("Message from server: %s\n", message);
        }
        std::cout << "averge delete time" << sum/num_tests << std::endl;
        sleep(5);
    }
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}