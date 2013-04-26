// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_FUNCTIONS_H_
#define STROOT_BESCOCKTAIL_FUNCTIONS_H_

#include <string>
#include <vector>
#include <TF1.h>
#include "StRoot/BesCocktail/Database.h"

using std::string;
using std::vector;

const int nGL = 10;  // Gauss-Legendre sample points

class Functions {
  private:
    string particle;
    double energy;
    DatabaseManager* dbm;
    vector<double> vhp;  // HagedornPower Params
    vector<double> vres;  // Pt Resolution Params
    vector<double> vcr;  // CrystalBall params
    double fsfac;  // final state factor
    double mh;  // hadron mass
    double wdth;  // BW decay width
    bool isPhiOm;  // true if is phi or omega
    double mhdec;  // decay hadron mass
    double l2;  // Lambda^(-2)
    double g02;  // Gamma0^2
    TF1* fF2;  // Formfactor function
    double mpi;
    double tsT;   // temperature
    double tsb;   // average flow velocity
    double tsbS;  // surface flow velocity
    double tsR;   // hard-spherical R (surface)
    double tsq;   // degree of equilibrium
    double tsn;   // order of flow profile
    TF1* fTsR;  // radial Integrand
    TF1* fTsPhi;  // azimuthal Integrand
    TF1* fTsRap;  // rapidity Integrand
    // Gauss-Legendre n-point quadrature formula:
    double xGL[nGL];  // abscissa
    double wGL[nGL];  // weight

    double HagedornPower(const double&);
    double getCrystalA(const double&, const double&);
    double getCrystalB(const double&, const double&);
    double QED(const double&);
    double PhiOmPS(const double&);
    double PS(const double&);
    double pionF2(double*, double*);
    double otherF2(double*, double*);
    double TsallisRho(const double&);
    double TsallisRadialBase(double*, double*);
    double TsallisPhiBase(double*, double*);
    double TsallisRapBase(double*, double*);

  public:
    Functions(const string&, const double&);
    virtual ~Functions() {}

    double MtScaling(double*, double*);
    double BreitWigner(double*, double*);
    double MomRes(double*, double*);
    double CrystalBall2(double*, double*);
    double KrollWada(double*, double*);
    double Tsallis(double*, double*);
};
#endif  // STROOT_BESCOCKTAIL_FUNCTIONS_H_
