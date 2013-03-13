// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_CONSTANTS_H_
#define STROOT_BESCOCKTAIL_CONSTANTS_H_

#include <string>
#include <map>

class Constants {
  private:
    static std::map<std::string, double> createMassMap() {
      std::map<std::string, double> m;
      m["pion"] = 0.1349766;
      m["eta"] = 0.547853;
      m["omega"] = 0.78265;
      m["phi"] = 1.019455;
      m["jpsi"] = 3.096916;
      return m;
    }

  public:
    Constants() {}
    virtual ~Constants() {}

    static std::map<std::string, double> mMass;

};
#endif  // STROOT_BESCOCKTAIL_CONSTANTS_H_
