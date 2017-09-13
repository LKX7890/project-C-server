#include "io_service_pool.h"



io_service_pool::io_service_pool(int n /*= 4*/) : m_next_io_services(0)
{
	// ���ٳ�ʼ��һ��io_service
	if (n <= 0)
	{
		n = 1;
	}

	// �����߳�ʹ�õ�io_service����
	init(n);
}

// ��ѯ��ȡ
io_service_pool::ios_type& io_service_pool::get()
{
	if (m_next_io_services >= m_io_services.size())
	{
		m_next_io_services = 0;
	}

	return m_io_services[m_next_io_services++];
}

// �������¼�ѭ��
void io_service_pool::start()
{
	// �߳���û�߳�ʱ�����߳�
	if (m_threads.size() > 0)
	{
		return;
	}

	// ����io_services�������̰߳�
	BOOST_FOREACH(ios_type& ios, m_io_services)
	{
		// �����̣߳�runΪio_serviceִ�к���
		m_threads.create_thread(boost::bind(&ios_type::run, boost::ref(ios)));
	}
}

void io_service_pool::run()
{
	start();

	// �ȴ�
	m_threads.join_all();
}

void io_service_pool::stop()
{
	m_works.clear();

	// ����io_service,ʹ��ʹ�ó�Ա����ֹͣ
	std::for_each(m_io_services.begin(), m_io_services.end(), boost::bind(&ios_type::stop, _1));
}

void io_service_pool::init(int n)
{
	for (int i = 0; i < n; ++n)
	{
		// ���ù����������󴴽�
		m_io_services.push_back(boost::factory<ios_type*>()());

		// �Ž�����������
		m_works.push_back(boost::factory<work_type*>()(m_io_services.back()));
	}
}
