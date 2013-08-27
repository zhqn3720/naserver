#include "TcpConnection.hpp"

#include <iostream>
#include <unistd.h>
#include <assert.h>
using namespace tanic;

TcpConnection::TcpConnection(Dispatcher * dispatcher, int connfd, 
               const InetAddress& localaddr, const InetAddress & peeraddr)
               :m_dispatcher(dispatcher),
               m_handle(connfd),
               m_localaddr(localaddr),
               m_peeraddr(localaddr),
               m_state(connecting),
               m_index(-1)
{
	;
}

void TcpConnection::connectionEstablished()
{
	assert(m_state == connecting);
	m_state = connected;
	m_dispatcher->registerEvents(this,POLLIN | POLLRDNORM);
}

void TcpConnection::handleEvent()
{
	 if (m_reventType & (POLLIN | POLLPRI | POLLRDHUP)) {
		 handleRead();
	 }
	 if ((m_reventType & POLLHUP) && !(m_reventType & POLLIN)) {
		 handleClose();
	 }
 }
 
 void TcpConnection::handleRead()
 {
	 char buf[100];
	 ssize_t n = read(m_handle, buf, sizeof(buf));
	 if (n > 0) {
		 std::cout << "received "<<n <<" bytes\n";
	 } else if (n == 0) {
		 handleClose();
	 } else {
		 handleError();
	 }
}

void TcpConnection::handleClose()
{
	assert(m_state = connected);
	std::cout << "closed" <<'\n';
	m_dispatcher->unregisterEvents(this);
}
