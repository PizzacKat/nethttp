#ifndef nethttp_body
#define nethttp_body

#include <string>

#include "http_headers.hpp"

namespace nethttp {
    std::string parse_body(const std::string &body, const http_headers &headers);
    std::istream &read_body(std::string &body, const http_headers &headers, std::istream &is);
}

#endif //nethttp_body
