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

enum block_status {
  BLOCK_VALID_UNKNOWN      =    0
  // parsed, version ok, hash satisfies claimed PoW,
  // 1 <= vtx count <= max, timestamp not in future
  , BLOCK_VALID_HEADER       =    1 
  // parent found, difficulty matches, timestamp >= median previous, checkpoint
  , BLOCK_VALID_TREE         =    2
  // only first tx is coinbase, 2 <= coinbase input script length <= 100,
  // transactions valid, no duplicate txids, sigops, size, merkle root
  , BLOCK_VALID_TRANSACTIONS =    3
  // outputs do not overspend inputs, no double spends, 
  // coinbase output ok, immature coinbase spends, BIP30
  , BLOCK_VALID_CHAIN        =    4
  // scripts/signatures ok
  , BLOCK_VALID_SCRIPTS      =    5
  , BLOCK_VALID_MASK         =    7
  // full block available in blk*.dat
  , BLOCK_HAVE_DATA          =    8
  // undo data available in rev*.dat
  , BLOCK_HAVE_UNDO          =   16
  , BLOCK_HAVE_MASK          =   24
  // stage after last reached validness failed
  , BLOCK_FAILED_VALID       =   32
  // descends from failed block
  , BLOCK_FAILED_CHILD       =   64
  , BLOCK_FAILED_MASK        =   96
};

struct block_transaction {
  std::vector<big_integer> in;
  std::string script;
  size_t version;
};

struct block_properties {
  std::map<std::string, script_ast::numeric_constant_> substitutions;
  std::string work_expr;
  std::vector<block_transaction> transactions;
  std::vector<big_integer> merkle_tree;
  big_integer tour_order_hash;
  big_integer merkle_root_hash;
  big_integer prev_block_hash;
  size_t height;
  size_t transaction_count;
  size_t time;
  size_t difficulty;
  block_status status;
  size_t version;
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
      ar & v.status;
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
