#include<sys/socket.h>
#include<arpa/inet.h> 
#include<string>
#include<cstring>
#include<iostream>
#include<stdio.h>
#include<unistd.h>

void errif(bool condition, const char *errmsg);

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

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    while(true){
        char buf[1024];
        bzero(&buf,sizeof(buf));
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
            close(clnt_sockfd);
            break;
        }else if(read_bytes==-1){
            close(clnt_sockfd);
            errif(true,"socket read error");
        }
    }
       

   
}

void errif(bool condition, const char *errmsg){
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}