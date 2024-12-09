#ifndef nethttp_enums
#define nethttp_enums

#include <pair>
#include <string>

namespace nethttp {
    typedef std::pair<std::uint16_t, std::uint16_t> version;

    enum http_method {
        GET = 0x01, HEAD = 0x02, POST = 0x04, PUT = 0x08, DELETE = 0x10, PATCH = 0x20, ANY = 0xFF
    };

    inline version make_version(std::uint16_t major, std::uint16_t minor) {
        return std::make_pair(major, minor);
    }

    inline std::istream &from_stream(version &version, std::istream &is) {
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
        if (!(is >> version.second))
            return is;
        return is;
    }

    inline std::ostream &to_stream(const version &version, std::ostream &os) {
        return os << "HTTP/" << version.first << "." << version.second;
    }

    inline std::string_view to_string(const http_method method) {
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
        }
        return "";
    }

    inline bool from_string(http_method &method, const std::string_view &str) {
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

#endif //nethttp_enums
