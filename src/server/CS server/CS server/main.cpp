#include <iostream>
#include "share/common.h"
#include "share/test_tcp_message.h"
#include "boost/asio/io_service.hpp"


using namespace std;
using namespace boost;

// �̴߳�����
void proc_data(socket_type sock)
{
	while (true)
	{
		try
		{
			//vector<char> str(100, 0);
			/*	sock->read_some(buffer(str));

				cout << "recv from:" << sock->remote_endpoint().address() << endl;

				cout << "data: " << &str[0] << endl;

				sock->write_some(buffer(str));*/
			cout << "///////////////////////////////////////" << endl;
			cout << "recv data from client:" << endl;
			string str = read_data(sock);
			cout << endl;

			cout << "send data to client:" << endl;
			send_data(sock, str);
			cout << "////////////////////////////////////////" << endl << endl;
		}
		catch (std::exception& e)
		{
			cout << e.what() << endl;
			break;			// �Ͽ�����
		}

	}
}

// �߳����Ӻ���
void proc_accept(io_service_pool& pool)
{
	cout << "wait connect..." << endl;

	// �첽������Ҫ����
	//io_service& ios = pool.get();

	// �˵����
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), PORT_NUM);

	// ���������������(���˵��io����)
	boost::asio::ip::tcp::acceptor acceptor(pool.get(), ep);

	while (true)
	{
		// ����socket
		socket_type sock(new boost::asio::ip::tcp::socket(pool.get()));

		// ͬ����������(�൱�ڷ����µ�socket������)
		acceptor.accept(*sock);

		// Ϊÿһ���������ӵĿͻ��˽���һ���̴߳������ݿͻ���
		// ��Ϊ�ǲ���ͬ���������Ի���ʹ���߳��������첽����
		// ����첽�Ļ��Ͳ���Ҫ�����߳����������ݵĶ�ȡ��ֱ�Ӵ���
		thread t(proc_data, sock);
	}
}

int main(int argc, char *argv[])
{
	// ���������̳߳�
	boost::shared_ptr<io_service_pool> ios_pool = boost::make_shared<io_service_pool>(4);

	// �����̣߳� �Լ������̴߳�����
	proc_accept(*ios_pool);

	ios_pool->run();

	// �������ӵ��߳�
	//thread t1(proc_accept);
	//t1.join();
	return 0;
}