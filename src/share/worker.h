#ifndef _WORKER_H
#define _WORKER_H

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include "job_queue.h"

using namespace std;

template<typename Queue>
class worker
{
public:

	typedef Queue queue_type;
	typedef typename Queue::Job_type job_type;
	typedef boost::funciton<bool(job_type &)> func_type;

public:

	template<typename Func>
	worker(queue_type& q, Func func, int n = 1);

	worker(queue_type& q, int n = 1);

	template<typename Func>				// 提供外部设置func的接口
	void set_workerFunc(Func func)
	{
		m_func = func
	}

	void start();
	void run();
	void stop();
private:

	void do_work();						// 循环处理工作队列函数
private:
	queue_type &m_queue;				// 关联任务队列（不可拷贝）
	func_type m_func;					// 回调处理函数
	int m_nThreadNum;					// 线程数
	boost::thread_group m_threads;		// 线程组
};

template<typename Queue>
template<typename Func>
worker<Queue>::worker(typename worker<Queue>::queue_type &q, Func func, int n = 1) :
m_queue(q), m_func(func), m_nThreadNum(n)
{
	if (m_nThreadNum < 1)
	{
		// 默认值
		m_nThreadNum = 1;
	}
}

template<typename Queue>
worker<Queue>::worker(typename worker<Queue>::queue_type &q, int n = 1) :
m_queue(q), m_nThreadNum(n)
{
	if (m_nThreadNum < 1)
	{
		m_nThreadNum = 1;
	}
}

template<typename Queue>
void worker<Queue>::do_work()
{
	while (true)
	{
		job_type job = m_queue.pop();

		if (!m_func)
		{
			// 没传入func不用操作
			break;
		}
	}
}

template<typename Queue>
void worker<Queue>::start()
{
	if (!m_func)
	{
		return;
	}

	if (m_threads.size() > 0)
	{
		return;
	}

	for (int i = 0; i < m_nThreadNum; ++i)
	{
		m_threads.create_thread(boost::bind(&worker::do_work, this));
	}
}

template<typename Queue>
void worker::run()
{
	start();
	m_threads.join_all();
}

template<typename Queue>
void worker<Queue>::stop()
{
	m_func = 0;
	m_queue.stop();
}

#endif