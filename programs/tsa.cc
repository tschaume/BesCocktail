// Copyright (c) 2013 Patrick Huck
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <string>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>

#include "StRoot/BesCocktail/Functions.h"
#include "StRoot/BesCocktail/Utils.h"

int main(int argc, char **argv) {
  try {
    int energies[4] = {19, 27, 39, 62};
    string particles[1] = {"pion"};
    TFile* fin = TFile::Open("tbw.root", "recreate");
    fin->cd(); TH1::AddDirectory(kFALSE);

    for ( int eidx = 0; eidx < 4; ++eidx ) {
      for ( int pidx = 0; pidx < 1; ++pidx ) {
        double energy = energies[eidx];
        std::string particle = particles[pidx];
        Functions* fp = new Functions(particle, energy);
        string suf = particle + Form("_%.1f", energy);
        TF1* fPt = new TF1(Form("fPt_%s", suf.c_str()),
            fp, &Functions::Tsallis, Utils::ptMin, Utils::ptMax, 0);
        fPt->SetNpx(500); fPt->Write();
        TH1D* hPt = (TH1D*)fPt->GetHistogram();
        hPt->SetName(Form("hPt_%s", suf.c_str())); hPt->Write();
        std::cout << energy << " " << particle << " done" << std::endl;
      }
    }

    fin->Close();
  }
  catch(const std::exception& e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
