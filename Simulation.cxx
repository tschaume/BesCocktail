// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Simulation.h"
#include "StRoot/BesCocktail/Utils.h"
#include <iostream>
#include <TMath.h>

double Simulation::ptMin = 0.;
double Simulation::ptMax = 10.;
double Simulation::mMin = 2*Utils::emass;
double Simulation::mMax = 4.;

Simulation::Simulation(const string& p, const double& e)
: particle(p), energy(e)
{
  for ( int i = 0; i < 4; ++i ) {  // y, phi, phi, cosT
    rndm.push_back(new TRandom3());
    rndm.back()->SetSeed(0);
  }
  dbm = DatabaseManager::Instance();
  mass = dbm->getMass(particle);
  mode = dbm->getDecayMode(particle);
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
  fKW = new TF1("fKW", fp, &Functions::KrollWada, mMin, mMax, 0);
  fKW->SetNpx(10000);
}

double Simulation::getEta(const double& pT) {
  double mT = sqrt(mass*mass+pT*pT);
  double y = rndm[0]->Uniform(-1.,1.);
  return asinh(mT/pT*sinh(y));
}

void Simulation::sampleInput() {
  double pt = fPt->GetRandom();
  vfill.push_back(pt);
  double eta = getEta(pt);  // random based on uniform y [-1,1]
  vfill.push_back(eta);
  double phi = rndm[1]->Uniform(0.,Utils::twoPi);  // random phi [0,2pi]
  vfill.push_back(phi);
  double m = fM->GetRandom();
  vfill.push_back(m);
  lvIn->SetPtEtaPhiM(pt, eta, phi, m);
}

void Simulation::setEeVmCm(const double& mVM) {  // electrons in VM center of mass
  double p = sqrt(mVM*mVM/4.-Utils::emass2);  // electron momentum
  double phi = rndm[2]->Uniform(0.,Utils::twoPi);  // random electron phi
  double pz = p*rndm[3]->Uniform(-1.,1.);  // electron pz, random cos(theta)
  double pT = sqrt(p*p-pz*pz);  // electron pT
  double eta = 0.5*log((p+pz)/(p-pz)); // electron eta
  ep->SetPtEtaPhiM(pT, eta, phi, Utils::emass);
  em->SetPtEtaPhiM(pT, -eta, phi+TMath::Pi(), Utils::emass);
}

void Simulation::doTwoBodyDecay() {
  // electrons in VM center of mass
  setEeVmCm(lvIn->M());
  // boost to lab frame
  TVector3 bv = lvIn->BoostVector();
  ep->Boost(bv);
  em->Boost(bv);
  vfill.push_back(ep->Pt());
  vfill.push_back(em->Pt());
}

void Simulation::doDalitzDecay() {
  setEeVmCm(lvIn->M());  // TODO: check input invariant mass
}

void Simulation::decay() {
  if ( mode == 1 ) return doTwoBodyDecay();
  if ( mode == 10 ) return doDalitzDecay();
  if ( mode == 11 ) return; // TODO: implement both decay's case
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
