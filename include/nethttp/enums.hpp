#ifndef nethttp_enums
#define nethttp_enums

namespace nethttp {
    enum http_method {
        GET = 0x01, HEAD = 0x02, POST = 0x04, PUT = 0x08, DELETE = 0x10, PATCH = 0x20
    };
}

#endif //nethttp_enums
