// Copyright (c) 2013 Patrick Huck
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <TF1.h>
#include <TH1D.h>
#include <TFile.h>

#include "StRoot/BesCocktail/CmdLine.h"
#include "StRoot/BesCocktail/Utils.h"
#include "StRoot/BesCocktail/Functions.h"

const double ptMin = 0.;
const double ptMax = 10.;

int main(int argc, char **argv) {
  try {
    // parse command line options & arguments
    CmdLine* clopts = new CmdLine();
    if ( !clopts->parse(argc, argv) ) return 0;
    clopts->print();

    // init outfile
    TFile* fout = new TFile("test.root","recreate");
    fout->cd();

    // init histograms TODO: ntuple
    TH1D* hPt = new TH1D("hPt","hPt",1000,ptMin,ptMax);

    // init functions
    Functions* fp = new Functions(clopts->particle, clopts->energy);
    TF1* fPt = new TF1("fPt", fp, &Functions::MtScaling, ptMin, ptMax, 0);
    fPt->SetNpx(10000);

    // start loop
    for ( int n = 0; n < clopts->ndecays; ++n ) {
      Utils::printInfo(n);
      Double_t pt = fPt->GetRandom(ptMin,ptMax);
      hPt->Fill(pt);
    }

    // finish up
    hPt->Write();
    fout->Close();
  }
  catch(const std::exception& e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
