cmake_minimum_required(VERSION 3.12)
project(nethttp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs/netsock)

set(src src/body.cpp
        src/enums.cpp
        src/http_body.cpp
        src/http_client.cpp
        src/http_headers.cpp
        src/http_request.cpp
        src/http_request_message.cpp
        src/http_response.cpp
        src/http_response_message.cpp
        src/http_server.cpp
)

set(CMAKE_DEBUG_POSTFIX  "-d")

add_library(nethttp ${src})
target_link_libraries(nethttp netsock)
target_include_directories(nethttp PUBLIC include)