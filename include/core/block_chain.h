#ifndef BLOCK_CHAIN_H_
#define BLOCK_CHAIN_H_

#include "script/ast.h"
#include "script/grammar.h"
#include "core/big_integer.h"

#include <sodiumpp/sodiumpp.h>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/graph/adjacency_list.hpp>

#include <memory>
#include <string>
#include <iostream>
#include <cstddef>

const unsigned int MAX_BLOCK_SIZE = (1 << 16);

struct block_transaction;
struct block_properties;
struct block_edge_properties;
struct block_chain_properties;

typedef 
boost::adjacency_list<
boost::listS
, boost::listS
  , boost::directedS
  , block_properties
  , block_edge_properties
  , block_chain_properties> 
  block_chain;
typedef boost::graph_traits<block_chain>::vertex_descriptor block_descriptor;
typedef std::shared_ptr<block_chain> block_chain_ptr;

struct block_transaction {
  std::vector<big_integer> in;
  std::string script;
  size_t version;
};

struct block_properties {
  std::string work_expr;
  big_integer tour_order_hash;
  big_integer merkle_root_hash;
  big_integer prev_block_hash;
  size_t height;
  size_t transaction_count;
  size_t time;
  size_t difficulty;
  size_t version;
  std::vector<big_integer> merkle_tree;
  std::map<std::string, script_ast::numeric_constant_> substitutions;
  std::vector<block_transaction> transactions;
};

struct block_edge_properties {};

struct block_chain_properties {
  boost::graph_traits<block_chain>::vertex_descriptor last_block;
  boost::multiprecision::uint512_t transaction_count;
  size_t height;
};

std::ostream& operator<<(std::ostream& os
			 , const block_edge_properties&);
std::istream& operator>>(std::istream& is
			 , block_edge_properties&);
std::ostream& operator<<(std::ostream& os
			 , const block_transaction& v);
std::ostream& operator>>(std::ostream& is
			 , block_transaction& v);
std::ostream& operator<<(std::ostream& os
			 , const block_properties& v);
std::istream& operator>>(std::istream& is
			 , block_properties& v);
std::ostream& operator<<(std::ostream& os
			 , const block_chain_properties& v);
std::istream& operator>>(std::istream& is
			 , block_chain_properties& v);
std::ostream& operator<<(std::ostream& os
			 , const script_ast::numeric_constant_& c);
std::istream& operator>>(std::istream& is
			 , script_ast::numeric_constant_& c);

namespace boost { namespace serialization {

    template <class Archive>
      void save(Archive& ar
		, const big_integer& v
		, const unsigned int version) {
      ar << v.str();
    }

    template <class Archive>
      void load(Archive& ar
		, big_integer& v
		, const unsigned int version) {
      std::string s;
      ar >> s;
      big_integer tmp(s.c_str());
      v.swap(tmp);
    }

    template <class Archive>
      void serialize(Archive& ar
		     , script_ast::numeric_constant_& c
		     , const unsigned int version) {
      ar & c.value;
    }

    template <class Archive>
      void serialize(Archive& ar
		     , block_properties& v
		     , const unsigned int version) {
      ar & v.substitutions;
      ar & v.work_expr;
      ar & v.transactions;
      ar & v.merkle_tree;
      ar & v.tour_order_hash;
      ar & v.merkle_root_hash;
      ar & v.prev_block_hash;
      ar & v.height;
      ar & v.transaction_count;
      ar & v.time;
      ar & v.difficulty;
      ar & v.version;
    }

    template <class Archive>
      void serialize(Archive& ar
		     , block_transaction& v
		     , const unsigned int version) {
      ar & v.in;
      ar & v.script;
      ar & v.version;
    }

  }
}

big_integer hash_block(const block_properties& blk);

#endif
