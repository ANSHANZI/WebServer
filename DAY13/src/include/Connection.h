#pragma once
#include <functional>
#include <string>
#include "Macros.h"

class Eventloop;
class Socket;
class Channel;
class Buffer;
class Connection {

public:

  enum State { 
    Invaild =1,//无效
    Handshaking,//握手
    Connected,//连接
    Closed,//关闭
    Failed,//失败
  };



  Connection(Eventloop *_loop, Socket *_sock);
  ~Connection();
  
  //禁止拷贝和移动构造
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;

  

  void Read();
  void Write();

  void SetDeleteConnectionCallback(std::function<void(Socket *)> const &callback);
  void SetOnConnectCallback(std::function<void(Connection *)> const &callback);

  State GetState();
  void Close();//关闭连接

  void SetSendBuffer(const char *str);
  Buffer *GetReadBuffer();
  const char *ReadBuffer();
  Buffer *GetSendBuffer();
  const char *SendBuffer();
  void GetlineSendBuffer();
  Socket *GetSocket();


  void OnConnect(std::function<void()> fn);
private:
  Eventloop *loop;
  Socket *clnt_sockfd;
  Channel *channel;
  State state{Invaild};

  std::function<void(Socket *)> deleteConnectionCallback;//删除连接回调
  std::function<void(Connection *)> onConnectionCallback;//设置每个连接的回调函数
  Buffer *readBuffer;  //读缓冲区
  Buffer *writeBuffer; //写缓冲区

  void ReadNonBlocking();//非阻塞读
  void WriteNonBlocking();//非阻塞写
  void ReadBlocking();//阻塞读
  void WriteBlocking();//阻塞写
};
