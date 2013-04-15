void ratios() {
  gStyle->SetOptStat(0);
  TFile* fMe  = TFile::Open("out/200/cocktail.root","read");
  //TFile* fXin = TFile::Open("CocktailQM2011.root","read");
  TFile* fJie = TFile::Open("Cocktail200GeV_latest.root","read");

  const int np = 8;
  TString hnXin[np] = {
    "pi0Rc", "etaRc", "etaprimRc", "omegaRc", "phiRc",
    "jpsiRc", "ccRc", "CocktailSum"
  };
  TString hnJie[np] = {
    "hPi0STAR", "hEtaSTAR", "hEtaprimSTAR", "hOmegaSTAR",
    "hPhiSTAR", "hJpsiSTAR", "hccSTAR", "CocktailSum"
  };
  TString hnMe[np]  = {
    "pion", "eta", "etap", "omega", "phi", "jpsi", "ccbar", "hCocktail"
  };
  double hrX1[np] = {0, 0, 0, 0, 0, 0.9, 0, 0};
  double hrX2[np] = {0.12, 0.5, 0.9, 0.9, 1.2, 2.5, 2.8, 3.3};
  double hrY1[np] = {0.5, 0.37, 0.45, 0.4, 0.3, 0.1, 0.8, 0.4};
  double hrY2[np] = {0.8, 0.7, 0.8, 0.8, 14, 1, 1.4, 1.1};
  TH1D* hR[np];
  TCanvas* c = new TCanvas("c", "c", 0, 0, 707, 1000);
  c->Divide(2,4);

  for ( int i = 0; i < np; ++i ) {
    c->cd(i+1);
    if ( i == 4 ) gPad->SetLogy();
    hR[i] = (TH1D*)fMe->Get(hnMe[i].Data());
    hR[i]->Divide((TH1D*)fJie->Get(hnJie[i].Data()));
    hR[i]->GetXaxis()->SetRangeUser(hrX1[i], hrX2[i]);
    hR[i]->GetYaxis()->SetRangeUser(hrY1[i], hrY2[i]);
    hR[i]->Draw();
  }

  c->Print("CocktailRatio200GeV.png");

}
