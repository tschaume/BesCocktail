// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Analysis.h"
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include <TNtuple.h>
#include <TFile.h>
#include "StRoot/BesCocktail/Utils.h"

namespace ad = boost::adaptors;

Analysis::Analysis(const double& e) : energy(e) {
  // database manager & variables
  dbm = DatabaseManager::Instance();
}

void Analysis::loop() {
  BOOST_FOREACH(string p, dbm->getDB().mDb | ad::map_keys) {
    TFile* f = TFile::Open(Utils::getOutFileName(p,energy),"read");
    if ( !f ) return;
    TNtuple* nt = (TNtuple*)f->Get(p.c_str());
    cout << nt->GetEntries() << endl;
  }
}
