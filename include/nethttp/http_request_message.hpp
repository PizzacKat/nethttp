#ifndef nethttp_http_request_message
#define nethttp_http_request_message
#include "http_headers.hpp"
#include "enums.hpp"

namespace nethttp {
    class http_request_message {
    public:
        explicit http_request_message(http_method method = GET);

        void set_method(http_method method);
        [[nodiscard]] http_method method() const;

        void set_path(const std::string &path);
        [[nodiscard]] const std::string &path() const;

        void set_version(std::uint16_t high, std::uint16_t low);
        [[nodiscard]] nethttp::version version() const;

        void set_headers(const http_headers &headers);
        [[nodiscard]] http_headers &headers();
        [[nodiscard]] const http_headers &headers() const;

        void set_body(const std::string &body);
        [[nodiscard]] std::string body() const;
        [[nodiscard]] std::string &raw_body();
        [[nodiscard]] const std::string &raw_body() const;

    private:
        http_method _method;
        std::string _path{"/"};
        nethttp::version _version{1, 1};
        http_headers _headers;
        std::string _body;
    };

    std::ostream &to_stream(const http_request_message &message, std::ostream &os);
    std::string to_string(const http_request_message &message);
    std::istream &from_stream(http_request_message &request, std::istream &is);
    bool from_string(http_request_message &request, const std::string &string);
}

#endif //nethttp_http_request_message
