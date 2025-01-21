#pragma once 
#include<map>

class Eventloop;
class Socket;
class Acceptor;
class Connection; // Forward declaration

class Server {
private:
    Eventloop *loop;
    Acceptor * acceptor;
    std::map<int, Connection*> connections; //所有TCP连接
public:
    Server(Eventloop*);
    ~Server();

    //void handleReadEvent(int);
    
    void newConnection(Socket *serv_sock);
    void deleteConnection(int sockfd);   //断开TCP连接
};