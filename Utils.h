// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_UTILS_H_
#define STROOT_BESCOCKTAIL_UTILS_H_

#include <string>
#include <TRandom3.h>
#include <TLorentzVector.h>
#include <TF1.h>
#include "StRoot/BesCocktail/Database.h"

class Utils {
  private:
    TRandom3* yGen;
    TRandom3* phiGen;
    double phiMax;
    double mass;  // hadron mass
    double getY();
    DatabaseManager* dbm;
    TLorentzVector* lvIn;
    TLorentzVector* ep;
    TLorentzVector* em;
    void applyMomSmear(TLorentzVector&, TF1*, TF1*);

  public:
    Utils(const std::string&);
    virtual ~Utils() {}

    static double emass;

    void printInfo(const int&);
    const char* getOutFileName(const std::string&);
    double getEta(const double&);
    double getPhi();
    void setLvIn(const double&, const double&, const double&, const double&);
    void doTwoBodyDecay();
    void smear(TF1*, TF1*);
    TLorentzVector* getEp() { return ep; }
    TLorentzVector* getEm() { return em; }
};
#endif  // STROOT_BESCOCKTAIL_UTILS_H_
