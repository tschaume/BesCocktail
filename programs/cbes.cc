// Copyright (c) 2013 Patrick Huck
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include "StRoot/BesCocktail/CmdLine.h"
#include "StRoot/BesCocktail/Utils.h"

int main(int argc, char **argv) {
  try {
    // parse command line options & arguments
    CmdLine* clopts = new CmdLine();
    if ( !clopts->parse(argc, argv) ) return 0;
    clopts->print();

    // start work
    for ( int n = 0; n < clopts->ndecays; ++n ) {
      Utils::printInfo(n);
      //Double_t pt = fPt->GetRandom(ptmin,ptmax);
    }
  }
  catch(const std::exception& e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
