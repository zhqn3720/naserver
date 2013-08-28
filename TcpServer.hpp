#ifndef _TANIC_TCPSERVER_HPP
#define _TANIC_TCPSERVER_HPP
#include "TcpConnection.hpp"
#include "Dispatcher.hpp"
#include "InetAddress.hpp"
#include "Acceptor.hpp"
#include "Callbacks.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>
namespace tanic
{
	
	class TcpServer
	{
	public:
		TcpServer(Dispatcher *, const InetAddress&);
		~TcpServer(){ }
		
		void start();
		void newConnection(int, const InetAddress&, const InetAddress&);

	private:
	    std::string m_name;
		
		//m_dispatcher对象生存周期由调用者管理
		Dispatcher * m_dispatcher;
	    bool m_started;
	    InetAddress m_addr;
	    
	    //m_acceptor生存周期等于TcpServer生存周期
	    boost::scoped_ptr<Acceptor> m_acceptor;
	    typedef std::vector<TcpConnectionPtr> Connections;
	    
	    Connections m_connections;
	    
	    ConnectionCallback m_connectionCallback;
	    MessageCallback m_messageCallback;
	};
	
}
#endif
		
