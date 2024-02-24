#include "expression.hpp"

#include <cmath>
#include <limits>
#include <cctype>

// system includes
#include <sstream>

Expression::Expression(bool tf){
  // TODO: implement this function
}

Expression::Expression(double num){
  // TODO: implement this function
}

Expression::Expression(const std::string & sym){
  // TODO: implement this function
}

bool Expression::operator==(const Expression & exp) const noexcept{
  // TODO: implement this function
  return false;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp){
  // TODO: implement this function
  return out;
}

bool token_to_atom(const std::string & token, Atom & atom){
  // TODO: implement this function
  // return true if it a token is valid. otherwise, return false.
  return true;
}

