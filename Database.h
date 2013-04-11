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
  StrVecTypeD mHgd;
  YldType mYld;
  bool operator==(const Database& db) const { return true; }
};

namespace YAML {
  template<> struct convert<Database> {
    static Node encode(const Database& db) {
      Node node;
      node.push_back(db.mHdr);
      node.push_back(db.mPrt);
      node.push_back(db.mHgd);
      node.push_back(db.mYld);
      return node;
    }
    static bool decode(const Node& node, Database& db) {
      if (!node.IsSequence()) return false;
      db.mHdr = node[0].as<StrVecTypeD>();  // Header
      db.mPrt = node[1].as<PrtType>();  // Particle Properties
      db.mHgd = node[2].as<StrVecTypeD>();  // Hagedorn
      db.mYld = node[3].as<YldType>();  // Yields
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

  public:
    static DatabaseManager* Instance(const string&, bool bWrite = false);
    static DatabaseManager* Instance() { return mDbm; }
    virtual ~DatabaseManager() {}

    Database& getDB() { return mDB; }
    bool checkParticle(const string& p) const { return mDB.mPrt.count(p); }
    bool checkEnergy(const double& e) { return mDB.mYld.count(e); }
    void print();

    double getProperty(const string& p, const string& pr) { return mDB.mPrt[p][pr]; }
    vector<double> getHdrVar(const string& var) { return mDB.mHdr[var]; }
    vector<double> getHgd(const string& p, const double& e) {
      string key = p+"_";
      key += boost::lexical_cast<string>(e);
      return mDB.mHgd[key];
    }

    double getAlpha() { return getHdrVar("alpha").at(0); }
    double getDecayMass(const string&);
    double getMaxMassBW(const string&);
    double getRatioBR(const string&);
    double getSumBR(const string&);

};
#endif  // STROOT_BESCOCKTAIL_DATABASE_H_
