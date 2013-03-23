// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Utils.h"
#include <iostream>

Utils::Utils() { }

void Utils::printInfo(const int& n) {
  if ( n > 0 && !(n%100000) ) std::cout << n/1000 << "k" << std::endl;
  else if ( !(n%10000) ) std::cout << "." << std::flush;
}

const char* Utils::getOutFileName(const std::string& p) {
  return (p+".root").c_str();
}
