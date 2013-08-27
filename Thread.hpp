#ifndef _TANIC_THREAD_HPP
#define _TANIC_THREAD_HPP
#include <pthread.h>
namespace tanic
{
	class Thread
	{
	public:
	    typedef void (*ThreadFunc)(void *); //线程实际执行的函数，也是暴露的接口
		Thread(ThreadFunc, void *);
		~Thread();
		//void start();
		void join();
		bool started() const { return m_started;}
		static int getNumCreated() { return m_numCreated; }
		
	private:
	    //static void * wrapperFunc(void *);
	   
		bool m_started;
		bool m_joined;
		pthread_t m_pid;
		
		ThreadFunc m_func;
	    void * m_argv;
	    
		static int m_numCreated;
	};
}
#endif
