#include "src/EventLoop.h"
#include "src/server.h"

int main() {
    Eventloop *loop = new Eventloop();
    Server *server = new Server(loop);
    loop->loop();
    delete server;
    delete loop;
    return 0;
}