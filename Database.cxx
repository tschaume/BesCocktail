// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Database.h"
#include <iostream>

Database::Database(const string& d) : dbfile(d) {}

void Database::writeDb() {
  YAML::Node node;
  node["test"] = HgdPars(496.,0.525,0.151,0.738,8.25);
  std::cout << node << std::endl;
}
