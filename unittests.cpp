#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"

#include "expression.hpp"
#include "interpreter.hpp"
#include "interpreter_semantic_error.hpp"

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

TEST_CASE ( "Test apply 0 args to m-ary operator", "[interpreter]") {

  std::vector<std::string> programs = {
    "(+)",
		"(*)",
		"(and)",
		"(or)"};
  for(auto s : programs){
    Interpreter interp;

    std::istringstream iss(s);
    
    bool ok = interp.parse(iss);

    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
  }
}