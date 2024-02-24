#include "expression.hpp"

#include <cmath>
#include <limits>
#include <cctype>
#include <exception>

// system includes
#include <sstream>

Expression::Expression(bool tf){
  head.type = BooleanType;
  head.value = Value{.bool_value=tf};
}

Expression::Expression(double num){
  head.type = NumberType;
  head.value = Value{.num_value=num};
}

Expression::Expression(const std::string & sym){
  head.type = SymbolType;
  head.value = Value{.sym_value=sym};
}

bool Expression::operator==(const Expression & exp) const noexcept{
  if (head.type != exp.head.type) return false;
  switch (head.type)
  {
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
    break;
  }
  if (tail.size() != exp.tail.size()) return false;
  return true;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp){
  out << "(";
  switch (exp.head.type)
  {
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
  if (token == "(" || token == ")") {
    return false;
  } else if (token == "True") {
    atom.type = BooleanType;
    atom.value.bool_value = true;
  } else if (token == "False") {
    atom.type = BooleanType;
    atom.value.bool_value = false;
  } else { // Number or Symbol
    try {
      atom.type = NumberType;
      atom.value.num_value = std::stod(token);
    } catch (const std::invalid_argument&) {
      atom.type = SymbolType;
      atom.value.sym_value = token;
    } catch (const std::out_of_range&) {
      return false;
    }
  }
  return true;
}

