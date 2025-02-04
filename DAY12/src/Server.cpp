#include "include/Server.h"
#include "include/Acceptor.h"
#include "include/Channel.h"
#include "include/Connection.h"
#include "include/EventLoop.h"
#include "include/InetAddress.h"
#include "include/Socket.h"
#include "include/ThreadPool.h"
#include <functional>
#include <iostream>
#include <unistd.h>
Server::Server(Eventloop *el) : mainReactor(el) {
  acceptor = new Acceptor(mainReactor);
  std::function<void(Socket *)> cb =
      std::bind(&Server::newConnection, this,
                std::placeholders::_1); // std::placeholders::_1占位
  acceptor->setNewConnectionCallback(cb);

  ///?
  int size = std::thread::hardware_concurrency(); //线程数量，也是subReactor数量
  std::cout << "subReacto num: " << size << std::endl;
  thpool = new ThreadPool(size); //新建线程池
  for (int i = 0; i < size; ++i) {
    subReactors.push_back(new Eventloop()); //几个线程就有几个subReactor
  }

  for (int i = 0; i < size; ++i) {
    std::function<void()> sub_loop =
        std::bind(&Eventloop::loop,
                  subReactors[i]); //将每个subReactor的loop函数绑定到sub_loop
    thpool->add(sub_loop); //将每个subReactor的loop函数添加到线程池
  }
}

Server::~Server() {
  delete acceptor;
  delete thpool;
}

void Server::newConnection(Socket *clnt_sockfd) {
  if (clnt_sockfd->getsockFd() != -1) {
    int random = clnt_sockfd->getsockFd() % subReactors.size();
    std::cout << "new connection, fd: " << clnt_sockfd->getsockFd()
              << " bind to subReactor: " << random << std::endl;
    Connection *conn = new Connection(
        subReactors[random],
        clnt_sockfd); //随机选择一个Eventloop 与新连接绑定 处理新连接的事件
    std::function<void(int)> cb =
        std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[clnt_sockfd->getsockFd()] = conn; //？？？？
  }
}

void Server::deleteConnection(int sockfd) {
  if (sockfd != -1) {
    auto it = connections.find(sockfd);
    if (it != connections.end()) {
      Connection *conn = connections[sockfd];
      connections.erase(sockfd);
      close(sockfd); // 正常关闭套接字
      delete conn;   // 删除连接对象
    }
  }
}
