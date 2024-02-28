#include "environment.hpp"

#include <cassert>
#include <cmath>

#include "interpreter_semantic_error.hpp"
#include "log.hpp"

EnvResult proc_not = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 1) throw InterpreterSemanticError("incorrect not");
    if (args[0].type != BooleanType) throw InterpreterSemanticError("incorrect arg type");
    return !args[0].value.bool_value;
  }
};

EnvResult proc_and = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    Boolean res = true;
    for (auto a : args) {
      if (a.type != BooleanType) throw InterpreterSemanticError("incorrect arg type");
      res &= a.value.bool_value;
    }
    return res;
  }
};

EnvResult proc_or = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    Boolean res = false;
    for (auto a : args) {
      if (a.type != BooleanType) throw InterpreterSemanticError("incorrect arg type");
      res |= a.value.bool_value;
    }
    return res;
  }
};

EnvResult proc_lt = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    if (args[0].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    if (args[1].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    return args[0].value.num_value < args[1].value.num_value;
  }
};

EnvResult proc_le = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    if (args[0].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    if (args[1].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    return args[0].value.num_value <= args[1].value.num_value;
  }
};

EnvResult proc_gt = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    if (args[0].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    if (args[1].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    return args[0].value.num_value > args[1].value.num_value;
  }
};

EnvResult proc_ge = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    if (args[0].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    if (args[1].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    return args[0].value.num_value >= args[1].value.num_value;
  }
};

EnvResult proc_eq = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    if (args[0].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    if (args[1].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    return args[0].value.num_value == args[1].value.num_value;
  }
};

EnvResult proc_add = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    Number sum = 0.;
    for (auto a : args) {
      if (a.type != NumberType) throw InterpreterSemanticError("incorrect arg type");
      sum += a.value.num_value;
    }
    return sum;
  }
};

EnvResult proc_sub = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    if (args.size() > 2) {
      throw InterpreterSemanticError("incorrect sub, too many args");
    } else if (args.size() == 2) {
      if (args[0].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
      if (args[1].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
      return args[0].value.num_value - args[1].value.num_value;
    } else if (args.size() == 1) {
      if (args[0].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
      return -args[0].value.num_value;
    } else {
      throw InterpreterSemanticError("incorrect sub, too few args");
    }
  }
};

EnvResult proc_mul = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    Number prod = 1.;
    for (auto a : args) {
      if (a.type != NumberType) throw InterpreterSemanticError("incorrect arg type");
      prod *= a.value.num_value;
    }
    return prod;
  }
};

EnvResult proc_div = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect compare");
    if (args[0].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    if (args[1].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    return args[0].value.num_value / args[1].value.num_value;
  }
};

EnvResult proc_log = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 1) throw InterpreterSemanticError("incorrect log");
    if (args[0].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    return log10(args[0].value.num_value);
  }
};

EnvResult proc_pow = {
  ProcedureType,
  Expression(),
  [](const std::vector<Atom>&args) -> Expression {
    if (args.size() != 2) throw InterpreterSemanticError("incorrect pow");
    if (args[0].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    if (args[1].type != NumberType) throw InterpreterSemanticError("incorrect arg type");
    return pow(args[0].value.num_value, args[1].value.num_value);
  }
};

EnvResult const_pi = {
  ExpressionType,
  Expression(atan2(0, -1))
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