#define BOOST_TEST_MODULE script_io_test

#define BOOST_SPIRIT_DEBUG_OUT std::cerr
#define BOOST_SPIRIT_DEBUG 1

#include <boost/test/included/unit_test.hpp>

#include "script/visitor.h"
#include "script/ast.h"
#include "script/grammar.h"
#include "script/generator.h"
#include "core/block_chain.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant.hpp>
#include <boost/assign.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <sstream>
#include <iostream>
#include <utility>

#include <cstdlib>

using namespace std;
using namespace boost;
using namespace script_ast;

struct redirect_guard {
public:
  redirect_guard(std::streambuf* new_buffer) 
    : old(std::cerr.rdbuf(new_buffer)) {}
  ~redirect_guard() { std::cerr.rdbuf(old); }
private:
  std::streambuf* old;
};

BOOST_AUTO_TEST_CASE(test_expr_parse_generate) {

  using namespace boost::spirit;
  
  std::vector<std::string> exprs = assign::list_of
    ("true")
    ("!((#a<$b and #c<#d))")
    ("(true and $x>#b)")
    ;

  script_grammar grammar;
  script_generator<back_insert_iterator<string> > generator;
  std::vector<script_ast::logical_expr_> expr_asts;

  for(string& e : exprs) {
    
    stringstream buffer;
    redirect_guard guard(buffer.rdbuf());
    script_ast::logical_expr_ ast;

    bool parse_result = qi::phrase_parse(e.begin()
					 , e.end()
					 , grammar
					 , ascii::space
					 , ast);
    expr_asts.push_back(ast);
    
    BOOST_CHECK_MESSAGE(parse_result, buffer.str());
    
  }

  vector<string> generated_strings;

  for(script_ast::logical_expr_& e : expr_asts) {
    
    stringstream buffer;
    redirect_guard guard(buffer.rdbuf());
    string generated_string;
    bool generate_result = karma::generate(std::back_inserter(generated_string)
					   , generator
					   , e);
    generated_strings.push_back(generated_string);
    BOOST_CHECK_MESSAGE(generate_result, buffer.str());

  }

  for(size_t i = 0; i < exprs.size(); ++i) {
    BOOST_CHECK_MESSAGE(generated_strings[i] == exprs[i]
			, generated_strings[i] << " != " << exprs[i]);
  }

}

BOOST_AUTO_TEST_CASE(test_work_expr_verification) {

  using namespace boost::spirit;
  
  std::string expr("!((#0<$b and $c<#10))");

  script_grammar grammar;
  script_generator<back_insert_iterator<string> > generator;
  script_ast::logical_expr_ expr_ast;

  stringstream buffer;
  redirect_guard guard(buffer.rdbuf());

  bool parse_result = qi::phrase_parse(expr.begin()
				       , expr.end()
				       , grammar
				       , ascii::space
				       , expr_ast);
    
  BOOST_CHECK_MESSAGE(parse_result, buffer.str());

  substitution_map subs;
  subs["b"] = {"9"};
  subs["c"] = {"20"};
  comparison_visitor v(subs);

  BOOST_CHECK_MESSAGE(boost::apply_visitor(v, expr_ast)
		      , "the expression was not satisfied");

}

BOOST_AUTO_TEST_CASE(test_block_chain_serialize) {

  std::string expr_str("!((#a<$b and #c<#d))");

  numeric_constant_ c = {"abcd"};
  substitution_map subst = 
    assign::map_list_of("b", c);
  std::vector<big_integer> ins = 
    assign::list_of
    (big_integer("1231"))
    (big_integer("2837"))
    ;

  block_transaction tx = { ins, "(#a<#b)", 1 };
  vector<block_transaction> txs = 
    assign::list_of(tx);

  vector<big_integer> mtree =
    assign::list_of
    (big_integer("1237"))
    (big_integer("1234"))
    (big_integer("1283"))
    (big_integer("1823"))
    ;

  block_properties props = {
    expr_str
    , big_integer("192837")
    , big_integer("192873")
    , big_integer("98734")
    , 0
    , 1
    , static_cast<unsigned long>(time(NULL))
    , 1
    , 1
    , mtree
    , subst
    , txs
  };

  std::stringstream ss;
  archive::text_oarchive oa(ss);
  oa << props;

  block_properties read_back;
  archive::text_iarchive ia(ss);
  ia >> read_back;

  for(const substitution_map::value_type& v : read_back.substitutions) {
    const std::string& label = v.first;
    const std::string& value = v.second.value;
    BOOST_CHECK_MESSAGE(value == props.substitutions[label].value
			, "expected: " << props.substitutions[label].value
			<< ", got: " << value);
  }
  BOOST_CHECK_MESSAGE(read_back.work_expr == props.work_expr
		      , "expected: " << props.work_expr
		      << ", got: " << read_back.work_expr);
  BOOST_CHECK_MESSAGE(read_back.transactions.size() == props.transactions.size()
		      , "expected transaction count: " 
		      << props.transactions.size()
		      << "observed count: " 
		      << read_back.transactions.size());
  for(size_t i = 0; i < read_back.transactions.size(); ++i) {

    const block_transaction& expected = props.transactions[i];
    const block_transaction& actual = read_back.transactions[i];

    BOOST_CHECK_MESSAGE(expected.script == actual.script
			, "expected: " << expected.script
			<< ", got: " << actual.script);
    BOOST_CHECK_MESSAGE(expected.version == actual.version
			, "expected: " << expected.script
			<< ", got: " << actual.script);
    BOOST_CHECK_MESSAGE(actual.in.size() == expected.in.size()
			, "transaction inputs list length: "
			<< actual.in.size());
    for(size_t j = 0; j < expected.in.size(); ++j) {
      if(actual.in.size() > j) {
	BOOST_CHECK_MESSAGE(actual.in[j] == expected.in[j]
			    , "expected: " << expected.in[j]
			    << ", got: " << actual.in[j]);
      }
    }
    
  }

}
