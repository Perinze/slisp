#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

// system includes
#include <map>

// module includes
#include "expression.hpp"

enum EnvResultType {ExpressionType, ProcedureType};
struct EnvResult{
  EnvResultType type;
  Expression exp;
  Procedure proc;
};

class Environment{
public:
  Environment();
  bool lookup(Symbol, EnvResult&);
  bool define(Symbol, Expression);
private:

  // Environment is a mapping from symbols to expressions or procedures
  std::map<Symbol,EnvResult> envmap;
};

#endif
