
#include <tanic/Dispatcher.hpp>
#include <tanic/TcpServer.hpp>
#include <iostream>
#include <unistd.h>
#include <poll.h>

using namespace tanic;


int main()
{
	typedef std::vector<struct pollfd> pollfds;
	pollfds m_pollfds;
	
	Dispatcher * loop = new Dispatcher();
	InetAddress servaddr(9877);
	
	TcpServer server(loop, servaddr);
	server.start();
	
	return 0;
}
