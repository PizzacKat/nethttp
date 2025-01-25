#ifndef nethttp_http_response_message
#define nethttp_http_response_message

#include "enums.hpp"
#include <cstdint>
#include <string>
#include <sstream>

#include "body.hpp"
#include "http_headers.hpp"

namespace nethttp {
    class http_response_message {
    public:
        explicit http_response_message(std::int16_t status = 200, std::string message = "OK");

        void set_status(std::int16_t status);
        [[nodiscard]] std::int16_t status() const;

        void set_message(std::string message);
        [[nodiscard]] const std::string &message() const;

        void set_version(std::uint16_t high, std::uint16_t low);
        [[nodiscard]] nethttp::version version() const;

        void set_headers(http_headers headers);
        [[nodiscard]] http_headers &headers();
        [[nodiscard]] const http_headers &headers() const;

        void set_body(std::string body);
        [[nodiscard]] std::string &raw_body();
        [[nodiscard]] const std::string &raw_body() const;
        [[nodiscard]] std::string body() const;

    private:
        nethttp::version _version{1, 1};
        std::int16_t _status;
        std::string _message;
        http_headers _headers;
        std::string _body;
    };

    std::ostream &to_stream(const http_response_message &message, std::ostream &os);
    std::string to_string(const http_response_message &message);
    std::istream &from_stream(http_response_message &response, std::istream &is);
    bool from_string(http_response_message &response, const std::string &string);
}

#endif //nethttp_http_response_message
