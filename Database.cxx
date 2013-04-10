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
  if ( p == "omega" ) return getProperty("pion", "mass");
  if ( p == "phi" ) return getProperty("eta", "mass");
  return -1.;
}

double DatabaseManager::getMaxMassBW(const string& p) {
  double arg = 1./getAlpha() - 1.;
  double w = getProperty(p, "width");
  double m = getProperty(p, "mass");
  return 0.5*w*sqrt(arg) + m;
}

double DatabaseManager::getRatioBR(const string& p) { // dalitz / (ee + dalitz)
  double br_ee = getProperty(p, "br_ee");
  double br_da = getProperty(p, "br_da");
  return br_da/(br_ee+br_da);
}

double DatabaseManager::getSumBR(const string& p) { // ee + dalitz
  double br_ee = getProperty(p, "br_ee");
  double br_da = getProperty(p, "br_da");
  return br_ee+br_da;
}
