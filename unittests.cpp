#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"

#include "expression.hpp"
#include "interpreter.hpp"
#include "interpreter_semantic_error.hpp"

#include <sstream>

TEST_CASE ( "Test boolean expression constructor", "[types]" ) {

  {
    Expression exp1(true);

    REQUIRE(exp1.head.type == BooleanType);
    REQUIRE(exp1.head.value.bool_value == true);
    REQUIRE(exp1.tail.empty());
  }

  {
    Expression exp2(false);

    REQUIRE(exp2.head.type == BooleanType);
    REQUIRE(exp2.head.value.bool_value == false);
    REQUIRE(exp2.tail.empty());
  }
}

TEST_CASE ( "Test number expression constructor", "[types]" ) {

  Expression exp(42.12);

  REQUIRE(exp.head.type == NumberType);
  REQUIRE(exp.head.value.num_value == 42.12);
  REQUIRE(exp.tail.empty());
}

TEST_CASE ( "Test symbol expression constructor", "[types]" ) {

  std::string sym("ab2");
  Expression exp(sym);

  REQUIRE(exp.head.type == SymbolType);
  REQUIRE(exp.head.value.sym_value == sym);
  REQUIRE(exp.tail.empty());
}

TEST_CASE ( "Test keyword inference", "[types]" ) {

  std::vector<std::string> keywords{"begin", "define", "if"};
  Atom atom;

  for (auto s : keywords) {
    REQUIRE(token_to_atom(s, atom) == true);
    REQUIRE(atom.type == KeywordType);
    REQUIRE(atom.value.sym_value == s);
  }
}

TEST_CASE ( "Test parenthesis inference", "[types]" ) {

  std::vector<std::string> tokens{"(", ")"};
  Atom atom;

  for (auto s : tokens) {
    REQUIRE(token_to_atom(s, atom) == false);
  }
}

TEST_CASE ( "Test default environment", "[environment]" ) {

  {
    std::vector<std::string> symbols = {
      "hello", "jdskfl", "^", "$", "_"
    };

    auto env = Environment();
    EnvResult env_res;
    for (auto s : symbols) {
      REQUIRE(env.lookup(s, env_res) == false);
    }
  }

  {
    std::vector<std::string> symbols = {
      "not", "and", "or", "<", "<=", ">", ">=", "=",
      "+", "-", "*", "/", "log10", "pow", "pi"
    };

    auto env = Environment();
    EnvResult env_res;
    for (auto s : symbols) {
      REQUIRE(env.lookup(s, env_res) == true);
    }
  }
}

TEST_CASE ( "Test lookup after define", "[environment]" ) {

  std::vector<std::string> symbols = {
    "hello", "jdskfl", "^", "$", "_"
  };

  {
    auto env = Environment();
    EnvResult env_res;
    for (auto s : symbols) {
      REQUIRE(env.define(s, Expression()) == true);
      REQUIRE(env.lookup(s, env_res) == true);
      REQUIRE(env_res.type == ExpressionType);
      REQUIRE(env_res.exp == Expression());
    }
  }

  {
    auto env = Environment();
    EnvResult env_res;
    for (auto s : symbols) {
      REQUIRE(env.define(s, Expression()) == true);
    }
    for (auto s : symbols) {
      REQUIRE(env.lookup(s, env_res) == true);
      REQUIRE(env_res.type == ExpressionType);
      REQUIRE(env_res.exp == Expression());
    }
  }

  {
    auto env = Environment();
    EnvResult env_res;
    for (auto s : symbols) {
      REQUIRE(env.define(s, Expression()) == true);
    }
    for (auto s : symbols) {
      REQUIRE(env.lookup("unbound", env_res) == false);
    }
  }
}

TEST_CASE ( "Test define different type of expression and lookup", "[environment]" ) {

  std::vector<std::pair<std::string, Expression>> exp{
    std::make_pair("unit", Expression()),
    std::make_pair("bool", Expression(true)),
    std::make_pair("num", Expression(42.12)),
    std::make_pair("sym", Expression("$t2"))
  };

  for (auto p : exp) {
    auto env = Environment();
    EnvResult env_res;
    REQUIRE(env.define(p.first, p.second) == true);
    REQUIRE(env.lookup(p.first, env_res) == true);
    REQUIRE(env_res.type == ExpressionType);
    REQUIRE(env_res.exp == p.second);
  }
}

TEST_CASE ( "Test apply invalid number of args to fix-ary operator", "[interpreter]") {

  struct op_record {
    std::string name;
    std::string arg;
    int lb, ub;
  };
  int max_arg = 8;

  std::vector<op_record> op = {
    op_record{"not", " False", 1, 1},
    op_record{"and", " True", 0, max_arg},
    op_record{"or", " True", 0, max_arg},
    op_record{"<", " 1", 2, 2},
    op_record{"<=", " 42", 2, 2},
    op_record{">", " 12", 2, 2},
    op_record{">=", " 16", 2, 2},
    op_record{"=", " 129", 2, 2},
    op_record{"+", " 19", 0, max_arg},
    op_record{"-", " 19", 1, 2},
    op_record{"*", " 19", 0, max_arg},
    op_record{"/", " 19", 2, 2},
    op_record{"log10", " 42", 1, 1},
    op_record{"pow", " 42", 2, 2}
  };
  for (auto r : op) {
    for (int i = 0; i < 8; i++) {
      std::ostringstream ss;
      for (int j = 0; j < i; j++) ss << r.arg;
      std::string program = std::string("(") + r.name + ss.str() + std::string(")");
      std::cout << program << std::endl;

      Interpreter interp;

      std::istringstream iss(program);
    
      bool ok = interp.parse(iss);

      if (i >= r.lb && i <= r.ub) {
        REQUIRE_NOTHROW(interp.eval());
      } else {
        REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
      }
    }
  }
}