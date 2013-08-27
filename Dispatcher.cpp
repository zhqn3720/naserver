
#include "Dispatcher.hpp"
#include <iostream>
#include <unistd.h>
using namespace tanic;

/*
Dispatcher * Dispatcher::s_dispatcher = NULL;

Dispatcher * Dispatcher::getInstance() 
{
		if (s_dispatcher == NULL) {
				s_dispatcher = new Dispatcher();
		} 
		return s_dispatcher;
		
}
*/
Dispatcher::Dispatcher()
{
	//std::cout << "construct\n";
	m_poller = new Poller();
}

void Dispatcher::registerEvents(EventHandler * eh, int et)
{
	/*
	* I/O复用
	* 用poll的复用机制注册事件，以后可以替换为其他更高效的复用机制
	*/
	//std::cout << "register" << std::endl;
	m_poller->registerHandles(eh, et);
	
}

void Dispatcher::unregisterEvents(EventHandler * eh)
{
	m_poller->removeHandles(eh);
}

void Dispatcher::handleEvents()
{
	//
	//std::cout << m_poller->getSize() << std::endl;
	m_poller->pollHandles(&m_activeEventlist);

	/*
	 * I/O解复用
	 * Dispatcher只关心ActiveEventlist，ActiveEventlist保存解复用后需要处理的事件 
	 * 
	 */
	for (ActiveEventlist::const_iterator it = m_activeEventlist.begin();
	      it != m_activeEventlist.end(); it++)
	 {
		      
			  (*it) -> handleEvent();
     }
     //结束一次事件分发都得清空ActiveEventList
     m_activeEventlist.clear();
 }
	
		

