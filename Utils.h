// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_UTILS_H_
#define STROOT_BESCOCKTAIL_UTILS_H_

#include <string>
#include <TRandom3.h>
#include "StRoot/BesCocktail/Database.h"

class Utils {
  private:
    TRandom3* yGen;
    TRandom3* phiGen;
    double phiMax;
    double mass2;
    double getY();
    DatabaseManager* dbm;

  public:
    Utils(const std::string&);
    virtual ~Utils() {}

    void printInfo(const int&);
    const char* getOutFileName(const std::string&);
    double getEta(const double&);
    double getPhi();
};
#endif  // STROOT_BESCOCKTAIL_UTILS_H_
