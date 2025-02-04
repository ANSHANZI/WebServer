#include "server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "epoll.h"
#include "Acceptor.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<vector>
#include<iostream>


Server::Server(Eventloop* el):loop(el){
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb  = std::bind(&Server::newConnection, this, std::placeholders::_1);//std::placeholders::_1占位
    acceptor->setNewConnectionCallback(cb);
}


Server::  ~Server(){
    delete acceptor;
}

void Server::handleReadEvent(int sockfd ){
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

void Server::newConnection(Socket *server_sockfd){
     InetAddress  *clnt_addr =new InetAddress(); //会发生内存泄露！没有delete
     Socket *clnt_sockfd = new Socket(server_sockfd->accept(clnt_addr)); //会发生内存泄露！没有delete 
     printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd->getsockFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
     clnt_sockfd->setnonblocking();
     Channel *clntChannel = new Channel(loop, clnt_sockfd->getsockFd());


    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sockfd->getsockFd());
    clntChannel->setCallback(cb);

    clntChannel->enableReading();
}