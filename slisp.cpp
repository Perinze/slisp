#include <cstdlib>
#include <iostream>
#include <sstream>
#include "tokenize.hpp"
#include "expression.hpp"
#include "interpreter.hpp"

int main(int argc, char **argv)
{
  //TokenSequenceType tokens = tokenize(std::cin);

  //for (auto s : tokens) {
  //  std::cout << "'" << s << "' ";
  //}
  //std::cout << std::endl;

  //// test token to atom
  //Atom a;
  //for (auto s : tokens) {
  //  if (token_to_atom(s, a)) {
  //    std::cout << a << " ";
  //  } else {
  //    std::cout << "invalid ";
  //  }
  //}
  //std::cout << std::endl;

  // test intepreter
  Interpreter interpreter;
  interpreter.parse(std::cin);
  std::cout << interpreter.eval();

  return EXIT_SUCCESS;
}
