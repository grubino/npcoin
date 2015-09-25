#include <cstdlib>
#include <string>
#include "npcoin.pb.h"

namespace net {
struct message {

public:
  static const size_t HEADER_ADDRESS_LENGTH = 128;
  static const size_t HEADER_MESSAGE_TYPE_LENGTH = 8;
  static const size_t HEADER_BODY_LENGTH_LENGTH = 8;
  static const size_t HEADER_LENGTH = 1024;

  typedef npcoin::BlockProperties block_properties;

message() : m_body(NULL), m_body_length(0) {}
  char* header() { return m_header; }
  char* body() { return m_body; }
  bool decode_header();
  bool decode_body();
  std::size_t body_length() { return m_body_length; }
  const block_properties& block() const { return m_block; }

private:
  char m_header[HEADER_LENGTH];
  char m_message_type[HEADER_MESSAGE_TYPE_LENGTH+1];
  char m_address[HEADER_ADDRESS_LENGTH+1];
  char* m_body;
  std::size_t m_body_length;
  block_properties m_block;

};
}
