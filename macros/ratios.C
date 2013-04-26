void ratios(bool qm11 = true) {
  gSystem->Load("StRoot/MyCollection/MyCollection_cxx.so");
  MyCollection* mycoll = new MyCollection();
  gStyle->SetOptStat(0);
  TFile* fMe200  = TFile::Open("out/200/cocktail.root","read");
  TFile* fMe19  = TFile::Open("out/19/cocktail.root","read");
  TFile* fXin = TFile::Open("root/CocktailQM2011.root","read");
  TFile* fJie = TFile::Open("root/Cocktail200GeV_latest.root","read");
  TFile* fBingchu = TFile::Open("root/auau19_ee_histo_qm2012.root","read");

  // definitions
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
  TString hnBingchu[np]  = {
    "hpi0dalAcc1_y", "hetadalAcc1_y", "hetaprimdalAcc1_y",
    "homegaAcc1_y", "hphiAcc1_y", "hjpsi2eeAcc1_y",
    "cc2eeAcc1_y", "hcocktail"
  };
  double hrX1[np] = {0.00, 0.00, 0.00, 0.00, 0.00, 2.70, 0.0, 0.0};
  double hrX2[np] = {0.12, 0.55, 0.90, 0.90, 1.15, 3.20, 2.8, 3.3};
  double hrY1[np] = {0.80, 0.45, 0.70, 0.70, 0.70, 0.85, 0.8, 0.4};
  double hrY2[np] = {1.10, 1.05, 1.05, 1.05, 30.0, 1.10, 1.2, 1.1};
  double hrY1_19[np] = {0.85, 0.80, 1.70, 0.9, 0.28, 1.00, 0.4, 0.70};
  double hrY2_19[np] = {1.20, 1.20, 2.05, 1.1, 1.10, 1.20, 1.4, 1.25};

  // cocktail 200GeV
  TCanvas* cc200 = (TCanvas*)fMe200->Get("cCocktail");
  cc200->SetName("cc200");
  cc200->Draw();
  TH1D* ht;
  if ( qm11 ) ht = (TH1D*)fXin->Get(hnXin[7].Data());
  else ht = (TH1D*)fJie->Get(hnJie[7].Data());
  ht->SetLineColor(kBlack);
  ht->SetLineWidth(2);
  gPad->SetLogy();
  gPad->SetGrid();
  ht->Draw("hsame");

  // cocktail 19GeV
  TCanvas* cc19 = (TCanvas*)fMe19->Get("cCocktail");
  cc19->SetName("cc19");
  cc19->Draw();
  TH1D* ht19 = new TH1D("ht19", "ht19", 300, 0, 6);
  ht19->SetLineColor(kBlack);
  ht19->SetLineWidth(2);
  for ( int i = 0; i < 7; ++i ) {
    TH1D* hcur = (TH1D*)fBingchu->Get(hnBingchu[i].Data());
    if ( i != 1 && i != 2 ) {
      hcur->Scale(hcur->GetBinWidth(1));
      hcur->Rebin(4);
      hcur->Scale(1./hcur->GetBinWidth(1));
    }
    ht19->Add(hcur);
  }
  gPad->SetLogy();
  gPad->SetGrid();
  ht19->Draw("hsame");

  // ratios 200GeV
  TH1D* hR200[np];
  TCanvas* c200 = new TCanvas("c200", "c200", 0, 0, 707, 1000);
  c200->Divide(2,4);
  for ( int i = 0; i < np; ++i ) {
    c200->cd(i+1);
    if ( i == 4 ) gPad->SetLogy();
    gPad->SetGrid();
    hR200[i] = (TH1D*)fMe200->Get(hnMe[i].Data());
    hR200[i]->Scale(hR200[i]->GetBinWidth(1));
    hR200[i]->Rebin(4);
    hR200[i]->Scale(1./hR200[i]->GetBinWidth(1));
    TH1D* h;
    if ( qm11 ) {
      h = (TH1D*)fXin->Get(hnXin[i].Data());
      h->Scale(2.);  // dN/dmdy
      h->Scale(h->GetBinWidth(1));
      h->Rebin(4);
      h->Scale(1./h->GetBinWidth(1));
    }
    else {
      h = (TH1D*)fJie->Get(hnJie[i].Data());
      h->Scale(h->GetBinWidth(1));
      h->Rebin(4);
      h->Scale(1./h->GetBinWidth(1));
    }
    hR200[i]->Divide(h);
    hR200[i]->GetXaxis()->SetRangeUser(hrX1[i], hrX2[i]);
    hR200[i]->GetYaxis()->SetRangeUser(hrY1[i], hrY2[i]);
    hR200[i]->Draw();
    //hR200[i]->GetYaxis()->UnZoom();
  }
  c200->Print("CocktailRatio200GeV.png");

  // ratios 19GeV
  TH1D* hR19[np];
  TCanvas* c19 = new TCanvas("c19", "c19", 0, 0, 707, 1000);
  c19->Divide(2,4);
  hR19[7] = new TH1D(Form("hR19[%d]", 7), "", 250, 0, 5);
  for ( int i = 0; i < np-1; ++i ) {
    c19->cd(i+1);
    //if ( i == 4 ) gPad->SetLogy();
    gPad->SetGrid();
    hR19[i] = (TH1D*)fMe19->Get(hnMe[i].Data());
    hR19[i]->Scale(hR19[i]->GetBinWidth(1));
    hR19[i]->Rebin(4);
    hR19[i]->Scale(1./hR19[i]->GetBinWidth(1));
    hR19[7]->Add(hR19[i]);
    TH1D* hb = (TH1D*)fBingchu->Get(hnBingchu[i].Data()); // already rebinned to 0.02
    TH1D* hbnew = new TH1D(Form("hbnew[%d]", i), "", 250, 0, 5);
    for (int b = 1; b <= hR19[i]->GetNbinsX(); ++b) {
      double bc = hb->GetBinContent(b);
      double be = hb->GetBinError(b);
      hbnew->SetBinContent(b, bc);
      hbnew->SetBinError(b, be);
    }
    hR19[i]->Divide(hbnew);
    hR19[i]->GetXaxis()->SetRangeUser(hrX1[i], hrX2[i]);
    hR19[i]->GetYaxis()->SetRangeUser(hrY1_19[i], hrY2_19[i]);
    hR19[i]->Draw();
    //hR19[i]->GetYaxis()->UnZoom();
  }
  c19->cd(8);
  gPad->SetGrid();
  TH1D* ht19new = new TH1D("ht19new", "", 250, 0, 5);
  for (int b = 1; b <= hR19[7]->GetNbinsX(); ++b) {
    double bc = ht19->GetBinContent(b);
    double be = ht19->GetBinError(b);
    ht19new->SetBinContent(b, bc);
    ht19new->SetBinError(b, be);
  }
  hR19[7]->Divide(ht19new);
  mycoll->SetHistoAtts(hR19[7], kRed, 1);
  hR19[7]->GetXaxis()->SetRangeUser(hrX1[7], hrX2[7]);
  hR19[7]->GetYaxis()->SetRangeUser(hrY1_19[7], hrY2_19[7]);
  hR19[7]->Draw();
  c19->Print("CocktailRatio19GeV.png");

}
