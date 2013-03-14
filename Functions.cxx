// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Functions.h"
#include "StRoot/BesCocktail/Constants.h"
#include <cmath>

Functions::Functions(const std::string& p) : particle(p) { }

double Functions::Hagedorn(double* x, double* p) {
  double pol1 = Constants::mParCont[particle].a * x[0];
  double pol2 = Constants::mParCont[particle].b * x[0] * x[0];
  double e = exp(-pol1-pol2);
  double base = e + x[0] / Constants::mParCont[particle].p0;
  double power = pow(base,-Constants::mParCont[particle].n);
  return Constants::mParCont[particle].A * x[0] * power;
}

#if 0
double Functions::calcMt(const double x) {
  double mh = Constants::mMass[particle];
  double mpi = Constants::mMass["pion"];
  return sqrt(x*x/c/c - mpi*mpi + mh*mh);
}

double Functions::MtScaling(double* x, double* p) {
  double A   = par[0];
  double a   = par[1];
  double b   = par[2];
  double p0  = par[4];
  double n   = par[5];

  double c   = par[3];
  double mt  = calcMt();

  return A*pow(exp(-(a*mt+b*mt*mt)) + mt/p0, -n) * x[0];
}
#endif
