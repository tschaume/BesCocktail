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
  map<string, double> m; // mass in GeV/c2
  map<string, double> w; // width in GeV/c2
  map<double, vector<double> > mhp; // energy, [A, a, b, p0, n]
  bool operator==(const Particle& prt) const {
    //BOOST_FOREACH(string s, m | ad::map_keys) {
    //  if ( m[s] != prt.m[s] ) return false;
    //}
    //BOOST_FOREACH(double e, mhp | ad::map_keys) {
    //  for ( unsigned int i = 0; i < mhp[e].size(); ++i ) {
    //    if ( mhp[e].at(i) != prt.mhp[e].at(i) ) return false;
    //  }
    //}
    return true;
  }
  void rndInit() { // for testing purposes only
    m["mass"] = rand() % 10;
    w["width"] = rand() % 10;
    for ( int i = 0; i < 3; ++i ) {
      vector<double> v;
      for ( int j = 0; j < 5; ++j ) v.push_back(rand()%1000);
      mhp[rand()%100+1] = v;
    }
  }
  void print() {
    cout << "mass: " << m["mass"] << endl;
    cout << "width: " << w["width"] << endl;
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
      node.push_back(prt.m);
      node.push_back(prt.w);
      node.push_back(prt.mhp);
      return node;
    }
    static bool decode(const Node& node, Particle& prt) {
      if (!node.IsSequence()) return false;
      prt.m = node[0].as< map<string, double> >();
      prt.w = node[1].as< map<string, double> >();
      prt.mhp = node[2].as< map< double, vector<double> > >();
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
  bool operator==(const Database& db) const {
    //BOOST_FOREACH(string s, mDb | ad::map_keys) {
    //  if ( mDb[s] == db.mDb[s] ) continue;
    //  else return false;
    //}
    return true;
  }
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
    double getMass(const string& p) { return this->getDB().mDb[p].m["mass"]; }

    // test database
    void writeDb();
};
#endif  // STROOT_BESCOCKTAIL_DATABASE_H_
