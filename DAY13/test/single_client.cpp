#include <Connection.h>
#include "Socket.h"
#include <iostream>
#include <chrono>
#include <thread>
int main() {
  Socket *sock = new Socket();
  //sock->Setnoneblocking();
  sock->connect("127.0.0.1", 1234);

  Connection *conn = new Connection(nullptr, sock);

  while (true) {
    conn->GetlineSendBuffer();
    conn->Write();
    if (conn->GetState() == Connection::State::Closed) {
      conn->Close();
      std::cout<<"close"<<std::endl;
      break;
    }
    std::cout << "Message to server: " << conn->SendBuffer() << std::endl;
    // 添加短暂延迟
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    conn->Read();
    std::cout << "Message from server: " << conn->ReadBuffer() << std::endl;
  }

  delete conn;
  return 0;
}