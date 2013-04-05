// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Functions.h"
#include "StRoot/BesCocktail/Utils.h"
#include <cmath>
#include <TMath.h>

Functions::Functions(const std::string& p, const double& e)
: particle(p), energy(e) {
  dbm = DatabaseManager::Instance();
  vhp = dbm->getDB().mDb[particle].mhp[energy];
  vres = dbm->getDB().mHdr["header"].mPars["res"];
  vcr = dbm->getDB().mHdr["header"].mPars["crystal"];
  fsfac = dbm->getDB().mDb[particle].fsfac["fsfac"];
  wdth = dbm->getDB().mDb[particle].w["width"];
  l2 = dbm->getDB().mDb[particle].l2["l2"];
  isPhiOm = ( particle == "phi" || particle == "omega" );
  mh = dbm->getMass(particle);
  mhdec = dbm->getMass("eta");
  if ( particle == "omega" ) mhdec = dbm->getMass("omega");
}

double Functions::HagedornPower(const double& x) {
  double e = exp(-vhp[1]*x-vhp[2]*x*x);
  double base = e + x / vhp[3];
  return vhp[0] * x * pow(base,-vhp[4]);
}

double Functions::MtScaling(double* x, double* p) {
  double mt = sqrt(x[0]*x[0] + mh*mh);
  return HagedornPower(mt);
}

double Functions::BreitWigner(double* x, double* p) {
  double denom = (x[0]-mh)*(x[0]-mh)+wdth*wdth/4.;
  return 0.5/TMath::Pi()*wdth/denom;
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

double Functions::QED(const double& x) {
  const double x2 = x*x;
  const double r = Utils::emass2/x2;
  const double A = sqrt(1.-4.*r)*(1.+2.*r);
  return fsfac * Utils::alpha / Utils::threePi * A/x2;
}

double Functions::PhiOmPS(const double& x) {
  double m2d = mh*mh - mhdec*mhdec;
  double tA = 1. + x*x / m2d; tA *= tA;
  double tB = 2*mh*x / m2d; tB *= tB;
  return pow(tA-tB, 1.5);
}

double Functions::PS(const double& x) {
  if ( isPhiOm ) return PhiOmPS(x);
  return pow(1.-x*x/(mh*mh), 3);
}

double Functions::F2(const double& x) {
  // TODO: use 1+(m/l)^2 for pion!?
  double base = 1. - x*x * l2;
  return 1. / (base*base);
}

double Functions::KrollWada(double* x, double* p) {
  return F2(x[0]) * QED(x[0]) * PS(x[0]);
}
