#ifndef _TANIC_CALLBACKS_HPP
#define _TANIC_CALLBACKS_HPP

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace tanic
{
	class TcpConnection;
	typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
	
	typedef boost::function<void (const TcpConnectionPtr&)> ConnectionCallback;
	typedef boost::function<void (const TcpConnectionPtr&,
	                              const char * data,
	                              ssize_t len)> MessageCallback;
}

#endif
