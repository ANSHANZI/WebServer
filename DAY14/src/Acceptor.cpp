#include "include/Acceptor.h"
#include "include/Channel.h"
#include "include/EventLoop.h"
#include "include/Socket.h"
#include <iostream>
Acceptor::Acceptor(Eventloop *_loop)
    : loop(_loop), accp_sock(nullptr), acceptChannel(nullptr) {
  accp_sock = new Socket();
  InetAddress *addr = new InetAddress("127.0.0.1", 1234);
  accp_sock->bind(addr);
  accp_sock->listen();
  // accp_sock->setnonblocking();
  acceptChannel = new Channel(loop, accp_sock->GetSockFd());

  std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
  acceptChannel->setreadCallback(cb);
  acceptChannel->enableReading();

  delete addr;
}
Acceptor::~Acceptor() {
  delete accp_sock;
  delete acceptChannel;
}

void Acceptor::acceptConnection() {

  InetAddress *clnt_addr = new InetAddress();
  Socket *clnt_sockfd =
      new Socket(accp_sock->accept(clnt_addr)); //可能会内存泄漏
  printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd->GetSockFd(),
         clnt_addr->GetIp(),clnt_addr->GetPort());
  clnt_sockfd->Setnoneblocking();
  newConnectionCallback(clnt_sockfd);
  delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> _cb) {
  newConnectionCallback = _cb;
}