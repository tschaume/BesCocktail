// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_FUNCTIONS_H_
#define STROOT_BESCOCKTAIL_FUNCTIONS_H_

#include <string>
#include <vector>
#include "StRoot/BesCocktail/Database.h"

using std::string;
using std::vector;

class Functions {
  private:
    string particle;
    double energy;
    DatabaseManager* dbm;
    vector<double> vhp;
    vector<double> vres;
    vector<double> vcr;

  public:
    Functions(const string&, const double&);
    virtual ~Functions() {}

    double HagedornPower(const double&);
    double MtScaling(double*, double*);
    double BreitWigner(double*, double*);
    double MomRes(double*, double*);
    double getCrystalA(const double&, const double&);
    double getCrystalB(const double&, const double&);
    double CrystalBall2(double*, double*);
};
#endif  // STROOT_BESCOCKTAIL_FUNCTIONS_H_
