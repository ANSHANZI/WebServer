server:
	g++ src/util.cpp client.cpp -o client && \
	g++ src/util.cpp server.cpp src/epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp src/EventLoop.cpp src/server.cpp src/Acceptor.cpp -o server
clean:
	rm server && rm client