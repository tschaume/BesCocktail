// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Utils.h"
#include <iostream>
#include <TMath.h>

Utils::Utils(const string& particle) : phiMax(2.*TMath::Pi()) {
  yGen = new TRandom3();
  yGen->SetSeed(0);
  phiGen = new TRandom3();
  phiGen->SetSeed(0);
  dbm = DatabaseManager::Instance();
  mass2 = dbm->getMass2(particle);
}

void Utils::printInfo(const int& n) {
  if ( n > 0 && !(n%100000) ) std::cout << n/1000 << "k" << std::endl;
  else if ( !(n%10000) ) std::cout << "." << std::flush;
}

const char* Utils::getOutFileName(const std::string& p) {
  return (p+".root").c_str();
}

double Utils::getPhi() { return phiGen->Uniform(0.,phiMax); }

double Utils::getY() { return yGen->Uniform(-1.,1.); }

double Utils::getEta(const double& pT) {
  double mT = sqrt(mass2+pT*pT);
  return asinh(mT/pT*sinh(getY()));
}

