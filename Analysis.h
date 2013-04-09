// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_ANALYSIS_H_
#define STROOT_BESCOCKTAIL_ANALYSIS_H_

#include <string>
#include "StRoot/BesCocktail/Database.h"

class Analysis {
  private:
    std::string particle;
    double energy;
    DatabaseManager* dbm;

  public:
    Analysis(const double&);  // energy
    virtual ~Analysis() {}
};
#endif  // STROOT_BESCOCKTAIL_ANALYSIS_H_
