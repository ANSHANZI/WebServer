#include "server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "epoll.h"
#include "Acceptor.h"
#include "Connection.h"
#include <functional>


Server::Server(Eventloop* el):loop(el){
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb  = std::bind(&Server::newConnection, this, std::placeholders::_1);//std::placeholders::_1占位
    acceptor->setNewConnectionCallback(cb);
}


Server::  ~Server(){
    delete acceptor;
}


void Server::newConnection(Socket *sock){
    
    Connection *conn = new Connection(loop, sock);
    std::function<void(Socket*)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[sock->getsockFd()] = conn;//？？？？
}

void Server::deleteConnection(Socket * sock){
    Connection *conn = connections[sock->getsockFd()];
    connections.erase(sock->getsockFd());
    delete conn;
}

