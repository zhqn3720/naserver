#ifndef _TANIC_INETADDRESS_H
#define _TANIC_INETADDRESS_H

#include <netinet/in.h>
#include <string>
namespace tanic
{
	class InetAddress
	{
	public:
	    //端口号
		explicit InetAddress(uint16_t port);
		//IP地址和端口号
		InetAddress(const std::string& ip, uint16_t port);
	    //套接字地址
	    InetAddress(const struct sockaddr_in& addr): m_addr(addr) { }
	    const struct sockaddr_in& getSockAddrInet() const { return m_addr;} 
	    void setSockAddrInet(const struct sockaddr_in addr) {
			m_addr = addr;
		}
		std::string toHostPort() const;
	private:
		struct sockaddr_in m_addr;
	};
} 
#endif
		
