#ifndef _TANIC_ACCEPTOR_HPP
#define _TANIC_ACCEPTOR_HPP
#include "Socket.hpp"
#include "EventHandler.hpp"
#include "Dispatcher.hpp"

#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
namespace tanic
{
	class InetAddress;//前向声明 InetAddress* 作为参数
	class Dispatcher;
	class TcpServer;
	class Acceptor:public EventHandler
	{
	public:
	     //回调
	     typedef boost::function<void (int, const InetAddress&,
	                                   const InetAddress&)> NewConnectionCallback;
	    //TcpSever的参赛通过TcpServer类的this指针传入，this指针是常指针
		Acceptor(Dispatcher *, const InetAddress&);
		~Acceptor() { }
		//newConnection回调函数为TcpServer的成员函数
		//typedef void (TcpServer::*NewConnectionCallback)(int, const InetAddress&, const InetAddress&);
		
		void listen();
		void handleEvent();
		int getHandle(){ return m_handle;}
		void setReventType(int ret) { m_revent = ret;}
		void setIndex(int index) { m_index = index; }
		int index() { return m_index;}
		
		void setNewConnectionCallback(NewConnectionCallback newConnectionCallback) {
			m_newConnectionCallback = newConnectionCallback;
		}
	private:
	    Dispatcher * m_dispatcher;
	   // TcpServer * const m_tcpserver;
	    bool m_listenning;
	    int m_index;//在Pollfds vector中的序列号
	    
	    boost::scoped_ptr<Socket> m_acceptSocket;
		int m_handle;
		int m_revent;

		
		NewConnectionCallback m_newConnectionCallback;
	};
}
#endif
