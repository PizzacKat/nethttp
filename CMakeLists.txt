cmake_minimum_required(VERSION 3.29)
project(nethttp)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
link_directories(lib)

add_executable(nethttp main.cpp
        include/nethttp/http_request.hpp
        include/nethttp/enums.hpp
        include/nethttp/http_headers.hpp)
target_link_libraries(nethttp netsock)