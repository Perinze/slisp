#include <cstdlib>
#include <iostream>
#include <sstream>
#include "tokenize.hpp"
#include "expression.hpp"

int main(int argc, char **argv)
{
  TokenSequenceType tokens;
  std::string line;

  while (std::getline(std::cin, line)) {
    std::istringstream is(line);
    auto ts = tokenize(is);
    for (auto t : ts) tokens.push_back(t);
  }

  for (auto s : tokens) {
    std::cout << "'" << s << "' ";
  }
  std::cout << std::endl;

  // test token to atom
  Atom a;
  for (auto s : tokens) {
    if (token_to_atom(s, a)) {
      std::cout << a << " ";
    } else {
      std::cout << "invalid ";
    }
  }
  std::cout << std::endl;
  return EXIT_SUCCESS;
}
