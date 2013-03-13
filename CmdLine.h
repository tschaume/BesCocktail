// Copyright (c) 2013 Patrick Huck
#ifndef STROOT_BESCOCKTAIL_CMDLINE_H_
#define STROOT_BESCOCKTAIL_CMDLINE_H_

#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;

class CmdLine {
  private:

  public:
    CmdLine();
    virtual ~CmdLine() {}

    bool bHelp;
    bool bVerbose;
    std::string particle;
    int ndecays;

    bool parse(int argc, char *argv[]);
    void print();
};
#endif  // STROOT_BESCOCKTAIL_CMDLINE_H_
