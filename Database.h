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
  map<double, vector<double> > mhp; // energy, [A, a, b, p0, n]
  void rndInit() { // for testing purposes only
    m["mass"] = rand() % 10;
    for ( int i = 0; i < 3; ++i ) {
      vector<double> v;
      for ( int j = 0; j < 5; ++j ) v.push_back(rand()%1000);
      mhp[rand()%100+1] = v;
    }
  }
  void print() {
    cout << "mass: " << m["mass"] << endl;
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
      node.push_back(prt.mhp);
      return node;
    }
    static bool decode(const Node& node, Particle& prt) {
      if (!node.IsSequence()) return false;
      prt.m = node[0].as< map<string, double> >();
      prt.mhp = node[1].as< map< double, vector<double> > >();
      return true;
    }
  };
}

struct Database {
  map<string, Particle> mDb;
  void print() {
    BOOST_FOREACH(string s, mDb | ad::map_keys) {
      cout << s << endl; mDb[s].print();
    }
  }
};

namespace YAML {
  template<> struct convert<Database> {
    static Node encode(const Database& db) {
      Node node;
      node.push_back(db.mDb);
      return node;
    }
    static bool decode(const Node& node, Database& db) {
      if (!node.IsSequence()) return false;
      db.mDb = node[0].as< map<string, Particle> >();
      return true;
    }
  };
}

class DatabaseManager {
  private:
    string dbfile;

  public:
    DatabaseManager(const string&);
    virtual ~DatabaseManager() {}

    void writeDb();
};
#endif  // STROOT_BESCOCKTAIL_DATABASE_H_
