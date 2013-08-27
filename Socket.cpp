#include "Socket.hpp"
#include "InetAddress.hpp"
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
using namespace tanic;

Socket::Socket()
{
	m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sockfd < 0) {
		std::cerr << "create socket failed\n";
	}
	
	int flags;
	if ((flags = ::fcntl(m_sockfd, F_GETFL, 0)) < 0) {
		std::cerr << "F_GETFL error\n";
	}
	flags |= O_NONBLOCK;
	if (::fcntl(m_sockfd, F_SETFL, 0) < 0) {
		std::cerr << "F_SETFL error\n";
	}
	
}

Socket::~Socket()
{
	close(m_sockfd);
}

void Socket::bind(const InetAddress& localaddr)
{
	struct sockaddr_in addr = localaddr.getSockAddrInet();
	if (::bind(m_sockfd, (struct sockaddr *)&addr, sizeof addr) < 0) {
		std::cerr << "bind error\n";
	}
}

void Socket::listen()
{
	if (::listen(m_sockfd, 5) < 0) {
		std::cerr << "listen error\n";
	}
}

int Socket::accept(InetAddress* peeraddr)
{
	int connfd;
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socklen_t addrlen = sizeof(addr);
	if ((connfd = ::accept(m_sockfd, (struct sockaddr*)&addr, &addrlen)) < 0) {
		std::cerr << "accept error\n";
	}
	peeraddr->setSockAddrInet(addr);
	return connfd;
}

	
void Socket::setReuseAddr(bool on)
{
	int optval = on ? 1: 0;
	::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

int Socket::getSockName(InetAddress *localaddr)
{
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socklen_t addrlen = sizeof(addr);
	int res;
	if ((res = ::getsockname(m_sockfd, (struct sockaddr *)&addr, &addrlen)) < 0) {
		std::cerr << "get local address name error\n";
	}
	localaddr->setSockAddrInet(addr);
	return res;
}

int Socket::getPeerName(InetAddress *peeraddr)
{
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socklen_t addrlen = sizeof(addr);
	int res;
	if ((res = ::getpeername(m_sockfd, (struct sockaddr *)&addr, &addrlen)) < 0) {
		std::cerr << "get peer address name error\n";
	}
	peeraddr->setSockAddrInet(addr);
	return res;
}
