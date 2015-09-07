#ifndef GRAMMAR_H_
#define GRAMMAR_H_
  
#define BOOST_SPIRIT_DEBUG_OUT std::cerr
#define BOOST_SPIRIT_DEBUG 1

#include "script/ast.h"
#include <boost/spirit/include/qi.hpp>
#include <string>
#include <map>

script_ast::logical_expr_ parse_logical_expr(const std::string& s);

typedef std::string::iterator script_iterator;
typedef std::map<std::string, script_ast::numeric_constant_> substitution_map;

struct script_grammar 
  : boost::spirit::qi::grammar<script_iterator, script_ast::logical_expr_(), boost::spirit::ascii::space_type> {

  script_grammar();
  
  boost::spirit::qi::rule<script_iterator, script_ast::bitxor_(), boost::spirit::ascii::space_type> bitxor__;
  boost::spirit::qi::rule<script_iterator, script_ast::bitor_(), boost::spirit::ascii::space_type> bitor__;
  boost::spirit::qi::rule<script_iterator, script_ast::bitand_(), boost::spirit::ascii::space_type> bitand__;
  boost::spirit::qi::rule<script_iterator, script_ast::shiftleft_(), boost::spirit::ascii::space_type> shiftleft__;
  boost::spirit::qi::rule<script_iterator, script_ast::shiftright_(), boost::spirit::ascii::space_type> shiftright__;
  boost::spirit::qi::rule<script_iterator, script_ast::logical_expr_(), boost::spirit::ascii::space_type> logical_expr__;
  boost::spirit::qi::rule<script_iterator, script_ast::numeric_expr_(), boost::spirit::ascii::space_type> numeric_expr__;
  boost::spirit::qi::rule<script_iterator, script_ast::add_(), boost::spirit::ascii::space_type> add__;
  boost::spirit::qi::rule<script_iterator, script_ast::subtract_(), boost::spirit::ascii::space_type> subtract__;
  boost::spirit::qi::rule<script_iterator, script_ast::multiply_(), boost::spirit::ascii::space_type> multiply__;
  boost::spirit::qi::rule<script_iterator, script_ast::divide_(), boost::spirit::ascii::space_type> divide__;
  boost::spirit::qi::rule<script_iterator, script_ast::modulo_(), boost::spirit::ascii::space_type> modulo__;
  boost::spirit::qi::rule<script_iterator, script_ast::negate_(), boost::spirit::ascii::space_type> negate__;
  boost::spirit::qi::rule<script_iterator, script_ast::abs_(), boost::spirit::ascii::space_type> abs__;
  boost::spirit::qi::rule<script_iterator, script_ast::invert_(), boost::spirit::ascii::space_type> invert__;
  boost::spirit::qi::rule<script_iterator, script_ast::not_(), boost::spirit::ascii::space_type> not__;
  boost::spirit::qi::rule<script_iterator, script_ast::and_(), boost::spirit::ascii::space_type> and__;
  boost::spirit::qi::rule<script_iterator, script_ast::or_(), boost::spirit::ascii::space_type> or__;
  boost::spirit::qi::rule<script_iterator, script_ast::numeric_constant_(), boost::spirit::ascii::space_type> numeric_constant__;
  boost::spirit::qi::rule<script_iterator, script_ast::equal_(), boost::spirit::ascii::space_type> equal__;
  boost::spirit::qi::rule<script_iterator, script_ast::less_(), boost::spirit::ascii::space_type> less__;
  boost::spirit::qi::rule<script_iterator, script_ast::less_equal_(), boost::spirit::ascii::space_type> less_equal__;
  boost::spirit::qi::rule<script_iterator, script_ast::greater_(), boost::spirit::ascii::space_type> greater__;
  boost::spirit::qi::rule<script_iterator, script_ast::greater_equal_(), boost::spirit::ascii::space_type> greater_equal__;
  boost::spirit::qi::rule<script_iterator, script_ast::_true_(), boost::spirit::ascii::space_type> true__;
  boost::spirit::qi::rule<script_iterator, script_ast::_false_(), boost::spirit::ascii::space_type> false__;
  boost::spirit::qi::rule<script_iterator, script_ast::variable_(), boost::spirit::ascii::space_type> variable__;
  boost::spirit::qi::rule<script_iterator, script_ast::nul_(), boost::spirit::ascii::space_type> nul__;

};

#endif
