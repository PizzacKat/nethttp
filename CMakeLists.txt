cmake_minimum_required(VERSION 3.29)
project(nethttp)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
link_directories(lib)

add_executable(nethttp cmake-build-release/main.cpp
        include/nethttp/http_request_message.hpp
        include/nethttp/enums.hpp
        include/nethttp/http_headers.hpp
        include/nethttp/http_response_message.hpp
        include/nethttp/body.hpp
        include/nethttp/http_client.hpp
        include/nethttp/http_request.hpp
        include/nethttp/http_body.hpp
        include/nethttp/http_response.hpp
        include/nethttp/url.hpp
        include/nethttp/http_server.hpp)
target_link_libraries(nethttp netsock)