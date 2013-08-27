#ifndef _TANIC_ECHOHANDLER_HPP
#define _TANIC_ECHOHANDLER_HPP
#include <unistd.h>
#include <stdlib.h>
#include "EventHandler.hpp"
namespace tanic
{
	class Dispatcher;
	class EchoHandler:public EventHandler
	{
	public:
		EchoHandler(Dispatcher *, int);
		~EchoHandler(){ delete this; }
		void handleEvent();
		void handleRead();
		void handleClose();
		void handleError(){}
		int getHandle() { return m_handle; }
		void setReventType(int ret) { m_reventType = ret; }
	private:
	    Dispatcher * m_dispatcher;
		int m_handle;
		int m_reventType;
	};
}
#endif
