// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Functions.h"
#include "StRoot/BesCocktail/Utils.h"
#include <cmath>
#include <TMath.h>

Functions::Functions(const std::string& p, const double& e)
: particle(p), energy(e) {
  dbm = DatabaseManager::Instance();
  vhp = dbm->getHgd(particle, energy);
  vres = dbm->getHdrVar("res");
  vcr = dbm->getHdrVar("crystal");
  fsfac = dbm->getProperty(particle, "fsfac");
  l2 = dbm->getProperty(particle, "l2");
  wdth = dbm->getProperty(particle, "width");
  mh = dbm->getProperty(particle, "mass");
  mhdec = dbm->getDecayMass(particle);
  g02 = dbm->getProperty(particle, "g02");
  mpi = dbm->getProperty("pion", "mass");
  isPhiOm = ( particle == "phi" || particle == "omega" );
  if ( particle == "pion" ) {
    fF2 = new TF1("fF2", this, &Functions::pionF2, Utils::mMin, mh-mhdec, 0);
  } else {
    fF2 = new TF1("fF2", this, &Functions::otherF2, Utils::mMin, mh-mhdec, 0);
  }
  // tsallis parameters
  tsT = 0.1191;
  tsq = 1.0132;
  tsb = 0.3974;
  tsbS = 0.5961;
  tsR = 1.;
  fTsR = new TF1("fTsR", this, &Functions::TsallisRadialBase, 0, tsR, 3);
  fTsPhi = new TF1("fTsPhi", this, &Functions::TsallisPhiBase, -TMath::Pi(), TMath::Pi(), 2);
  fTsRap = new TF1("fTsRap", this, &Functions::TsallisRapBase, -1, 1, 1);
}

double Functions::HagedornPower(const double& x) {
  double e = exp(-vhp[1]*x-vhp[2]*x*x);
  double base = e + x / vhp[3];
  return vhp[0] * x * pow(base,-vhp[4]);
}

double Functions::MtScaling(double* x, double* p) {
  double mt = sqrt(x[0]*x[0] + mh*mh - mpi*mpi);
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

double Functions::pionF2(double* x, double* par) {  // (1+m*m*l2)^2
  double b = 1.+x[0]*x[0]*l2; return b*b;
}

double Functions::otherF2(double* x, double* par) {  // 1/[ (1-m*m*l2)^2 + g02*l2 ]
  double b = 1.-x[0]*x[0]*l2; return 1./(b*b+g02*l2);
}

double Functions::KrollWada(double* x, double* p) {
  double mee = x[0];
  double F2 = fF2->Eval(mee);
  return 2.*mee * F2 * QED(mee) * PS(mee);
}

double Functions::TsallisRho(const double& r) {
  double tsn = 1. / (tsq - 1.);
  return atanh(tsbS * pow(r/tsR, tsn));
}

double Functions::TsallisRadialBase(double* x, double* p) {  // x = r, p = [pT, y, phi]
  double r(x[0]), pT(p[0]), y(p[1]), phi(p[2]);
  double mT = sqrt(pT*pT+mh*mh);
  double rho = TsallisRho(r);
  double hypgeo = mT * cosh(y) * cosh(rho);
  hypgeo -= pT * sinh(rho) * cos(phi);
  double base = r * pow(1. + (tsq-1.)/tsT * hypgeo, -1./(tsq-1.));
  return base;
}

double Functions::TsallisPhiBase(double* x, double* p) {  // x = phi, p = [pT, y]
  fTsR->FixParameter(0, p[0]);
  fTsR->FixParameter(1, p[1]);
  fTsR->FixParameter(2, x[0]);
  return fTsR->Integral(0, tsR);
}

double Functions::TsallisRapBase(double* x, double* p) {  // x = y, p = [pT]
  double y(x[0]);
  fTsPhi->FixParameter(0, p[0]);
  fTsPhi->FixParameter(1, y);
  double phiInt = fTsPhi->Integral(-TMath::Pi(), TMath::Pi());
  return cosh(y) * phiInt;
}

double Functions::Tsallis(double* x, double* p) {  // x = pT
  double pT(x[0]);
  double mT = sqrt(pT*pT+mh*mh);
  fTsRap->FixParameter(0, pT);
  return pT * mT * fTsRap->Integral(-1., 1.);
}
