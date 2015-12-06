cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(integrity)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

set(INTEGRITYCTL_SOURCE integrityctl.cpp argument_parser.h argument_parser.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#find_package(OpenSSL REQUIRED)
#find_package(BerkeleyDB REQUIRED)
#find_package(Boost COMPONENTS system filesystem REQUIRED)

add_executable(integrityctl ${INTEGRITYCTL_SOURCE})

#include_directories(${OPENSSL_INCLUDE_DIR})
#include_directories(${BERKELEY_DB_INCLUDE_DIR})
#include_directories(${Boost_INCLUDE_DIRS})

#target_link_libraries(integrityctl ${Boost_LIBRARIES})
