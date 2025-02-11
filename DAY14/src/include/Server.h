#pragma once
#include<map>
#include<vector>
#include<functional>
class Eventloop;
class Socket;
class Acceptor;
class Connection; // Forward declaration
class ThreadPool;

class Server {
private:
  Eventloop *mainReactor; //主反应堆 负责接受新连接
  Acceptor *acceptor;
  std::map<int, Connection *> connections; //所有TCP连接
  std::vector<Eventloop *> subReactors;    //负责处理事件循环
  ThreadPool *thpool;                      //线程池

   std::function<void(Connection *)> on_connect_callback_; //连接建立回调函数
public:
  Server(Eventloop *);
  ~Server();

  // void handleReadEvent(int);

  void newConnection(Socket *clnt_sockfd);
  void deleteConnection(Socket *sock); //断开TCP连接
  void OnConnect(std::function<void(Connection *)> fn);//
  
};