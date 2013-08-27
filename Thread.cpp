#include "Thread.hpp"
#include <assert.h>
#include <unistd.h>
using namespace tanic;

int Thread::m_numCreated = 0;
/*
void* Thread::wrapperFunc(void *obj)
{
	Thread * thread = static_cast<Thread*>(obj);
	thread->m_func();
	return NULL;
}
*/
Thread::Thread(ThreadFunc func, void * argv)
     :m_started(false),
     m_joined(false),
     m_func(func),
     m_argv(argv)
{
	m_numCreated++;
	
}

Thread::~Thread()
{
	if (m_started && !m_joined) {
		pthread_detach(pthread_self());
	}
}

void Thread::join()
{
	pthread_join(m_pid, NULL);
}


	
	

