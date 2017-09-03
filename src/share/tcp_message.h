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

	typedef boost::function<void(tcp_message*)>destroy_type;			// function类型 销毁器类型

	typedef char char_type;												// 消息头类型
	typedef boost::array<char_type, sizeof(msg_head)> head_data_type;

	typedef boost::array<char_type, MAX_MSG_SIZE> body_data_type;		// 消息体类型

public:

	template<typename Func>												// 模板构造函数
	tcp_message(Func func) : m_destroy(func)
	{
	}

	tcp_message() {}

	head_data_type& head_data();   // 访问消息头
	msg_head *get_head();
	body_data_type& msg_data();
	bool check_head();				// 检查消息头是否正确
	void set_msg_crc();				// 设置校验核
	bool check_msg_crc();			// 检查校验核
	void destroy();
private:

	destroy_type m_destroy;			// 销毁器
	head_data_type m_head;			// 消息头
	body_data_type m_msg;			// 消息
};


#endif