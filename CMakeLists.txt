cmake_minimum_required(VERSION 3.3)
project(integrity)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcrypto -ldb_cxx" )

set(SOURCE_FILES main.cpp)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenSSL REQUIRED)
find_package(BerkeleyDB REQUIRED)

add_executable(integrity ${SOURCE_FILES})