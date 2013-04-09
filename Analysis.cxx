// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Analysis.h"
#include <iostream>

Analysis::Analysis(const double& e) : energy(e) {
  // database manager & variables
  dbm = DatabaseManager::Instance();
}
