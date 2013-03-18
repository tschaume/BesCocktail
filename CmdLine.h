// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_CMDLINE_H_
#define STROOT_BESCOCKTAIL_CMDLINE_H_

#include <boost/program_options.hpp>
#include <string>
#include "StRoot/BesCocktail/Database.h"

namespace po = boost::program_options;

class CmdLine {
  private:
    DatabaseManager* dbm;

  public:
    CmdLine();
    virtual ~CmdLine() {}

    bool bHelp;
    bool bVerbose;
    std::string particle;
    int ndecays;
    std::string dbfile;
    double energy;

    bool parse(int argc, char *argv[]);
    void print();
};
#endif  // STROOT_BESCOCKTAIL_CMDLINE_H_
