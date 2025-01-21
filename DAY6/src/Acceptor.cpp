#include"Acceptor.h"
#include"EventLoop.h"
#include"Socket.h"
#include"Channel.h"
#include "InetAddress.h"
Acceptor::Acceptor(Eventloop *_loop):loop(_loop) {
   sock= new Socket();
   addr =new InetAddress("127.0.0.1", 8888);
   sock->bind(addr);
   sock->listen();
   sock->setnonblocking();
   acceptChannel = new Channel(loop, sock->getsockFd());

   std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
   
   acceptChannel->setCallback(cb);
   acceptChannel->enableReading();        
}
Acceptor::~Acceptor(){
    delete sock;
    delete addr;
    delete acceptChannel;
}

void Acceptor::acceptConnection(){
    newConnectionCallback(sock);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb){
    newConnectionCallback = _cb;
}