#include"Socket.h"
#include"util.h"
#include<sys/socket.h>
#include <fcntl.h>
#include<unistd.h>
#include "InetAddress.h"

Socket:: Socket():sockfd(-1){
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    errif(sockfd==-1,"socket create error");
}

//?????
Socket:: Socket(int fd): sockfd(fd){
    errif(sockfd==-1,"socket create error");
}

Socket::~Socket(){
   if(sockfd!=-1){
     close(sockfd);
     sockfd=-1;
   }
}

 void Socket::bind(InetAddress * _addr){
  errif(::bind(sockfd, (sockaddr*)&_addr->addr, _addr->addr_len) == -1, "socket bind error");
 }

void Socket::listen(){
    errif(::listen(sockfd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking(){
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

int Socket::accept(InetAddress *_addr){
 int clnt_sockfd = ::accept(sockfd, (sockaddr*)&_addr->addr, &_addr->addr_len);
 errif(clnt_sockfd == -1, "socket accept error");
 return clnt_sockfd;
}

void Socket::connect(InetAddress *_addr){
    struct sockaddr_in addr = _addr->getAddr();
    errif(::connect(sockfd, (sockaddr*)&addr, sizeof(addr)) == -1, "socket connect error");
}

int  Socket:: getsockFd(){
    return sockfd;
}