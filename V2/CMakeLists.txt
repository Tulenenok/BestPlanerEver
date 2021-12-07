cmake_minimum_required(VERSION 3.21)
project(DB)

set(CMAKE_CXX_STANDARD 20)
include_directories(/usr/include/mysql/mysql.h)
add_library(database SHARED DB.cpp)
add_executable(DB DB.cpp )
target_link_libraries(DB mysqlclient)
