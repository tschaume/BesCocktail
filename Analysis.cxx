// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Analysis.h"
#include <iostream>
#include <map>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include <TH1D.h>
#include <TFile.h>
#include "StRoot/BesCocktail/Utils.h"

using std::map;
namespace ad = boost::adaptors;

Analysis::Analysis(const double& e) : energy(e) {
  // database manager & variables
  dbm = DatabaseManager::Instance();
  mPtTrCut = dbm->getDB().mHdr["header"].mPars["cuts"].at(0);
  mEtaTrCut = dbm->getDB().mHdr["header"].mPars["cuts"].at(1);
  mRapPairCut = dbm->getDB().mHdr["header"].mPars["cuts"].at(2);
}

bool Analysis::passTrackCuts(const Float_t& pt, const Float_t& eta) {
  if ( pt > mPtTrCut && fabs(eta) < mEtaTrCut ) return true;
  return false;
}

bool Analysis::passCuts() {
  if ( !passTrackCuts(ep_pt, ep_eta) ) return false;
  if ( !passTrackCuts(em_pt, em_eta) ) return false;
  if ( fabs(ee_rap) >= mRapPairCut ) return false;
  return true;
}

void Analysis::setBranchAddresses(TTree* t) {
  t->SetBranchAddress("eeMassR", &Mee);
  t->SetBranchAddress("ptEpR", &ep_pt);
  t->SetBranchAddress("etaEpR", &ep_eta);
  t->SetBranchAddress("ptEmR", &em_pt);
  t->SetBranchAddress("etaEmR", &em_eta);
  t->SetBranchAddress("eeRap", &ee_rap);
}

void Analysis::loop() {
  TFile* fout = TFile::Open(Utils::getOutFileName("rawhMee",energy),"recreate");
  map<string, TH1D*> mhMee;
  BOOST_FOREACH(string p, dbm->getDB().mDb | ad::map_keys) {
    cout << endl << p << endl;
    mhMee[p] = new TH1D(p.c_str(), p.c_str(), 700, 0, 3.5);
    mhMee[p]->Sumw2();
    TFile* f = TFile::Open(Utils::getOutFileName(p,energy),"read");
    if ( !f ) return;
    TTree* t = (TTree*)f->Get(p.c_str());
    Long64_t nentries = t->GetEntries();
    setBranchAddresses(t);
    for ( Long64_t n = 0; n < nentries; ++n ) {
      Utils::printInfo(n, 1000);
      t->GetEntry(n);
      if ( !passCuts() ) continue;
      mhMee[p]->Fill(Mee);
    }
    fout->cd();
    mhMee[p]->Write();
  }
  fout->Close();
}

void Analysis::genCocktail() {
  // TODO: add charm continuum from pythia
  TFile* fin = TFile::Open(Utils::getOutFileName("rawhMee",energy),"read");
  if ( !fin ) return;
  TH1D* hMeeTotal = new TH1D("hCocktail", "hCocktail", 700, 0, 3.5);
  BOOST_FOREACH(string p, dbm->getDB().mDb | ad::map_keys) {
    hMeeTotal->Add((TH1D*)fin->Get(p.c_str()));
  }
  TFile* fout = TFile::Open(Utils::getOutFileName("cocktail",energy),"recreate");
  hMeeTotal->Write();
  fout->Close();
}
