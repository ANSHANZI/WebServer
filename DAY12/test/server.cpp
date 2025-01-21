#include "Server.h"
#include "EventLoop.h"

int main() {
  Eventloop *loop = new Eventloop();
  Server *server = new Server(loop);
  loop->loop();
  return 0;
}