#include <iostream>
#include "share/common.h"
#include "share/test_tcp_message.h"
#include "boost/asio/io_service.hpp"


using namespace std;
using namespace boost;

// 线程处理函数
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
			break;			// 断开连接
		}

	}
}

// 线程连接函数
void proc_accept(io_service_pool& pool)
{
	cout << "wait connect..." << endl;

	// 异步操作需要对象
	//io_service& ios = pool.get();

	// 端点对象
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), PORT_NUM);

	// 创建连接请求对象(将端点和io连接)
	boost::asio::ip::tcp::acceptor acceptor(pool.get(), ep);

	while (true)
	{
		// 创建socket
		socket_type sock(new boost::asio::ip::tcp::socket(pool.get()));

		// 同步机制连接(相当于返回新的socket并监听)
		acceptor.accept(*sock);

		// 为每一个建立连接的客户端建立一个线程处理数据客户端
		// 因为是采用同步机制所以还是使用线程来处理异步数据
		// 如果异步的话就不需要创建线程来进行数据的读取，直接处理
		thread t(proc_data, sock);
	}
}

int main(int argc, char *argv[])
{
	// 创建并发线程池
	boost::shared_ptr<io_service_pool> ios_pool = boost::make_shared<io_service_pool>(4);

	// 创建线程， 以及传递线程处理函数
	proc_accept(*ios_pool);

	ios_pool->run();

	// 处理连接的线程
	//thread t1(proc_accept);
	//t1.join();
	return 0;
}