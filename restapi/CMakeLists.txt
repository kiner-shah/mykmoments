cmake_minimum_required(VERSION 3.15)
project(MyKMomentsRestAPI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

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

# TODO: add sources

add_executable(MyKMomentsRestAPI src/main.cpp)
target_link_libraries(MyKMomentsRestAPI PRIVATE ZLIB::ZLIB OpenSSL::SSL OpenSSL::Crypto Crow::Crow pqxx)