#include "core/net/message.h"
#include <cstring>
#include <string>

namespace net {

bool message::decode_header() {

  char* ptr = m_header;

  std::strncpy(ptr, m_address, HEADER_ADDRESS_LENGTH);
  m_address[HEADER_ADDRESS_LENGTH] = '\0';
  ptr += HEADER_ADDRESS_LENGTH;

  std::strncpy(ptr, m_message_type, HEADER_MESSAGE_TYPE_LENGTH);
  m_message_type[HEADER_MESSAGE_TYPE_LENGTH] = '\0';
  ptr += HEADER_MESSAGE_TYPE_LENGTH;

  char raw_body_length[HEADER_BODY_LENGTH_LENGTH+1];
  std::strncpy(ptr, raw_body_length, HEADER_BODY_LENGTH_LENGTH);
  raw_body_length[HEADER_BODY_LENGTH_LENGTH] = '\0';
  ptr += HEADER_BODY_LENGTH_LENGTH;

  if(m_body != NULL) {
    delete [] m_body;
  }
  m_body_length = atoi(raw_body_length);
  m_body = new char[m_body_length+1];
  m_body[m_body_length] = '\0';
  
  return true;

}

bool message::decode_body() {

  bool result = m_block.ParseFromString(m_body);
  m_body_length = 0;
  delete [] m_body;
  m_body = NULL;
  return result;
  
}

}
