// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_ANALYSIS_H_
#define STROOT_BESCOCKTAIL_ANALYSIS_H_

#include <string>
#include <map>
#include <TH1D.h>
#include "StRoot/BesCocktail/Database.h"

using std::string;
using std::map;

class Analysis {
  private:
    string particle;
    double energy;
    DatabaseManager* dbm;
    map<string, TH1D*> mhMee;

  public:
    Analysis(const double&);  // energy
    virtual ~Analysis() {}

    void loop();
    void genCocktail();
};
#endif  // STROOT_BESCOCKTAIL_ANALYSIS_H_
