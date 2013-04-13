void ratios() {
  TFile* fMe  = TFile::Open("out/200/cocktail.root","read");
  TFile* fXin = TFile::Open("CocktailQM2011.root","read");

  const int np = 8;
  TString hnXin[np] = {"pi0Rc", "etaRc", "etaprimRc", "omegaRc", "phiRc", "jpsiRc", "ccRc", "CocktailSum"};
  TString hnMe[np]  = {"pion", "eta", "etap", "omega", "phi", "jpsi", "ccbar", "hCocktail"};
  double hrX1[np] = {0, 0, 0, 0, 0.4, 1, 0, 0};
  double hrX2[np] = {0.14, 0.55, 0.9, 0.8, 2, 3.5, 3.5, 3.5};
  double hrY[np] = {2.4, 2, 2.5, 2.6, 10, 2.5, 2, 2.5};
  TH1D* hR[np];
  TCanvas* c = new TCanvas("c", "c", 0, 0, 707, 1000);
  c->Divide(2,4);

  for ( int i = 0; i < np; ++i ) {
    c->cd(i+1);
    hR[i] = (TH1D*)fMe->Get(hnMe[i].Data());
    hR[i]->Divide((TH1D*)fXin->Get(hnXin[i].Data()));
    hR[i]->GetXaxis()->SetRangeUser(hrX1[i], hrX2[i]);
    hR[i]->GetYaxis()->SetRangeUser(0, hrY[i]);
    hR[i]->Draw();
  }

  c->Print("CocktailRatio200GeV.png");

}
