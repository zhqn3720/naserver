#ifndef _TANIC_TCPCONNECTION_HPP
#define _TANIC_TCPCONNECTION_HPP
#include "EventHandler.hpp"
#include "InetAddress.hpp"
#include "Dispatcher.hpp"

#include <boost/enable_shared_from_this.hpp>
namespace tanic
{
	
	class TcpConnection:public EventHandler,
	{
	public:
		TcpConnection(Dispatcher *, int, const InetAddress&, const InetAddress&);
		~TcpConnection(){ }
		
		virtual void handleEvent();
		virtual int getHandle() { return m_handle; }
		virtual void setReventType(int ret) { m_reventType = ret; }
		virtual void setIndex(int index) { m_index = index; }
		virtual int index() { return m_index; }
		
		void handleRead();
		void handleClose();
		void handleError(){}
		void connectionEstablished();
	private:
	    enum state { connecting, connected };
	    void setState(state s) { m_state = s; }
	    
	    Dispatcher * m_dispatcher;
	    int m_handle;
	    InetAddress m_localaddr;
	    InetAddress m_peeraddr;
	
		int m_reventType;
		
		state m_state;
		
		int m_index;
	};
}
#endif
