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

    std::vector<TRandom3*> rndm;
    double mass;  // hadron mass
    double mass_dec;  // daughter hadron mass
    double mPt;  // hadron pt
    double mEta;  // hadron eta
    double mPhi;  // hadron phi
    int mode;  // decay mode
    DatabaseManager* dbm;
    TLorentzVector* ep;
    TLorentzVector* em;
    Functions* fp;
    TF1* fPt;  // input pt distribution
    TF1* fM;  // input mass distribution
    TF1* fRes;  // pt momentum resolution
    TF1* fCB;  // CrystalBall for momentum smearing
    TF1* fKW;  // KrollWada for dalitz decays
    std::vector<double> vfill;
    Float_t* afill;

    double getEta(const double&);
    void applyMomSmear(TLorentzVector&);
    void doTwoBodyDecay();
    void doDalitzDecay();
    void eeDecayVM(const double&);

  public:
    Simulation(const std::string&, const double&);
    virtual ~Simulation() {}

    static double ptMin;
    static double ptMax;
    static double mMin;

    void sampleInput();
    void decay();
    void smear();
    Float_t* getFillArray();
};
#endif  // STROOT_BESCOCKTAIL_SIMULATION_H_
