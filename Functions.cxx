// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Functions.h"
#include <cmath>
#include <TMath.h>

Functions::Functions(const std::string& p, const double& e)
: particle(p), energy(e) {
  dbm = DatabaseManager::Instance();
}

double Functions::HagedornPower(const double& x) {
  double pol1 = dbm->getDB().mDb[particle].mhp[energy].at(1) * x;
  double pol2 = dbm->getDB().mDb[particle].mhp[energy].at(2) * x*x;
  double e = exp(-pol1-pol2);
  double base = e + x / dbm->getDB().mDb[particle].mhp[energy].at(3);
  double power = pow(base,-dbm->getDB().mDb[particle].mhp[energy].at(4));
  return dbm->getDB().mDb[particle].mhp[energy].at(0) * x * power;
}

double Functions::MtScaling(double* x, double* p) {
  double mh = dbm->getDB().mDb[particle].m["mass"];
  double mt = sqrt(x[0]*x[0] + mh*mh);
  return HagedornPower(mt);
}

double Functions::BreitWigner(double* x, double* p) {
  double gamma = dbm->getDB().mDb[particle].w["width"];
  double m0 = dbm->getDB().mDb[particle].m["mass"];
  double denom = (x[0]-m0)*(x[0]-m0)+gamma*gamma/4.;
  return 0.5/TMath::Pi()*gamma/denom;
}
