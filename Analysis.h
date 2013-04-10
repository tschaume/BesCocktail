// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_ANALYSIS_H_
#define STROOT_BESCOCKTAIL_ANALYSIS_H_

#include <string>
#include <TTree.h>
#include "StRoot/BesCocktail/Database.h"

using std::string;

class Analysis {
  private:
    string particle;
    double energy;
    DatabaseManager* dbm;
    double mPtTrCut;
    double mEtaTrCut;
    double mRapPairCut;
    Float_t Mee;
    Float_t ep_pt;
    Float_t ep_eta;
    Float_t em_pt;
    Float_t em_eta;
    Float_t ee_rap;

    void setBranchAddresses(TTree*);
    bool passTrackCuts(const Float_t&, const Float_t&);
    bool passCuts();

  public:
    Analysis(const double&);  // energy
    virtual ~Analysis() {}

    void loop();
    void genCocktail();
};
#endif  // STROOT_BESCOCKTAIL_ANALYSIS_H_
