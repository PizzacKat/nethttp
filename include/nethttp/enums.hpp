#ifndef nethttp_enums
#define nethttp_enums

#include <string>
#include <iosfwd>

namespace nethttp {
    typedef std::pair<std::uint16_t, std::uint16_t> version;

    enum http_method {
        GET = 0x01, HEAD = 0x02, POST = 0x04, PUT = 0x08, DELETE = 0x10, PATCH = 0x20, ANY = 0xFF
    };

    version make_version(std::uint16_t major, std::uint16_t minor);
    std::istream &from_stream(version &version, std::istream &is);
    std::ostream &to_stream(const version &version, std::ostream &os);
    std::string_view to_string(http_method method);
    bool from_string(http_method &method, const std::string_view &str);
}

#endif //nethttp_enums
