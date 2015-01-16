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
