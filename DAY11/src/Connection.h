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
     Socket *clnt_sockfd;
     Channel *channel;
     std::function<void(int)> deleteConnectionCallback;
     std::string *inBuffer;
     Buffer *readBuffer;//读缓冲区
     Buffer *writeBuffer;//写缓冲区
public:
    Connection(Eventloop *_loop, Socket *_sock);
    ~Connection();

    void echo(int socketfd);
    void setDeleteConnectionCallback(std::function<void(int)> cb);
    void send(int socketfd);
};


