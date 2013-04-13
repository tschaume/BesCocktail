// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Simulation.h"
#include "StRoot/BesCocktail/Utils.h"
#include <iostream>
#include <TMath.h>
#include <TFile.h>

double Simulation::ptMin = 0.;
double Simulation::ptMax = 10.;

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
  mode = dbm->getProperty(particle, "decay");
  mass_dec = dbm->getDecayMass(particle);
  double mMax = dbm->getMaxMassBW(particle);
  mBR = dbm->getRatioBR(particle);  // dalitz br / total br
  // e+/e-/daughter-hadron lorentz vectors
  ep = new TLorentzVector();
  em = new TLorentzVector();
  dh = new TLorentzVector();  // denotes decay hadron OR photon
  dlp = new TLorentzVector();  // denotes decay lepton pair (virtual photon)
  // init Functions
  fp = new Functions(particle, energy);
  fPt = new TF1("fPt", fp, &Functions::MtScaling, ptMin, ptMax, 0);
  fPt->SetNpx(10000);
  fM = new TF1("fM", fp, &Functions::BreitWigner, Utils::mMin, mMax, 0);
  fM->SetNpx(10000);
  fRes = new TF1("fRes", fp, &Functions::MomRes, ptMin, ptMax, 0);
  fM->SetNpx(10000);
  fCB = new TF1("fCB", fp, &Functions::CrystalBall2, -1., 1., 0);
  fCB->SetNpx(10000);
  fKW = new TF1("fKW", fp, &Functions::KrollWada, Utils::mMin, mass-mass_dec, 0);
  fKW->SetNpx(10000);
}

double Simulation::getEta(const double& pT) {
  double mT = sqrt(mass*mass+pT*pT);
  double y = rndm[1]->Uniform(-1.,1.);
  return asinh(mT/pT*sinh(y));
}

void Simulation::sampleInput() {
  mPt = fPt->GetRandom(); // rndm[0]->Uniform(ptMin, ptMax);
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
  double mBW = fM->GetRandom();  // Breit-Wigner mass
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
  double mll = fKW->GetRandom();  // fKW range = allowed phase space
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
  ptrc *= 1 + fCB->GetRandom() * fRes->Eval(l.Pt())/0.01;
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
  vfill.push_back((*em+*ep).Rapidity());
}

Float_t* Simulation::getFillArray() {
  afill = new Float_t[vfill.size()];
  for ( unsigned int i = 0; i < vfill.size(); ++i ) { afill[i] = vfill[i]; }
  vfill.clear();
  return afill;
}
