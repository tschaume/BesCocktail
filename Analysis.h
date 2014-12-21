// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_ANALYSIS_H_
#define STROOT_BESCOCKTAIL_ANALYSIS_H_

#include <string>
#include <TTree.h>
#include <TH1D.h>
#include <TFile.h>
#include "StRoot/BesCocktail/Database.h"
#include "StRoot/MyCollection/MyCollection.h"

using std::string;

class Analysis {
  private:
    string particle;
    double energy;
    string pyfile;
    DatabaseManager* dbm;
    double mPtTrCut;
    double mEtaTrCut;
    double mRapPairCut;
    Float_t Mee;
    Float_t Pt;
    Float_t ep_pt;
    Float_t ep_eta;
    Float_t em_pt;
    Float_t em_eta;
    Float_t ee_rap;
    Int_t ep_id;
    Int_t em_id;
    MyCollection* mycoll;
    TFile* fpy;
    double Ncc;
    double Ncoll;
    double rBRcc;
    double rel_ccXe;

    TTree* getTree(const string&);
    void setBranchAddresses(TTree*, bool py = false);
    bool passTrackCuts(const Float_t&, const Float_t&);
    bool passCuts();
    void scale(TH1D*, const string&, const int&);
    void scalePy(TH1D*);
    void divByCenter(TH1D*);

  public:
    Analysis(const double&, const string&);  // energy, pydir
    virtual ~Analysis() {}

    void loop();
    void genCocktail();
};
#endif  // STROOT_BESCOCKTAIL_ANALYSIS_H_
