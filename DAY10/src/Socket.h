#pragma once 

class InetAddress;
class Socket{
private:
    int sockfd;
public:
    Socket();
    Socket(int fd);
    ~Socket();


    void bind(InetAddress *);
    void listen();
    void setnonblocking();
    int accept(InetAddress *);
    void connect(InetAddress *);
    int getsockFd();

};