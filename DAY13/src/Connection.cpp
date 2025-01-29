#include "include/Connection.h"
#include "include/Buffer.h"
#include "include/Channel.h"
#include "include/EventLoop.h"
#include "include/Socket.h"
#include "include/util.h"
#include"include/Macros.h"
#include <iostream>
#include <stdio.h>
#include <utility>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#define READ_BUFFER 1024

Connection::Connection(Eventloop *_loop, Socket *_sock)
    : loop(_loop), clnt_sockfd(_sock), channel(nullptr), readBuffer(nullptr),
      writeBuffer(nullptr) {

  if (_loop!= nullptr) {
    channel= new Channel(_loop, _sock->GetSockFd());
    channel->enableReading();
    channel->useET();
  }
  // std::function<void()> cb =
  //     std::bind(&Connection::echo, this, clnt_sockfd->getsockFd());
  // channel->setreadCallback(cb);
  readBuffer = new Buffer();
  writeBuffer = new Buffer();
  state = State::Connected;
  // printf("new connection\n");
}

Connection::~Connection() {
  delete channel;
  delete clnt_sockfd;
  delete readBuffer;
  delete writeBuffer;
}


void Connection::Read(){
    ASSERT(state == State::Connected, "Connection is not connected");
    readBuffer->clear();
    if(clnt_sockfd->IsNoneBlocking()){
      ReadNonBlocking();
    }else{
      ReadBlocking();
    }
}

void Connection::Write(){
    //std::cout<<"write"<<std::endl;
    ASSERT(state == State::Connected, "Connection is not connected");
    if(clnt_sockfd->IsNoneBlocking()){
      WriteNonBlocking();
    }else{
      WriteBlocking();
    }
}

//非阻塞读
void Connection::ReadNonBlocking(){
  int sockfd = clnt_sockfd->GetSockFd();
  char buf[READ_BUFFER];
  while (true) {
    memset(buf,0,sizeof(buf));
    ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
    // std::cout << read_bytes;
    if (read_bytes > 0) {
      // printf("%s", buf);
      readBuffer->append(buf, read_bytes);
    } else if (read_bytes == 0) { // 客户端断开连接
      printf("client fd %d disconnected\n", sockfd);
      state = State::Closed;
      break;
    } else if (read_bytes == -1 && errno == EINTR) { // 客户端正常中断、继续读取
      printf("continue reading\n");
      continue;
    }else if (read_bytes == -1 &&
               ((errno == EAGAIN) ||(errno ==EWOULDBLOCK))) { // 非阻塞IO，这个条件表示数据全部读取完毕
      //printf("message from client fd %d: %s\n",sockfd,readBuffer->c_str());
      break;
    }else{
      printf("Other error on client fd %d\n", sockfd);
      state= State::Closed;
      break;
    }
  }
}



//阻塞读
void Connection::ReadBlocking(){
  int sockfd =  clnt_sockfd->GetSockFd();
  unsigned int rcv_size = 0;
  socklen_t len = sizeof(rcv_size);
  getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
  char buf[rcv_size];
  ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
  if (bytes_read > 0) {
    readBuffer->append(buf, bytes_read);
  } else if (bytes_read == 0) {
    printf("read EOF, blocking client fd %d disconnected\n", sockfd);
    state = State::Closed;
  } else if (bytes_read == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state = State::Closed;
  }
}


//非阻塞写
void Connection::WriteNonBlocking(){
  int sockfd = clnt_sockfd->GetSockFd();
  char buf[writeBuffer->size()];
  memcpy(buf, writeBuffer->c_str(), writeBuffer->size());
  //std::cout<<writeBuffer->c_str()<<std::endl;
  int data_size = writeBuffer->size();//要发送的总数据量
  int data_left = data_size;//表示还剩余需要发送的数据量
  while (data_left>0) {
    ssize_t write_bytes = write(sockfd,buf + data_size - data_left, data_left);//当前需要发送的数据在 buf 数组中的起始位置
    if (write_bytes == -1 && errno == EINTR) {//中断继续写
      printf("continue writing\n");
      continue;
    }
    if (write_bytes== -1 && errno == EAGAIN) {//写缓冲区已满
      break;
    }
    if (write_bytes == -1) {//其他错误
      printf("Other error on client fd %d\n", sockfd);
      state = State::Closed;
      break;
    }
    data_left -= write_bytes;
  }
}

//阻塞写
void Connection::WriteBlocking(){
 // 没有处理send_buffer_数据大于TCP写缓冲区，的情况，可能会有bug
  int sockfd = clnt_sockfd->GetSockFd();
  ssize_t bytes_write = write(sockfd, writeBuffer->c_str(), writeBuffer->size());
  if (bytes_write == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state = State::Closed;
  }
}

void Connection::Close(){
    deleteConnectionCallback(clnt_sockfd);
}

Connection::State Connection::GetState(){
    return state;
}

void Connection::SetSendBuffer(const char *str){//设置发送缓冲区
      writeBuffer->setBuf(str);
}
Buffer * Connection::GetReadBuffer(){
  return readBuffer;
}
const char *  Connection::ReadBuffer(){//获取读缓冲区
  return readBuffer->c_str();
}
Buffer * Connection::GetSendBuffer(){
  return writeBuffer;
}
const char * Connection::SendBuffer(){
  return writeBuffer->c_str();
}
void  Connection::GetlineSendBuffer(){
  writeBuffer->getline();
}
Socket *  Connection::GetSocket(){
  return clnt_sockfd;
}

void Connection::SetDeleteConnectionCallback(std::function<void(Socket *)> const &callback){
  deleteConnectionCallback = callback;
}
void Connection::SetOnConnectCallback(std::function<void(Connection *)> const &callback){
  onConnectionCallback = callback;
  channel->setreadCallback([this]() {onConnectionCallback(this); });
}



// void Connection::echo(int clnt_sockfd) {
//   while (true) {
//     char buf[READ_BUFFER];
//     bzero(&buf, sizeof(buf));
//     ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));
//     // std::cout << read_bytes;

//     if (read_bytes > 0) {
//       // printf("%s", buf);
//       readBuffer->append(buf, read_bytes);
//     } else if (read_bytes == 0) {
//       printf("client fd %d disconnected\n", clnt_sockfd);
//       deleteConnectionCallback(
//           clnt_sockfd); // 关闭socket会自动将文件描述符从epoll树上移除
//       break;
//     } else if (read_bytes == -1 && errno == EINTR) { // 客户端正常中断、继续读取
//       printf("continue reading\n");
//       continue;
//     } else if (read_bytes == -1 &&
//                ((errno == EAGAIN) ||
//                 (errno ==
//                  EWOULDBLOCK))) { // 非阻塞IO，这个条件表示数据全部读取完毕
//       printf("message from client fd %d: %s\n", clnt_sockfd,
//              readBuffer->c_str());

//       std::string echowords = "The server has received from client fd " +
//                               std::to_string(clnt_sockfd) + ": " +
//                               readBuffer->c_str() + "\n";
//       // 使用 c_str() 获取字符串内容的 C 风格指针，使用 size() 获取实际长度
//       writeBuffer->clear();
//       writeBuffer->setBuf(echowords.c_str());
//       size_t write_bytes =
//           write(clnt_sockfd, writeBuffer->c_str(), writeBuffer->size());
//       if (write_bytes == -1 && errno == EAGAIN) {
//         printf("write buffer is full\n");
//         break;
//       }
//       readBuffer->clear();
//       break;
//     }
//   }
// }



