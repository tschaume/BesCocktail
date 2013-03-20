// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Utils.h"
#include <iostream>
#include <TMath.h>

double Utils::emass = 0.510998928e-3;

Utils::Utils(const string& particle) : phiMax(2.*TMath::Pi()) {
  yGen = new TRandom3();
  yGen->SetSeed(0);
  phiGen = new TRandom3();
  phiGen->SetSeed(0);
  dbm = DatabaseManager::Instance();
  mass = dbm->getMass(particle);
  lvIn = new TLorentzVector();
  ep = new TLorentzVector();
  em = new TLorentzVector();
}

void Utils::printInfo(const int& n) {
  if ( n > 0 && !(n%100000) ) std::cout << n/1000 << "k" << std::endl;
  else if ( !(n%10000) ) std::cout << "." << std::flush;
}

const char* Utils::getOutFileName(const std::string& p) {
  return (p+".root").c_str();
}

double Utils::getPhi() { return phiGen->Uniform(0.,phiMax); }

double Utils::getY() { return yGen->Uniform(-1.,1.); }

double Utils::getEta(const double& pT) {
  double mT = sqrt(mass*mass+pT*pT);
  return asinh(mT/pT*sinh(getY()));
}

void Utils::setLvIn(const double& pt, const double& eta,
    const double& phi, const double& m) {
  lvIn->SetPtEtaPhiM(pt, eta, phi, m);
}

void Utils::doTwoBodyDecay() {
  // electron in VM center of mass
  double mVM = lvIn->M();
  double pCM = sqrt(mVM*mVM/4.-emass*emass);  // electron momentum
  double phiCM = getPhi();  // random electron phi
  double pzCM = pCM*getY();  // electron pz, based on random cos(theta)
  double pTCM = sqrt(pCM*pCM-pzCM*pzCM);  // electron pT
  double etaCM = 0.5*log((pCM+pzCM)/(pCM-pzCM)); // electron eta
  // e+/e- four-vectors in CM of VM
  ep->SetPtEtaPhiM(pTCM,etaCM,phiCM,emass);
  em->SetPtEtaPhiM(pTCM,-etaCM,phiCM+TMath::Pi(),emass);
  // boost to lab frame
  TVector3 bv = lvIn->BoostVector();
  ep->Boost(bv);
  em->Boost(bv);
}
