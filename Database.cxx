// Copyright (c) 2013 Patrick Huck
#include "StRoot/BesCocktail/Database.h"
#include <fstream>

DatabaseManager* DatabaseManager::mDbm = NULL;

DatabaseManager::DatabaseManager(const string& d, bool bWrite)
: dbfile(d)
{
  if ( !bWrite ) {
    // load database (set mDB)
    YAML::Node nodeIn = YAML::LoadFile(dbfile);
    mDB = nodeIn["DB"].as<Database>();
  }
}

DatabaseManager* DatabaseManager::Instance(const string& d, bool bWrite) {
  if ( !mDbm ) mDbm = new DatabaseManager(d, bWrite);
  return mDbm;
}

double DatabaseManager::getDecayMass(const string& p) {
  if ( p == "pion" || p == "eta" ) return 0.;
  if ( p == "omega" ) return getMass("pion");
  if ( p == "phi" ) return getMass("eta");
  return -1.;
}

double DatabaseManager::getMaxMassBW(const string& p) {
  double arg = 1./getAlpha() - 1.;
  return 0.5*getWidth(p)*sqrt(arg) + getMass(p);
}

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
