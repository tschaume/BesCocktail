// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Analysis.h"
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include <TTree.h>
#include <TFile.h>
#include "StRoot/BesCocktail/Utils.h"

namespace ad = boost::adaptors;

Analysis::Analysis(const double& e) : energy(e) {
  // database manager & variables
  dbm = DatabaseManager::Instance();
}

void Analysis::loop() {
  TFile* fout = TFile::Open(Utils::getOutFileName("rawhMee",energy),"recreate");
  BOOST_FOREACH(string p, dbm->getDB().mDb | ad::map_keys) {
    cout << endl << p << endl;
    mhMee[p] = new TH1D(p.c_str(), p.c_str(), 700, 0, 3.5);
    mhMee[p]->Sumw2();
    TFile* f = TFile::Open(Utils::getOutFileName(p,energy),"read");
    if ( !f ) return;
    TTree* t = (TTree*)f->Get(p.c_str());
    Long64_t nentries = t->GetEntries();
    Float_t Mee;
    t->SetBranchAddress("eeMassR", &Mee);
    for ( Long64_t n = 0; n < nentries; ++n ) {
      Utils::printInfo(n, 1000);
      t->GetEntry(n);
      mhMee[p]->Fill(Mee);
    }
    fout->cd();
    mhMee[p]->Write();
  }
  fout->Close();
}

void Analysis::genCocktail() {
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
