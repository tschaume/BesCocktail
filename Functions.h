// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_FUNCTIONS_H_
#define STROOT_BESCOCKTAIL_FUNCTIONS_H_

#include <string>
#include "StRoot/BesCocktail/Database.h"

class Functions {
  private:
    std::string particle;
    double energy;
    DatabaseManager* dbm;

  public:
    Functions(const std::string&, const double&);
    virtual ~Functions() {}

    double HagedornPower(const double&);
    double MtScaling(double*, double*);
};
#endif  // STROOT_BESCOCKTAIL_FUNCTIONS_H_
