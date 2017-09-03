#ifndef _MSGDEF_H
#define _MSGDEF_H

#include <boost/cstdint.hpp>

// 处理的最大长度
#define  MAX_MSG_SIZE 1024

using boost::uint32_t;
typedef boost::uint32_t uint32_t;

struct msg_head
{
	uint32_t type;		// 消息头类型
	uint32_t size;		// 消息头大小
	uint32_t chksum;	// 消息校验
};




#endif