#ifndef VISITOR_H_
#define VISITOR_H_

#include "script/ast.h"
#include "core/big_integer.h"

#include <boost/variant/apply_visitor.hpp>
#include <boost/variant.hpp>

#include <map>
#include <string>
#include <stdexcept>

typedef std::map<std::string, script_ast::numeric_constant_> substitution_map;

namespace script_ast {

  class arithmetic_visitor : public boost::static_visitor<big_integer> {
  public:
    arithmetic_visitor(const substitution_map& subs)
      : m_substitution_map(subs) {}

    big_integer operator() (const numeric_constant_& n) const {
      return big_integer(n.value);
    }
    big_integer operator() (const variable_& v) const {
      return big_integer(m_substitution_map.at(v.identifier).value);
    }
    big_integer operator() (const nul_& n) const {
      throw std::runtime_error("null values are not allowed in expressions");
    }
    big_integer operator() (const negate_& op) const {
      return -boost::apply_visitor(*this, op.expr);
    }
    big_integer operator() (const abs_& op) const {
      big_integer result = boost::apply_visitor(*this, op.expr);
      return result < 0 ? -result : result;
    }
    big_integer operator() (const add_& op) const {
      return boost::apply_visitor(*this, op.lhs) +
	boost::apply_visitor(*this, op.rhs);
    }
    big_integer operator() (const subtract_& op) const {
      return boost::apply_visitor(*this, op.lhs) -
	boost::apply_visitor(*this, op.rhs);
    }
    big_integer operator() (const multiply_& op) const {
      return boost::apply_visitor(*this, op.lhs) * 
	boost::apply_visitor(*this, op.rhs);
    }
    big_integer operator() (const divide_& op) const {
      return boost::apply_visitor(*this, op.lhs) / 
	boost::apply_visitor(*this, op.rhs);
    }
    big_integer operator() (const modulo_& op) const {
      return boost::apply_visitor(*this, op.lhs) %
	boost::apply_visitor(*this, op.rhs);
    }
    big_integer operator() (const shiftright_& op) const {
      return boost::apply_visitor(*this, op.lhs) >>
	static_cast<unsigned>(boost::apply_visitor(*this, op.rhs));
    }
    big_integer operator() (const shiftleft_& op) const {
      return boost::apply_visitor(*this, op.lhs) <<
	static_cast<unsigned>(boost::apply_visitor(*this, op.rhs));
    }
    big_integer operator() (const bitand_& op) const {
      return boost::apply_visitor(*this, op.lhs) &
	boost::apply_visitor(*this, op.rhs);
    }
    big_integer operator() (const bitor_& op) const {
      return boost::apply_visitor(*this, op.lhs) |
	boost::apply_visitor(*this, op.rhs);
    }
    big_integer operator() (const bitxor_& op) const {
      return boost::apply_visitor(*this, op.lhs) ^
	boost::apply_visitor(*this, op.rhs);
    }
    big_integer operator() (const invert_& op) const {
      return boost::apply_visitor(*this, op.expr);
    }

  private:
    const substitution_map& m_substitution_map;
    
  };

  class comparison_visitor : public boost::static_visitor<bool> {
  public:
    comparison_visitor(const arithmetic_visitor& a) 
      : m_visitor(a) {}

    bool operator() (const script_ast::nul_& e) const {
      throw std::runtime_error("null values are not allowed in expressions");
    }

    bool operator() (const script_ast::_true_& t) const {
      return true;
    }

    bool operator() (const script_ast::_false_& f) const {
      return false;
    }

    bool operator() (const script_ast::equal_& expr) const {
      return boost::apply_visitor(m_visitor, expr.lhs) == 
	boost::apply_visitor(m_visitor, expr.rhs);
    }
    bool operator() (const script_ast::greater_& expr) const {
      return boost::apply_visitor(m_visitor, expr.lhs) >
	boost::apply_visitor(m_visitor, expr.rhs);
    }
    bool operator() (const script_ast::greater_equal_& expr) const {
      return boost::apply_visitor(m_visitor, expr.lhs) >=
	boost::apply_visitor(m_visitor, expr.rhs);
    }
    bool operator() (const script_ast::less_& expr) const {
      return boost::apply_visitor(m_visitor, expr.lhs) <
	boost::apply_visitor(m_visitor, expr.rhs);
    }
    bool operator() (const script_ast::less_equal_& expr) const {
      return boost::apply_visitor(m_visitor, expr.lhs) <=
	boost::apply_visitor(m_visitor, expr.rhs);
    }
    bool operator() (const script_ast::not_& expr) const {
      return !boost::apply_visitor(*this, expr.expr);
    }
    bool operator() (const script_ast::or_& expr) const {
      return boost::apply_visitor(*this, expr.lhs) ||
	boost::apply_visitor(*this, expr.rhs);
    }
    bool operator() (const script_ast::and_& expr) const {
      return boost::apply_visitor(*this, expr.lhs) &&
	boost::apply_visitor(*this, expr.rhs);
    }
  private:
    const arithmetic_visitor& m_visitor;
  };

}

#endif
