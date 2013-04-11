// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Analysis.h"
#include <iostream>
#include <map>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include <TCanvas.h>
#include "StRoot/BesCocktail/Utils.h"

using std::map;
using std::cout;
using std::endl;
namespace ad = boost::adaptors;

Analysis::Analysis(const double& e, const string& s) : energy(e), pyfile(s) {
  mycoll = new MyCollection();
  // database manager & variables
  dbm = DatabaseManager::Instance();
  vector<double> vcuts = dbm->getHdrVar("cuts");
  mPtTrCut = vcuts[0];
  mEtaTrCut = vcuts[1];
  mRapPairCut = vcuts[2];
  // pythia input and Ncc
  fpy = TFile::Open(pyfile.c_str(),"read");
  if ( fpy ) Ncc = ((TH1F*)fpy->Get("eventCounter"))->GetBinContent(2);
  Ncoll = dbm->getNcoll(energy);
  rBRcc = dbm->getCcbarBrRatio(energy);
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

void Analysis::setBranchAddresses(TTree* t, bool py) {
  if ( !py ) {
    t->SetBranchAddress("eeMassR", &Mee);
    t->SetBranchAddress("ptEpR", &ep_pt);
    t->SetBranchAddress("etaEpR", &ep_eta);
    t->SetBranchAddress("ptEmR", &em_pt);
    t->SetBranchAddress("etaEmR", &em_eta);
    t->SetBranchAddress("eeRap", &ee_rap);
  } else {
    t->SetBranchAddress("llmass", &Mee);
    t->SetBranchAddress("pospt", &ep_pt);
    t->SetBranchAddress("poseta", &ep_eta);
    t->SetBranchAddress("negpt", &em_pt);
    t->SetBranchAddress("negeta", &em_eta);
    t->SetBranchAddress("mrap", &ee_rap);
    t->SetBranchAddress("posPGID", &ep_id);
    t->SetBranchAddress("negPGID", &em_id);
  }
}

TTree* Analysis::getTree(const string& p) {
  TFile* f = TFile::Open(Utils::getOutFileName(p,energy),"read");
  if ( !f ) return NULL;
  return (TTree*)f->Get(p.c_str());
}

void Analysis::loop() {
  TFile* fout = TFile::Open(Utils::getOutFileName("rawhMee",energy),"recreate");
  // charm continuum from pythia
  TH1D* mhMeePy = new TH1D("ccbar", "ccbar", 700, 0, 3.5); mhMeePy->Sumw2();
  if ( fpy ) {
    TTree* tpy = (TTree*)fpy->Get("meTree"); if ( !tpy ) return;
    cout << endl << "ccbar" << endl;
    setBranchAddresses(tpy, 1);
    for ( Long64_t n = 0; n < tpy->GetEntries(); ++n ) {
      Utils::printInfo(n, 1000);
      tpy->GetEntry(n);
      if ( !passCuts() ) continue;
      mhMeePy->Fill(Mee, dbm->getPyBrWeight2(ep_id, em_id));
    }
    fout->cd();
    mhMeePy->Write();
  }
  // hadron decay contributions
  map<string, TH1D*> mhMee;
  BOOST_FOREACH(string p, dbm->getDB().mPrt | ad::map_keys) {
    cout << endl << p << endl;
    mhMee[p] = new TH1D(p.c_str(), p.c_str(), 700, 0, 3.5);
    mhMee[p]->Sumw2();
    TTree* t = getTree(p); if ( !t ) return;
    setBranchAddresses(t);
    for ( Long64_t n = 0; n < t->GetEntries(); ++n ) {
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
  TFile* fin = TFile::Open(Utils::getOutFileName("rawhMee",energy),"read");
  if ( !fin ) return;
  TFile* fout = TFile::Open(Utils::getOutFileName("cocktail",energy),"recreate");
  TH1D* hMeeTotal = new TH1D("hCocktail", "hCocktail", 700, 0, 3.5);
  mycoll->SetHistoAtts(hMeeTotal, kRed, 1);
  TCanvas* can = new TCanvas("cCocktail", "cocktail", 0, 0, 1000, 707);
  TH1D* h = (TH1D*)can->DrawFrame(0, 1e-7, 3.5, 10);
  mycoll->SetHistoAtts(h, 0, 0);
  // hadron decay contributions
  BOOST_FOREACH(string p, dbm->getDB().mPrt | ad::map_keys) {
    TTree* t = getTree(p); if ( !t ) return;
    h = (TH1D*)fin->Get(p.c_str());
    mycoll->SetHistoAtts(h, Utils::mColorMap[p], 1);
    h->SetFillColor(Utils::mColorMap[p]);
    h->SetFillStyle(3003);
    scale(h, p, t->GetEntries());
    h->DrawCopy("hsame");
    fout->cd(); h->Write();
    hMeeTotal->Add(h);
  }
  // charm continuum from pythia
  h = (TH1D*)fin->Get("ccbar");
  if ( h ) {
    mycoll->SetHistoAtts(h, Utils::mColorMap["ccbar"], 1);
    h->SetFillColor(Utils::mColorMap["ccbar"]);
    h->SetFillStyle(3003);
    double s = Ncoll / h->GetBinWidth(1) / Ncc * rBRcc;
    h->Scale(s);
    h->DrawCopy("hsame");
    fout->cd(); h->Write();
    hMeeTotal->Add(h);
  }
  hMeeTotal->Draw("hsame");
  fout->cd(); hMeeTotal->Write(); can->Write();
  fout->Close();
}
