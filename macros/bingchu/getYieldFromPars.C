
void getYieldFromPars(){
  double beta = 0.3974;
  double Tfo = 0.1191;
  double n = 1.0132;

  double m0 = 0.1349766;
  double pTmin = 0.;
  double pTmax = 10.;
  int nPtBins = 1000;
  TH1D *hFit = new TH1D("hPi","pi",nPtBins,pTmin,pTmax);
  for(int i=0;i<nPtBins;++i){
    double pT = hFit->GetBinCenter(i+1);
    double y = getYield(beta,Tfo,n,pT,m0);
    if ( !(i%100) ) cout<<"pT:"<<pT<<endl;
    hFit->SetBinContent(i+1,pT*y);
    hFit->SetBinError(i+1,0.);
  }
  TFile *fout = new TFile("TBW_Pi_Bingchu.root","recreate");
  hFit->Write();
  fout->Close();
}

double getYield(double beta,double T,double kappa,double pT,double m0){
  double betaS=(1+1./(1+1))*beta;
  double SumB=-1e6;
  if(betaS>=1.)return SumB;
  double mT=sqrt(pT*pT+m0*m0);
  SumB=0;
  TF1 *TsallisCosInt = new TF1("TsallisCosInt","[1]*TMath::CosH([5])*TMath::Power(1+([1]*TMath::CosH([2])*TMath::CosH([5])-[0]*TMath::SinH([2])*TMath::Cos(x))*([3]-1.0)/[4],-1.0/([3]-1.0))",-TMath::Pi(),TMath::Pi()); // SetParameters(pt,m0,rho,q,T,rapidity)
  for (int i=0;i<10;i++){
    Double_t rho = TMath::ATanH(betaS*(i*0.1+0.05)); //rho Integral is rdr = (i*0.1+0.05)*0.1
    //cout<<"rho= "<<rho<<endl;
    for(int j=0;j<48;j++){
      Double_t rapidity = (j-24.0)/4.0;//rapidity integral from	[-6,6] dy=0.25
      TsallisCosInt->SetParameters(pT,mT,rho,kappa,T,rapidity);
      SumB+=((i*0.01+0.005)*0.125*TsallisCosInt->Integral(-TMath::Pi(),TMath::Pi()));
      //rapidity *=-1;//rapidity integral from [-6,6] dy=0.5
      //TsallisCosInt->SetParameters(pT,mT,rho,kappa,T,rapidity);
      //SumB+=((i*0.01+0.0005)*0.125*TsallisCosInt->Integral(-TMath::Pi(),TMath::Pi()));
    }
  }
  return SumB;
}
