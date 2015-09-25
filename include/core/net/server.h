#include <deque>
#include <boost/asio.hpp>
#include <boost/signals2.hpp>

#include "core/net/message.h"


using boost::asio::ip::tcp;

namespace net {
class np_server {

 public:
  typedef std::deque<message> message_queue;
  typedef boost::signals2::signal<void(message::block_properties)> signal_publisher;

  np_server(boost::asio::io_service& ios,
	    tcp::endpoint endpoint) : 
  m_acceptor(ios, endpoint)
    , m_socket(ios) {
    do_accept();
  }

  void connect(signal_publisher::slot_function_type f) {
    m_signal.connect(f);
  }

 private:

  void do_accept();
  void do_read_header();
  void do_read_body();

  message_queue m_queue;
  message m_incoming;
  tcp::acceptor m_acceptor;
  tcp::socket m_socket;
  signal_publisher m_signal;
  
};

}
