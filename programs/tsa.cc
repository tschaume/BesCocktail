// Copyright (c) 2013 Patrick Huck
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <TF1.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>

#include "StRoot/BesCocktail/Functions.h"
#include "StRoot/BesCocktail/Utils.h"

int main(int argc, char **argv) {
  try {
    const int nC = 5;
    TFile* fTs = TFile::Open("fTs.root", "recreate");
    TF1* fPt[nC];
    Functions* fp[nC];
    double p[nC][6] = {  // T, beta, betaS, R, q, n
      { 0.122, 0.470, 0.470/1.5, 1.0, 1.018, 1.0 },  // 0-10%
      { 0.122, 0.475, 0.475/1.5, 1.0, 1.015, 1.0 },  // 10-20%
      { 0.124, 0.441, 0.441/1.5, 1.0, 1.024, 1.0 },  // 20-40%
      { 0.119, 0.282, 0.282/1.5, 1.0, 1.066, 1.0 },  // 40-60%
      { 0.114, 0.000, 0.000/1.5, 1.0, 1.086, 1.0 }   // 60-80%
    };
    //double yld[nC] = {};

    TCanvas* c = new TCanvas("c" , "", 1);
    c->DrawFrame(0, 1e-7, 3, 0.1);
    TH1D* hwSum;
    for ( int i = 0; i < nC; ++i ) { 
      fp[i] = new Functions(p[i]);
      fPt[i] = new TF1(Form("fPt[%d]", i), fp[i], &Functions::Tsallis, 0, 10, 0);
      fPt[i]->SetNpx(100);
      //if ( i == 0 ) hwSum = (TH1D*)fPt[i]->GetHistogram()->Scale(yld[i]);
      //else hwSum->Add((TH1D*)fPt[i]->GetHistogram()->Scale(yld[i]));
      fPt[i]->Draw("same");
      fPt[i]->Write();
    }

    hwSum->Draw("same");
    hwSum->Write();
    c->Write();
    fTs->Close();
  }
  catch(const std::exception& e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
