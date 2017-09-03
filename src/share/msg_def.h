#ifndef _MSGDEF_H
#define _MSGDEF_H

#include <boost/cstdint.hpp>

// �������󳤶�
#define  MAX_MSG_SIZE 1024

using boost::uint32_t;
typedef boost::uint32_t uint32_t;

struct msg_head
{
	uint32_t type;		// ��Ϣͷ����
	uint32_t size;		// ��Ϣͷ��С
	uint32_t chksum;	// ��ϢУ��
};




#endif