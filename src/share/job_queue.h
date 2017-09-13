#ifndef _JOB_QUEUE_H
#define _JOB_QUEUE_H

#include <queue>
#include <boost/noncopyable.hpp>			// 禁止拷贝
#include <boost/utility/value_init.hpp>		// 任何类型初始化，没初始化的，临时给一个值
#include <boost/concept_check.hpp>			// 概念检查
#include <boost/thread.hpp>					// 线程库

template<typename Job>
class job_queue : boost::noncopyable
{
public:

	typedef Job Job_type;
	typedef std::queue<Job_type> queue_type;					// 容器，存数据
	typedef boost::mutex mutex_type;							// 互斥量
	typedef typename mutex_type::scoped_lock lock_type;			// 锁
	typedef boost::condition_variable_any condition_type;		// 条件变量

	BOOST_CONCEPT_ASSERT((boost::SGIAssignable<Job_type>));		// 是否可以赋值检查
	BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<Job_type>)); // 概念检查

public:

	job_queue();
	void push(const Job_type& x);
	Job_type pop();
	void stop();

private:

	queue_type m_queue;		// (泛型工作)工作队列
	mutex_type m_mutex;		// 互斥量
	condition_type m_hasJob;// 条件变量
	bool m_stop_flag;
};

template<typename Job>
job_queue<Job>::job_queue() : m_stop_flag(false)
{

}

template<typename Job>
void job_queue<Job>::push(const job_queue::Job_type& x)
{
	lock_type lock(m_mutex);		// 锁住互斥量
	m_queue.push_back(x);			// 压入队列
	m_hasJob.notify_one();			// 通知
}

template<typename Job>
typename job_queue<Job>::Job_type job_queue<Job>::pop()
{
	lock_type lock(m_mutex);

	// 为空并没结束
	while (m_queue.empty() && !m_stop_flag)
	{
		// 等待
		m_hasJob.wait(m_mutex);
	}

	if (m_stop_flag)
	{
		// 返回Job_type临时对象
		return boost::initialized_value;
	}

	if (m_queue.empty())
	{
		return boost::initialized_value;
	}

	// 返回前面的
	Job_type tmp = m_queue.front();
	m_queue.pop_front();
	return tmp;
}

template<typename T>
void job_queue<T>::stop()
{
	m_stop_flag = true;
	m_hasJob.notify_all();	// 通知所有等待的线程
}

#endif