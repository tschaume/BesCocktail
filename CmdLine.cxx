// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/CmdLine.h"
#include <iostream>

using std::cout;
using std::endl;

CmdLine::CmdLine()
: particle(""), ndecays(100000), dbfile("db.yml"), energy(-1.)
{ }

bool CmdLine::parse(int argc, char *argv[]) {
  po::options_description generic("Generic Options");
  generic.add_options()
    ("help,h", po::bool_switch(&bHelp), "show this help")
    ("verbose,v", po::bool_switch(&bVerbose), "verbose output")
    ("particle", po::value<std::string>(&particle), "simulate particle")
    ("ndecays,n", po::value<int>(&ndecays), "# decays")
    ("energy,e", po::value<double>(&energy), "energy")
    ("loop,l", po::bool_switch(&bLoop), "loop ntuple in ana mode")
    ("db", po::value<std::string>(&dbfile), "input database file");

  po::options_description userOpts;
  userOpts.add(generic);

  po::positional_options_description posOpts;
  posOpts.add("particle", 1);

  po::variables_map vm;

  try {
    // parse command line
    // don't allow unregistered on command line
    po::parsed_options cl_parsed =
      po::command_line_parser(argc, argv).options(userOpts)
      .positional(posOpts).run();
    po::store(cl_parsed, vm);

    po::notify(vm);  // throws on error

    if ( bHelp ) {  // --help option
      cout << "cbes -- simulate cocktail for dielectron analyses" << endl;
      cout << userOpts << endl;
      return false;
    }

    // get DatabaseManager (loads database)
    dbm = DatabaseManager::Instance(dbfile);

    // check particle
    if ( particle != "ana" ) {
      if ( particle.empty() || !dbm->checkParticle(particle) ) {
        cout << "particle " << particle << " not in database" << endl;
        return false;
      }
      // check energy
      if ( !dbm->checkEnergy(energy) ) {
        cout << "energy " << energy << " not in database" << endl;
        return false;
      }
    }
    else {
      cout << "analysis mode" << endl;
    }

    return true;
  }
  catch(const po::error& e) {
    std::cerr << "ERROR: " << e.what() << endl << endl << generic << endl;
    return false;
  }
}

void CmdLine::print() {
  cout << endl;
  cout << "particle  : " << particle << endl;
  cout << "#decays   : " << ndecays << endl;
  if ( bVerbose ) dbm->print();
}
