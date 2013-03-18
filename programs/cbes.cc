// Copyright (c) 2013 Patrick Huck
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <TF1.h>
#include <TNtuple.h>
#include <TFile.h>

#include "StRoot/BesCocktail/CmdLine.h"
#include "StRoot/BesCocktail/Functions.h"

const double ptMin = 0.;
const double ptMax = 10.;

void printInfo(int n) {
  if ( n > 0 && !(n%100000) ) std::cout << n/1000 << "k" << std::endl;
  else if ( !(n%10000) ) std::cout << "." << std::flush;
}

int main(int argc, char **argv) {
  try {
    // parse command line options & arguments
    CmdLine* clopts = new CmdLine();
    if ( !clopts->parse(argc, argv) ) return 0;
    clopts->print();

    // init outfile & ntuple
    const char* fn = (clopts->particle + ".root").c_str();
    TFile* fout = new TFile(fn, "recreate");
    fout->cd();
    const char* nt_name = clopts->particle.c_str();
    TNtuple* nt = new TNtuple(nt_name, nt_name, "PtIn", 0);

    // init functions
    Functions* fp = new Functions(clopts->particle, clopts->energy);
    TF1* fPt = new TF1("fPt", fp, &Functions::MtScaling, ptMin, ptMax, 0);
    fPt->SetNpx(10000);

    // start loop
    for ( int n = 0; n < clopts->ndecays; ++n ) {
      printInfo(n);
      Double_t pt = fPt->GetRandom(ptMin,ptMax);
      nt->Fill(pt);
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
