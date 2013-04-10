// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_DATABASE_H_
#define STROOT_BESCOCKTAIL_DATABASE_H_

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <yaml-cpp/yaml.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;

namespace ad = boost::adaptors;

struct Particle {
  map<string, double> mpr;  // properties
  // mass (GeV/c2), width (GeV/c2), final state factor, Lambda^(-2), decay mode, BR's
  // mass, width, fsfac, l2, decay, br_ee, br_da
  map<double, vector<double> > mhp; // energy, [A, a, b, p0, n]
  bool operator==(const Particle& prt) const { return true; }
  void print() {
    BOOST_FOREACH(string k, mpr | ad::map_keys) {
      cout << k << ": " << mpr[k] << endl;
    }
    BOOST_FOREACH(double e, mhp | ad::map_keys) {
      cout << e << ": [";
      BOOST_FOREACH(double p, mhp[e]) { cout << " " << p; }
      cout << " ]" << endl;
    }
  }
};

namespace YAML {
  template<> struct convert<Particle> {
    static Node encode(const Particle& prt) {
      Node node;
      node.push_back(prt.mpr);
      node.push_back(prt.mhp);
      return node;
    }
    static bool decode(const Node& node, Particle& prt) {
      if (!node.IsSequence()) return false;
      prt.mpr = node[0].as< map<string, double> >();
      prt.mhp = node[1].as< map<double, vector<double> > >();
      return true;
    }
  };
}

struct DbHeader {
  map<string, vector<double> > mPars;  // general container for parameters
  bool operator==(const DbHeader& dbh) const { return true; }
  void print() {
    BOOST_FOREACH(string s, mPars | ad::map_keys) {
      BOOST_FOREACH(double p, mPars[s]) { cout << p << " " << std::flush; }
      cout << endl;
    }
  }
};

namespace YAML {
  template<> struct convert<DbHeader> {
    static Node encode(const DbHeader& dbh) {
      Node node;
      node.push_back(dbh.mPars);
      return node;
    }
    static bool decode(const Node& node, DbHeader& dbh) {
      if (!node.IsSequence()) return false;
      dbh.mPars = node[0].as< map<string, vector<double> > >();
      return true;
    }
  };
}

struct Database {
  map<string, DbHeader > mHdr;
  map<string, Particle> mDb;
  bool operator==(const Database& db) const { return true; }
  void print() {
    BOOST_FOREACH(string s, mDb | ad::map_keys) {
      cout << s << endl; mDb[s].print();
    }
    BOOST_FOREACH(string s, mHdr | ad::map_keys) {
      cout << s << endl; mHdr[s].print();
    }
  }
};

namespace YAML {
  template<> struct convert<Database> {
    static Node encode(const Database& db) {
      Node node;
      node.push_back(db.mHdr);
      node.push_back(db.mDb);
      return node;
    }
    static bool decode(const Node& node, Database& db) {
      if (!node.IsSequence()) return false;
      db.mHdr = node[0].as< map<string, DbHeader> >();
      db.mDb = node[1].as< map<string, Particle> >();
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
    bool checkParticle(const string& p) const { return mDB.mDb.count(p); }
    bool checkEnergy(const string& p, const double& e) {
      return mDB.mDb[p].mhp.count(e);
    }
    void print() { mDB.print(); }

    double getProperty(const string& p, const string& pr)  {
      return getDB().mDb[p].mpr[pr];
    }

    double getAlpha() { return getDB().mHdr["header"].mPars["alpha"].at(0); }
    double getDecayMass(const string&);
    double getMaxMassBW(const string&);
    double getRatioBR(const string&);
    double getSumBR(const string&);

};
#endif  // STROOT_BESCOCKTAIL_DATABASE_H_
