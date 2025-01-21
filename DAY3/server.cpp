#include"Socket.h"
#include"util.h"
#include"epoll.h"
#include"InetAddress.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<vector>
#include<iostream>
#define MAX_EVENTS 1024

void setnonblocking(int sockfd);
void handleReadEvent(int sockfd);
int main(){

    Socket *server_sockfd= new Socket();
    InetAddress  *server_addr =new InetAddress("127.0.0.1", 8888);
    server_sockfd->bind(server_addr);
    server_sockfd->listen();
  
    Epoll *server_epoll= new Epoll();
    server_epoll->addFd(server_sockfd->getsockFd(),EPOLLIN | EPOLLET);

    while(true){
        std::vector<epoll_event> events = server_epoll->poll();
        int nfds = events.size();
        for(int i = 0; i < nfds; ++i){
            if(events[i].data.fd == server_sockfd->getsockFd()){    //发生事件的fd是服务器socket fd，表示有新客户端连接
            InetAddress  *clnt_addr =new InetAddress(); //会发生内存泄露！没有delete
            Socket *clnt_sockfd = new Socket(server_sockfd->accept(clnt_addr));  
            printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd->getsockFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
            clnt_sockfd->setnonblocking();
            server_epoll->addFd(clnt_sockfd->getsockFd(), EPOLLIN | EPOLLET);
            }else if(events[i].events & EPOLLIN){
                handleReadEvent(events[i].data.fd);
            }else{
                 printf("something else happened\n");
            }
        }
    }

    delete server_sockfd;
    delete server_addr;
    delete server_epoll;
    return 0;
}

void handleReadEvent(int sockfd){
    while(true){
        char buf[1024];
        bzero(&buf,sizeof(buf));
        ssize_t read_bytes=read(sockfd,buf,sizeof(buf));
        if(read_bytes>0){
            printf("message from client fd %d: %s\n", sockfd,buf);
            std::string echo = "The server has received from client fd " 
                     + std::to_string(sockfd) 
                    + ": " 
                    + buf 
                    + "\n";
                     // 使用 c_str() 获取字符串内容的 C 风格指针，使用 size() 获取实际长度
                    write(sockfd, echo.c_str(), echo.size());
                }else if(read_bytes==0){
                    printf("client fd %d disconnected\n", sockfd);
                    close(sockfd);// //关闭socket会自动将文件描述符从epoll树上移除
                    break;
                }else if(read_bytes == -1 && errno == EINTR){//客户端正常中断、继续读取
                    printf("continue reading\n");
                        continue;
                }else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
                    printf("client fd %d finish reading\n",sockfd);
                        break;
                    }
              }  


}


// 设置 socket 为非阻塞
void setnonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl(F_GETFL) error");
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK;  // 设置 O_NONBLOCK 标志
    if (fcntl(sockfd, F_SETFL, flags) == -1) {
        perror("fcntl(F_SETFL) error");
        exit(EXIT_FAILURE);
    }
}