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
        explicit http_response_message(const std::int16_t status = 200, std::string message = "OK"): _status(status), _message(std::move(message)) {

        }

        void set_status(const std::int16_t status) {
            _status = status;
        }

        [[nodiscard]] std::int16_t status() const {
            return _status;
        }

        void set_message(std::string message) {
            _message = std::move(message);
        }

        [[nodiscard]] const std::string &message() const {
            return _message;
        }

        void set_version(const std::uint16_t high, const std::uint16_t low) {
            _version = make_version(high, low);
        }

        [[nodiscard]] nethttp::version version() const {
            return _version;
        }

        void set_headers(http_headers headers) {
            _headers = std::move(headers);
        }

        [[nodiscard]] http_headers &headers() {
            return _headers;
        }

        [[nodiscard]] const http_headers &headers() const {
            return _headers;
        }

        void set_body(std::string body) {
            _body = std::move(body);
        }

        [[nodiscard]] std::string &raw_body() {
            return _body;
        }

        [[nodiscard]] const std::string &raw_body() const {
            return _body;
        }

        [[nodiscard]] std::string body() const {
            return parse_body(_body, _headers);
        }
    private:
        nethttp::version _version{1, 1};
        std::int16_t _status;
        std::string _message;
        http_headers _headers;
        std::string _body;
    };

    inline std::ostream &to_stream(const http_response_message &message, std::ostream &os) {
        return to_stream(message.headers(), to_stream(message.version(), os) << ' ' << message.status() << ' ' << message.message() << "\r\n") << message.body();
    }

    inline std::string to_string(const http_response_message &message) {
        std::stringstream ss;
        to_stream(message, ss);
        return ss.str();
    }

    inline std::istream &from_stream(http_response_message &response, std::istream &is) {
        version version;
        if (!from_stream(version, is))
            return is;
        response.set_version(version.first, version.second);
        std::int16_t status;
        if (!(is >> status))
            return is;
        response.set_status(status);
        std::string message;
        if (!std::getline(is, message, '\r'))
            return is;
        if (is.get() != '\n') {
            is.setstate(std::ios::failbit);
            return is;
        }
        response.set_message(message);
        auto &headers = response.headers();
        if (!from_stream(headers, is))
            return is;
        std::string body;
        if (!read_body(body, headers, is))
            return is;
        response.set_body(body);
        return is;
    }

    inline bool from_string(http_response_message &response, const std::string &string) {
        std::stringstream ss(string);
        from_stream(response, ss);
        return ss.good();
    }
}

#endif //nethttp_http_response_message
