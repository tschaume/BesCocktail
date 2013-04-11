// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_UTILS_H_
#define STROOT_BESCOCKTAIL_UTILS_H_

#include <string>
#include <map>
#include <Rtypes.h>

class Utils {
  private:
    static std::map<std::string, int> createColorMap() {
      std::map<std::string, int> m;
      m["pion"] = kYellow+2;
      m["eta"] = kMagenta;
      m["omega"] = kGreen+3;
      m["phi"] = kCyan-3;
      m["jpsi"] = kMagenta+2;
      m["ccbar"] = kOrange+7;
      return m;
    }

  public:
    Utils();
    virtual ~Utils() {}

    static double emass;
    static double emass2;
    static double twoPi;
    static double threePi;
    static double alpha;
    static std::map<std::string, int> mColorMap;

    static void printInfo(const int&, const int& div = 100);
    static const char* getOutFileName(const std::string&, const double&);
};
#endif  // STROOT_BESCOCKTAIL_UTILS_H_
