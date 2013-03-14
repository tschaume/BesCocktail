// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Utils.h"
#include <iostream>
#include "StRoot/BesCocktail/Constants.h"

void Utils::printInfo(int n) {
  if ( n > 0 && !(n%100000) ) std::cout << n/1000 << "k" << std::endl;
  else if ( !(n%10000) ) std::cout << "." << std::flush;
}

bool Utils::checkParticle(const std::string& p) {
  return Constants::mParCont.count(p);
}
