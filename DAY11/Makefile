src=$(wildcard src/*.cpp)  #获取src目录下的所有.cpp文件

server:
	g++ -std=c++11 -pthread -g \
	$(src) \
	server.cpp \
	-o server
	
client:
	g++ src/util.cpp src/Buffer.cpp client.cpp src/InetAddress.cpp src/Socket.cpp -o client

th:
	g++ -pthread src/ThreadPool.cpp ThreadPoolTest.cpp -o ThreadPoolTest

test:
	g++ src/util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp src/ThreadPool.cpp \
	-pthread \
	test.cpp -o test

clean:
	rm server && rm client 

k:
	fuser -i -k 1234/tcp
