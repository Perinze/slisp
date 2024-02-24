#include "interpreter.hpp"

// system includes
#include <stack>
#include <stdexcept>
#include <iostream>

// module includes
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"

bool Interpreter::parse(std::istream & expression) noexcept{
  // return true if input is valid. otherwise, return false.
  TokenSequenceType tokens = tokenize(expression);

  // in case of empty program
  if (tokens.empty()) {
    ast = Expression();
    //std::cout << "no tokens\n";
  } else {
    auto it = tokens.begin();
    ast = parse_top_down(it);
    std::cout << "ast: " << ast << std::endl;
  }
  // warning: not handling invalid input
  return true;
};

Expression Interpreter::eval(){
  return eval_top_down(ast);
}

Expression Interpreter::parse_top_down(TokenSequenceType::iterator & it) {
  Expression exp;
  if (*it != "(") { // atom without parenthesis
    if (!token_to_atom(*it, exp.head)) exit(EXIT_FAILURE);
  } else { // (...), *it == "("
    ++it;
    if (*it == ")") { // special case: none
      exp.head.type = NoneType;
    } else {
      // assert first is atom due to its syntax
      if (!token_to_atom(*it, exp.head)) exit(EXIT_FAILURE);

      ++it;
      while (*it != ")") {
        exp.tail.push_back(parse_top_down(it));
      }
    }
  }
  it++;
  return exp;
}

Expression Interpreter::eval_top_down(const Expression & exp) {
  EnvResult envres;
  if (exp.head.type == SymbolType) {
    if (exp.head.value.sym_value == "begin") {
      Expression r; // result
      for (auto a : exp.tail) {
        r = eval_top_down(a);
      }
      return r;
    } else if (exp.head.value.sym_value == "define") {
      if (exp.tail.size() != 2) throw "incorrect define";
      if (exp.tail[0].head.type != SymbolType) throw "incorrect define symbol";
      env.define(exp.tail[0].head.value.sym_value, eval_top_down(exp.tail[1]));
      return Expression();
    } else if (exp.head.value.sym_value == "if") {
      if (exp.tail.size() != 3) throw "incorrect if";
      auto cond = eval_top_down(exp.tail[0]);
      if (cond.head.type != BooleanType) throw "incorrect cond type";
      if (cond.head.value.bool_value) {
        return eval_top_down(exp.tail[1]);
      } else {
        return eval_top_down(exp.tail[2]);
      }
    } else if (env.lookup(exp.head.value.sym_value, envres)) {
      if (envres.type == ProcedureType) {
        // 1. eval all args, retrieve their head as atom
        std::vector<Atom> args;
        for (auto a : exp.tail) {
          args.push_back(eval_top_down(a).head);
        }
        // 2. apply
        return envres.proc(args);
      } else {
        return envres.exp;
      }
    }
  }
  // otherwise value
  return exp;
}