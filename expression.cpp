#include "expression.hpp"

#include <cmath>
#include <limits>
#include <cctype>
#include <exception>
#include <regex>

// system includes
#include <sstream>

Expression::Expression(bool tf){
  head.type = BooleanType;
  head.value.bool_value = tf;
}

Expression::Expression(double num){
  head.type = NumberType;
  head.value.num_value = num;
}

Expression::Expression(const std::string & sym){
  head.type = SymbolType;
  head.value.sym_value = sym;
}

bool Expression::operator==(const Expression & exp) const noexcept{
  if (head.type != exp.head.type) return false;
  switch (head.type)
  {
  case NoneType:
    break;

  case BooleanType:
    if (head.value.bool_value != exp.head.value.bool_value)
      return false;
    break;

  case NumberType:
    if (head.value.num_value != exp.head.value.num_value)
      return false;
    break;

  case SymbolType:
    if (head.value.sym_value != exp.head.value.sym_value)
      return false;
    break;

  default:
    return false;
    break;
  }
  if (tail.size() != exp.tail.size()) return false;
  return true;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp){
  out << "(";
  switch (exp.head.type)
  {
  case NoneType:
    out << "";
    break;
  case BooleanType:
    out << (exp.head.value.bool_value ? "True" : "False");
    break;
  case NumberType:
    out << exp.head.value.num_value;
    break;
  case SymbolType:
    out << exp.head.value.sym_value;
    break;
  default:
    break;
  }
  for (auto e : exp.tail) {
    out << e;
  }
  out << ")";
  return out;
}

bool token_to_atom(const std::string & token, Atom & atom){
  // return true if it a token is valid. otherwise, return false.
  auto is_num = [](const std::string & s) -> bool {
    char *end = 0;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
  };
  auto is_sym = [](const std::string & s) -> bool {
    std::regex sym("[^\\d\\s]\\S*");
    if (std::regex_match(s, sym)) return true;
    else return false;
  };

  if (token == "begin" || token == "define" || token == "if") {
    atom.type = KeywordType;
    atom.value.sym_value = token;
  } else if (token == "(" || token == ")") {
    return false;
  } else if (token == "True") {
    atom.type = BooleanType;
    atom.value.bool_value = true;
  } else if (token == "False") {
    atom.type = BooleanType;
    atom.value.bool_value = false;
  } else if (is_num(token)) {
    try {
      atom.type = NumberType;
      atom.value.num_value = std::stod(token);
    } catch (const std::out_of_range&) {
      return false;
    }
  } else if (is_sym(token)) {
    atom.type = SymbolType;
    atom.value.sym_value = token;
  } else {
    return false;
  }
  return true;
}

