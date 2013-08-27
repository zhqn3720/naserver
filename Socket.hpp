#ifndef _TANIC_SOCKET_HPP
#define _TANIC_SOCKET_HPP

namespace tanic
{
	class InetAddress;
	class Socket
	{
	public:
		//explicit Socket(int sockfd):m_sockfd(sockfd) {}
		Socket();
		~Socket();
		int fd() const { return m_sockfd;}
		void bind(const InetAddress &);
		void listen();
		int accept(InetAddress *);
		void setReuseAddr(bool on);
		int getSockName(InetAddress *);
		int getPeerName(InetAddress *);
	private:
		int m_sockfd;
	};
}
#endif
		
		
