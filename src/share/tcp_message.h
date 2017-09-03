#ifndef _TCPMESSAGE_H
#define _TCPMESSAGE_H

#include "msg_def.h"
#include <boost/checked_delete.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/crc.hpp>

using namespace boost;

class tcp_message : boost::noncopyable
{
public:

	typedef boost::function<void(tcp_message*)>destroy_type;			// function���� ����������

	typedef char char_type;												// ��Ϣͷ����
	typedef boost::array<char_type, sizeof(msg_head)> head_data_type;

	typedef boost::array<char_type, MAX_MSG_SIZE> body_data_type;		// ��Ϣ������

public:

	template<typename Func>												// ģ�幹�캯��
	tcp_message(Func func) : m_destroy(func)
	{
	}

	tcp_message() {}

	head_data_type& head_data();   // ������Ϣͷ
	msg_head *get_head();
	body_data_type& msg_data();
	bool check_head();				// �����Ϣͷ�Ƿ���ȷ
	void set_msg_crc();				// ����У���
	bool check_msg_crc();			// ���У���
	void destroy();
private:

	destroy_type m_destroy;			// ������
	head_data_type m_head;			// ��Ϣͷ
	body_data_type m_msg;			// ��Ϣ
};


#endif