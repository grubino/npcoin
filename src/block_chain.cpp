#include "core/block_chain.h"
#include "sodiumpp/sodiumpp.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/assign.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstddef>

namespace assign = boost::assign;
namespace archive = boost::archive;

big_integer hash_block(const block_properties& blk) {

  std::stringstream ss;
  ss << blk;
  return big_integer(sodiumpp::crypto_hash(ss.str()).c_str());

}

std::ostream& operator<<(std::ostream& os
			 , const block_edge_properties&) {
  return os;
}
std::istream& operator>>(std::istream& is
			 , block_edge_properties&) {
  return is;
}

std::ostream& operator<<(std::ostream& os
			 , const block_transaction& v) {
  os << v.in.size() << std::endl;
  for(const big_integer& i : v.in) {
    os << i << std::endl;
  }
  os << std::endl 
     << v.script << std::endl
     << v.version
    ;
  return os;
}
std::istream& operator>>(std::istream& is
			 , block_transaction& v) {
  size_t in_cnt;
  is >> in_cnt;
  v.in = std::vector<big_integer>(in_cnt);
  for(size_t i = 0; i < in_cnt; ++i) {
    is >> v.in[i];
  }
  getline(is, v.script);
  is >> v.version;
  return is;
}

std::ostream& operator<<(std::ostream& os
			 , const block_properties& v) {
  return os;
}
std::istream& operator>>(std::istream& is
			 , block_properties& v) {
  return is;
}

std::ostream& operator<<(std::ostream& os
			 , const block_chain_properties& v) {
  return os;
}
std::istream& operator>>(std::istream& is
			 , block_chain_properties& v) {
  return is;
}

std::ostream& operator<<(std::ostream& os
			 , const script_ast::numeric_constant_& c) {
  os << c.value;
  return os;
}
std::istream& operator>>(std::istream& is
			 , script_ast::numeric_constant_& c) {
  is >> c.value;
  return is;
}

