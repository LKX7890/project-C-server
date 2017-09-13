#include "io_service_pool.h"



io_service_pool::io_service_pool(int n /*= 4*/) : m_next_io_services(0)
{
	// 至少初始化一个io_service
	if (n <= 0)
	{
		n = 1;
	}

	// 创建线程使用的io_service对象
	init(n);
}

// 轮询获取
io_service_pool::ios_type& io_service_pool::get()
{
	if (m_next_io_services >= m_io_services.size())
	{
		m_next_io_services = 0;
	}

	return m_io_services[m_next_io_services++];
}

// 非阻塞事件循环
void io_service_pool::start()
{
	// 线程组没线程时创建线程
	if (m_threads.size() > 0)
	{
		return;
	}

	// 遍历io_services容器跟线程绑定
	BOOST_FOREACH(ios_type& ios, m_io_services)
	{
		// 创建线程，run为io_service执行函数
		m_threads.create_thread(boost::bind(&ios_type::run, boost::ref(ios)));
	}
}

void io_service_pool::run()
{
	start();

	// 等待
	m_threads.join_all();
}

void io_service_pool::stop()
{
	m_works.clear();

	// 遍历io_service,使其使用成员函数停止
	std::for_each(m_io_services.begin(), m_io_services.end(), boost::bind(&ios_type::stop, _1));
}

void io_service_pool::init(int n)
{
	for (int i = 0; i < n; ++n)
	{
		// 采用工厂函数对象创建
		m_io_services.push_back(boost::factory<ios_type*>()());

		// 放进包含对象里
		m_works.push_back(boost::factory<work_type*>()(m_io_services.back()));
	}
}
