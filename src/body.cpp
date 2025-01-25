#include "nethttp/body.hpp"
#include <iostream>
#include <sstream>

namespace nethttp {
    std::string parse_body(const std::string &body, const http_headers &headers) {
        if (headers.has("Transfer-Encoding") && headers.get("Transfer-Encoding").contains("chunked")) {
            std::string parsed_body;
            std::size_t idx = body.find("\r\n"), end = 0;
            while (idx != std::string::npos) {
                const std::size_t length = std::stoul(body.substr(end, idx - end), nullptr, 16);
                if (length == 0)
                    break;
                end = idx + 2;
                parsed_body += body.substr(end, length);
                end = end + length + 2;
                idx = body.find("\r\n", end);
            }
            return parsed_body;
        }
        return body;
    }

    std::istream &read_body(std::string &body, const http_headers &headers, std::istream &is) {
        if (headers.has("Content-Length")) {
            body.resize(std::stoul(headers.get("Content-Length").get()));
            is.read(body.data(), static_cast<std::streamsize>(body.size()));
        }
        if (headers.has("Transfer-Encoding") && headers.get("Transfer-Encoding").contains("chunked")) {
            std::size_t length;
            while (is) {
                is >> std::hex;
                is >> length;
                is >> std::dec;
                if (!is || is.get() != '\r' || is.get() != '\n') {
                    is.setstate(std::ios::failbit);
                    return is;
                }
                body += (std::stringstream() << std::hex << length << "\r\n").str();
                const auto offset = body.size();
                if (length != 0) {
                    body.resize(offset + length);
                    is.read(body.data() + offset,  static_cast<std::streamsize>(length));
                }
                body += "\r\n";
                if (!is || is.get() != '\r' || is.get() != '\n') {
                    is.setstate(std::ios::failbit);
                    return is;
                }
                if (length == 0)
                    break;
            }
        }
        return is;
    }
}
