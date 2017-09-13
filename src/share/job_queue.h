#ifndef _JOB_QUEUE_H
#define _JOB_QUEUE_H

#include <queue>
#include <boost/noncopyable.hpp>			// ��ֹ����
#include <boost/utility/value_init.hpp>		// �κ����ͳ�ʼ����û��ʼ���ģ���ʱ��һ��ֵ
#include <boost/concept_check.hpp>			// ������
#include <boost/thread.hpp>					// �߳̿�

template<typename Job>
class job_queue : boost::noncopyable
{
public:

	typedef Job Job_type;
	typedef std::queue<Job_type> queue_type;					// ������������
	typedef boost::mutex mutex_type;							// ������
	typedef typename mutex_type::scoped_lock lock_type;			// ��
	typedef boost::condition_variable_any condition_type;		// ��������

	BOOST_CONCEPT_ASSERT((boost::SGIAssignable<Job_type>));		// �Ƿ���Ը�ֵ���
	BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<Job_type>)); // ������

public:

	job_queue();
	void push(const Job_type& x);
	Job_type pop();
	void stop();

private:

	queue_type m_queue;		// (���͹���)��������
	mutex_type m_mutex;		// ������
	condition_type m_hasJob;// ��������
	bool m_stop_flag;
};

template<typename Job>
job_queue<Job>::job_queue() : m_stop_flag(false)
{

}

template<typename Job>
void job_queue<Job>::push(const job_queue::Job_type& x)
{
	lock_type lock(m_mutex);		// ��ס������
	m_queue.push_back(x);			// ѹ�����
	m_hasJob.notify_one();			// ֪ͨ
}

template<typename Job>
typename job_queue<Job>::Job_type job_queue<Job>::pop()
{
	lock_type lock(m_mutex);

	// Ϊ�ղ�û����
	while (m_queue.empty() && !m_stop_flag)
	{
		// �ȴ�
		m_hasJob.wait(m_mutex);
	}

	if (m_stop_flag)
	{
		// ����Job_type��ʱ����
		return boost::initialized_value;
	}

	if (m_queue.empty())
	{
		return boost::initialized_value;
	}

	// ����ǰ���
	Job_type tmp = m_queue.front();
	m_queue.pop_front();
	return tmp;
}

template<typename T>
void job_queue<T>::stop()
{
	m_stop_flag = true;
	m_hasJob.notify_all();	// ֪ͨ���еȴ����߳�
}

#endif