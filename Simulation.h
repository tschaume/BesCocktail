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
    double mBR;  // fraction of dalitz in total branching ratio (ee+dalitz)
    int mode;  // decay mode
    DatabaseManager* dbm;
    TLorentzVector* ep;
    TLorentzVector* em;
    TLorentzVector* dh;
    TLorentzVector* dlp;
    Functions* fp;
    TF1* fPt;  // input pt distribution
    TF1* fM;  // input mass distribution
    TF1* fRes;  // pt momentum resolution
    TF1* fCB;  // CrystalBall for momentum smearing
    TF1* fKW;  // KrollWada for dalitz decays
    std::vector<double> vfill;
    Float_t* afill;
    bool isDhKin;  // flag true if decay hadron kinematics

    double getEta(const double&);
    void applyMomSmear(TLorentzVector&);
    void doTwoBodyDecay();
    void doDalitzDecay();
    void twoBodyKinematics(const double&, double&, double&, double&);
    void eeDecayVM(const double&);
    void hDecayVM(const double&);
    void pushNtVars();

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
