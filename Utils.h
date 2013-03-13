// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_UTILS_H_
#define STROOT_BESCOCKTAIL_UTILS_H_

#include <string>

class Utils {
  private:

  public:
    Utils() {}
    virtual ~Utils() {}

    static void printInfo(int);
    static bool checkParticle(const std::string&);
};
#endif  // STROOT_BESCOCKTAIL_UTILS_H_
