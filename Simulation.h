// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_SIMULATION_H_
#define STROOT_BESCOCKTAIL_SIMULATION_H_

#include <string>
#include <vector>
#include <TRandom3.h>
#include <TLorentzVector.h>
#include <TF1.h>
#include "StRoot/BesCocktail/Database.h"
#include "StRoot/BesCocktail/Functions.h"

class Simulation {
  private:
    std::string particle;
    double energy;

    TRandom3* yGen;
    TRandom3* phiGen;
    double mass;  // hadron mass
    DatabaseManager* dbm;
    TLorentzVector* lvIn;
    TLorentzVector* ep;
    TLorentzVector* em;
    Functions* fp;
    TF1* fPt;  // input pt distribution
    TF1* fM;  // input mass distribution
    TF1* fRes;  // pt momentum resolution
    TF1* fCB;  // CrystalBall for momentum smearing
    std::vector<double> vfill;
    Float_t* afill;

    double getEta(const double&);
    double getPhi();
    double getY();
    void applyMomSmear(TLorentzVector&);

  public:
    Simulation(const std::string&, const double&);
    virtual ~Simulation() {}

    static double ptMin;
    static double ptMax;
    static double mMin;
    static double mMax;

    void sampleInput();
    void doTwoBodyDecay();
    void smear();
    Float_t* getFillArray();
};
#endif  // STROOT_BESCOCKTAIL_SIMULATION_H_
