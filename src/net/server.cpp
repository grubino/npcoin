#include "core/net/server.h"

namespace net {

void np_server::do_accept() {

  while(true) {
    m_acceptor.async_accept(m_socket
			    , [this](boost::system::error_code ec){
			      if(!ec) {
				m_acceptor.accept(m_socket);
				do_read_header();
			      }
			    });
  }
}

void np_server::do_read_header() {
  boost::asio::async_read(m_socket,
			  boost::asio::buffer(m_incoming.header(),
					      message::HEADER_LENGTH)
			  , [this](boost::system::error_code ec, std::size_t) {
			    if(!ec && m_incoming.decode_header()) {
			      do_read_body();
			    } else {
			      m_socket.close();
			    }
			  });
}

void np_server::do_read_body() {
  boost::asio::async_read(m_socket,
			  boost::asio::buffer(m_incoming.body(),
					      m_incoming.body_length())
			  , [this](boost::system::error_code ec, std::size_t) {
			    if(!ec && m_incoming.decode_body()) {
			      m_signal(m_incoming.block());
			    } else {
			      m_socket.close();
			    }
			  });
}

}
