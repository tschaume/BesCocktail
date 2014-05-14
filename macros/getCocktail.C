void write(TH1D* h, const char* name, const int& energy, const char* suffix) {
  ofstream outData;
  outData.open(Form("%s%d%s.dat", name, energy, suffix));
  for ( Int_t b = 1; b <= h->GetNbinsX(); ++b ) {
    Double_t bc = h->GetBinContent(b);
    if ( bc > 0. ) {
      outData
	<< h->GetBinCenter(b) << " "
	<< bc << " "
	<< h->GetBinWidth(b)/2. << " "
	<< h->GetBinError(b) << " "
	<< bc * 0.3 << endl;
    }
  }
  outData.close();
}

void getCocktail(const Int_t& energy, const string& in, const string& out, const string& suffix) {
  TFile* f = TFile::Open(Form("%d/cocktail.root", energy), "read");
  TH1D* h = (TH1D*)f->Get(in.c_str());
  write(h, out.c_str(), energy, suffix.c_str());
}
