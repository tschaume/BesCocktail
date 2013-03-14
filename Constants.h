// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_CONSTANTS_H_
#define STROOT_BESCOCKTAIL_CONSTANTS_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::pair;
using std::cout;
using std::endl;
using std::vector;

class ParCont {
  public:
    ParCont() {}
    virtual ~ParCont() {}

    string name;
    double m;
    double A;
    double a;
    double b;
    double p0;
    double n;

    void set(
        const string& s, const double& a1,
        const double& a2, const double& a3, const double& a4,
        const double& a5, const double& a6)
    {
      name = s; m = a1;
      A = a2; a = a3; b = a4; p0 = a5; n = a6;
    }
    void print() { 
      cout << name << endl;
      cout << "   m : " << m << endl;
      cout << "   A : " << A << endl;
      cout << "   a : " << a << endl;
      cout << "   b : " << b << endl;
      cout << "   p0: " << p0 << endl;
      cout << "   n : " << n << endl;
    }
};

typedef pair<string, ParCont> pcpair;

class Constants {
  private:
    static map<string, ParCont> createParContMap() {
      map<string, ParCont> m;
      ParCont pc;
      pc.set("pion",0.1349766,496,0.525,0.151,0.738,8.25);
      m.insert(pcpair(pc.name, pc));
      pc.set("eta",0.547853,211.1,-7.54e-03,3.84e-02,2.53,8.13);
      m.insert(pcpair(pc.name, pc));
      pc.set("omega",0.78265,297.3,0.491,0.134,0.817,8.31);
      m.insert(pcpair(pc.name, pc));
      pc.set("phi",1.019455,185.2,0.315,0.197,0.78,8.28);
      m.insert(pcpair(pc.name, pc));
      pc.set("jpsi",3.096916,3.098,-3.8,1.5,0.97,8.32);
      m.insert(pcpair(pc.name, pc));
      return m;
    }

  public:
    Constants() {}
    virtual ~Constants() {}

    static map<string, ParCont> mParCont;
    static double mPtMin;  // 0.0
    static double mPtMax;  // 10.0
};
#endif  // STROOT_BESCOCKTAIL_CONSTANTS_H_
