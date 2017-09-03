#ifndef _TEST_MSG_H
#define _TEST_MSG_H

#include "common.h"
#include "tcp_message.h"
#include "boost\asio\buffer.hpp"


void print_msg(boost::shared_ptr<tcp_message> pMsg)
{
	cout << "type:" << pMsg->get_head()->type << endl;
	cout << "size:" << pMsg->get_head()->size << endl;
	cout << "chksum:" << pMsg->get_head()->chksum << endl;
	cout << "data:" << pMsg->msg_data().data() << endl;
}

void send_data(socket_type sock, const std::string& str)
{
	// 工厂方法创建对象
	boost::shared_ptr<tcp_message>pSendMsg =
		boost::make_shared<tcp_message>();

	pSendMsg->get_head()->size = str.length() + 1;
	pSendMsg->get_head()->type = 999;

	memcpy(pSendMsg->msg_data().begin(), str.c_str(), str.length() + 1);
	pSendMsg->set_msg_crc();

	sock->write_some(boost::asio::buffer(pSendMsg->head_data().data(), pSendMsg->head_data().size()));

	sock->write_some(boost::asio::buffer(pSendMsg->head_data().data(), pSendMsg->get_head()->size));

	print_msg(pSendMsg);
}

std::string read_data(socket_type sock)
{
	boost::shared_ptr<tcp_message> pRecvMsg = boost::make_shared<tcp_message>();
	sock->read_some(boost::asio::buffer(pRecvMsg->head_data().data(), pRecvMsg->head_data().size()));
	sock->read_some(boost::asio::buffer(pRecvMsg->msg_data().data(), pRecvMsg->get_head()->size));

	bool bFlag = pRecvMsg->check_msg_crc();

	if (bFlag)
	{
		print_msg(pRecvMsg);
	}
	else
	{
		return "";
	}

	return pRecvMsg->msg_data().data();
}
#endif