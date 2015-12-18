cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(integrity)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Ofast -march=native -std=c++11")

set(INTEGRITYCTL_SOURCE integrityctl.cpp argument_parser.cpp argument_parser.h daemon.cpp daemon.h ipc.cpp ipc.h
        storage.cpp storage.h path_list_parser.cpp path_list_parser.h database.cpp database.h digest.cpp digest.h
        logging.cpp logging.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenSSL REQUIRED)
find_package(BerkeleyDB REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_executable(integrityctl ${INTEGRITYCTL_SOURCE})

include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${BERKELEY_DB_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(integrityctl ${Boost_LIBRARIES})
target_link_libraries(integrityctl ${BERKELEY_DB_LIBRARIES})
target_link_libraries(integrityctl ${OPENSSL_LIBRARIES})
