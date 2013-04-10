// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Database.h"
#include <fstream>

DatabaseManager* DatabaseManager::mDbm = NULL;

DatabaseManager::DatabaseManager(const string& d, bool bWrite)
: dbfile(d)
{
  if ( !bWrite ) {
    // load database (set mDB)
    YAML::Node nodeIn = YAML::LoadFile(dbfile);
    mDB = nodeIn["DB"].as<Database>();
  }
}

DatabaseManager* DatabaseManager::Instance(const string& d, bool bWrite) {
  if ( !mDbm ) mDbm = new DatabaseManager(d, bWrite);
  return mDbm;
}

double DatabaseManager::getDecayMass(const string& p) {
  if ( p == "pion" || p == "eta" ) return 0.;
  if ( p == "omega" ) return getMass("pion");
  if ( p == "phi" ) return getMass("eta");
  return -1.;
}

double DatabaseManager::getMaxMassBW(const string& p) {
  double arg = 1./getAlpha() - 1.;
  return 0.5*getWidth(p)*sqrt(arg) + getMass(p);
}

double DatabaseManager::getRatioBR(const string& p) { // dalitz / (ee + dalitz)
  double br_ee = getDB().mDb[p].mbr["br"].at(0);
  double br_da = getDB().mDb[p].mbr["br"].at(1);
  return br_da/(br_ee+br_da);
}

double DatabaseManager::getSumBR(const string& p) { // dalitz / (ee + dalitz)
  double br_ee = getDB().mDb[p].mbr["br"].at(0);
  double br_da = getDB().mDb[p].mbr["br"].at(1);
  return br_ee+br_da;
}
