#include<sys/socket.h>
#include"src/Buffer.h"
#include"src/InetAddress.h"
#include"src/Socket.h"
#include<arpa/inet.h> 
#include<string.h>
#include<cstring>
#include<iostream>
#include <unistd.h>
#include"src/util.h"
int main(){

    Socket *sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 1234);
    sock->connect(addr);
  
    
    int sockfd = sock->getsockFd();
    Buffer *sendBuffer = new Buffer();//发送缓冲区
    Buffer *readBuffer = new Buffer();//接收缓冲区
    
    while(true){
        sendBuffer->getline();
        //std::cout<<sendBuffer->c_str()<<std::endl;
        ssize_t write_bytes = write(sockfd, sendBuffer->c_str(), sendBuffer->size());
        //std:: cout<<write_bytes<<std::endl;
        if(write_bytes == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        int already_read = 0;
        char buf[1024];   
        bzero(&buf, sizeof(buf));  // 确保在读取数据之前将 buf 清零
        while(true){
           // printf("%s\n",buf);
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if(read_bytes > 0){
                readBuffer->append(buf, read_bytes);
                already_read += read_bytes;
            } else if(read_bytes == 0){         //EOF
                printf("server disconnected!\n");
                exit(EXIT_SUCCESS);
            }
            if(already_read >= sendBuffer->size()){
                printf("message from server: %s\n", readBuffer->c_str());
                break;
            } 
        }
        readBuffer->clear();
    }
    
   delete addr;
   delete sock;
   return 0;
}

