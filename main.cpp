#include <iostream>
#include <optional>
#include <netsock/tcp_client.hpp>
#include "nethttp/http_request_message.hpp"

int main() {
    nethttp::http_request_message request;
    request.headers().add("Host", "www.example.com");
    request.headers().add("Accept", "*/*");
    request.set_body("Meow");
    auto str = to_string(request);
    for (std::size_t n = str.find('\r'); n != std::string::npos; n = str.find('\r'))
        str.erase(n, 1);
    std::cout << str;
    return 0;
}
