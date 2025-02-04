#include"Connection.h"
#include"EventLoop.h"
#include"Socket.h"
#include"Channel.h"
#include<string>
#include<string.h>
#include<stdio.h>
#include<unistd.h>

#define READ_BUFFER 1024

Connection::Connection(Eventloop *_loop, Socket *_sock):loop(_loop),socket(_sock),channel(nullptr)
{      
  channel=new Channel(loop,socket->getsockFd());
  std::function<void()> cb = std::bind(&Connection::echo, this, socket->getsockFd());
  channel->setCallback(cb);
  channel->enableReading();

}

Connection::~Connection()
{      
    delete channel;
    delete socket;
}

void Connection::echo(int socketfd){
   while(true){
        char buf[READ_BUFFER];
        bzero(&buf,sizeof(buf));
        ssize_t read_bytes=read(socketfd,buf,sizeof(buf));
        if(read_bytes>0){
            printf("message from client fd %d: %s\n", socketfd,buf);
            std::string echo = "The server has received from client fd " 
                     + std::to_string(socketfd) 
                    + ": " 
                    + buf 
                    + "\n";
                     // 使用 c_str() 获取字符串内容的 C 风格指针，使用 size() 获取实际长度
                    write(socketfd, echo.c_str(), echo.size());
                }else if(read_bytes==0){
                    printf("client fd %d disconnected\n", socketfd);
                    close(socketfd);// //关闭socket会自动将文件描述符从epoll树上移除
                    break;
                }else if(read_bytes == -1 && errno == EINTR){//客户端正常中断、继续读取
                    printf("continue reading\n");
                        continue;
                }else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
                    printf("client fd %d finish reading\n",socketfd);
                        break;
                    }
              }  

}
void  Connection::setDeleteConnectionCallback(std::function<void(Socket*)> cb){
       deleteConnectionCallback = cb;
}