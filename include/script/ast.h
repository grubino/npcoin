#ifndef SCRIPT_AST_H_
#define SCRIPT_AST_H_

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/fusion/include/io.hpp>

#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <boost/mpl/map.hpp>
#include <boost/mpl/string.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/or.hpp>

#include <string>

namespace script_ast {

  struct nul_ {};
  struct _true_ {};
  struct _false_ {};

  std::ostream& operator<<(std::ostream&, const script_ast::nul_&);
  std::ostream& operator<<(std::ostream&, const script_ast::_true_&);
  std::ostream& operator<<(std::ostream&, const script_ast::_false_&);

  struct numeric_constant_ {
    std::string value;
  };

  struct variable_ {
    std::string identifier;
  };

  struct string_constant_{
    std::string value;
  };

  struct abs_;
  struct negate_;
  struct not_;
  struct invert_;

  struct add_;
  struct subtract_;
  struct multiply_;
  struct divide_;
  struct modulo_;

  struct or_;
  struct and_;

  struct assign_;
  struct equal_;
  struct greater_;
  struct greater_equal_;
  struct less_;
  struct less_equal_;

  struct bitxor_;
  struct bitor_;
  struct bitand_;
  struct shiftleft_;
  struct shiftright_;

  namespace mpl = boost::mpl;
  
  typedef mpl::map< 
    mpl::pair<script_ast::add_, mpl::c_str<mpl::string<'+'> > > 
    , mpl::pair<script_ast::subtract_, mpl::c_str<mpl::string<'-'> > >
    , mpl::pair<script_ast::multiply_, mpl::c_str<mpl::string<'*'> > >
    , mpl::pair<script_ast::divide_, mpl::c_str<mpl::string<'/'> > >
    , mpl::pair<script_ast::modulo_, mpl::c_str<mpl::string<'%'> > >
    , mpl::pair<script_ast::or_, mpl::c_str<mpl::string<' or '> > >
    , mpl::pair<script_ast::and_, mpl::c_str<mpl::string<' and '> > >
    , mpl::pair<script_ast::equal_, mpl::c_str<mpl::string<'=='> > >
    , mpl::pair<script_ast::greater_, mpl::c_str<mpl::string<'>'> > >
    , mpl::pair<script_ast::greater_equal_, mpl::c_str<mpl::string<'>='> > >
    , mpl::pair<script_ast::less_, mpl::c_str<mpl::string<'<'> > >
    , mpl::pair<script_ast::less_equal_, mpl::c_str<mpl::string<'<='> > >
    > binary_op_symbol_map;

  typedef mpl::map< 
    mpl::pair<script_ast::abs_, mpl::c_str<mpl::string<'+'> > > 
    , mpl::pair<script_ast::negate_, mpl::c_str<mpl::string<'-'> > >
    , mpl::pair<script_ast::not_, mpl::c_str<mpl::string<'!'> > >
    > unary_op_symbol_map;

  typedef boost::variant<
    nul_, numeric_constant_, variable_
    , boost::recursive_wrapper<invert_>
    , boost::recursive_wrapper<bitand_>
    , boost::recursive_wrapper<bitor_>
    , boost::recursive_wrapper<bitxor_>
    , boost::recursive_wrapper<shiftleft_>
    , boost::recursive_wrapper<shiftright_>
    , boost::recursive_wrapper<negate_>
    , boost::recursive_wrapper<abs_>
    , boost::recursive_wrapper<add_>
    , boost::recursive_wrapper<subtract_>
    , boost::recursive_wrapper<multiply_>
    , boost::recursive_wrapper<divide_>
    , boost::recursive_wrapper<modulo_> > numeric_expr_;

  typedef boost::variant<
    nul_, _true_, _false_
    , boost::recursive_wrapper<equal_>
    , boost::recursive_wrapper<greater_>
    , boost::recursive_wrapper<greater_equal_>
    , boost::recursive_wrapper<less_>
    , boost::recursive_wrapper<less_equal_>
    , boost::recursive_wrapper<not_>
    , boost::recursive_wrapper<or_>
    , boost::recursive_wrapper<and_> > logical_expr_;

  struct abs_ {
    numeric_expr_ expr;
  };
  struct negate_ {
    numeric_expr_ expr;
  };
  struct invert_ {
    numeric_expr_ expr;
  };
  struct not_ {
    logical_expr_ expr;
  };

  struct add_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };
  struct subtract_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };
  struct multiply_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };
  struct divide_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };
  struct modulo_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };

  struct or_ {
    logical_expr_ lhs;
    logical_expr_ rhs;
  };
  struct and_ {
    logical_expr_ lhs;
    logical_expr_ rhs;
  };

  struct equal_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };
  struct greater_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };
  struct greater_equal_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };
  struct less_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };
  struct less_equal_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };

  struct bitxor_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };

  struct bitor_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };
  struct bitand_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };
  struct shiftleft_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };
  struct shiftright_ {
    numeric_expr_ lhs;
    numeric_expr_ rhs;
  };

}

BOOST_FUSION_ADAPT_STRUCT(script_ast::numeric_constant_, (std::string, value))

BOOST_FUSION_ADAPT_STRUCT(script_ast::string_constant_, (std::string, value))

BOOST_FUSION_ADAPT_STRUCT(script_ast::variable_, (std::string, identifier))

BOOST_FUSION_ADAPT_STRUCT(script_ast::abs_, (script_ast::numeric_expr_, expr))

BOOST_FUSION_ADAPT_STRUCT(script_ast::negate_, (script_ast::numeric_expr_, expr))

BOOST_FUSION_ADAPT_STRUCT(script_ast::not_, (script_ast::logical_expr_, expr))

BOOST_FUSION_ADAPT_STRUCT(script_ast::invert_, (script_ast::numeric_expr_, expr))

BOOST_FUSION_ADAPT_STRUCT(script_ast::add_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::subtract_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::multiply_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::divide_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::modulo_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::and_, (script_ast::logical_expr_, lhs)(script_ast::logical_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::or_, (script_ast::logical_expr_, lhs)(script_ast::logical_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::equal_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::greater_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::greater_equal_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::less_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::less_equal_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::bitxor_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::bitor_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::bitand_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::shiftleft_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

BOOST_FUSION_ADAPT_STRUCT(script_ast::shiftright_, (script_ast::numeric_expr_, lhs)(script_ast::numeric_expr_, rhs))

#endif
