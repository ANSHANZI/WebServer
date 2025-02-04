#include<sys/socket.h>
#include<arpa/inet.h> 
#include<string.h>
#include<cstring>
#include<iostream>
#include<stdio.h>

int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0); 
    struct sockaddr_in serv_addr;
   //bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    bind(sockfd,(sockaddr*)&serv_addr, sizeof(serv_addr));
    //第二个参数是listen函数的最大监听队列长度，系统建议的最大值SOMAXCONN被定义为128。
    listen(sockfd,SOMAXCONN);//

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    //bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
}
