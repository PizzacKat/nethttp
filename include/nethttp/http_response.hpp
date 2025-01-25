#ifndef nethttp_http_response
#define nethttp_http_response

#include "http_body.hpp"
#include "http_response_message.hpp"

namespace nethttp {
    class http_response {
    public:
        http_response(http_response_message message);
        http_response();
        explicit http_response(std::int16_t status, std::string message = "OK");

        void set_version(std::uint16_t major, std::uint16_t minor);
        [[nodiscard]] nethttp::version version() const;

        void set_status(std::int16_t status);
        [[nodiscard]] std::int16_t status() const;

        void set_status_message(const std::string &message);
        [[nodiscard]] const std::string &status_message() const;

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

        [[nodiscard]] http_response_message &message();
        [[nodiscard]] const http_response_message &message() const;

    private:
        http_response_message _message;
    };
}

#endif //nethttp_http_response
