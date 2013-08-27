#ifndef _TANIC_THREADPOOL_HPP
#define _TANIC_THREADPOOL_HPP

#include <list>
#include <iostream>
#include "Thread.hpp"


	
namespace tanic
{
	struct tagThread;
	
	typedef struct tagThread Thread_t;
	
	class ThreadPool
	{ 
		
		friend void testFriend(ThreadPool* thp) {
			thp->m_total = 0;
		}
	public:
	    typedef void (* DispatchFunc)(void *);
	    ThreadPool(int maxThreads, const char * tag = 0);
	    ~ThreadPool();
	    int dispatch(DispatchFunc dispatchFunc, void * arg);
	    int getMaxThreads() { return m_maxThreads;}
	    int saveThread(Thread_t *);
	    
	private:
	    char *m_tag;
	    
	    unsigned m_maxThreads;//可容纳的最大线程数
	   // int m_index;
	    unsigned m_total;//目前总共的线程数，包括活动的和空闲的
	    int m_isShutdown;
	    static void * wrapperFunc(void * arg);//线程实际执行的函数(不能为类成员函数）
	    pthread_mutex_t m_mainMutex;//用于空闲队列访问的锁
	   
	    pthread_cond_t m_idleCond;//空闲队列有线程时
	    
	    pthread_cond_t m_fullCond;//空闲队列满时
	    
	    std::list<Thread_t *> m_threadList;
	};
	struct tagThread{
		pthread_t m_tid;
		pthread_mutex_t m_mutex;
		pthread_cond_t m_cond;
		ThreadPool::DispatchFunc m_func;
		void * m_arg;
		ThreadPool * m_parent;
	};
}
#endif
		
