cmake_minimum_required(VERSION 3.15)
project(MyKMomentsRestAPI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/installdir)

# for SSL/TLS connections
find_package(OpenSSL REQUIRED)
if (NOT OPENSSL_FOUND)
    message(FATAL_ERROR "OpenSSL not found")
endif()
# for compressed response
find_package(ZLIB REQUIRED)
if (NOT ZLIB_FOUND)
    message(FATAL_ERROR "Zlib not found")
endif()

# C++ web server
set(ASIO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/asio-1.28.0/include CACHE PATH "Path to ASIO include directory")
set(CROW_FEATURES ssl compression)
set(CROW_BUILD_EXAMPLES OFF)
set(CROW_BUILD_TESTS OFF)
add_subdirectory(external/Crow)

# C++ wrapper for PostgreSQL C library (libpq)
set(SKIP_BUILD_TEST ON)
add_subdirectory(external/libpqxx)

# Json Web Tokens
# set(JWT_BUILD_EXAMPLES OFF)
# add_subdirectory(external/jwt-cpp)
include(ExternalProject)
ExternalProject_Add(jwt-cpp
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/jwt-cpp
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_ARGS -DSKIP_BUILD_TEST=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
)

ExternalProject_Get_property(jwt-cpp INSTALL_DIR)

# TODO: add sources

add_executable(MyKMomentsRestAPI src/Error.cpp src/db_utils.cpp src/main.cpp)
add_dependencies(MyKMomentsRestAPI jwt-cpp)

# for jwt-cpp headers
target_include_directories(MyKMomentsRestAPI PRIVATE ${CMAKE_INSTALL_PREFIX}/include)

target_link_libraries(MyKMomentsRestAPI PRIVATE ZLIB::ZLIB OpenSSL::SSL OpenSSL::Crypto Crow::Crow pqxx)