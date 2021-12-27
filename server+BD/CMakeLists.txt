cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 14)

project(boost.beast)

find_package(Boost)
include_directories(Boost::boost V2)



add_subdirectory(V2)

# add_executable(main src/main.c)
# target_link_libraries(main recsys recsys_multi)



include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


# Async HTTP Client
# add_executable(async.http.client  async.http.client.cpp)
# target_link_libraries(async.http.client pthread ${Boost_LIBRARIES})

#  Async HTTP Server
add_executable(async.http.server async.http.server.h async.http.server.cpp connection.h connection.cpp)
target_link_libraries(async.http.server  pthread Boost::boost nlohmann_json::nlohmann_json database)


# target_link_libraries(async.http.server PRIVATE nlohmann_json::nlohmann_json)
