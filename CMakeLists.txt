cmake_minimum_required(VERSION 3.3)
project(integrity)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcrypto" )

set(SOURCE_FILES main.cpp)

find_package(OpenSSL)

add_executable(integrity ${SOURCE_FILES})