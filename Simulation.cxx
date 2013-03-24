// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Simulation.h"
#include "StRoot/BesCocktail/Utils.h"
#include <iostream>
#include <TMath.h>

double Simulation::ptMin = 0.;
double Simulation::ptMax = 10.;
double Simulation::mMin = 2*Utils::emass;
double Simulation::mMax = 2.;

Simulation::Simulation(const string& p, const double& e)
: particle(p), energy(e)
{
  yGen = new TRandom3();
  yGen->SetSeed(0);
  phiGen = new TRandom3();
  phiGen->SetSeed(0);
  dbm = DatabaseManager::Instance();
  mass = dbm->getMass(particle);
  lvIn = new TLorentzVector();
  ep = new TLorentzVector();
  em = new TLorentzVector();
  // init Functions
  fp = new Functions(particle, energy);
  fPt = new TF1("fPt", fp, &Functions::MtScaling, ptMin, ptMax, 0);
  fPt->SetNpx(10000);
  fM = new TF1("fM", fp, &Functions::BreitWigner, mMin, mMax, 0);
  fM->SetNpx(10000);
  fRes = new TF1("fRes", fp, &Functions::MomRes, ptMin, ptMax, 0);
  fM->SetNpx(10000);
  fCB = new TF1("fCB", fp, &Functions::CrystalBall2, -1., 1., 0);
  fCB->SetNpx(10000);
}

double Simulation::getPhi() { return phiGen->Uniform(0.,Utils::twoPi); }

double Simulation::getY() { return yGen->Uniform(-1.,1.); }

double Simulation::getEta(const double& pT) {
  double mT = sqrt(mass*mass+pT*pT);
  return asinh(mT/pT*sinh(getY()));
}

void Simulation::sampleInput() {
  double pt = fPt->GetRandom();
  vfill.push_back(pt);
  double eta = getEta(pt);  // random based on uniform y [-1,1]
  vfill.push_back(eta);
  double phi = getPhi();  // random phi [0,2pi]
  vfill.push_back(phi);
  double m = fM->GetRandom();
  vfill.push_back(m);
  lvIn->SetPtEtaPhiM(pt, eta, phi, m);
}

void Simulation::doTwoBodyDecay() {
  // electron in VM center of mass
  double mVM = lvIn->M();
  double pCM = sqrt(mVM*mVM/4.-Utils::emass2);  // electron momentum
  double phiCM = getPhi();  // random electron phi
  double pzCM = pCM*getY();  // electron pz, based on random cos(theta)
  double pTCM = sqrt(pCM*pCM-pzCM*pzCM);  // electron pT
  double etaCM = 0.5*log((pCM+pzCM)/(pCM-pzCM)); // electron eta
  // e+/e- four-vectors in CM of VM
  ep->SetPtEtaPhiM(pTCM,etaCM,phiCM,Utils::emass);
  em->SetPtEtaPhiM(pTCM,-etaCM,phiCM+TMath::Pi(),Utils::emass);
  // boost to lab frame
  TVector3 bv = lvIn->BoostVector();
  ep->Boost(bv);
  em->Boost(bv);
  vfill.push_back(ep->Pt());
  vfill.push_back(em->Pt());
}

void Simulation::applyMomSmear(TLorentzVector& l) {
  double ptrc = l.Pt();
  ptrc *= 1 + fCB->GetRandom() * fRes->Eval(l.Pt())/0.01;
  l.SetPtEtaPhiM(ptrc, l.Eta(), l.Phi(), Utils::emass);
}

void Simulation::smear() {
  applyMomSmear(*ep);
  vfill.push_back(ep->Pt());
  applyMomSmear(*em);
  vfill.push_back(em->Pt());
}

Float_t* Simulation::getFillArray() {
  afill = new Float_t[vfill.size()];
  for ( unsigned int i = 0; i < vfill.size(); ++i ) { afill[i] = vfill[i]; }
  vfill.clear();
  return afill;
}
