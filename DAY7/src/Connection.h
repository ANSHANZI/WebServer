#pragma once
#include<functional>

class Eventloop;
class Socket;
class Channel;
class Connection
{
private:
     Eventloop *loop;
     Socket *socket;
     Channel *channel;
     std::function<void(Socket*)> deleteConnectionCallback;
public:
    Connection(Eventloop *_loop, Socket *_sock);
    ~Connection();
    void echo(int socketfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)> cb);
};


