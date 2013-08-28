#include "TcpServer.hpp"
#include "Acceptor.hpp"

#include <boost/bind.hpp>
using namespace tanic;

TcpServer::TcpServer(Dispatcher * dispatcher, const InetAddress & inetaddr)
           :m_name(inetaddr.toHostPort()),
            m_dispatcher(dispatcher),
            m_started(false),
            m_addr(inetaddr),
            m_acceptor(new Acceptor(m_dispatcher, m_addr))
{
	;
}

void TcpServer::newConnection(int connfd, const InetAddress& localaddr, const InetAddress& peeraddr)
{
	std::cout << "newConnection(): accepted a new connection from "
		       << peeraddr.toHostPort() <<"\n";
	TcpConnectionPtr  conn(new TcpConnection(m_dispatcher, connfd, localaddr, peeraddr));
	m_connections.push_back(conn);
	//connect（将m_tcpconnection注册进分发器）
	conn->connectionEstablished();
}

void TcpServer::start()
{
    m_acceptor->setNewConnectionCallback(boost::bind(&TcpServer::newConnection, this, _1, _2, _3));
	//accept(将m_acceptor注册进分发器）
	m_acceptor->listen();
	for(; ;) {
		m_dispatcher->handleEvents();
	}
}
