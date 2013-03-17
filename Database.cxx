// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Database.h"
#include <fstream>

DatabaseManager::DatabaseManager(const string& d) : dbfile(d) {}

void DatabaseManager::writeDb() {
  // particles
  vector<string> prts;
  prts.push_back("pion");
  prts.push_back("eta");
  prts.push_back("omega");
  prts.push_back("phi");
  // init db
  Database DB;
  BOOST_FOREACH(string s, prts) {
    Particle mPrt;
    mPrt.rndInit();
    DB.mDb[s] = mPrt;
  }
  // encode node with DB
  YAML::Node node;
  node = DB;
  // emit yaml
  YAML::Emitter emYml;
  emYml << YAML::BeginMap;
  emYml << YAML::Key << "DB" << YAML::Value << node;
  emYml << YAML::EndMap;
  std::cout << emYml.c_str();
  // write to file
  std::ofstream outYml;
  outYml.open(dbfile.c_str());
  outYml << emYml.c_str();
  outYml.close();
  // load from file again
  YAML::Node nodeIn = YAML::LoadFile(dbfile);
  Database DBin = nodeIn["DB"].as<Database>();
  cout << endl;
  DBin.print();
}
