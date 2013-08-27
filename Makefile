objects = InetAddress.o Socket.o Poller.o Dispatcher.o Acceptor.o TcpServer.o TcpConnection.o test.o
test : $(objects)
	g++ -o test $(objects) 

InetAddress.o:InetAddress.hpp
	g++ -c InetAddress.cpp
Socket.o: InetAddress.hpp Socket.hpp
	g++ -c Socket.cpp	
Dispatcher.o: Poller.hpp Dispatcher.hpp
	g++ -c Dispatcher.cpp
Poller.o: EventHandler.hpp Poller.hpp
	g++ -c Poller.cpp
Acceptor.o:  Dispatcher.hpp Acceptor.hpp
	g++ -c Acceptor.cpp
TcpServer.o: TcpServer.cpp TcpServer.hpp
	g++ -c TcpServer.cpp
TcpConnection.o: TcpConnection.cpp TcpConnection.hpp
	g++ -c TcpConnection.cpp
test.o: test.cpp
	g++ -c test.cpp
clean:
	rm test $(objects)
