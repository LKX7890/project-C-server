#ifndef _COMM_H_
#define _COMM_H_

#include <iostream>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/io_service.hpp"
#include "share/io_service_pool.h"

// ¶Ë¿ÚºÅ
#define PORT_NUM 9999

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_type;

using namespace boost;
using namespace std;



#endif