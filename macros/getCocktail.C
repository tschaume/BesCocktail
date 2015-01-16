#include "write.h"

void getCocktail(const Int_t& energy, const string& in, const string& out, const string& suffix) {
  TFile* f = TFile::Open(Form("%d/cocktail.root", energy), "read");
  TH1D* h = (TH1D*)f->Get(in.c_str());
  string key = (in.find("Pt")!=std::string::npos) ? in+"_SysErr" : in+"SysErr";
  TH1D* hSysErr = (TH1D*)f->Get(key.c_str());
  if ( in == "ccbar" ) { h->Rebin(10)->Scale(1./10.); }
  write(h, out.c_str(), energy, suffix.c_str(), hSysErr);
}
