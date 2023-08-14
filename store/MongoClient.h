//
// Created by lml on 23-7-27.
//

#ifndef IMPORT_DEM_MONGOCLIENT_H
#define IMPORT_DEM_MONGOCLIENT_H
#include <string>
#include <vector>
#include <fstream>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/exception.hpp>
#include <Poco/Base64Encoder.h>
#include <sstream>
#include "Dem.h"
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;
using std::string;
using std::vector;

class MongoClient {
public:
    static MongoClient* getInstance();

    bool insertDemFile(int id, string fileName);

    void insertCraters(int id, vector<Store::Crater> craters);

    Store::DemData getDemDataById(int id);

private:
    mongocxx::instance instance{};
    mongocxx::client client;
    string mongouri = "mongodb://localhost:27017";
    string databaseName = "env_db";
    string collectionName = "col_terrain";
    mongocxx::database db;
    mongocxx::collection collection;

    static MongoClient* mongoClient;

    MongoClient();

    ~MongoClient();

    MongoClient(const MongoClient&);

    MongoClient& operator=(const MongoClient&);
};


#endif //IMPORT_DEM_MONGOCLIENT_H