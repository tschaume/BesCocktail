// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Simulation.h"
#include "StRoot/BesCocktail/Utils.h"
#include <iostream>
#include <TMath.h>
#include <TFile.h>

using namespace std;

Simulation::Simulation(const string& p, const double& e)
: particle(p), energy(e)
{
  // random number generators:
  // parent-flatPt, parent-y, parent-phi [0, 1, 2]
  // ee: daughter-phi, daughter-cosT [3, 4]
  // dh: daughter-phi, daughter-cosT [5, 6]
  // ratio of branching ratios [7]
  for ( int i = 0; i < 8; ++i ) {
    rndm.push_back(new TRandom3());
    rndm.back()->SetSeed(0);
  }
  // database manager & variables
  dbm = DatabaseManager::Instance();
  mass = dbm->getProperty(particle, "mass");
  mode = (int)dbm->getProperty(particle, "decay");
  mass_dec = dbm->getDecayMass(particle);
  double mMax = dbm->getMaxMassBW(particle);
  mBR = dbm->getRatioBR(particle);  // dalitz br / total br
  sigma0 = dbm->getHdrVar("crystal")[1];
  // e+/e-/daughter-hadron lorentz vectors
  ep = new TLorentzVector();
  em = new TLorentzVector();
  dh = new TLorentzVector();  // denotes decay hadron OR photon
  dlp = new TLorentzVector();  // denotes decay lepton pair (virtual photon)
  // root output file for input functions and histograms
  const char* fn = Utils::getOutFileName(particle, energy, "_input");
  TFile* fin = TFile::Open(fn , "recreate");
  fin->cd(); TH1::AddDirectory(kFALSE);
  // init Functions
  Int_t npx = 10000;
  fp = new Functions(particle, energy);
  string suf = particle + Form("_%.1f", energy);
  fM = new TF1(Form("fM_%s", suf.c_str()),
      fp, &Functions::BreitWigner, Utils::mMin, mMax, 0);
  fM->SetNpx(npx); fM->SetTitle("BreitWigner"); fM->Write();
  hM = (TH1D*)fM->GetHistogram();
  hM->SetName(Form("hM_%s", suf.c_str())); hM->Write();
  cout << "saved fM & hM" << endl;
  fRes = new TF1(Form("fRes_%s", suf.c_str()),
      fp, &Functions::MomRes, Utils::ptMin, Utils::ptMax, 0);
  fRes->SetNpx(npx); fRes->SetTitle("MomRes"); fRes->Write();
  cout << "saved fRes" << endl;
  fCB = new TF1(Form("fCB_%s", suf.c_str()),
      fp, &Functions::CrystalBall2, -1., 1., 0);
  fCB->SetNpx(npx); fCB->SetTitle("CrystalBall2"); fCB->Write();
  hCB = (TH1D*)fCB->GetHistogram();
  hCB->SetName(Form("hCB_%s", suf.c_str())); hCB->Write();
  cout << "saved fCB & hCB" << endl;
  fKW = new TF1(Form("fKW_%s", suf.c_str()),
      fp, &Functions::KrollWada, Utils::mMin, mass-mass_dec, 0);
  fKW->SetNpx(npx); fKW->SetTitle("KrollWada"); fKW->Write();
  hKW = (TH1D*)fKW->GetHistogram();
  hKW->SetName(Form("hKW_%s", suf.c_str())); hKW->Write();
  cout << "saved fKW & hKW" << endl;
  //fRapJpsi = new TF1("fRapJpsi", "gaus", -1, 1);
  //fRapJpsi->SetNpx(10000);
  //fRapJpsi->SetParameters(1., 0., 1.1);
  //hRapJpsi = (TH1D*)fRapJpsi->GetHistogram();
  // init pT distribution
  //fPt = new TF1("fPt", fp, &Functions::MtScaling, Utils::ptMin, Utils::ptMax, 0);
  fPt = new TF1(Form("fPt_%s", suf.c_str()),
      fp, &Functions::Tsallis, Utils::ptMin, Utils::ptMax, 0);
  fPt->SetNpx(500); fPt->SetTitle("TsallisBlastWave"); fPt->Write();
  hPt = (TH1D*)fPt->GetHistogram();
  hPt->SetName(Form("hPt_%s", suf.c_str())); hPt->Write();
  cout << "saved fPt & hPt" << endl;
  fin->Close();
  cout << "fin closed" << endl;
#if 0
  if ( energy != 200 ) {
    hPt = (TH1D*)fPt->GetHistogram();
  } else {
    TFile* fYif = TFile::Open("root/TBWinput/mesons_baryons_noOmega_080.root", "read");
    map<string, string> mhYif;
    mhYif["pion"] = "hFit20";
    mhYif["eta"] = "hFit21";
    mhYif["etap"] = "hFit24";
    mhYif["omega"] = "hFit23";
    mhYif["phi"] = "hFit10";
    mhYif["jpsi"] = "hFit26";
    hPtInv = (TH1D*)fYif->Get(mhYif[particle].c_str());
    hPt = (TH1D*)hPtInv->Clone("hPt");
    for ( Int_t i = 1; i < hPtInv->GetNbinsX()+1; ++i ) {
      Double_t bc = hPtInv->GetBinContent(i);
      Double_t cx = hPtInv->GetBinCenter(i);
      hPt->SetBinContent(i, cx*bc*2.*TMath::Pi());
    }
    TFile* fYif = TFile::Open("root/AllInputPt_200GeV.root", "read");
    hPt = (TH1D*)fYif->Get(particle.c_str());
    std::cout << "hPt = " << hPt << std::endl;
  }
#endif
}

double Simulation::getEta(const double& pT) {
  double mT = sqrt(mass*mass+pT*pT);
  double y = rndm[1]->Uniform(-1.,1.);
  //if ( particle == "jpsi" ) y = hRapJpsi->GetRandom();
  return asinh(mT/pT*sinh(y));
}

void Simulation::sampleInput() {
  mPt = hPt->GetRandom(); // rndm[0]->Uniform(Utils::ptMin, Utils::ptMax);
  mEta = getEta(mPt);  // random based on uniform y [-1,1]
  mPhi = rndm[2]->Uniform(0., Utils::twoPi);  // random phi [0,2pi]
}

void Simulation::twoBodyKinematics(const double& p, double& pT, double& eta, double& phi) {
  double pz = p*rndm[3+2*isDhKin+1]->Uniform(-1., 1.);  // daughter pz, random cos(theta)
  pT = sqrt(p*p-pz*pz);  // daughter pT
  eta = 0.5*log((p+pz)/(p-pz)); // daughter eta
  phi = rndm[3+2*isDhKin]->Uniform(0., Utils::twoPi);  // random daughter phi
}

void Simulation::eeDecayVM(const double& mVM) {  // electrons in VM center of mass
  double p = sqrt(mVM*mVM/4.-Utils::emass2);  // electron momentum
  isDhKin = false;
  double pT, eta, phi;
  twoBodyKinematics(p, pT, eta, phi);
  ep->SetPtEtaPhiM(pT, eta, phi, Utils::emass);
  em->SetPtEtaPhiM(pT, -eta, phi+TMath::Pi(), Utils::emass);
}

void Simulation::hDecayVM(const double& mll) { // daughter hadron in parent VM center of mass
  double e = (mass*mass + mass_dec*mass_dec - mll*mll)/2./mass;  // daughter hadron energy
  double p = sqrt(e*e-mass_dec*mass_dec);  // daughter hadron momentum
  isDhKin = true;
  double pT, eta, phi;
  twoBodyKinematics(p, pT, eta, phi);
  dh->SetPtEtaPhiM(pT, eta, phi, mass_dec);
  dlp->SetPtEtaPhiM(pT, -eta, phi+TMath::Pi(), mll);
}

void Simulation::doTwoBodyDecay() {
  // electrons in VM center of mass
  double mBW = hM->GetRandom();  // Breit-Wigner mass
  vfill.push_back(mBW);
  eeDecayVM(mBW);  // ee decay the vector meson
  // boost to lab frame
  TLorentzVector parent;
  parent.SetPtEtaPhiM(mPt, mEta, mPhi, mBW);  // reset VM mass
  TVector3 bv = parent.BoostVector();
  ep->Boost(bv);
  em->Boost(bv);
}

void Simulation::doDalitzDecay() {
  vfill.push_back(mass);
  double mll = hKW->GetRandom();  // fKW range = allowed phase space
  eeDecayVM(mll);  // ee decay the virtual photon
  hDecayVM(mll);  // daughter hadron kinematics
  // boost e+/e- into lepton pair (virtual photon) frame (dlp)
  TVector3 bv1 = dlp->BoostVector();
  ep->Boost(bv1);
  em->Boost(bv1);
  // boost all decay products into parent lab frame
  TLorentzVector parent;
  parent.SetPtEtaPhiM(mPt, mEta, mPhi, mass);
  TVector3 bv2 = parent.BoostVector();
  ep->Boost(bv2);
  em->Boost(bv2);
  dh->Boost(bv2);
}

void Simulation::decay() { // decay mode = isTwoBody + 10 * isDalitz
  vfill.push_back(mPt);
  vfill.push_back(mEta);
  vfill.push_back(mPhi);
  switch (mode) {
  case 1:
    doTwoBodyDecay(); break;
  case 10:
    doDalitzDecay(); break;
  case 11:
    if ( rndm[7]->Rndm() < mBR ) doDalitzDecay();
    else doTwoBodyDecay();
  default: break;
  }
  pushNtVars();
}

void Simulation::applyMomSmear(TLorentzVector& l) {
  double ptrc = l.Pt();
  ptrc *= 1 + hCB->GetRandom() * fRes->Eval(l.Pt())/sigma0;
  l.SetPtEtaPhiM(ptrc, l.Eta(), l.Phi(), Utils::emass);
}

void Simulation::smear() {
  applyMomSmear(*ep);
  applyMomSmear(*em);
  if ( isDhKin ) applyMomSmear(*dh);
  pushNtVars();
}

void Simulation::pushNtVars() {
  vfill.push_back(ep->Pt());
  vfill.push_back(em->Pt());
  if ( isDhKin ) vfill.push_back(dh->Pt());
  else vfill.push_back(-999.);
  vfill.push_back(ep->Eta());
  vfill.push_back(em->Eta());
  vfill.push_back((*em+*ep).M());
  vfill.push_back((*em+*ep).Pt());
  vfill.push_back((*em+*ep).Rapidity());
}

Float_t* Simulation::getFillArray() {
  afill = new Float_t[vfill.size()];
  for ( unsigned int i = 0; i < vfill.size(); ++i ) { afill[i] = vfill[i]; }
  vfill.clear();
  return afill;
}
