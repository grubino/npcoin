#include "script/io.h"
#include "script/ast.h"

#include <iosfwd>

namespace script_ast {

  std::ostream& operator<<(std::ostream& os, const script_ast::nul_& val) {
    return os;
  }
  std::ostream& operator<<(std::ostream& os, const script_ast::_true_& val) {
    os << "true";
    return os;
  }
  std::ostream& operator<<(std::ostream& os, const script_ast::_false_& val) {
    os << "false";
    return os;
  }

}
