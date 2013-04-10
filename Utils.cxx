// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Utils.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <TMath.h>

double Utils::emass = 0.510998928e-3;
double Utils::emass2 = emass*emass;
double Utils::twoPi = 2.*TMath::Pi();
double Utils::threePi = 3.*TMath::Pi();
double Utils::alpha = 1./137.035999174;

Utils::Utils() { }

void Utils::printInfo(const int& n) {
  if ( n > 0 && !(n%100000) ) std::cout << n/1000 << "k" << std::endl;
  else if ( !(n%10000) ) std::cout << "." << std::flush;
}

const char* Utils::getOutFileName(const std::string& p, const double& e) {
  std::string fn = "out/";
  fn += boost::lexical_cast<std::string>(e);
  return (fn+"/"+p+".root").c_str();
}
