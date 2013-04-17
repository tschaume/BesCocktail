// Copyright (c) 2013 Patrick Huck
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <TF1.h>
#include <TNtuple.h>
#include <TFile.h>

#include "StRoot/BesCocktail/CmdLine.h"
#include "StRoot/BesCocktail/Functions.h"
#include "StRoot/BesCocktail/Utils.h"
#include "StRoot/BesCocktail/Simulation.h"
#include "StRoot/BesCocktail/Analysis.h"

int main(int argc, char **argv) {
  try {
    // parse command line options & arguments
    CmdLine* clopts = new CmdLine();
    if ( !clopts->parse(argc, argv) ) return 0;
    clopts->print();

    if ( clopts->particle != "ana" ) {  // simulation mode
      // init Simulation, inits functions and random generators, too
      Simulation* sim = new Simulation(clopts->particle, clopts->energy);  

      // init output
      const char* fn = Utils::getOutFileName(clopts->particle, clopts->energy);
      TFile* fout = new TFile(fn, "recreate");
      const char* nt_name = clopts->particle.c_str();
      const char* nt_vars =
        "ptVM:etaVM:phiVM:mVM:"
        "ptEp:ptEm:ptDh:etaEp:etaEm:eeMass:eePt:eeRap:"
        "ptEpR:ptEmR:ptDhR:etaEpR:etaEmR:eeMassR:eePtR:eeRapR";
      TNtuple* nt = new TNtuple(nt_name, nt_name, nt_vars, 0);

      // start loop
      for ( int n = 0; n < clopts->ndecays; ++n ) {
        Utils::printInfo(n);
        // init
        sim->sampleInput();
        // decay
        sim->decay();
        // smear decay particle momenta
        sim->smear();
        // fill output
        nt->Fill(sim->getFillArray());
      }

      // finish up
      nt->Write();
      fout->Close();
    }
    else {  // analysis mode
      Analysis* ana = new Analysis(clopts->energy, clopts->pyfile);
      if ( clopts->bLoop ) ana->loop();
      ana->genCocktail();
    }
  }
  catch(const std::exception& e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
