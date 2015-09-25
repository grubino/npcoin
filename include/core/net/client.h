#include <boost/asio.hpp>
#include <deque>

#include "core/net/message.h"


using boost::asio::ip::tcp;

namespace net {

class np_client {
 public:
  np_server(boost::asio::io_service& ios,
	    tcp::endpoint endpoint) : 
  m_acceptor(ios, endpoint)
    , m_socket(ios) {
    do_accept();
  }

 private:

  void do_accept() {
    while(true) {
      m_acceptor.async_accept(m_socket
			      , [this](boost::system::error_code ec){
				if(!ec) {
				  m_acceptor.accept(m_socket);
				  handle_message();
				}
			      });
    }
  }

  void handle_message() {
    std::vector<char> incoming;
    boost::system::error_code err;
    boost::asio::async_read(m_socket, boost::asio::buffer(incoming), err);
  }
  tcp::acceptor m_acceptor;
  tcp::socket m_socket;
};

}
