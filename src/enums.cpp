#include "nethttp/enums.hpp"

#include <iostream>

namespace nethttp {
    version make_version(std::uint16_t major, std::uint16_t minor) {
        return std::make_pair(major, minor);
    }

    std::istream & from_stream(version &version, std::istream &is) {
        version = make_version(0, 0);
        std::string http(4, ' ');
        is.get(http.data(), 5);
        if (http != "HTTP") {
            is.setstate(std::ios::failbit);
            return is;
        }
        if (is.get() != '/') {
            is.setstate(std::ios::failbit);
            return is;
        }
        if (!(is >> version.first))
            return is;
        if (is.peek() != '.')
            return is;
        is.ignore();
        if (!(is >> version.second))
            return is;
        return is;
    }

    std::ostream & to_stream(const version &version, std::ostream &os) {
        return os << "HTTP/" << version.first << "." << version.second;
    }

    std::string_view to_string(const http_method method) {
        switch (method) {
            case GET:
                return "GET";
            case HEAD:
                return "HEAD";
            case POST:
                return "POST";
            case PUT:
                return "PUT";
            case DELETE:
                return "DELETE";
            case PATCH:
                return "PATCH";
            default:
                return "";
        }
    }

    bool from_string(http_method &method, const std::string_view &str) {
        if (str == "GET") {
            method = GET;
            return true;
        }
        if (str == "HEAD") {
            method = HEAD;
            return true;
        }
        if (str == "POST") {
            method = POST;
            return true;
        }
        if (str == "PUT") {
            method = PUT;
            return true;
        }
        if (str == "DELETE") {
            method = DELETE;
            return true;
        }
        if (str == "PATCH") {
            method = PATCH;
            return true;
        }
        return false;
    }
}
