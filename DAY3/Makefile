server:
	g++ util.cpp client.cpp -o client && \ 
	g++ util.cpp server.cpp epoll.cpp InetAddress.cpp Socket.cpp -o server
clean:
	rm server && rm client
	