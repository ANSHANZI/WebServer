#pragma once
#include<functional>
#include<string>

class Eventloop;
class Socket;
class Channel;
class Buffer;
class Connection
{
private:
     Eventloop *loop;
     Socket *socket;
     Channel *channel;
     std::function<void(Socket*)> deleteConnectionCallback;

     std::string *inBuffer;
     Buffer *readBuffer;

public:
    Connection(Eventloop *_loop, Socket *_sock);
    ~Connection();

    void echo(int socketfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)> cb);
};


