#ifndef _IO_SERVICE_POOL_H
#define _IO_SERVICE_POOL_H

#include "common.h"

#include <algorithm>					// std  算法
#include <boost/foreach.hpp>			// 遍历
#include <boost/bind.hpp>				// 绑定
#include <boost/ref.hpp>				// 明确传入引用
#include <boost/functional/factory.hpp>	// 工厂
#include <boost/ptr_container/ptr_vector.hpp>		// 容器

class io_service_pool : boost::noncopyable
{
public:

	typedef boost::asio::io_service ios_type;					// io操作对象
	typedef boost::asio::io_service::work work_type;			// 确保io_service正确运行对象
	typedef boost::ptr_vector<ios_type> io_services_type;
	typedef boost::ptr_vector<work_type> works_type;
	
public:

	explicit  io_service_pool(int n = 4);	// 创建线程数(不能隐式转换构造函数)

	ios_type& get();		// 轮询方法，从外部获取io_service对象,用于组成acceptor对象
	void start();			// 线程池启动
	void run();				// 线程池运行
	void stop();			// 线程池停止

private:

	void init(int n);			// 创建系列io_services对象并加入m_io_service 容器中

private:

	io_services_type m_io_services;
	works_type m_works;						// 保证io_services正常运行
	boost::thread_group m_threads;			// 线程组
	std::size_t m_next_io_services;			// 轮训使用
};


#endif