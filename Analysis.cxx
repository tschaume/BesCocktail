// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Analysis.h"
#include <iostream>
#include <map>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include <TFile.h>
#include "StRoot/BesCocktail/Utils.h"

using std::map;
using std::cout;
using std::endl;
namespace ad = boost::adaptors;

Analysis::Analysis(const double& e) : energy(e) {
  // database manager & variables
  dbm = DatabaseManager::Instance();
  vector<double> vcuts = dbm->getHdrVar("cuts");
  mPtTrCut = vcuts[0];
  mEtaTrCut = vcuts[1];
  mRapPairCut = vcuts[2];
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

TTree* Analysis::getTree(const string& p) {
  TFile* f = TFile::Open(Utils::getOutFileName(p,energy),"read");
  if ( !f ) return NULL;
  return (TTree*)f->Get(p.c_str());
}

void Analysis::loop() {
  TFile* fout = TFile::Open(Utils::getOutFileName("rawhMee",energy),"recreate");
  map<string, TH1D*> mhMee;
  BOOST_FOREACH(string p, dbm->getDB().mPrt | ad::map_keys) {
    cout << endl << p << endl;
    mhMee[p] = new TH1D(p.c_str(), p.c_str(), 700, 0, 3.5);
    mhMee[p]->Sumw2();
    TTree* t = getTree(p); if ( !t ) return;
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

void Analysis::scale(TH1D* h, const string& p, const int& n) {
  double s = 2.*mRapPairCut * dbm->getSumBR(p);
  s *= dbm->getdNdy(p, energy);
  s /= h->GetBinWidth(1) * n;
  h->Scale(s);
}

void Analysis::genCocktail() {
  // TODO: add charm continuum from pythia
  TFile* fin = TFile::Open(Utils::getOutFileName("rawhMee",energy),"read");
  if ( !fin ) return;
  TFile* fout = TFile::Open(Utils::getOutFileName("cocktail",energy),"recreate");
  TH1D* hMeeTotal = new TH1D("hCocktail", "hCocktail", 700, 0, 3.5);
  BOOST_FOREACH(string p, dbm->getDB().mPrt | ad::map_keys) {
    TTree* t = getTree(p); if ( !t ) return;
    TH1D* h = (TH1D*)fin->Get(p.c_str());
    scale(h, p, t->GetEntries());
    fout->cd(); h->Write();
    hMeeTotal->Add(h);
  }
  fout->cd(); hMeeTotal->Write();
  fout->Close();
}
