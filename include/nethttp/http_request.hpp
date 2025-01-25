#ifndef nethttp_http_request
#define nethttp_http_request

#include "http_body.hpp"
#include "http_request_message.hpp"

namespace nethttp {
    class http_request {
    public:
        http_request(http_request_message message);
        http_request();
        explicit http_request(http_method method);

        void set_method(http_method method);
        [[nodiscard]] http_method method() const;

        void set_path(const std::string &path);
        [[nodiscard]] const std::string &path() const;

        void set_version(std::uint16_t major, std::uint16_t minor);
        [[nodiscard]] nethttp::version version() const;

        void set_headers(const http_headers &headers);
        void add_header(const std::string &name, const http_header_values &value);
        [[nodiscard]] bool has_header(const std::string &name) const;
        [[nodiscard]] http_header_values &get_header(const std::string &name);
        [[nodiscard]] const http_header_values &get_header(const std::string &name) const;

        [[nodiscard]] http_headers &headers();
        [[nodiscard]] const http_headers &headers() const;

        void set_body(const std::string &body);
        void set_body(const http_body &body);
        [[nodiscard]] std::string body() const;
        [[nodiscard]] const std::string &raw_body() const;

        [[nodiscard]] http_request_message &message();
        [[nodiscard]] const http_request_message &message() const;

        [[nodiscard]] const std::string &wildcard(std::size_t i = 0) const;

    private:
        friend class http_server;
        http_request_message _message;
        std::vector<std::string> _wildcards;
    };
}

#endif //nethttp_http_request
