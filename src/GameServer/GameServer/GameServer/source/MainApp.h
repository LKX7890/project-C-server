#ifndef _MAINAPP_H
#define _MAINAPP_H

#include <boost/pool/detail/singleton.hpp>
using boost::details::pool::singleton_default;

#include "MServer.h"

class MainApp
{
public:
	MainApp();
	~MainApp();
public:
	void InitAppInstance();
	void SendPacket(tcp_session_ptr& session, serial_packet& packet);
private:
	void StartServer();
private:
	boost::shared_ptr<MServer> m_pServer;
};

//��������
typedef singleton_default<MainApp> theApp;


#endif
