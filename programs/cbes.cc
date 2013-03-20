// Copyright (c) 2013 Patrick Huck
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <TF1.h>
#include <TNtuple.h>
#include <TFile.h>

#include "StRoot/BesCocktail/CmdLine.h"
#include "StRoot/BesCocktail/Functions.h"
#include "StRoot/BesCocktail/Utils.h"

const double ptMin = 0.;
const double ptMax = 10.;

int main(int argc, char **argv) {
  try {
    // parse command line options & arguments
    CmdLine* clopts = new CmdLine();
    if ( !clopts->parse(argc, argv) ) return 0;
    clopts->print();

    // init utils, outfile & ntuple
    Utils* ut = new Utils(clopts->particle);  // inits random generators, too
    TFile* fout = new TFile(ut->getOutFileName(clopts->particle), "recreate");
    const char* nt_name = clopts->particle.c_str();
    TNtuple* nt = new TNtuple(nt_name, nt_name, "ptM:etaM:phiM", 0);

    // init functions
    Functions* fp = new Functions(clopts->particle, clopts->energy);
    TF1* fPt = new TF1("fPt", fp, &Functions::MtScaling, ptMin, ptMax, 0);
    fPt->SetNpx(10000);

    // start loop
    for ( int n = 0; n < clopts->ndecays; ++n ) {
      ut->printInfo(n);
      Double_t pt = fPt->GetRandom(ptMin,ptMax);
      Double_t eta = ut->getEta(pt);  // random based on uniform y [-1,1]
      Double_t phi = ut->getPhi();  // random phi [0,2pi]
      nt->Fill(pt,eta,phi);
    }

    // finish up
    nt->Write();
    fout->Close();
  }
  catch(const std::exception& e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
