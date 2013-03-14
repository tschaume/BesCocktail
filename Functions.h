// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_FUNCTIONS_H_
#define STROOT_BESCOCKTAIL_FUNCTIONS_H_

#include <string>

class Functions {
  private:
    std::string particle;

  public:
    Functions(const std::string&);
    virtual ~Functions() {}

    double Hagedorn(double*, double*);
};
#endif  // STROOT_BESCOCKTAIL_FUNCTIONS_H_
