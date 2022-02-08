#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <iostream>
#include <thread>
#include <list>
#include <vector>
#include <algorithm>
#include <mutex>
#include <unistd.h>
using namespace std;


// a global variable tn hold fd
std::vector<int> myList;

#define BUF_SIZE 100
#define EPOLL_SIZE 50
void error_handling(char *message);

void printList()
{
  // std::lock_guard<std::mutex> guard(myMutex);
  std::cout << "print a vector of fds"<< std::endl;
  while(1) {
//      for (int i = 0; i<myList.size(); i++) {
//        cout << myList[i] << endl;
//      }
    for(auto itr = myList.begin(), end_itr = myList.end(); itr != end_itr; ++itr) {
      cout << *itr << ",";
    }
    cout << endl;
    sleep(5);
  }
}

// todo with consistent hashing
int pick_client(int user_fd)
{
    int res = 0;
    int ind = rand() % (myList.size());
    res = myList[ind];
    do{
        ind = rand() % (myList.size());
        res = myList[ind];
    }while(user_fd==res);
    std::cout << "picked fd: " << res << std::endl;
    return res;  
}


int main(int argc, char *argv[])
{
    srand(getpid());
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[BUF_SIZE];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    if (argc != 2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }
    
    // start thread to print list
    std::thread t1(printList);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    epfd = epoll_create(EPOLL_SIZE); //可以忽略这个参数，填入的参数为操作系统参考
    ep_events = (epoll_event*)malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    event.events = EPOLLIN; //需要读取数据的情况
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event); //例程epfd 中添加文件描述符 serv_sock，目的是监听 enevt 中的事件

    while (1)
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1); //获取改变了的文件描述符，返回数量
        if (event_cnt == -1)
        {
            puts("epoll_wait() error");
            break;
        }
	
	std::cout << event_cnt << std::endl;
        for (i = 0; i < event_cnt; i++)
        {
            if (ep_events[i].data.fd == serv_sock) //客户端请求连接时
            {
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
		myList.push_back(clnt_sock);
		sort( myList.begin(), myList.end() );
		myList.erase( unique( myList.begin(), myList.end() ), myList.end() );

                event.events = EPOLLIN;
                event.data.fd = clnt_sock; //把客户端套接字添加进去
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client : %d \n", clnt_sock);
            }
            else //是客户端套接字时
            {
		memset(buf, 0, BUF_SIZE);
                str_len = recv(ep_events[i].data.fd, buf, BUF_SIZE, 0);
		std::cout << "message from client: " << buf <<" len: " <<str_len << std::endl;
                if (str_len == 0)
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL); //从epoll中删除套接字
		    myList.erase(std::remove(myList.begin(), myList.end(), ep_events[i].data.fd), myList.end());
		    close(ep_events[i].data.fd);
		    printf("closed client : %d \n", ep_events[i].data.fd);
                }
                else if(std::string(buf).find("[user]") != string::npos)
		{
		    // only user's request contain [user]
		    // trigger random pick the client 
		    //todo: consistent hashing 
		    int fd = pick_client(ep_events[i].data.fd);
		    send(fd, buf, str_len, 0);
		}
		else
                {  
		   std::cout << "len: " << str_len << std::endl;
		   std::cout << "sending message to user "<< buf << std::endl;  
                   // todo add map relationship between cleint and user
		   send(6, buf, str_len, 0);
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
