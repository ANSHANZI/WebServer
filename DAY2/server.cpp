#include<sys/socket.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<arpa/inet.h> 
#include<string>
#include<cstring>
#include<iostream>
#include<stdio.h>
#include<unistd.h>

#define MAX_EVENTS 1024

void errif(bool condition, const char *errmsg);
void setnonblocking(int sockfd);

int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0); 
    errif(sockfd==-1,"socket create error");
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");
  
    //第二个参数是listen函数的最大监听队列长度，系统建议的最大值SOMAXCONN被定义为128。
    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

    
    //参数表示监听事件的大小，如超过内核会自动调整，已经被舍弃，无实际意义，传入一个大于0的数即可

    int epfd = epoll_create1(0);  //epoll 的文件描述符
    // 将监听的文件描述符相关的检测信息添加到epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = sockfd;

    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &epev);

    struct epoll_event events[MAX_EVENTS];
    while(true){
      int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
      errif(nfds == -1, "epoll wait error");

      for(int i = 0; i < nfds; ++i){  //处理这nfds个事件
            if(events[i].data.fd == sockfd){    //发生事件的fd是服务器socket fd，表示有新客户端连接
            struct sockaddr_in clnt_addr;
            socklen_t clnt_addr_len = sizeof(clnt_addr);
            bzero(&clnt_addr, sizeof(clnt_addr));
            int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
            errif(clnt_sockfd == -1, "socket accept error");
            printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

            bzero(&epev, sizeof(epev));
            epev.data.fd = clnt_sockfd;   
            epev.events = EPOLLIN | EPOLLET;
            setnonblocking(clnt_sockfd);
            epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &epev);   //将该客户端的socket fd添加到epoll

            } else if(events[i].events & EPOLLIN){      //发生事件的是客户端，并且是可读事件（EPOLLIN）
            while(true){
                char buf[1024];
                bzero(&buf,sizeof(buf));
                int clnt_sockfd=events[i].data.fd;
                ssize_t read_bytes=read(clnt_sockfd,buf,sizeof(buf));
                if(read_bytes>0){
                    printf("message from client fd %d: %s\n", clnt_sockfd,buf);
                     std::string echo = "The server has received from client fd " 
                     + std::to_string(clnt_sockfd) 
                    + ": " 
                    + buf 
                    + "\n";
                     // 使用 c_str() 获取字符串内容的 C 风格指针，使用 size() 获取实际长度
                    write(clnt_sockfd, echo.c_str(), echo.size());
                }else if(read_bytes==0){
                    printf("client fd %d disconnected\n", clnt_sockfd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, clnt_sockfd, NULL);
                    close(clnt_sockfd);// //关闭socket会自动将文件描述符从epoll树上移除
                    break;
                }else if(read_bytes == -1 && errno == EINTR){//客户端正常中断、继续读取
                    printf("continue reading");
                        continue;
                }else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
                    printf("client fd %d finish reading ",clnt_sockfd);
                        break;
                    }
              }
            }else{         //其他事件，之后的版本实现
                printf("something else happened\n");
        }
    }


}
    close(epfd);
    close(sockfd);
   
}

void errif(bool condition, const char *errmsg){
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
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