cmake_minimum_required(VERSION 3.25)

find_path(MYSQL_INCLUDE_DIR mysql.h /usr/include/mysql)
find_library(MYSQL_LIBRARY NAMES mysqlclient libmysqlclient rdmserver PATHS /lib /usr/lib /usr/local/lib)
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)
find_package(Poco REQUIRED Foundation Net Util)

# 检查是否找到
if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
    message(STATUS "Found MySQL")
else()
    message(FATAL_ERROR "Cannot find MySQL")
endif()

# 源文件列表
set(STORE_SRC
        MySQLClient.cpp
        MongoClient.cpp
        ImportData.cpp
        )

# 添加头文件路径
include_directories(${MYSQL_INCLUDE_DIR})
add_library(store ${STORE_SRC})

target_include_directories(store PUBLIC ${MYSQL_INCLUDE_DIR})
target_include_directories(store PUBLIC "/usr/local/include/mongocxx/v_noabi")
target_include_directories(store PUBLIC "/usr/local/include/bsoncxx/v_noabi")

# 设置 C++ 版本
target_compile_features(store PRIVATE cxx_std_17)

target_link_libraries(store PRIVATE ${MYSQL_LIBRARY})
target_link_libraries(store PRIVATE  mongo::mongocxx_shared)
target_link_libraries(store PRIVATE  mongo::bsoncxx_shared)
target_link_libraries(store PRIVATE Poco::Foundation Poco::Net Poco::Util)
