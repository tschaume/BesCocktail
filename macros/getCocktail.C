void write(TH1D* h, const char* name, const int& energy, const char* suffix, TH1D* hSysErr) {
  ofstream outData;
  outData.open(Form("%s%d%s.dat", name, energy, suffix));
  for ( Int_t b = 1; b <= h->GetNbinsX(); ++b ) {
    Double_t bc = h->GetBinContent(b);
    Double_t beSysErr = 0.;
    if ( hSysErr != NULL ) { beSysErr = hSysErr->GetBinContent(b); }
    if ( bc > 0. ) {
      outData
	<< h->GetBinCenter(b) << " "
	<< bc << " "
	<< h->GetBinWidth(b)/2. << " "
	<< h->GetBinError(b) << " "
	<< beSysErr << endl;
    }
  }
  outData.close();
}

void getCocktail(const Int_t& energy, const string& in, const string& out, const string& suffix) {
  TFile* f = TFile::Open(Form("%d/cocktail.root", energy), "read");
  TH1D* h = (TH1D*)f->Get(in.c_str());
  string key = (in.find("Pt")!=std::string::npos) ? in+"_SysErr" : in+"SysErr";
  TH1D* hSysErr = (TH1D*)f->Get(key.c_str());
  if ( in == "ccbar" ) { h->Rebin(10)->Scale(1./10.); }
  write(h, out.c_str(), energy, suffix.c_str(), hSysErr);
}
