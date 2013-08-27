#include "Acceptor.hpp"
#include "InetAddress.hpp"
#include "EchoHandler.hpp"
#include <unistd.h>
#include <iostream>
using namespace tanic;

Acceptor::Acceptor(Dispatcher * dispatcher, TcpServer * const tcpserver, const InetAddress & servaddr)
           :m_dispatcher(dispatcher),
           m_tcpserver(tcpserver),
           m_listenning(false),
           m_index(-1)
{
	m_acceptSocket = new Socket();
	m_handle = m_acceptSocket->fd();
	m_acceptSocket->setReuseAddr(true);
	m_acceptSocket->bind(servaddr);
	
}

void Acceptor::listen()
{
	m_listenning = true;
	m_acceptSocket->listen();
	//std::cout << "acceptor\n";
	m_dispatcher->registerEvents(this, POLLRDNORM | POLLIN);
}

void Acceptor::handleEvent()
{
	/*
	 * 事务处理
	 */
	 InetAddress peerAddr(0);
	 int connfd = m_acceptSocket->accept(&peerAddr);
	 if (connfd > 0) {
		 if (m_newConnectionCallback) {
			 InetAddress localAddr(0);
			 m_acceptSocket->getSockName(&localAddr);
		     m_newConnectionCallback(connfd, localAddr, peerAddr);
		 }
	 }
}
