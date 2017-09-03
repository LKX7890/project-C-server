#include "share/common.h"
#include "share/test_tcp_message.h"
#include "boost/asio/io_service.hpp"
#include "boost/asio/deadline_timer.hpp"

using namespace std;
using namespace boost::asio;

void proc_connect(socket_type sock, boost::asio::ip::tcp::endpoint& ep)
{
	bool bConnet = false;

	// 阻塞timer 
	boost::asio::io_service ios;
	boost::asio::deadline_timer timer(ios, boost::posix_time::seconds(1));
	timer.wait();  // 进行同步等待
	sock->connect(ep);

	while (true)
	{
		try
		{
			if (!bConnet)
			{
				sock->connect(ep);
			}
			else
			{
				cout << endl << "please input:";
				string str;
				cout << "input: " << endl;
				cin >> str;
				//sock->write_some(buffer(str);
				send_data(sock, str);

				/*vector<char> str(100, 0);
				sock->read_some(buffer(strBuf));
				cout << "data from server: " << &strBuf[0] << end;*/

				read_data(sock);
			}

			bConnet = true;
		}
		catch (std::exception& e)
		{
			bConnet = false;
			cout << e.what();
		}
	}
}

int main(int argc, char *argv[])
{
	//io_service ios;

	// 创建并发线程池
	boost::shared_ptr<io_service_pool> ios_pool = boost::make_shared<io_service_pool>(4);

	socket_type sock(new boost::asio::ip::tcp::socket(ios_pool->get()));

	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), PORT_NUM);

	// 连接服务器线程
	//thread t1(proc_connect,sock, ep));
	proc_connect(sock, ep);
	ios_pool->run();

	//t1.join();
	return 0;
}
