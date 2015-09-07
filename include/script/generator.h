#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "script/ast.h"
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <string>

template <class OutputIterator>
struct script_generator 
  : boost::spirit::karma::grammar<OutputIterator, script_ast::logical_expr_()> {

  script_generator() 
    : script_generator::base_type(logical_expr__) {

    using boost::spirit::ascii::string;
    using boost::spirit::lit;
    using boost::spirit::karma::_1;
    using boost::spirit::karma::_2;
    using boost::spirit::karma::_val;
    using boost::phoenix::at_c;
    using namespace boost::spirit;
    using namespace script_ast;
    
    logical_expr__ %= 
      nul__
      | true__ | false__
      | equal__ 
      | greater__ 
      | greater_equal__ 
      | less__ 
      | less_equal__
      | not__ | and__ | or__;
    not__ %= "!(" << logical_expr__ << ")";
    and__ %= "(" << logical_expr__ << " and " << logical_expr__ << ")";
    or__ %= "(" << logical_expr__ << " or " << logical_expr__ << ")";
    true__ = eps << "true";
    false__ = eps << "false";
    equal__ %= numeric_expr__ << "==" << numeric_expr__;
    less__ %= numeric_expr__ << "<" << numeric_expr__;
    less_equal__ %= numeric_expr__ << "<=" << numeric_expr__;
    greater__ %= numeric_expr__ << ">" << numeric_expr__;
    greater_equal__ %= numeric_expr__ << ">=" << numeric_expr__;
    
    invert__ = "~(" << numeric_expr__[_1 = at_c<0>(_val)] << ")";
    bitxor__ %= "(" << numeric_expr__ << "^" << numeric_expr__ << ")";
    bitor__ %= "(" << numeric_expr__ << "|" << numeric_expr__ << ")";
    bitand__ %= "(" << numeric_expr__ << "&" << numeric_expr__ << ")";
    shiftleft__ %= "(" << numeric_expr__ << "<<" << numeric_expr__ << ")";
    shiftright__ %= "(" << numeric_expr__ << "<<" << numeric_expr__ << ")";
    
    numeric_expr__ %= 
      nul__ | numeric_constant__ | variable__
      | invert__
      | negate__ 
      | abs__ 
      | add__ 
      | subtract__ 
      | multiply__ 
      | divide__ 
      | modulo__
      | bitxor__ 
      | bitor__ 
      | bitand__ 
      | shiftleft__ 
      | shiftright__
      ;
    negate__ %= "-(" << numeric_expr__ << ")";
    abs__ %= "+(" << numeric_expr__ << ")";
    add__ %= "(" << numeric_expr__ << "+" << numeric_expr__ << ")";
    subtract__ %= "(" << numeric_expr__ << "-" << numeric_expr__ << ")";
    multiply__ %= "(" << numeric_expr__ << "*" << numeric_expr__ << ")";
    divide__ %= "(" << numeric_expr__ << "/" << numeric_expr__ << ")";
    modulo__ %= "(" << numeric_expr__ << "%" <<numeric_expr__ << ")";
    numeric_constant__ = "#" << string[_1 = at_c<0>(_val)];
    variable__ = "$" << string[_1 = at_c<0>(_val)];
    nul__ %= eps(false);

    BOOST_SPIRIT_DEBUG_NODE(nul__);
    BOOST_SPIRIT_DEBUG_NODE(variable__);
    BOOST_SPIRIT_DEBUG_NODE(false__);
    BOOST_SPIRIT_DEBUG_NODE(true__);
    BOOST_SPIRIT_DEBUG_NODE(greater_equal__);
    BOOST_SPIRIT_DEBUG_NODE(greater__);
    BOOST_SPIRIT_DEBUG_NODE(less_equal__);
    BOOST_SPIRIT_DEBUG_NODE(less__);
    BOOST_SPIRIT_DEBUG_NODE(equal__);
    BOOST_SPIRIT_DEBUG_NODE(numeric_constant__);
    BOOST_SPIRIT_DEBUG_NODE(or__);
    BOOST_SPIRIT_DEBUG_NODE(and__);
    BOOST_SPIRIT_DEBUG_NODE(abs__);
    BOOST_SPIRIT_DEBUG_NODE(negate__);
    BOOST_SPIRIT_DEBUG_NODE(not__);
    BOOST_SPIRIT_DEBUG_NODE(numeric_expr__);
    BOOST_SPIRIT_DEBUG_NODE(logical_expr__);
    BOOST_SPIRIT_DEBUG_NODE(shiftleft__);
    BOOST_SPIRIT_DEBUG_NODE(bitand__);
    BOOST_SPIRIT_DEBUG_NODE(bitxor__);
    BOOST_SPIRIT_DEBUG_NODE(bitor__);
    BOOST_SPIRIT_DEBUG_NODE(shiftright__);
    BOOST_SPIRIT_DEBUG_NODE(add__);
    BOOST_SPIRIT_DEBUG_NODE(subtract__);
    BOOST_SPIRIT_DEBUG_NODE(multiply__);
    BOOST_SPIRIT_DEBUG_NODE(divide__);
    BOOST_SPIRIT_DEBUG_NODE(modulo__);

  }
 
  boost::spirit::karma::rule<OutputIterator, script_ast::invert_()> invert__;
  boost::spirit::karma::rule<OutputIterator, script_ast::bitxor_()> bitxor__;
  boost::spirit::karma::rule<OutputIterator, script_ast::bitor_()> bitor__;
  boost::spirit::karma::rule<OutputIterator, script_ast::bitand_()> bitand__;
  boost::spirit::karma::rule<OutputIterator, script_ast::shiftleft_()> shiftleft__;
  boost::spirit::karma::rule<OutputIterator, script_ast::shiftright_()> shiftright__;
  boost::spirit::karma::rule<OutputIterator, script_ast::logical_expr_()> logical_expr__;
  boost::spirit::karma::rule<OutputIterator, script_ast::add_()> add__;
  boost::spirit::karma::rule<OutputIterator, script_ast::subtract_()> subtract__;
  boost::spirit::karma::rule<OutputIterator, script_ast::multiply_()> multiply__;
  boost::spirit::karma::rule<OutputIterator, script_ast::divide_()> divide__;
  boost::spirit::karma::rule<OutputIterator, script_ast::modulo_()> modulo__;
  boost::spirit::karma::rule<OutputIterator, script_ast::numeric_expr_()> numeric_expr__;
  boost::spirit::karma::rule<OutputIterator, script_ast::not_()> not__;
  boost::spirit::karma::rule<OutputIterator, script_ast::negate_()> negate__;
  boost::spirit::karma::rule<OutputIterator, script_ast::abs_()> abs__;
  boost::spirit::karma::rule<OutputIterator, script_ast::and_()> and__;
  boost::spirit::karma::rule<OutputIterator, script_ast::or_()> or__;
  boost::spirit::karma::rule<OutputIterator, script_ast::numeric_constant_()> numeric_constant__;
  boost::spirit::karma::rule<OutputIterator, script_ast::equal_()> equal__;
  boost::spirit::karma::rule<OutputIterator, script_ast::less_()> less__;
  boost::spirit::karma::rule<OutputIterator, script_ast::less_equal_()> less_equal__;
  boost::spirit::karma::rule<OutputIterator, script_ast::greater_()> greater__;
  boost::spirit::karma::rule<OutputIterator, script_ast::greater_equal_()> greater_equal__;
  boost::spirit::karma::rule<OutputIterator, script_ast::_true_()> true__;
  boost::spirit::karma::rule<OutputIterator, script_ast::_false_()> false__;
  boost::spirit::karma::rule<OutputIterator, script_ast::variable_()> variable__;
  boost::spirit::karma::rule<OutputIterator, script_ast::nul_()> nul__;

};

#endif
