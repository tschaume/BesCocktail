cbes - simulate and analyze dielectron cocktails
================================================

general
-------

- download cocktail, contributions and input functions for all energies:
  http://downloads.the-huck.com/star/data/root/BesCocktails.tar.gz 
- more documentation (somewhat outdated), also has comparisons to 19/200 GeV:
  http://star.the-huck.com/models-simulations/cocktail-simulations/
- code: http://cgit.the-huck.com/cgit.cgi/BesCocktail/tree/ (start with programs/cbes.cc)
- configuration file: http://cgit.the-huck.com/cgit.cgi/BesCocktailDb/tree/db.yml


archive contents
----------------

- `rawhMee.root`: raw histograms filled from ntuple output of single mesons & ccbar simulations
   includes passCuts and branching ratio weight for ccbar, also for pt-differential
- `cocktail.root`: final scaled cocktails/contributions as compared to data
- `*_input.root`s contain functions (and acc. histograms) for the different input
  sources (implementation see
  http://cgit.the-huck.com/cgit.cgi/BesCocktail/tree/Functions.cxx):
  * fM: BreitWigner to sample invariant mass for two-body decays
  * fRes: MomentumResolution (MomRes) used in momentum smearing
  * fCB: double-sided CrystalBall used in momentum smearing
  * fKW: Kroll-Wada to sample invariant mass in dalitz decays
  * fPt: Tsallis-Blast-Wave dN/dpT such that Integral{dN/dpT dpT} = 1
    (Probability Distribution), resulting e+e- yield later scaled acc. to dN/dy in db.yml
- see http://cgit.the-huck.com/cgit.cgi/BesCocktail/tree/Analysis.cxx


install & usage
---------------

- install [boost](http://www.boost.org/)
- install [yaml-cpp](https://code.google.com/p/yaml-cpp/)
- install [ckon](http://tschaume.github.io/ckon/)
- get [BesCocktail](http://cgit.the-huck.com/cgit.cgi/BesCocktail/tree/)
- get [MyCollection](http://cgit.the-huck.com/cgit.cgi/MyCollection/tree/)
- put BesCocktail and MyCollection in StRoot/
- run `ckon setup` and adjust `ckon.cfg`
- run `ckon -j4` to compile `cbes`
- see `./build/bin/cbes -h` for help
- get an example [database](http://cgit.the-huck.com/cgit.cgi/BesCocktailDb/tree/db.yml) yaml file
- get pythia ccbar [simulation](http://www.star.bnl.gov/protected/lfspectra/huck/pythia/) input files
- simulate particles:
  `./build/bin/cbes -e 200 -v <particle>`
  `<particle>` = pion, eta, etap, rho, omega, phi, jpsi
- loop and generate output histos:
  `./build/bin/cbes -e 200 -l --pyfile <file> ana`
- use `--db` to set link to db.yml file
