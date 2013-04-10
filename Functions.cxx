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
  fsfac = dbm->getProperty(particle, "fsfac");
  l2 = dbm->getProperty(particle, "l2");
  wdth = dbm->getProperty(particle, "width");
  isPhiOm = ( particle == "phi" || particle == "omega" );
  mh = dbm->getProperty(particle, "mass");
  mhdec = dbm->getDecayMass(particle);
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

double Functions::QED(const double& q) {
  const double x2 = q*q;
  const double r = Utils::emass2/x2;
  const double A = sqrt(1.-4.*r)*(1.+2.*r);
  return fsfac * Utils::alpha / Utils::threePi * A/x2;
}

double Functions::PhiOmPS(const double& q) {
  double m2d = mh*mh - mhdec*mhdec;
  double tA = 1. + q*q / m2d; tA *= tA;
  double tB = 2*mh*q / m2d; tB *= tB;
  return sqrt(pow(tA-tB, 3));
}

double Functions::PS(const double& q) {
  double ps = -1.;
  if ( isPhiOm ) ps = PhiOmPS(q);
  else ps = pow(1.-q*q/(mh*mh), 3);
  return ps;
}

double Functions::F2(const double& q) {
  // TODO: use 1+(m/l)^2 for pion!?
  double base = 1.-q*q*l2;
  return 1./(base*base);
}

double Functions::KrollWada(double* x, double* p) {
  double mee = x[0];
  return 2.*mee * F2(mee) * QED(mee) * PS(mee);
}
