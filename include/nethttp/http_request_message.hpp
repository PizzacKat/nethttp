#ifndef nethttp_http_request
#define nethttp_http_request
#include "http_headers.hpp"
#include "enums.hpp"
#include "netsock/tcp_client.hpp"

namespace nethttp {
    class http_request_message {
    public:
        explicit http_request_message(const http_method method = GET): _method(method) {

        }

        void set_method(const http_method method) {
            _method = method;
        }

        [[nodiscard]] http_method method() const {
            return _method;
        }

        void set_path(const std::string &path) {
            _path = path;
        }

        [[nodiscard]] const std::string &path() const {
            return _path;
        }

        void set_version(const std::uint16_t high, const std::uint16_t low) {
            _version[0] = high;
            _version[1] = low;
        }

        [[nodiscard]] std::pair<std::uint16_t, std::uint16_t> version() const {
            return {_version[0], _version[1]};
        }

        void set_headers(const http_headers &headers) {
            _headers = headers;
        }

        [[nodiscard]] http_headers &headers() {
            return _headers;
        }

        [[nodiscard]] const http_headers &headers() const {
            return _headers;
        }

        void set_body(const std::string &body) {
            _body = body;
        }

        [[nodiscard]] const std::string &body() const {
            return _body;
        }
    private:
        http_method _method;
        std::string _path{"/"};
        std::uint16_t _version[2]{1, 1};
        http_headers _headers;
        std::string _body;
    };

    inline std::string to_string(const http_request_message &message) {
        std::string res(to_string(message.method()));
        res += " ";
        res += message.path();
        res += " HTTP/";
        res += std::to_string(message.version().first);
        res += ".";
        res += std::to_string(message.version().second);
        res += "\r\n";
        for (const auto &[name, values] : message.headers()) {
            res += name;
            res += ": ";
            for (std::size_t i = 0; i < values.count(); i++) {
                res += values.get(i);
                if (i != values.count() - 1)
                    res += ", ";
            }
            res += "\r\n";
        }
        res += "\r\n";
        res += message.body();
        return res;
    }
}

#endif //nethttp_http_request
