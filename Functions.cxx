// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Functions.h"
#include <cmath>
#include <TMath.h>

Functions::Functions(const std::string& p, const double& e)
: particle(p), energy(e) {
  dbm = DatabaseManager::Instance();
  vhp = dbm->getDB().mDb[particle].mhp[energy];
  vres = dbm->getDB().mHdr["header"].mPars["res"];
  vcr = dbm->getDB().mHdr["header"].mPars["crystal"];
}

double Functions::HagedornPower(const double& x) {
  double e = exp(-vhp[1]*x-vhp[2]*x*x);
  double base = e + x / vhp[3];
  return vhp[0] * x * pow(base,-vhp[4]);
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

double Functions::MomRes(double* x, double* p) {
  double a = vres[0];
  return sqrt(a*a*x[0]*x[0]+vres[1]*vres[1]);
}

double Functions::getCrystalA(const double& n, const double& a) {
  return pow(n/fabs(a), n) * exp(-a*a/2.);
}

double Functions::getCrystalB(const double& n, const double& a) {
  return n/fabs(a)-fabs(a);
}

double Functions::CrystalBall2(double* x, double* p) {  // both power law tails
  double arg = (x[0]-vcr[0])/vcr[1];
  double A = getCrystalA(vcr[2], vcr[3]);  // lower tail
  double B = getCrystalB(vcr[2], vcr[3]);
  double C = getCrystalA(vcr[4], vcr[5]);  // upper tail
  double D = getCrystalB(vcr[4], vcr[5]);
  if ( arg < -vcr[3] ) return A*pow(B-arg, -vcr[2]);
  if ( arg >  vcr[5] ) return C*pow(D+arg, -vcr[4]);
  return exp(-0.5*arg*arg);  // gaussian part
}
