#ifndef _IO_SERVICE_POOL_H
#define _IO_SERVICE_POOL_H

#include "common.h"

#include <algorithm>					// std  �㷨
#include <boost/foreach.hpp>			// ����
#include <boost/bind.hpp>				// ��
#include <boost/ref.hpp>				// ��ȷ��������
#include <boost/functional/factory.hpp>	// ����
#include <boost/ptr_container/ptr_vector.hpp>		// ����

class io_service_pool : boost::noncopyable
{
public:

	typedef boost::asio::io_service ios_type;					// io��������
	typedef boost::asio::io_service::work work_type;			// ȷ��io_service��ȷ���ж���
	typedef boost::ptr_vector<ios_type> io_services_type;
	typedef boost::ptr_vector<work_type> works_type;
	
public:

	explicit  io_service_pool(int n = 4);	// �����߳���(������ʽת�����캯��)

	ios_type& get();		// ��ѯ���������ⲿ��ȡio_service����,�������acceptor����
	void start();			// �̳߳�����
	void run();				// �̳߳�����
	void stop();			// �̳߳�ֹͣ

private:

	void init(int n);			// ����ϵ��io_services���󲢼���m_io_service ������

private:

	io_services_type m_io_services;
	works_type m_works;						// ��֤io_services��������
	boost::thread_group m_threads;			// �߳���
	std::size_t m_next_io_services;			// ��ѵʹ��
};


#endif