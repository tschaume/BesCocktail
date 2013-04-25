// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Database.h"
#include <fstream>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>

namespace ad = boost::adaptors;
using std::cout;
using std::endl;

DatabaseManager* DatabaseManager::mDbm = NULL;

DatabaseManager::DatabaseManager(const string& d, bool bWrite)
: dbfile(d)
{
  if ( !bWrite ) {
    // load database (set mDB)
    YAML::Node nodeIn = YAML::LoadFile(dbfile);
    mDB = nodeIn.as<Database>();
    initCcbrMap();
  }
}

DatabaseManager* DatabaseManager::Instance(const string& d, bool bWrite) {
  if ( !mDbm ) mDbm = new DatabaseManager(d, bWrite);
  return mDbm;
}

void DatabaseManager::initCcbrMap() {
  int id[4] = { 411, 421, 431, 4122 };
  for ( int i = 0; i < 4; ++i ) {
    mMapCcbr[id[i]] = getHdrVar("ccbr").at(i);
  }
}

double DatabaseManager::getDecayMass(const string& p) {
  if ( p == "omega" ) return getProperty("pion", "mass");
  if ( p == "phi" ) return getProperty("eta", "mass");
  return 0.;
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

double DatabaseManager::getPyBrWeight2(const int& p, const int& n) {
  return getPyBrWeight(p) * getPyBrWeight(n);
}

void DatabaseManager::print() {
  cout << "=== Header ===" << endl;
  BOOST_FOREACH(string s, mDB.mHdr | ad::map_keys) {
    cout << "  " << s << ": ";
    BOOST_FOREACH(double p, mDB.mHdr[s]) { cout << p << " " << std::flush; }
    cout << endl;
  }
  cout << "=== Particles ===" << endl;
  BOOST_FOREACH(string s, mDB.mPrt | ad::map_keys) {
    cout << s << endl;
    BOOST_FOREACH(string k, mDB.mPrt[s] | ad::map_keys) {
      cout << "  " << k << ": " << mDB.mPrt[s][k] << endl;
    }
  }
  cout << "=== Hagedorn Parameters ===" << endl;
  BOOST_FOREACH(string k, mDB.mHgd | ad::map_keys) {
    cout << "  " << k << ": ";
    BOOST_FOREACH(double n, mDB.mHgd[k]) { cout << n << " "; }
    cout << endl;
  }
  cout << "=== Tsallis Parameters ===" << endl;
  BOOST_FOREACH(double e, mDB.mTsa | ad::map_keys) {
    cout << "  " << e << ": ";
    BOOST_FOREACH(string s, mDB.mTsa[e] | ad::map_keys) {
      cout << s << "=" << mDB.mTsa[e][s] << " ";
    }
    cout << endl;
  }
  cout << "=== Invariant Yields ===" << endl;
  BOOST_FOREACH(double e, mDB.mYld | ad::map_keys) {
    cout << "  " << e << ": ";
    BOOST_FOREACH(string p, mDB.mYld[e] | ad::map_keys) {
      cout << p << "=" << mDB.mYld[e][p] << " ";
    }
    cout << endl;
  }
}
