// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_DATABASE_H_
#define STROOT_BESCOCKTAIL_DATABASE_H_

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

using std::string;
using std::vector;

struct HgdPars { 
  double A, a, b, p0, n; 
  HgdPars(double a1, double a2, double a3, double a4, double a5)
    : A(a1), a(a2), b(a3), p0(a4), n(a5) {}
  bool operator==(const HgdPars& hp) const {
    return A == hp.A && a == hp.a && b == hp.b && p0 == hp.p0 && n == hp.n;
  }
};

struct Particle {
  string name;
  double m;
  vector<HgdPars> hp; // for each energy
};

namespace YAML {
  template<>
    struct convert<HgdPars> {
      static Node encode(const HgdPars& hp) {
        Node node;
        node.push_back(hp.A);
        node.push_back(hp.a);
        node.push_back(hp.b);
        node.push_back(hp.p0);
        node.push_back(hp.n);
        return node;
      }

      static bool decode(const Node& node, HgdPars& hp) {
        if(!node.IsSequence() || node.size() != 5) return false;
        hp.A = node[0].as<double>();
        hp.a = node[1].as<double>();
        hp.b = node[2].as<double>();
        hp.p0 = node[3].as<double>();
        hp.n = node[4].as<double>();
        return true;
      }
    };
}

class Database {
  private:
    string dbfile;

  public:
    Database(const string&);
    virtual ~Database() {}

    void writeDb();
};
#endif  // STROOT_BESCOCKTAIL_DATABASE_H_
