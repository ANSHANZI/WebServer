#pragma once
#include<arpa/inet.h>
#include"Macros.h"


class InetAddress {
private:
  struct sockaddr_in addr {};

public:
 
  socklen_t addr_len;

  InetAddress();
  InetAddress(const char *ip, uint16_t port);
  DISALLOW_COPY_AND_MOVE(InetAddress);

  ~InetAddress();

  void SetAddr(sockaddr_in addr);//设置地址
  sockaddr_in GetAddr();
  const char *GetIp();
  uint16_t GetPort();
};



class Socket {
private:
  int sockfd;

public:
  Socket();
  DISALLOW_COPY_AND_MOVE(Socket);
  explicit Socket(int fd);// explicit 防止隐式转换
  ~Socket();

  void bind(InetAddress *);
  void listen();
  
  int accept(InetAddress *);
  void connect(InetAddress *);
  void connect(const char *ip, uint16_t port);

  bool IsNoneBlocking();
  void Setnoneblocking();
  int GetSockFd();
};