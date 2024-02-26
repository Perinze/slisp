#include "environment.hpp"

#include <cassert>
#include <cmath>

#include "interpreter_semantic_error.hpp"
#include "log.hpp"

EnvResult proc_not = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 1) throw InterpreterSemanticError("incorrect not");
    return !args[0].value.bool_value;
  }
};

EnvResult proc_and = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    Boolean res = true;
    for (auto a : args) {
      // TODO: check for non-number
      res &= a.value.bool_value;
    }
    return res;
  }
};

EnvResult proc_or = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    Boolean res = false;
    for (auto a : args) {
      // TODO: check for non-number
      res |= a.value.bool_value;
    }
    return res;
  }
};

EnvResult proc_lt = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    return args[0].value.num_value < args[1].value.num_value;
  }
};

EnvResult proc_le = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    return args[0].value.num_value <= args[1].value.num_value;
  }
};

EnvResult proc_gt = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    return args[0].value.num_value > args[1].value.num_value;
  }
};

EnvResult proc_ge = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    return args[0].value.num_value >= args[1].value.num_value;
  }
};

EnvResult proc_eq = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    return args[0].value.num_value == args[1].value.num_value;
  }
};

EnvResult proc_add = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    Number sum = 0.;
    for (auto a : args) {
      // TODO: check for non-number
      sum += a.value.num_value;
    }
    return sum;
  }
};

EnvResult proc_sub = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    if (args.size() > 2) {
      throw InterpreterSemanticError("incorrect sub, too many args");
    } else if (args.size() == 2) {
      return args[0].value.num_value - args[1].value.num_value;
    } else if (args.size() == 1) {
      return -args[0].value.num_value;
    } else {
      throw InterpreterSemanticError("incorrect sub, too few args");
    }
  }
};

EnvResult proc_mul = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    Number prod = 1.;
    for (auto a : args) {
      // TODO: check for non-number
      prod *= a.value.num_value;
    }
    return prod;
  }
};

EnvResult proc_div = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    return args[0].value.num_value / args[1].value.num_value;
  }
};

EnvResult proc_log = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 1) throw InterpreterSemanticError("incorrect log");
    return log10(args[0].value.num_value);
  }
};

EnvResult proc_pow = {
  .type=ProcedureType,
  .proc=[](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect pow");
    return pow(args[0].value.num_value, args[1].value.num_value);
  }
};

EnvResult const_pi = {
  .type=ExpressionType,
  .exp=Expression(atan2(0, -1))
};

Environment::Environment(){
  // add default env
  envmap["not"] = proc_not;
  envmap["and"] = proc_and;
  envmap["or"] = proc_or;
  envmap["<"] = proc_lt;
  envmap["<="] = proc_le;
  envmap[">"] = proc_gt;
  envmap[">="] = proc_ge;
  envmap["="] = proc_eq;
  envmap["+"] = proc_add;
  envmap["-"] = proc_sub;
  envmap["*"] = proc_mul;
  envmap["/"] = proc_div;
  envmap["log10"] = proc_log;
  envmap["pow"] = proc_pow;
  envmap["pi"] = const_pi;
}

bool Environment::lookup(Symbol sym, EnvResult &res) {
  auto it = envmap.find(sym);
  if (it == envmap.end()) {
    return false;
  }
  res = it->second;
  return true;
}

bool Environment::define(Symbol sym, Expression exp) {
  if (envmap.find(sym) != envmap.end()) {
    return false;
  }
  envmap[sym] = {.type=ExpressionType, .exp=exp};
  return true;
}