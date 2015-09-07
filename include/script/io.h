#ifndef TROLLCOIN_SCRIPT_IO_H_
#define TROLLCOIN_SCRIPT_IO_H_

#include "script/ast.h"

#include <iosfwd>

namespace script_ast {

  std::ostream& operator<<(std::ostream&, const script_ast::nul_&);
  std::ostream& operator<<(std::ostream&, const script_ast::_true_&);
  std::ostream& operator<<(std::ostream&, const script_ast::_false_&);

}

#endif
