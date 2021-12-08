cmake_minimum_required(VERSION 3.16)


set(CMAKE_CXX_STANDARD 14)
add_library(database SHARED DB.cpp)
target_link_libraries(database PRIVATE /usr/lib/x86_64-linux-gnu/libmysqlclient.so)
target_include_directories(database PUBLIC /usr/include/mysql)
# target_link_libraries(DB mysqlclient)
