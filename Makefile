objects = InetAddress.o Socket.o  Dispatcher.o Poller.o Acceptor.o TcpServer.o TcpConnection.o test.o
test : $(objects)
	g++ -o test $(objects) 

InetAddress.o:InetAddress.hpp
Socket.o: InetAddress.hpp Socket.hpp
Poller.o: EventHandler.hpp Poller.hpp
Dispatcher.o: Poller.hpp Dispatcher.hpp EventHandler.hpp
Acceptor.o:  Dispatcher.hpp Acceptor.hpp EventHandler.hpp InetAddress.hpp
TcpServer.o:  TcpConnection.hpp Acceptor.hpp Dispatcher.hpp\
              Callbacks.hpp InetAddress.hpp TcpServer.hpp
TcpConnection.o: Dispatcher.hpp TcpConnection.hpp EventHandler.hpp InetAddress.hpp	
test.o: InetAddress.hpp Dispatcher.hpp Acceptor.hpp TcpServer.hpp
clean:
	rm test $(objects)
install:
	mkdir -p ./build/include/tanic
	mkdir -p ./build/lib
	cp *.hpp ./build/include/tanic
	ar -rv libtanic.a *.o
	mv libtanic.a ./build/lib
uninstall:
	rm -r ./build
