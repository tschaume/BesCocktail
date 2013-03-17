// Copyright (c) 2013 Patrick Huck
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include "StRoot/BesCocktail/Database.h"

int main(int argc, char **argv) {
  try {
    DatabaseManager* dbm = new DatabaseManager("test.yml");
    dbm->writeDb();
  }
  catch(const std::exception& e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
