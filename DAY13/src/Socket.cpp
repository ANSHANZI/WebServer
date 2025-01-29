#include "include/Socket.h"
#include "include/util.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <cstdio>

Socket::Socket() : sockfd(-1) {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  errif(sockfd == -1, "socket create error");
}

Socket::Socket(int fd) : sockfd(fd) {
  errif(sockfd == -1, "socket create error");
}

Socket::~Socket() {
  if (sockfd != -1) {
    close(sockfd);
    sockfd = -1;
  }
}

void Socket::bind(InetAddress *_addr) {
  struct sockaddr_in tmp_addr =_addr->GetAddr();
  errif(::bind(sockfd, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1,
        "socket bind error");
}

void Socket::listen() {
  errif(::listen(sockfd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::Setnoneblocking() { // 设置为非阻塞
  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags == -1) {
    perror("fcntl(F_GETFL) error");
    exit(EXIT_FAILURE);
  }
  flags |= O_NONBLOCK; // 设置 O_NONBLOCK 标志
  if (fcntl(sockfd, F_SETFL, flags) == -1) {
    perror("fcntl(F_SETFL) error");
    exit(EXIT_FAILURE);
  }
}


bool Socket::IsNoneBlocking() {
  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags == -1) {
    perror("fcntl(F_GETFL) error");
    exit(EXIT_FAILURE);
  }
  return flags & O_NONBLOCK;
}


int Socket::accept(InetAddress *addr) {
  // for server socket
  int clnt_sockfd = -1;
  struct sockaddr_in tmp_addr {};
  socklen_t addr_len = sizeof(tmp_addr);
  if (IsNoneBlocking()) {// 如果是非阻塞的
    while (true) {
      clnt_sockfd = ::accept(sockfd,(sockaddr *)&tmp_addr, &addr_len);
      if (clnt_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
        // printf("no connection yet\n");
        continue;
      }
      if (clnt_sockfd == -1) {
        errif(true, "socket accept error");
      } else {
        break;
      }
    }
  } else {
    clnt_sockfd = ::accept(sockfd,(sockaddr *)&tmp_addr, &addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
  }
  addr->SetAddr(tmp_addr);
  return clnt_sockfd;
}




void Socket::connect(InetAddress *addr) {
  // for client socket
  struct sockaddr_in tmp_addr = addr->GetAddr();
  if (fcntl(sockfd, F_GETFL) & O_NONBLOCK) {
    while (true) {
      int ret = ::connect(sockfd, (sockaddr *)&tmp_addr, sizeof(tmp_addr));
      if (ret == 0) {
        break;
      }
      if (ret == -1 && (errno == EINPROGRESS)) {
        continue;
        /* 连接非阻塞式sockfd建议的做法：
            The socket is nonblocking and the connection cannot be
          completed immediately.  (UNIX domain sockets failed with
          EAGAIN instead.)  It is possible to select(2) or poll(2)
          for completion by selecting the socket for writing.  After
          select(2) indicates writability, use getsockopt(2) to read
          the SO_ERROR option at level SOL_SOCKET to determine
          whether connect() completed successfully (SO_ERROR is
          zero) or unsuccessfully (SO_ERROR is one of the usual
          error codes listed here, explaining the reason for the
          failure).
          这里为了简单、不断连接直到连接完成，相当于阻塞式
        */
      }
      if (ret == -1) {
        errif(true, "socket connect error");
      }
    }
  } else {
    errif(::connect(sockfd, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1, "socket connect error");
  }
}

void Socket::connect(const char *ip, uint16_t port) {
  InetAddress *addr = new InetAddress(ip, port);
  connect(addr);
  delete addr;
}

int Socket::GetSockFd() { return sockfd; }


InetAddress::InetAddress() : addr_len(sizeof(addr)) {
  bzero(&addr, sizeof(addr));
}
InetAddress::InetAddress(const char *ip, uint16_t port)
    : addr_len(sizeof(addr)) {
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip);
  addr.sin_port = htons(port);
  addr_len = sizeof(addr);
}

void InetAddress::SetAddr(sockaddr_in addr_) { addr = addr_; }


InetAddress::~InetAddress() {}

sockaddr_in InetAddress::GetAddr() { return addr; }

const char *InetAddress::GetIp() { return inet_ntoa(addr.sin_addr); }

uint16_t InetAddress::GetPort() { return ntohs(addr.sin_port); }