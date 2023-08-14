//
// Created by lml on 23-7-26.
//

#include "MySQLClient.h"

MySQLClient* MySQLClient::mySqlClient = nullptr;

MySQLClient* MySQLClient::getInstance() {
    if (mySqlClient == nullptr) {
        mySqlClient = new MySQLClient();
    }
    return mySqlClient;
}

MySQLClient::MySQLClient() {
    con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, host, user, password,
                           database, 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }
}

void MySQLClient::finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
//    exit(1);
}

MySQLClient::~MySQLClient() {
    mysql_close(con);
    delete mySqlClient;
}

int MySQLClient::insertDataToFileStorage(string pieceId, int fileType, int timestamp) {
    string sql = "insert into " + table + " values(NULL,'" + std::to_string(timestamp) + "','" + pieceId + "','" + std::to_string(fileType) + "'); ";
    if (mysql_query(con, sql.c_str())) {
        finish_with_error(con);
        return -1;
    }
    return mysql_insert_id(con);
}

int MySQLClient::getDataIdFromFileStorage(std::string pieceId, int fileType, int timestamp) {
    string sql = "select id from " + table + " where timestamp=" + std::to_string(timestamp)
                 + " AND piece_id='" + pieceId + "' AND file_type=" + std::to_string(fileType);
    if (mysql_query(con, sql.c_str()))
    {
        finish_with_error(con);
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
        return -1;
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    if (!row) {
        return -1;
    }
    int id = atoi(row[0]);
    mysql_free_result(result);
    return id;
}