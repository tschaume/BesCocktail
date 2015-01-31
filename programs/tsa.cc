// Copyright (c) 2013 Patrick Huck
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <string>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TMath.h>
#include <TGraphErrors.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>

#include "StRoot/BesCocktail/Functions.h"
#include "StRoot/BesCocktail/Utils.h"
#include "StRoot/BesCocktail/Database.h"

namespace ad = boost::adaptors;

int main(int argc, char **argv) {
  try {
    DatabaseManager* dbm = DatabaseManager::Instance();
    // TODO: 19 GeV from Bingchu Text Files
    const int nEnergies = 3;
    int energies[nEnergies] = {27, 39, 62};
    TFile* fin = TFile::Open("out/TsallisBlastWaveFits/tbw.root", "recreate");
    fin->cd(); TH1::AddDirectory(kFALSE);

    for ( int eidx = 0; eidx < nEnergies; ++eidx ) {
      int energy = energies[eidx];
      TFile *f = TFile::Open(Form("out/TsallisBlastWaveFits/testTBW_%d.root", energy), "read");
      fin->cd();
      BOOST_FOREACH(string particle, dbm->getDB().mPrtTsa | ad::map_keys) {
        string suf = particle + Form("_%.1f", (float)energy);
        // get fit and yield from Joey's file
        string particle_mod = particle;
        if ( energy == 27 ) {
          if ( particle == "kp" ) particle_mod = "kap";
          if ( particle == "km" ) particle_mod = "kam";
          if ( particle == "p" ) particle_mod = "prp";
          if ( particle == "pbar" ) particle_mod = "prm";
        }
        string hname = particle_mod;
        if ( energy != 27 ) { hname += Form("_spectra_%d_MinBias", energy); }
        else { hname += Form("_pTspectra_%dgev_MinBias", energy); }
        TGraphErrors* dN2pipTdpTdy_Data = (TGraphErrors*)f->Get(hname.c_str());
        dN2pipTdpTdy_Data->SetName(Form("2pipTdpTdy_Data_%s", suf.c_str()));
        dN2pipTdpTdy_Data->Write();
        TF1* func_dN2pipTdpTdy_Joey = dN2pipTdpTdy_Data->GetFunction("pTTBW");
        double norm_factor = func_dN2pipTdpTdy_Joey->GetParameter(3)/0.8;
        std::cout << norm_factor << std::endl;
        TH1* dN2pipTdpTdy_Joey = func_dN2pipTdpTdy_Joey->GetHistogram();
        dN2pipTdpTdy_Joey->SetName(Form("dN2pipTdpTdy_Joey_%s", suf.c_str()));
        dN2pipTdpTdy_Joey->Scale(1/0.8); // factor 1/0.8 omitted by Joey
        dN2pipTdpTdy_Joey->Write();
        // reproduce TBW from PH's database
        Functions* fp = new Functions(particle, energy);
        TF1* fPt = new TF1(Form("fPt_%s", suf.c_str()),
            fp, &Functions::Tsallis, Utils::ptMin, Utils::ptMax, 0);
        fPt->SetNpx(500);
        TH1D* dN_Pat = (TH1D*)fPt->GetHistogram();
        dN_Pat->SetName(Form("dN_Pat_%s", suf.c_str())); //dN_Pat->Write();
        TH1D* dN2pipTdpTdy_Pat = (TH1D*)dN_Pat->Clone(Form("dN2pipTdpTdy_Pat_%s", suf.c_str()));
        for ( Int_t bx = 1; bx <= dN2pipTdpTdy_Pat->GetNbinsX(); ++bx) {
          Double_t pT = dN2pipTdpTdy_Pat->GetBinCenter(bx);
          Double_t bc = dN2pipTdpTdy_Pat->GetBinContent(bx);
          dN2pipTdpTdy_Pat->SetBinContent(bx, norm_factor*bc/pT);
        }
        dN2pipTdpTdy_Pat->Write();
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
