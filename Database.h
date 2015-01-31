// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_DATABASE_H_
#define STROOT_BESCOCKTAIL_DATABASE_H_

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <yaml-cpp/yaml.h>
#include <boost/lexical_cast.hpp>

using std::string;
using std::vector;
using std::map;

typedef map< string, vector<double> > StrVecTypeD;
typedef map< string, double > StrNumTypeD;
typedef map< double, StrNumTypeD > YldType;
typedef map< string, StrNumTypeD > PrtType;

struct Database {
  StrVecTypeD mHdr;
  PrtType mPrt;
  PrtType mPrtTsa;
  StrVecTypeD mHgd;
  YldType mTsa;
  YldType mYld;
  PrtType mRto;
  YldType mCcb;
  StrVecTypeD mMRg;
  bool operator==(const Database& db) const { return true; }
};

namespace YAML {
  template<> struct convert<Database> {
    static Node encode(const Database& db) {
      Node node;
      node.push_back(db.mHdr);
      node.push_back(db.mPrt);
      node.push_back(db.mPrtTsa);
      node.push_back(db.mHgd);
      node.push_back(db.mTsa);
      node.push_back(db.mYld);
      node.push_back(db.mRto);
      node.push_back(db.mCcb);
      node.push_back(db.mMRg);
      return node;
    }
    static bool decode(const Node& node, Database& db) {
      if (!node.IsSequence()) return false;
      db.mHdr = node[0].as<StrVecTypeD>();  // Header
      db.mPrt = node[1].as<PrtType>();  // Particle Properties
      db.mPrtTsa = node[2].as<PrtType>();  // Particle Properties for Tsallis Fits
      db.mHgd = node[3].as<StrVecTypeD>();  // Hagedorn
      db.mTsa = node[4].as<YldType>();  // Tsallis
      db.mYld = node[5].as<YldType>();  // Yields
      db.mRto = node[6].as<PrtType>();  // meson to pi ratios
      db.mCcb = node[7].as<YldType>();  // values for ccbar
      db.mMRg = node[8].as<StrVecTypeD>();  // mee ranges for pt spectra
      return true;
    }
  };
}

class DatabaseManager {
  private:
    string dbfile;
    DatabaseManager(const string&, bool bWrite = false);
    static DatabaseManager* mDbm;
    Database mDB;
    map<int, double> mMapCcbr;  // map of ccbar branching ratios
    void initCcbrMap();

  public:
    static DatabaseManager* Instance(const string& = "", bool bWrite = false);
    virtual ~DatabaseManager() {}

    Database& getDB() { return mDB; }
    bool checkParticle(const string& p) const { return mDB.mPrt.count(p); }
    bool checkEnergy(const double& e) { return mDB.mYld.count(e); }
    void print();

    double getProperty(const string& p, const string& pr) {
      if ( checkParticle(p) ) { return mDB.mPrt[p][pr]; }
      return mDB.mPrtTsa[p][pr];
    }
    vector<double> getHdrVar(const string& var) { return mDB.mHdr[var]; }
    vector<double> getMRngLimits(const string& var) { return mDB.mMRg[var]; }
    vector<double> getHgd(const string& p, const double& e) {
      string key = p+"_";
      key += boost::lexical_cast<string>(e);
      if ( mDB.mHgd.count(key) ) return mDB.mHgd[key];
      else return mDB.mHgd["pion_200"];
    }

    double getAlpha() { return getHdrVar("alpha").at(0); }
    double getDecayMass(const string&);
    double getMaxMassBW(const string&);
    double getRatioBR(const string&);
    double getSumBR(const string&);
    double getdNdy(const string& p, const double& e) {
      if ( mDB.mYld[e].count(p) > 0 ) { return mDB.mYld[e][p]; }
      return mDB.mYld[e]["pion"] * mDB.mRto["m2pi"][p];
    }
    double getPyBrWeight(const int& i) { return mMapCcbr[i]; }
    double getPyBrWeight2(const int&, const int&);
    double getNcoll(const double& e) { return mDB.mCcb[e]["Ncoll"]; }
    double getCcbarXRatio(const double& e) {
      return mDB.mCcb[e]["ccX"]/mDB.mCcb[e]["totX"];
    }
    double getCcbarXRelErr(const double& e) {
      return mDB.mCcb[e]["ccXe"]/mDB.mCcb[e]["ccX"];
    }
    double getTsallisPar(const double& e, const string& s) { return mDB.mTsa[e][s]; }
    int getNrMRnges() { return (int)mDB.mMRg.size(); }

};
#endif  // STROOT_BESCOCKTAIL_DATABASE_H_
