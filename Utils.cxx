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
double Utils::mMin = 2*emass;
double Utils::ptMin = 0.;
double Utils::ptMax = 2.;
int Utils::nBins = 1000; // 4 MeV/c2
int Utils::nBinsPt = 80; // 25 MeV/c
double Utils::mMax = 4.;
std::map<std::string, int> Utils::mColorMap = Utils::createColorMap();

Utils::Utils() { }

void Utils::printInfo(const int& n, const int& div) {  // div in k
  if ( n > 0 && !(n%(div*1000)) ) std::cout << n/1000 << "k" << std::endl;
  else if ( !(n%(div*100)) ) std::cout << "." << std::flush;
}

const char* Utils::getOutFileName(const std::string& p, const double& e) {
  std::string fn = "out/BesCocktail/";
  fn += boost::lexical_cast<std::string>(e);
  return (fn+"/"+p+".root").c_str();
}
