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
const double mMin = 2*Utils::emass;
const double mMax = 2.;

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
    TNtuple* nt = new TNtuple(nt_name, nt_name, "ptM:etaM:phiM:mM", 0);

    // init functions
    Functions* fp = new Functions(clopts->particle, clopts->energy);
    TF1* fPt = new TF1("fPt", fp, &Functions::MtScaling, ptMin, ptMax, 0);
    fPt->SetNpx(10000);
    TF1* fM = new TF1("fM", fp, &Functions::BreitWigner, mMin, mMax, 0);
    fM->SetNpx(10000);
    TF1* fRes = new TF1("fRes", fp, &Functions::MomRes, ptMin, ptMax, 0);
    fM->SetNpx(10000);
    TF1* fCB = new TF1("fCB", fp, &Functions::CrystalBall2, -1., 1., 0);
    fCB->SetNpx(10000);

    // start loop
    for ( int n = 0; n < clopts->ndecays; ++n ) {
      ut->printInfo(n);
      // input
      Double_t pt = fPt->GetRandom(ptMin,ptMax);
      Double_t eta = ut->getEta(pt);  // random based on uniform y [-1,1]
      Double_t phi = ut->getPhi();  // random phi [0,2pi]
      Double_t m = fM->GetRandom(mMin, mMax);
      ut->setLvIn(pt, eta, phi, m);
      // decay
      ut->doTwoBodyDecay();
      // smear decay particle momenta
      ut->smear(fCB, fRes);
      // fill output
      nt->Fill(pt,eta,phi,m);
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
