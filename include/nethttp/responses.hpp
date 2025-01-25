#ifndef nethttp_responses
#define nethttp_responses
#include "http_response.hpp"

namespace nethttp::responses {
    inline http_response ok() {
        return http_response(200, "OK");
    }

    inline http_response created() {
        return http_response(201, "Created");
    }

    inline http_response accepted() {
        return http_response(202, "Accepted");
    }

    inline http_response no_content() {
        return http_response(203, "No Content");
    }

    inline http_response redirect(const std::string &path) {
        http_response response(301, "Moved Permanently");
        response.add_header("Location", path);
        return response;
    }

    inline http_response found() {
        return http_response(302, "Found");
    }

    inline http_response bad_request() {
        return http_response(400, "Bad Request");
    }

    inline http_response forbidden() {
        return http_response(403, "Forbidden");
    }

    inline http_response not_found() {
        return http_response(404, "Not Found");
    }

    inline http_response internal_error(const std::string &message = "Internal Server Error") {
        return http_response(500, message);
    }
}

#endif //nethttp_responses
