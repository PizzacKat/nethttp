#ifndef nethttp_enums
#define nethttp_enums

namespace nethttp {
    enum http_method {
        GET = 0x01, HEAD = 0x02, POST = 0x04, PUT = 0x08, DELETE = 0x10, PATCH = 0x20
    };

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
}

#endif //nethttp_enums
