#ifndef _TANIC_DISPATCHER_H
#define _TANIC_DISPATCHER_H

#include <vector>

#include "Poller.hpp"
#include "EventHandler.hpp"

namespace tanic{
	//class EventHandler;
	class Dispatcher
	{
	public:
	    Dispatcher();
	    ~Dispatcher() { delete m_poller;}
	    typedef std::vector<EventHandler *> ActiveEventlist;
	  //  static Dispatcher * s_dispatcher;
	  //  static Dispatcher * getInstance();
		void registerEvents(EventHandler *, int);
		void unregisterEvents(EventHandler *);
		void handleEvents();
	private:
	    
	    ActiveEventlist m_activeEventlist;
	    Poller * m_poller;
	};
}	
#endif
