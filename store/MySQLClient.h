//
// Created by lml on 23-7-26.
//

#ifndef IMPORT_DEM_MYSQLCLIENT_H
#define IMPORT_DEM_MYSQLCLIENT_H
#include <mysql.h>
#include <string>
using std::string;

class MySQLClient {
public:
    static MySQLClient* getInstance();

    // return data's id, return -1 if failed
    int insertDataToFileStorage(string pieceId, int fileType, int timestamp = 1645671600);

    // return -1 if data don't exit
    int getDataIdFromFileStorage(string pieceId, int fileType, int timestamp = 1645671600);
private:
    MYSQL *con;
    const char *host = "localhost";
    const char *user = "root";
    const char *password = "1984";
    const char *database = "env_data";
    const int port = 3306;
    const string table = "file_storage";

    static MySQLClient* mySqlClient;

    ~MySQLClient();

    MySQLClient();

    MySQLClient(const MySQLClient&);

    MySQLClient& operator=(const MySQLClient&);

    void finish_with_error(MYSQL *con);
};


#endif //IMPORT_DEM_MYSQLCLIENT_H