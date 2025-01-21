#include "Connection.h"
#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "util.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#define READ_BUFFER 1024

Connection::Connection(Eventloop *_loop, Socket *_sock)
    : loop(_loop), clnt_sockfd(_sock), channel(nullptr), readBuffer(nullptr),
      writeBuffer(nullptr) {

  channel = new Channel(loop, clnt_sockfd->getsockFd());
  channel->enableReading();
  channel->useET();

  std::function<void()> cb =
      std::bind(&Connection::echo, this, clnt_sockfd->getsockFd());
  channel->setreadCallback(cb);
  readBuffer = new Buffer();
  writeBuffer = new Buffer();
  // printf("new connection\n");
}

Connection::~Connection() {
  delete channel;
  delete clnt_sockfd;
  delete readBuffer;
  delete writeBuffer;
}

void Connection::echo(int clnt_sockfd) {
  while (true) {
    char buf[READ_BUFFER];
    bzero(&buf, sizeof(buf));
    ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));
    // std::cout << read_bytes;

    if (read_bytes > 0) {
      // printf("%s", buf);
      readBuffer->append(buf, read_bytes);
    } else if (read_bytes == 0) {
      printf("client fd %d disconnected\n", clnt_sockfd);
      deleteConnectionCallback(
          clnt_sockfd); // 关闭socket会自动将文件描述符从epoll树上移除
      break;
    } else if (read_bytes == -1 && errno == EINTR) { // 客户端正常中断、继续读取
      printf("continue reading\n");
      continue;
    } else if (read_bytes == -1 &&
               ((errno == EAGAIN) ||
                (errno ==
                 EWOULDBLOCK))) { // 非阻塞IO，这个条件表示数据全部读取完毕
      printf("message from client fd %d: %s\n", clnt_sockfd,
             readBuffer->c_str());

      std::string echowords = "The server has received from client fd " +
                              std::to_string(clnt_sockfd) + ": " +
                              readBuffer->c_str() + "\n";
      // 使用 c_str() 获取字符串内容的 C 风格指针，使用 size() 获取实际长度
      writeBuffer->clear();
      writeBuffer->setBuf(echowords.c_str());
      size_t write_bytes =
          write(clnt_sockfd, writeBuffer->c_str(), writeBuffer->size());
      if (write_bytes == -1 && errno == EAGAIN) {
        printf("write buffer is full\n");
        break;
      }
      readBuffer->clear();
      break;
    }
  }
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> cb) {
  deleteConnectionCallback = cb;
}

void Connection::send(int sockfd) {
  char buf[readBuffer->size()];
  strcpy(buf, readBuffer->c_str());
  int data_size = readBuffer->size();
  int data_left = data_size;
  while (data_left > 0) {
    ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    data_left -= bytes_write;
  }
}