#include "environment.hpp"

#include <cassert>
#include <cmath>

#include "interpreter_semantic_error.hpp"

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


Environment::Environment(){
  // add default env
  envmap["+"] = proc_add;
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