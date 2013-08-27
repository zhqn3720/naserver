#include "Poller.hpp"
#include <iostream>
#include <assert.h>
#define TIME_DELAY 60*60*60*5
using namespace tanic;

void Poller::registerHandles(EventHandler * eh, int et)
{
	//eh序列号为-1，表示还没有将eh的句柄加入到Pollfds中
	//std::cout << eh->index() << '\n';
	if (eh->index() < 0) {
		assert(m_handlerMap.find(eh->getHandle()) == m_handlerMap.end());
		struct pollfd pfd;
		pfd.fd = eh -> getHandle();
		pfd.events = et;
		pfd.revents = 0;
		m_pollfds.push_back(pfd);
		int idx = m_pollfds.size() - 1;
		eh->setIndex(idx);
		//std::cout << m_pollfds.size() << std::endl;
		m_handlerMap[pfd.fd] = eh;
	} else {
		//更新
		assert(m_handlerMap.find(eh->getHandle()) != m_handlerMap.end());
		assert(m_handlerMap[eh->getHandle()] == eh);
		unsigned idx = eh->index();
		assert(0 <= idx && idx < m_pollfds.size());
		struct pollfd& pfd = m_pollfds[idx];
		assert(pfd.fd == eh->getHandle() || pfd.fd == -1);
		pfd.fd = eh->getHandle();
		pfd.events = et;
		pfd.revents = 0;
	}
}


void Poller::removeHandles(EventHandler * eh)
{
	if (eh->index() < 0) return;
	//eh在m_handleMap中且Map中的元素正确
	assert(m_handlerMap.find(eh->getHandle()) != m_handlerMap.end());
	assert(m_handlerMap[eh->getHandle()] == eh);
	unsigned idx = eh->index();
	assert(0 <= idx && idx < m_pollfds.size());
	struct pollfd& pfd = m_pollfds[idx];
	assert(pfd.fd == eh->getHandle());
	pfd.fd = -1;
}
		
void Poller::pollHandles(ActiveEventlist *m_activeEventlist)
{
	//std::cout << "poll" << std::endl;
	//std::cout << m_pollfds.size() << std::endl;
	int res = ::poll(&*m_pollfds.begin(), m_pollfds.size(), -1);
	//std::cout << "after poll" << res << std::endl;
	if (res > 0) {
		for (Pollfds::const_iterator pfd = m_pollfds.begin(); 
		     pfd != m_pollfds.end() && res > 0; pfd++)
		     {
				if (pfd->revents > 0) {
					--res;
					HandlerMap::const_iterator it = m_handlerMap.find(pfd->fd);
					EventHandler * eh = it->second;
					eh-> setReventType(pfd->revents);
					
					m_activeEventlist->push_back(eh); 
				}
			}
	}
}
