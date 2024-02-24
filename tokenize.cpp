#include "tokenize.hpp"
#include <cctype>

#include <iostream>
#include <string>

TokenSequenceType tokenize(std::istream & seq){
  TokenSequenceType tokens;

  char c;
  std::string tmp;
  bool on_atom = false;

  auto commit_atom_if_leaving = [&]() {
    if (on_atom) {
      tokens.push_back(tmp);
      on_atom = false;
    }
  };

  while (seq.get(c)) {
    switch (c)
    {
    case OPEN:
      commit_atom_if_leaving();
      tokens.push_back("(");
      break;

    case CLOSE:
      commit_atom_if_leaving();
      tokens.push_back(")");
      break;

    case COMMENT:
      commit_atom_if_leaving();
      while (seq.get(c) && c != '\n');
      break;

    case ' ':
    case '\t':
    case '\r':
    case '\n':
      commit_atom_if_leaving();
      break;

    default:
      if (on_atom) { // continue reading to tmp
        tmp.push_back(c);
      } else { // start reading to tmp
        tmp = c;
        on_atom = true;
      }
    }
  }
  commit_atom_if_leaving();

  return tokens;
}
