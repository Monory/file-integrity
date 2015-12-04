cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(integrity)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

#set(SOURCE_FILES digest.cpp digest.h database.cpp database.h storage.cpp storage.h argument_parser.cpp
#        argument_parser.h config_parser.cpp config_parser.h config_unit.cpp config_unit.h db_record.cpp db_record.h
#        daemon.cpp)

set(DAEMON_SOURCE daemon.cpp digest.cpp digest.h database.cpp database.h 
    storage.cpp storage.h argument_parser.cpp argument_parser.h 
    config_parser.cpp config_parser.h config_unit.cpp config_unit.h 
    db_record.cpp db_record.h ipc.cpp ipc.h)

set(CLIENT_SOURCE client.cpp ipc.cpp ipc.h argument_parser.cpp 
    argument_parser.h config_parser.cpp config_parser.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenSSL REQUIRED)
find_package(BerkeleyDB REQUIRED)
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)

add_executable(daemon ${DAEMON_SOURCE})
add_executable(client ${CLIENT_SOURCE})

include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${BERKELEY_DB_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(client ${Boost_LIBRARIES})

target_link_libraries(daemon ${Boost_LIBRARIES})
target_link_libraries(daemon ${BERKELEY_DB_LIBRARIES})
target_link_libraries(daemon ${OPENSSL_LIBRARIES})




#add_executable(integrity ${SOURCE_FILES})

#include_directories(${OPENSSL_INCLUDE_DIR})
#target_link_libraries(integrity ${OPENSSL_LIBRARIES})

#include_directories(${BERKELEY_DB_INCLUDE_DIR})
#target_link_libraries(integrity ${BERKELEY_DB_LIBRARIES})

#include_directories(${Boost_INCLUDE_DIRS})
#target_link_libraries(integrity ${Boost_LIBRARIES})
