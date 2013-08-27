#include "InetAddress.hpp"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
using namespace tanic;

InetAddress::InetAddress(uint16_t port)
{
	bzero(&m_addr, sizeof m_addr);
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(port);
	m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

InetAddress::InetAddress(const std::string& ip, uint16_t port)
{
	bzero(&m_addr, sizeof m_addr);
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(port);
	if (inet_pton(AF_INET, ip.c_str(), &m_addr.sin_addr) <= 0) {
		std::cerr << "form presentation to numeric error.\n";
	}
}

std::string InetAddress::toHostPort() const
{
	char buf[32];
	char host[INET_ADDRSTRLEN] = "INVALID";
	::inet_ntop(AF_INET, &m_addr.sin_addr, host, sizeof(host));
	uint16_t port = ntohs(m_addr.sin_port);
	snprintf(buf, sizeof(buf), "%s:%u", host, port);
	return buf;
}
