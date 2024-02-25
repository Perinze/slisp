#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fstream>
#include "tokenize.hpp"
#include "expression.hpp"
#include "interpreter.hpp"
#include "interpreter_semantic_error.hpp"

int main(int argc, char **argv)
{
  auto parse_and_eval = [](Interpreter &interpreter, std::istream &is) {
    try {
      interpreter.parse(is);
      std::cout << interpreter.eval() << std::endl;
    } catch (const InterpreterSemanticError &e) {
      std::cout << "Error: " << e.what() << std::endl;
    }
  };

  Interpreter interpreter;
  if (argc == 1) { // repl
    std::string line;
    while (std::getline(std::cin, line)) {
      std::istringstream is(line);
      parse_and_eval(interpreter, is);
    }
  } else if (!strcmp(argv[1], "-e")) {
    if (argc != 3) return EXIT_FAILURE;
    std::string s(&argv[2][1]);
    s.pop_back();
    std::istringstream is(s);
    parse_and_eval(interpreter, is);
  } else {
    if (argc != 2) return EXIT_FAILURE;
    std::ifstream ifs;
    ifs.open(argv[1]);
    parse_and_eval(interpreter, ifs);
    ifs.close();
  }

  return EXIT_SUCCESS;
}
