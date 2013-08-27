#include "ThreadPool.hpp"
#include <pthread.h>
#include <assert.h>
#include <list>
#include <queue>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <syslog.h>
using namespace tanic;


void * ThreadPool::wrapperFunc( void * arg) 
{
	Thread_t * thread = (Thread_t *) arg;			
	for ( ; 0 == thread->m_parent->m_isShutdown; ) {
		    //std::cout << "wrapperFunc\n";
			thread->m_func( thread->m_arg );
			if ( 0 != thread->m_parent->m_isShutdown) break;
			
			pthread_mutex_lock(&thread->m_mutex);
			if ( 0 == thread->m_parent->saveThread( thread )) {
				//执行完一次后，被添加进空闲队列，等待唤醒
				pthread_cond_wait(&thread->m_cond, &thread->m_mutex);
				pthread_mutex_unlock(&thread->m_mutex);
			} else {
				//空闲线程已满，活动线程不能加入到空闲线程中，只有退出
				
				pthread_mutex_lock(&thread->m_parent->m_mainMutex);
				thread->m_parent->m_total--;
				/*if (thread->m_parent->m_total < thread->m_parent->m_maxThreads) {
					pthread_cond_signal(&thread->m_parent->m_idleCond);
				}*/
				pthread_mutex_unlock(&thread->m_parent->m_mainMutex);
				delete thread;
				thread = NULL;
				break;
			}
	}
	//父线程退出时，子线程退出
	if (NULL != thread) {
		std::cout << "Thread " << thread->m_tid << "exit.\n";
		return (void*)thread->m_tid;
	}
	
}

ThreadPool::ThreadPool(int maxThreads, const char * tag)
{
	if (maxThreads <= 0) maxThreads = 2;
	pthread_mutex_init(&m_mainMutex, NULL);
	pthread_cond_init(&m_idleCond, NULL);
	pthread_cond_init(&m_fullCond, NULL);
	m_maxThreads = maxThreads;
	m_isShutdown = 0;
	m_total = 0;
	
	tag = NULL == tag ? "unkown" : tag;
}

ThreadPool::~ThreadPool()
{
	    //访问空闲队列
		pthread_mutex_lock(&m_mainMutex);
		
		unsigned index = m_threadList.size();
		
		if (index < m_total) {
			syslog( LOG_NOTICE , " waiting for %d thread(s) to finish\n", m_total - index);
			pthread_cond_wait(&m_fullCond, &m_mainMutex);
		}
		//std::cout <<"size " << m_threadList.size() << "total " << m_total << std::endl;
		m_isShutdown = 1;
		std::list<Thread_t *>::iterator it;
		
		for ( it = m_threadList.begin(); it != m_threadList.end(); it++) {
			
			pthread_mutex_lock(&((*it)->m_mutex));
			pthread_cond_signal(&((*it)->m_cond));
			pthread_mutex_unlock(&((*it)->m_mutex));
			
		}
		
		for (it = m_threadList.begin(); it != m_threadList.end(); it++) {
		
			void * r;
			pthread_join((*it)->m_tid, &r);
			std::cout << "Thread returned "<< (long)r << std::endl;
			
			
		}
		
		pthread_mutex_unlock(&m_mainMutex);
		
}

int ThreadPool::dispatch(DispatchFunc dispatchFunc, void * arg)
{
	int ret = 0;
	pthread_attr_t attr;
	Thread_t * thread = NULL;
	//需要访问队列，加锁
	pthread_mutex_lock(&m_mainMutex);
	int size = m_threadList.size();
	
	//如若没有空闲线程（size <= 0)且总线程数大于等于最大可创建线程数，则等待
	for( ; size <= 0 && m_total >= m_maxThreads; ) {
		pthread_cond_wait(&m_idleCond, &m_mainMutex);
	}
	
	if (size <= 0) {
		Thread_t * thread = new Thread_t;
		pthread_mutex_init(&thread->m_mutex, NULL);
		pthread_cond_init(&thread->m_cond, NULL);
		thread->m_tid = 0;
		thread->m_func = dispatchFunc;
		thread->m_arg = arg;
		thread->m_parent = this;
		
		//创建可分离的线程
		//pthread_attr_init(&attr);
		//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		if ( 0 == pthread_create(&(thread->m_tid), NULL, wrapperFunc, thread)) {
			m_total++;
		} else {
			ret = -1;
			delete thread;
		}
	} else {
		
		
		thread = m_threadList.front();
		thread->m_func = dispatchFunc;
		thread->m_arg = arg;
		thread->m_parent = this;
		//std::cout << "ok.\n";
		pthread_mutex_lock(&thread->m_mutex);
		pthread_cond_signal(&thread->m_cond);
		pthread_mutex_unlock(&thread->m_mutex);
		
	}
	pthread_mutex_unlock(&m_mainMutex);
	
	return ret;
}



int ThreadPool::saveThread(Thread_t * thread)
{
	int ret = -1;
	//访问空闲队列
	pthread_mutex_lock(&m_mainMutex);
	
	if (m_threadList.size() < m_maxThreads) {
		
		m_threadList.push_back(thread);
		ret = 0;
		pthread_cond_signal(&m_idleCond);
		//空闲队列大小等于总的线程数，表明所有线程都成了空闲线程
		if (m_threadList.size() == m_total) {
			pthread_cond_signal(&m_fullCond);
		}
		
	}
	
	pthread_mutex_unlock(&m_mainMutex);
	return ret;
}

		
