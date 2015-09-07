#include "script/grammar.h"
#include "script/ast.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;

using script_ast::_true_;
using script_ast::_false_;

script_grammar::script_grammar() 
  : script_grammar::base_type(logical_expr__) {

  using boost::spirit::lit;
  using boost::spirit::eps;
  using boost::spirit::lexeme;
  using namespace boost::spirit::qi::labels;
  using namespace script_ast;

  not__ %= lit("!(") >> logical_expr__ >> lit(")");
  and__ %= lit("(") >> logical_expr__ >> lit("and") >> logical_expr__ >> lit(")");
  or__ %= lit("(") >> logical_expr__ >> lit("or") >> logical_expr__ >> lit(")");
  true__ = lit("true")[qi::_val = phoenix::construct<_true_>()];
  false__ = lit("false")[qi::_val = phoenix::construct<_false_>()];
  logical_expr__ %= 
    nul__ | true__ | false__
    | equal__ 
    | greater__ 
    | greater_equal__ 
    | less__ 
    | less_equal__ 
    | and__ | or__ | not__;
  equal__ %= numeric_expr__ >> lit("==") >> numeric_expr__;
  less__ %= numeric_expr__ >> lit("<") >> numeric_expr__;
  less_equal__ %= numeric_expr__ >> lit("<=") >> numeric_expr__;
  greater__ %= numeric_expr__ >> lit(">") >> numeric_expr__;
  greater_equal__ %= numeric_expr__ >> lit(">=") >> numeric_expr__;

  invert__ %= lit("~(") >> numeric_expr__ >> lit(")");
  bitxor__ %= lit("(") >> numeric_expr__ >> lit("^") >> numeric_expr__ >> lit(")");
  bitor__ %= lit("(") >> numeric_expr__ >> lit("|") >> numeric_expr__ >> lit(")");
  bitand__ %= lit("(") >> numeric_expr__ >> lit("&") >> numeric_expr__ >> lit(")");
  shiftleft__ %= lit("(") >> numeric_expr__ >> lit("<<") >> numeric_expr__ >> lit(")");
  shiftright__ %= lit("(") >> numeric_expr__ >> lit(">>") >> numeric_expr__ >> lit(")");

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
  negate__ %= lit("-(") >> numeric_expr__ >> lit(")");
  abs__ %= lit("+(") >> numeric_expr__ >> lit(")");
  add__ %= lit("(") >> numeric_expr__ >> lit("+") >> numeric_expr__ >> lit(")");
  subtract__ %= lit("(") >> numeric_expr__ >> lit("-") >> numeric_expr__ >> lit(")");
  multiply__ %= lit("(") >> numeric_expr__ >> lit("*") >> numeric_expr__ >> lit(")");
  divide__ %= lit("(") >> numeric_expr__ >> lit("/") >> numeric_expr__ >> lit(")");
  modulo__ %= lit("(") >> numeric_expr__ >> lit("%") >>numeric_expr__ >> lit(")");
  numeric_constant__ %= lit("#") >> lexeme[+ascii::char_("0-9a-f")];
  variable__ = 
    lit("$") >> lexeme[+ascii::char_("A-Za-z")[phoenix::at_c<0>(_val) = _1]];    

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

script_ast::logical_expr_ parse_logical_expr(std::string& s) {

  script_ast::logical_expr_ ast;
  script_grammar grammar;

  bool parse_success = qi::phrase_parse(s.begin()
				       , s.end()
				       , grammar
				       , ascii::space
				       , ast);

  if(not parse_success) {
    throw std::runtime_error("parsing failed");
  }
  return ast;

}

