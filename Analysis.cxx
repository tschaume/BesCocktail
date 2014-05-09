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
    t->SetBranchAddress("eePtR", &Pt);
    t->SetBranchAddress("ptEpR", &ep_pt);
    t->SetBranchAddress("etaEpR", &ep_eta);
    t->SetBranchAddress("ptEmR", &em_pt);
    t->SetBranchAddress("etaEmR", &em_eta);
    t->SetBranchAddress("eeRap", &ee_rap);
  } else {
    t->SetBranchAddress("llmass", &Mee);
    t->SetBranchAddress("mpt", &Pt);
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
  TH1D* mhMeePy = new TH1D("ccbar", "ccbar", Utils::nBins, 0, Utils::mMax); mhMeePy->Sumw2();
  map<string, TH1D*> mhPtPy;
  BOOST_FOREACH(string mr, dbm->getDB().mMRg | ad::map_keys) {
    string key = "ccbar_" + mr;
    mhPtPy[key] = new TH1D(key.c_str(), key.c_str(),
	Utils::nBinsPt, Utils::ptMin, Utils::ptMax);
    mhPtPy[key]->Sumw2();
  }
  if ( fpy ) {
    TTree* tpy = (TTree*)fpy->Get("meTree"); if ( !tpy ) return;
    cout << endl << "ccbar" << endl;
    setBranchAddresses(tpy, 1);
    for ( Long64_t n = 0; n < tpy->GetEntries(); ++n ) {
      Utils::printInfo(n, 1000);
      tpy->GetEntry(n);
      if ( !passCuts() ) continue;
      double wght = dbm->getPyBrWeight2(ep_id, em_id);
      mhMeePy->Fill(Mee, wght);
      BOOST_FOREACH(string mr, dbm->getDB().mMRg | ad::map_keys) {
	vector<double> rnge = dbm->getMRngLimits(mr);
	if ( Mee >= rnge[0] && Mee < rnge[1] ) {
	  string key = "ccbar_" + mr;
	  mhPtPy[key]->Fill(Pt, wght); break;
	}
      }
    }
    fout->cd();
    mhMeePy->Write();
    BOOST_FOREACH(string mr, dbm->getDB().mMRg | ad::map_keys) {
      string key = "ccbar_" + mr;
      mhPtPy[key]->Write();
    }
  }
  // hadron decay contributions
  map<string, TH1D*> mhMee;
  map<string, TH1D*> mhPt;
  BOOST_FOREACH(string p, dbm->getDB().mPrt | ad::map_keys) {
    cout << endl << p << endl;
    mhMee[p] = new TH1D(p.c_str(), p.c_str(), Utils::nBins, 0, Utils::mMax);
    mhMee[p]->Sumw2();
    BOOST_FOREACH(string mr, dbm->getDB().mMRg | ad::map_keys) {
      string key = p + "_" + mr;
      mhPt[key] = new TH1D(key.c_str(), key.c_str(),
	  Utils::nBinsPt, Utils::ptMin, Utils::ptMax);
      mhPt[key]->Sumw2();
    }
    TTree* t = getTree(p); if ( !t ) return;
    setBranchAddresses(t);
    for ( Long64_t n = 0; n < t->GetEntries(); ++n ) {
      Utils::printInfo(n, 1000);
      t->GetEntry(n);
      if ( !passCuts() ) continue;
      mhMee[p]->Fill(Mee);
      BOOST_FOREACH(string mr, dbm->getDB().mMRg | ad::map_keys) {
	vector<double> rnge = dbm->getMRngLimits(mr);
	if ( Mee >= rnge[0] && Mee < rnge[1] ) {
	  string key = p + "_" + mr;
	  mhPt[key]->Fill(Pt); break;
	}
      }
    }
    fout->cd();
    mhMee[p]->Write();
    BOOST_FOREACH(string mr, dbm->getDB().mMRg | ad::map_keys) {
      string key = p + "_" + mr;
      mhPt[key]->Write();
    }
  }
  fout->Close();
}

void Analysis::scale(TH1D* h, const string& p, const int& n) {
  double s = 2.*dbm->getSumBR(p);  // 2 = dy for rapidity window of VM simulation (not eeRap Cut!)
  s *= dbm->getdNdy(p, energy);
  s /= h->GetBinWidth(1) * n;
  h->Scale(s);
}

void Analysis::divByCenter(TH1D* h) {
  for ( Int_t b = 1; b <= h->GetNbinsX(); ++b ) {
    Double_t bx = h->GetBinCenter(b);
    Double_t bc = h->GetBinContent(b);
    Double_t be = h->GetBinError(b);
    h->SetBinContent(b, bc/bx);
    h->SetBinError(b, be/bx);
  }
}

void Analysis::genCocktail() {
  TFile* fin = TFile::Open(Utils::getOutFileName("rawhMee",energy),"read");
  if ( !fin ) return;
  TFile* fout = TFile::Open(Utils::getOutFileName("cocktail",energy),"recreate");
  TH1D* hMeeTotal = new TH1D("hCocktail", "hCocktail", Utils::nBins, 0, Utils::mMax);
  mycoll->SetHistoAtts(hMeeTotal, kRed, 1);
  map<string, TH1D*> hPtTotal;
  BOOST_FOREACH(string mr, dbm->getDB().mMRg | ad::map_keys) {
    string key = "hCocktailPt_" + mr;
    hPtTotal[key] = new TH1D(key.c_str(), key.c_str(),
	Utils::nBinsPt, Utils::ptMin, Utils::ptMax);
    hPtTotal[key]->Sumw2();
  }
  TCanvas* can = new TCanvas("cCocktail", "cocktail", 0, 0, 1000, 707);
  TH1D* h = (TH1D*)can->DrawFrame(0, 1e-6, 3.3, 20);
  mycoll->SetHistoAtts(h, 0, 0);
  // hadron decay contributions
  BOOST_FOREACH(string p, dbm->getDB().mPrt | ad::map_keys) {
    TTree* t = getTree(p); if ( !t ) return;
    h = (TH1D*)fin->Get(p.c_str());
    mycoll->SetHistoAtts(h, Utils::mColorMap[p], 1);
    //h->SetFillColor(Utils::mColorMap[p]);
    //h->SetFillStyle(3003);
    scale(h, p, t->GetEntries());
    h->DrawCopy("hsame");
    fout->cd(); h->Write();
    hMeeTotal->Add(h);
    BOOST_FOREACH(string mr, dbm->getDB().mMRg | ad::map_keys) {
      string key = p + "_" + mr;
      h = (TH1D*)fin->Get(key.c_str());
      scale(h, p, t->GetEntries()); // dN/dpT (dpT = constant)
      vector<double> rnge = dbm->getMRngLimits(mr);
      h->Scale(1./(rnge[1]-rnge[0])); // dN/dpTdMee (dMee = constant)
      fout->cd(); h->Write(); // single contribution not divided by pT!!
      hPtTotal["hCocktailPt_"+mr]->Add(h);
    }
  }
  // charm continuum from pythia
  h = (TH1D*)fin->Get("ccbar");
  if ( h ) {
    mycoll->SetHistoAtts(h, Utils::mColorMap["ccbar"], 1);
    //h->SetFillColor(Utils::mColorMap["ccbar"]);
    //h->SetFillStyle(3003);
    double s = Ncoll / h->GetBinWidth(1) / Ncc * rBRcc;
    h->Scale(s);
    h->DrawCopy("hsame");
    fout->cd(); h->Write();
    hMeeTotal->Add(h);
  }
  BOOST_FOREACH(string mr, dbm->getDB().mMRg | ad::map_keys) {
    h = (TH1D*)fin->Get(("ccbar_"+mr).c_str());
    if ( h ) {
      double s = Ncoll / h->GetBinWidth(1) / Ncc * rBRcc;
      h->Scale(s);
      vector<double> rnge = dbm->getMRngLimits(mr);
      s /= rnge[1]-rnge[0]; // divide pt spectrum by Mee width
      fout->cd(); h->Write();
      hPtTotal["hCocktailPt_"+mr]->Add(h);
    }
  }
  hMeeTotal->Draw("hsame");
  mycoll->plotLatexLine(Form("%.0f GeV", energy), .5, .5);
  fout->cd(); hMeeTotal->Write(); can->Write();
  BOOST_FOREACH(string mr, dbm->getDB().mMRg | ad::map_keys) {
    //divByCenter(hPtTotal["hCocktailPt_"+mr]);
    hPtTotal["hCocktailPt_"+mr]->Write();
  }
  fout->Close();
}
