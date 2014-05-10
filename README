cbes - simulate and analyze dielectron cocktails
================================================

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
- simulate particles: `./build/bin/cbes -e 200 -v <particle>`. `<particle>` = pion, eta, omega, phi, jpsi
- loop and generate output histos: `./build/bin/cbes -e 200 -l --pyfile <file> ana`
