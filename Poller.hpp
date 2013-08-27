
#ifndef _TANIC_POLLER_HPP
#define _TANIC_POLLER_HPP
#include <poll.h>
#include <vector>
#include <map>
#include <sys/stropts.h>
#ifndef INFTIM
#define INFTIM -1
#endif
#include "EventHandler.hpp"
namespace tanic
{
	//class EventHandler;
	class Poller
	{
	public:
	    
		typedef std::vector<EventHandler *> ActiveEventlist;
		
		void registerHandles(EventHandler *, int);
		void removeHandles(EventHandler *);
		void pollHandles(ActiveEventlist *);
	private:
	    typedef std::vector<struct pollfd> Pollfds;
	    typedef std::map<int, EventHandler *> HandlerMap;
		Pollfds m_pollfds;
		HandlerMap m_handlerMap;
	};
}
#endif
